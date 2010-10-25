/*******************************************************************

 Copyright(c) 2000-2005 

 PurpleACE Wireless Solutions Pvt. Ltd.
 www.purpleace.com
 #708, A Wing, Carlton Towers, Airport Road, Bangalore - 560017,
 India
 All rights reserved.

 This software is the confidential and proprietary information of
 PurpleACE Wireless Solutions Pvt. Ltd. ("Confidential Information").
 You shall not disclose such Confidential Information and
 shall use it only in accordance with the terms of the license
 Agreement you entered into with PurpleACE Wireless Solutions Pvt. Ltd..

 Project Name               : Ripple Vault Symbian Client
 Module Name				: PhotosEngine.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :   Photos Engine
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#ifndef __UIQ__
#include <AknGlobalListQuery.h>
#include <aknlists.h> 
#include <aknpopup.h>
#include <aknmessagequerydialog.h>
#endif

#ifdef __UIQ__
#include <eiktxlbx.h>
#include "PhotoTimeOutTimer.h"
#endif

#include <bautils.h>
#include <eikgted.h>
#include "PhotosEngine.h"
//#include "Sockets.pan"
#include "RippleVaultAppUi.h"
#include <eikenv.h>


#ifdef __UIQ__
#ifdef __S80__
	#include <RippleVault_series80.rsg>	
	#include "RippleVault_series80.hrh"
#else
	#include <RippleVault_uiq.rsg>	
	#include "RippleVault_uiq.hrh"
#endif
#else
#include <RippleVault.rsg>
#include "RippleVault.hrh"
#endif
#include "socketsenginewriter.h"
class CSocketsEngineWriter;
class CRippleVaultAppUi;

#include <etel.h>

//#include <AgentClient.h>
//#include <cdbcols.h>
//#include <commdb.h>


#ifdef __UIQ__
/*	_LIT(KDirImageFolder1,"c:\\documents\\media files\\image\\unfiled\\*");
	_LIT(KDirImageFolder2,"d:\\media files\\image\\unfiled\\*");
	_LIT(KDir1,"c:\\documents\\media files\\image\\unfiled");
	_LIT(KDir2,"d:\\media files\\image\\unfiled");

	_LIT(KDirImageFolder3,"c:\\documents\\media files\\image\\Sony Ericsson\\*");
	_LIT(KDirImageFolder4,"d:\\media files\\image\\Sony Ericsson\\*");
	_LIT(KDir3,"c:\\documents\\media files\\image\\Sony Ericsson");
	_LIT(KDir4,"d:\\media files\\image\\Sony Ericsson");
*/
	const TInt CPhotosEngine::KTimeOut = 2000000; //1 seconds time-out
#else 
/*	_LIT(KDirImageFolder1,"c:\\Nokia\\Images\\*");
	_LIT(KDirImageFolder2,"e:\\Images\\*");
	_LIT(KDir1,"c:\\Nokia\\Images");
	_LIT(KDir2,"e:\\Images");*/
#endif


CPhotosEngine* CPhotosEngine::NewL(CRippleVaultAppUi& aAppUi, TDesC& aFolderName)
    {
	CPhotosEngine* self = CPhotosEngine::NewLC(aAppUi, aFolderName);
	CleanupStack::Pop(self);
	return self;
    }

	
CPhotosEngine* CPhotosEngine::NewLC(CRippleVaultAppUi& aAppUi, TDesC& aFolderName)
    {
	CPhotosEngine* self = new (ELeave) CPhotosEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL(aFolderName);
	return self;
    }


CPhotosEngine::CPhotosEngine(CRippleVaultAppUi& aAppUi)
: CActive(EPriorityStandard),
  iAppUi(aAppUi)
    {
    	iPublic_Photo=EFalse;
    }


CPhotosEngine::~CPhotosEngine()
{
#ifdef __UIQ__
     delete iTimer;
#endif
    iRFsession.NotifyChangeCancel(iStatus);

    Cancel();	
	iRFsession.Close();
}


void CPhotosEngine::ConstructL(TDesC& aFolderName)
{
	User::LeaveIfError(iRFsession.Connect());    
	//Anupam
	icompareflag = 0;
	icompareflag = aFolderName.Compare(KDirPhonePhoto);
#ifdef __UIQ__
	if(icompareflag == 0)
	{
		icompareflag = 1;
	}
	else
	{
		icompareflag = aFolderName.Compare(KDirMMCPhoto);
		if(icompareflag == 0)
		{
			icompareflag = 2;
		}
		else
		{
			icompareflag = aFolderName.Compare(KDirPhonePhotoSony);
			if(icompareflag == 0)
			{
				icompareflag = 3;
			}
			else
			{
				icompareflag = aFolderName.Compare(KDirMMCPhotoSony);
				if(icompareflag == 0)
				{
					icompareflag = 4;
				}
				else
				{
					icompareflag = 0;
				}
			}
		}
	}
#else
	if(icompareflag == 0)
	{
		icompareflag = 1;
	}
	else
	{
		icompareflag = aFolderName.Compare(KDirMMCPhoto);
		if(icompareflag == 0)
		{
			icompareflag = 2;
		}
		else
		{
			icompareflag = 0;
		}
	}
#endif

#ifdef __UIQ__
		//GetTimeStamp();
		iTimer = CPhotoTimeoutTimer::NewL(EPriorityHigh, *this);
		//iTimer = CPhotoTimeoutTimer::NewL(EPriorityIdle, *this);

		//CActiveScheduler::Add(this); 
		//iTimer->After(KTimeOut);
#endif

	if (BaflUtils::FileExists(iRFsession,aFolderName))
	{
		GetImageCount();

		//iRFsession.NotifyChange(ENotifyWrite,iStatus,aFolderName);
		iRFsession.NotifyChange(ENotifyFile,iStatus,aFolderName);
#ifdef __UIQ__
		ChangeStatus(EOpenMenu);	
#else
		ChangeStatus(ESmartCharge);
#endif
		CActiveScheduler::Add(this); 

		SetActive();
	}
}

// from CActive
void CPhotosEngine::DoCancel()
    {
#ifdef __UIQ__
		iTimer->Cancel();
#endif
    }


// from CActive

#ifdef __UIQ__
void CPhotosEngine::RunL()
{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("\nCPhotosEngine::RunL\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	iCurrentTimeStamp.HomeTime();

	if (iEngineStatus == EOpenMenu)
	{
		iTimer->After(KTimeOut);
		ChangeStatus(EConnecting);
		return;
	}
	
	//iTimer->Wait(KTimeOut);
	
	TInt answer=0;
	TInt8 intrusionmode;
//	TInt Index;
//	TTime CurrentTimeStamp;
	TTime LastTimeStamp;
//	TDateTime Dt;
	TBuf<150> description;
//	TBuf<20> tempbuf;
	TInt iScreenState = -1;

	CDir *dirList;
	TInt err;
	
	imageFile.Copy(_L(""));
	
	if( icompareflag == 1)
	{
		err = iRFsession.GetDir(KFilesPhonePhoto, KEntryAttNormal, ESortByDate, dirList); 
	}
	else if(icompareflag == 2)
	{
		err = iRFsession.GetDir(KFilesMMCPhoto, KEntryAttNormal, ESortByDate, dirList);
	}
	else if(icompareflag == 3)
	{
		err = iRFsession.GetDir(KFilesPhonePhotoSony, KEntryAttNormal, ESortByDate, dirList);
	}
	else if(icompareflag == 4)
	{
		err = iRFsession.GetDir(KFilesMMCPhotoSony, KEntryAttNormal, ESortByDate, dirList);
	}
	else
	{
		return; //Error 
	}
	CleanupStack::PushL(dirList);	
	if (err != KErrNone)
	{				
		return;			
	}
	
	TParse fullentry;
	TInt filesize;
	//for(int i=0 ; i < dirList->Count(); i++)
	for(int i=dirList->Count()-1 ; i >=0; i--)
	{
		if( icompareflag == 1)
		{
			fullentry.Set((*dirList)[i].iName, &KFilesPhonePhoto, NULL);
			filesize = (*dirList)[i].iSize;
		}
		else if(icompareflag == 2)
		{
			fullentry.Set((*dirList)[i].iName, &KFilesMMCPhoto, NULL);
			filesize = (*dirList)[i].iSize;
		}
		else if(icompareflag == 3)
		{
			fullentry.Set((*dirList)[i].iName, &KFilesPhonePhotoSony, NULL);
			filesize = (*dirList)[i].iSize;
		}
		else if(icompareflag == 4)
		{
			fullentry.Set((*dirList)[i].iName, &KFilesMMCPhotoSony, NULL);
			filesize = (*dirList)[i].iSize;
		}
		else
		{
			return; //Error 
		}

		TTime timestamp;

		iRFsession.Modified(fullentry.FullName(),timestamp);

		TTimeIntervalSeconds sec;
		iCurrentTimeStamp.SecondsFrom(timestamp,sec);

		TInt sec1;
		sec1 = sec.Int();

		if(sec1 < 5)// && sec1 >= 0)
		{					
			imageFile.Copy(fullentry.FullName());			
			break;
		}
	}
	CleanupStack::PopAndDestroy();	// delete dirList

	intrusionmode = iAppUi.GetMode();

	iPublic_Photo=EFalse;

//	iAppUi.iPhotoCount = 1;

	TInt value = 2;
	TInt imagestatus = 0;

	if(intrusionmode==MODE_PROMPT)
	{
		iAppUi.BringToForeground();	
	}

	if(imageFile.Compare(_L(""))!=0)
	{
		do
		{
			iScreenState = iAppUi.ReturnScreenValue();
			iAppUi.DeleteWindow(iScreenState);

			if(intrusionmode == MODE_PROMPT)
			{
				imagestatus = iAppUi.ProcessAlbum();			
			}

			if(imagestatus == 0)
			{
				value = 1;
				iAppUi.SendToBackground();
			}
			else if(imagestatus == 2)
			{
				TRAPD(delerr, BaflUtils::DeleteFile(iRFsession,imageFile));
				value = 1;
				iAppUi.SendToBackground();
			}
			else
			{
				if(intrusionmode==MODE_PROMPT)
				{

	#ifdef __LOGME__
			iAppUi.LogText.Copy(_L("\nCPhotosEngine::RunL MODE_PROMPT\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);
	#endif
					if (iStatus != -1) // want to upload?
					{
						answer=iAppUi.FolderDialog();	
						if(answer)
						{
							value = 1;
							comment.Copy(iAppUi.FolderName);
							
							CDesCArray* array;
							TInt aPos;
							array = iAppUi.GetFolderList();
							if(array)
							{
								if(array->Find(iAppUi.FolderName, aPos) != 0)
								{
									iNewFolder=1;
									iAppUi.CheckAndSaveFolderToDB(iAppUi.FolderName);
								}
								else
								{
									//Folder already exist
									iNewFolder=0;
								}
							}
							else
							{
								iNewFolder=1;
								iAppUi.CheckAndSaveFolderToDB(iAppUi.FolderName);
							}
						} // if(answer)
						else
						{
		#ifdef __LOGME__
							iAppUi.LogText.Copy(_L("\nCPhotosEngine::RunL MODE_PROMPT Cancel\n"));
							iAppUi.WriteLogFile(iAppUi.LogText);
		#endif

							value = 2;
							// cancelled
							//iAppUi.SendToBackground();
						}
					} // if(iStatus		
				} // if(intrusionmode
				else if(intrusionmode==MODE_AUTO)// non-intrusive mode
				{
		#ifdef __LOGME__
					iAppUi.LogText.Copy(_L("\nCPhotosEngine::RunL MODE_AUTO\n"));
					iAppUi.WriteLogFile(iAppUi.LogText);
		#endif
					value = 1;
					//if(iAppUi.iDescription == 1)
					//	iAppUi.BringToForeground();	
					Count=-1;
					iPublic_Photo=EFalse;	
					comment.Copy(iAppUi.GetDefaultFolder());
					iNewFolder=0;
					answer = 1; // upload
				}
			}
		}
		while(value == 2);
	}///

	if(imagestatus == 1)
	{
		if (answer)
		{
			if(iAppUi.iDescription == 1 && intrusionmode == MODE_PROMPT)
			{
				answer = 0;
				answer=iAppUi.DescDialog();	
				if(answer)
				{
					description.Copy(iAppUi.DescName);

	#ifdef __LOGME__
					iAppUi.LogText.Copy(_L("\nphoto="));
					iAppUi.WriteLogFile(iAppUi.LogText);

					iAppUi.LogText.Copy(imageFile);
					iAppUi.WriteLogFile(iAppUi.LogText);

					iAppUi.LogText.Copy(_L("\n"));
					iAppUi.WriteLogFile(iAppUi.LogText);
	#endif

					iAppUi.AddPhotoRecord(imageFile, iPublic_Photo, comment,description, iNewFolder, filesize);	

					iAppUi.ScheduleToSend();

					/*TInt network;	
					network = iAppUi.CheckNetwork();

					if(network == NETWORK_ROAMING)
					{
						if(iAppUi.iRoaming == 1)
						{
							iAppUi.ScheduleToSend();
						}
					}
					else
					{
						iAppUi.ScheduleToSend();				
					}*/
				}
				else
				{
					iAppUi.AddPhotoRecord(imageFile, iPublic_Photo, comment,description, iNewFolder, filesize);	

					iAppUi.ScheduleToSend();
					
					/*TInt network;	
					network = iAppUi.CheckNetwork();

					if(network == NETWORK_ROAMING)
					{
						if(iAppUi.iRoaming == 1)
						{
							iAppUi.ScheduleToSend();
						}
					}
					else
					{
						iAppUi.ScheduleToSend();				
					}*/
				}
				iAppUi.SendToBackground();
			}	
			else
			{
				iAppUi.AddPhotoRecord(imageFile, iPublic_Photo, comment,description, iNewFolder, filesize);	
				
				iAppUi.ScheduleToSend();

				/*TInt network;	
				network = iAppUi.CheckNetwork();

				if(network == NETWORK_ROAMING)
				{
					if(iAppUi.iRoaming == 1)
					{
						iAppUi.ScheduleToSend();
					}
				}
				else
				{
					iAppUi.ScheduleToSend();
				}*/

				iAppUi.SendToBackground();
			}
		}
	}
	
	iPublic_Photo=EFalse;


	if( iScreenState != -1)
	{
		if(iScreenState == KViewIdStatusScreen){
			iAppUi.iMainScreenValue = 6;
			iAppUi.ShowWindow(KViewIdStatusScreen); //status screen
			iAppUi.ShowStatusScreen();
		}
		else
			iAppUi.ShowWindow(iScreenState);
	}

	if( icompareflag == 1)
	{
		iRFsession.NotifyChange(ENotifyFile,iStatus,KDirPhonePhoto);
	}
	else if(icompareflag == 2)
	{
		iRFsession.NotifyChange(ENotifyFile,iStatus,KDirMMCPhoto);
	}
	else if(icompareflag == 3)
	{
		iRFsession.NotifyChange(ENotifyFile,iStatus,KDirPhonePhotoSony);
	}
	else if(icompareflag == 4)
	{
		iRFsession.NotifyChange(ENotifyFile,iStatus,KDirMMCPhotoSony);
	}
	else
	{
		return; //Error 
	}
	ChangeStatus(EOpenMenu);
	SetActive();
}

#else // Series 60 code below

void CPhotosEngine::RunL()
{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("\nCPhotosEngine::RunL\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	int answer;
	TInt8 intrusionmode;
	TInt Index;	
	TInt8 imagecount;
//	TInt ret;	

	switch(iEngineStatus)
	{
		case ESmartCharge:
		{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("\nCPhotosEngine::ESmartCharge\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

//			iAppUi.ActivateView(2);
//			iAppUi.SendToBackground();	
//			iAppUi.BringToForeground();
			
			iCurrentTimeStamp.HomeTime();			

			intrusionmode = iAppUi.GetMode();

			if(intrusionmode==MODE_PROMPT || intrusionmode==MODE_AUTO)
			{				
				/*TBuf<150> msg;
				msg.Copy(_L("Do you want to Publish this Photos? You would be charged 3 pesos."));
				CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(msg);
				dlg->PrepareLC(R_SMART_CHARGE);
				ret = dlg->RunLD();
				if(ret)
				{
					ChangeStatus(EOpenMenu);
					Notify();
				}
				else
				{
					ChangeStatus(EWritten);
					Notify();
				}*/
			
				CDir *dirList;
				// get the dirs contents
				TInt err;
				if( icompareflag == 1)
				{
					err = iRFsession.GetDir(KFilesPhonePhoto, KEntryAttNormal, ESortByDate, dirList);
				}
				else if(icompareflag == 2)
				{
					err = iRFsession.GetDir(KFilesMMCPhoto, KEntryAttNormal, ESortByDate, dirList);
				}
				else
				{
					return; //Error 
				}
				CleanupStack::PushL(dirList);
				
				if (err != KErrNone)		// if error, just return, dir must not exist
					return;			
		
				imagecount = dirList->Count();

				CleanupStack::PopAndDestroy();// delete dirList

				if(imagecount <= iImageCount)
				{				
					iCountflag = 1;					
					ChangeStatus(EWritten);			
					Notify();
				}
				else
				{
					iCountflag = 0;
					GetImageCount();
					
					if(intrusionmode==MODE_PROMPT)
					{
						//iAppUi.HideView();
						//iAppUi.ActivateView(KViewIdEmptyScreen);
						//iAppUi.SendToBackground();
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("CPhotosEngine::1\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

						iAppUi.BringToForeground();
					}
					ChangeStatus(ECheckFile);
					Notify();
				}				
			}
			else
			{				
			}			
			break;
		}
	case ECheckFile:
		{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("CPhotosEngine::2\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
			intrusionmode = iAppUi.GetMode();
			
			if(intrusionmode==MODE_PROMPT)
			{
				/*
				TInt index;
				CAknListQueryDialog* listquery = new (ELeave) CAknListQueryDialog(&index);
				if(listquery->ExecuteLD(R_SELECT_IMAGE_OPTION))			
				{
					if(index == 1)
						ChangeStatus(EOpenMenu);
					else
					{
						ChangeStatus(EWritten);
					}
					Notify();
				}*/
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("CPhotosEngine::3\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
				ProcessImage();

				if(iStatus.Int() == 0) 
				{
					iStatus=-2;
					ChangeStatus(EWritten);
					Notify();
				}
				else if(iStatus.Int() == 1) 
				{
					iStatus=-2;
					ChangeStatus(EOpenMenu);
					Notify();
				}
				else if(iStatus.Int() == 2) 
				{
					CDir *dirList;
					// get the dirs contents
					TInt err;
					if( icompareflag == 1)
					{
						err = iRFsession.GetDir(KFilesPhonePhoto, KEntryAttNormal, ESortByDate, dirList);
					}
					else if(icompareflag == 2)
					{
						err = iRFsession.GetDir(KFilesMMCPhoto, KEntryAttNormal, ESortByDate, dirList);
					}
					else
					{
						return; //Error 
					}
					CleanupStack::PushL(dirList);
					if (err != KErrNone)		// if error, just return, dir must not exist
						return;			
					TParse fullentry;

					//////
					TBuf<160> imageFile;
					TInt fileSize;
					//for(int i=0 ; i < dirList->Count(); i++)
					for(int i=dirList->Count()-1 ; i >=0; i--)
					{
						if( icompareflag == 1)
						{
							fullentry.Set((*dirList)[i].iName, &KFilesPhonePhoto, NULL);
						}
						else if(icompareflag == 2)
						{
							fullentry.Set((*dirList)[i].iName, &KFilesMMCPhoto, NULL);
						}
						else
						{
							return; //Error 
						}

						TTime timestamp;

						iRFsession.Modified(fullentry.FullName(),timestamp);

						TTimeIntervalSeconds sec;
						iCurrentTimeStamp.SecondsFrom(timestamp,sec);

						TInt sec1;
						sec1 = sec.Int();

		#ifdef __LOGME__
						iAppUi.LogText.Copy(_L("\n\ntimediff="));
						iAppUi.LogText.AppendNum(sec1);
						iAppUi.LogText.Append(_L("\n\n"));
						iAppUi.WriteLogFile(iAppUi.LogText);
		#endif
						if(sec1 < 5)// && sec1 >= 0)
						{					
							imageFile.Copy(fullentry.FullName());			
							fileSize = (*dirList)[i].iSize;
							break;
						}
					}			
					CleanupStack::PopAndDestroy();		// delete dirList

					//iAppUi.LogText.Copy(_L("\n\n\nFILENAME="));
					//iAppUi.WriteLogFile(iAppUi.LogText);

					//iAppUi.LogText.Copy(imageFile);
					//iAppUi.WriteLogFile(iAppUi.LogText);

					TRAPD(delerr, BaflUtils::DeleteFile(iRFsession,imageFile));

					iStatus=-2;
					ChangeStatus(EWritten);
					Notify();
				}
			}
			else
			{
				ChangeStatus(EOpenMenu);
                Notify();
			}
		}
		break;
	case EOpenMenu:
	{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("\nCPhotosEngine::EOpenMenu\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		iStatus=-2;	 	   					
		iNewFolder = 0;
		
		//iAppUi.HideView();
//		iAppUi.ActivateView(2);

//		iAppUi.SendToBackground();

//		iAppUi.BringToForeground();

		intrusionmode = iAppUi.GetMode();
		
		if(intrusionmode==MODE_PROMPT)
		{		
			//iAppUi.ActivateView(KViewIdEmptyScreen);

			//iAppUi.BringToForeground();

#ifdef __LOGME__
			iAppUi.LogText.Copy(_L("get folder\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);
#endif

			GetFolder();

			iAppUi.Show=1;
			// if iStatus == -1, no folder is selected
			// i.e. dont want to send
			if (iStatus != -1) // want to upload?
			{
				if(iStatus.Int()==Count) 
				{
					iStatus=-2;
					Index=0;
					iStatus=0;
					iPublic_Photo=EFalse;
					ChangeStatus(EConnectFolder);
				}
				else
				{
					iStatus=0;
					if(iAppUi.iDescription == 1)
						ChangeStatus(EDescription);
					else
						ChangeStatus(EConnecting);
				}
                Notify();
			}
			else // no folder selected
			{
				//ChangeStatus(EWritten);
				ChangeStatus(ECheckFile);
				Notify();
			}
		}
		else if(intrusionmode==MODE_AUTO)// non-intrusive mode
		{	
			Count=-1;	
			comment.Copy(iAppUi.GetDefaultFolder());
			//if(iAppUi.iDescription == 1)
			//	ChangeStatus(EDescription);
			//else
				ChangeStatus(EConnecting);
			Notify();
		}		
	}
	break;
	case EConnectFolder:
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("\nCPhotosEngine::EConnectFolder\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		if(iStatus.Int()>=0)
		{
			TInt compareflag;
            TBuf<160> folderName;

            folderName.FillZ();
			folderName.Zero();
			folderName.SetLength(0);
			CAknTextQueryDialog* dlg = CAknTextQueryDialog::NewL(folderName, CAknQueryDialog::ENoTone );
			answer=( dlg->ExecuteLD( R_FOLDER_NAME_QUERY ) ); // new folder name
			if(answer)
			{
				iStatus=0;			
				
				compareflag=iAppUi.CheckAndSaveFolderToDB(folderName);

				if(compareflag!=1)
				{
					comment.Copy(folderName);
					iNewFolder = 1;
					if(iAppUi.iDescription == 1)
						ChangeStatus(EDescription);
					else
						ChangeStatus(EConnecting);
				}
				else
				{
					CDesCArray* folderexist = iAppUi.FolderExist();
//					CEikonEnv::Static ()->AlertWin (folderexist->MdcaPoint(0)) ;
					TBuf<100> msg;
					msg.Copy(folderexist->MdcaPoint(0));
					CAknMessageQueryDialog* verdlg = CAknMessageQueryDialog::NewL(msg);	
					verdlg->PrepareLC(R_ABOUT_QUERY);
					verdlg->RunLD();
					ChangeStatus(EConnectFolder);
					folderexist->Reset();
				}
                Notify();
			}
			else
			{
				Index=0;				
				iStatus=0;
				iPublic_Photo=EFalse;
				//ChangeStatus(EWritten);
                ChangeStatus(EOpenMenu);
				Notify();
			}
		}
		break;
	case EDescription:
		{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("\nCPhotosEngine::EDescription\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

			TBuf<150> descName;
			CAknTextQueryDialog* dlg = CAknTextQueryDialog::NewL(descName, CAknQueryDialog::ENoTone );
			answer= dlg->ExecuteLD(R_DESC_NAME_QUERY); // desc name
			if(answer)
			{
				description.Copy(descName);
				ChangeStatus(EConnecting);
				Notify();
			}
			else
			{
				description.Copy(_L(""));
				ChangeStatus(EConnecting);
				Notify();
			}
		}
		break;
	case EConnecting:
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("\nCPhotosEngine::EConnecting\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
		if(iStatus.Int()>=0)
		{
			CDir *dirList;
			// get the dirs contents
			TInt err;
			if( icompareflag == 1)
			{
				err = iRFsession.GetDir(KFilesPhonePhoto, KEntryAttNormal, ESortByDate, dirList);
			}
			else if(icompareflag == 2)
			{
				err = iRFsession.GetDir(KFilesMMCPhoto, KEntryAttNormal, ESortByDate, dirList);
			}
			else
			{
				return; //Error 
			}
			CleanupStack::PushL(dirList);
			if (err != KErrNone)		// if error, just return, dir must not exist
				return;			
			TParse fullentry;

			//////
			TBuf<160> imageFile;
			TInt fileSize;
			//for(int i=0 ; i < dirList->Count(); i++)
			for(int i=dirList->Count()-1 ; i >=0; i--)
			{
				if( icompareflag == 1)
				{
					fullentry.Set((*dirList)[i].iName, &KFilesPhonePhoto, NULL);
				}
				else if(icompareflag == 2)
				{
					fullentry.Set((*dirList)[i].iName, &KFilesMMCPhoto, NULL);
				}
				else
				{
					return; //Error 
				}

				TTime timestamp;

				iRFsession.Modified(fullentry.FullName(),timestamp);

				TTimeIntervalSeconds sec;
				iCurrentTimeStamp.SecondsFrom(timestamp,sec);

				TInt sec1;
				sec1 = sec.Int();

#ifdef __LOGME__
				iAppUi.LogText.Copy(_L("\n\ntimediff="));
				iAppUi.LogText.AppendNum(sec1);
				iAppUi.LogText.Append(_L("\n\n"));
				iAppUi.WriteLogFile(iAppUi.LogText);
#endif
				if(sec1 < 5)// && sec1 >= 0)
				{					
					imageFile.Copy(fullentry.FullName());			
					fileSize = (*dirList)[i].iSize;
					break;
				}
			}			
			CleanupStack::PopAndDestroy();		// delete dirList
			//Anupam
			iPublic_Photo=EFalse;
			iAppUi.AddPhotoRecord(imageFile, iPublic_Photo, comment, description, iNewFolder, fileSize);

			iAppUi.ScheduleToSend();

			/*TInt network;	

			network = iAppUi.CheckNetwork();

			if(network == NETWORK_ROAMING)
			{
				if(iAppUi.iRoaming == 1)
				{
					iAppUi.ScheduleToSend();
				}
			}
			else
				iAppUi.ScheduleToSend();*/
			//////
			ChangeStatus(EWritten);
            Notify();
		}
		break;

	case EWritten:
		{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("\nCPhotosEngine::EWritten\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
		 if(iCountflag == 1)
		 {
			 iCountflag = 0;
		 }
		 else
		 {			 
			 iCountflag = 0;
			 
			 iAppUi.SendToBackground();
		 }

		 GetImageCount();

		 ChangeStatus(ESmartCharge);
		 if( icompareflag == 1)
		 {
			iRFsession.NotifyChange(ENotifyFile,iStatus,KDirPhonePhoto);
		 }
		 else if(icompareflag == 2)
		 {
			iRFsession.NotifyChange(ENotifyFile,iStatus,KDirMMCPhoto);
		 }
		 else
		 {
			return; //Error 
		 }
		 SetActive();
		}
	break;

	default:
	break;
	}   	
}
#endif //__UIQ__

void CPhotosEngine::Notify()
{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
    if (!IsActive())
    {
	    SetActive();
    }
}
   
void CPhotosEngine::ChangeStatus(TSocketsEngineState aEngineStatus)
{
	iEngineStatus=aEngineStatus;
}

void CPhotosEngine::GetFolder()
{
#ifdef __UIQ__
//P800
#else
	TInt PopValue;		

	CDesCArray* items = iAppUi.GetFolderList();	
    CDesCArray* foldernames = iAppUi.ReturnText();

	if (items == NULL)
	{		

		Count = 0;
		items = new (ELeave) CDesCArrayFlat(1);
		//items->AppendL(_L("New Folder"));
		items->AppendL(foldernames->MdcaPoint(0));
	}
	else
	{		

		Count = items->MdcaCount();	
		items->AppendL(foldernames->MdcaPoint(0));
	}
	foldernames->Reset();

	CEikTextListBox* list = new( ELeave ) CAknSinglePopupMenuStyleListBox;
    CleanupStack::PushL( list );

    CAknPopupList* popupList = CAknPopupList::NewL(
       list,
       R_AVKON_SOFTKEYS_SELECT_BACK,
       AknPopupLayouts::EMenuWindow);

    CleanupStack::PushL( popupList );
    
    popupList->SetMaximumHeight(2);

    list->ConstructL( popupList, CEikListBox::ELeftDownInViewRect );
    list->CreateScrollBarFrameL( ETrue );
    list->ScrollBarFrame()->SetScrollBarVisibilityL(
		   CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );

    CTextListBoxModel* model = list->Model();
    model->SetItemTextArray( items );
    model->SetOwnershipType( ELbmOwnsItemArray );

	//TBuf<30> msg;
//	msg.Copy(_L("Charge "));
//	msg.Append(iAppUi.iCharge);
//	msg.Append(_L(" Pesos"));

	//msg.Copy(_L("Select Album:"));
	
	popupList->SetTitleL(*(CEikonEnv::Static()->AllocReadResourceL(R_IMAGES_SELECT_ALBUM)));
	
	PopValue=popupList->ExecuteLD();		
	
	if(PopValue==0)
	{
		iStatus=-1;
	}
	else if(PopValue==1)
	{
		PopValue=list->CurrentItemIndex();		
		iStatus=PopValue;		
		TPtrC ptr1(items->MdcaPoint(PopValue));
		comment.Copy(ptr1);		
	}
	
	items->Reset();
	CleanupStack::Pop(); // popupList
    CleanupStack::PopAndDestroy(); // list	
#endif	
}

void CPhotosEngine::ProcessImage()
{
#ifdef __UIQ__
//P800
#else
	TInt PopValue;		

	CDesCArray* items = iAppUi.ReturnTextForImage();	
    
	if (items == NULL)
	{
		Count = 0;
		items = new (ELeave) CDesCArrayFlat(1);		
	}
	else
	{		
		Count = items->MdcaCount();	
	}

	CEikTextListBox* list = new( ELeave ) CAknSinglePopupMenuStyleListBox;
    CleanupStack::PushL( list );

    CAknPopupList* popupList = CAknPopupList::NewL(
       list,
       R_AVKON_SOFTKEYS_OK_EMPTY,
       AknPopupLayouts::EMenuWindow);

    CleanupStack::PushL( popupList );
    
    popupList->SetMaximumHeight(3);

    list->ConstructL( popupList, CEikListBox::ELeftDownInViewRect );
    list->CreateScrollBarFrameL( ETrue );
    list->ScrollBarFrame()->SetScrollBarVisibilityL(
		   CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );

    CTextListBoxModel* model = list->Model();
    model->SetItemTextArray( items );
    model->SetOwnershipType( ELbmOwnsItemArray );

	PopValue=popupList->ExecuteLD();		
	
	if(PopValue==0)
	{
		iStatus=-1;
	}
	else if(PopValue==1)
	{
		PopValue=list->CurrentItemIndex();		
		iStatus=PopValue;		
	}
	
	items->Reset();
	CleanupStack::Pop(); // popupList
    CleanupStack::PopAndDestroy(); // list	
#endif	
}

#ifdef __UIQ__
void CPhotosEngine::TimerExpired()
{
	Notify();
	//iTimer->After(KTimeOut);
}

void CPhotosEngine::GetTimeStamp()
{
	CDir *dirList;
	// get the dirs contents
	TInt err;
	if( icompareflag == 1)
	{
		err = iRFsession.GetDir(KFilesPhonePhoto, KEntryAttNormal, ESortByDate, dirList);
	}
	else if(icompareflag == 2)
	{
		err = iRFsession.GetDir(KFilesMMCPhoto, KEntryAttNormal, ESortByDate, dirList);
	}
	else
	{
		return; //Error 
	}
	CleanupStack::PushL(dirList);			

	if (err != KErrNone)		// if error, just return, dir must not exist
	{				
		return;			
	}
		
	TParse fullentry;		
			
	if(dirList->Count() > 0)
	{
		if( icompareflag == 1)
		{
			fullentry.Set((*dirList)[dirList->Count()-1].iName, &KFilesPhonePhoto, NULL);
		}
		else if(icompareflag == 2)
		{
			fullentry.Set((*dirList)[dirList->Count()-1].iName, &KFilesMMCPhoto, NULL);	
		}
		else
		{
			return; //Error 
		}
		iRFsession.Modified(fullentry.FullName(),TimeStamp);
	}
	else
	{
		TimeStamp=0;	
	}
		
	CleanupStack::PopAndDestroy();		
}
#endif

void CPhotosEngine::GetImageCount()
{
	CDir* dirList1;
	// get the dirs contents			
	TInt err;
	if( icompareflag == 1)
	{
		err = iRFsession.GetDir(KFilesPhonePhoto, KEntryAttNormal , ESortByDate, dirList1);
	}
	else if(icompareflag == 2)
	{
		err = iRFsession.GetDir(KFilesMMCPhoto, KEntryAttNormal , ESortByDate, dirList1);
	}
	else
	{
		return; //Error 
	}
	CleanupStack::PushL(dirList1);

	iImageCount = dirList1->Count(); 
	
	CleanupStack::PopAndDestroy();		// delete dirList1	
}
