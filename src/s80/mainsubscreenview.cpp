/* Copyright (c) 2004, Nokia Mobile Phones. All rights reserved */

#include <cknenv.h>
#include <ckninfo.h>
#include <cknsfld.h>
#include <eikclb.h>
#include <eikclbd.h>
#include <eiktxlbm.h>
#include <ckntitle.h>

#include <Ripplevault_series80.rsg>

#include "MainSubScreenView.h"
#include "subicons.mbg"
#include "Ripplevaultappui.h"

/******************************************************************************
 *
 *   CMainSubScreenView implementation
 *
 *****************************************************************************/

static const TInt KArrayItemMaxLength = 128;
static const TUint32 KFocusedBkgColor = 0xCCCCCC;
static const TUint32 KNonFocusedBkgColor = 0xEEEEEE;

void CMainSubScreenView::ConstructL(CCoeControl* aWindowOwningContainer)
{
    // Application title
    iAppTitle = CCknAppTitle::NewL(CCknAppTitle::EWindow);
    iAppTitle->SetContainerWindowL(*this);
 
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

CMainSubScreenView::CMainSubScreenView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
    // No implementation required
}


CMainSubScreenView::~CMainSubScreenView()
{
    delete iAppTitle;
    delete iListbox;
}

// Return currently highlighted listbox item index.
TInt CMainSubScreenView::CurrentItemIndex()
{
    return iListbox->CurrentItemIndex();
}

// Draw this control to the screen. Draw the area empty with
// appropriate color. If the control is focused, draw shadow border.
void CMainSubScreenView::Draw(const TRect& aRect) const
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
TKeyResponse CMainSubScreenView::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,TEventCode aType)
{
    return iListbox->OfferKeyEventL(aKeyEvent, aType);
}

// Return number of controls within this container. Called by framework.
TInt CMainSubScreenView::CountComponentControls() const
{
    return 2;
}

// Return controls within this container. Called by framework to draw them.
CCoeControl* CMainSubScreenView::ComponentControl(TInt aIndex) const
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
TSize CMainSubScreenView::MinimumSize()
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
void CMainSubScreenView::SizeChanged()
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
void CMainSubScreenView::FocusChanged(TDrawNow /*aDrawNow*/)
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
void CMainSubScreenView::HandleControlEventL(CCoeControl* aControl,
    TCoeEvent aEventType)
{
    if(aEventType == EEventStateChanged && aControl == iListbox)
        {
        }
}

// Set data to the listbox.
void CMainSubScreenView::SetData(TInt val)
{
	iAppUi.DeleteWindow(KViewIdStatusScreen);
	
	iListbox->HandleItemRemovalL();

	TBuf<30> titleName;
	if(val == 0)
	{
		titleName.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_CONTACTS)));
		iAppTitle->SetTextL(titleName, CCknAppTitle::EMainTitle);
	}
	else if(val == 1)
	{
		titleName.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_PHOTOS)));
		iAppTitle->SetTextL(titleName, CCknAppTitle::EMainTitle);
	}
	else if(val == 2)
	{
		titleName.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_VIDEOS)));
		iAppTitle->SetTextL(titleName, CCknAppTitle::EMainTitle);
	}
	else if(val == 3)
	{
		titleName.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_SMS)));
		iAppTitle->SetTextL(titleName, CCknAppTitle::EMainTitle);
	}
	else if(val == 4)
	{
		titleName.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_STATUS)));
		iAppTitle->SetTextL(titleName, CCknAppTitle::EMainTitle);
	}

	SetIconsArray(val);
	
	CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(1);
	listItems->Reset();
	delete listItems;

	if(val == 0)
	{
		CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(3);
		CleanupStack::PushL(listItems);
    
		TBuf<KArrayItemMaxLength> item;
		_LIT(KTab, "\t");
    
		for (TInt i = 0; i < 3 ; i++)
		{
			item.Copy(_L(""));
			item.AppendNum(i);
			item.Append(KTab);    
    
			if(i == 0)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_CONTACTS_VIEW)));
			else if(i == 1)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_CONTACTS_SENDTOVAULT)));
			else if(i == 2)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_CONTACTS_SETTINGS)));
		
			listItems->AppendL(item);	
		}
        
		CleanupStack::Pop(listItems);
	}
	else if(val == 1)
	{
		CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(4);
		CleanupStack::PushL(listItems);
    
		TBuf<KArrayItemMaxLength> item;
		_LIT(KTab, "\t");
    
		for (TInt i = 0; i < 5 ; i++)
		{
			item.Copy(_L(""));
			item.AppendNum(i);
			item.Append(KTab);    
    
			if(i == 0)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_VIEW)));
			else if(i == 1)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SENDTOVAULT)));
			else if(i == 2)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SHARE)));
			else if(i == 3)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_UNSHARE)));
			else if(i == 4)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SETTINGS)));

			listItems->AppendL(item);	
		}
        
		CleanupStack::Pop(listItems);
	}
	else if(val == 2)
	{
		CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(4);
		CleanupStack::PushL(listItems);
    
		TBuf<KArrayItemMaxLength> item;
		_LIT(KTab, "\t");
    
		for (TInt i = 0; i < 5 ; i++)
		{
			item.Copy(_L(""));
			item.AppendNum(i);
			item.Append(KTab);    
    
			if(i == 0)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_VIEW)));
			else if(i == 1)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SENDTOVAULT)));
			else if(i == 2)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SHARE)));
			else if(i == 3)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_UNSHARE)));
			else if(i == 4)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SETTINGS)));

			listItems->AppendL(item);	
		}
        
		CleanupStack::Pop(listItems);
	}
	else if(val == 3)
	{
		CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(3);
		CleanupStack::PushL(listItems);
    
		TBuf<KArrayItemMaxLength> item;
		_LIT(KTab, "\t");
    
		for (TInt i = 0; i < 3 ; i++)
		{
			item.Copy(_L(""));
			item.AppendNum(i);
			item.Append(KTab);    
    
			if(i == 0)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_SMS_VIEW)));
			else if(i == 1)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_SMS_SENDTOVAULT)));
			else if(i == 2)
				item.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_SMS_SETTINGS)));

			listItems->AppendL(item);	
		}
        
		CleanupStack::Pop(listItems);
	}
	else if(val == 4)
	{
		CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(1);
		CleanupStack::PushL(listItems);
            
		CleanupStack::Pop(listItems);

		iAppUi.ShowWindow(KViewIdStatusScreen);
		iAppUi.ShowStatusScreen();
	}

    CTextListBoxModel* model = iListbox->Model();
    model->SetItemTextArray(listItems);

    // Set ListBox model responsible for deleting the listItems array
    model->SetOwnershipType(ELbmOwnsItemArray);
    
    iListbox->HandleItemAdditionL();
    DrawDeferred();
}

void CMainSubScreenView::SetIconsArray(TInt val)
{
	CArrayPtr<CGulIcon>* iconArray = new (ELeave) CArrayPtrFlat<CGulIcon>(1);

	iconArray->Reset();
	delete iconArray;

	if(val == 0)
	{
		CArrayPtr<CGulIcon>* iconArray = new (ELeave) CArrayPtrFlat<CGulIcon>(3);
		CleanupStack::PushL(iconArray);

		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KSubIconsMbmFileName);

		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsContactsview, EMbmSubiconsContactsviewm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsPhotossendtovault, EMbmSubiconsPhotossendtovaultm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsPhotossendtovault, EMbmSubiconsPhotossendtovaultm));
		
		CleanupStack::Pop(); 
	}
	else if(val == 1)
	{
		CArrayPtr<CGulIcon>* iconArray = new (ELeave) CArrayPtrFlat<CGulIcon>(4);
		CleanupStack::PushL(iconArray);

		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KSubIconsMbmFileName);

		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsPhotosviewgallery, EMbmSubiconsPhotosviewgallerym));
		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsPhotossendtovault, EMbmSubiconsPhotossendtovaultm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsPhotossharealbum, EMbmSubiconsPhotossharealbumm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsVideosunsharealbum,EMbmSubiconsVideosunsharealbumm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsListsettings, EMbmSubiconsListsettingsm));
		
		CleanupStack::Pop(); // iconArray
	}
	else if(val == 2)
	{
		CArrayPtr<CGulIcon>* iconArray = new (ELeave) CArrayPtrFlat<CGulIcon>(4);
		CleanupStack::PushL(iconArray);

		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KSubIconsMbmFileName);

		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsVideosviewgallery, EMbmSubiconsVideosviewgallerym));
		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsVideossendtovault, EMbmSubiconsVideossendtovaultm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsVideossharealbum, EMbmSubiconsVideossharealbumm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsVideosunsharealbum,EMbmSubiconsVideosunsharealbumm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsListsettings, EMbmSubiconsListsettingsm));

		CleanupStack::Pop(); // iconArray
	}
	else if(val == 3)
	{
		CArrayPtr<CGulIcon>* iconArray = new (ELeave) CArrayPtrFlat<CGulIcon>(3);
		CleanupStack::PushL(iconArray);

		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KSubIconsMbmFileName);

		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsSmsview, EMbmSubiconsSmsviewm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsSmssendtovault, EMbmSubiconsSmssendtovaultm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path,	EMbmSubiconsListsettings, EMbmSubiconsListsettingsm));

		CleanupStack::Pop(); // iconArray
	}
	else if(val == 4)
	{
		CArrayPtr<CGulIcon>* iconArray = new (ELeave) CArrayPtrFlat<CGulIcon>(3);
		CleanupStack::PushL(iconArray);

		CleanupStack::Pop(); // iconArray
	}

	// Set the listbox's first column to fill the listbox's whole width
    CColumnListBoxData* colData = iListbox->ItemDrawer()->ColumnData();
    if(colData)
    {
		colData->SetIconArray(iconArray);

		colData->SetGraphicsColumnL(0, ETrue);
        colData->SetColumnWidthPixelL(0, 30);        
		colData->SetColumnAlignmentL(0, CGraphicsContext::ELeft); 

		colData->SetColumnWidthPixelL(1, 200);       
    }

	iListbox->SetItemHeightL(40);
}
