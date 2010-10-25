/*
* ============================================================================
*  Name     : ContactsView.h
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CContactsView header
*  Copyright: PurpleAce
* ============================================================================
*/


#ifndef __CONTACTSVIEW_H
#define __CONTACTSVIEW_H

#include <coecntrl.h>
#include "RippleVaultAppUi.h"
#include <eikbctrl.h>

class CCknAppTitle;
class CRippleVaultAppUi;
class CContactsSubScreenView;
class CContactsScreenView;

class CContactsView : public CEikBorderedControl, public MCoeControlObserver
                            
    {
	public:
		static CContactsView* NewL( const TRect& aRect,CRippleVaultAppUi& aAppUi);
		~CContactsView();
		void ConstructL( const TRect& aRect );
		void DrawFocusShadow() const;
		virtual void SetFocus(TBool aFocus, TDrawNow aDrawNow=ENoDrawNow);

		CRippleVaultAppUi& iAppUi;

		CContactsView(CRippleVaultAppUi& aAppUi);
		TInt itemIndex;
		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);
		TInt GetContactsScreenCurrentItemIndex();
		TInt GetContactsSubScreenCurrentItemIndex();
		void AddContacts();
		void DeleteContacts();
		void ShareContacts();

	private: // From CCoeControl
		void Draw(const TRect& /*aRect*/) const;
		TInt CountComponentControls() const;
		CCoeControl* ComponentControl(TInt aIndex) const;
	    void SizeChanged();
	    void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEventType);

	private:
	    CContactsSubScreenView* iContactsSubScreenView;
		CContactsScreenView *iContactsScreenView;

    };

#endif
