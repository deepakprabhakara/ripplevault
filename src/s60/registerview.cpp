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
 Module Name				: RegisterView.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 17-06-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     Status Screen for S60
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/

// INCLUDE FILES
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <ripplevault.rsg>
#include  <in_sock.h> 
#include  <aknlistquerydialog.h> 
#include  <eikmenup.h>

#include "RippleVaultAppUi.h"
#include  "RegisterView.h"
#include  "RegisterContainer.h" 
#include  "RippleVault.hrh"
#include  "MailIdListForm.h"

#include <aknmessagequerydialog.h>

#include <bautils.h>

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCRegisterView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CRegisterView::ConstructL()
    {
    BaseConstructL( R_REG_VIEW);
    }

// ---------------------------------------------------------
// CCRegisterView::~CCRegisterView()
// ?implementation_description
// ---------------------------------------------------------
//
CRegisterView::~CRegisterView()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CCRegisterView::Id()
// ?implementation_description
// ---------------------------------------------------------
//
TUid CRegisterView::Id() const
    {
    return KViewIdStatusScreen;
    }

// ---------------------------------------------------------
// CCRegisterView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------
//
void CRegisterView::HandleCommandL(TInt aCommand)
    {   
	//TInt Mode;
	//TBuf<128> addr;
	//TInt Port;
	//TBuf<20> buf;
	//TInt desc;
	//TInt roam;
	//TInt autostart;

    switch ( aCommand )
        {
		case EViewAlbum:
			{
				iPhotosAppUi->OpenBrowser();
			}
		break;
		case EChangePassword:
			{
				TBuf<50> oldpass;
				TBuf<50> newpass;

				TInt flag = 2;
				TInt check;
					do
					{
						CAknMultiLineDataQueryDialog* dialog =	CAknMultiLineDataQueryDialog::NewL(oldpass, newpass);
						check=dialog->ExecuteLD(R_CHANGE_PASSWORD_DIALOG);
						if(check)
						{					
							if(oldpass.Compare(iPhotosAppUi->iPassword) == 0 && newpass.Length() >= 6)
							{
								flag = 1;
								iPhotosAppUi->iNewPassword.Copy(newpass);
								iPhotosAppUi->ChangePassword();
							}
							else{
								iPhotosAppUi->ShowMessageBoxFromResource(R_TEXT_INVALID_PASSWORD);	
							}
						}
						else
							flag = 1;
					}
					while(flag == 2);
			}
			break;
		case EShareAlbum:
			{
				//share album
				TInt ret;
				ret = iPhotosAppUi->ListAlbums();
				if(ret)
				{
					iPhotosAppUi->MailNames.Copy(_L(""));
					iPhotosAppUi->MailId.Copy(_L(""));
					//iPhotosAppUi->albumName.Copy(_L(""));
					iPhotosAppUi->iSharingCount=0;

					CMailIdListForm* iform= new (ELeave) CMailIdListForm;
					CleanupStack::PushL( iform );
					iform->iPhotosAppUi = iPhotosAppUi;
					iform->ConstructL();
					CleanupStack::Pop();
					iform->ExecuteLD(R_SHAREALBUM_DIALOG);
				}
			}
			break;
/*
		case EStart:
            {	
				iPhotosAppUi->GetSettingsFromDB(addr, Port, autostart);
				iPhotosAppUi->GetPhotosSettingsFromDB(Mode, desc, roam);


				Mode = MODE_PROMPT;     		
				iPhotosAppUi->SaveSettingsToDB(addr, (TUint16)Port, (TInt8)autostart);		
				iPhotosAppUi->SavePhotosSettingsToDB((TInt8)Mode, (TInt8)desc, (TInt8)roam);
				//buf.Copy(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RUNNING)));
				iContainer->ApplicationStatus(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RUNNING)));

				iPhotosAppUi->EnableUpload(REGISTERED);	
				break;
            }
		case EStop:
            {
				iPhotosAppUi->GetSettingsFromDB(addr, Port, autostart);
				iPhotosAppUi->GetPhotosSettingsFromDB(Mode, desc, roam);


				Mode = MODE_OFF;     		
				iPhotosAppUi->SaveSettingsToDB(addr, (TUint16)Port, (TInt8)autostart);		
				iPhotosAppUi->SavePhotosSettingsToDB((TInt8)Mode, (TInt8)desc, (TInt8)roam);
				//buf.Copy(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_STOPPED)));
				iContainer->ApplicationStatus(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_STOPPED)));

				iPhotosAppUi->EnableUpload(MODE_OFF);	
				break;
            }		    
		case EShare:
			{
				iPhotosAppUi->ListAlbums();				
			}
			break;
		case EShareClient:
			{
				//iPhotosAppUi->ListAlbums();
				iPhotosAppUi->albumName.Copy(_L(""));
				iPhotosAppUi->ActivateView(6);
			}
			break;
		case EImagesPh:
			{
				iPhotosAppUi->iRipplePhotos = 2;
				iPhotosAppUi->iStoragePath = 0;
				iPhotosAppUi->ActivateView(5);				
				break;
			}*/
		/*case EImagesMmc:
			{
				iPhotosAppUi->iStoragePath = 1;
				iPhotosAppUi->ActivateView(5);				
				break;
			}*/
/*		case EVideosPh:
			{
				iPhotosAppUi->iRipplePhotos = 2;
				iPhotosAppUi->iStoragePath = 2;
				iPhotosAppUi->ActivateView(5);				
				break;
			}*/
		/*case EVideosMmc:
			{
				iPhotosAppUi->iStoragePath = 3;
				iPhotosAppUi->ActivateView(5);				
				break;
			}*/
/*		case ESettings:
            {
				AppUi()->HandleCommandL(ESettings);
				break;
            }
		case Esmsbackup:
            {
				iPhotosAppUi->iStoragePath = 0;
				AppUi()->HandleCommandL(Esmsbackup);
				break;
            }
        // case EAknSoftkeyBack:*/
        case EMinimize:
            {
				iPhotosAppUi->CaseMinimize();
				break;
            }
/*		case ESendToWeb:
			{
				if(iContainer->iPendingCountContacts>0)
				{
					iPhotosAppUi->UpdateContacts(0);
				}
				if(iContainer->iPendingCountPhotos>0 || iContainer->iPendingCountVideos>0)
				{
					iPhotosAppUi->ScheduleToSend();

				}
				
			}
			break;*/
/*		case EAboutErrorCode:
			{
				if(iContainer)
				if(iContainer->isError)
				{
					switch(iContainer->iErrCode)
					{
						case NETWORK_ERRCODE_PARSING:
							iPhotosAppUi->ShowMessageBoxFromResource(R_TEXT_NETWORK_ERRCODE_PARSING);
							break;
						case NETWORK_ERRCODE_FAILED:
							iPhotosAppUi->ShowMessageBoxFromResource(R_TEXT_NETWORK_ERRCODE_FAILED);
							break;
						case NETWORK_ERRCODE_TIMEDOUT:
							iPhotosAppUi->ShowMessageBoxFromResource(R_TEXT_NETWORK_ERRCODE_TIMEDOUT);
							break;
						case NETWORK_ERRCODE_MSISDNMISMATCH:
							iPhotosAppUi->ShowMessageBoxFromResource(R_TEXT_NETWORK_ERRCODE_MSISDNMISMATCH);
							break;
						case NETWORK_ERRCODE_IMEIMISMATCH:
							iPhotosAppUi->ShowMessageBoxFromResource(R_TEXT_NETWORK_ERRCODE_IMEIMISMATCH);
							break;
						case NETWORK_ERRCODE_ACCSUSPENDED:
							iPhotosAppUi->ShowMessageBoxFromResource(R_TEXT_NETWORK_ERRCODE_ACCSUSPENDED);
							break;
						default:
							break;
					}			
				}
			}
			break;*/
		case EHelp:
			{
				AppUi()->HandleCommandL(EHelp);
				//iPhotosAppUi->ShowMessageBoxFromResource(R_STATUSTEXT_HELP);
			}
			break;
		case ESuspend:
			{
				iPhotosAppUi->CaseSuspend();
			}
			break;
/*		case EShowUploadCount:
			{
				TBuf<250> uploadcount;

				TBuf<50> message;
				TBuf<50> formatBuf;
				TInt album;
				TInt photos;
				TInt videos;
				TInt albumshares;
				//TInt photoshares;
				//TInt videoshares;
				TInt ccount;
				TInt smscount;
				
				iPhotosAppUi->GetUploadSettingsFromDB(album, photos, videos, albumshares);
				iPhotosAppUi->GetContactsUploadSettingsFromDB(ccount);
				iPhotosAppUi->GetSmsUploadSettingsFromDB(smscount);

				message.Copy(_L(""));
				formatBuf.Copy(_L(""));
				formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_UPLOADINFO_CONTACTS_UPLOADED)));
				message.Format(formatBuf, ccount);
				
				uploadcount.Append(message);
				uploadcount.Append(_L("\n"));

				message.Copy(_L(""));
				formatBuf.Copy(_L(""));
				formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_UPLOADINFO_SMS_UPLOADED)));
				message.Format(formatBuf, smscount);

				uploadcount.Append(message);
				uploadcount.Append(_L("\n"));
				
				message.Copy(_L(""));
				formatBuf.Copy(_L(""));
				formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_UPLOADINFO_ALBUMS)));
				message.Format(formatBuf, album, albumshares);

				uploadcount.Append(message);
				uploadcount.Append(_L("\n"));

				message.Copy(_L(""));
				formatBuf.Copy(_L(""));
				formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_UPLOADINFO_PHOTOS_UPLOADED)));
				message.Format(formatBuf, photos);

				uploadcount.Append(message);
				uploadcount.Append(_L("\n"));

				message.Copy(_L(""));
				formatBuf.Copy(_L(""));
				formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_UPLOADINFO_VIDEOS_UPLOADED)));
				message.Format(formatBuf, videos);

				uploadcount.Append(message);
				//uploadcount.Append(_L("\n"));

				CAknMessageQueryDialog* verdlg = CAknMessageQueryDialog::NewL(uploadcount);
				verdlg->PrepareLC(R_ABOUT_QUERY);
				verdlg->RunLD();
			}
			break;*/
		case EUpgradeClient:
			{
				iPhotosAppUi->CopyUpgradeFile();
			}
			break;
		case EBackupPhoneBook:
			{
				iPhotosAppUi->UpdateContactsManually();
			}
			break;
		case EBackupPhotos:
			{
				iPhotosAppUi->iRipplePhotos = 2;
				iPhotosAppUi->iStoragePath = PHOTOS_PHONEMEMORY;
				iPhotosAppUi->ActivateView(KViewIdPhotoList );
			}
			break;
		case EBackupVideos:
			{
				iPhotosAppUi->iRipplePhotos = 2;
				iPhotosAppUi->iStoragePath = VIDEOS_PHONEMEMORY;
				iPhotosAppUi->ActivateView(KViewIdPhotoList );
			}
			break;
		case EBackupSMS:
			{
				if(iPhotosAppUi->PendingSMS())
				{
					iPhotosAppUi->ShowMessageBoxFromResource(R_SMS_SENDTOWEB_BUSY);
				}
				else{
					//manually sms backup		
					iPhotosAppUi->iStoragePath = SMS_INBOX;
					iPhotosAppUi->ActivateView(KViewIdSmsList );
				}
			}
			break;
		case ESettingsPhoneBook:
			{
				iPhotosAppUi->iSettingsScreenValue = 1;
				iPhotosAppUi->ActivateView(KViewIdSettingsScreen );
			}
			break;
		case ESettingsPhotos:
			{
				iPhotosAppUi->iSettingsScreenValue = 2;
				iPhotosAppUi->ActivateView(KViewIdSettingsScreen );
			}
			break;
		case ESettingsSMS:
			{
				iPhotosAppUi->iSettingsScreenValue = 3;
				iPhotosAppUi->ActivateView(KViewIdSettingsScreen );
			}
			break;
		case ESettingsAntitheft:
			{
				iPhotosAppUi->iSettingsScreenValue = 4;
				iPhotosAppUi->ActivateView(KViewIdSettingsScreen);
			}
			break;
		case ESettingsGeneral:
			{
				iPhotosAppUi->iSettingsScreenValue = 0;
				iPhotosAppUi->ActivateView(KViewIdSettingsScreen);
			}
			break;
		case EShareClient:
			{
				iPhotosAppUi->MailNames.Copy(_L(""));
				iPhotosAppUi->MailId.Copy(_L(""));
				iPhotosAppUi->albumName.Copy(_L(""));
				iPhotosAppUi->iSharingCount=0;
				iPhotosAppUi->iStoragePath = 2;
				//iPhotosAppUi->ActivateView(6);
				DisplayFormL();
			}
			break;
		case EClearMediaQueue:
			{
				iPhotosAppUi->ClearMediaQueue();
			}
			break;
/*		case EExit:
            {
				AppUi()->HandleCommandL(EExit);
				break;
            }
        default:
            {
				AppUi()->HandleCommandL( aCommand );
				break;
            }*/
        }
    }

// ---------------------------------------------------------
// CCRegisterView::HandleClientRectChange()
// ---------------------------------------------------------
//
void CRegisterView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

void CRegisterView::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
{
	//if(aResourceId == R_REG_APP_MENU)
	if(aResourceId == R_CASCADE_OTHER_LIST)
	{
/*		TBool pending;

		pending = EFalse;

		if(((iContainer->iPendingCountContacts + iContainer->iPendingCountPhotos +
			iContainer->iPendingCountVideos) > 0) && (iContainer->iStatusCode == NETWORK_STATUS_RUNNING || iContainer->iStatusCode == NETWORK_STATUS_FAILED))
			pending = ETrue;

		if(!iContainer->isQueuedInfo)
			pending = EFalse;
*/
		//aMenuPane->SetItemDimmed(ESendToWeb, !pending);

		//aMenuPane->SetItemDimmed(EAboutErrorCode, !iContainer->isError);

		aMenuPane->SetItemDimmed(EUpgradeClient, !iContainer->isUpgrade);

		TBool queued;

		if(iPhotosAppUi->iPhotosDB->PhotosRowCount() == 0 && iPhotosAppUi->iPhotosDB->VideosRowCount() == 0)
			queued = ETrue;
		else
			queued = EFalse;

		aMenuPane->SetItemDimmed(EQueueList, queued );

		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KResumeFile);

		RFs fs1;

		fs1.Connect();
		
		if (BaflUtils::FileExists(fs1, path)) //suspended
			aMenuPane->SetItemTextL(ESuspend, R_TEXT_SUSPEND);
		else
			aMenuPane->SetItemTextL(ESuspend, R_TEXT_RESUME);

		fs1.Close();

		//aMenuPane->SetItemDimmed(EShowUploadCount, !iContainer->isQueuedInfo);

		//if(!pending && !iContainer->isError)
		//	aMenuPane->SetItemTextL(EStatusScreenOptions, R_STATUSTEXT_OPTIONS);
		//else
		//	aMenuPane->SetItemTextL(EStatusScreenOptions, R_STATUSTEXT_EMPTYTEXT);
	}
}

// ---------------------------------------------------------
// CCRegisterView::DoActivateL(...)
// ?implementation_description
// ---------------------------------------------------------
//
void CRegisterView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
     {
        iContainer = new (ELeave) CRegisterContainer;
        iContainer->SetMopParent(this);
		iContainer->iPhotosAppUi=iPhotosAppUi;
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );

		//iPhotosAppUi->ShowMessage();

		if( iPhotosAppUi->iSmsValue == 1)
		{
			iPhotosAppUi->iSmsValue = 3;
			iPhotosAppUi->ExitApplication();		
		}

		TInt8 choice = 0;
		iPhotosAppUi->GetChoice(choice);

		if(choice==0){ 
			iPhotosAppUi->CallSetupWizardUploadAll();
			}
		}
	}

// ---------------------------------------------------------
// CCRegisterView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------
//
void CRegisterView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }

// End of File
/*
void CRegisterView::UpdateScreen()
{
	iContainer->UpdateScreen();
}
*/
void CRegisterView::DisplayFormL()
{
	CMailIdListForm* iform= new (ELeave) CMailIdListForm;
	CleanupStack::PushL( iform );
	iform->iPhotosAppUi = iPhotosAppUi;
    iform->ConstructL();
    CleanupStack::Pop();

	iform->ExecuteLD(R_MAILID_DIALOG);
}

/*
void CRegisterView::CaseMinimize()
{
	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KResumeFile);

	RFs fs1;

	fs1.Connect();
	
	TBool isSuspended = EFalse;

	if (BaflUtils::FileExists(fs1, path)) //resume
		isSuspended = EFalse;	
	else
		isSuspended = ETrue;

	fs1.Close();

	if(isSuspended)
		AppUi()->HandleCommandL(EExit);
	else
		AppUi()->HandleCommandL(EHide);
	//AppUi()->HandleCommandL(EExit);
}

void CRegisterView::CaseSuspend()
{
	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KResumeFile);

	RFs fs1;

	fs1.Connect();
	
	if (BaflUtils::FileExists(fs1, path)) //suspended
	{
		//delete file
		//fs1.Delete(KRegStep1);
		BaflUtils::DeleteFile(fs1, path);

		iPhotosAppUi->SaveSettingsToDB(ENABLED);
		iPhotosAppUi->SavePhotosSettingsToDB(PHOTOSBACKUP_PROMPT);
		iPhotosAppUi->SaveSmsSettingsToDB(SMSBACKUP_MANUAL);
	}
	else
	{
		RFile file;
		TInt err = file.Replace(fs1,path ,EFileWrite);

		if (err == KErrNone) {
			file.Close();
		}

		iPhotosAppUi->SaveSettingsToDB(DISABLED);
		iPhotosAppUi->SavePhotosSettingsToDB(PHOTOSBACKUP_MANUAL);
		iPhotosAppUi->SaveSmsSettingsToDB(SMSBACKUP_MANUAL);
	}
	fs1.Close();
}
*/
