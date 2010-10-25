/*
* ============================================================================
*  Name     : SmsListView.h
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CSmsListView header
*  Copyright: PurpleAce
* ============================================================================
*/


#ifndef __SMSLISTVIEW_H
#define __SMSLISTVIEW_H

#include <coecntrl.h>
#include "RippleVaultAppUi.h"
#include <eikbctrl.h>

class CCknAppTitle;
class CRippleVaultAppUi;
class CMessageTextListBox;
class CMessageListBox;

class CSmsListView : public CEikBorderedControl, public MCoeControlObserver
                            
    {
	public:
		static CSmsListView* NewL( const TRect& aRect,CRippleVaultAppUi& aAppUi);
		~CSmsListView();
		void ConstructL( const TRect& aRect );
		void DrawFocusShadow() const;
		virtual void SetFocus(TBool aFocus, TDrawNow aDrawNow=ENoDrawNow);

		CRippleVaultAppUi& iAppUi;

		CSmsListView(CRippleVaultAppUi& aAppUi);
		void MarkSelectedPhoto();
		void UnMarkSelectedPhoto();
		void MarkUnMarkSelectedItem();
		void MarkAllItems();
		void UnMarkAllItems();
		TInt itemIndex;
		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

		void SendSelected();

	private: // From CCoeControl
		void Draw(const TRect& /*aRect*/) const;
		TInt CountComponentControls() const;
		CCoeControl* ComponentControl(TInt aIndex) const;
	    void SizeChanged();
	    void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEventType);

	private:
	    CMessageTextListBox* iMessageTextListBox;
		CMessageListBox *iMessageListBox;

    };

#endif
