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
 Module Name				: TestFileView2.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 29-03-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Creates Empty View for S60
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/
 
//INCLUDE FILES
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <RippleVault.rsg>
#include  "TestFileView2.h"
#include  "TestFileContainer2.h" 
#include  "RippleVault.hrh"

#include  "constants.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CTestFileView2::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CTestFileView2::ConstructL()
    {
    //BaseConstructL( R_TESTFILE_VIEW2 );
    }

// ---------------------------------------------------------
// CTestFileView2::~CTestFileView2()
// ?implementation_description
// ---------------------------------------------------------
//
CTestFileView2::~CTestFileView2()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CTestFileView2::Id()
// ?implementation_description
// ---------------------------------------------------------
//
TUid CTestFileView2::Id() const
    {
    return KViewIdEmptyScreen;
    }

// ---------------------------------------------------------
// CTestFileView2::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------
//
void CTestFileView2::HandleCommandL(TInt aCommand)
    {   
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
            {            
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
// CTestFileView2::HandleClientRectChange()
// ---------------------------------------------------------
//
void CTestFileView2::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CTestFileView2::DoActivateL(...)
// ?implementation_description
// ---------------------------------------------------------
//
void CTestFileView2::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CTestFileContainer2;
        iContainer->SetMopParent(this);
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );		
        }
   }

// ---------------------------------------------------------
// CTestFileView2::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------
//
void CTestFileView2::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }

// End of File

