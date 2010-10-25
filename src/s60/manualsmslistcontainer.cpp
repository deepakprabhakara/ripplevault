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
 Module Name				: ManualsmsListContainer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 1-06-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     SMS selection list for manual upload
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/

// INCLUDE FILES
#include "ManualSmsListContainer.h"
#include "RippleVaultAppUi.h"
#include "tick.mbg"
#include <badesca.h> 
#include <msvstd.h> 
#include <mtclbase.h> 
#include <msvids.h> 
#include <smut.h> 
#include <smuthdr.h> 
#include <mtclreg.h> 
#include <txtrich.h> 
#include <smsclnt.h>
#include <gsmupdu.h>
#include <utf.h>
#include <ripplevault.rsg>
#include <akntitle.h>
#include <akncontext.h>
#include "rippleimage.mbg"
#include "sms.mbg"

#include "waitdialog.h" 
#include <aknwaitdialog.h> 
#include <aknmessagequerydialog.h>
#include <aknnavi.h>

#include "smsbackuptimer.h"

//#include "smsbackupao.h"

#include  "RippleVault.hrh"

const TInt KAknExListGraphicGulIconIndex = 10;

#include "ReadSmsAO.h"
#include "WriteSmsAO.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CManualSmsListContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------

void CManualSmsListContainer::ConstructL(const TRect& aRect)
{
	CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
	
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
	tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH))); // Set the text string.

	//CEikStatusPane * statusPane = iAvkonAppUi->StatusPane();     
	iNaviPane = static_cast<CAknNavigationControlContainer*>(statusPane->ControlL(TUid::Uid(EEikStatusPaneUidNavi))); 

	iNaviDeco = iNaviPane->CreateTabGroupL(); 
	iNaviPane->PushL(static_cast<CAknNavigationDecorator &>(*iNaviDeco)); 
	iTabGroup = static_cast<CAknTabGroup*> (iNaviDeco->DecoratedControl()); 

	iTabGroup->AddTabL(  0, *(iEikonEnv->AllocReadResourceL(R_TAB_INBOX)) ); 

	iTabGroup->AddTabL(  1, *(iEikonEnv->AllocReadResourceL(R_TAB_SENTITEMS)) ); 

	if(iPhotosAppUi->iStoragePath == SMS_INBOX)
		iTabGroup->SetActiveTabById(0); 
	else 
		iTabGroup->SetActiveTabById(1); 
	
	CreateListBox();

	iReadSmsAO = NULL;	
	iReadSmsAO = new (ELeave) CReadSmsAO(*this);

	iFlag = 1;
	iTempCount = 0;

	GetInboxSms();

	GetSentItemSms();

	iReadSmsAO->StartL();

	SetFileList(2);

	ShowListBox();
}

// Destructor
CManualSmsListContainer::~CManualSmsListContainer()
{
	items->Reset();	
	delete iInboxSelectedSms;
	delete iSentItemsSelectedSms;

	if (iReadSmsAO != NULL)
	{
		delete iReadSmsAO;
		iReadSmsAO = NULL;
	}

	/*if (iWriteSmsAO != NULL)
	{
		delete iWriteSmsAO;
		iWriteSmsAO = NULL;
	}*/

	delete iNaviDeco;		
	delete iListBox;		
	delete iFindBox;		
}

// ---------------------------------------------------------
// CManualSmsListContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------

void CManualSmsListContainer::SizeChanged()
    {
        // TODO: Add here control resize code etc.
	//iListBox->SetExtent(TPoint(0,0),TSize(176,50));
	if ( iListBox )        
			if ( iFindBox )
			{
	            SizeChangedForFindBox(); // Sets rectangle of listbox with findbox.
		    }        
		iFindBox->SetExtent(TPoint(0,100),TSize(176,160));
    }

void CManualSmsListContainer::SizeChangedForFindBox()
    {
    if ( iListBox && iFindBox )
        {
        // Initialize of resource ID for layout of findbox(These initial
        // values are layout for fixed findbox).
        TInt findWindowResourceId( R_AVKON_FIND_PANE );
        TInt listAreaResourceId( R_AVKON_LIST_GEN_PANE_X );        
        TBool flagsOfPopup( ETrue ); // Flag for find popup.
		TInt findWindowParentId = R_AVKON_MAIN_PANE_WITH_STATUS_PANE;
		
		// Calls HandleFindSizeChanged after creates findbox.
        AknFind::HandleFindSizeChanged(
            this, iListBox, iFindBox, flagsOfPopup, findWindowResourceId,
            listAreaResourceId, R_AVKON_LIST_GEN_PANE_WITH_FIND_POPUP,
            findWindowParentId );
        }
    }

// ---------------------------------------------------------
// CManualSmsListContainer::CountComponentControls() const
// ---------------------------------------------------------

TInt CManualSmsListContainer::CountComponentControls() const
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
// CManualSmsListContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------

CCoeControl* CManualSmsListContainer::ComponentControl(TInt aIndex) const
    {
   switch ( aIndex )
        {
        case 0:
            return iListBox;
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
// CManualSmsListContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CManualSmsListContainer::Draw(const TRect& aRect ) const
    {
	   CWindowGc& gc = SystemGc();
    // TODO: Add your drawing code here
    // example code...
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushColor(KRgbGray);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(aRect);
    }

void CManualSmsListContainer::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* /*aArg3*/)
{
}

TKeyResponse CManualSmsListContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,TEventCode aType)
    {
    // See if we have a selection
    TInt code = aKeyEvent.iCode;
	TInt active = iTabGroup->ActiveTabIndex();
    TInt count = iTabGroup->TabCount();

    switch(code)
        {
		// is navigator button pressed
		case EKeyLeftArrow:
            if ( active > 0 )
                {
					active--;										

					iTabGroup->SetActiveTabById(0); 
					
					if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
						iPhotosAppUi->iStoragePath = SMS_INBOX;
					
					if(iReadSmsAO != NULL)
					{
						iReadSmsAO->iInboxFlag	  = 0;
						iReadSmsAO->iSentItemFlag = 1;

						iReadSmsAO->ReleaseMtm();
						iReadSmsAO->PrepareMtm(SMS_INBOX);
					}

					TBuf<20> searchtext;
					searchtext.Copy(_L(""));
					iFindBox->SetSearchTextL(searchtext);

					SizeChanged();

					iCursorPos = 0;
					SetFileList(1);
					//iListBox->SetCurrentItemIndex(iCursorPos);
                }
            break;
		case EKeyRightArrow:
            if( (active + 1) < count )
                {
					active++;
		
					iTabGroup->SetActiveTabById(1); 
		
					if( iPhotosAppUi->iStoragePath == SMS_INBOX)
						iPhotosAppUi->iStoragePath = SMS_SENTITEMS;
					
					if(iReadSmsAO != NULL)
					{
						iReadSmsAO->iInboxFlag	  = 1;
						iReadSmsAO->iSentItemFlag = 0;

						iReadSmsAO->ReleaseMtm();
						iReadSmsAO->PrepareMtm(SMS_SENTITEMS);
					}

					TBuf<20> searchtext;
					searchtext.Copy(_L(""));
					iFindBox->SetSearchTextL(searchtext);

					SizeChanged();

					iCursorPos = 0;
					SetFileList(1);
					//iListBox->SetCurrentItemIndex(iCursorPos);
					}
            break;	
		case EKeyUpArrow:
			
			if(iCursorPos >= 0)
				iCursorPos--;

			if( iPhotosAppUi->iStoragePath == SMS_INBOX)
					{					
				if(iCursorPos == -1)
					iCursorPos = iInboxCount-1;
						}
			else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
						{
				if(iCursorPos == -1)
					iCursorPos = iSentItemCount-1;
						}

			//iCursorPos = iListBox->CurrentItemIndex();
			return iListBox->OfferKeyEventL(aKeyEvent, aType);
			break;
		case EKeyDownArrow:
			if( iPhotosAppUi->iStoragePath == SMS_INBOX)
						{
				if(iCursorPos < iInboxCount)
					iCursorPos++;

				if(iCursorPos == iInboxCount)
					iCursorPos = 0;
					}
			else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
			{
				if(iCursorPos < iSentItemCount)
					iCursorPos++;

				if(iCursorPos == iSentItemCount)
					iCursorPos = 0;
			}

			//iCursorPos = iListBox->CurrentItemIndex();
			return iListBox->OfferKeyEventL(aKeyEvent, aType);
            break;	
		case 63557:
			{
				TInt count=0;
			if( iPhotosAppUi->iStoragePath == SMS_INBOX)
			{
				count = iInboxCount;
			}
			else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
			{
				count = iSentItemCount;
			}

			if(count > 0)
				MarkUnMarkSelectedItem();
			}
			//return iListBox->OfferKeyEventL(aKeyEvent, aType);
			break;
        default:
            // Let Listbox take care of its key handling
            //return iListBox->OfferKeyEventL(aKeyEvent, aType);
            break;
        }
		
		if ( iListBox )
		{
			if ( iFindBox )
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

void CManualSmsListContainer::SetGraphicIconL( CEikFormattedCellListBox* aListBox )
{
    if ( aListBox )
    {
        // Creates gul icon.
        CArrayPtr<CGulIcon>* icons = new( ELeave ) CAknIconArray(KAknExListGraphicGulIconIndex);

        CleanupStack::PushL( icons );

        GraphicIconL( icons ); // Appends graphic data.

        // Sets graphics as ListBox icon.
        //aListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );
		aListBox->ItemDrawer()->FormattedCellData()->SetIconArray( icons );

        CleanupStack::Pop();
    }
}

void CManualSmsListContainer::GraphicIconL( CArrayPtr<CGulIcon>* aIcons )
    {

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KTickMbmFileName);

	TBuf<128> path1;
	path1 = CRippleVaultAppUi::ApplicationDriveAndPath();
	path1.Append(KSmsMbmFileName);

    if ( aIcons )
        {
                // Appends icon.
                aIcons->AppendL( iEikonEnv->CreateIconL( path,
                                                 EMbmTickTick,
                                                 EMbmTickTickm ) );
				
				aIcons->AppendL( iEikonEnv->CreateIconL( path1,
                                                 EMbmSmsInbox,
                                                 EMbmSmsInboxm ) );
				       
        }
    }

void CManualSmsListContainer::CreateListBox()
{
	CreateWindowL();		

	iListBox = new (ELeave) CAknDoubleGraphicStyleListBox;

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
}

void CManualSmsListContainer::ShowListBox()
{
	SizeChanged();

	SetGraphicIconL( iListBox );

	iListBox->ActivateL();	

	TRect Please(TPoint(0,42),TPoint(176,186));
	SetRect(Please);		

	ActivateL();	
}

void CManualSmsListContainer::SetFileList(TInt val)
{
	TBuf<2000> smsnumber;	
	TBuf<10> bufb0(_L("\t0"));
	TBuf<10> bufb1(_L("\t1"));	

	if(val == 3)
	{
		CTextListBoxModel* model = iListBox->Model();  // not taking ownership
		model->SetOwnershipType (ELbmOwnsItemArray);
		CDesCArray* dataArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
		
		TInt count=0;
		if( iPhotosAppUi->iStoragePath == SMS_INBOX)
		{
			count = iInboxCount;
		}
		else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
		{
			count = iSentItemCount;
		}

#ifdef __LOGME__
		iPhotosAppUi->LogText.Copy(_L("Inbox Count="));
		iPhotosAppUi->LogText.AppendNum(iInboxCount);
		iPhotosAppUi->LogText.Append(_L("Sent Count"));
		iPhotosAppUi->LogText.AppendNum(iSentItemCount);
		iPhotosAppUi->LogText.Append(_L("\n"));
		iPhotosAppUi->WriteLogFile(iPhotosAppUi->LogText);
#endif	
		
		if(iTempCount != count)
		{
			//for (TInt i=0;i<count;i++)
			TInt i = count - 1;
			if(count != 0)
			{
				smsnumber.Copy(_L("1\t"));
				
				if( iPhotosAppUi->iStoragePath == SMS_INBOX)
					smsnumber.Append((*InboxSmsDetailsarray)[i]);
				else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
					smsnumber.Append((*SentSmsDetailsarray)[i]);

				smsnumber.Append(_L("\t"));
				
				if( iPhotosAppUi->iStoragePath == SMS_INBOX)
					smsnumber.Append((*InboxSmsMessagearray)[i]);
				else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
					smsnumber.Append((*SentSmsMessagearray)[i]);

				if( iPhotosAppUi->iStoragePath == SMS_INBOX)
				{
					if( iInboxSelectedSms->At(i) == ITEM_SELECTED)
					{	
					}
					else
					{
						smsnumber.Append(bufb0);
					}
				}
				else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
				{
					if( iSentItemsSelectedSms->At(i) == ITEM_SELECTED)
					{	
					}
					else
					{
						smsnumber.Append(bufb0);
					}
				}
				dataArray->AppendL(smsnumber);

				iTempCount = count;
			}
		}

		//if(iCursorPos >= 0)
		//	iListBox->SetCurrentItemIndex(iCursorPos);

		//iListBox->HandleItemAdditionL();

		TBuf<20> searchtext;
		iFindBox->GetSearchText(searchtext);

		if(searchtext.Length()>0)
		{
			//SizeChangedForFindBox();
			//DrawNow();
		}
		else
		{
			iListBox->HandleItemAdditionL();

			if(iCursorPos >= 0)
				iListBox->SetCurrentItemIndex(iCursorPos);

		}

		//if(count != 0)
			//iListBox->SetCurrentItemIndexAndDraw(iCursorPos);
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
	
	TInt count=0;
	if( iPhotosAppUi->iStoragePath == SMS_INBOX)
	{
		count = iInboxCount;
	}
	else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
	{
		count = iSentItemCount;
	}

	iTempCount = count;

	for (TInt i=0;i<count;i++)
	{
		smsnumber.Copy(_L("1\t"));
		
		if( iPhotosAppUi->iStoragePath == SMS_INBOX)
			smsnumber.Append((*InboxSmsDetailsarray)[i]);
		else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
			smsnumber.Append((*SentSmsDetailsarray)[i]);
	
		smsnumber.Append(_L("\t"));
		
		if( iPhotosAppUi->iStoragePath == SMS_INBOX)
			smsnumber.Append((*InboxSmsMessagearray)[i]);
		else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
			smsnumber.Append((*SentSmsMessagearray)[i]);
	
		if( val == 0)
		{
			smsnumber.Append(bufb1);
		}
		else if( val == 1 || val == 2)
		{
			if( iPhotosAppUi->iStoragePath == SMS_INBOX)
			{
				if( iInboxSelectedSms->At(i) == ITEM_SELECTED)
				{	
				}
				else
				{
					smsnumber.Append(bufb0);
				}
			}
			else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
			{
				if( iSentItemsSelectedSms->At(i) == ITEM_SELECTED)
				{	
				}
				else
				{
					smsnumber.Append(bufb0);
				}
			}
		}		
		items->AppendL(smsnumber);		
	}		

	//TInt iTopPos = iCursorPos - 1;

	//if(iTopPos<0)
	//	iTopPos = 0;

	//iListBox->SetTopItemIndex(iTopPos);//iListBox->TopItemIndex());

	if(iCursorPos >= 0)
		iListBox->SetCurrentItemIndex(iCursorPos);

	iListBox->HandleItemAdditionL();

	/*if(iInboxCount == 1)
		iListBox->SetCurrentItemIndex(iCursorPos);
	else if(iInboxCount == 1)
		iListBox->SetCurrentItemIndex(iCursorPos);*/

	iListBox->DrawNow();
	}
}

void CManualSmsListContainer::SendSelected()
{
	TInt count;
	//CnvUtfConverter converter;
	//TBuf8<500> data;
	//TBuf<500> tempdata;

	iPhotosAppUi->iSmsCount = GetSelectedSmsCount();
	count = IsImageSelected();

	if(!count){
		//iPhotosAppUi->ShowMessageBoxFromResource(R_TEXT_SELECT_MESSAGE_FIRST);		
			CAknMessageQueryDialog* abtdlg;
			
			abtdlg= CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SELECT_MESSAGE_FIRST)));

				abtdlg->PrepareLC(R_UPGRADE_QUERY);

				if (abtdlg->RunLD()) 
				{
				}
				else
				{
					TInt8 choice;
					iPhotosAppUi->GetChoice(choice);
					if(choice)
						iPhotosAppUi->ActivateView(KViewIdStatusScreen );
					else
					{
						choice = 1;
						iPhotosAppUi->SaveChoice(choice);
						iPhotosAppUi->HandleCommandL(EMainView);
					}
				}
	}
	else
	{

		//iWriteSmsAO = NULL;	
		iPhotosAppUi->iWriteSmsAO = new (ELeave) CWriteSmsAO(*iPhotosAppUi);
		iPhotosAppUi->iWriteSmsAO->StartL(iInboxCount, iSentItemCount, iInboxSelectedSms, iSentItemsSelectedSms, InboxSmsDatearray, SentSmsDatearray, InboxSmsDetailsarray, SentSmsDetailsarray, InboxSmsMessagearray, SentSmsMessagearray);

		CloseWriteSms();
		
		/*
		fs.Connect();

		iFile.Replace(fs, iFileName, EFileWrite | EFileStream);

		tempdata.Copy(_L("<root><h><msgt>smsbackup</msgt><msg></msg><uid>"));
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		iFile.Write(data);
		
		tempdata.Copy(iPhotosAppUi->iUserNames);
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		iFile.Write(data);

		tempdata.Copy(_L("</uid><hp>"));
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		iFile.Write(data);
		
		tempdata.Copy(iPhotosAppUi->iMobileNo);
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		iFile.Write(data);

		tempdata.Copy(_L("</hp><did>"));
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		iFile.Write(data);

        tempdata.Copy(iPhotosAppUi->PhoneImei()); 
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
			if(iSentItemsSelectedSms->At(j) != ITEM_SELECTED)
					WriteSmsToFile(j, SMS_SENTITEMS);
		}

		tempdata.Copy(_L("</b></root>"));
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		iFile.Write(data);

		iFile.Close();
		fs.Close();

		TInt smsbackuptype = 0;
		//iPhotosAppUi->GetSmsXML(smsbackuptype);
		iPhotosAppUi->UploadSmsManually(smsbackuptype);

		TInt8 choice;
		iPhotosAppUi->GetChoice(choice);
		if(!choice)
		{
			choice = 1;
			iPhotosAppUi->SaveChoice(choice);
			iPhotosAppUi->ActivateView(KViewIdMainScreen);	
		}
		else
		{
			iPhotosAppUi->ActivateView(KViewIdEmptyScreen);
			iPhotosAppUi->ActivateView(KViewIdStatusScreen);
		}*/
	}
}
/*
void CManualSmsListContainer::WriteSmsToFile(TInt aId,TInt aSmsState)
{
	CnvUtfConverter converter;
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
		converter.ConvertFromUnicodeToUtf8(data,iPhotosAppUi->iMobileNo);
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
		converter.ConvertFromUnicodeToUtf8(data,iPhotosAppUi->iMobileNo);
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
void CManualSmsListContainer::MarkUnMarkSelectedItem()
{
	TBool iIsMark = ETrue;

	//const CArrayFix<TInt> *aIndexArray = iListBox->SelectionIndexes() ;
	// Cursor's current position
	if(iListBox->CurrentItemIndex() >= 0)
	{
	TInt count = iIndex->FilteredItemIndex(iListBox->CurrentItemIndex());

	if( iPhotosAppUi->iStoragePath == SMS_INBOX)
	{				
		if(iInboxSelectedSms->At(count) != ITEM_SELECTED)
		{
			iIsMark = EFalse;
		}
		else
		{
			iIsMark = ETrue;
		}
	}
	else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
	{				
		if(iSentItemsSelectedSms->At(count) != ITEM_SELECTED)
		{
			iIsMark = EFalse;
		}
		else
		{
			iIsMark = ETrue;
		}
	}
			
	if(iIsMark)
	{
		MarkSelectedPhoto();		
	}
	else
	{
		//UnMarkSelectedPhoto();
		MarkSelectedPhoto();
	}
	}
}

void CManualSmsListContainer::MarkAllItems()
{ 	
	TInt count1=0;
	if( iPhotosAppUi->iStoragePath == SMS_INBOX)
	{
		//count1 = iInboxCount;
		count1 = iCount1;
	}
	else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
	{
		//count1 = iSentItemCount;
		count1 = iCount2;
	}

	for(TInt count = 0; count < count1; count++)
	{
		if( iPhotosAppUi->iStoragePath == SMS_INBOX)
		{				
			iInboxSelectedSms->Delete(count);
			iInboxSelectedSms->InsertL(count, count);
		}
		else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
		{				
			iSentItemsSelectedSms->Delete(count);
			iSentItemsSelectedSms->InsertL(count, count);
		}
	}

	iCursorPos = 0;

	SetFileList(1);
}

void CManualSmsListContainer::UnMarkAllItems()
{	
	TInt count1=0;
	if( iPhotosAppUi->iStoragePath == SMS_INBOX)
	{
		//count1 = iInboxCount;
		count1 = iCount1;
	}
	else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
	{
		//count1 = iSentItemCount;
		count1 = iCount2;
	}

	for(TInt count = 0; count < count1; count++)
	{
		if( iPhotosAppUi->iStoragePath == SMS_INBOX)
		{				
			iInboxSelectedSms->Delete(count);
			iInboxSelectedSms->InsertL(count, ITEM_SELECTED);
		}
		else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
		{				
			iSentItemsSelectedSms->Delete(count);
			iSentItemsSelectedSms->InsertL(count, ITEM_SELECTED);
		}
	}

	iCursorPos = 0;

	SetFileList(1);
}

void CManualSmsListContainer::MarkSelectedPhoto()
{
	if  (iListBox)
	{
	//const CArrayFix<TInt> *aIndexArray = iListBox->SelectionIndexes() ;
	// Cursor's current position
	if(iListBox->CurrentItemIndex() >= 0)
	{
	TInt itemIndex = iIndex->FilteredItemIndex(iListBox->CurrentItemIndex());			

	if (itemIndex >= 0) 
		{
		CTextListBoxModel* model = iListBox->Model();  // not taking ownership
		model->SetOwnershipType (ELbmOwnsItemArray);
		CDesCArray* smsArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
		
		TBuf <2000> changeItem( (*smsArray)[itemIndex] );
		
		TInt len = changeItem.Length();
		
		if ( changeItem.Right(1) == _L("0") ) // Selected -> Deselected
			{
				changeItem.Delete(len-2, 2);
				if( iPhotosAppUi->iStoragePath == SMS_INBOX)
				{
					iInboxSelectedSms->Delete(itemIndex);
					iInboxSelectedSms->InsertL(itemIndex, ITEM_SELECTED);
				}
				else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
				{
					iSentItemsSelectedSms->Delete(itemIndex);
					iSentItemsSelectedSms->InsertL(itemIndex, ITEM_SELECTED);
				}
			}		
		else  // Deselected -> Selected		
			{
				changeItem.Append(_L("\t0"));
				if( iPhotosAppUi->iStoragePath == SMS_INBOX)
				{
					iInboxSelectedSms->Delete(itemIndex);
					iInboxSelectedSms->InsertL(itemIndex, itemIndex);
				}
				else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
				{
					iSentItemsSelectedSms->Delete(itemIndex);
					iSentItemsSelectedSms->InsertL(itemIndex, itemIndex);
				}
			}
		smsArray->Delete(itemIndex);
		smsArray->InsertL(itemIndex, changeItem);

		//iListBox->HandleItemAdditionL();
		iListBox->DrawNow();
		}
	}
	}
}

void CManualSmsListContainer::MarkSelectedItem()
{
	if  (iListBox)
	{
	//const CArrayFix<TInt> *aIndexArray = iListBox->SelectionIndexes() ;
	// Cursor's current position
	if(iListBox->CurrentItemIndex() >= 0)
	{
	TInt itemIndex = iIndex->FilteredItemIndex(iListBox->CurrentItemIndex());			

	if (itemIndex >= 0) 
		{
		CTextListBoxModel* model = iListBox->Model();  // not taking ownership
		model->SetOwnershipType (ELbmOwnsItemArray);
		CDesCArray* smsArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
		
		TBuf <2000> changeItem( (*smsArray)[itemIndex] );
		
		TInt len = changeItem.Length();
		
		if ( changeItem.Right(1) == _L("0") ) // Selected -> Deselected
			{
			}		
		else  // Deselected -> Selected		
			{
				changeItem.Append(_L("\t0"));
				if( iPhotosAppUi->iStoragePath == SMS_INBOX)
				{
					iInboxSelectedSms->Delete(itemIndex);
					iInboxSelectedSms->InsertL(itemIndex, itemIndex);
				}
				else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
				{
					iSentItemsSelectedSms->Delete(itemIndex);
					iSentItemsSelectedSms->InsertL(itemIndex, itemIndex);
				}
			}
		smsArray->Delete(itemIndex);
		smsArray->InsertL(itemIndex, changeItem);

		//iListBox->HandleItemAdditionL();
		iListBox->DrawNow();
		}
	}
	}
}

void CManualSmsListContainer::UnMarkSelectedPhoto()
{
	
	//const CArrayFix<TInt> *aIndexArray = iListBox->SelectionIndexes() ;
	// Cursor's current position
	if(iListBox->CurrentItemIndex() >= 0)
	{
	TInt count = iIndex->FilteredItemIndex(iListBox->CurrentItemIndex());

	if( iPhotosAppUi->iStoragePath == SMS_INBOX)
	{
		iInboxSelectedSms->Delete(count);
		iInboxSelectedSms->InsertL(count, ITEM_SELECTED);
	}
	else if( iPhotosAppUi->iStoragePath == SMS_SENTITEMS)
	{
		iSentItemsSelectedSms->Delete(count);
		iSentItemsSelectedSms->InsertL(count, ITEM_SELECTED);
	}

	iCursorPos = count;
	
	SetFileList(1);
	}
}

TInt CManualSmsListContainer::IsImageSelected()
{
	TInt Ret=0;	

	for(TInt count = 0; count < iInboxCount; count++)
	{
		if(	iInboxSelectedSms->At(count) != ITEM_SELECTED)
		{
			Ret = 1;
			break;
		}
	}

	if(Ret == 0)
	{
		for(TInt count1 = 0; count1 < iSentItemCount; count1++)
		{
			if(	iSentItemsSelectedSms->At(count1) != ITEM_SELECTED)
			{
				Ret = 1;
				break;
			}
		}
	}

	return Ret;
}

TInt CManualSmsListContainer::GetSelectedSmsCount()
{
	TInt count=0;	

	for(TInt i = 0; i < iInboxCount; i++)
	{
		if(	iInboxSelectedSms->At(i) != ITEM_SELECTED)
		{
			count++;
		}
	}

	for(TInt j = 0; j < iSentItemCount; j++)
	{
		if(	iSentItemsSelectedSms->At(j) != ITEM_SELECTED)
		{
			count++;
		}
	}

	return count;
}

void CManualSmsListContainer::GetInboxSms()
{	
	TMsvId smstype;

		smstype = KMsvGlobalInBoxIndexEntryId;

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
	iCount1 = entries->Count();
	
	if(iReadSmsAO != NULL)
	{
		iReadSmsAO->iInboxCount	= entries->Count();
	}

	if( entries->Count() == 0)
	{
		InboxSmsDatearray = new (ELeave) CDesCArrayFlat(1);
		InboxSmsMessagearray = new (ELeave) CDesCArrayFlat(1);
		InboxSmsDetailsarray = new (ELeave) CDesCArrayFlat(1);
		iInboxSelectedSms = new CArrayFixFlat<TInt>(1);
	}
	else
	{
		InboxSmsDatearray = new (ELeave) CDesCArrayFlat(entries->Count());
		InboxSmsMessagearray = new (ELeave) CDesCArrayFlat(entries->Count());
		InboxSmsDetailsarray = new (ELeave) CDesCArrayFlat(entries->Count());
		iInboxSelectedSms = new CArrayFixFlat<TInt>(entries->Count());
	}
	
	for(TInt count = 0; count < entries->Count(); count++)
	{
		iInboxSelectedSms->InsertL(count, ITEM_SELECTED);
	}

	CleanupStack::PopAndDestroy(2); // entries, parentEntry
	delete pMsv;
}

void CManualSmsListContainer::GetSentItemSms()
{	
	TMsvId smstype;

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
	iCount2 = entries->Count();
	
	if(iReadSmsAO != NULL)
	{
		iReadSmsAO->iSentItemCount	= entries->Count();
	}

	if( entries->Count() == 0)
	{
		SentSmsDatearray = new (ELeave) CDesCArrayFlat(1);
		SentSmsMessagearray = new (ELeave) CDesCArrayFlat(1);
		SentSmsDetailsarray = new (ELeave) CDesCArrayFlat(1);
		iSentItemsSelectedSms = new CArrayFixFlat<TInt>(1);
	}
	else
	{
		SentSmsDatearray = new (ELeave) CDesCArrayFlat(entries->Count());
		SentSmsMessagearray = new (ELeave) CDesCArrayFlat(entries->Count());
		SentSmsDetailsarray = new (ELeave) CDesCArrayFlat(entries->Count());
		iSentItemsSelectedSms = new CArrayFixFlat<TInt>(entries->Count());
	}
	
	for(TInt count = 0; count < entries->Count(); count++)
	{
		iSentItemsSelectedSms->InsertL(count, ITEM_SELECTED);
	}

	CleanupStack::PopAndDestroy(2); // entries, parentEntry
	delete pMsv;
}

void CManualSmsListContainer::CloseReadSms()
{
	iFlag = 0;

	if (iReadSmsAO != NULL)
	{
		delete iReadSmsAO;
		iReadSmsAO = NULL;
	}
}
		
void CManualSmsListContainer::CloseWriteSms()
{
	/*if (iWriteSmsAO != NULL)
		{	
		delete iWriteSmsAO;
		iWriteSmsAO = NULL;
	}*/

	//TInt smsbackuptype = 0;
	//iPhotosAppUi->GetSmsXML(smsbackuptype);
	//iPhotosAppUi->UploadSmsManually(smsbackuptype);

	TInt8 choice;
	iPhotosAppUi->GetChoice(choice);
	if(!choice)
			{
		choice = 1;
		iPhotosAppUi->SaveChoice(choice);
		iPhotosAppUi->ActivateView(KViewIdEmptyScreen);
		iPhotosAppUi->ActivateView(KViewIdStatusScreen);
		//iPhotosAppUi->ActivateView(KViewIdMainScreen);	
			}
			else
			{
		iPhotosAppUi->ActivateView(KViewIdEmptyScreen);
		iPhotosAppUi->ActivateView(KViewIdStatusScreen);
			}
}
CAknSearchField* CManualSmsListContainer::CreateFindBoxL(
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
