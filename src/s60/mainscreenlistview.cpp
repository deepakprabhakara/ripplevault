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
 Module Name				: MainScreenListView.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     To Show Main screen list for S60
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/
 
//INCLUDE FILES
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <RippleVault.rsg>
#include  "MainScreenListView.h"
#include  "MainScreenListContainer.h" 
#include  "RippleVault.hrh"
#include  "MailIdListForm.h"

#include <aknquerydialog.h> 

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CMainScreenListView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------

void CMainScreenListView::ConstructL()
    {		
		BaseConstructL( R_MAINSCREEN_VIEW );	
    }

// ---------------------------------------------------------
// CMainScreenListView::~CMainScreenListView()
// ?implementation_description
// ---------------------------------------------------------

CMainScreenListView::~CMainScreenListView()
    {
		if ( iContainer )
			{
			AppUi()->RemoveFromViewStack( *this, iContainer );
			}

		delete iContainer;
    }

// ---------------------------------------------------------
// TUid CMainScreenListView::Id()
// ?implementation_description
// ---------------------------------------------------------

TUid CMainScreenListView::Id() const
    {
    return KViewIdMainScreen;
    }

// ---------------------------------------------------------
// CMainScreenListView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------

void CMainScreenListView::HandleCommandL(TInt aCommand)
    {   
    switch ( aCommand )
        {
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

		case ESettings:
            {
				AppUi()->HandleCommandL(ESettings);
				break;
            }
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
		case EMinimize:
            {
				iPhotosAppUi->iMainScreenValue = 1;
				AppUi()->HandleCommandL(EHide);
				break;
            }
		case EExit:
            {
				AppUi()->HandleCommandL(EExit);
				break;
            }
        default:
            {       
			AppUi()->HandleCommandL(aCommand);
            break;
            }
        }
    }

// ---------------------------------------------------------
// CMainScreenListView::HandleClientRectChange()
// ---------------------------------------------------------

void CMainScreenListView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CMainScreenListView::DoActivateL(...)
// ?implementation_description
// ---------------------------------------------------------

void CMainScreenListView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
{
    if (!iContainer)
    {
        iContainer = new (ELeave) CMainScreenListContainer;
        iContainer->SetMopParent(this);
		iContainer->iPhotosAppUi=iPhotosAppUi;
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );	
		
		//iPhotosAppUi->ShowSetupWizard(0);

		//iPhotosAppUi->ShowMessage();
		TInt8 choice = 0;
		iPhotosAppUi->GetChoice(choice);

		/*iPhotosAppUi->iSettingsString.Copy(_L(""));
		TBuf<60> tmpstr;
		tmpstr.Copy(_L("settings"));	
		iPhotosAppUi->LoadFromConfigFile(iPhotosAppUi->iSettingsString,tmpstr );
		*/

		if(choice==0){ 
			/*if(iPhotosAppUi->iSettingsString.Length() == 0)
			{
				if(iPhotosAppUi->iSetupWizardScreenValue == 1 || iPhotosAppUi->iSetupWizardScreenValue == 0)
					iPhotosAppUi->CallSetupWizardStep1();
				else if(iPhotosAppUi->iSetupWizardScreenValue == 5)
					iPhotosAppUi->CallSetupWizardStep5();
				else if(iPhotosAppUi->iSetupWizardScreenValue == 12)
					iPhotosAppUi->CallSetupWizardStep12();

			}
			else
			{
				iPhotosAppUi->SaveChoice(1);
				iPhotosAppUi->SaveSettingsString();
			}*/
			iPhotosAppUi->CallSetupWizardUploadAll();
		}
		
		/*if( iPhotosAppUi->iSmsValue == 2)
		{
			iPhotosAppUi->iSmsValue = 3;
			iPhotosAppUi->ExitApplication();
		}*/
    }
}

// ---------------------------------------------------------
// CMainScreenListView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------

void CMainScreenListView::DoDeactivate()
    {
     if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }

void CMainScreenListView::DisplayFormL()
{
	CMailIdListForm* iform= new (ELeave) CMailIdListForm;
	CleanupStack::PushL( iform );
	iform->iPhotosAppUi = iPhotosAppUi;
    iform->ConstructL();
    CleanupStack::Pop();

	iform->ExecuteLD(R_MAILID_DIALOG);
}
// End of File 