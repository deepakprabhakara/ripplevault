/*
* ============================================================================
*  Name     : RegisterView.cpp
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CRegisterView implementation
*  Copyright: PurpleAce
* ============================================================================
*/


#include <cknenv.h>
#include <eikenv.h>
#include <ckntitle.h>

#include "RegisterView.h"
#include "RippleVaultUtils.h"
#include <RippleVault_series80.rsg>
#include "MainSubScreenView.h"
#include "MainScreenView.h"
#include "Ripplevaultappui.h"

#include "logo.mbg"
#include "fbs.h"
#include "constants.h"

#include <eikmenub.h>

CRegisterView* CRegisterView::NewL( const TRect& aRect,CRippleVaultAppUi& aAppUi)
{
    CRegisterView* self = new (ELeave) CRegisterView(aAppUi);
    CleanupStack::PushL(self);
    self->ConstructL( aRect );
    CleanupStack::Pop();
    return self;
}

CRegisterView::CRegisterView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
}

void CRegisterView::ConstructL( const TRect& aRect )
{
    CreateWindowL();

	itemIndex = 0;
	
	iMainScreenView = new(ELeave)CMainScreenView(iAppUi);
    iMainScreenView->ConstructL(this);
	iMainScreenView->SetData();
    iMainScreenView->SetFocus(ETrue, ENoDrawNow);
    iMainScreenView->SetFocus(ETrue);
    iMainScreenView->SetObserver(this);

	// Construct rights side "popup window" looking area
    iMainSubScreenView = new(ELeave)CMainSubScreenView(iAppUi);
    iMainSubScreenView->ConstructL(this);
	iMainSubScreenView->SetData(0);
    iMainSubScreenView->SetObserver(this);
	// Set logical border type to EWindow to get the shadow look (See also
    
	// CknEnv::SetDropShadowToLogicalBorder
    iMainScreenView->SetBorder(TGulBorder::EWindow);
    iMainSubScreenView->SetBorder(TGulBorder::EWindow);

    // Set the windows size
    SetRect(aRect);
    // Activate the window, which makes it ready to be drawn
    ActivateL();
}

CRegisterView::~CRegisterView()
{
	delete iMainScreenView;
    delete iMainSubScreenView;
}

TInt CRegisterView::CountComponentControls() const
{
    return 2;
}

void CRegisterView::SetFocus(TBool aFocus, TDrawNow aDrawNow)
{
	/*
	if (iTitle) 
		{
		iTitle->SetFocus( aFocus, aDrawNow );
		}
	CCoeControl::SetFocus(aFocus, aDrawNow);
	*/
}

CCoeControl* CRegisterView::ComponentControl(TInt aIndex) const
{
    switch (aIndex)
        {
		case 0:
            return iMainScreenView;
			break;
		case 1:
			return iMainSubScreenView;
			break;
		default:
			return NULL;
        }
}

void CRegisterView::Draw(const TRect& /* aRect */) const
{
	CWindowGc& gc = SystemGc();
	gc.Clear();
}

// Handle key events. Called by framework
TKeyResponse CRegisterView::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,TEventCode aType)
 {
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("keycode="));
	iAppUi.LogText.AppendNum(aKeyEvent.iCode);
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	// First handle right/left arrow keys. Switch focused control, if needed
    if(aType == EEventKey && iMainScreenView->IsFocused() &&
       aKeyEvent.iCode == EKeyRightArrow && itemIndex != 4)
    {
        iMainScreenView->SetFocus(EFalse, EDrawNow);
        iMainSubScreenView->SetFocus(ETrue, EDrawNow);
        return EKeyWasConsumed;
    }
    else if(aType == EEventKey && iMainSubScreenView->IsFocused() &&
       aKeyEvent.iCode == EKeyLeftArrow)
    {
        iMainScreenView->SetFocus(ETrue, EDrawNow);
        iMainSubScreenView->SetFocus(EFalse, EDrawNow);
        return EKeyWasConsumed;
    }
    else if(aType == EEventKey && iMainScreenView->IsFocused() &&
       aKeyEvent.iCode == EKeyDownArrow)
    {
       	if( itemIndex < 4)
			itemIndex++;

		if(itemIndex == 1)
        {
		    iMainSubScreenView->SetData(1);
        }
        else if(itemIndex == 2)
        {
			iMainSubScreenView->SetData(2);
        }
        else if(itemIndex == 3)
        {
			iMainSubScreenView->SetData(3);
        }
        else if(itemIndex == 4)
        {
			iMainSubScreenView->SetData(4);
        }
    }
    else if(aType == EEventKey && iMainScreenView->IsFocused() &&
       aKeyEvent.iCode == EKeyUpArrow)
    {
        if( itemIndex > 0)
			itemIndex--;

		 if(itemIndex == 0)
		 {
			iMainSubScreenView->SetData(0);
		 }
         else if(itemIndex == 1)
         {
			iMainSubScreenView->SetData(1);
		 }
         else if(itemIndex == 2)
         {
			iMainSubScreenView->SetData(2);
		 }
         else if(itemIndex == 3)
         {
			CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
			menuBar->ChangeMenuBarL(0, R_REGISTER_MENUBAR, EFalse);

			iMainSubScreenView->SetData(3);
		 }
    }
    else if(aType == EEventKey && aKeyEvent.iCode == 13 && itemIndex != 4)
    {
		if(iMainScreenView->IsFocused())
		{
			iMainScreenView->SetFocus(EFalse, EDrawNow);
			iMainSubScreenView->SetFocus(ETrue, EDrawNow);
			return EKeyWasConsumed;
		}
		else if(iMainSubScreenView->IsFocused())
		{
			iAppUi.ActivateWindow();
			return EKeyWasConsumed;
		}

	}
	
	//if(itemIndex != 4)
	{
		// Let the focused control handle any other key events
		if(iMainScreenView->IsFocused())
			return iMainScreenView->OfferKeyEventL(aKeyEvent, aType);
		else if(iMainSubScreenView->IsFocused())
			return iMainSubScreenView->OfferKeyEventL(aKeyEvent, aType);
		else
			return EKeyWasNotConsumed;
	}
}

// Handle search results listbox data or user selection changes. Refresh
// right side view details. This is called by framework, when one of contained
// controls calls their ReportEventL functions.
void CRegisterView::HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType)
{
	if(aEventType == EEventStateChanged && aControl == iMainScreenView)
    {
       // User selected an item from right side listbox. Load the detail
       // data to the right side control
       //TInt itemIndex = iMainScreenView->CurrentItemIndex();
       if(itemIndex >= 0)
       {
          //iMainSubScreenView->SetData();
       }
       else
       {
          //iMainSubScreenView->SetData();
       }
    }
}

// Set the left and right view controls positions. Called by framework
void CRegisterView::SizeChanged()
{
    TRect innerRect = Border().InnerRect(Rect()); // This should be same as
                                                  // Rect(), because this
                                                  // window has no border.
    if(iMainScreenView)
    {
        TRect MainScreenView(innerRect);
        MainScreenView.SetWidth(innerRect.Width()/3);
        MainScreenView.Shrink(5,5); // Allow space for border shadows
        iMainScreenView->SetRect(MainScreenView);
    }
    
	if(iMainSubScreenView)
    {
        TRect MainSubScreenView(innerRect);
        MainSubScreenView.iTl.iX = innerRect.iTl.iX + innerRect.Width()/3;
        MainSubScreenView.SetWidth(innerRect.Width()/1.5);
        MainSubScreenView.Shrink(5,5); // Allow space for border shadows
        iMainSubScreenView->SetRect(MainSubScreenView);
    }
}

TInt CRegisterView::GetMainScreenCurrentItemIndex()
{
	TInt index;
    index = iMainScreenView->CurrentItemIndex();
	return index;
}

TInt CRegisterView::GetMainSubScreenCurrentItemIndex()
{
    TInt index;
	if(iMainSubScreenView->IsFocused())
		index = iMainSubScreenView->CurrentItemIndex();
	else
		index = -1;
	return index;
}

void CRegisterView::ShowStatusScreen()
{
	//iAppUi.ShowWindow(4);
	iAppUi.ShowStatusScreen();
	iMainScreenView->SetFocus(ETrue, EDrawNow);
	iMainSubScreenView->SetFocus(EFalse, EDrawNow);
	itemIndex = 4;
	iMainScreenView->SetCurrentItemIndex(4);
}

void CRegisterView::SelectSubScreen()
{
    iMainScreenView->SetFocus(EFalse, EDrawNow);
    iMainSubScreenView->SetFocus(ETrue, EDrawNow);
}
