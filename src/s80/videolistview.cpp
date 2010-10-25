/*
* ============================================================================
*  Name     : VideoListView.cpp
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CVideoListView implementation
*  Copyright: PurpleAce
* ============================================================================
*/


#include <cknenv.h>
#include <eikenv.h>
#include <ckntitle.h>

#include "VideoListView.h"
#include "RippleVaultUtils.h"
#include <RippleVault_series80.rsg>
#include "VideoScreenListBox.h"

#include "logo.mbg"
#include "fbs.h"
#include "constants.h"

CVideoListView* CVideoListView::NewL( const TRect& aRect, CRippleVaultAppUi& aAppUi)
{
    CVideoListView* self = new (ELeave) CVideoListView(aAppUi);
    CleanupStack::PushL(self);
    self->ConstructL( aRect );
    CleanupStack::Pop();
    return self;
}

CVideoListView::CVideoListView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
}

void CVideoListView::ConstructL( const TRect& aRect )
{
	iFilename.Copy(_L("c:\\"));

    CreateWindowL();

	// Construct rights side "popup window" looking area
    iVideoScreenListBox = new(ELeave)CVideoScreenListBox(iAppUi);
    iVideoScreenListBox->ConstructL(this,aRect);
    iVideoScreenListBox->SetObserver(this);
    if( iVideoScreenListBox->Start(iFilename) == -1)
		iVideoScreenListBox->ClearData();
    iVideoScreenListBox->SetFocus(ETrue, ENoDrawNow);
    iVideoScreenListBox->SetFocus(ETrue);

	// Set logical border type to EWindow to get the shadow look (See also
    
    iVideoScreenListBox->SetBorder(TGulBorder::EWindow);

    // Set the windows size
    SetRect(aRect);
    // Activate the window, which makes it ready to be drawn
    ActivateL();
}

CVideoListView::~CVideoListView()
{
	delete iVideoScreenListBox;
}

TInt CVideoListView::CountComponentControls() const
{
    return 1;
}

void CVideoListView::SetFocus(TBool aFocus, TDrawNow aDrawNow)
{
}

CCoeControl* CVideoListView::ComponentControl(TInt aIndex) const
{
    switch (aIndex)
        {
		case 0:
			return iVideoScreenListBox;
			break;
		default:
			return NULL;
        }
}

void CVideoListView::Draw(const TRect& /* aRect */) const
{
	CWindowGc& gc = SystemGc();
	gc.Clear();
}

// Handle key events. Called by framework
TKeyResponse CVideoListView::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,TEventCode aType)
{
	if(aType == EEventKey && aKeyEvent.iCode == 13)
	{
		MarkUnMarkSelectedItem();
		return EKeyWasConsumed;
	}

	 // Let the focused control handle any other key events
    if(iVideoScreenListBox->IsFocused())
        return iVideoScreenListBox->OfferKeyEventL(aKeyEvent, aType);
    else
        return EKeyWasNotConsumed; 
}

// Handle search results listbox data or user selection changes. Refresh
// right side view details. This is called by framework, when one of contained
// controls calls their ReportEventL functions.
void CVideoListView::HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType)
{
	if(aEventType == EEventStateChanged && aControl == iVideoScreenListBox)
    {
       // User selected an item from right side listbox. Load the detail
       // data to the right side control
       TInt itemIndex ;//= iVideoScreenListBox->CurrentItemIndex();
       if(itemIndex >= 0)
       {
          //iVideoScreenListBox->SetData();
       }
       else
       {
          //iVideoScreenListBox->SetData();
       }
    }
}

// Set the left and right view controls positions. Called by framework
void CVideoListView::SizeChanged()
{
    TRect innerRect = Border().InnerRect(Rect()); // This should be same as
                                                  // Rect(), because this
                                              // window has no border.
    if(iVideoScreenListBox)
    {
        TRect Videoscreenlistbox(innerRect);
        //Videoscreenlistbox.iTl.iX = innerRect.iTl.iX + innerRect.Width();
    
        Videoscreenlistbox.SetWidth(innerRect.Width());
    
        Videoscreenlistbox.Shrink(5,5); // Allow space for border shadows
        iVideoScreenListBox->SetRect(Videoscreenlistbox);
    }
}

void CVideoListView::MarkUnMarkSelectedItem()
{
	if(iVideoScreenListBox->IsFocused())
		iVideoScreenListBox->MarkUnMarkSelectedItem();
}

void CVideoListView::MarkAllItems()
{
	if(iVideoScreenListBox->IsFocused())
		iVideoScreenListBox->MarkAllItems();
}

void CVideoListView::UnMarkAllItems()
{
	if(iVideoScreenListBox->IsFocused())
		iVideoScreenListBox->UnMarkAllItems();
}

void CVideoListView::SelectFolder()
{
	//Create and launch Select-folder dialog using static function. 
	//Function retrieves selected folder path into iPath. 
	
	if(CCknTargetFolderDialog::RunSelectFolderDlgLD(iFilename,CCknFileListDialogBase::EShowAllDrives))
	{
		//iFilename.Copy(_L("c:\\My Files\\Images1\\"));

		iVideoScreenListBox->ResetData();

		if( iVideoScreenListBox->Start(iFilename) == -1)
			iVideoScreenListBox->ClearData();
	}
}

TInt CVideoListView::IsImageSelected()
{
	return iVideoScreenListBox->IsImageSelected();
}

void CVideoListView::SendVideoToServer()
{
	iVideoScreenListBox->SendVideoToServer();
}
