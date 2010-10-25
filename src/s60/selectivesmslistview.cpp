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
 Module Name				: SelectiveSmsListView.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 10-05-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Screen to allow selection of recipients for Selective SMS
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/
 
//INCLUDE FILES
#include  <aknlistquerydialog.h> 
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <RippleVault.rsg>
#include  <aknmessagequerydialog.h>
#include  "SelectiveSmsListView.h"
#include  "SelectiveSmsListContainer.h" 
#include  "RippleVault.hrh"

#include <aknnotewrappers.h> 

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CSelectiveSmsListView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------

void CSelectiveSmsListView::ConstructL()
    {
		BaseConstructL( R_MANUALSMSLIST_VIEW );
    }

// ---------------------------------------------------------
// CSelectiveSmsListView::~CSelectiveSmsListView()
// ?implementation_description
// ---------------------------------------------------------

CSelectiveSmsListView::~CSelectiveSmsListView()
    {
		if ( iContainer )
			{
			AppUi()->RemoveFromViewStack( *this, iContainer );
			}

		delete iContainer;
    }

// ---------------------------------------------------------
// TUid CSelectiveSmsListView::Id()
// ?implementation_description
// ---------------------------------------------------------

TUid CSelectiveSmsListView::Id() const
    {
    return KViewIdSelectiveSms;
    }

// ---------------------------------------------------------
// CSelectiveSmsListView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------

void CSelectiveSmsListView::HandleCommandL(TInt aCommand)
    {   
//	TInt path;
//	TInt count;

    switch ( aCommand )
        {
		case EMarkSel:
			{
				iPhotosAppUi->MailNames.Copy(_L(""));
				iPhotosAppUi->MailId.Copy(_L(""));
				iPhotosAppUi->albumName.Copy(_L(""));
				iPhotosAppUi->iSharingCount=0;

				iPhotosAppUi->ActivateView(KViewIdFindBox);
				break;
			}
        case ESendSel:
            {
				iContainer->DeleteContacts();
				break;
            }
		case ERecpCancel:
			{
				TInt8 choice=0;
	
				iPhotosAppUi->iSmsMode = 1;
				iPhotosAppUi->SaveSmsSettingsToDB(iPhotosAppUi->iSmsMode,iPhotosAppUi->iSmsTime,iPhotosAppUi->iSmsRoam);

				iPhotosAppUi->GetChoice(choice);
				//iPhotosDB->GetChoice(choice);

				iPhotosAppUi->iStoragePath = 0; 
				//iPhotosAppUi->ActivateView(13);
				iPhotosAppUi->iSettingsScreenValue = 3;

				//if(choice==0)
				//{
				//	iPhotosAppUi->ActivateView(KViewIdSetupWizard);
				//}
				//else{
					iPhotosAppUi->ActivateView(KViewIdSettingsScreen);
				//}
			}
			break;
		case EAknSoftkeyDone:
            {
				if(iContainer->GetStoredContactsCount() == 0)
				{
					CAknInformationNote* note = new(ELeave) CAknInformationNote;
					note->ExecuteLD(*(iEikonEnv->AllocReadResourceL(R_SELECT_RECEPIENT)));
				}
				else
				{
					TInt8 choice=0;
		
					iPhotosAppUi->GetChoice(choice);
					//iPhotosDB->GetChoice(choice);

					iPhotosAppUi->iStoragePath = 0; 
					//iPhotosAppUi->ActivateView(13);
					iPhotosAppUi->iSettingsScreenValue = 3;

					//if(choice==0)
					//{
					//	iPhotosAppUi->ActivateView(KViewIdSetupWizard);
					//}else{
						iPhotosAppUi->ActivateView(KViewIdSettingsScreen);
					//}
				}
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
// CSelectiveSmsListView::HandleClientRectChange()
// ---------------------------------------------------------

void CSelectiveSmsListView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CSelectiveSmsListView::DoActivateL(...)
// ?implementation_description
// ---------------------------------------------------------

void CSelectiveSmsListView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CSelectiveSmsListContainer;
        iContainer->SetMopParent(this);
		iContainer->iPhotosAppUi=iPhotosAppUi;
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );		
        }
   }

// ---------------------------------------------------------
// CSelectiveSmsListView::HandleCommandL(TInt aCommand)
// implementation_description
// ---------------------------------------------------------

void CSelectiveSmsListView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }

// End of File 