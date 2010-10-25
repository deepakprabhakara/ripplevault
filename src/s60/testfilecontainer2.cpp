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
 Module Name				: TestFileContainer2.cpp
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

// INCLUDE FILES
#include "TestFileContainer2.h"
#include <eiklabel.h>  // for example label control

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CTestFileContainer2::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CTestFileContainer2::ConstructL(const TRect& aRect)
    {
    CreateWindowL();

	TRect rect(TPoint(0,0),TPoint(0,0));
    SetRect(rect);
    ActivateL();
    }

// Destructor
CTestFileContainer2::~CTestFileContainer2()
    {
    }

// ---------------------------------------------------------
// CTestFileContainer2::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CTestFileContainer2::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    }

// ---------------------------------------------------------
// CTestFileContainer2::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CTestFileContainer2::CountComponentControls() const
    {
    return 0; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CTestFileContainer2::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CTestFileContainer2::ComponentControl(TInt aIndex) const
    {
	   aIndex=0;
       return NULL;        
    }

// ---------------------------------------------------------
// CTestFileContainer2::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CTestFileContainer2::Draw(const TRect& aRect ) const
    {
    }

// ---------------------------------------------------------
// CTestFileContainer2::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CTestFileContainer2::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }

// End of File  
