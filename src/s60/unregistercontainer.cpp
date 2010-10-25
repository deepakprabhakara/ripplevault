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
 Module Name				: UnRegisterContainer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 17-06-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     View seen by user before registration
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/

// INCLUDE FILES
#include "UnRegisterContainer.h"
#include <RippleVault.rsg>
#include <avkon.hrh>
#include <eiklabel.h>  // for example label control
#include <akncontext.h> 
#include <akntitle.h>
#include <eikspane.h>
#include <fbs.h> 
#include "ripplevaultappui.h"

#include "rippleimage.mbg"
#include "logo.mbg"

#include <aknnavi.h>

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CUnRegisterContainer::ConstructL(const TRect& aRect)		
// EPOC two phased constructor
// ---------------------------------------------------------

void CUnRegisterContainer::ConstructL(const TRect& aRect)
    {
    CreateWindowL();

    iLabel = new (ELeave) CEikLabel;
    iLabel->SetContainerWindowL( *this );
	iLabel->SetFont(iEikonEnv->LegendFont());
    iLabel->SetTextL(*(iEikonEnv->AllocReadResourceL(R_PURPLEACE_WEB)));

	CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();

	TBuf<10> message;
	message.Copy(_L(""));

	CAknNavigationControlContainer* aNaviPane = (CAknNavigationControlContainer *)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidNavi));

	CAknNavigationDecorator* aNaviDecorator = aNaviPane->CreateMessageLabelL(message);

    aNaviPane->PushL(*aNaviDecorator);

	CAknContextPane* cp=(CAknContextPane *)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidContext));

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KRippleImageMbmFileName);

	CFbsBitmap* bitmap = iEikonEnv->CreateBitmapL(path, EMbmRippleimageRippleimage);
	CleanupStack::PushL(bitmap);
	CFbsBitmap* bitmapmask = iEikonEnv->CreateBitmapL(path, EMbmRippleimageRippleimagem);
	CleanupStack::PushL(bitmapmask);
	cp->SetPicture(bitmap, bitmapmask);
	CleanupStack::Pop(); // bitmapmask
	CleanupStack::Pop(); // bitmap
	DrawNow();

	CAknTitlePane* tp=(CAknTitlePane*)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidTitle)); 
    tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH))); // Set the text string.

	TRect Please(TPoint(0,44),TPoint(176,188));
    SetRect(Please);
    //SetRect(aRect);
    ActivateL();
    }

// Destructor
CUnRegisterContainer::~CUnRegisterContainer()
    {
    delete iLabel;
    }

// ---------------------------------------------------------
// CUnRegisterContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CUnRegisterContainer::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    iLabel->SetExtent( TPoint(30,120), iLabel->MinimumSize() );
    }

// ---------------------------------------------------------
// CUnRegisterContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CUnRegisterContainer::CountComponentControls() const
    {
    return 1; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CUnRegisterContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CUnRegisterContainer::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        case 0:
            return iLabel;
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CUnRegisterContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CUnRegisterContainer::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();
	
	gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushColor(KRgbWhite);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc.DrawRect(aRect);

	CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KLogoMbmFileName);

	User::LeaveIfError(bitmap->Load(path, EMbmLogoLogo));
	TPoint pos(50,30);
	gc.BitBlt(pos, bitmap);	
	CleanupStack::PopAndDestroy();
    
    }

// ---------------------------------------------------------
// CUnRegisterContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CUnRegisterContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }

// End of File  
