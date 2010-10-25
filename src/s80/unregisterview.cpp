/*
* ============================================================================
*  Name     : UnRegisterView.cpp
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CUnRegisterView implementation
*  Copyright: PurpleAce
* ============================================================================
*/


#include <cknenv.h>
#include <eikenv.h>
#include <ckntitle.h>

#include "UnRegisterView.h"
#include "RippleVaultUtils.h"
#include <RippleVault_series80.rsg>

#include "logo.mbg"
#include "fbs.h"
#include "constants.h"

CUnRegisterView* CUnRegisterView::NewL( const TRect& aRect, CRippleVaultAppUi& aAppUi)
{
    CUnRegisterView* self = new (ELeave) CUnRegisterView(aAppUi);
    CleanupStack::PushL(self);
    self->ConstructL( aRect );
    CleanupStack::Pop();
    return self;
}

CUnRegisterView::CUnRegisterView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
}

void CUnRegisterView::ConstructL( const TRect& aRect )
{
    CreateWindowL();

    iTitle = CCknAppTitle::NewL(CCknAppTitle::EList);
    iTitle->SetContainerWindowL(*this);
    HBufC* text = iCoeEnv->AllocReadResourceLC(R_EXAMPLE_RIPPLE_PUBLISH);
    iTitle->SetTextL(*text, CCknAppTitle::EMainTitle);
    CleanupStack::PopAndDestroy(text);
	TRect titleRect(1, 1, aRect.Width()-KShadowMargin, iTitle->MinimumSize().iHeight);
	iTitle->SetRect(titleRect);

	// TODO : ADD YOUR OTHER CONTROLS HERE

	// COMPLETE VIEW SETUP

	SetRect(aRect);

	// Indicate that the control is blank
    SetBlank();

    // Set the control's border
    //SetBorder(TGulBorder::EFlatContainer);

    // Set the correct application view (Note: ESkinAppViewWithCbaNoToolband is the default)
    CknEnv::Skin().SetAppViewType(ESkinAppViewWithCbaNoToolband);

	ActivateL();
}

CUnRegisterView::~CUnRegisterView()
{
    delete iTitle;
}

TInt CUnRegisterView::CountComponentControls() const
{
    return 1;
}

void CUnRegisterView::SetFocus(TBool aFocus, TDrawNow aDrawNow)
{
	if (iTitle) 
		{
		iTitle->SetFocus( aFocus, aDrawNow );
		}
	CCoeControl::SetFocus(aFocus, aDrawNow);
}

CCoeControl* CUnRegisterView::ComponentControl(TInt aIndex) const
{
    switch (aIndex)
        {
		case 0:
			return iTitle;
			break;
		default:
			return NULL;
        }
}

void CUnRegisterView::Draw(const TRect& /* aRect */) const
{
	CWindowGc& gc = SystemGc();
	gc.Clear();

	CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KLogoMbmFileName);

	User::LeaveIfError(bitmap->Load(path, EMbmLogoLogo));
	TPoint pos(30,80);
	gc.BitBlt(pos, bitmap);	
	CleanupStack::PopAndDestroy();

    // Let's draw shadow around the window if window is focused. 
	if (IsFocused())
	{
		RippleVaultUtils::DrawShadowL( gc, Rect() );
	}
	return;
}
