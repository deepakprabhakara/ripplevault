/* Copyright (c) 2004, Nokia Mobile Phones. All rights reserved */

#include <cknenv.h>
#include <ckninfo.h>
#include <Ripplevault_series80.rsg>
#include <ckntitle.h>
#include <cknsfld.h>
#include <eikclb.h>
#include <eikclbd.h>
#include <eiktxlbm.h>
#include <ckntitle.h>
#include <eikclb.h>

#include "MessageTextListBox.h"
#include <eikappui.h>
#include "Ripplevaultappui.h"
#include "CustomListBox.h"
#include "S80sms.mbg"
#include "tick.mbg"
#include "ReadSmsAO.h"
#include "WriteSmsAO.h"

/******************************************************************************
 *
 *   CMessageTextListBox implementation
 *
 *****************************************************************************/

// Background colour depending on the controls focus
static const TUint32 KFocusedBkgColor = 0xCCCCCC;
static const TUint32 KNonFocusedBkgColor = 0xEEEEEE;

// Label name for this control

// Listbox properties
static const TInt KLabelFontHeightInTwips = 180;  // Normal font height is ~200
static const TInt KDataFontHeightInTwips = 180;   // Normal font height is ~200
static const TInt KHorizontalGapBetweenColumns = 5; // Gap betw label and value

void CMessageTextListBox::ConstructL(CCoeControl* aWindowOwningContainer,TRect aRect)
{
	SetContainerWindowL(*aWindowOwningContainer);

	// Application title
    iAppTitle = CCknAppTitle::NewL(CCknAppTitle::EWindow);
    iAppTitle->SetContainerWindowL(*this);
	iAppTitle->SetTextL(*iEikonEnv->AllocReadResourceL(R_TAB_INBOX), CCknAppTitle::EMainTitle);
    
    // Sms details listbox:
    iListbox = new(ELeave)CCustomListBox();

    iListbox->ConstructL(aWindowOwningContainer,
        CEikListBox::ENoExtendedSelection);

    iListbox->CreateScrollBarFrameL(ETrue);

    iListbox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

	iAppUi.iImageFlag = 5;

	iTempCount = 0;

	iReadSmsAO = NULL;	
	iReadSmsAO = new (ELeave) CReadSmsAO(*this);

	SetIconsArray();

	GetSms(0);

	GetSms(1);

	iReadSmsAO->StartL();

	SetData(0);
}


CMessageTextListBox::CMessageTextListBox(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
    // No implementation required
}


CMessageTextListBox::~CMessageTextListBox()
{
	if(InboxSmsMessagearray)
	{
		InboxSmsMessagearray->Reset();
	}

	if(InboxSmsDetailsarray)
	{
		InboxSmsDetailsarray->Reset();
	}

	if(InboxSmsDatearray)
	{
		InboxSmsDatearray->Reset();
	}
	
	if(SentSmsMessagearray)
	{
		SentSmsMessagearray->Reset();
	}

	if(SentSmsDetailsarray)
	{
		SentSmsDetailsarray->Reset();
	}

	if(SentSmsDatearray)
	{
		SentSmsDatearray->Reset();
	}

	if (iReadSmsAO != NULL)
	{
		delete iReadSmsAO;
		iReadSmsAO = NULL;
	}

    delete iAppTitle;
    delete iListbox;
}

// Set app title to the listbox
void CMessageTextListBox::SetTitle(TInt aTitle)
{
	if(aTitle == 0)
		iAppTitle->SetTextL(*iEikonEnv->AllocReadResourceL(R_TAB_INBOX), CCknAppTitle::EMainTitle);
	else	
		iAppTitle->SetTextL(*iEikonEnv->AllocReadResourceL(R_TAB_SENTITEMS), CCknAppTitle::EMainTitle);
    //iAppTitle->SetTextL(subtitleName, CCknAppTitle::ETitleExtension);

    DrawDeferred();
}

void CMessageTextListBox::SetFileList(TInt aIsmark)
{
	TInt count=0;
	if(iAppUi.iImageFlag == 5)
		count = iInboxCount;
	else if(iAppUi.iImageFlag == 6)
		count = iSentItemCount;

	CTextListBoxModel* model = iListbox->Model();  // not taking ownership
	model->SetOwnershipType (ELbmOwnsItemArray);
	CDesCArray* dataArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
	
	if(iTempCount != count)
	if(count != 0)
	{		
		TInt i;
		i = count + 1;

		TBuf<3000> item;
		item.Copy(_L(""));
		
		if(aIsmark == 0)
		{
			if(iAppUi.iImageFlag == 5)
				item.Format(_L("%d\t%S\t%S\t%d"),2 , &(*InboxSmsDetailsarray)[i-2], &(*InboxSmsMessagearray)[i-2], 1); 
			else if(iAppUi.iImageFlag == 6)
				item.Format(_L("%d\t%S\t%S\t%d"),2 , &(*SentSmsDetailsarray)[i-2], &(*SentSmsMessagearray)[i-2], 1); 
		}
		else
		{
			if(iAppUi.iImageFlag == 5)
			{
				if(iInboxSelectedSms->At(i-2) == 10000)
				{
					item.Format(_L("%d\t%S\t%S\t%d"),2 , &(*InboxSmsDetailsarray)[i-2], &(*InboxSmsMessagearray)[i-2], 1);
				}
				else
				{
					item.Format(_L("%d\t%S\t%S\t%d"),2 , &(*InboxSmsDetailsarray)[i-2], &(*InboxSmsMessagearray)[i-2], 0); 
				}
			}
			else if(iAppUi.iImageFlag == 6)
			{
				if(iSentItemSelectedSms->At(i-2) == 10000)
				{
					item.Format(_L("%d\t%S\t%S\t%d"),2 , &(*SentSmsDetailsarray)[i-2], &(*SentSmsMessagearray)[i-2], 1);
				}
				else
				{
					item.Format(_L("%d\t%S\t%S\t%d"),2 , &(*SentSmsDetailsarray)[i-2], &(*SentSmsMessagearray)[i-2], 0); 
				}
			}
		}		

		dataArray->AppendL(item);

		iTempCount = count;
	}

	if(iCursorPos >= 0)
		iListbox->SetCurrentItemIndex(iCursorPos);

	iListbox->HandleItemAdditionL();
}

// Set data values to the listbox
void CMessageTextListBox::SetData(TInt aIsmark)
{
	TInt count;
	if(iAppUi.iImageFlag == 5)
		count = iInboxCount;
	else if(iAppUi.iImageFlag == 6)
		count = iSentItemCount;

	iTempCount = count;

    // Construct listbox items ()
	CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(1);

	if(count != 0)
	{
		listItems->Reset();
		CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(count);
	}
	
    CleanupStack::PushL(listItems);

	TBuf<2000> bufc;

	iListbox->HandleItemRemovalL();

	for (TInt i=2;i<count+2;i++)
	{
		bufc.Copy(_L(""));
		
		if(aIsmark == 0)
		{
			if(iAppUi.iImageFlag == 5)
				bufc.Format(_L("%d\t%S\t%S\t%d"),2 , &(*InboxSmsDetailsarray)[i-2], &(*InboxSmsMessagearray)[i-2], 1); 
			else if(iAppUi.iImageFlag == 6)
				bufc.Format(_L("%d\t%S\t%S\t%d"),2 , &(*SentSmsDetailsarray)[i-2], &(*SentSmsMessagearray)[i-2], 1); 
		}
		else
		{
			if(iAppUi.iImageFlag == 5)
			{
				if(iInboxSelectedSms->At(i-2) == 10000)
				{
					bufc.Format(_L("%d\t%S\t%S\t%d"),2 , &(*InboxSmsDetailsarray)[i-2], &(*InboxSmsMessagearray)[i-2], 1); 
				}
				else
				{
					bufc.Format(_L("%d\t%S\t%S\t%d"),2 , &(*InboxSmsDetailsarray)[i-2], &(*InboxSmsMessagearray)[i-2], 0); 
				}
			}
			else if(iAppUi.iImageFlag == 6)
			{
				if(iSentItemSelectedSms->At(i-2) == 10000)
				{
					bufc.Format(_L("%d\t%S\t%S\t%d"),2 , &(*SentSmsDetailsarray)[i-2], &(*SentSmsMessagearray)[i-2], 1); 
				}
				else
				{
					bufc.Format(_L("%d\t%S\t%S\t%d"),2 , &(*SentSmsDetailsarray)[i-2], &(*SentSmsMessagearray)[i-2], 0); 
				}
			}
		}
		
		listItems->AppendL(bufc);
	}

    CleanupStack::Pop(listItems);

    CTextListBoxModel* model = iListbox->Model();
    model->SetItemTextArray(listItems);

	if(iCursorPos >= 0)
		iListbox->SetCurrentItemIndex(iCursorPos);

    iListbox->HandleItemAdditionL();
    // Set model responsible for deleting the listItems array
    model->SetOwnershipType(ELbmOwnsItemArray);
    DrawDeferred();
}

void CMessageTextListBox::GetTopIndex()
{
	iCursorPos = iListbox->TopItemIndex();
}

// Draw this control to the screen. Draw the area empty with
// appropriate color. If the control is focused, draw shadow border.
void CMessageTextListBox::Draw(const TRect& aRect) const
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
TKeyResponse CMessageTextListBox::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,TEventCode aType)
{
    return iListbox->OfferKeyEventL(aKeyEvent, aType);
}

// Return number of controls within this container. Called by framework.
TInt CMessageTextListBox::CountComponentControls() const
{
    return 2;
}

// Return controls within this container. Called by framework to draw them.
CCoeControl* CMessageTextListBox::ComponentControl(TInt aIndex) const
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
TSize CMessageTextListBox::MinimumSize()
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
void CMessageTextListBox::SizeChanged()
{
    TRect innerRect = Border().InnerRect(Rect());
	innerRect.iBr.iX = innerRect.iTl.iX + innerRect.Width();

    // Application Title. Fill the top of the control
    TRect titleRect(innerRect.iTl,
        TSize(innerRect.Width(), iAppTitle->MinimumSize().iHeight));
    iAppTitle->SetRect(titleRect);
	
    // Listbox. Fill the remaining control area
    iListbox->SetRect(
        TRect(innerRect.iTl + TPoint(0, iAppTitle->Size().iHeight + 1),
            innerRect.iBr));
}

// Called by framework when focus has changed for this control (SetFocus has
// been called)
void CMessageTextListBox::FocusChanged(TDrawNow aDrawNow)
{
    if (IsFocused()) // This control got focus.
        {
        iAppTitle->SetFocus(ETrue, aDrawNow);
        iListbox->SetFocus(ETrue, aDrawNow);
        }
    else // This control lost focus
        {
        iAppTitle->SetFocus(EFalse, aDrawNow);
        iListbox->SetFocus(EFalse, aDrawNow);
        }
    DrawDeferred();
}

void CMessageTextListBox::SetIconsArray()
{
	// Creates a new icon array.	
	CArrayPtr<CGulIcon>* iconArray = new (ELeave) CArrayPtrFlat<CGulIcon>(3);

	CleanupStack::PushL(iconArray);

	TBuf<128> path;
	path.Copy(_L(""));
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KTickMbmFileName);


	iconArray->AppendL( CEikonEnv::Static ()->CreateIconL( path,
                                         EMbmTickTick,
                                         EMbmTickTickm ) );

	iconArray->AppendL( CEikonEnv::Static ()->CreateIconL( path,
                                         EMbmTickEmpty,EMbmTickEmpty ) );

	path.Copy(_L(""));
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KS80SmsMbmFileName);

	iconArray->AppendL(iEikonEnv->CreateIconL(path, 
							EMbmS80smsSms, EMbmS80smsSmsm));

	CleanupStack::Pop(); // iconArray

	// Sets the icon array.
	CCustomListItemDrawer* itemDrawer = static_cast<CCustomListItemDrawer*>
		(iListbox->View()->ItemDrawer());

	itemDrawer->SetIconArray(iconArray); // transfer ownership

	// Sets the height of the list box item
	TSize size = itemDrawer->MaxIconSize();
	iListbox->SetItemHeightL(2 * size.iHeight);
}

void CMessageTextListBox::MarkUnMarkSelectedSms()
{
	if  (iListbox)
	{
	// Cursor's current position
	TInt itemIndex = iListbox->CurrentItemIndex();			

	if (itemIndex >= 0) 
		{
		CTextListBoxModel* model = iListbox->Model();  // not taking ownership
		model->SetOwnershipType (ELbmOwnsItemArray);
		CDesCArray* dataArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
		
		TBuf <3000> changeItem( (*dataArray)[itemIndex] );
		
		TInt len = changeItem.Length();
		
		if ( changeItem.Right(1) == _L("0") ) // Selected -> Deselected
			{
			changeItem.Delete(len-1, 1);
			changeItem.Append(_L("1"));

				if(iAppUi.iImageFlag == 5)
				{
					iInboxSelectedSms->Delete(itemIndex);
					iInboxSelectedSms->InsertL(itemIndex, 10000);
				}
				else if(iAppUi.iImageFlag == 6)
				{
					iSentItemSelectedSms->Delete(itemIndex);
					iSentItemSelectedSms->InsertL(itemIndex, 10000);
				}
			}		
		else  if ( changeItem.Right(1) == _L("1") )// Deselected -> Selected		
			{
			changeItem.Delete(len-1, 1);
			changeItem.Append(_L("0"));

				if(iAppUi.iImageFlag == 5)
				{
					iInboxSelectedSms->Delete(itemIndex);
					iInboxSelectedSms->InsertL(itemIndex, itemIndex);
				}
				else if(iAppUi.iImageFlag == 6)
				{
					iSentItemSelectedSms->Delete(itemIndex);
					iSentItemSelectedSms->InsertL(itemIndex, itemIndex);
				}
			}
		dataArray->Delete(itemIndex);
		dataArray->InsertL(itemIndex, changeItem);

		iListbox->HandleItemAdditionL();
		}
	}
}

void CMessageTextListBox::MarkAllItems()
{
	if(iAppUi.iImageFlag == 5)	
	{
		for(TInt count = 0; count < iCount1; count++)
		{
			iInboxSelectedSms->Delete(count);
			iInboxSelectedSms->InsertL(count, count);
		}
	}
	else if(iAppUi.iImageFlag == 6)	
	{
		for(TInt count = 0; count < iCount2; count++)
		{
			iSentItemSelectedSms->Delete(count);
			iSentItemSelectedSms->InsertL(count, count);
		}
	}

	MarkUnMarAllSms(0);
}

void CMessageTextListBox::UnMarkAllItems()
{
	if(iAppUi.iImageFlag == 5)	
	{
		for(TInt count = 0; count < iCount1; count++)
		{
			iInboxSelectedSms->Delete(count);
			iInboxSelectedSms->InsertL(count, 10000);
		}
	}
	else if(iAppUi.iImageFlag == 6)	
	{
		for(TInt count = 0; count < iCount2; count++)
		{
			iSentItemSelectedSms->Delete(count);
			iSentItemSelectedSms->InsertL(count, 10000);
		}
	}

	MarkUnMarAllSms(1);
}

void CMessageTextListBox::MarkUnMarAllSms(TInt aIsMark)
{
	if  (iListbox)
	{
		TInt smscount;

		if(iAppUi.iImageFlag == 5)	
			smscount = iInboxCount;
		else if(iAppUi.iImageFlag == 6)	
			smscount = iSentItemCount;

		for(TInt count = 0; count < smscount; count++)
		{
			CTextListBoxModel* model = iListbox->Model();  // not taking ownership
			model->SetOwnershipType (ELbmOwnsItemArray);
			CDesCArray* dataArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
			
			TBuf <3000> changeItem( (*dataArray)[count] );
			
			TInt len = changeItem.Length();
			
			if(aIsMark == 0)
			{
				// Deselected -> Selected
				changeItem.Delete(len-1, 1);
				changeItem.Append(_L("0"));
			}
			
			if(aIsMark == 1)
			{
				// Selected -> Deselected
				changeItem.Delete(len-1, 1);
				changeItem.Append(_L("1"));
			}

			dataArray->Delete(count);
			dataArray->InsertL(count, changeItem);

			//iListbox->HandleItemAdditionL();
		}

		iListbox->HandleItemAdditionL();
	}
}

void CMessageTextListBox::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* /*aArg3*/)
{
}

void CMessageTextListBox::GetSms(TInt val)
{	
	TMsvId smstype;

	if( val == 0)
	{
		smstype = KMsvGlobalInBoxIndexEntryId;
	}
	else if( val == 1)
	{
		smstype = KMsvSentEntryId;
	}

	CMsvSession* pMsv = CMsvSession::OpenSyncL(*this);
	
	TMsvSelectionOrdering sort;
	sort.SetShowInvisibleEntries(ETrue);

	CMsvEntry* parentEntry  = CMsvEntry::NewL(*pMsv, smstype, sort);

	CleanupStack::PushL(parentEntry );

	// get list of SMS entries in inbox
	CMsvEntrySelection* entries  = parentEntry->ChildrenL();
	CleanupStack::PushL(entries);
	
	//CMsvEntry* pMsvEntry = pMsv->GetEntryL(smstype); 
	parentEntry = pMsv->GetEntryL(smstype); 
	
	if( val == 0)
	{
		iCount1 = entries->Count();

		if(iReadSmsAO != NULL)
		{
			iReadSmsAO->iInboxCount	= entries->Count();
		}

		if( iCount1 == 0)
		{
			InboxSmsMessagearray = new (ELeave) CDesCArrayFlat(1);
			InboxSmsDetailsarray = new (ELeave) CDesCArrayFlat(1);
			InboxSmsDatearray = new (ELeave) CDesCArrayFlat(1);
			iInboxSelectedSms = new CArrayFixFlat<TInt>(1);
		}
		else
		{
			InboxSmsMessagearray = new (ELeave) CDesCArrayFlat(iCount1);
			InboxSmsDetailsarray = new (ELeave) CDesCArrayFlat(iCount1);
			InboxSmsDatearray = new (ELeave) CDesCArrayFlat(iCount1);
			iInboxSelectedSms = new CArrayFixFlat<TInt>(iCount1);
		}

		for(TInt count = 0; count < iCount1; count++)
		{
			iInboxSelectedSms->InsertL(count, ITEM_SELECTED);
		}
	}
	else if(val == 1)
	{
		iCount2 = entries->Count();
		
		if(iReadSmsAO != NULL)
		{
			iReadSmsAO->iSentItemCount	= entries->Count();
		}
	
		if( iCount2 == 0)
		{
			SentSmsMessagearray = new (ELeave) CDesCArrayFlat(1);
			SentSmsDetailsarray = new (ELeave) CDesCArrayFlat(1);
			SentSmsDatearray = new (ELeave) CDesCArrayFlat(1);
			iSentItemSelectedSms = new CArrayFixFlat<TInt>(1);
		}
		else
		{
			SentSmsMessagearray = new (ELeave) CDesCArrayFlat(iCount2);
			SentSmsDetailsarray = new (ELeave) CDesCArrayFlat(iCount2);
			SentSmsDatearray = new (ELeave) CDesCArrayFlat(iCount2);
			iSentItemSelectedSms = new CArrayFixFlat<TInt>(iCount2);
		}

		for(TInt count = 0; count < iCount2; count++)
		{
			iSentItemSelectedSms->InsertL(count, ITEM_SELECTED);
		}
	}

	CleanupStack::PopAndDestroy(2); // entries, parentEntry
	delete pMsv;
}

void CMessageTextListBox::SendSelected()
{
	iAppUi.iSmsCount = GetSelectedSmsCount();

	if(!iAppUi.iSmsCount)
	{
		iAppUi.ShowMessageBoxFromResource(R_TEXT_SELECT_MESSAGE_FIRST);
	}
	else
	{
		iAppUi.iSmsCount = 0;

		for(int i = 0 ; i < iInboxCount ; i++)
		{	
			if(iInboxSelectedSms->At(i) != ITEM_SELECTED)
				iAppUi.iSmsCount++;
		}

		for(int j = 0 ; j < iSentItemCount ; j++)
		{
			if(iSentItemSelectedSms->At(j) != ITEM_SELECTED)
				iAppUi.iSmsCount++;
		}

		iAppUi.iWriteSmsAO = new (ELeave) CWriteSmsAO(iAppUi);
		iAppUi.iWriteSmsAO->StartL(iInboxCount, iSentItemCount, iInboxSelectedSms, iSentItemSelectedSms, InboxSmsDatearray, SentSmsDatearray, InboxSmsDetailsarray, SentSmsDetailsarray, InboxSmsMessagearray, SentSmsMessagearray);

		/*TInt8 choice;
		iAppUi.GetChoice(choice);
		if(!choice)
		{
			choice = 1;
			iAppUi.SaveChoice(choice);
			iAppUi.ShowWindow(KViewIdStatusScreen);
			iAppUi.DeleteWindow(KViewIdSmsList);
		}
		else{*/
			//iAppUi.ShowWindow(KViewIdRegisterScreen);
			iAppUi.ShowWindow(KViewIdStatusScreen);
			iAppUi.ShowStatusScreen();
			iAppUi.DeleteWindow(KViewIdSmsList);
		//}	
	}
}

TInt CMessageTextListBox::GetSelectedSmsCount()
{
	TInt count=0;	

	for (TInt i = 0; i< iInboxCount; i++)
	{
		if(iInboxSelectedSms->At(i) != ITEM_SELECTED)
			count++;
	}

	for (TInt j = 0; j< iSentItemCount; j++)
	{
		if(iSentItemSelectedSms->At(j) != ITEM_SELECTED)
			count++;
	}

	return count;
}

void CMessageTextListBox::ChangeScreen()
{
	if(iAppUi.iImageFlag == 5)
	{
		if(iReadSmsAO != NULL)
		{
			iReadSmsAO->iInboxFlag	  = 0;
			iReadSmsAO->iSentItemFlag = 1;

			iReadSmsAO->ReleaseMtm();
			iReadSmsAO->PrepareMtm(SMS_INBOX);
		}
	}
	else if(iAppUi.iImageFlag == 6)
	{
		if(iReadSmsAO != NULL)
		{
			iReadSmsAO->iInboxFlag	  = 1;
			iReadSmsAO->iSentItemFlag = 0;

			iReadSmsAO->ReleaseMtm();
			iReadSmsAO->PrepareMtm(SMS_SENTITEMS);
		}
	}
}

void CMessageTextListBox::CloseReadSms()
{
	//iFlag = 0;

	if (iReadSmsAO != NULL)
	{
		delete iReadSmsAO;
		iReadSmsAO = NULL;
	}
}
