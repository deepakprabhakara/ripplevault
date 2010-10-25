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
 Module Name				: FindBoxView.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Displays list of contacts and a findbox for sharing album, tell a friend features, selective sms
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

// INCLUDE FILES
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <ripplevault.rsg>
#include  "ripplevault.hrh"
#include  "FindBoxView.h"
#include  "RippleVaultAppUi.h"
#include  "FindBoxContainer.h" 
#include  "MailIdListForm.h"
#include <aknquerydialog.h>
#include <aknnotewrappers.h> 

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CFindBoxView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CFindBoxView::ConstructL()
    {
    BaseConstructL( R_FINDBOX_VIEW);
    }

// ---------------------------------------------------------
// CFindBoxView::~CFindBoxView()
// ?implementation_description
// ---------------------------------------------------------
//
CFindBoxView::~CFindBoxView()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CFindBoxView::Id()
// ?implementation_description
// ---------------------------------------------------------
//
TUid CFindBoxView::Id() const
    {
    return KViewIdFindBox;
    }

// ---------------------------------------------------------
// CFindBoxView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------
//
void CFindBoxView::HandleCommandL(TInt aCommand)
    {   

    switch ( aCommand )
        {
        case EMail:
            {				
				iContainer->OkPressKey();				
				break;
            }
		case EMobile:
            {				
				iContainer->OkPressKey();
				break;
            }
        //case EAknSoftkeyDone:
		case EFindOk:
            {				
				if(iPhotosAppUi->iStoragePath == 1)
				{
					if(iContainer->GetSelectedCount() == 0)
					{
						CAknInformationNote* note = new(ELeave) CAknInformationNote;
						//TBuf<200> msg;
						//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_SELECT_RECEPIENT)));
						note->ExecuteLD(*(iEikonEnv->AllocReadResourceL(R_SELECT_RECEPIENT)));
						return;
					}
				
					if(iContainer->GetSelectedCount() > 10)
					{
						CAknInformationNote* note = new(ELeave) CAknInformationNote;
						note->ExecuteLD(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_RECEPIENT)));
						return;
					}

					iContainer->SaveContacts();

					iPhotosAppUi->SaveSelectiveSmsMails(iPhotosAppUi->MailNames,iPhotosAppUi->MailId, iPhotosAppUi->iSharingCount);

					iPhotosAppUi->ActivateView(KViewIdSelectiveSms);

					break;
				}
				
				if(iContainer->GetSelectedCount()==0)
				{
					CAknInformationNote* note = new(ELeave) CAknInformationNote;
					//TBuf<200> msg;
					//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_SELECT_RECEPIENT)));
					note->ExecuteLD(*(iEikonEnv->AllocReadResourceL(R_SELECT_RECEPIENT)));
					return;
				}

				if(iPhotosAppUi->iSharingCount > 10)
				{
					CAknInformationNote* note = new(ELeave) CAknInformationNote;
					note->ExecuteLD(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_RECEPIENT)));
					return;
				}
				
				iContainer->SaveContacts();

				DisplayFormL();

				break;
            }
		case EFindCancel:
			{
				if(iPhotosAppUi->iStoragePath == 1)
					iPhotosAppUi->ActivateView(KViewIdSelectiveSms);
				else
					DisplayFormL();
			break;        
		}    
        default:
            {
            AppUi()->HandleCommandL( aCommand );
            break;
            }
        }
    }

// ---------------------------------------------------------
// CFindBoxView::HandleClientRectChange()
// ---------------------------------------------------------
//
void CFindBoxView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CFindBoxView::DoActivateL(...)
// ?implementation_description
// ---------------------------------------------------------
//
void CFindBoxView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
			iContainer = new (ELeave) CFindBoxContainer;
			iContainer->SetMopParent(this);
			iContainer->iPhotosAppUi=iPhotosAppUi;
			iContainer->ConstructL();
			AppUi()->AddToStackL( *this, iContainer );
        } 
   }

// ---------------------------------------------------------
// CFindBoxView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------
//
void CFindBoxView::DoDeactivate()
{
    if ( iContainer )
    {
        AppUi()->RemoveFromViewStack( *this, iContainer );
    }
    
    delete iContainer;
    iContainer = NULL;
}

void CFindBoxView::DisplayFormL()
{
	CMailIdListForm* iform= new (ELeave) CMailIdListForm;
	CleanupStack::PushL( iform );
	iform->iPhotosAppUi = iPhotosAppUi;
    iform->ConstructL();
    CleanupStack::Pop();

	if(iPhotosAppUi->iStoragePath == 2)
		iform->ExecuteLD(R_MAILID_DIALOG);
	else if(iPhotosAppUi->iStoragePath==3)
		iform->ExecuteLD(R_SHAREALBUM_DIALOG);
}
// End of File

