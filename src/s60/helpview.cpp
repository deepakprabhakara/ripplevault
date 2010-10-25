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
 Module Name				: HelpView.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Displays help for Series 60
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

// INCLUDE FILES
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <ripplevault.rsg>
#include  "RippleVaultAppUi.h"
#include  "HelpView.h"
#include  "HelpContainer.h" 
#include  "ripplevault.hrh"

#include <aknmessagequerydialog.h>

// ---------------------------------------------------------
// CHelpView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------

void CHelpView::ConstructL()
{
    BaseConstructL( R_HELP_VIEW );
}

// ---------------------------------------------------------
// CHelpView::~CHelpView()
// ?implementation_description
// ---------------------------------------------------------

CHelpView::~CHelpView()
{
    if ( iHelpContainer )
	{
        AppUi()->RemoveFromViewStack( *this, iHelpContainer );
	}
    delete iHelpContainer;
}

// ---------------------------------------------------------
// TUid CHelpView::Id()
// ?implementation_description
// ---------------------------------------------------------

TUid CHelpView::Id() const
{
    return KViewIdHelp;
}

// ---------------------------------------------------------
// CHelpView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------
//
void CHelpView::HandleCommandL(TInt aCommand)
{   
    switch ( aCommand )	
	{        
	case EAknSoftkeyOk:
	case EAknSoftkeyCancel:
		{     	
			if(iPhotosAppUi->RegFileExists())
					//AppUi()->HandleCommandL(ERegView);	
					AppUi()->HandleCommandL(EMainView);	
				else
					AppUi()->HandleCommandL(EUnRegView);

			iPhotosAppUi->iRipplePhotos = 2;
            break;
		}
	case ERipplevault:
		{
			iHelpContainer->iHelpFlag = 1;
			iHelpContainer->AboutRippleVault();
		}
		break;
	case EContacts:
		{
			iHelpContainer->iHelpFlag = 2;
			iHelpContainer->AboutRippleVault();
		}
		break;
	case ERestoring:
		{
			iHelpContainer->iHelpFlag = 3;
			iHelpContainer->AboutRippleVault();
		}
		break;
	case EPhotos:
		{
			iHelpContainer->iHelpFlag = 4;
			iHelpContainer->AboutRippleVault();
		}
		break;
	case EViewing:
		{
			iHelpContainer->iHelpFlag = 5;
			iHelpContainer->AboutRippleVault();
		}
		break;
	case ESMSBackup:
		{
			iHelpContainer->iHelpFlag = 6;
			iHelpContainer->AboutRippleVault();
		}
		break;
	default:
		{
            AppUi()->HandleCommandL( aCommand );
            break;
		}
	}
}

// ---------------------------------------------------------
// CHelpView::HandleClientRectChange()
// ---------------------------------------------------------

void CHelpView::HandleClientRectChange()
{
    if ( iHelpContainer )
	{
        iHelpContainer->SetRect( ClientRect() );
	}
}

// ---------------------------------------------------------
// CHelpView::DoActivateL(...)
// ?implementation_description
// ---------------------------------------------------------
//
void CHelpView::DoActivateL(
							 const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
							 const TDesC8& /*aCustomMessage*/)
{
    if (!iHelpContainer)
	{
        iHelpContainer = new (ELeave) CHelpContainer();
        iHelpContainer->SetMopParent(this);		
        iHelpContainer->ConstructL();
        AppUi()->AddToStackL( *this, iHelpContainer );
	} 
}

// ---------------------------------------------------------
// CHelpView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------
//
void CHelpView::DoDeactivate()
{
    if ( iHelpContainer )
	{
        AppUi()->RemoveFromViewStack( *this, iHelpContainer );
	}
    
    delete iHelpContainer;
    iHelpContainer = NULL;
}


// End of File

