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
 Module Name				: MailIdListForm.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/


#include <aknform.h>
#include <eikenv.h>
#include <eikmenub.h>
#include <eikappui.h>
#include <AknQueryValue.h>
#include <aknpopupfield.h>
#include <avkon.hrh>
#include <avkon.rsg>
#include <aknwaitdialog.h> 
#include <ripplevault.rsg>
#include "ripplevault.hrh"
#include <eikmenup.h>
#include <aknquerydialog.h>
#include <aknnotewrappers.h> 
#include "ripplevaultappui.h"
#include "MailIdListForm.h"

CMailIdListForm::~CMailIdListForm()
{
	if(iTextArray[0])
	{
		delete iTextArray[0];
		iTextArray[0]=NULL;
	}
	if(iValueTextArray[0])
	{
		delete iValueTextArray[0];
		iValueTextArray[0]=NULL;
	}
	if(iTextValues[0])
	{
		delete iTextValues[0];
		iTextValues[0]=NULL;
	}
}
	


CMailIdListForm::CMailIdListForm()
{
	iTextValues[0] = NULL;	
}

void CMailIdListForm::ConstructL()
{
	CAknForm::ConstructL();	
}

void CMailIdListForm::PreLayoutDynInitL()
{
	SetEdwinTextL(EmRecepientId, &(iPhotosAppUi->MailId));
	//SetEdwinTextL(EmRecepientId, &(iPhotosAppUi->MailNames));
}

void CMailIdListForm::PostLayoutDynInitL()
{	
	CAknForm::PostLayoutDynInitL();	
}

void CMailIdListForm::ProcessCommandL(TInt aCommandId)
{
	switch(aCommandId)
	{
	case EAddRecp:
		GetEdwinText(iPhotosAppUi->MailId, EmRecepientId);
		iPhotosAppUi->ActivateView(KViewIdEmptyScreen);
		iPhotosAppUi->HandleCommandL(EFindView);		
		break;
	case EAknSoftkeyDone:		
		//iPhotosAppUi->HandleCommandL(ERegView);
		break;
	case ERecpCancel:
		iPhotosAppUi->MailNames.Copy(_L(""));
		iPhotosAppUi->MailId.Copy(_L(""));
		iPhotosAppUi->albumName.Copy(_L(""));
		iPhotosAppUi->iSharingCount=0;

		iPhotosAppUi->ActivateView(KViewIdEmptyScreen);
		//iPhotosAppUi->ActivateView(4);
		if(iPhotosAppUi->iStoragePath == 1)
			iPhotosAppUi->ActivateView(KViewIdSelectiveSms);
		else if(iPhotosAppUi->iStoragePath==2){
			iPhotosAppUi->ActivateView(KViewIdStatusScreen);
		}
		else if(iPhotosAppUi->iStoragePath==3)
			iPhotosAppUi->ActivateView(KViewIdStatusScreen);
		else
		{
			iPhotosAppUi->ActivateView(KViewIdStatusScreen);
			iPhotosAppUi->ListAlbums();
		}
		break;
	default:
		CAknForm::ProcessCommandL(aCommandId);
		break;
	}
}

void CMailIdListForm::DynInitMenuPaneL( 
     TInt aResourceId, 
     CEikMenuPane* aMenuPane )
    {
    CAknForm::DynInitMenuPaneL( aResourceId, aMenuPane );
    if ( aResourceId == R_AVKON_FORM_MENUPANE )
        {
        aMenuPane->AddMenuItemsL( R_MAILID_EXTRA_MENU);
		aMenuPane->SetItemDimmed( EAknFormCmdLabel, ETrue );
		aMenuPane->SetItemDimmed(EAknFormCmdAdd, ETrue );
		aMenuPane->SetItemDimmed( EAknFormCmdDelete, ETrue );
		aMenuPane->SetItemDimmed( EAknFormCmdSave, ETrue );
        }
    }

TBool CMailIdListForm::OkToExitL( TInt aButtonId )
	{
		if(aButtonId == 3007)
		{			
			if(ControlOrNull(EmRecepientId))
			{
				GetEdwinText(iPhotosAppUi->MailId, EmRecepientId);
			}				

			TBuf<200> message;
			message.Copy(_L(""));

			if(ControlOrNull(EmRecepientMessage))
			{
				GetEdwinText(message, EmRecepientMessage);
			}			

			if(iPhotosAppUi->iSharingCount == 0)
			{
				CAknInformationNote* note = new(ELeave) CAknInformationNote;
				note->ExecuteLD(*(iEikonEnv->AllocReadResourceL(R_SELECT_RECEPIENT)));
				return CAknForm::OkToExitL(-1);
			}

			if(iPhotosAppUi->albumName.Compare(message) == 0)
			{
				CAknWaitDialog* SmsDialog;
	
				SmsDialog = new(ELeave)CAknWaitDialog( (REINTERPRET_CAST(CEikDialog**,&SmsDialog)));
				SmsDialog->SetTone( CAknNoteDialog::EConfirmationTone ); 
				SmsDialog->SetTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SHARING_APPLICATION_PROGRESS)));
				SmsDialog->ExecuteLD(R_WAITNOTE_DIALOG);

				iPhotosAppUi->SaveMails(iPhotosAppUi->albumName,iPhotosAppUi->MailNames,iPhotosAppUi->MailId,message, iPhotosAppUi->iSharingCount);
				//iPhotosAppUi->GetClientSharingAlbumXML();
				iPhotosAppUi->SendSmsForClientSharing();

				SmsDialog->ProcessFinishedL();
				
				iPhotosAppUi->ShowMessageBoxFromResource(R_TEXT_SHARING_APPLICATION_SUCCESS);

				iPhotosAppUi->ActivateView(KViewIdStatusScreen );
			}
			else
			{			
				//CAknTextQueryDialog* dlg = CAknTextQueryDialog::NewL(message, CAknQueryDialog::ENoTone );
				//if(dlg->ExecuteLD(R_SHARING_OPTION))			
				//{
				iPhotosAppUi->SaveMails(iPhotosAppUi->albumName,iPhotosAppUi->MailNames,iPhotosAppUi->MailId,message, iPhotosAppUi->iSharingCount);
				iPhotosAppUi->GetSharingAlbumXML();				
				//}				
			}

		}
		return CAknForm::OkToExitL( aButtonId );
	}

TBool CMailIdListForm::SaveFormDataL()
{	
	return ETrue;
}

void CMailIdListForm::DoNotSaveFormDataL()
{

}

TBool CMailIdListForm::QuerySaveChangesL()
{
	SaveFormDataL();
    return 1;
}
