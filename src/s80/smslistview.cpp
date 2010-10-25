/*
* ============================================================================
*  Name     : SmsListView.cpp
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CSmsListView implementation
*  Copyright: PurpleAce
* ============================================================================
*/


#include <cknenv.h>
#include <eikenv.h>
#include <ckntitle.h>

#include "SmsListView.h"
#include "RippleVaultUtils.h"
#include <RippleVault_series80.rsg>
#include "MessageTextListBox.h"
#include "MessageListBox.h"
#include "Ripplevaultappui.h"

#include "logo.mbg"
#include "fbs.h"
#include "constants.h"

CSmsListView* CSmsListView::NewL( const TRect& aRect,CRippleVaultAppUi& aAppUi)
{
    CSmsListView* self = new (ELeave) CSmsListView(aAppUi);
    CleanupStack::PushL(self);
    self->ConstructL( aRect );
    CleanupStack::Pop();
    return self;
}

CSmsListView::CSmsListView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
}

void CSmsListView::ConstructL( const TRect& aRect )
{
    CreateWindowL();

	itemIndex = 0;
	
	iMessageListBox = new(ELeave)CMessageListBox(iAppUi);
    iMessageListBox->ConstructL(this);
	iMessageListBox->SetData();
    iMessageListBox->SetFocus(ETrue, ENoDrawNow);
    iMessageListBox->SetFocus(ETrue);
    iMessageListBox->SetObserver(this);

	// Construct rights side "popup window" looking area
    iMessageTextListBox = new(ELeave)CMessageTextListBox(iAppUi);
    iMessageTextListBox->ConstructL(this,aRect);
    iMessageTextListBox->SetObserver(this);
	// Set logical border type to EWindow to get the shadow look (See also
    
	// CknEnv::SetDropShadowToLogicalBorder
    iMessageListBox->SetBorder(TGulBorder::EWindow);
    iMessageTextListBox->SetBorder(TGulBorder::EWindow);

    // Set the windows size
    SetRect(aRect);
    // Activate the window, which makes it ready to be drawn
    ActivateL();
}

CSmsListView::~CSmsListView()
{
	delete iMessageListBox;
    delete iMessageTextListBox;
}

TInt CSmsListView::CountComponentControls() const
{
    return 2;
}

void CSmsListView::SetFocus(TBool aFocus, TDrawNow aDrawNow)
{
	/*
	if (iTitle) 
		{
		iTitle->SetFocus( aFocus, aDrawNow );
		}
	CCoeControl::SetFocus(aFocus, aDrawNow);
	*/
}

CCoeControl* CSmsListView::ComponentControl(TInt aIndex) const
{
    switch (aIndex)
        {
		case 0:
            return iMessageListBox;
			break;
		case 1:
			return iMessageTextListBox;
			break;
		default:
			return NULL;
        }
}

void CSmsListView::Draw(const TRect& /* aRect */) const
{
	CWindowGc& gc = SystemGc();
	gc.Clear();
}

// Handle key events. Called by framework
TKeyResponse CSmsListView::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,TEventCode aType)
 {
	if(aType == EEventKey && aKeyEvent.iCode == 13 && iMessageTextListBox->IsFocused())
	{
		MarkUnMarkSelectedItem();
		return EKeyWasConsumed;
	}

	if(aType == EEventKey && aKeyEvent.iCode == 13 && iMessageListBox->IsFocused())
	{
        iMessageListBox->SetFocus(EFalse, EDrawNow);
        iMessageTextListBox->SetFocus(ETrue, EDrawNow);		
		return EKeyWasConsumed;
	}

	// First handle right/left arrow keys. Switch focused control, if needed
    if(aType == EEventKey && iMessageListBox->IsFocused() &&
       aKeyEvent.iCode == EKeyRightArrow)
    {
        iMessageListBox->SetFocus(EFalse, EDrawNow);
        iMessageTextListBox->SetFocus(ETrue, EDrawNow);
        return EKeyWasConsumed;
    }
    else if(aType == EEventKey && iMessageTextListBox->IsFocused() &&
       aKeyEvent.iCode == EKeyLeftArrow)
    {
        iMessageListBox->SetFocus(ETrue, EDrawNow);
        iMessageTextListBox->SetFocus(EFalse, EDrawNow);
        return EKeyWasConsumed;
    }
    else if(aType == EEventKey && iMessageListBox->IsFocused() &&
       aKeyEvent.iCode == EKeyDownArrow)
    {
		if( itemIndex < 1)
		{
			itemIndex++;

		    if(itemIndex == 1)
			{
			   //iMessageTextListBox->GetSms(1);
			   //iMessageTextListBox->SetIconsArray();
			   iAppUi.iImageFlag = 6;
			   iMessageTextListBox->ChangeScreen();
			   iMessageTextListBox->SetData(1);
			   iMessageTextListBox->SetTitle(1);
			}
		}
    }
    else if(aType == EEventKey && iMessageListBox->IsFocused() &&
       aKeyEvent.iCode == EKeyUpArrow)
    {
        //TInt itemIndex = iMessageListBox->CurrentItemIndex();
		if( itemIndex > 0)
		{
			itemIndex--;

			if(itemIndex == 0)
			{
			   //iMessageTextListBox->GetSms(0);
			   //iMessageTextListBox->SetIconsArray();
			   iAppUi.iImageFlag = 5;
			   iMessageTextListBox->ChangeScreen();
			   iMessageTextListBox->SetData(1);
			   iMessageTextListBox->SetTitle(0);
			}
		}
		 //return EKeyWasConsumed;
    }

    // Let the focused control handle any other key events
    if(iMessageListBox->IsFocused())
        return iMessageListBox->OfferKeyEventL(aKeyEvent, aType);
    else if(iMessageTextListBox->IsFocused())
        return iMessageTextListBox->OfferKeyEventL(aKeyEvent, aType);
    else
        return EKeyWasNotConsumed;
}

// Handle search results listbox data or user selection changes. Refresh
// right side view details. This is called by framework, when one of contained
// controls calls their ReportEventL functions.
void CSmsListView::HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType)
{
	if(aEventType == EEventStateChanged && aControl == iMessageListBox)
    {
       // User selected an item from right side listbox. Load the detail
       // data to the right side control
       //TInt itemIndex = iMessageListBox->CurrentItemIndex();
       if(itemIndex >= 0)
       {
          //iMessageTextListBox->SetData();
       }
       else
       {
          //iMessageTextListBox->SetData();
       }
    }
}

// Set the left and right view controls positions. Called by framework
void CSmsListView::SizeChanged()
{
    TRect innerRect = Border().InnerRect(Rect()); // This should be same as
                                                  // Rect(), because this
                                                  // window has no border.
    if(iMessageListBox)
    {
        TRect messagelistbox(innerRect);
        messagelistbox.SetWidth(innerRect.Width()/3);
        messagelistbox.Shrink(5,5); // Allow space for border shadows
        iMessageListBox->SetRect(messagelistbox);
    }
    
	if(iMessageTextListBox)
    {
        TRect messagetextlistbox(innerRect);
        messagetextlistbox.iTl.iX = innerRect.iTl.iX + innerRect.Width()/3;
        messagetextlistbox.SetWidth(innerRect.Width()/1.5);
        messagetextlistbox.Shrink(5,5); // Allow space for border shadows
        iMessageTextListBox->SetRect(messagetextlistbox);
    }
}

void CSmsListView::MarkUnMarkSelectedItem()
{
	if(iMessageTextListBox->IsFocused())
		iMessageTextListBox->MarkUnMarkSelectedSms();
}

void CSmsListView::MarkAllItems()
{
	if(iMessageTextListBox->IsFocused())
		iMessageTextListBox->MarkAllItems();
}

void CSmsListView::UnMarkAllItems()
{
	if(iMessageTextListBox->IsFocused())
		iMessageTextListBox->UnMarkAllItems();
}

void CSmsListView::SendSelected()
{
	iMessageTextListBox->SendSelected();
}
