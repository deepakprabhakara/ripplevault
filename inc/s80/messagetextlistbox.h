/* Copyright (c) 2004, Nokia Mobile Phones. All rights reserved */

#ifndef __MESSAGETEXTLISTBOX_H
#define __MESSAGETEXTLISTBOX_H

#include <eikbctrl.h>
#include "RippleVaultAppUi.h"
///
#include <msvapi.h>
#include <msvids.h>
#include <mtclreg.h>
#include <smsclnt.h>
#include <msvstd.h>
#include <txtrich.h>
#include <smuthdr.h>

// Forward declarations:
class CFbsFont;
class CCknAppTitle;
class CEikColumnListBox;
class CCustomListBox;
class CRippleVaultAppUi;

class CMsvSession;
class MMsvSessionObserver;
class CReadSmsAO;

/**
 *  This control owns column listbox,
 *  first column of which shows labels for each contact detail data.
 *  The data fields are contained in the second column.
 *
 *  This is non-window owning control.
 *
 *  This represents the right side view in this application.
 */
class CMessageTextListBox : public CEikBorderedControl, MMsvSessionObserver
    {
public:

    CMessageTextListBox(CRippleVaultAppUi& aAppUi);
    void ConstructL(CCoeControl* aWindowOwningContainer,TRect aRect);
     ~CMessageTextListBox();

	CRippleVaultAppUi& iAppUi;

	 
	CCustomListBox* ListBox()
	{
		return iListbox; 
	}

public:    // From CEikBorderedControl

    void Draw(const TRect& aRect) const;
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

public:

    virtual TSize MinimumSize();
    void SetData(TInt aIsmark); // Don't take ownership
	TRect iSize;
	void SetIconsArray();
	void GetSms(TInt val);

	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* /*aArg3*/);

	void MarkUnMarAllSms(TInt aIsMark);
	void MarkUnMarkSelectedItem();
	void MarkAllItems();
	void UnMarkAllItems();
	void MarkUnMarkSelectedSms();
	void SetFileList(TInt aIsmark);
	void GetTopIndex();

	void ChangeScreen();
	void CloseReadSms();

	void SetTitle(TInt aTitle);

	TInt iInboxCount;
	TInt iSentItemCount;

	TInt iCursorPos;
	TInt iIsMark;

	TInt iCount1;
	TInt iCount2;

	TInt iTempCount;
	
	CArrayFix<TInt>* iInboxSelectedSms;
	CArrayFix<TInt>* iSentItemSelectedSms;

	TInt GetSelectedSmsCount();		
	void SendSelected();	
	void WriteSmsToFile(TInt aId);

	CReadSmsAO	*iReadSmsAO;

protected: // From CCoeControl

    virtual TInt CountComponentControls() const;
    virtual CCoeControl* ComponentControl(TInt aIndex) const;
    virtual void SizeChanged();
    virtual void FocusChanged(TDrawNow aDrawNow);

private: // Data

    CFbsFont*            iLabelFont;
    CFbsFont*            iDataFont;

    CCknAppTitle*        iAppTitle;
	CCustomListBox*		 iListbox;		
	
public:
	CDesCArray *InboxSmsDatearray;
	CDesCArray *InboxSmsMessagearray;
	CDesCArray *InboxSmsDetailsarray;

	CDesCArray *SentSmsDatearray;
	CDesCArray *SentSmsMessagearray;
	CDesCArray *SentSmsDetailsarray;
    
    //CEikColumnListBox*   iListbox;
	//TFileName smstext;
    };


#endif // __CONTACT_DETAIL_CONTROL_H
