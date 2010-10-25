/* Copyright (c) 2004, Nokia Mobile Phones. All rights reserved */

#ifndef __CONTACTSSCREENVIEW_H__
#define __CONTACTSSCREENVIEW_H__

#include <eikbctrl.h>
#include "RippleVaultAppUi.h"
#include <e32std.h>
#include <cntdb.h>
#include <cntfield.h>
#include <cntitem.h>
#include <cntfldst.h>

// Forward declarations
class CEikColumnListBox;
class CCknAppTitle;
class CRippleVaultAppUi;
class CReadContactsAO;

/**
 *  Search results control. This control owns listbox, which shows search
 *  results.
 *
 *  This is non-window owning control.
 *
 *  This represents the left side view in this application.
 */
class CContactsScreenView : public CEikBorderedControl, public MCoeControlObserver
    {
public:

    CContactsScreenView(CRippleVaultAppUi& aAppUi);
    void ConstructL(CCoeControl* aWindowOwningContainer);
     ~CContactsScreenView();

	CRippleVaultAppUi& iAppUi;

    TInt CurrentItemIndex(); // -1, if listbox is empty, otherwise 0 or greater

    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

public:

    virtual TSize MinimumSize();
    void SetData();
	void CloseReadContact();
	TInt GetContacts(TInt aContactId, TDesC& aContactDetails, TDesC& aMobileDetails);
	void GetTextFieldFromFieldSet(CContactItemFieldSet& aFieldSet, const TUid aField,TDesC& name);

	TRect innerRect;

    TInt iContactsCount;
	CDesCArray *iContactsArray;
	CArrayFix<TInt>* iContactsIdArray;

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
    CContactDatabase* iContactsDB;
  
	CReadContactsAO	*iReadContactsAO;
    };


#endif // __CONTACTSSCREENVIEW_H__
