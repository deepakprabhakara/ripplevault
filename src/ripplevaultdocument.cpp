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
 Module Name				: RippleVaultDocument.cpp
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

#include "RippleVaultAppUi.h"
#include "RippleVaultDocument.h"


// Standard Epoc construction sequence
#ifdef __UIQ__
#ifdef __S80__
CRippleVaultDocument* CRippleVaultDocument::NewL(CEikApplication& aApp)
#else
CRippleVaultDocument* CRippleVaultDocument::NewL(CQikApplication& aApp)
#endif
#else
CRippleVaultDocument* CRippleVaultDocument::NewL(CEikApplication& aApp)
#endif
    {
    CRippleVaultDocument* self = NewLC(aApp);
    CleanupStack::Pop();
    return self;
    }
#ifdef __UIQ__
#ifdef __S80__
CRippleVaultDocument* CRippleVaultDocument::NewLC(CEikApplication& aApp)
#else
CRippleVaultDocument* CRippleVaultDocument::NewLC(CQikApplication& aApp)
#endif
#else
CRippleVaultDocument* CRippleVaultDocument::NewLC(CEikApplication& aApp)
#endif
    {
    CRippleVaultDocument* self = new (ELeave) CRippleVaultDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

void CRippleVaultDocument::ConstructL()
    {
    // Add any construction that can leave here
    }    
#ifdef __UIQ__
#ifdef __S80__
CRippleVaultDocument::CRippleVaultDocument(CEikApplication& aApp) : CEikDocument(aApp) 
#else
CRippleVaultDocument::CRippleVaultDocument(CQikApplication& aApp) : CQikDocument(aApp)
#endif
#else
CRippleVaultDocument::CRippleVaultDocument(CEikApplication& aApp) : CEikDocument(aApp) 
#endif
    {
    // Add any construction that can not leave here
    }   

CRippleVaultDocument::~CRippleVaultDocument()
    {
    // Any any destruction code here
    }

CEikAppUi* CRippleVaultDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it
    CEikAppUi* appUi = new (ELeave) CRippleVaultAppUi;
    return appUi;
    }

