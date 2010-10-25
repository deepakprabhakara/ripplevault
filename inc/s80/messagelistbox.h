/* Copyright (c) 2004, Nokia Mobile Phones. All rights reserved */

#ifndef __MESSAGELISTBOX_H__
#define __MESSAGELISTBOX_H__

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
class CMessageListBox : public CEikBorderedControl, public MCoeControlObserver
    {
public:

    CMessageListBox(CRippleVaultAppUi& aAppUi);
    void ConstructL(CCoeControl* aWindowOwningContainer);
     ~CMessageListBox();

	CRippleVaultAppUi& iAppUi;

    TInt CurrentItemIndex(); // -1, if listbox is empty, otherwise 0 or greater

    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

public:

    virtual TSize MinimumSize();
    void SetData();
	void SetIconsArray();
	TRect innerRect;
	void MarkSelectedPhoto();
	void UnMarkSelectedPhoto();
	void MarkUnMarkSelectedItem();
	void MarkAllItems();
	void UnMarkAllItems();

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


#endif // __MESSAGELISTBOX_H__
