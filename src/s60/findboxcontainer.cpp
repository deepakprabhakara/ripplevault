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
 Module Name				: FindBoxContainer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Displays list of contacts and a findbox for sharing album, tell a friend features, selective sms
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/


// INCLUDE FILES
#include "FindBoxContainer.h"
#include "RippleVaultAppUi.h"
#include "marking.mbg"
#include <badesca.h> 
#include <AknGlobalListQuery.h>
#include <aknmessagequerydialog.h>
#include <RippleVault.rsg>
#include <aknlistquerydialog.h> 

#include <akntitle.h>
#include <akncontext.h>
#include "rippleimage.mbg"

#include <aknnavi.h>

#include "ReadContactsAO.h"

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CFindBoxContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------

void CFindBoxContainer::ConstructL()
{
	SetIconAndHeadings();

	CreateWindowL();		

	TInt count = GetContactsCount();
	if(count == 0)
	{
		iSelectedArray	   = new CArrayFixFlat<TInt>(1);
		iContactNames = new (ELeave) CDesCArrayFlat(1);
		iMobiles = new (ELeave) CDesCArrayFlat(1);
	}
	else
	{
		iSelectedArray = new CArrayFixFlat<TInt>(count);

		iContactNames = new (ELeave) CDesCArrayFlat(count);
		iMobiles = new (ELeave) CDesCArrayFlat(count);
	}
	
	for(TInt i = 0; i < count; i++)
	{
		iSelectedArray->InsertL(i, ITEM_SELECTED);
	}

	iCursorPos = 0;

	CreateListBox();

	SetFileList(2);
	iReadContactsAO = NULL;

	iReadContactsAO = new (ELeave) CReadContactsAO(*this);
	iReadContactsAO->StartL();

	ActivateL();
}

// Destructor
CFindBoxContainer::~CFindBoxContainer()
{
	items->Reset();	
	
	delete iSelectedArray;
	delete iListBox;
	delete iFindBox;
	
  	if (iReadContactsAO != NULL)
	{
		delete iReadContactsAO;
		iReadContactsAO = NULL;
	}
}

// ---------------------------------------------------------
// CFindBoxContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CFindBoxContainer::SizeChanged()
{
	if ( iListBox )        
		if ( iFindBox )
		{
	        SizeChangedForFindBox(); // Sets rectangle of listbox with findbox.
		}        
	iFindBox->SetExtent(TPoint(0,110),TSize(176,160));
}

void CFindBoxContainer::SizeChangedForFindBox()
{
	if ( iListBox && iFindBox )
    {
		// Initialize of resource ID for layout of findbox(These initial
		// values are layout for fixed findbox).
		TInt findWindowResourceId( R_AVKON_FIND_PANE );
		TInt listAreaResourceId( R_AVKON_LIST_GEN_PANE_X );        
		TBool flagsOfPopup( EFalse ); // Flag for find popup.
		TInt findWindowParentId = R_AVKON_MAIN_PANE_WITH_STATUS_PANE;
		
		// Calls HandleFindSizeChanged after creates findbox.
		AknFind::HandleFindSizeChanged(
			this, iListBox, iFindBox, flagsOfPopup, findWindowResourceId,
			listAreaResourceId, R_AVKON_LIST_GEN_PANE_WITH_FIND_POPUP,
			findWindowParentId );
    }
}

// ---------------------------------------------------------
// CFindBoxContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CFindBoxContainer::CountComponentControls() const
{
	TInt count=0;

	 if ( iListBox )
		{
		count++;
		}

	if ( iFindBox )
		{
		count++;
		}

	return count; // return nbr of controls inside this container
}

// ---------------------------------------------------------
// CFindBoxContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CFindBoxContainer::ComponentControl(TInt aIndex) const
{
	switch ( aIndex )
    {
		case 0:
			return iListBox;
			break;
		case 1:
			if ( iFindBox )
				{
				return iFindBox; // Returns the pointer to findbox object.
				}
			return iListBox;
			break;
		default:
			return NULL;
    }
}

// ---------------------------------------------------------
// CFindBoxContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CFindBoxContainer::Draw(const TRect& aRect) const
{
	CWindowGc& gc = SystemGc();
	// TODO: Add your drawing code here
	// example code...
	gc.SetPenStyle(CGraphicsContext::ENullPen);
	gc.SetBrushColor(KRgbGray);
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc.DrawRect(aRect);
}

// ---------------------------------------------------------
// CFindBoxContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CFindBoxContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
{
	// TODO: Add your control event handler code here
}

TKeyResponse CFindBoxContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,TEventCode aType)
{
	if ( aType != EEventKey ) // Is not key event?
    {
	    return EKeyWasNotConsumed;
    }
    switch ( aKeyEvent.iCode ) // The code of key event is...
    {
		case EKeyUpArrow:
			{
				if(iCursorPos >= 0)
					iCursorPos--;

				if(iCursorPos == -1)
					iCursorPos = iContactCount-1;
			}
			return iListBox->OfferKeyEventL(aKeyEvent, aType);
			break;
		case EKeyDownArrow:
			{
				if(iCursorPos < iContactCount)
					iCursorPos++;

				if(iCursorPos == iContactCount)
					iCursorPos = 0;
			}
				
			return iListBox->OfferKeyEventL(aKeyEvent, aType);
            break;	
		case 63557:
			{
				if(iContactCount > 0)
					MarkUnMarkSelectedItem();
			}
			break;
        default:
            break;
    }

	if( iListBox )
	{
		if(iFindBox)
		{
				TBool needRefresh( EFalse );
				TBool flagsOfPopup( EFalse );
	
				// Offers the key event to find box.
				if ( AknFind::HandleFindOfferKeyEventL( aKeyEvent, aType, this,
														iListBox, iFindBox,
														flagsOfPopup,
														needRefresh ) ==EKeyWasConsumed )
				{
					if ( needRefresh )
					{
						SizeChangedForFindBox();
						DrawNow();
					}					
				}
				//return iListBox->OfferKeyEventL( aKeyEvent, aType );
		}
		//return iListBox->OfferKeyEventL( aKeyEvent, aType );
	}
	return (TKeyResponse)0;
}

TInt CFindBoxContainer::OkPressKey()
    {

	TKeyEvent aKeyEvent;
	TEventCode aType;
	aKeyEvent.iCode=63557;
	aKeyEvent.iScanCode=167;
	aKeyEvent.iModifiers=1;
	aKeyEvent.iRepeats=0;
	aType=EEventKey;

	if ( aType != EEventKey ) // Is not key event?
    {
	    return EKeyWasNotConsumed;
    }
    switch ( aKeyEvent.iCode ) // The code of key event is...
    {
		case 63557:
			{
				if(iContactCount > 0)
					MarkUnMarkSelectedItem();
			}
			break;
        default:			
			break;
	}

	if ( iListBox )
	{
		if(iFindBox)
		{
			TBool needRefresh( EFalse );
			TBool flagsOfPopup( EFalse );
	
				// Offers the key event to find box.
			if ( AknFind::HandleFindOfferKeyEventL( aKeyEvent, aType, this,
														iListBox, iFindBox,
														flagsOfPopup,
														needRefresh ) ==EKeyWasConsumed )
			{
				if ( needRefresh )
				{
					SizeChangedForFindBox();
					DrawNow();
				}					
			}
			//return iListBox->OfferKeyEventL( aKeyEvent, aType );
		}
		//return iListBox->OfferKeyEventL( aKeyEvent, aType );
	}
	return (TKeyResponse)0;
 }


CAknSearchField* CFindBoxContainer::CreateFindBoxL(
    CEikListBox* aListBox,
    CTextListBoxModel* aModel,
    CAknSearchField::TSearchFieldStyle aStyle )
{
    CAknSearchField* findbox = NULL;

    if ( aListBox && aModel )
        {
        // Gets pointer of CAknFilteredTextListBoxModel.
        CAknFilteredTextListBoxModel* model =
                STATIC_CAST( CAknFilteredTextListBoxModel*, aListBox->Model());
        // Creates FindBox.
        findbox = CAknSearchField::NewL( *this,
                                         aStyle,
                                         NULL,
                                         20 );
        CleanupStack::PushL(findbox);
        // Creates CAknListBoxFilterItems class.
        model->CreateFilterL( aListBox, findbox );

		iIndex = model->Filter();				

        //Filter can get by model->Filter();
        CleanupStack::Pop(findbox); // findbox
        }
    return findbox;
}

void CFindBoxContainer::SetGraphicIconL( CEikColumnListBox* aListBox )
{
    if ( aListBox )
    {
        // Creates gul icon.
        CArrayPtr<CGulIcon>* icons = new( ELeave ) CAknIconArray(3);

        CleanupStack::PushL( icons );

        GraphicIconL( icons ); // Appends graphic data.

        // Sets graphics as ListBox icon.
        aListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );

        CleanupStack::Pop();
    }
}
void CFindBoxContainer::GraphicIconL( CArrayPtr<CGulIcon>* aIcons )
    {

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KMarkingMbmFileName);

    if ( aIcons )
        {
                // Appends icon.
                aIcons->AppendL( iEikonEnv->CreateIconL( path,
                                                 EMbmMarkingCheck,
                                                 EMbmMarkingCheckm ) );
                
				// Appends icon.
                aIcons->AppendL( iEikonEnv->CreateIconL( path,
                                                 EMbmMarkingNon_check,
                                                 EMbmMarkingNon_checkm ) );        
        }
    }

void CFindBoxContainer::SetFileList(TInt val)
{
	TBuf<500> contacts;	
	
	if(val == 3)
	{
		CTextListBoxModel* model = iListBox->Model();  // not taking ownership
		model->SetOwnershipType (ELbmOwnsItemArray);
		
		CDesCArray* dataArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
		
		TInt i = iContactCount - 1;
		
		if(iContactCount != 0)
		{
			contacts.Copy(_L(""));

			if( iSelectedArray->At(i) == ITEM_SELECTED)
				contacts.AppendNum(1);
			else
				contacts.AppendNum(0);

			contacts.Append(_L("\t"));

			contacts.Append((*iContactNames)[i]);
			
			dataArray->AppendL(contacts);
		}

		TBuf<20> searchtext;
		iFindBox->GetSearchText(searchtext);

		if(searchtext.Length()>0)
		{
			//SizeChangedForFindBox();
			//DrawNow();
		}
		else
		{
			if(iCursorPos >= 0)
				iListBox->SetCurrentItemIndex(iCursorPos);

			iListBox->HandleItemAdditionL();
		}
	}
	else
	{	
		if(val == 2)
			items = static_cast<CDesCArray*>(iListBox->Model()->ItemTextArray());
		else if(val == 1)
		{
			items->Reset();
			items = static_cast<CDesCArray*>(iListBox->Model()->ItemTextArray());
		}

		iListBox->HandleItemRemovalL(); 

		for (TInt i=0;i<iContactCount;i++)
		{
			contacts.Copy(_L(""));

			if( iSelectedArray->At(i) == ITEM_SELECTED)
				contacts.AppendNum(1);
			else
				contacts.AppendNum(0);

			contacts.Append(_L("\t"));
			
			contacts.Append((*iContactNames)[i]);
			
			items->AppendL(contacts);		
		}		

		if(iCursorPos >= 0)
			iListBox->SetCurrentItemIndex(iCursorPos);

		iListBox->HandleItemAdditionL();

		iListBox->DrawNow();
	}
}

void CFindBoxContainer::GetNames(TInt aCursorPos, TInt aIdValue)
{
	TInt mailLen;
	TInt nameLen;

	_LIT(KEmail,"(E)");
	_LIT(KMobile,"(M)");
	
	mailLen = iPhotosAppUi->MailId.Length();
	mailLen = 1000 - mailLen;

	nameLen = (*iContactNames)[aCursorPos].Length();

	if(mailLen > nameLen)
	{
		if(aIdValue == 1)
		{		
			numbers = new (ELeave) CDesCArrayFlat(50);

			GetMobileNumbers(aCursorPos);

			TInt ret = 0;

			if(numbers->MdcaCount() > 1 )
			{
				ret = ShowContactsDialog((*iContactNames)[aCursorPos]);
			}
			else
			{
				TPtrC ptr(numbers->MdcaPoint(0));
				iMobileNumber.Copy(ptr);
				ret = 1;
			}

			if(ret != -1)
			{
				iPhotosAppUi->MailNames.Append(iMobileNumber);
				iPhotosAppUi->MailNames.Append(_L(";"));		

				iPhotosAppUi->MailId.Append((*iContactNames)[aCursorPos]);
				iPhotosAppUi->MailId.Append(KMobile);
				iPhotosAppUi->MailId.Append(_L(";"));
			}

			/*TBuf<300> temp;
			temp.Append((*iContactNames)[aCursorPos]);
			temp.Append(_L("\n"));
			temp.Append(iMobileNumber);

			CAknMessageQueryDialog* verdlg = CAknMessageQueryDialog::NewL(temp);
			verdlg->PrepareLC(R_ABOUT_QUERY);
			verdlg->RunLD();*/
		}		
	}
	else
	{
		CAknMessageQueryDialog* verdlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_RECEPIENT)));
		verdlg->PrepareLC(R_ABOUT_QUERY);
		verdlg->RunLD();
	}
}

void CFindBoxContainer::SaveContacts()
{
	if(iPhotosAppUi->iStoragePath == 1)
		iPhotosAppUi->iSharingCount = GetSelectedCount();

	for(int i = 0 ; i < iContactCount ; i++)
	{	
		if(iSelectedArray->At(i) != ITEM_SELECTED)
			GetNames(i, 1);
	}
}

TInt CFindBoxContainer::ShowContactsDialog(TDesC& name)
{
	TInt PopValue;					

	CEikTextListBox* list = new( ELeave ) CAknSinglePopupMenuStyleListBox;
    CleanupStack::PushL( list );

    CAknPopupList* popupList = CAknPopupList::NewL(list,R_AVKON_SOFTKEYS_SELECT_CANCEL,AknPopupLayouts::EMenuWindow);

    CleanupStack::PushL( popupList );
    
    popupList->SetMaximumHeight(2);

    list->ConstructL( popupList, CEikListBox::ELeftDownInViewRect );
    list->CreateScrollBarFrameL( ETrue );
    list->ScrollBarFrame()->SetScrollBarVisibilityL(
		   CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );

    CTextListBoxModel* model = list->Model();
    model->SetItemTextArray( numbers );
    model->SetOwnershipType( ELbmOwnsItemArray );

	popupList->SetTitleL(name);
	
	PopValue=popupList->ExecuteLD();		
	
	if(PopValue==0)
	{	
		PopValue = -1;
		iPhotosAppUi->iSharingCount--;
	}
	else if(PopValue == 1)
	{
		PopValue=list->CurrentItemIndex();		
		TPtrC ptr1(numbers->MdcaPoint(PopValue));
		iMobileNumber.Copy(ptr1);
	}		

	CleanupStack::Pop(); // popupList
    CleanupStack::PopAndDestroy(); // list	

	return PopValue;
}

void CFindBoxContainer::SetIconAndHeadings()
{
	CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
	
	CAknNavigationControlContainer* aNaviPane = (CAknNavigationControlContainer *)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidNavi));

	TBuf<50> message;
	TBuf<50> formatBuf;

	if(iPhotosAppUi->iStoragePath == 3) //share album
	{
		TInt album;
		TInt photos;
		TInt videos;
		TInt albumshares;
		
		iPhotosAppUi->GetUploadSettingsFromDB(album, photos, videos, albumshares);

		message.Copy(_L(""));
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_ALBUMS)));
		message.Format(formatBuf, album, albumshares);
	}
	else
	{
		message.Copy(_L(""));
	}

	CAknNavigationDecorator* aNaviDecorator = aNaviPane->CreateMessageLabelL(message);

	aNaviPane->PushL(*aNaviDecorator);
	
	CAknContextPane* cp=(CAknContextPane *)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidContext));

	TBuf<128> iconpath;
	iconpath = CRippleVaultAppUi::ApplicationDriveAndPath();
	iconpath.Append(KRippleImageMbmFileName);

	CFbsBitmap* bitmap = iEikonEnv->CreateBitmapL(iconpath, EMbmRippleimageRippleimage);
	CleanupStack::PushL(bitmap);
	CFbsBitmap* bitmapmask = iEikonEnv->CreateBitmapL(iconpath, EMbmRippleimageRippleimagem);
	CleanupStack::PushL(bitmapmask);
	cp->SetPicture(bitmap, bitmapmask);
	CleanupStack::Pop(); // bitmapmask
	CleanupStack::Pop(); // bitmap
	DrawNow();


	CAknTitlePane* tp=(CAknTitlePane*)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidTitle)); 
	if(iPhotosAppUi->iStoragePath == 2) //tell a friend
		tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_TITLE_TELLAFRIEND))); // Set the text string.
	else if(iPhotosAppUi->iStoragePath == 3) //share album
		tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_TITLE_SHARE_ALBUM))); // Set the text string.
}

TInt CFindBoxContainer::GetContactsCount()
{
	TInt count = 0;

	CContactDatabase* iContactsDB;

	iContactsDB = CContactDatabase::OpenL();

	CContactIdArray* cArray = (CContactIdArray*)iContactsDB->SortedItemsL();
	CContactIdArray& cArr = *cArray;

	count = cArray->Count();
	
	cArray->Reset();
	cArr.Reset();
	
	delete iContactsDB;

	return count;
}

void CFindBoxContainer::CreateListBox()
{
	iListBox = new (ELeave) CAknSingleGraphicStyleListBox;

	iListBox->SetContainerWindowL( *this );
	iListBox->ConstructL( this, EAknListBoxMultiselectionList);

	// Create the scroll indicator
	iListBox->CreateScrollBarFrameL(ETrue);
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

	iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );
	
	// Sets style of findbox.
    CAknSearchField::TSearchFieldStyle style(CAknSearchField::ESearch );
    
	// Creates FindBox.
    iFindBox = CreateFindBoxL( iListBox, iListBox->Model(), style );

	SizeChanged();

	SetGraphicIconL( iListBox );

	iListBox->ActivateL();	

	TRect rect(TPoint(0,44),TPoint(176,188));
	SetRect(rect);
}

void CFindBoxContainer::MarkUnMarkSelectedItem()
{
	const CArrayFix<TInt> *aIndexArray = iListBox->SelectionIndexes() ;
	// Cursor's current position
	if(iListBox->CurrentItemIndex() >= 0)
	{
		MarkSelectedContact();
	}
}

void CFindBoxContainer::MarkSelectedContact()
{
	if(iListBox)
	{
		const CArrayFix<TInt> *aIndexArray = iListBox->SelectionIndexes() ;
		// Cursor's current position
		if(iListBox->CurrentItemIndex() >= 0)
		{
			TInt itemIndex = iIndex->FilteredItemIndex(iListBox->CurrentItemIndex());			

			if (itemIndex >= 0) 
			{
				CTextListBoxModel* model = iListBox->Model();  // not taking ownership
				model->SetOwnershipType (ELbmOwnsItemArray);
				CDesCArray* contactsArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
				
				TBuf<200> changeItem( (*contactsArray)[itemIndex] );
								
				if ( changeItem.Left(1) == _L("0") ) // Selected -> Deselected
				{
					changeItem.Delete(0, 1);
					changeItem.Insert(0, _L("1"));
					
					iSelectedArray->Delete(itemIndex);
					iSelectedArray->InsertL(itemIndex, ITEM_SELECTED);

					iPhotosAppUi->iSharingCount--;
				}		
				else if ( changeItem.Left(1) == _L("1") ) // Deselected -> Selected		
				{
					changeItem.Delete(0, 1);
					changeItem.Insert(0, _L("0"));
					
					iSelectedArray->Delete(itemIndex);
					iSelectedArray->InsertL(itemIndex, itemIndex);

					iPhotosAppUi->iSharingCount++;
				}

				contactsArray->Delete(itemIndex);
				contactsArray->InsertL(itemIndex, changeItem);

				iListBox->DrawNow();
			}
		}
	}
}

TInt CFindBoxContainer::GetSelectedCount()
{
	TInt count=0;	

	for(TInt i = 0; i < iContactCount; i++)
	{
		if(	iSelectedArray->At(i) != ITEM_SELECTED)
		{
			count++;
		}
	}

	return count;
}

void CFindBoxContainer::GetMobileNumbers(TInt aCursorPos)
{
	TBuf<50> mobile;
	TBuf<500> data;
	TInt pos = 0;
	
	data.Copy((*iMobiles)[aCursorPos]);

	while(data.Length() != 0)
	{
		pos = data.Locate('\n');
		
		mobile.Copy(_L(""));
		mobile.Copy(&data[0],pos);

		data.Delete(0,pos+1);

		if(mobile.Length() > 0)
			numbers->AppendL(mobile);
	}
}

void CFindBoxContainer::CloseReadContacts()
{
	if (iReadContactsAO != NULL)
	{
		delete iReadContactsAO;
		iReadContactsAO = NULL;
	}
}
// End of File  
