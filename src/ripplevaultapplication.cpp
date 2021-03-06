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
 Module Name				: RippleVaultApplication.cpp
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


#include "RippleVaultDocument.h"

#include "RippleVaultApplication.h"

#include "constants.h"

CApaDocument* CRippleVaultApplication::CreateDocumentL()

    { 

    // Create an RippleVault document, and return a pointer to it

    CApaDocument* document = CRippleVaultDocument::NewL(*this);

    return document;

    }



TUid CRippleVaultApplication::AppDllUid() const

    {

    // Return the UID for the RippleVault application

    return KUidRippleVaultApp;

    }
