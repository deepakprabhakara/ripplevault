/*
* ============================================================================
*  Name     : UnRegisterView_Uiq.cpp
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CUnRegisterView implementation
*  Copyright: PurpleAce
* ============================================================================
*/


#include "UnRegisterView_Uiq.h"
#include <RippleVault_Uiq.rsg>

#include "logo.mbg"
#include "fbs.h"

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

	SetRect(aRect);

	ActivateL();
}

CUnRegisterView::~CUnRegisterView()
{
}

TInt CUnRegisterView::CountComponentControls() const
{
    return 0;
}

void CUnRegisterView::SetFocus(TBool aFocus, TDrawNow aDrawNow)
{
	CCoeControl::SetFocus(aFocus, aDrawNow);
}

CCoeControl* CUnRegisterView::ComponentControl(TInt aIndex) const
{
    switch (aIndex)
        {
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
	TPoint pos(50,60);
	gc.BitBlt(pos, bitmap);	
	CleanupStack::PopAndDestroy();

	const CFont* fontUsed;  
	fontUsed = iEikonEnv->TitleFont();
	gc.UseFont(fontUsed);

	//TPoint pos1(50,30);
	//gc.DrawText(*(iEikonEnv->AllocReadResourceL(R_RIPPLE_VAULT)),pos1);
	TPoint pos2(30,180);
	gc.DrawText(*(iEikonEnv->AllocReadResourceL(R_PURPLEACE_WEB)),pos2);
	
    // Finished using the font
	gc.DiscardFont();

}
