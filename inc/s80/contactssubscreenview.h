/* Copyright (c) 2004, Nokia Mobile Phones. All rights reserved */

#ifndef __CONTACTSSUBSCREENVIEW_H__
#define __CONTACTSSUBSCREENVIEW_H__

#include <eikbctrl.h>
#include "RippleVaultAppUi.h"

// Forward declarations
class CEikColumnListBox;
class CCknAppTitle;
class CRippleVaultAppUi;

/**
 *  Search results control. This control owns listbox, which shows search
 *  results.
 *
 *  This is non-window owning control.
 *
 *  This represents the left side view in this application.
 */
class CContactsSubScreenView : public CEikBorderedControl, public MCoeControlObserver
    {
public:

    CContactsSubScreenView(CRippleVaultAppUi& aAppUi);
    void ConstructL(CCoeControl* aWindowOwningContainer);
     ~CContactsSubScreenView();

	CRippleVaultAppUi& iAppUi;

    TInt CurrentItemIndex(); // -1, if listbox is empty, otherwise 0 or greater

    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

public:

    virtual TSize MinimumSize();
    void SetData();
	TRect innerRect;
	CDesCArray *iContactsArray;
	CDesCArray *iMobileArray;

protected: // From CCoeControl:

    virtual void Draw(const TRect& aRect) const;
    virtual TInt CountComponentControls() const;
    virtual CCoeControl* ComponentControl(TInt aIndex) const;
    virtual void SizeChanged();
    virtual void FocusChanged(TDrawNow aDrawNow);

private:    // From MCoeControlObserver

    void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEventType);

private: // Data


    CCknAppTitle* iAppTitle;
    CEikColumnListBox* iListbox;
    };


#endif // __CONTACTSSUBSCREENVIEW_H__
