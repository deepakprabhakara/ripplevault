/*******************************************************************

 Copyright(c) 2000-2005 

 PurpleACE Wireless Solutions Pvt. Ltd.
 www.purpleace.com
 #708, A Wing, Carlton Towers, Airport Road, Bangalore - 560017,
 India
 All rights reserved.

 This software is the confidential and proprietary information of
 PurpleACE Wireless Solutions Pvt. Ltd. ("Confidential Information").
 You shall not disclose such Confidential Information and
 shall use it only in accordance with the terms of the license
 Agreement you entered into with PurpleACE Wireless Solutions Pvt. Ltd..

 Project Name               : Ripple Vault Symbian Client
 Module Name				: SmsScreenView_Uiq.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     To Show Main screen list for UIQ
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/

// --------------------------------------------------------------------------
// SmsScreenView.cpp
//
//
// Main screen list for UIQ.
// --------------------------------------------------------------------------

// INCLUDE FILES
#include <eikenv.h>
#include <eikappui.h>
#include <eiktxlbx.h>   // CEikTextListBox
#include <eiktxlbm.h>   // CTestListBoxModel
#include <badesca.h>    // for array of descriptors
#include "tick.mbg"    // for the icon
#include "video.mbg"    // for the icon
#include "images.mbg"    // for the icon
#include <QuartzKeys.h> // keys for jog dial
#include <eikmenub.h>
#include <gulicon.h>
#include <ripplevault_uiq.rsg>
#include "uiqsms.mbg"
#include <utf.h>

#include "SmsScreenView_Uiq.h"
#include "MediaScreenListBox.h"
#include "RippleVaultAppUi.h"
//#include "SmsBackupAO.h"
#include "constants.h"
#include "ReadSmsAO.h"
#include "WriteSmsAO.h"

//Static NewL() function to start the standard two phase construction.
CSmsScreenView* CSmsScreenView::NewL(const TRect& aRect, CRippleVaultAppUi& aAppUi)
{
	CSmsScreenView* self = new(ELeave) CSmsScreenView(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop();
	return self;
}

CSmsScreenView::CSmsScreenView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
}

//Destructor for the view.
CSmsScreenView::~CSmsScreenView()
{	
	CEikonEnv::Static ()->BusyMsgCancel();

	if(iIconList)
	{
		iIconList->Reset();
	}
	
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

	delete iListBox;	             
}

void CSmsScreenView::ResetData()
{
	CEikonEnv::Static ()->BusyMsgCancel();

	if(iAppUi.iImageFlag == 5)
	{
		if(iInboxCount == 0)
			CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_WEBUPLOAD_NOSMS)));
	}
	else if(iAppUi.iImageFlag == 6)
	{
		if(iSentItemCount == 0)
			CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_WEBUPLOAD_NOSMS)));
	}

	iCursorPos = 0;

	UpdateListBoxL(1);
}

void CSmsScreenView::ConstructL(const TRect& aRect)
{
	CreateWindowL();

	iFileName = CRippleVaultAppUi::ApplicationDriveAndPath();
	iFileName.Append(_L("smsbackup.man"));

	CreateListBoxL();

	iTempCount = 0;

	SetRect(aRect);
	ActivateL();
}

void CSmsScreenView::SetFileList(TInt aIsmark)
{
	//iListBox->HandleItemRemovalL();

	TInt count;
	if(iAppUi.iImageFlag == 5)
		count = iInboxCount;
	else if(iAppUi.iImageFlag == 6)
		count = iSentItemCount;

	if(count == 0)
		CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_WEBUPLOAD_NOSMS)));

	CTextListBoxModel* model = iListBox->Model();  // not taking ownership
	model->SetOwnershipType (ELbmOwnsItemArray);
	CDesCArray* dataArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
	
	if(iTempCount != count)
	{
	//for (TInt i=2;i<count+2;i++)
	if(count != 0)
	{		
		TInt i;
		i = count + 1;

		CEikonEnv::Static ()->BusyMsgCancel();

		TBuf<3000> item;		
		
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
	}

	if(iCursorPos >= 0)
		iListBox->SetCurrentItemIndex(iCursorPos);

	iListBox->HandleItemAdditionL();
}

void CSmsScreenView::UpdateListBoxL(TInt aIsmark)
{
	iListBox->HandleItemRemovalL(); 

	//SetIconsArray();

	TInt count;
	if(iAppUi.iImageFlag == 5)
		count = iInboxCount;
	else if(iAppUi.iImageFlag == 6)
		count = iSentItemCount;

	iTempCount = count;

	if(count == 0)
		CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_WEBUPLOAD_NOSMS)));

	CDesCArrayFlat* array;

	if(count == 0)
		array = new (ELeave) CDesCArrayFlat(1);
	else
		array = new (ELeave) CDesCArrayFlat(count);

	CleanupStack::PushL(array);

	for (TInt i=2;i<count+2;i++)
	{		
		CEikonEnv::Static ()->BusyMsgCancel();

		TBuf<3000> item;		
		
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
	
		array->AppendL(item);
	}

	CleanupStack::Pop();

	iListBox->Model()->SetItemTextArray(array);

	// We need to set the current item index and call DrawNow()
	// so that when the user changes the list box to single/double,
	// it will be updated properly.

	if(iCursorPos >= 0)
		iListBox->SetCurrentItemIndex(iCursorPos);
	
	iListBox->HandleItemAdditionL(); 

	iListBox->DrawNow();	
}

void CSmsScreenView::GetTopIndex()
{
	iCursorPos = iListBox->TopItemIndex();
	
	//iCursorPos = iListBox->CurrentItemIndex();
}

void CSmsScreenView::SizeChanged()
{
	if (iListBox)
	{
		iListBox->SetRect(Rect());
	}
}

TInt CSmsScreenView::CountComponentControls() const
{
	if (iListBox)
	{
		return 1;
	}
	return 0;
}

CCoeControl* CSmsScreenView::ComponentControl(TInt aIndex) const
{
	switch (aIndex)
		{
		case 0:
			return iListBox;
		default:
			return NULL;
		}
}

void CSmsScreenView::Draw(const TRect& aRect) const
{
	CWindowGc& gc = SystemGc();
	gc.SetPenStyle(CGraphicsContext::ENullPen);
	gc.SetBrushColor(KRgbWhite);
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc.DrawRect(aRect);
}

void CSmsScreenView::DrawText()
{
	CWindowGc& gc = SystemGc();
	gc.SetPenStyle(CGraphicsContext::ENullPen);
	gc.SetBrushColor(KRgbWhite);
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);

	const CFont* fontUsed;  
	fontUsed = iEikonEnv->TitleFont();
	gc.UseFont(fontUsed);

	TPoint pos1(50,30);
	gc.DrawText(*(iEikonEnv->AllocReadResourceL(R_RIPPLE_VAULT)),pos1);
	
    // Finished using the font
	gc.DiscardFont();
}

TKeyResponse CSmsScreenView::OfferKeyEventL(const TKeyEvent& aKeyEvent,
											   TEventCode aType)
{
	if (iListBox)
		{
		if ((EEventKeyDown == aType)
			&& (EStdQuartzKeyConfirm == aKeyEvent.iScanCode))
			{
			HandleListBoxEventL(
				iListBox, MEikListBoxObserver::EEventEnterKeyPressed);

			
			iListBox->UpdateScrollBarsL();
			}
		}
	return EKeyWasNotConsumed;
}

void CSmsScreenView::HandleListBoxEventL(CEikListBox* aListBox,
											TListBoxEvent aEventType)
{
	if ((MEikListBoxObserver::EEventEnterKeyPressed == aEventType)
		|| (MEikListBoxObserver::EEventItemClicked == aEventType))
        {
			MarkUnMarkSelectedItem();
		}
}

void CSmsScreenView::CreateListBoxL()
{
	// Creates a new SmsScreen list box.
	iListBox = new (ELeave) CMediaScreenListBox();
	iListBox->ConstructL(this,CEikListBox::ENoFirstLetterMatching| CEikListBox::ENoExtendedSelection);
	iListBox->SetListBoxObserver(this);
/*
	iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
*/
	iListBox->CreateScrollBarFrameL();
	// Set the horizontal scroll bars to be invisible and vertical scroll bars
	// to be visible, when needed
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
	// Make the scrollbar floating (removing the scroll bars)
	iListBox->ScrollBarFrame()->SetScrollBarManagement(CEikScrollBar::EVertical,CEikScrollBarFrame::EFloating);
	// Give the floating scrollbar standard scroll arrows
	iListBox->ScrollBarFrame()->SetScrollBarControlType(CEikScrollBar::EVertical,EQikCtArrowHeadPageScrollBar);

	iCursorPos  = 0;
}

void CSmsScreenView::SetIconsArray()
{
	// Sets the icon array.
	CMediaScreenListItemDrawer* itemDrawer = static_cast<CMediaScreenListItemDrawer*>
		(iListBox->View()->ItemDrawer());
	
	ResetIcons();
	
	itemDrawer->SetIconArray(iIconList); // transfer ownership 

	// Sets the height of the list box item
	// (= 2 * maximum height of the icon).
	TSize size = itemDrawer->MaxIconSize();
	//iListBox->SetItemHeightL(1.5 * size.iHeight);
		
	iListBox->SetItemHeightL(55);
}

void CSmsScreenView::ResetIcons()
{
	TInt count;

	if(iAppUi.iImageFlag == 5)
		count = iInboxCount;
	else if(iAppUi.iImageFlag == 6)
		count = iSentItemCount;

	if(iIconList)
	{
		iIconList->Reset();
	}

	if(count == 0)
		iIconList = new(ELeave) CArrayPtrFlat<CGulIcon>(3);
	else
		iIconList = new(ELeave) CArrayPtrFlat<CGulIcon>(count);	 	

	TBuf<128> path;	
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KTickMbmFileName);	

	iIconList->AppendL( CEikonEnv::Static ()->CreateIconL( path,
										 EMbmTickTick,
										 EMbmTickTickm ) );

	iIconList->AppendL( CEikonEnv::Static ()->CreateIconL( path,
										 EMbmTickEmpty,EMbmTickEmpty ) );

	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KUiqSmsMbmFileName);

	
	//for(int i=0; i<count; i++)
	{
		iIconList->AppendL( CEikonEnv::Static ()->CreateIconL( path,
            												 EMbmUiqsmsUiqsms,
                    										 EMbmUiqsmsUiqsmsm ) );
	}
}

TInt CSmsScreenView::Start()
{
	iReadSmsAO = NULL;	
	iReadSmsAO = new (ELeave) CReadSmsAO(*this);

	SetIconsArray();

	GetSms(0);

	GetSms(1);

	iReadSmsAO->StartL();

	UpdateListBoxL(0);

	return 0;
}

void CSmsScreenView::MarkAllItems()
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

void CSmsScreenView::UnMarkAllItems()
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

void CSmsScreenView::MarkUnMarkSelectedItem()
{
	if(iAppUi.iImageFlag == 5)
	{
		for(TInt count = 0; count < iInboxCount; count++)
		{
			if(iListBox->CurrentItemIndex() == count)
			{
				if(iInboxSelectedSms->At(count) != 10000)
				{
					iIsMark = EFalse;
				}
				else
				{
					iIsMark = ETrue;
				}
				break;
			}
		}
	}
	else if(iAppUi.iImageFlag == 6)	
	{
		for(TInt count = 0; count < iSentItemCount; count++)
	{
		if(iListBox->CurrentItemIndex() == count)
		{
				if(iSentItemSelectedSms->At(count) != 10000)
			{
				iIsMark = EFalse;
			}
			else
			{
				iIsMark = ETrue;
			}
			break;
		}
	}
	}
	
	if( iIsMark  )
	{
		MarkUnMarkSelectedSms();
	}
	else
	{
		MarkUnMarkSelectedSms();
	}
}

void CSmsScreenView::MarkUnMarkSelectedSms()
{
	if  (iListBox)
	{
	// Cursor's current position
	TInt itemIndex = iListBox->CurrentItemIndex();			

	if (itemIndex >= 0) 
		{
		CTextListBoxModel* model = iListBox->Model();  // not taking ownership
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

		iListBox->HandleItemAdditionL();
		}
	}
}

void CSmsScreenView::MarkSelectedSms()
{
	if  (iListBox)
	{
	// Cursor's current position
	TInt itemIndex = iListBox->CurrentItemIndex();			

	if (itemIndex >= 0) 
		{
		CTextListBoxModel* model = iListBox->Model();  // not taking ownership
		model->SetOwnershipType (ELbmOwnsItemArray);
		CDesCArray* dataArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
		
		TBuf <3000> changeItem( (*dataArray)[itemIndex] );
		
		TInt len = changeItem.Length();
		
		if ( changeItem.Right(1) == _L("0") ) // Selected -> Deselected
			{
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

		iListBox->HandleItemAdditionL();
		}
	}
}

void CSmsScreenView::MarkUnMarAllSms(TInt aIsMark)
{
	if  (iListBox)
	{
		TInt smscount;

		if(iAppUi.iImageFlag == 5)	
			smscount = iInboxCount;
		else if(iAppUi.iImageFlag == 6)	
			smscount = iSentItemCount;

		for(TInt count = 0; count < smscount; count++)
		{
			CTextListBoxModel* model = iListBox->Model();  // not taking ownership
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

			//iListBox->HandleItemAdditionL();
		}

		iListBox->HandleItemAdditionL();
	}
}

TInt CSmsScreenView::IsSmsSelected()
{
	TInt Ret=0;

	for(TInt count = 0; count < iInboxCount; count++)
	{
		if(	iInboxSelectedSms->At(count) != 10000)
		{
			Ret = 1;
			break;
		}
	}

	if(Ret == 0)
	{
		for(TInt count = 0; count < iSentItemCount; count++)
	{
			if(	iSentItemSelectedSms->At(count) != 10000)
		{
			Ret = 1;
			break;
		}
	}
	}
	return Ret;
}

void CSmsScreenView::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* /*aArg3*/)
{
}

void CSmsScreenView::GetSms(TBool aVal)
{
	TMsvId smstype=0;

	//CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_TEXT_SCAN)));

	if( aVal == 0)
		smstype = KMsvGlobalInBoxIndexEntryId;
	else if( aVal == 1)
		smstype = KMsvSentEntryId;

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
	
	//iCount = pMsvEntry->Count() ;
	if(aVal == 0)
	{
		iIconList = new(ELeave) CArrayPtrFlat<CGulIcon>(1);

		iCount1 = entries->Count();
		
		if(iCount1 == 0)
			CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_WEBUPLOAD_NOSMS)));
	
		if(iReadSmsAO != NULL)
		{
			iReadSmsAO->iInboxCount	= entries->Count();
		}

		if( iInboxCount == 0)
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
	else if(aVal == 1)
	{
		iCount2 = entries->Count();
		
		if(iCount2 == 0)
			CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_WEBUPLOAD_NOSMS)));
	
		if(iReadSmsAO != NULL)
		{
			iReadSmsAO->iSentItemCount	= entries->Count();
		}
	
		if( iSentItemCount == 0)
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

	//if(count == 0)
		//CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_WEBUPLOAD_NOSMS)));

	//if(aVal == 0)
		//iInboxCount = count;
	//else if(aVal == 1)
		//iSentItemCount = count;

	CleanupStack::PopAndDestroy(2); // entries, parentEntry
	delete pMsv;	
}

void CSmsScreenView::SendSmsToServer()
{/*
	//iEikonEnv->InfoMsg(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMSBACKUP_INFO)));
	TBuf<500> tempdata;
	TInt val;
	CnvUtfConverter converter;
	TBuf8<500> data;
	
	TInt foldertype;

	if(iAppUi.iImageFlag == 5)
		foldertype = 0;
	else if(iAppUi.iImageFlag == 6)
		foldertype = 1;

	iAppUi.iSmsCount = 0;

	CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_PROGRESS_BACKINGUPMESSAGES)));

	//if( iAppUi.iSmsBackupAO == NULL)
	//{
		//iAppUi.iSmsBackupAO = CSmsBackupAO::NewL(*(&iAppUi));
		//iAppUi.iSmsBackupAO->Start(iSelectedSms,foldertype);
	//}

	fs.Connect();

	iFile.Replace(fs, iFileName, EFileWrite | EFileStream);

	tempdata.Copy(_L("<root><h><msgt>smsbackup</msgt><msg></msg><uid>"));
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
	
	tempdata.Copy(iAppUi.iUserNames);
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

	tempdata.Copy(_L("</uid><hp>"));
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
	
	tempdata.Copy(iAppUi.iMobileNo);
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

	tempdata.Copy(_L("</hp><did>"));
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

    tempdata.Copy(iAppUi.PhoneImei()); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
	
	tempdata.Copy(_L("</did><currec></currec><totrec></totrec><loc>en_us</loc><ctime></ctime><lstime>0</lstime><synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype></h><b>"));
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

	for(int i = 0 ; i < iInboxCount ; i++)
	{	
		if(iInboxSelectedSms->At(i) != ITEM_SELECTED)
			WriteSmsToFile(i, SMS_INBOX);
	}

	for(int j = 0 ; j < iSentItemCount ; j++)
	{
		if(iSentItemSelectedSms->At(j) != ITEM_SELECTED)
			WriteSmsToFile(j, SMS_SENTITEMS);
	}

	tempdata.Copy(_L("</b></root>"));
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

	iFile.Close();
	fs.Close();

	TInt smsbackuptype = 0;
	//iAppUi.GetSmsXML(smsbackuptype);
	iAppUi.UploadSmsManually(smsbackuptype);

	CEikonEnv::Static ()->BusyMsgCancel();
*/
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

	TInt8 choice;
	iAppUi.GetChoice(choice);
	if(choice == 0)
	{
		iAppUi.SaveChoice(1);
		iAppUi.iMainScreenValue = 6;
		iAppUi.ShowWindow(KViewIdStatusScreen); //status screen
		iAppUi.DeleteWindow(KViewIdSmsList);
		iAppUi.ShowStatusScreen();
		//iAppUi.ShowWindow(KViewIdRegisterScreen);
		//iAppUi.DeleteWindow(KViewIdSmsList);
	}
	else
	{
		//iAppUi.ShowView();
		iAppUi.iMainScreenValue = 6;
		iAppUi.ShowWindow(KViewIdStatusScreen); //status screen
		iAppUi.DeleteWindow(KViewIdSmsList);
		iAppUi.ShowStatusScreen();
	}
}

/*void CSmsScreenView::WriteSmsToFile(TInt aId,TInt aSmsState)
{
	iAppUi.iSmsCount++;

	CnvUtfConverter converter;
	TMsvId smstype;	
	TBuf8<2000> data;
	TBuf<500> tempdata;

	tempdata.Copy(_L("<sms>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

	tempdata.Copy(_L("<folder>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
	
	if( aSmsState == SMS_INBOX)
	{
		converter.ConvertFromUnicodeToUtf8(data,_L("Inbox"));		
		iFile.Write(data);
	}
	else if( aSmsState == SMS_SENTITEMS)
	{
		converter.ConvertFromUnicodeToUtf8(data,_L("SentItems"));		
		iFile.Write(data);
	}

	tempdata.Copy(_L("</folder>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
//from address
	tempdata.Copy(_L("<from>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
	
	iFile.Write(_L8("<![CDATA["));
				
	if( aSmsState == SMS_INBOX)
	{
		converter.ConvertFromUnicodeToUtf8(data,(*InboxSmsDetailsarray)[aId]);
		iFile.Write(data);
	}
	else if( aSmsState == SMS_SENTITEMS)
	{
		converter.ConvertFromUnicodeToUtf8(data,iAppUi.iMobileNo);
		iFile.Write(data);
	}
	
	iFile.Write(_L8("]]>"));
	
	tempdata.Copy(_L("</from>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

//to address
	tempdata.Copy(_L("<to>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
	
	iFile.Write(_L8("<![CDATA["));
	
	if( aSmsState == SMS_INBOX)
	{
		converter.ConvertFromUnicodeToUtf8(data,iAppUi.iMobileNo);
		iFile.Write(data);
	}
	else if( aSmsState == SMS_SENTITEMS)
	{
		converter.ConvertFromUnicodeToUtf8(data,(*SentSmsDetailsarray)[aId]);
		iFile.Write(data);
	}

	iFile.Write(_L8("]]>"));
	
	tempdata.Copy(_L("</to>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

//sms time
	tempdata.Copy(_L("<time>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
			
	//message.Copy(_L(""));
	
	if( aSmsState == SMS_INBOX)
		converter.ConvertFromUnicodeToUtf8(data,(*InboxSmsDatearray)[aId]);
	else if( aSmsState == SMS_SENTITEMS)
		converter.ConvertFromUnicodeToUtf8(data,(*SentSmsDatearray)[aId]);

	iFile.Write(data);

	tempdata.Copy(_L("</time>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

//sms text
	tempdata.Copy(_L("<text>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
				
	TBuf<2000> message;
	message.Copy(_L("<![CDATA["));
	
	if( aSmsState == SMS_INBOX)
		message.Append((*InboxSmsMessagearray)[aId]);
	else if( aSmsState == SMS_SENTITEMS)
		message.Append((*SentSmsMessagearray)[aId]);
	
	message.Append(_L("]]>"));

	converter.ConvertFromUnicodeToUtf8(data,message);
	iFile.Write(data);

	tempdata.Copy(_L("</text>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

	tempdata.Copy(_L("</sms>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);			
}
*/
void CSmsScreenView::ChangeScreen()
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

void CSmsScreenView::CloseReadSms()
{
	iFlag = 0;

	if (iReadSmsAO != NULL)
	{
		delete iReadSmsAO;
		iReadSmsAO = NULL;
	}
}

TInt CSmsScreenView::GetCount()
{
	TInt count = 0;

	for(int i = 0 ; i < iInboxCount ; i++)
	{	
		if(iInboxSelectedSms->At(i) != ITEM_SELECTED)
			count++;
	}

	for(int j = 0 ; j < iSentItemCount ; j++)
	{
		if(iSentItemSelectedSms->At(j) != ITEM_SELECTED)
			count++;
	}
	
	return count;
}

// End of File  
