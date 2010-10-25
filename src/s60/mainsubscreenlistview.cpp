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
 Module Name				: MainSubScreenListView.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     To Show sub-screen list for S60
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/
 
//INCLUDE FILES
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <RippleVault.rsg>
#include  "MainSubScreenListView.h"
#include  "MainSubScreenListContainer.h" 
#include  "RippleVault.hrh"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CMainSubScreenListView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------

void CMainSubScreenListView::ConstructL()
    {	
		BaseConstructL( R_SUBSCREEN_VIEW );
    }

// ---------------------------------------------------------
// CMainSubScreenListView::~CMainSubScreenListView()
// ?implementation_description
// ---------------------------------------------------------

CMainSubScreenListView::~CMainSubScreenListView()
    {
		if ( iContainer )
			{
			AppUi()->RemoveFromViewStack( *this, iContainer );
			}

		delete iContainer;
    }

// ---------------------------------------------------------
// TUid CMainSubScreenListView::Id()
// ?implementation_description
// ---------------------------------------------------------

TUid CMainSubScreenListView::Id() const
    {
    return KViewIdSubScreen;
    }

// ---------------------------------------------------------
// CMainSubScreenListView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------

void CMainSubScreenListView::HandleCommandL(TInt aCommand)
    {   
    switch ( aCommand )
        {
		case EMarkSel:
			iContainer->ShowScreen();
			break;
		case EAknSoftkeyBack:
            {
				iPhotosAppUi->iMainScreenValue = 1;
				AppUi()->HandleCommandL(EMainView);
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
// CMainSubScreenListView::HandleClientRectChange()
// ---------------------------------------------------------

void CMainSubScreenListView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CMainSubScreenListView::DoActivateL(...)
// ?implementation_description
// ---------------------------------------------------------

void CMainSubScreenListView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CMainSubScreenListContainer;
        iContainer->SetMopParent(this);
		iContainer->iPhotosAppUi=iPhotosAppUi;
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );		
        }
   }

// ---------------------------------------------------------
// CMainSubScreenListView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------

void CMainSubScreenListView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }
// End of File 