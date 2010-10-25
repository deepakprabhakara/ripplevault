/* Copyright (c) 2004, Nokia Mobile Phones. All rights reserved */

#include <cknenv.h>
#include <ckninfo.h>
#include <cknsfld.h>
#include <eikclb.h>
#include <eikclbd.h>
#include <eiktxlbm.h>
#include <ckntitle.h>

#include <Ripplevault_series80.rsg>

#include "ContactsScreenView.h"
#include "icons.mbg"
#include "Ripplevaultappui.h"
#include "ReadContactsAO.h"

/******************************************************************************
 *
 *   CContactsScreenView implementation
 *
 *****************************************************************************/

static const TInt KArrayItemMaxLength = 128;
static const TUint32 KFocusedBkgColor = 0xCCCCCC;
static const TUint32 KNonFocusedBkgColor = 0xEEEEEE;

void CContactsScreenView::ConstructL(CCoeControl* aWindowOwningContainer)
{
    // Application title
    iAppTitle = CCknAppTitle::NewL(CCknAppTitle::EWindow);
    iAppTitle->SetContainerWindowL(*this);
    iAppTitle->SetTextL(*(iEikonEnv->AllocReadResourceL(R_LIST_MAINMENU_CONTACTS)), CCknAppTitle::EMainTitle);

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
		colData->SetColumnWidthPixelL(0, 120);       
    }

	iListbox->SetItemHeightL(20);

	iContactsDB = CContactDatabase::OpenL();

	CContactIdArray* cArray = (CContactIdArray*)iContactsDB->SortedItemsL();
	CContactIdArray& cArr = *cArray;

	iContactsCount = cArray->Count();

	cArray->Reset();
	cArr.Reset();

	if (iContactsCount > 0)
	{
		// Reschedule ourselves.	  
		iContactsIdArray = new CArrayFixFlat<TInt>(iContactsCount);
		iContactsArray = new (ELeave) CDesCArrayFlat(iContactsCount);
		iReadContactsAO = NULL;
		
		//CEikonEnv::Static ()->BusyMsgL(_L("Busy"));
		iAppUi.DisplayProgressBallL(R_TEXT_SCANNING_PROGRESS);

		iReadContactsAO = new (ELeave) CReadContactsAO(*this);
		iReadContactsAO->StartL();
	}
	else	  
	{
		iContactsIdArray = new CArrayFixFlat<TInt>(1);
		iContactsArray = new (ELeave) CDesCArrayFlat(1);
		
		//CEikonEnv::Static ()->BusyMsgL(_L("No Contacts"));
	}
}

CContactsScreenView::CContactsScreenView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
    // No implementation required
}


CContactsScreenView::~CContactsScreenView()
{	
	//CEikonEnv::Static ()->BusyMsgCancel();
	iAppUi.DestroyProgressBall();

    delete iAppTitle;
    delete iListbox;

	delete iContactsIdArray;
	delete iContactsDB;

	if (iReadContactsAO != NULL)
	{
		delete iReadContactsAO;
		iReadContactsAO = NULL;
	}
}

// Return currently highlighted listbox item index.
TInt CContactsScreenView::CurrentItemIndex()
{
    return iListbox->CurrentItemIndex();
}

// Set data to the listbox.
void CContactsScreenView::SetData()
{
	iListbox->HandleItemRemovalL();

	CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(1);

	if(iContactsCount > 0)
	{
		listItems->Reset();
		CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(iContactsCount);
	}

    CleanupStack::PushL(listItems);
	
	for (TInt i=0; i < iContactsCount ; i++)
	{
		listItems->AppendL((*iContactsArray)[i]);	
	}
        
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
void CContactsScreenView::Draw(const TRect& aRect) const
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
TKeyResponse CContactsScreenView::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,TEventCode aType)
{
    return iListbox->OfferKeyEventL(aKeyEvent, aType);
}

// Return number of controls within this container. Called by framework.
TInt CContactsScreenView::CountComponentControls() const
{
    return 2;
}

// Return controls within this container. Called by framework to draw them.
CCoeControl* CContactsScreenView::ComponentControl(TInt aIndex) const
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
TSize CContactsScreenView::MinimumSize()
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
void CContactsScreenView::SizeChanged()
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
void CContactsScreenView::FocusChanged(TDrawNow /*aDrawNow*/)
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
void CContactsScreenView::HandleControlEventL(CCoeControl* aControl,
    TCoeEvent aEventType)
{
    if(aEventType == EEventStateChanged && aControl == iListbox)
        {
        }
}

TInt CContactsScreenView::GetContacts(TInt aContactId, TDesC& aContactDetails, TDesC& aMobileDetails)
{
	TBuf<150> fname;	
	TBuf<150> lname;	
	TBuf<150> mobile;

	iContactsDB = CContactDatabase::OpenL();

	CContactIdArray* cArray = (CContactIdArray*)iContactsDB->SortedItemsL();
	CContactIdArray& cArr = *cArray;

	CContactItem* contact;

	TInt cid=(*cArray)[aContactId];

	contact = iContactsDB->ReadContactL(cid);

	CContactItemFieldSet& fieldSet = contact->CardFields();		   

	GetTextFieldFromFieldSet(fieldSet, KUidContactFieldGivenName, fname);

	GetTextFieldFromFieldSet(fieldSet, KUidContactFieldFamilyName, lname);
		
	if(lname.Compare(_L("")) !=0 )
	{
		lname.Append(_L(" "));
		fname.Insert(0,lname);
	}
		
	((TDes&)aContactDetails).Copy(fname);

	iAppUi.numbers = new (ELeave) CDesCArrayFlat(50);

	for (int i = 0; i < fieldSet.Count(); i++) 
	{				
		// Home mobile nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber)
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) 
		{
			CContactItemField& textfield = fieldSet[i];
			CContactTextField* textValue = textfield.TextStorage();
			((TDes&)mobile).Copy(textValue->Text());
			iAppUi.numbers->AppendL(mobile);
			continue;
		}				
		// Work mobile nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) 
		{
			CContactItemField& textfield = fieldSet[i];
			CContactTextField* textValue = textfield.TextStorage();
			((TDes&)mobile).Copy(textValue->Text());
			iAppUi.numbers->AppendL(mobile);
			continue;
		}								
		// Mobile nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) 
		{
			CContactItemField& textfield = fieldSet[i];
			CContactTextField* textValue = textfield.TextStorage();
			((TDes&)mobile).Copy(textValue->Text());			
			iAppUi.numbers->AppendL(mobile);
			continue;
		}
		// Home Telephone nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) 
		{
			CContactItemField& textfield = fieldSet[i];
			CContactTextField* textValue = textfield.TextStorage();
			((TDes&)mobile).Copy(textValue->Text());
			iAppUi.numbers->AppendL(mobile);
			continue;
		}
		// Work Telephone nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) 
		{
			CContactItemField& textfield = fieldSet[i];
			CContactTextField* textValue = textfield.TextStorage();
			((TDes&)mobile).Copy(textValue->Text());
			iAppUi.numbers->AppendL(mobile);
			continue;
		}
		// Telephones
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) 
		{
			CContactItemField& textfield = fieldSet[i];
			CContactTextField* textValue = textfield.TextStorage();
			((TDes&)mobile).Copy(textValue->Text());
			iAppUi.numbers->AppendL(mobile);
			continue;
		}
	}

	TInt ret;

	if(iAppUi.numbers->MdcaCount() > 1 )
	{
		ret = iAppUi.ShowContactsDialog(fname);

		if( ret == 7)
			ret = 0;
		else
		{			
			((TDes&)aMobileDetails).Copy(iAppUi.iMobBuffer);
			ret = 1;
		}
	}
	else
	{
		TPtrC ptr1(iAppUi.numbers->MdcaPoint(0));
		((TDes&)aMobileDetails).Copy(ptr1);
		ret = 1;
	}
		
	delete contact;

	cArray->Reset();
	cArr.Reset();

	return ret;
}

void CContactsScreenView::GetTextFieldFromFieldSet(CContactItemFieldSet& aFieldSet, const TUid aField,TDesC& name)
{
	TInt index = aFieldSet.Find(aField);

    if ((index >= 0) && (index < aFieldSet.Count()))
    {
   	    CContactItemField& textfield = aFieldSet[index];
        CContactTextField* textValue = textfield.TextStorage();

		((TDes&)name).Copy(textValue->Text());		
    } 
}

void CContactsScreenView::CloseReadContact()
{
	//CEikonEnv::Static ()->BusyMsgCancel();
	iAppUi.DestroyProgressBall();  

	// Handle the addition of items to the list box model. 
	iListbox->HandleItemAdditionL(); // This also redraws the list box.

	iListbox->DrawNow();

	if (iReadContactsAO != NULL)
	{
		delete iReadContactsAO;
		iReadContactsAO = NULL;
	}

	SetData();
}