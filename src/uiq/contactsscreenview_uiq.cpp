/*
* ============================================================================
*  Name     : StatusScreenView_Uiq.cpp
*  Part of  : RippleVault
*  Created  : 15.12.2005 by Ezhil
*  Description:
*     CContactsScreenView implementation
*  Copyright: PurpleAce
* ============================================================================
*/


#include <eiktxlbm.h>
#include <eikclbd.h>
#include <quartzkeys.h>             // EQuartzKeyXxx
#include <eikmenub.h>               // CEikMenuBar
#include <w32std.h>                 // RWindow
#include <gulicon.h>                // CGulIcon

#include "ContactsScreenView_uiq.h"
#include "ReadContactsAO.h"
#include <RippleVault_Uiq.rsg>

CContactsScreenView* CContactsScreenView::NewL(const TRect& aRect, CRippleVaultAppUi& aAppUi)
  {
	CContactsScreenView* self = new (ELeave) CContactsScreenView(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
    CleanupStack::Pop();
	return self;
  }

CContactsScreenView::CContactsScreenView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
}

CContactsScreenView::~CContactsScreenView()
  {	
	CEikonEnv::Static ()->BusyMsgCancel();

    delete iTextListEmpty;
    if(iListBox)
      delete iListBox;
  
	if (iReadContactsAO != NULL)
	{
		delete iReadContactsAO;
		iReadContactsAO = NULL;
	}

	delete iContactsIdArray;
	delete iContactsDB;
  }

void CContactsScreenView::ConstructL(const TRect& aRect)
  {

  iListBox = 0;
  
  iContactCount = 0;

  iCount = 0;

  CreateWindowL();

  // initiate iTextListEmpty - Fetch the text from the resource file.
  iTextListEmpty = iEikonEnv->AllocReadResourceL(R_EXAMPLE_TEXT_EMPTY);

  CreateList();

  // Get the array of text items held by the list box's model
  array = ((CDesCArray *)iListBox->Model()->ItemTextArray());
  
  if(iAppUi.iStoragePath == 1)
  {
	  iContactsIdArray = new CArrayFixFlat<TInt>(1);

	  CDesCArray* smsnames = iAppUi.GetSmsNameList();	

	  CDesCArray* smsmobiles = iAppUi.GetSmsMobileList();

	  for(int i = 0 ; i < smsnames->Count() ; i++)
	  {
		 array->AppendL(smsnames->MdcaPoint(i)); 
	  }	

	  iContactCount = smsnames->Count();

	  iCount = smsnames->Count();

	  if(smsnames->Count() == 0)
		CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SELECT_RECIPIENTS)));
	  else 
		CEikonEnv::Static ()->BusyMsgCancel();
  }
  else
  {
	  iContactsDB = CContactDatabase::OpenL();

	  CContactIdArray* cArray = (CContactIdArray*)iContactsDB->SortedItemsL();
	  CContactIdArray& cArr = *cArray;

	  iCount = cArray->Count();

	  cArray->Reset();
	  cArr.Reset();

	  if (iCount > 0)
	  {
		// Reschedule ourselves.	  
		iContactsIdArray = new CArrayFixFlat<TInt>(iCount);
		iReadContactsAO = NULL;
		
		CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SCANNING_PROGRESS)));

		iReadContactsAO = new (ELeave) CReadContactsAO(*this);
		iReadContactsAO->StartL();
	  }
	  else	  
	  {
		iContactsIdArray = new CArrayFixFlat<TInt>(1);
		
		//CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_SELECT_RECEPIENT)));
	  }
  }
  // Handle the addition of items to the list box model. 
  iListBox->HandleItemAdditionL(); // This also redraws the list box.

  // Set the list box control with a focus - this gives the blue highlighting
  // effect shown in the pictures above.
  iListBox->SetFocus(ETrue);
  iListBox->DrawNow();
  
  SetRect(aRect);

  ActivateL();
  }

TKeyResponse CContactsScreenView::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
  {
  if (aType == EEventKey)
    {
    if (aKeyEvent.iCode == EQuartzKeyTwoWayDown)
      {
      iListBox->View()->MoveCursorL( CListBoxView::ECursorNextItem,
                                     CListBoxView::ENoSelection);
      iListBox->UpdateScrollBarsL();
      return EKeyWasConsumed;
      }
    else if (aKeyEvent.iCode == EQuartzKeyTwoWayUp)
      {
      iListBox->View()->MoveCursorL( CListBoxView::ECursorPreviousItem,
                                     CListBoxView::ENoSelection );
      iListBox->UpdateScrollBarsL();
      return EKeyWasConsumed;
      }
    }
  return iListBox->OfferKeyEventL(aKeyEvent, aType);
  }

void CContactsScreenView::HandlePointerEventL(const TPointerEvent& aPointerEvent)
  {
  if (aPointerEvent.iType == TPointerEvent::EButton1Down)
    {
    TPoint penXY = aPointerEvent.iPosition;
    int yMod = (int)( ((penXY.iY-6) / iListBox->ItemHeight()) +
                      iListBox->View()->TopItemIndex() );
    if (yMod < 0)
      yMod = 0;

    if ((yMod <= iListBox->View()->BottomItemIndex()) && (yMod < iContactCount))
      {
      iListBox->View()->VerticalMoveToItemL(yMod,
                                            CListBoxView::EDisjointSelection);
      iListBox->UpdateScrollBarsL();
      }
    }
  }

void CContactsScreenView::Draw(const TRect& /*aRect*/) const
  {
    // Window graphics context
  CWindowGc& gc = SystemGc();
    // Start with a clear screen
  gc.Clear();
  TRect rect = Rect();

  const CFont* font = iEikonEnv->TitleFont();
  gc.UseFont(font);
  //TInt baseline = rect.Height()/2 - font->AscentInPixels()/2;
  //gc.DrawText(*iTextListEmpty, rect, baseline, CGraphicsContext::ECenter);
			   
  TPoint pos2(30,70);
  gc.DrawText(*iTextListEmpty,pos2);	

  gc.DiscardFont();
  }

TInt CContactsScreenView::CountComponentControls() const
  {
	if (iListBox)
		{
		return 1;
		}
	return 0;
  }

CCoeControl* CContactsScreenView::ComponentControl(TInt aIndex) const
  {
  switch (aIndex)
    {
    case 0:
      return iListBox;
    default:
        return 0;
    }
  }

void CContactsScreenView::CreateList()
  {
  // Construct a list box
  iListBox = new (ELeave) CEikColumnListBox;

  iListBox->ConstructL(this, CEikListBox::EMultipleSelection);
  
  // set this view as the parent of the control
  iListBox->SetContainerWindowL( *this );

  // set look and behaviour:
  CColumnListBoxData* listBoxColumns = 
             ((CColumnListBoxItemDrawer*)iListBox->ItemDrawer())->ColumnData();
  
  listBoxColumns->SetColumnWidthPixelL(0, 200);
  listBoxColumns->SetColumnAlignmentL(0, CGraphicsContext::ELeft);
  
  // - set scrolling capabilities
  iListBox->CreateScrollBarFrameL();
  iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EAuto,
                                                      CEikScrollBarFrame::EAuto);
  // - set listbox bordes
  iListBox->SetBorder(TGulBorder::EDeepRaisedWithOutline); 

  // - set position
  TPoint p;
  TSize sz;
  p.iX = 0;
  p.iY = 0;
  sz.iWidth = 208;
  sz.iHeight = 230; // max 255

  iListBox->SetExtent(p, sz);

  // - set text color
  iListBox->View()->ItemDrawer()->SetTextColor(KRgbDarkMagenta);
  // set font
  iListBox->ItemDrawer()->SetFont(iEikonEnv->AnnotationFont());
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

TInt CContactsScreenView::GetContactNamesForSharingAlbum(TInt id, TDesC& name, TDesC& mobile)
{
	iContactsDB = CContactDatabase::OpenL();
	
	CContactIdArray* cArray = (CContactIdArray*)iContactsDB->SortedItemsL();
	CContactIdArray& cArr = *cArray;

	TBuf<150> fname;	
	TBuf<150> lname;	

	fname.Copy(_L(""));
	lname.Copy(_L(""));
		
	CContactItem* contact;

	TInt val = iContactsIdArray->At(id);

	contact = iContactsDB->ReadContactL(cArr[val]);
		
	CContactItemFieldSet& fieldSet = contact->CardFields();		   

	GetTextFieldFromFieldSet(fieldSet, KUidContactFieldGivenName, fname);

	GetTextFieldFromFieldSet(fieldSet, KUidContactFieldFamilyName, lname);

	if(fname.Length()>0 && lname.Length()>0 )
	{
		fname.Insert(0,lname);
		((TDes&)name).Copy(fname);
	}
	else if(fname.Compare(_L("")) ==0)
	{
		((TDes&)name).Copy(lname);
	}
	else
	{
		((TDes&)name).Copy(fname);
	}	

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

		if( iAppUi.iSettingsScreenValue == -1)
			ret = 0;
		else
		{			
			((TDes&)mobile).Copy(iAppUi.iMobBuffer);
			ret = 1;
		}
	}
	else
	{
		TPtrC ptr1(iAppUi.numbers->MdcaPoint(0));
		((TDes&)mobile).Copy(ptr1);
		ret = 1;
	}

	delete contact;		
	
	cArray->Reset();
	cArr.Reset();	
	
	return ret;
}

TInt CContactsScreenView::SendSharingXml()
{
	TBuf<150> names;
	TBuf<150> mnumbers;
    TInt sharecount=0;

	// Get the items currently selected.
    const CListBoxView::CSelectionIndexArray* indexarray = iListBox->SelectionIndexes();
    
	// Get the number of items in the indexarray.
    TInt count = indexarray->Count();

    if(count > 0)
    {
		if(count > 10)
		{
			//iEikonEnv->InfoMsg(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_RECEPIENT)));
			iAppUi.ShowMessageBoxFromResource(R_PHOTOS_RECEPIENT);
			return 0;
		}
		else
		{
			for(TInt id = 0; id < count; id++)
			{
				TInt ret;
				ret = GetContactNamesForSharingAlbum(indexarray->At(id), names, mnumbers);
				
				if(ret == 1)
				{
					iAppUi.MailId.Append(names);
					iAppUi.MailId.Append(_L(";"));
					
					iAppUi.MailNames.Append(mnumbers);
					iAppUi.MailNames.Append(_L(";"));

					sharecount++;
				}					
			}  
			
			if(iAppUi.iStoragePath == 2)
				iAppUi.SharingListBoxDialog(sharecount);
			else if(iAppUi.iStoragePath == 3)
			{
				iAppUi.ShowWindow(KViewIdStatusScreen);
				iAppUi.SaveMails(iAppUi.albumName,iAppUi.MailNames,iAppUi.MailId,iAppUi.iMessage,sharecount);
				iAppUi.SendSmsForClientSharing();
				iAppUi.DeleteWindow(KViewIdContactsViewScreen);
				iAppUi.ShowMessageBoxFromResource(R_TEXT_SHARING_APPLICATION_SUCCESS);
				//iAppUi.GetClientSharingAlbumXML();
			}
			
			return 1;
		}
    }
    else
    {
        iEikonEnv->InfoMsg(*(iEikonEnv->AllocReadResourceL(R_SELECT_RECEPIENT)));
		return 0;
    }
}

TInt CContactsScreenView::AddNewDatatoDB()
{
	TBuf<150> names;
	TBuf<150> mnumbers;
    TInt sharecount=0;

	// Get the items currently selected.
    const CListBoxView::CSelectionIndexArray* indexarray = iListBox->SelectionIndexes();
    
	// Get the number of items in the indexarray.
    TInt count = indexarray->Count();

    if(count > 0)
    {
		if(count > 10)
		{
			iEikonEnv->InfoMsg(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_RECEPIENT)));
			return 0;
		}
		else
		{
			for(TInt id = 0; id < count; id++)
			{
				TInt ret;
				ret = GetContactNamesForSharingAlbum(indexarray->At(id), names, mnumbers);
				
				if(ret == 1)
				{
					iAppUi.MailId.Append(names);
					iAppUi.MailId.Append(_L(";"));
					
					iAppUi.MailNames.Append(mnumbers);
					iAppUi.MailNames.Append(_L(";"));

					sharecount++;
				}					
			}  
			
			if(iAppUi.iStoragePath == 4)
			{			
				iAppUi.SaveSelectiveSmsMails(iAppUi.MailNames,iAppUi.MailId, sharecount);
			}
			
			return 1;
		}
    }
    else
    {
        iEikonEnv->InfoMsg(*(iEikonEnv->AllocReadResourceL(R_SELECT_RECEPIENT)));
		return 0;
    }
}

void CContactsScreenView::CloseReadContact()
{
	CEikonEnv::Static ()->BusyMsgCancel();

	// Handle the addition of items to the list box model. 
	iListBox->HandleItemAdditionL(); // This also redraws the list box.

	iListBox->DrawNow();

	if (iReadContactsAO != NULL)
	{
		delete iReadContactsAO;
		iReadContactsAO = NULL;
	}
}

void CContactsScreenView::AddContacts()
{
	// Handle the addition of items to the list box model. 
  iListBox->HandleItemRemovalL(); // This also redraws the list box.
	
  iContactCount = 0;

  iCount = 0;

  iContactsDB = CContactDatabase::OpenL();

  CContactIdArray* cArray = (CContactIdArray*)iContactsDB->SortedItemsL();
  CContactIdArray& cArr = *cArray;

  iCount = cArray->Count();

  cArray->Reset();
  cArr.Reset();

  if (iCount > 0)
  {
	iContactsIdArray->Reset();	  
	iContactsIdArray = new CArrayFixFlat<TInt>(iCount);

	iReadContactsAO = NULL;
	
	CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SCANNING_PROGRESS)));
	
	array->Reset();
    array = ((CDesCArray *)iListBox->Model()->ItemTextArray());

	iReadContactsAO = new (ELeave) CReadContactsAO(*this);
	iReadContactsAO->StartL();
  }
  else	  
  {
	iContactsIdArray->Reset();	  
	iContactsIdArray = new CArrayFixFlat<TInt>(1);
	
	//CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_SELECT_RECEPIENT)));
  }
}

void CContactsScreenView::ResetData()
{
  if (iReadContactsAO != NULL)
  {	
	CEikonEnv::Static ()->BusyMsgCancel();

	delete iReadContactsAO;
	iReadContactsAO = NULL;
  }
	// Handle the addition of items to the list box model. 
  iListBox->HandleItemRemovalL(); // This also redraws the list box.
	
  CDesCArray* smsnames = iAppUi.GetSmsNameList();	

  CDesCArray* smsmobiles = iAppUi.GetSmsMobileList();	

  array->Reset();
  array = ((CDesCArray *)iListBox->Model()->ItemTextArray());

  if(smsnames->Count() == 0)
	CEikonEnv::Static ()->BusyMsgL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SELECT_RECIPIENTS)));
  else 
	CEikonEnv::Static ()->BusyMsgCancel();

  for(int i = 0 ; i < smsnames->Count() ; i++)
  {
	 array->AppendL(smsnames->MdcaPoint(i)); 
  }	

  iContactCount = smsnames->Count();

  iCount = smsnames->Count();

  // Handle the addition of items to the list box model. 
  iListBox->HandleItemAdditionL(); // This also redraws the list box.

  iListBox->DrawNow();
}

void CContactsScreenView::DeleteContacts()
{
	// Get the items currently selected.
    const CListBoxView::CSelectionIndexArray* indexarray = iListBox->SelectionIndexes();
    
	CArrayFix<TInt>* iSmsArray;

	// Get the number of items in the indexarray.
    TInt count = indexarray->Count();
	TInt val;
	TInt val1;
	
	//sorting array
	if(count > 0)
	{
		iSmsArray = new CArrayFixFlat<TInt>(count);

		val = indexarray->At(0);
		do
		{
			for(int i = 1 ; i < count ; i++)
			{		
				val1 = indexarray->At(i);
			
				if(val1 > val )
					val = val1;		
			}
			iSmsArray->AppendL(val);
						
			TInt pos;
			TKeyArrayFix keyfix(0, ECmpTInt);
			
			indexarray->Find(val, keyfix, pos);

			indexarray->Delete(pos);

			count = indexarray->Count();
			if(count > 0)
				val = indexarray->At(0);
		}
		while(count != 0);
	}
	else
		iSmsArray = new CArrayFixFlat<TInt>(1);
	
	count = iSmsArray->Count();

    if(count > 0)
    {
		for(TInt id = 0; id < count; id++)
		{
			iAppUi.DeleteContactsForSms(iSmsArray->At(id));				
		}  

		ResetData();
    }
    else
    {
        iEikonEnv->InfoMsg(*(iEikonEnv->AllocReadResourceL(R_SELECT_RECEPIENT)));
    }

	delete iSmsArray;	
}

TInt CContactsScreenView::GetStoredContactsCount()
{
	CDesCArray* smsnames = iAppUi.GetSmsNameList();	

	TInt count = smsnames->Count();

	return count;
}