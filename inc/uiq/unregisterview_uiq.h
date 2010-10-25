/*
* ============================================================================
*  Name     : RippleVaultLeftView.h
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CUnRegisterView header
*  Copyright: PurpleAce
* ============================================================================
*/


#ifndef __UNREGISTERVIEW_UIQ_H
#define __UNREGISTERVIEW_UIQ_H

#include <coecntrl.h>
#include "RippleVaultAppUi.h"

class CRippleVaultAppUi;

class CUnRegisterView : public CCoeControl
    {
	public:
		static CUnRegisterView* NewL( const TRect& aRect,CRippleVaultAppUi& aAppUi );
		~CUnRegisterView();
		void ConstructL( const TRect& aRect );
		virtual void SetFocus(TBool aFocus, TDrawNow aDrawNow=ENoDrawNow);

		CRippleVaultAppUi& iAppUi;

		CUnRegisterView(CRippleVaultAppUi& aAppUi);

	private: // From CCoeControl
		void Draw(const TRect& /*aRect*/) const;
		TInt CountComponentControls() const;
		CCoeControl* ComponentControl(TInt aIndex) const;

	private:
		
    };

#endif
