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
 Module Name				: PhotoScreenView_Uiq.cpp
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
// PhotoScreenView.cpp
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
#include "images.mbg"    // for the icon
#include "statustick.mbg"    // for the icon
#include "statustick1.mbg"    // for the icon
#include <QuartzKeys.h> // keys for jog dial
#include <eikmenub.h>
#include <gulicon.h>
#include <ripplevault_uiq.rsg>

#include "PhotoScreenView_Uiq.h"
#include "MediaScreenListBox.h"
#include "RippleVaultAppUi.h"

//Static NewL() function to start the standard two phase construction.
CPhotoScreenView* CPhotoScreenView::NewL(const TRect& aRect, CRippleVaultAppUi& aAppUi)
{
	CPhotoScreenView* self = new(ELeave) CPhotoScreenView(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop();
	return self;
}

CPhotoScreenView::CPhotoScreenView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
}


//Destructor for the view.
CPhotoScreenView::~CPhotoScreenView()
{
	if(iBitmapScaler)
	{
		iBitmapScaler->CancelScaling();
		iBitmapScaler = NULL;
	}

	if(iConverter)
	{
		iConverter->CancelConvertL();
		iConverter->Close();
		iConverter = NULL;
	}
	
	if(iIsFileOpen)
	{
		iRFsession.Close();
		iIsFileOpen = EFalse;
	}
	
	if(iconList)
	{
		iconList->Reset();
	}
	
	if(ImageFilearray)
	{
		ImageFilearray->Reset();
	}

	if(ImageFilePatharray)
	{
		ImageFilePatharray->Reset();
	}

	if(ImageDatearray)
	{
		ImageDatearray->Reset();
	}

	if( dirList != NULL )
	{
		delete dirList;
		dirList = NULL;
	}

	delete iListBox;	             
}

void CPhotoScreenView::ResetData()
{
	if(iBitmapScaler)
	{
		iBitmapScaler->CancelScaling();
		iBitmapScaler = NULL;
	}

	if(iConverter)
	{
		iConverter->CancelConvertL();
		iConverter->Close();
		iConverter = NULL;
	}

	if(iIsFileOpen)
	{
		iRFsession.Close();
		iIsFileOpen = EFalse;
	}
	
	if(iconList)
	{
		iconList->Reset();
	}
	
	if(ImageFilearray)
	{
		ImageFilearray->Reset();
	}
	
	if(ImageFilePatharray)
	{
		ImageFilePatharray->Reset();
	}
	
	if(ImageDatearray)
	{
		ImageDatearray->Reset();
	}

	if( dirList != NULL )
	{
		delete dirList;
		dirList = NULL;
	}
}

void CPhotoScreenView::ConstructL(const TRect& aRect)
{
	CreateWindowL();

	CreateListBoxL();

	SetRect(aRect);
	ActivateL();
}

void CPhotoScreenView::UpdateListBoxL(TInt aIsmark)
{
	iListBox->HandleItemRemovalL(); 

	SetIconsArray();

	CDesCArrayFlat* array = new (ELeave) CDesCArrayFlat(iPhotosCount);

	CleanupStack::PushL(array);

	for (TInt i=4;i<iPhotosCount+4;i++)
	{
		TBuf<500> item;		
		
		TInt ret = 3;
		TBuf<128> filearray;

		filearray.Copy((*ImageFilePatharray)[i-4]);

		ret = iAppUi.CheckMediaUploadExists(filearray);

		if(aIsmark == 0)
		{
			item.Format(_L("%d\t%S\t%S\t%d\t%d"),i , &(*ImageFilearray)[i-4], &(*ImageDatearray)[i-4], ret, 1); 
		}
		else
		{
			if(iSelectedPhotos->At(i-4) == 10000)
			{
				item.Format(_L("%d\t%S\t%S\t%d\t%d"),i , &(*ImageFilearray)[i-4], &(*ImageDatearray)[i-4], ret, 1);
			}
			else
			{
				item.Format(_L("%d\t%S\t%S\t%d\t%d"),i , &(*ImageFilearray)[i-4], &(*ImageDatearray)[i-4], ret, 0); 
			}
		}
	
		array->AppendL(item);	
	}

	CleanupStack::Pop();

	iListBox->Model()->SetItemTextArray(array);

	// We need to set the current item index and call DrawNow()
	// so that when the user changes the list box to single/double,
	// it will be updated properly.
	iListBox->HandleItemAdditionL(); 
	iListBox->SetCurrentItemIndex(iCursorPos);
	iListBox->DrawNow();	
	//iListBox->SetCurrentItemIndexAndDraw(0);
	//iListBox->UpdateCurrentItem(0); 
}

void CPhotoScreenView::EmptyListBoxL()
{
	iListBox->HandleItemRemovalL(); 

	//SetIconsArray();

	CDesCArrayFlat* array = new (ELeave) CDesCArrayFlat(1);
	CleanupStack::PushL(array);

	CleanupStack::Pop();

	iListBox->Model()->SetItemTextArray(array);

	// We need to set the current item index and call DrawNow()
	// so that when the user changes the list box to single/double,
	// it will be updated properly.
	iListBox->HandleItemAdditionL(); 
	iListBox->DrawNow();

	//DrawText();
}

void CPhotoScreenView::SizeChanged()
	{
	if (iListBox)
		{
		iListBox->SetRect(Rect());
		}
	}

TInt CPhotoScreenView::CountComponentControls() const
{
	if (iListBox)
		{
		return 1;
		}
	return 0;
}

CCoeControl* CPhotoScreenView::ComponentControl(TInt aIndex) const
{
	switch (aIndex)
		{
		case 0:
			return iListBox;
		default:
			return NULL;
		}
}

void CPhotoScreenView::Draw(const TRect& aRect) const
{
	CWindowGc& gc = SystemGc();
	gc.SetPenStyle(CGraphicsContext::ENullPen);
	gc.SetBrushColor(KRgbWhite);
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc.DrawRect(aRect);
}

void CPhotoScreenView::DrawText()
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

TKeyResponse CPhotoScreenView::OfferKeyEventL(const TKeyEvent& aKeyEvent,
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

void CPhotoScreenView::HandleListBoxEventL(CEikListBox* aListBox,
											TListBoxEvent aEventType)
{
	if ((MEikListBoxObserver::EEventEnterKeyPressed == aEventType)
		|| (MEikListBoxObserver::EEventItemClicked == aEventType))
        {
			MarkUnMarkSelectedItem();
		}
}

void CPhotoScreenView::CreateListBoxL()
	{
	// Creates a new PhotoScreen list box.
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

	//iListBox->SetListEmptyTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_TEXT_EMPTY)));

	iCursorPos  = 0;
	}

void CPhotoScreenView::SetIconsArray()
{
	// Sets the icon array.
	CMediaScreenListItemDrawer* itemDrawer = static_cast<CMediaScreenListItemDrawer*>
		(iListBox->View()->ItemDrawer());
	//itemDrawer->SetIconArray(iconArray); // transfer ownership
	itemDrawer->SetIconArray(iconList); // transfer ownership 

	// Sets the height of the list box item
	// (= 2 * maximum height of the icon).
	TSize size = itemDrawer->MaxIconSize();
	//iListBox->SetItemHeightL(1.5 * size.iHeight);
	iListBox->SetItemHeightL(55);
}

TInt CPhotoScreenView::Start(TDesC& aFilename)
{
	TBuf<150> tmpbuf;
	iPhotosCount = 0;
	iPhotoFileId = 0;
	iPhotoFileIdEnd = 0;
	iIsScale = EFalse;
	
	KDir1.Copy(aFilename);

	KDirImageFolder1.Copy(aFilename);	
	KDirImageFolder1.Append(_L("\\*"));

	iBitmapScaler = NULL;
	iConverter = NULL;

	if( !iIsFileOpen)
	{
		User::LeaveIfError(iRFsession.Connect());
		iIsFileOpen = ETrue;
	}
	else
	{
		return -1;
	}
	
	TParse fullentry;
	TInt err = iRFsession.GetDir(KDirImageFolder1, KEntryAttNormal, ESortByDate, dirList);
	
	if (err != KErrNone)
	{
		EmptyListBoxL();
		
		return -1;
	}		

	iPhotosCount = dirList->Count(); 

	if(iPhotosCount > 0)
	{
		ImageFilearray = new (ELeave) CDesCArrayFlat(iPhotosCount);
		ImageDatearray = new (ELeave) CDesCArrayFlat(iPhotosCount);			
		ImageFilePatharray = new (ELeave) CDesCArrayFlat(iPhotosCount);
		iSelectedPhotos = new CArrayFixFlat<TInt>(iPhotosCount);

		iconList = new(ELeave) CArrayPtrFlat<CGulIcon>(iPhotosCount);
	}
	else
	{
		ImageFilearray = new (ELeave) CDesCArrayFlat(1);		
		ImageDatearray = new (ELeave) CDesCArrayFlat(1);			
		ImageFilePatharray = new (ELeave) CDesCArrayFlat(1);
		iSelectedPhotos = new CArrayFixFlat<TInt>(1);

		iconList = new(ELeave) CArrayPtrFlat<CGulIcon>(1);
	}

	for(TInt count = 0; count < iPhotosCount; count++)
	{
		iSelectedPhotos->InsertL(count, 10000);
	}

	if(iPhotosCount == 0)
	{
		iconList->Reset();
		
		if(iIsFileOpen)
		{
			iRFsession.Close();
			iIsFileOpen = EFalse;
		}

		EmptyListBoxL();
		
		return -1;
	}

	TBuf<128> path;	
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KTickMbmFileName);

	iconList->AppendL( CEikonEnv::Static ()->CreateIconL( path,
                                         EMbmTickTick,
                                         EMbmTickTickm ) );

	iconList->AppendL( CEikonEnv::Static ()->CreateIconL( path,
                                         EMbmTickEmpty,EMbmTickEmpty ));

	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KStatusMbmFileName);

	iconList->AppendL( iEikonEnv->CreateIconL(path,
                                             EMbmStatustickStatustick,
                                             EMbmStatustickStatustickm ));
	
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KStatusMbmFileName1);

	iconList->AppendL( iEikonEnv->CreateIconL(path,
                                             EMbmStatustick1Statustick1,
                                             EMbmStatustick1Statustick1m ));

	for (TInt i=0;i<iPhotosCount;i++)
	{
		tmpbuf.Copy((*dirList)[iPhotosCount-1-i].iName);
		TInt pos =tmpbuf.Find(_L("."));
		tmpbuf.Delete(pos, 5);

		TTime time;
		TBuf<160> datetime;

		time = (*dirList)[iPhotosCount-1-i].iModified;

		TDateTime aDateTime = time.DateTime();		
		_LIT(KFormatTxt,"%d/%d/%d %d:%d:%d ");		
				
		TInt hour = aDateTime.Hour();
		
		datetime.Copy(_L(""));

		datetime.Format(KFormatTxt, aDateTime.Day()+1, TInt(aDateTime.Month()+1), 
			aDateTime.Year(), hour, aDateTime.Minute(), aDateTime.Second());

		ImageFilearray->InsertL(i, tmpbuf);
		ImageDatearray->InsertL(i, datetime);

		TBuf<128> filepathName;

		if( iAppUi.iImageFlag == 1)
		{
			filepathName.Copy(KFolderPhonePhoto);
		}
		else if( iAppUi.iImageFlag == 2)
		{
			filepathName.Copy(KFolderMMCPhoto);
		}
		else if( iAppUi.iImageFlag == 3)
		{
			filepathName.Copy(KFolderPhoneVideo);
		}
		else if( iAppUi.iImageFlag == 4)
		{
			filepathName.Copy(KFolderMMCVideo);
		}

		filepathName.Append((*dirList)[iPhotosCount-1-i].iName);
		
		ImageFilePatharray->InsertL(i, filepathName);

		TBuf<128> path1;
		path1 = CRippleVaultAppUi::ApplicationDriveAndPath();
		path1.Append(KImageMbmFileName);


		iconList->AppendL( CEikonEnv::Static ()->CreateIconL( path1,
            		                         EMbmImagesImage,
                    		                 EMbmImagesImagem ) );
	}	

	UpdateListBoxL(0);

	iBitmapScaler = CMdaBitmapScaler::NewL();

	iConverter = CMdaImageFileToBitmapUtility::NewL(*this);
	
	//Start an asynchronous process to open the gif file
	iBitmap = new (ELeave) CFbsBitmap();
	iBitmap->Create(TSize(30,30),iEikonEnv->SystemGc().Device()->DisplayMode());
	fullentry.Set((*dirList)[iPhotosCount-1-iPhotoFileIdEnd].iName, &KDirImageFolder1, NULL);
	iConverter->OpenL(fullentry.FullName());
	
	return 0;
}

void CPhotoScreenView::ScaleBitmap()
{
	iIsScale = ETrue;
	iBitmapScaler->ScaleL(*this, *iBitmap,TSize(44,35));
}

void CPhotoScreenView::MiuoOpenComplete(TInt aError)
{
	switch (aError)
	{	
		case KErrNone:
		{
			iIsScale = EFalse;
			iPhotoFileId++;
			TFrameInfo info;
			iConverter->FrameInfo (0,info);
			TInt err = iBitmap->Create (info.iOverallSizeInPixels,iEikonEnv->SystemGc().Device()->DisplayMode ());
			
			if( err == KErrNone )
			{
				TRAP(err, iConverter->ConvertL(*iBitmap));
			}
		
			if( err!= KErrNone )
			{
				iIsScale = EFalse;
				MiuoConvertComplete(err);
			}
		}
		break;
		
		default:
		{
			iConverter->Close();
			break;
		}
	}
}

void CPhotoScreenView::MiuoConvertComplete(TInt aError)
{
	if(!iIsScale)
	{
		//when the Convert is complete, I set an inner variable iLoaded
		if (aError == KErrNone)
		{
			ScaleBitmap();
		}
		else
		{
			//Display Error
		}		
	}
	else
	{
		if (aError == KErrNone)
		{
			iPhotoFileIdEnd++;
			iBitmap->Resize(TSize(44,35));

			CFbsBitmap *iBitmapMask;
			iBitmapMask = iBitmap;

			ico = CGulIcon::NewL(iBitmap, iBitmapMask);

			TInt pos;
			pos = iPhotoFileIdEnd + 3;
            iconList->Delete(pos);
            iconList->InsertL(pos,ico);

            UpDateIconList();

			if(iPhotoFileIdEnd == iPhotosCount)
			{
				//Transformation done show the final list
				iPhotoFileIdEnd = 0;
				
				iBitmapScaler = NULL;
				iConverter = NULL;
			}
			else
			{
				iBitmap = new (ELeave) CFbsBitmap();
				TParse fullentry;
				fullentry.Set((*dirList)[iPhotosCount-1-iPhotoFileIdEnd].iName, &KDirImageFolder1, NULL);
				iConverter->OpenL(fullentry.FullName());				
			}
		}
	}
}

void CPhotoScreenView::MiuoCreateComplete(TInt aError)
{
}

void CPhotoScreenView::MarkAllItems()
{
	for(TInt count = 0; count < iPhotosCount; count++)
	{
		iSelectedPhotos->Delete(count);
		iSelectedPhotos->InsertL(count, count);
	}

	MarkUnMarAllPhoto(0);
}

void CPhotoScreenView::UnMarkAllItems()
{
	for(TInt count = 0; count < iPhotosCount; count++)
	{
		iSelectedPhotos->Delete(count);
		iSelectedPhotos->InsertL(count, 10000);
	}

	MarkUnMarAllPhoto(1);
}

void CPhotoScreenView::MarkUnMarkSelectedItem()
{
	for(TInt count = 0; count < iPhotosCount; count++)
	{
		if(iListBox->CurrentItemIndex() == count)
		{
			if(iSelectedPhotos->At(count) != 10000)
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
	
	if( iIsMark  )
	{
		MarkUnMarkSelectedPhoto();
	}
	else
	{
		MarkUnMarkSelectedPhoto();
	}
}

void CPhotoScreenView::MarkUnMarkSelectedPhoto()
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
		
		TBuf <300> changeItem( (*dataArray)[itemIndex] );
		
		TInt len = changeItem.Length();
		
		if ( changeItem.Right(1) == _L("0") ) // Selected -> Deselected
			{
			changeItem.Delete(len-1, 1);
			changeItem.Append(_L("1"));
			iSelectedPhotos->Delete(itemIndex);
			iSelectedPhotos->InsertL(itemIndex, 10000);
			}		
		else  if ( changeItem.Right(1) == _L("1") )// Deselected -> Selected		
			{
			changeItem.Delete(len-1, 1);
			changeItem.Append(_L("0"));
			iSelectedPhotos->Delete(itemIndex);
			iSelectedPhotos->InsertL(itemIndex, itemIndex);
			}
		dataArray->Delete(itemIndex);
		dataArray->InsertL(itemIndex, changeItem);

		iListBox->HandleItemAdditionL();
		}
	}
}

void CPhotoScreenView::MarkSelectedPhoto()
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
		
		TBuf <300> changeItem( (*dataArray)[itemIndex] );
		
		TInt len = changeItem.Length();
		
		if ( changeItem.Right(1) == _L("0") ) // Selected -> Deselected
			{
			}		
		else  if ( changeItem.Right(1) == _L("1") )// Deselected -> Selected		
			{
			changeItem.Delete(len-1, 1);
			changeItem.Append(_L("0"));
			iSelectedPhotos->Delete(itemIndex);
			iSelectedPhotos->InsertL(itemIndex, itemIndex);
			}
		dataArray->Delete(itemIndex);
		dataArray->InsertL(itemIndex, changeItem);

		iListBox->HandleItemAdditionL();
		}
	}
}

void CPhotoScreenView::MarkUnMarAllPhoto(TInt aIsMark)
{
	if  (iListBox)
	{
		for(TInt count = 0; count < iPhotosCount; count++)
		{
			CTextListBoxModel* model = iListBox->Model();  // not taking ownership
			model->SetOwnershipType (ELbmOwnsItemArray);
			CDesCArray* dataArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
			
			TBuf <300> changeItem( (*dataArray)[count] );
			
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


void CPhotoScreenView::UpDateIconList()
{
	if (iListBox)
	{
		TInt top = iListBox->TopItemIndex();
		iListBox->SetCurrentItemIndex(top);

		iListBox->HandleItemAdditionL();
	}
}

TInt CPhotoScreenView::IsImageSelected()
{
	TInt Ret=0;
	for(TInt count = 0; count < iPhotosCount; count++)
	{
		if(	iSelectedPhotos->At(count) != 10000)
		{
			Ret = 1;
			break;
		}
	}
	return Ret;
}

void CPhotoScreenView::SendPhotoToServer()
{
	TInt answer=0;
	TInt8 intrusionmode;
	TBuf<160> imageFile;
	TBuf<160> description;
	TParse fullentry;
	TInt iPublic_Photo=0;
	
	intrusionmode = iAppUi.GetMode();

	iPublic_Photo=EFalse;

	if(intrusionmode == MODE_PROMPT || intrusionmode == 2)
	{
		iAppUi.BringToForeground();	
		answer=iAppUi.FolderDialog();	

		if(answer)
		{
			comment.Copy(iAppUi.FolderName);

			CDesCArray* array;
			TInt aPos;
			array = iAppUi.GetFolderList();

			if(array)
			{
				if(array->Find(iAppUi.FolderName, aPos) != 0)
				{
					iNewFolder=1;

					iAppUi.CheckAndSaveFolderToDB(iAppUi.FolderName);
				}
				else
				{
					//Folder already exist
					iNewFolder=0;
				}
			}
			else
			{
				iNewFolder=1;
				iAppUi.CheckAndSaveFolderToDB(iAppUi.FolderName);
			}
		} 
		else
		{
			// cancelled
		}		
	}
	else if(intrusionmode==MODE_AUTO)// non-intrusive mode
	{
		iPublic_Photo=EFalse;	
	
		comment.Copy(iAppUi.GetDefaultFolder());
		iNewFolder=0;
		answer = 1; // upload
	}
	
	if (answer)
	{
		for (TInt PhotoNo =0; PhotoNo< iPhotosCount; PhotoNo++)
		{	
			if(iSelectedPhotos->At(PhotoNo) != 10000)
			{	
				fullentry.Set((*dirList)[iPhotosCount-1-PhotoNo].iName, &KDirImageFolder1, NULL);
	
				imageFile.Copy(fullentry.FullName());
				iPublic_Photo=EFalse;
				iAppUi.AddPhotoRecord(imageFile, iPublic_Photo, comment,description, iNewFolder, (*dirList)[iPhotosCount-1-PhotoNo].iSize);//
				//iEikonEnv->InfoWinL(comment,imageFile);
			}
		}

		iAppUi.ScheduleToSend();

		/*TInt network;		
		network = iAppUi.CheckNetwork();
		if(network == NETWORK_ROAMING)
		{
			if(iAppUi.iRoaming == 1)
			{
				iAppUi.ScheduleToSend();
			}
			else
			{
				iAppUi.RoamingDisabledDialogPublish();
			}
			
		}
		else
		{	
			iAppUi.ScheduleToSend();
		}*/
	}	

	//iAppUi.ShowView();
	TInt8 choice;
	iAppUi.GetChoice(choice);
	if(choice == 0)
	{
		iAppUi.iImageFlag = 3;
		iAppUi.ShowWindow(KViewIdVideoList); //video screen
		iAppUi.DeleteWindow(KViewIdPhotoList); //photo screen
	}
	else
	{
		iAppUi.iMainScreenValue = 6;
		iAppUi.ShowWindow(KViewIdStatusScreen); //status screen
		iAppUi.DeleteWindow(KViewIdPhotoList);
		iAppUi.ShowStatusScreen();
	}
}

TInt CPhotoScreenView::GetCount()
{
	TInt count=0;
	for (TInt PhotoNo =0; PhotoNo< iPhotosCount; PhotoNo++)
	{	
		if(iSelectedPhotos->At(PhotoNo) != 10000)
		{	
			count++;
		}
	}
	return count;
}
// End of File  
