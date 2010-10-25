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
 Module Name				: VideosEngine.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :    Videos engine for Sym6 & UIQ 
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#ifndef __UIQ__
#include <AknGlobalListQuery.h>
#include <aknmessagequerydialog.h>
#endif
#include <eikgted.h>

#ifdef __UIQ__
#include <eiktxlbx.h>
#include "PhotoTimeOutTimer.h"
#endif

#include "VideosEngine.h"
//#include "Sockets.pan"
#include "RippleVaultAppUi.h"
#include <eikenv.h>
class CRippleVaultAppUi;

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

#include <etel.h>
//#include <etelbgsm.h>
#include <bautils.h>
//#include <AgentClient.h>
//#include <cdbcols.h>
//#include <commdb.h>
#ifndef __UIQ__
#include <aknlists.h> 
#include <aknpopup.h> 
#endif

#ifdef __UIQ__
	//p800 need to change 
/*	_LIT(KDir1,"c:\\documents\\media files\\video\\unfiled");
	_LIT(KDir2,"d:\\media files\\video\\unfiled");
	_LIT(KDirImageFolder1,"c:\\documents\\media files\\video\\unfiled\\*");
	_LIT(KDirImageFolder2,"d:\\media files\\video\\unfiled\\*");

	_LIT(KDir3,"c:\\documents\\media files\\video\\Sony Ericsson");
	_LIT(KDir4,"d:\\media files\\video\\Sony Ericsson");
	_LIT(KDirImageFolder3,"c:\\documents\\media files\\video\\Sony Ericsson\\*");
	_LIT(KDirImageFolder4,"d:\\media files\\video\\Sony Ericsson\\*");
*/
	const TInt CVideosEngine::KTimeOut = 1000000; //1 seconds time-out
#else 
/*	_LIT(KDirImageFolder1,"c:\\Nokia\\Videos\\*");
	_LIT(KDirImageFolder2,"e:\\Videos\\*");
	_LIT(KDir1,"c:\\Nokia\\Videos");
	_LIT(KDir2,"e:\\Videos");*/
#endif

CVideosEngine* CVideosEngine::NewL(CRippleVaultAppUi& aAppUi, TDesC& aFolderName)
{
	CVideosEngine* self = CVideosEngine::NewLC(aAppUi, aFolderName);
	CleanupStack::Pop(self);
	return self;
}
	
CVideosEngine* CVideosEngine::NewLC(CRippleVaultAppUi& aAppUi, TDesC& aFolderName)
{
	CVideosEngine* self = new (ELeave) CVideosEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL(aFolderName);
	return self;
}

CVideosEngine::CVideosEngine(CRippleVaultAppUi& aAppUi)
: CActive(EPriorityStandard),
  iAppUi(aAppUi)
  //iPort(KDefaultPortNumber)
{ 
}

CVideosEngine::~CVideosEngine()
{
#ifdef __UIQ__
     delete iTimer;
#endif
    iRFsession.NotifyChangeCancel(iStatus);
    Cancel();	
	iRFsession.Close();
}

void CVideosEngine::ConstructL(TDesC& aFolderName)
{
	User::LeaveIfError(iRFsession.Connect());    

#ifdef __UIQ__
	icompareflag1 = 0;
	icompareflag1 = aFolderName.Compare(KDirPhoneVideo);
	if(icompareflag1 == 0)
	{
		icompareflag1 = 1;
	}
	else
	{
		icompareflag1 = aFolderName.Compare(KDirMMCVideo);
		if(icompareflag1 == 0)
		{
			icompareflag1 = 2;
		}
		else
		{
			icompareflag1 = aFolderName.Compare(KDirPhoneVideoSony);
			if(icompareflag1 == 0)
			{
				icompareflag1 = 3;
			}
			else
			{
				icompareflag1 = aFolderName.Compare(KDirMMCVideoSony);
				if(icompareflag1 == 0)
				{
					icompareflag1 = 4;
				}
				else
				{
					icompareflag1 = 0;
				}
			}
		}
	}
#else
	icompareflag1 = 0;
	icompareflag1 = aFolderName.Compare(KDirPhoneVideo);
	if(icompareflag1 == 0)
	{
		icompareflag1 = 1;
	}
	else
	{
		icompareflag1 = aFolderName.Compare(KDirMMCVideo);
		if(icompareflag1 == 0)
		{
			icompareflag1 = 2;
		}
		else
		{
			icompareflag1 = 0;
		}
	}
#endif
#ifdef __UIQ__
		//GetTimeStamp();
		iTimer = CPhotoTimeoutTimer::NewL(EPriorityHigh, *this);
		//CActiveScheduler::Add(this); 
		//iTimer->After(KTimeOut);
#endif
	if (BaflUtils::FileExists(iRFsession,aFolderName))
	{
		iRFsession.NotifyChange(ENotifyFile,iStatus,aFolderName);

		iCount = CheckCount();

		ChangeStatus(ECheckDisk);
		CActiveScheduler::Add(this); 
		SetActive();
	}
}
/*
void CVideosEngine::Disconnect()
{
}
*/
// from CActive
void CVideosEngine::DoCancel()
{ 
#ifdef __UIQ__
		iTimer->Cancel();
#endif
}

// from CActive

#ifdef __UIQ__
void CVideosEngine::RunL()
{
	iCurrentTimeStamp.HomeTime();

	if (iEngineStatus == ECheckDisk)
	{
		iTimer->After(KTimeOut);
		ChangeStatus(EConnecting);
		return;
	}
	
	TInt count;
	count = CheckCount();
	if(count > iCount)
	{	
		iCount = count;
			
	//iTimer->Wait(KTimeOut);
	
	TInt answer=0;
	TInt8 intrusionmode;
	//	TInt Index;
	//	TTime CurrentTimeStamp;
	TTime LastTimeStamp;
	//	TDateTime Dt;
	TBuf<160> description;
	TBuf<160> extension;
	_LIT(GPPEXT,".3gp");
	_LIT(GPPEXT1,".mp4");

	CDir *dirList;
	TInt err;
	TInt iScreenState;

	
	//	iAppUi.iPhotoCount = 1; 

	imageFile.Copy(_L(""));
	
	if( icompareflag1 == 1)
	{
		err = iRFsession.GetDir(KFilesPhoneVideo, KEntryAttNormal, ESortByDate, dirList); 
	}
	else if(icompareflag1 == 2)
	{
		err = iRFsession.GetDir(KFilesMMCVideo, KEntryAttNormal, ESortByDate, dirList);
	}
	else if( icompareflag1 == 3)
	{
		err = iRFsession.GetDir(KFilesPhoneVideoSony, KEntryAttNormal, ESortByDate, dirList); 
	}
	else if(icompareflag1 == 4)
	{
		err = iRFsession.GetDir(KFilesMMCVideoSony, KEntryAttNormal, ESortByDate, dirList);
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
	for(int i=0 ; i < dirList->Count(); i++)
	{
		if( icompareflag1 == 1)
		{
			fullentry.Set((*dirList)[i].iName, &KFilesPhoneVideo, NULL);
			filesize = (*dirList)[i].iSize;
		}
		else if(icompareflag1 == 2)
		{
			fullentry.Set((*dirList)[i].iName, &KFilesMMCVideo, NULL);
			filesize = (*dirList)[i].iSize;
		}
		else if(icompareflag1 == 3)
		{
			fullentry.Set((*dirList)[i].iName, &KFilesPhoneVideoSony, NULL);
			filesize = (*dirList)[i].iSize;
		}
		else if(icompareflag1 == 4)
		{
			fullentry.Set((*dirList)[i].iName, &KFilesMMCVideoSony, NULL);
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

		if(sec1 < 5)
		{					
			imageFile.Copy(fullentry.FullName());	
			extension.Copy(fullentry.Ext());
		}
	}
	CleanupStack::PopAndDestroy();	// delete dirList	
	
	intrusionmode = iAppUi.GetMode();

	iPublic_Photo=EFalse;

	if(extension.Compare(GPPEXT) == 0 || extension.Compare(GPPEXT1) == 0)
	{
		if(extension.Compare(GPPEXT1) == 0)
		{
			TInt err;						
			do
			{
				err = TempFile.Open(iRFsession, imageFile, EFileRead|EFileStream);
				if (err == KErrNone)
				{
					TempFile.Close();
					err=0;
				}
				else
					err=1;				
			}
			while(err == 1);			
		}

		TInt value = 2;
		TInt imagestatus = 0;

		if(intrusionmode==MODE_PROMPT)
		{
			iAppUi.BringToForeground();	
		}

		do
		{
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
					iScreenState = iAppUi.ReturnScreenValue();
					iAppUi.DeleteWindow(iScreenState);

					if (iStatus != -1) // want to upload?
					{
						answer=iAppUi.FolderDialog();	
						if(answer)
						{
							value = 1;
							comment.Copy(iAppUi.FolderName);
							//Anupam
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
							value = 2;
							// cancelled
							//iAppUi.SendToBackground();
						}
					} // if(iStatus		
				} // if(intrusionmode
				else if(intrusionmode==MODE_AUTO)// non-intrusive mode
				{
					value = 1;
					Count=-1;
					iPublic_Photo=EFalse;	
					comment.Copy(iAppUi.GetDefaultFolder());
					iNewFolder=0;
					answer = 1; // upload
				}
			}
		}
		while(value == 2);

		if (answer)
		{
			if(intrusionmode==MODE_PROMPT && iAppUi.iDescription == 1)
			{
				answer = 0;
				answer=iAppUi.DescDialog();	
				if(answer)
				{
					description.Copy(iAppUi.DescName);
					iAppUi.AddVideoRecord(imageFile, iPublic_Photo, comment, description, iNewFolder, filesize);	

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
				}
				else
				{
					iAppUi.AddVideoRecord(imageFile, iPublic_Photo, comment, description, iNewFolder, filesize);	
					
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
				iAppUi.AddVideoRecord(imageFile, iPublic_Photo, comment, description, iNewFolder, filesize);				
				
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
	}

	if( icompareflag1 == 1)
	{
		iRFsession.NotifyChange(ENotifyFile,iStatus,KDirPhoneVideo);
	}
	else if(icompareflag1 == 2)
	{
		iRFsession.NotifyChange(ENotifyFile,iStatus,KDirMMCVideo);
	}
	else if( icompareflag1 == 3)
	{
		iRFsession.NotifyChange(ENotifyFile,iStatus,KDirPhoneVideoSony);
	}
	else if(icompareflag1 == 4)
	{
		iRFsession.NotifyChange(ENotifyFile,iStatus,KDirMMCVideoSony);
	}
	else
	{
		return; //Error 
	}
	ChangeStatus(EOpenMenu);
	SetActive();
}
#else //Series 60
void CVideosEngine::RunL()
{
	_LIT8(GPPEXT,".3gp");
	int answer;
	TInt8 intrusionmode;
	TInt Index;	
	TInt count;
//	TInt ret;	

	switch(iEngineStatus)
	{
	case ECheckDisk:
		count = CheckCount();
		if(count > iCount)
		{	
			iCount = count;
			ChangeStatus(EOpenMenu);
			Notify();
		}
		else
		{				
			iCount = count;
			ChangeStatus(EWritten);
			Notify();
		}		
		break;
	case EOpenMenu:
		{
			iCurrentTimeStamp.HomeTime();

			CDir* dirList;
			TInt err;
			if( icompareflag1 == 1)
			{
				err = iRFsession.GetDir(KFilesPhoneVideo, KEntryAttNormal, ESortByDate, dirList);
			}
			else if(icompareflag1 == 2)
			{
				err = iRFsession.GetDir(KFilesMMCVideo, KEntryAttNormal, ESortByDate, dirList);
			}
			else
			{
				return; //Error 
			}
			CleanupStack::PushL(dirList);

			TParse fullentry;
			TBuf8<128> tempp;
			//fullentry.Set((*dirList)[dirList->Count()-1].iName, &aDir, NULL);
			for(int i=0 ; i < dirList->Count(); i++)
			{
				if( icompareflag1 == 1)
				{
					fullentry.Set((*dirList)[i].iName, &KFilesPhoneVideo, NULL);
				}
				else if(icompareflag1 == 2)
				{
					fullentry.Set((*dirList)[i].iName, &KFilesMMCVideo, NULL);
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

				if(sec1 < 5)
				{				
					iFilepos=i;
					tempp.Copy(fullentry.FullName());
					break;
				}
			}
			
			CleanupStack::PopAndDestroy();		// delete dirList
			
			ChangeStatus(EProcessFile);
			Notify();
			
			/*if(tempp.Find(GPPEXT)==KErrNotFound)
			{
				ChangeStatus(EOpenMenu);
				Notify();
			}			
			else
			{	
				ChangeStatus(ESmartCharge);
				Notify();
			}*/
		}
		break;
	case EProcessFile:
		{			
			CDir* dirList;
			TInt err;
			if( icompareflag1 == 1)
			{
				err = iRFsession.GetDir(KFilesPhoneVideo, KEntryAttNormal, ESortByDate, dirList);
			}
			else if(icompareflag1 == 2)
			{
				err = iRFsession.GetDir(KFilesMMCVideo, KEntryAttNormal, ESortByDate, dirList);
			}
			else
			{
				return; //Error 
			}
			CleanupStack::PushL(dirList);

			TParse fullentry;
			TBuf8<128> tempp;
			
			if( icompareflag1 == 1)
			{
				fullentry.Set((*dirList)[iFilepos].iName, &KFilesPhoneVideo, NULL);
			}
			else if(icompareflag1 == 2)
			{
				fullentry.Set((*dirList)[iFilepos].iName, &KFilesMMCVideo, NULL);
			}
			else
			{
				return; //Error 
			}

			CleanupStack::PopAndDestroy();		// delete dirList
			
			tempp.Copy(fullentry.FullName());

			if(tempp.Find(GPPEXT)==KErrNotFound)
			{
				ChangeStatus(EProcessFile);
				Notify();
			}			
			else
			{					
				//ChangeStatus(ESmartCharge);
				ChangeStatus(ECheckFile);
				Notify();
			}
		}
		break;		
	case ECheckFile:
		{
			intrusionmode = iAppUi.GetMode();
			
			if(intrusionmode==MODE_PROMPT)
			{
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
					ChangeStatus(ESmartCharge);
					Notify();
				}
				else if(iStatus.Int() == 2) 
				{
					CDir *dirList;
					TInt err;
					if( icompareflag1 == 1)
					{
						err = iRFsession.GetDir(KFilesPhoneVideo, KEntryAttNormal, ESortByDate, dirList);
					}
					else if(icompareflag1 == 2)
					{
						err = iRFsession.GetDir(KFilesMMCVideo, KEntryAttNormal, ESortByDate, dirList);
					}
					else
					{
						return; //Error 
					}
					CleanupStack::PushL(dirList);

					if (err != KErrNone)		// if error, just return, dir must not exist
						return;			
					TParse fullentry;
					TInt filesize;
					
					//////
					TBuf<160> imageFile;
					if( icompareflag1 == 1)
					{
						fullentry.Set((*dirList)[iFilepos].iName, &KFilesPhoneVideo, NULL);
						filesize = (*dirList)[iFilepos].iSize;
					}
					else if(icompareflag1 == 2)
					{
						fullentry.Set((*dirList)[iFilepos].iName, &KFilesMMCVideo, NULL);
						filesize = (*dirList)[iFilepos].iSize;
					}
					else
					{
						return; //Error 
					}
					imageFile.Copy(fullentry.FullName());
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
				ChangeStatus(ESmartCharge);
                Notify();
			}
		}
		break;

	case ESmartCharge:
		{
			//iAppUi.ActivateView(KViewIdEmptyScreen);
			//iAppUi.SendToBackground();
			
			//iAppUi.BringToForeground();

			intrusionmode = iAppUi.GetMode();

			if(intrusionmode==MODE_PROMPT)// || intrusionmode==MODE_AUTO)
			{
				//iAppUi.ActivateView(KViewIdEmptyScreen);
				//iAppUi.SendToBackground();
				
				iAppUi.BringToForeground();

				/*TBuf<150> msg;
				msg.Copy(_L("Do you want to Publish this Video? You would be charged 3 pesos."));
				CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(msg);
				dlg->PrepareLC(R_SMART_CHARGE);
				ret = dlg->RunLD();
				if(ret)
				{
					ChangeStatus(EOptionChosen);
					Notify();
				}
				else
				{
					ChangeStatus(EWritten);
					Notify();
				}*/
			}
				ChangeStatus(EOptionChosen);
				Notify();			
		}
		break;
	case EOptionChosen:
	{		
		iStatus=-2;	 	   			
		//iAppUi.BringToForeground();
		iNewFolder = 0;
		
		intrusionmode = iAppUi.GetMode();

			if(intrusionmode==MODE_PROMPT)
			{		
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
						Notify();
					}
					else
					{
						iStatus=0;
						if(iAppUi.iDescription == 1)
							ChangeStatus(EDescription);
						else
							ChangeStatus(EConnecting);
						Notify();
					}
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
			else
			{
				ChangeStatus(EWritten);
				Notify();
			}
	}
	break;
	case EConnectFolder:
		if(iStatus.Int()>=0)
		{
			TInt compareflag;
			TBuf<160> FolderName;

			FolderName.FillZ();
			FolderName.Zero();
			FolderName.SetLength(0);
#ifndef __UIQ__
			CAknTextQueryDialog* dlg = CAknTextQueryDialog::NewL(FolderName, CAknQueryDialog::ENoTone );
			answer=( dlg->ExecuteLD( R_FOLDER_NAME_QUERY ) ); // new folder name
			if(answer)
			{
				iStatus=0;			
				
				compareflag=iAppUi.CheckAndSaveFolderToDB(FolderName);

				if(compareflag!=1)
				{
					comment.Copy(FolderName);
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
				ChangeStatus(ECheckFile);
				Notify();
			}
#endif
		}
		break;
	case EDescription:
		{
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
		if(iStatus.Int()>=0)
		{
			CDir *dirList;
			TInt err;
			if( icompareflag1 == 1)
			{
				err = iRFsession.GetDir(KFilesPhoneVideo, KEntryAttNormal, ESortByDate, dirList);
			}
			else if(icompareflag1 == 2)
			{
				err = iRFsession.GetDir(KFilesMMCVideo, KEntryAttNormal, ESortByDate, dirList);
			}
			else
			{
				return; //Error 
			}
			CleanupStack::PushL(dirList);

			if (err != KErrNone)		// if error, just return, dir must not exist
				return;			
			TParse fullentry;
			TInt filesize;
			
			//////
			TBuf<160> imageFile;
			if( icompareflag1 == 1)
			{
				fullentry.Set((*dirList)[iFilepos].iName, &KFilesPhoneVideo, NULL);
				filesize = (*dirList)[iFilepos].iSize;
			}
			else if(icompareflag1 == 2)
			{
				fullentry.Set((*dirList)[iFilepos].iName, &KFilesMMCVideo, NULL);
				filesize = (*dirList)[iFilepos].iSize;
			}
			else
			{
				return; //Error 
			}
			imageFile.Copy(fullentry.FullName());
			CleanupStack::PopAndDestroy();		// delete dirList

			iPublic_Photo=EFalse;
			iAppUi.AddVideoRecord(imageFile, iPublic_Photo, comment, description, iNewFolder, filesize);

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

			ChangeStatus(EWritten);
			Notify();
		}
		break;
	case EWritten:
		{
		 iAppUi.SendToBackground();
		 ChangeStatus(ECheckDisk);
		 if( icompareflag1 == 1)
		 {
			iRFsession.NotifyChange(ENotifyFile,iStatus,KDirPhoneVideo);
		 }
		 else if(icompareflag1 == 2)
		 {
			iRFsession.NotifyChange(ENotifyFile,iStatus,KDirMMCVideo);
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
#endif

void CVideosEngine::Notify()
{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
    if (!IsActive())
    {
	    SetActive();
    }
}

/*
void CVideosEngine::SetServerName(const TDesC& aName)
    {
    iServerName.Copy(aName);
    }

const TDesC& CVideosEngine::ServerName() const
    {
    return iServerName;
    }

void CVideosEngine::SetPort(TInt aPort)
    {
    iPort = aPort;
    }

TInt CVideosEngine::Port() const
    {
    return iPort;
    }

TBool CVideosEngine::Connected() const
    {
    	return ETrue;
    }
*/   
void CVideosEngine::ChangeStatus(TSocketsEngineState aEngineStatus)
{
	iEngineStatus=aEngineStatus;
}
void CVideosEngine::GetFolder()
{
#ifndef __UIQ__
	TInt PopValue;	

	CDesCArray* items = iAppUi.GetFolderList();	
    CDesCArray* foldernames = iAppUi.ReturnText();

	if (items == NULL)
	{
		Count = 0;
		items = new (ELeave) CDesCArrayFlat(1);
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
#endif //__UIQ__
}

#ifdef __UIQ__
void CVideosEngine::TimerExpired()
{
	Notify();
	//iTimer->After(KTimeOut);
}

void CVideosEngine::GetTimeStamp()
{
	CDir *dirList;
	// get the dirs contents
	TInt err;
	if( icompareflag1 == 1)
	{
		err = iRFsession.GetDir(KFilesPhoneVideo, KEntryAttNormal, ESortByDate, dirList);
	}
	else if(icompareflag1 == 2)
	{
		err = iRFsession.GetDir(KFilesMMCVideo, KEntryAttNormal, ESortByDate, dirList);
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
		if( icompareflag1 == 1)
		{
			fullentry.Set((*dirList)[dirList->Count()-1].iName, &KFilesPhoneVideo, NULL);
		}
		else if(icompareflag1 == 2)
		{
			fullentry.Set((*dirList)[dirList->Count()-1].iName, &KFilesMMCVideo, NULL);	
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


TInt CVideosEngine::CheckCount()
{
	CDir* dirList;
	// get the dirs contents			
	TInt err;
	TInt count;
#ifdef __UIQ__
	if( icompareflag1 == 1)
	{
		err = iRFsession.GetDir(KFilesPhoneVideo, KEntryAttNormal, ESortByDate, dirList); 
	}
	else if(icompareflag1 == 2)
	{
		err = iRFsession.GetDir(KFilesMMCVideo, KEntryAttNormal, ESortByDate, dirList);
	}
	else if( icompareflag1 == 3)
	{
		err = iRFsession.GetDir(KFilesPhoneVideoSony, KEntryAttNormal, ESortByDate, dirList); 
	}
	else if(icompareflag1 == 4)
	{
		err = iRFsession.GetDir(KFilesMMCVideoSony, KEntryAttNormal, ESortByDate, dirList);
	}
	else
	{
		return 0; //Error 
	}
#else
	if( icompareflag1 == 1)
	{
		err = iRFsession.GetDir(KFilesPhoneVideo, KEntryAttNormal, ESortByDate, dirList);
	}
	else if(icompareflag1 == 2)
	{
		err = iRFsession.GetDir(KFilesMMCVideo, KEntryAttNormal, ESortByDate, dirList);
	}
	else
	{
		return 0; //Error 
	}
#endif
	CleanupStack::PushL(dirList);			
	
	count = dirList->Count();

	CleanupStack::PopAndDestroy();		// delete dirList

	return count;
}


void CVideosEngine::ProcessImage()
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
