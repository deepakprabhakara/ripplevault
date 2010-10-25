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
 Module Name				: MailIdView.cpp
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

// INCLUDE FILES
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <ripplevault.rsg>
#include  "ripplevault.hrh"
#include  "RippleVaultAppUi.h"
#include  "MailIdView.h"
#include  "MailIdContainer.h" 


#include <cntitem.h>
#include <cntfldst.h>

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CMailIdView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CMailIdView::ConstructL()
    {
		BaseConstructL( R_MAILID_VIEW );
    }

// ---------------------------------------------------------
// CMailIdView::~CMailIdView()
// ?implementation_description
// ---------------------------------------------------------
//
CMailIdView::~CMailIdView()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CMailIdView::Id()
// ?implementation_description
// ---------------------------------------------------------
//
TUid CMailIdView::Id() const
    {
    return KView8Id;
    }

// ---------------------------------------------------------
// CMailIdView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------
//
void CMailIdView::HandleCommandL(TInt aCommand)
    {   
    switch ( aCommand )
        {       
        default:
            {
            AppUi()->HandleCommandL( aCommand );
            break;
            }
        }
    }

// ---------------------------------------------------------
// CMailIdView::HandleClientRectChange()
// ---------------------------------------------------------
//
void CMailIdView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CMailIdView::DoActivateL(...)
// ?implementation_description
// ---------------------------------------------------------
//
void CMailIdView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CMailIdContainer;
        iContainer->SetMopParent(this);
		iContainer->iPhotosAppUi=iPhotosAppUi;
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );	

        } 
   }

// ---------------------------------------------------------
// CMailIdView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------
//
void CMailIdView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }

// End of File
