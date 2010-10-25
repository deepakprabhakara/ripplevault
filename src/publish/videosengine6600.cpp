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
 Module Name				: VideosEngine6600.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   : Videos Engine for Symbian 7 & 8
 

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
/*
#ifdef __UIQ__
	//p800 need to change 
	_LIT(KDir1,"c:\\Nokia\\Videos");
	_LIT(KDirImageFolder,"c:\\Nokia\\Videos\\*");
	_LIT(KDefaultServerName, "61.95.206.73");
#else //3650
	_LIT(KDirImageFolder1,"c:\\Nokia\\Videos\\*");
	_LIT(KDirImageFolder2,"e:\\Videos\\*");
	_LIT(KDir1,"c:\\Nokia\\Videos");
	_LIT(KDir2,"e:\\Videos");
#endif
*/
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
  //,
  //iPort(KDefaultPortNumber)
{
}

CVideosEngine::~CVideosEngine()
{
    iRFsession.NotifyChangeCancel(iStatus);
    Cancel();	
	iRFsession.Close();
}

void CVideosEngine::ConstructL(TDesC& aFolderName)
{
	User::LeaveIfError(iRFsession.Connect());    
	
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

	if (BaflUtils::FileExists(iRFsession,aFolderName))
	{
		//iRFsession.NotifyChange(ENotifyWrite,iStatus,aFolderName);
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
}

// from CActive
void CVideosEngine::RunL()
{
	_LIT8(GPPEXT,".3gp");
	_LIT8(MP4, ".mp4");
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

			TParse fullentry;
			TBuf8<100> tempp;

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

				if(sec1 < 5)// && sec1 >= 0)
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
			TBuf8<100> tempp;
			
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

			if(tempp.Find(GPPEXT)==KErrNotFound && tempp.Find(MP4) == KErrNotFound)
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
						return;			
					TParse fullentry;
					
					//////
					TBuf<160> imageFile;
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
				//iAppUi.HideView();
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
				}
				else
				{
					ChangeStatus(EWritten);				
				}*/
			}
				ChangeStatus(EOptionChosen);
				Notify();
			/*}
			else if(intrusionmode==MODE_AUTO)// || intrusionmode==MODE_AUTO)
			{
				ChangeStatus(EOptionChosen);
				Notify();
			}*/			
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

					//iAppUi.SendToBackground();
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

				//iAppUi.SendToBackground();
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
				//ChangeStatus(EWritten);
				ChangeStatus(ECheckFile);

				//iAppUi.SendToBackground();
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
			tempp1.Copy(fullentry.FullName());

			CleanupStack::PopAndDestroy();		// delete dirList
			iPublic_Photo=EFalse;
			iAppUi.AddVideoRecord(imageFile, iPublic_Photo, comment, description, iNewFolder, filesize);

			//iAppUi.ScheduleToSend();			
			ChangeStatus(EFileOpen);

			iAppUi.SendToBackground();
			Notify();
		}
		break;
		case EFileOpen:
			TInt err;
			//TInt network;
			err = TempFile.Open(iRFsession, tempp1, EFileRead|EFileStream);
			if (err == KErrNone)
			{
				TempFile.Close();				
				
				iAppUi.ScheduleToSend();

				/*network = iAppUi.CheckNetwork();

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
			else
			{
				ChangeStatus(EFileOpen);
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

TInt CVideosEngine::CheckCount()
{
	CDir* dirList;
	// get the dirs contents			
	TInt err;
	TInt count;
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
