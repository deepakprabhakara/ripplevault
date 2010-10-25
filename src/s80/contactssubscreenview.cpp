/* Copyright (c) 2004, Nokia Mobile Phones. All rights reserved */

#include <cknenv.h>
#include <ckninfo.h>
#include <cknsfld.h>
#include <eikclb.h>
#include <eikclbd.h>
#include <eiktxlbm.h>
#include <ckntitle.h>

#include <Ripplevault_series80.rsg>

#include "ContactsSubScreenView.h"
#include "Ripplevaultappui.h"

/******************************************************************************
 *
 *   CContactsSubScreenView implementation
 *
 *****************************************************************************/

static const TInt KArrayItemMaxLength = 128;
static const TUint32 KFocusedBkgColor = 0xCCCCCC;
static const TUint32 KNonFocusedBkgColor = 0xEEEEEE;

void CContactsSubScreenView::ConstructL(CCoeControl* aWindowOwningContainer)
{
    // Application title
    iAppTitle = CCknAppTitle::NewL(CCknAppTitle::EWindow);
    iAppTitle->SetContainerWindowL(*this);

	TBuf<50> titlename;
	TBuf<50> formatBuf;

	titlename.Copy(_L(""));
	formatBuf.Copy(_L(""));
	formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_CONTACTS_UPLOADED)));
	titlename.Format(formatBuf, 0);

    iAppTitle->SetTextL(titlename, CCknAppTitle::EMainTitle);

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

	// Set the listbox's first column to fill the listbox's whole width
    CColumnListBoxData* colData = iListbox->ItemDrawer()->ColumnData();
    if(colData)
    {
		colData->SetColumnWidthPixelL(0, 200);       
    }

	iListbox->SetItemHeightL(30);

	if(iAppUi.iStoragePath == 1)
	{
	  CDesCArray* smsnames = iAppUi.GetSmsNameList();	

	  CDesCArray* smsmobiles = iAppUi.GetSmsMobileList();

	  if(smsnames->Count() == 0)
	  {
		iContactsArray = new (ELeave) CDesCArrayFlat(1);
		iMobileArray   = new (ELeave) CDesCArrayFlat(1);
	  }
	  else
	  {
		iContactsArray = new (ELeave) CDesCArrayFlat(smsnames->Count());
		iMobileArray   = new (ELeave) CDesCArrayFlat(smsnames->Count());
	  }

	  for(int i = 0 ; i < smsnames->Count() ; i++)
	  {
		 iContactsArray->AppendL(smsnames->MdcaPoint(i)); 
		 iMobileArray->AppendL(smsmobiles->MdcaPoint(i)); 
	  }	

	  SetData();
	}
	else
	{
		iContactsArray = new (ELeave) CDesCArrayFlat(12);
		iMobileArray   = new (ELeave) CDesCArrayFlat(12);
	}
}

CContactsSubScreenView::CContactsSubScreenView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
    // No implementation required
}


CContactsSubScreenView::~CContactsSubScreenView()
{
    delete iAppTitle;
    delete iListbox;
}

// Return currently highlighted listbox item index.
TInt CContactsSubScreenView::CurrentItemIndex()
{
    return iListbox->CurrentItemIndex();
}

// Draw this control to the screen. Draw the area empty with
// appropriate color. If the control is focused, draw shadow border.
void CContactsSubScreenView::Draw(const TRect& aRect) const
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
TKeyResponse CContactsSubScreenView::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,TEventCode aType)
{
    return iListbox->OfferKeyEventL(aKeyEvent, aType);
}

// Return number of controls within this container. Called by framework.
TInt CContactsSubScreenView::CountComponentControls() const
{
    return 2;
}

// Return controls within this container. Called by framework to draw them.
CCoeControl* CContactsSubScreenView::ComponentControl(TInt aIndex) const
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
TSize CContactsSubScreenView::MinimumSize()
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
void CContactsSubScreenView::SizeChanged()
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
void CContactsSubScreenView::FocusChanged(TDrawNow /*aDrawNow*/)
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
void CContactsSubScreenView::HandleControlEventL(CCoeControl* aControl,
    TCoeEvent aEventType)
{
    if(aEventType == EEventStateChanged && aControl == iListbox)
        {
        }
}

// Set data to the listbox.
void CContactsSubScreenView::SetData()
{
	TBuf<50> titlename;
	TBuf<50> formatBuf;

	titlename.Copy(_L(""));
	formatBuf.Copy(_L(""));
	formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_CONTACTS_UPLOADED)));
	titlename.Format(formatBuf, iContactsArray->MdcaCount());

    iAppTitle->SetTextL(titlename, CCknAppTitle::EMainTitle);

	iListbox->HandleItemRemovalL();

	CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(1);

	if(iContactsArray->MdcaCount() > 0)
	{
		listItems->Reset();
		CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(iContactsArray->MdcaCount());
	}
    CleanupStack::PushL(listItems);

	TBuf<500> item;
    
	for (TInt i = 0; i < iContactsArray->MdcaCount() ; i++)
	{
		item.Copy(_L(""));
		item.Append((*iContactsArray)[i]);
		item.Append(_L(","));
		item.Append((*iMobileArray)[i]);

		listItems->AppendL(item);	
	}
        
	CleanupStack::Pop(listItems);
	
    CTextListBoxModel* model = iListbox->Model();
    model->SetItemTextArray(listItems);

    // Set ListBox model responsible for deleting the listItems array
    model->SetOwnershipType(ELbmOwnsItemArray);
    
    iListbox->HandleItemAdditionL();
    DrawDeferred();
}
