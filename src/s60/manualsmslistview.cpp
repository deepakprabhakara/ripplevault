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
 Module Name				: ManualsmsListView.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 1-06-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     SMS selection list for manual upload
 

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
#include  "ManualSmsListView.h"
#include  "ManualSmsListContainer.h" 
#include  "RippleVault.hrh"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CManualSmsListView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------

void CManualSmsListView::ConstructL()
    {
		BaseConstructL( R_SMSLIST_VIEW );
    }

// ---------------------------------------------------------
// CManualSmsListView::~CManualSmsListView()
// ?implementation_description
// ---------------------------------------------------------

CManualSmsListView::~CManualSmsListView()
    {
		if ( iContainer )
			{
			AppUi()->RemoveFromViewStack( *this, iContainer );
			}

		delete iContainer;
    }

// ---------------------------------------------------------
// TUid CManualSmsListView::Id()
// ?implementation_description
// ---------------------------------------------------------

TUid CManualSmsListView::Id() const
    {
    return KViewIdSmsList;
    }

// ---------------------------------------------------------
// CManualSmsListView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------

void CManualSmsListView::HandleCommandL(TInt aCommand)
    {   
//	TInt path;
//	TInt count;

    switch ( aCommand )
        {
		case EMarkSel:
			{				
				if(iContainer->iInboxCount > 0 || iContainer->iSentItemCount > 0)
					iContainer->MarkUnMarkSelectedItem();
			}
			break;
		case EMarkAll:
			{				
				if(iContainer->iInboxCount > 0 || iContainer->iSentItemCount > 0)
					iContainer->MarkAllItems();
			}
			break;
		case EUnMarkAll:
			{		
				if(iContainer->iInboxCount > 0 || iContainer->iSentItemCount > 0)
					iContainer->UnMarkAllItems();
			}
			break;
        //case ESendSel:
		//case EAknSoftkeyDone:
		case EAknSoftkeyBack:
            {
				if(iContainer->GetSelectedSmsCount())
				{
					CAknMessageQueryDialog* abtdlg;
					abtdlg= CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_CANCEL_BACKUP)));
					abtdlg->PrepareLC(R_GALLERY_INFO);
					if (abtdlg->RunLD()) 
					{
						iPhotosAppUi->ActivateView(KViewIdStatusScreen);
					}
				}
				else
					iPhotosAppUi->ActivateView(KViewIdStatusScreen);
				//iContainer->SendSelected();
				break;
            }
		case ESendAll:
			{
				//SendPhotoToServer();
				break;
			}
		//case EAknSoftkeyBack:
		case ESendSel:
            {
				TInt count=0;
				if( iPhotosAppUi->iStoragePath == SMS_INBOX)
				{
					count = iContainer->iInboxCount;
				}
				else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
				{
					count = iContainer->iSentItemCount;
				}

				if(count > 0)
					iContainer->MarkSelectedItem();

				TBuf<150> msg;
				count = iContainer->GetSelectedSmsCount();

				msg.Copy(_L(""));
				msg.AppendNum(count);

				msg.Append(*(iEikonEnv->AllocReadResourceL(R_SMS_CONFIRM)));
				msg.Append(*(iEikonEnv->AllocReadResourceL(R_IMAGES_CONFIRM)));

				CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(msg);
				dlg->PrepareLC(R_UPLOAD_ALLPHOTOS);
				if(dlg->RunLD())
				{
					iContainer->SendSelected();
				}
				/*
				//iPhotosAppUi->ActivateView(4);
				//iPhotosAppUi->ActivateView(16);
				TInt8 choice;
				iPhotosAppUi->GetChoice(choice);
				if(choice)
					iPhotosAppUi->ActivateView(KViewIdSubScreen );
				else
				{
					choice = 1;
					iPhotosAppUi->SaveChoice(choice);
					iPhotosAppUi->HandleCommandL(EMainView);
				}
				*/
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
// CManualSmsListView::HandleClientRectChange()
// ---------------------------------------------------------

void CManualSmsListView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CManualSmsListView::DoActivateL(...)
// ?implementation_description
// ---------------------------------------------------------

void CManualSmsListView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CManualSmsListContainer;
        iContainer->SetMopParent(this);
		iContainer->iPhotosAppUi=iPhotosAppUi;
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );		
        }
   }

// ---------------------------------------------------------
// CManualSmsListView::HandleCommandL(TInt aCommand)
// implementation_description
// ---------------------------------------------------------

void CManualSmsListView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }

/*void CManualSmsListView::SendPhotoToServer()
{
}
*/

// End of File 