/*
* ============================================================================
*  Name     : RegisterView.h
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CRegisterView header
*  Copyright: PurpleAce
* ============================================================================
*/


#ifndef __REGISTERVIEW_H
#define __REGISTERVIEW_H

#include <coecntrl.h>
#include "RippleVaultAppUi.h"
#include <eikbctrl.h>

class CCknAppTitle;
class CRippleVaultAppUi;
class CMainSubScreenView;
class CMainScreenView;

class CRegisterView : public CEikBorderedControl, public MCoeControlObserver
                            
    {
	public:
		static CRegisterView* NewL( const TRect& aRect,CRippleVaultAppUi& aAppUi);
		~CRegisterView();
		void ConstructL( const TRect& aRect );
		void DrawFocusShadow() const;
		virtual void SetFocus(TBool aFocus, TDrawNow aDrawNow=ENoDrawNow);

		CRippleVaultAppUi& iAppUi;

		CRegisterView(CRippleVaultAppUi& aAppUi);
		TInt itemIndex;
		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);
		TInt GetMainScreenCurrentItemIndex();
		TInt GetMainSubScreenCurrentItemIndex();
		void ShowStatusScreen();
		void SelectSubScreen();

	private: // From CCoeControl
		void Draw(const TRect& /*aRect*/) const;
		TInt CountComponentControls() const;
		CCoeControl* ComponentControl(TInt aIndex) const;
	    void SizeChanged();
	    void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEventType);

	private:
	    CMainSubScreenView* iMainSubScreenView;
		CMainScreenView *iMainScreenView;

    };

#endif
