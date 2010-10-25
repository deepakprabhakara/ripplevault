/*
* ============================================================================
*  Name     : PhotoListView.cpp
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CPhotoListView implementation
*  Copyright: PurpleAce
* ============================================================================
*/


#include <cknenv.h>
#include <eikenv.h>
#include <ckntitle.h>

#include "PhotoListView.h"
#include "RippleVaultUtils.h"
#include <RippleVault_series80.rsg>
#include "PhotoScreenListBox.h"

#include "logo.mbg"
#include "fbs.h"
#include "constants.h"

CPhotoListView* CPhotoListView::NewL( const TRect& aRect, CRippleVaultAppUi& aAppUi)
{
    CPhotoListView* self = new (ELeave) CPhotoListView(aAppUi);
    CleanupStack::PushL(self);
    self->ConstructL( aRect );
    CleanupStack::Pop();
    return self;
}

CPhotoListView::CPhotoListView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
}

void CPhotoListView::ConstructL( const TRect& aRect )
{
	iFilename.Copy(_L("c:\\"));

    CreateWindowL();

	// Construct rights side "popup window" looking area
    iPhotoScreenListBox = new(ELeave)CPhotoScreenListBox(iAppUi);
    iPhotoScreenListBox->ConstructL(this,aRect);
    iPhotoScreenListBox->SetObserver(this);
    if( iPhotoScreenListBox->Start(iFilename) == -1)
		iPhotoScreenListBox->ClearData();
    iPhotoScreenListBox->SetFocus(ETrue, ENoDrawNow);
    iPhotoScreenListBox->SetFocus(ETrue);

	// Set logical border type to EWindow to get the shadow look (See also
    
    iPhotoScreenListBox->SetBorder(TGulBorder::EWindow);

    // Set the windows size
    SetRect(aRect);
    // Activate the window, which makes it ready to be drawn
    ActivateL();
}

CPhotoListView::~CPhotoListView()
{
	delete iPhotoScreenListBox;
}

TInt CPhotoListView::CountComponentControls() const
{
    return 1;
}

void CPhotoListView::SetFocus(TBool aFocus, TDrawNow aDrawNow)
{
}

CCoeControl* CPhotoListView::ComponentControl(TInt aIndex) const
{
    switch (aIndex)
        {
		case 0:
			return iPhotoScreenListBox;
			break;
		default:
			return NULL;
        }
}

void CPhotoListView::Draw(const TRect& /* aRect */) const
{
	CWindowGc& gc = SystemGc();
	gc.Clear();
}

// Handle key events. Called by framework
TKeyResponse CPhotoListView::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,TEventCode aType)
 {	
	if(aType == EEventKey && aKeyEvent.iCode == 13)
	{
		MarkUnMarkSelectedItem();
		return EKeyWasConsumed;
	}

	 // Let the focused control handle any other key events
    if(iPhotoScreenListBox->IsFocused())
        return iPhotoScreenListBox->OfferKeyEventL(aKeyEvent, aType);
    else
        return EKeyWasNotConsumed; 
}

// Handle search results listbox data or user selection changes. Refresh
// right side view details. This is called by framework, when one of contained
// controls calls their ReportEventL functions.
void CPhotoListView::HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType)
{
	if(aEventType == EEventStateChanged && aControl == iPhotoScreenListBox)
    {
       // User selected an item from right side listbox. Load the detail
       // data to the right side control
       TInt itemIndex ;//= iPhotoScreenListBox->CurrentItemIndex();
       if(itemIndex >= 0)
       {
          //iPhotoScreenListBox->SetData();
       }
       else
       {
          //iPhotoScreenListBox->SetData();
       }
    }
}

// Set the left and right view controls positions. Called by framework
void CPhotoListView::SizeChanged()
{
    TRect innerRect = Border().InnerRect(Rect()); // This should be same as
                                                  // Rect(), because this
                                              // window has no border.
    if(iPhotoScreenListBox)
    {
        TRect photoscreenlistbox(innerRect);
        //photoscreenlistbox.iTl.iX = innerRect.iTl.iX + innerRect.Width();
    
        photoscreenlistbox.SetWidth(innerRect.Width());
    
        photoscreenlistbox.Shrink(5,5); // Allow space for border shadows
        iPhotoScreenListBox->SetRect(photoscreenlistbox);
    }
}

void CPhotoListView::MarkUnMarkSelectedItem()
{
	if(iPhotoScreenListBox->IsFocused())
		iPhotoScreenListBox->MarkUnMarkSelectedItem();
}

void CPhotoListView::MarkAllItems()
{
	if(iPhotoScreenListBox->IsFocused())
		iPhotoScreenListBox->MarkAllItems();
}

void CPhotoListView::UnMarkAllItems()
{
	if(iPhotoScreenListBox->IsFocused())
		iPhotoScreenListBox->UnMarkAllItems();
}

void CPhotoListView::SelectFolder()
{
	//Create and launch Select-folder dialog using static function. 
	//Function retrieves selected folder path into iPath. 
	
	if(iPhotoScreenListBox->iPhotoFileIdEnd == -1)
	{
		if(CCknTargetFolderDialog::RunSelectFolderDlgLD(iFilename,CCknFileListDialogBase::EShowAllDrives))
		{
			//iFilename.Copy(_L("c:\\My Files\\Images1\\"));

			iPhotoScreenListBox->ResetData();

			if( iPhotoScreenListBox->Start(iFilename) == -1)
				iPhotoScreenListBox->ClearData();
		}
	}
}

TInt CPhotoListView::IsImageSelected()
{
	return iPhotoScreenListBox->IsImageSelected();
}

void CPhotoListView::SendPhotoToServer()
{
	iPhotoScreenListBox->SendPhotoToServer();
}
