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
 Module Name				: SelectiveSmsListContainer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 10-05-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Screen to allow selection of recipients for Selective SMS
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/


// INCLUDE FILES
#include "SelectiveSmsListContainer.h"
#include "RippleVaultAppUi.h"
#include "marking.mbg"
#include <badesca.h> 
#include  <ripplevault.rsg>

#include <akncontext.h>
#include "rippleimage.mbg"
#include <akntitle.h> 

#include <aknnavi.h>

const TInt KAknExListGraphicGulIconIndex = 10;

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CSelectiveSmsListContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------

void CSelectiveSmsListContainer::ConstructL(const TRect& aRect)
{	
	CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();

	TBuf<10> message;
	message.Copy(_L(""));

	CAknNavigationControlContainer* aNaviPane = (CAknNavigationControlContainer *)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidNavi));

	CAknNavigationDecorator* aNaviDecorator = aNaviPane->CreateMessageLabelL(message);

    aNaviPane->PushL(*aNaviDecorator);

	CAknContextPane* cp=(CAknContextPane *)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidContext));
	
	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KRippleImageMbmFileName);

	CFbsBitmap* bitmap = iEikonEnv->CreateBitmapL(path, EMbmRippleimageRippleimage);
	CleanupStack::PushL(bitmap);
	CFbsBitmap* bitmapmask = iEikonEnv->CreateBitmapL(path, EMbmRippleimageRippleimagem);
	CleanupStack::PushL(bitmapmask);
	cp->SetPicture(bitmap, bitmapmask);
	CleanupStack::Pop(); // bitmapmask
	CleanupStack::Pop(); // bitmap
	DrawNow();

	CAknTitlePane* tp=(CAknTitlePane*)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidTitle));     
	tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_TITLE_CONTACTS))); // Set the text string.


	CreateWindowL();		

	//iListBox = new (ELeave) CAknSingleNumberStyleListBox;		
	iListBox = new (ELeave) CAknSingleGraphicStyleListBox;

	iListBox->SetContainerWindowL( *this );
	iListBox->ConstructL( this, EAknListBoxMultiselectionList);

	// Create the scroll indicator
	iListBox->CreateScrollBarFrameL(ETrue);
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

	iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );

	iListBox->View()->SetListEmptyTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SELECT_RECIPIENTS)));
	
	SetFileList();	
	
	SizeChanged();

	SetGraphicIconL( iListBox );

	iListBox->ActivateL();	

	TRect Please(TPoint(0,42),TPoint(176,186));
	SetRect(Please);		

	iContactsArray = new CArrayFixFlat<TInt>(50);	

	ActivateL();	
}

// Destructor
CSelectiveSmsListContainer::~CSelectiveSmsListContainer()
{
	items->Reset();		
	delete iContactsArray;	
	delete iListBox;		
}

// ---------------------------------------------------------
// CSelectiveSmsListContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------

void CSelectiveSmsListContainer::SizeChanged()
    {
        // TODO: Add here control resize code etc.
		iListBox->SetExtent(TPoint(0,0),TSize(176,150));
    }

// ---------------------------------------------------------
// CSelectiveSmsListContainer::CountComponentControls() const
// ---------------------------------------------------------

TInt CSelectiveSmsListContainer::CountComponentControls() const
    {
    return 1; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CSelectiveSmsListContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------

CCoeControl* CSelectiveSmsListContainer::ComponentControl(TInt aIndex) const
    {
   switch ( aIndex )
        {
        case 0:
            return iListBox;
        default:
            return NULL;
        }        
    }

// ---------------------------------------------------------
// CSelectiveSmsListContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CSelectiveSmsListContainer::Draw(const TRect& aRect ) const
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
// CSelectiveSmsListContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//

void CSelectiveSmsListContainer::HandleControlEventL(
    CCoeControl* ,TCoeEvent)
    {
    // TODO: Add your control event handler code here
    }


TKeyResponse CSelectiveSmsListContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,TEventCode aType)
    {
    // See if we have a selection
    TInt code = aKeyEvent.iCode;
	
    switch(code)
        {
		// is navigator button pressed
		case 63557:
			ListContacts();
			return iListBox->OfferKeyEventL(aKeyEvent, aType);
			break;
        default:
            // Let Listbox take care of its key handling
            return iListBox->OfferKeyEventL(aKeyEvent, aType);
            break;
        }
   		return (TKeyResponse)0;
    }

TInt CSelectiveSmsListContainer::OkPressKey()
 {
	TKeyEvent aKeyEvent;
	TEventCode aType;
	aKeyEvent.iCode=63557;
	aKeyEvent.iScanCode=167;
	aKeyEvent.iModifiers=1;
	aKeyEvent.iRepeats=0;
	aType=EEventKey;

	TInt err=0;
	if ( aType != EEventKey ) // Is not key event?
    {
	    return EKeyWasNotConsumed;
    }
    switch ( aKeyEvent.iCode ) // The code of key event is...
    {
		case 63557:
			err = ListContacts();
			break;
        default:			
			break;
	}

    if ( iListBox )
	{		
		return iListBox->OfferKeyEventL( aKeyEvent, aType );
	}
	return (TKeyResponse)0;
}

void CSelectiveSmsListContainer::SetGraphicIconL( CEikColumnListBox* aListBox )
{
    if ( aListBox )
    {
        // Creates gul icon.
        CArrayPtr<CGulIcon>* icons = new( ELeave ) CAknIconArray(KAknExListGraphicGulIconIndex);

        CleanupStack::PushL( icons );

        GraphicIconL( icons ); // Appends graphic data.

        // Sets graphics as ListBox icon.
        aListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );

        CleanupStack::Pop();
    }
}
void CSelectiveSmsListContainer::GraphicIconL( CArrayPtr<CGulIcon>* aIcons )
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

void CSelectiveSmsListContainer::SetFileList()
{
	//TBuf<50> bufc;		
	//_LIT(KStringFolder,"1\t");

	items = static_cast<CDesCArray*>(iListBox->Model()->ItemTextArray());

    iListBox->HandleItemRemovalL();  	
		
	/*bufc.Copy(KStringFolder);
	bufc.Append(_L("12"));
	items->AppendL(bufc);		

	bufc.Copy(KStringFolder);
	bufc.Append(_L("34"));
	items->AppendL(bufc);		

	bufc.Copy(KStringFolder);
	bufc.Append(_L("56"));
	items->AppendL(bufc);		

	bufc.Copy(KStringFolder);
	bufc.Append(_L("78"));
	items->AppendL(bufc);		

	bufc.Copy(KStringFolder);
	bufc.Append(_L("90"));
	items->AppendL(bufc);*/

	ListStoredContacts();
	
	// Refresh the listbox due to model change
	iListBox->HandleItemAdditionL();
    iListBox->SetCurrentItemIndex(0);
    iListBox->DrawNow();	
}

TInt CSelectiveSmsListContainer::ListContacts()
{
	TInt Index=-1;
	TInt err;
	TInt pos;
	TInt count;
	TInt flag=0;
	
	const CArrayFix<TInt> *aIndexArray = iListBox->SelectionIndexes() ;

	if(iListBox->CurrentItemIndex() == -1)
		return -1;

	pos = iListBox->CurrentItemIndex();
	
	count = aIndexArray->Count();
	
	for(int i=0 ; i < count ; i++)
	{
		err= aIndexArray->At(i); 
		
		if(err == pos)
		{
			flag = 1;
			Index = -2;
			break;
		}
	}

	if(flag==0)
	{
		CopyContactsId(pos);
	}
	else
	{
		RemoveContactsId(pos);
	}

	return Index;
}

void CSelectiveSmsListContainer::CopyContactsId(TInt pos)
{
	iContactsArray->AppendL(pos);	
}

void CSelectiveSmsListContainer::RemoveContactsId(TInt position)
{
	TInt err;	
	TInt pos;
	TKeyArrayFix keyfix(0, ECmpTInt);
	
	err = iContactsArray->Find(position, keyfix, pos);

	iContactsArray->Delete(pos);	
	
}
void CSelectiveSmsListContainer::DeleteContacts()
{
	TInt count;
	TInt val;
	TInt val1;
	TBuf<20> num;
	CArrayFix<TInt>* iSmsArray;

	count = iContactsArray->Count();
	
	//sorting array
	iSmsArray = new CArrayFixFlat<TInt>(50);

	if(count > 0)
	{
		val = iContactsArray->At(0);
		do
		{
			for(int i = 1 ; i < count ; i++)
			{		
				val1 = iContactsArray->At(i);
			
				if(val1 > val )
					val = val1;		
			}
			iSmsArray->AppendL(val);
			RemoveContactsId(val);
			count = iContactsArray->Count();
			if(count > 0)
				val = iContactsArray->At(0);
		}
		while(count != 0);
	}
	
	count = iSmsArray->Count();

	for(int i = 0 ; i < count ; i++)
	{
		val = iSmsArray->At(i);
		num.Copy(_L(""));
		num.AppendNum(val);
		//CEikonEnv::Static ()->AlertWin(num);		
		iPhotosAppUi->DeleteContactsForSms(val);
	}

	delete iSmsArray;	

	iPhotosAppUi->ActivateView(KViewIdEmptyScreen );
	iPhotosAppUi->ActivateView(KViewIdSelectiveSms );
}

void CSelectiveSmsListContainer::ListStoredContacts()
{
	TBuf<50> bufc;
	//TBuf<150> names;
	
	_LIT(KStringFolder,"1\t");
	
	CDesCArray* smsnames = iPhotosAppUi->GetSmsNameList();	

	//CDesCArray* smsmobiles = iPhotosAppUi->GetSmsMobileList();	

	for(int i = 0 ; i < smsnames->Count() ; i++)
	{
		bufc.Copy(KStringFolder);
		bufc.Append(smsnames->MdcaPoint(i));
//		bufc.Append(_L("---"));
//		bufc.Append(smsmobiles->MdcaPoint(i));
		items->AppendL(bufc);
	}
}

TInt CSelectiveSmsListContainer::GetStoredContactsCount()
{
	CDesCArray* smsnames = iPhotosAppUi->GetSmsNameList();	

	TInt count = smsnames->Count();

	return count;
}
// End of File  
