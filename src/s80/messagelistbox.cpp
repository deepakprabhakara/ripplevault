/* Copyright (c) 2004, Nokia Mobile Phones. All rights reserved */

#include <cknenv.h>
#include <ckninfo.h>
#include <cknsfld.h>
#include <eikclb.h>
#include <eikclbd.h>
#include <eiktxlbm.h>
#include <ckntitle.h>

#include <Ripplevault_series80.rsg>

#include "MessageListBox.h"
#include "s80sms.mbg"
#include "Ripplevaultappui.h"

/******************************************************************************
 *
 *   CMessageListBox implementation
 *
 *****************************************************************************/

static const TInt KArrayItemMaxLength = 128;
static const TUint32 KFocusedBkgColor = 0xCCCCCC;
static const TUint32 KNonFocusedBkgColor = 0xEEEEEE;

void CMessageListBox::ConstructL(CCoeControl* aWindowOwningContainer)
{
    // Application title
    iAppTitle = CCknAppTitle::NewL(CCknAppTitle::EWindow);
    iAppTitle->SetContainerWindowL(*this);
    iAppTitle->SetTextL(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SENDTOVAULT)), CCknAppTitle::EMainTitle);

    // Search results listbox
    iListbox = new(ELeave)CEikColumnListBox; // Column listbox with one column
    iListbox->ConstructL(aWindowOwningContainer,
        CEikListBox::ENoExtendedSelection);
    iListbox->CreateScrollBarFrameL(ETrue);
    iListbox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
    iListbox->SetObserver(this); // See HandleControlEventL
    iListbox->SetContainerWindowL(*aWindowOwningContainer);

    SetContainerWindowL(*aWindowOwningContainer);
}

CMessageListBox::CMessageListBox(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
    // No implementation required
}


CMessageListBox::~CMessageListBox()
{
    delete iAppTitle;
    delete iListbox;
}

// Return currently highlighted listbox item index.
TInt CMessageListBox::CurrentItemIndex()
{
    return iListbox->CurrentItemIndex();
}

// Set data to the listbox.
void CMessageListBox::SetData()
{
	iListbox->HandleItemRemovalL();

	SetIconsArray();

    CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(2);
    CleanupStack::PushL(listItems);
    
	TBuf<KArrayItemMaxLength> item;
	_LIT(KTab, "\t");
    
	item.Copy(_L("0"));
    item.Append(KTab);
    item.Append(*iEikonEnv->AllocReadResourceL(R_TAB_INBOX));
    listItems->AppendL(item);
    
    item.Copy(_L("1"));
    item.Append(KTab);
    item.Append(*iEikonEnv->AllocReadResourceL(R_TAB_SENTITEMS));
    listItems->AppendL(item);
        
    CleanupStack::Pop(listItems);

    CTextListBoxModel* model = iListbox->Model();
    model->SetItemTextArray(listItems);

    // Set ListBox model responsible for deleting the listItems array
    model->SetOwnershipType(ELbmOwnsItemArray);
    
    iListbox->HandleItemAdditionL();
    DrawDeferred();
}

// Draw this control to the screen. Draw the area empty with
// appropriate color. If the control is focused, draw shadow border.
void CMessageListBox::Draw(const TRect& aRect) const
{
    CWindowGc& gc = SystemGc();
    gc.SetClippingRect(aRect);
    gc.Clear(iBorder.OuterRect(aRect));
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);

    if (IsFocused())
        {
        CEikBorderedControl::Draw(iBorder.OuterRect(aRect)); // Shadow border
        gc.SetBrushColor(TRgb(KFocusedBkgColor));
        }
    else
        {
        gc.SetBrushColor(TRgb(KNonFocusedBkgColor));
        }

    // Fill the controls background. The color depends on the focus status. In
    // this example this has no effect, because contained controls are drawn
    // over the same area. Here just for demonstrating design principle.
    gc.DrawRect(Border().InnerRect(Rect()));
}

// Handle key events. Called by owning control (CPhonebookExAppView)
TKeyResponse CMessageListBox::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,TEventCode aType)
{
    return iListbox->OfferKeyEventL(aKeyEvent, aType);
}

// Return number of controls within this container. Called by framework.
TInt CMessageListBox::CountComponentControls() const
{
    return 2;
}

// Return controls within this container. Called by framework to draw them.
CCoeControl* CMessageListBox::ComponentControl(TInt aIndex) const
{
    switch(aIndex)
        {
        case 0:
            return iAppTitle;
        case 1:
            return iListbox;
        default:
            return NULL;
        }
}

// Provide minimum size information for this control.
TSize CMessageListBox::MinimumSize()
{
    // Find the biggest minimum width of the two contained controls
    TInt minimumWidth = iAppTitle->MinimumSize().iWidth;
    if(iListbox->MinimumSize().iWidth > minimumWidth)
        minimumWidth = iListbox->MinimumSize().iWidth;

    // Calculate sum of minimum heights of the contained controls.
    TInt minimumHeight = iAppTitle->MinimumSize().iHeight +
                         iListbox->MinimumSize().iHeight;
    return TSize(minimumWidth, minimumHeight);
}

// Handle size changes. Set the contained controls extents. Called by framework
void CMessageListBox::SizeChanged()
{
    innerRect = Border().InnerRect(Rect());

    // Application Title rect. Fills the controls top area
    TSize titleSize(innerRect.Width(), iAppTitle->MinimumSize().iHeight);
    TRect titleRect(innerRect.iTl, titleSize);
    iAppTitle->SetRect(titleRect);

    // List box size and rect. Fills the remaining area
    TSize listBoxSize(innerRect.Width(), innerRect.Height() -
        iAppTitle->MinimumSize().iHeight);
    TRect listBoxRect(TPoint(titleRect.iTl.iX, titleRect.iBr.iY+1),
        listBoxSize);

    iListbox->SetRect(listBoxRect);
}

// Framework calls this method, when SetFocus has been called for this control
void CMessageListBox::FocusChanged(TDrawNow /*aDrawNow*/)
{
    // If the view is focused, set all it's components to be focused.
    if (IsFocused())
    {
        iAppTitle->SetFocus(ETrue);
        iListbox->SetFocus(ETrue);
    }
    else
    {
        iAppTitle->SetFocus(EFalse);
        iListbox->SetFocus(EFalse);
    }
    DrawDeferred();
}

// Handle events that occur in the contained controls. Report also on events to
// container of this control
void CMessageListBox::HandleControlEventL(CCoeControl* aControl,
    TCoeEvent aEventType)
{
    if(aEventType == EEventStateChanged && aControl == iListbox)
        {
        }
}

void CMessageListBox::SetIconsArray()
{
	CArrayPtr<CGulIcon>* iconArray = new (ELeave)
		CArrayPtrFlat<CGulIcon>(2);

	CleanupStack::PushL(iconArray);

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KS80SmsMbmFileName);

	iconArray->AppendL(iEikonEnv->CreateIconL(path, 
			EMbmS80smsInbox, EMbmS80smsInboxm));
	
	iconArray->AppendL(iEikonEnv->CreateIconL(path, 
			EMbmS80smsSent, EMbmS80smsSentm));
	

	CleanupStack::Pop(); // iconArray

	// Set the listbox's first column to fill the listbox's whole width
    CColumnListBoxData* colData = iListbox->ItemDrawer()->ColumnData();
    if(colData)
    {
		colData->SetIconArray(iconArray);

		colData->SetGraphicsColumnL(0, ETrue);
        colData->SetColumnWidthPixelL(0, 30);        
		colData->SetColumnAlignmentL(0, CGraphicsContext::ELeft); 

		colData->SetColumnWidthPixelL(1, 100);       
    }

	iListbox->SetItemHeightL(30);
}
