/* Copyright (c) 2004, Nokia Mobile Phones. All rights reserved */

#include <cknenv.h>
#include <ckninfo.h>
#include <Ripplevault_series80.rsg>
#include <ckntitle.h>
#include <cknsfld.h>

#include "PhotoScreenListBox.h"
#include <eikappui.h>
#include "Ripplevaultappui.h"
#include "CustomListBox.h"
#include "sms.mbg"
#include "tick.mbg"

#include "statustick.mbg"    // for the icon
#include "statustick1.mbg"    // for the icon

/******************************************************************************
 *
 *   CPhotoScreenListBox implementation
 *
 *****************************************************************************/

// Background colour depending on the controls focus
static const TUint32 KFocusedBkgColor = 0xCCCCCC;
static const TUint32 KNonFocusedBkgColor = 0xEEEEEE;

// Listbox properties
static const TInt KLabelFontHeightInTwips = 180;  // Normal font height is ~200
static const TInt KDataFontHeightInTwips = 180;   // Normal font height is ~200
static const TInt KHorizontalGapBetweenColumns = 10;// Gap betw label and value

void CPhotoScreenListBox::ConstructL(CCoeControl* aWindowOwningContainer,TRect aRect)
{	
    SetContainerWindowL(*aWindowOwningContainer);

    // Application title
    iAppTitle = CCknAppTitle::NewL(CCknAppTitle::EWindow);
    iAppTitle->SetContainerWindowL(*this);
    iAppTitle->SetTextL(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SENDTOVAULT)), CCknAppTitle::EMainTitle);
    // Contact details listbox: first column is label, second column is value
    iListbox = new(ELeave)CEikColumnListBox();
    
    iListbox->ConstructL(aWindowOwningContainer, CEikListBox::ENoExtendedSelection);

    iListbox->CreateScrollBarFrameL(ETrue);

    iListbox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
}


CPhotoScreenListBox::CPhotoScreenListBox(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
    // No implementation required
}

CPhotoScreenListBox::~CPhotoScreenListBox()
{
	if(iPhotoFileIdEnd != -1)
	{
		iConverter->Close();
		iAppUi.DestroyProgressBall();
		delete iBitmapScaler;
		delete iBitmap;
		delete iConverter;
	}

	if(iIsFileOpen)
	{
		iRFsession.Close();
		iIsFileOpen = EFalse;
	}
	
	if(iconList != NULL)
	{
		iconList->Reset();
		iconList = NULL;
	}
	
	if(ImageFilearray != NULL)
	{
		ImageFilearray->Reset();
		ImageFilearray = NULL;
	}

	if(ImageFilePatharray != NULL)
	{
		ImageFilePatharray->Reset();
		ImageFilePatharray = NULL;
	}

	if(ImageDatearray != NULL)
	{
		ImageDatearray->Reset();
		ImageDatearray = NULL;
	}

	if( dirList != NULL )
	{
		delete dirList;
		dirList = NULL;
	}

    delete iAppTitle;
    delete iListbox;
}

void CPhotoScreenListBox::ResetData()
{
	if(iIsFileOpen)
	{
		iRFsession.Close();
		iIsFileOpen = EFalse;
	}

	if(iconList != NULL)
	{
		//iconList->Reset();
		iconList = NULL;
	}
	
	if(ImageFilearray != NULL)
	{
		ImageFilearray->Reset();
		ImageFilearray = NULL;
	}
	
	if(ImageFilePatharray != NULL)
	{
		ImageFilePatharray->Reset();
		ImageFilePatharray = NULL;
	}
	
	if(ImageDatearray != NULL)
	{
		ImageDatearray->Reset();
		ImageDatearray = NULL;
	}
	
	if( dirList != NULL )
	{
		delete dirList;
		dirList = NULL;
	}
}

// Set data values to the listbox
void CPhotoScreenListBox::SetTitle()
{	
	/*TBuf<30> subtitleName;
	
	subtitleName.Copy(_L(""));
	subtitleName.AppendNum(iPhotosCount);
	subtitleName.Append(_L(" Photos"));
	
    iAppTitle->SetTextL(subtitleName, CCknAppTitle::ETitleExtension);*/
	
    DrawDeferred();
}

// Set data values to the listbox
void CPhotoScreenListBox::SetData(TInt aIsmark)
{
	// Construct listbox items (). 
	CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(1);
	
	if(iPhotosCount != 0)
	{
		listItems->Reset();
		CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(iPhotosCount);
	}

    TBuf<256> listItem;
    CleanupStack::PushL(listItems);

	iListbox->HandleItemRemovalL();

	for (TInt i=4;i<iPhotosCount+4;i++)
	{
		TBuf<200> item;		
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
	
		listItems->AppendL(item);	
	}

    CleanupStack::Pop(listItems);

    CTextListBoxModel* model = iListbox->Model();
    model->SetItemTextArray(listItems);   

    // Set model responsible for deleting the listItems array
    model->SetOwnershipType(ELbmOwnsItemArray);

	SetTitle();	
}

// Set data values to the listbox
void CPhotoScreenListBox::ClearData()
{
	SetIconsArray();

	// Construct listbox items (). 
	iListbox->HandleItemRemovalL();

    CDesCArrayFlat* listItems = new (ELeave) CDesCArrayFlat(1);

    TBuf<256> listItem;
    CleanupStack::PushL(listItems);

    CleanupStack::Pop(listItems);

    CTextListBoxModel* model = iListbox->Model();
    model->SetItemTextArray(listItems);   

    // Set model responsible for deleting the listItems array
    model->SetOwnershipType(ELbmOwnsItemArray);	

	iListbox->HandleItemAdditionL();
	iListbox->DrawNow();
}

// Draw this control to the screen. Draw the area empty with
// appropriate color. If the control is focused, draw shadow border.
void CPhotoScreenListBox::Draw(const TRect& aRect) const
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
TKeyResponse CPhotoScreenListBox::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,TEventCode aType)
{
    return iListbox->OfferKeyEventL(aKeyEvent, aType);
}

// Return number of controls within this container. Called by framework.
TInt CPhotoScreenListBox::CountComponentControls() const
{
    return 2;
}

// Return controls within this container. Called by framework to draw them.
CCoeControl* CPhotoScreenListBox::ComponentControl(TInt aIndex) const
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
TSize CPhotoScreenListBox::MinimumSize()
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
void CPhotoScreenListBox::SizeChanged()
{
    innerRect = Border().InnerRect(Rect());
	
	innerRect.iBr.iX = innerRect.iTl.iX + innerRect.Width();

    // Application Title. Fill the top of the control
    TRect titleRect(innerRect.iTl,
        TSize(innerRect.Width(), iAppTitle->MinimumSize().iHeight));
    iAppTitle->SetRect(titleRect);
	
    // Listbox. Fill the remaining control area
    iListbox->SetRect(
        TRect(innerRect.iTl + TPoint(0, iAppTitle->Size().iHeight + 1),
            innerRect.iBr));

    // Update scroll bar to match the control size.
    TRAPD(err, iListbox->UpdateScrollBarsL();)
}

// Called by framework when focus has changed for this control (SetFocus has
// been called)
void CPhotoScreenListBox::FocusChanged(TDrawNow aDrawNow)
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

void CPhotoScreenListBox::SetIconsArray()
{
	CColumnListBoxData* colData = iListbox->ItemDrawer()->ColumnData();
    if(colData)
    {
		colData->SetIconArray(iconList);

		colData->SetGraphicsColumnL(0, ETrue);
        colData->SetColumnWidthPixelL(0, 40);        
		colData->SetColumnHorizontalGapL(0, KHorizontalGapBetweenColumns);        
		colData->SetColumnAlignmentL(0, CGraphicsContext::ELeft);
        
		TInt pos = innerRect.Width() - 70 - KHorizontalGapBetweenColumns ;
		//colData->SetColumnWidthPixelL(1,pos);
		colData->SetColumnWidthPixelL(1,120);		
		colData->SetColumnHorizontalGapL(1, 10);

		colData->SetColumnWidthPixelL(2,200);

		colData->SetGraphicsColumnL(3, ETrue);
		colData->SetColumnWidthPixelL(3,20);	
        colData->SetColumnAlignmentL(3, CGraphicsContext::ELeft);

		colData->SetGraphicsColumnL(4, ETrue);
		colData->SetColumnWidthPixelL(4,20);	
        colData->SetColumnAlignmentL(4, CGraphicsContext::ELeft);
    }

	iListbox->SetItemHeightL(40);
}

TInt CPhotoScreenListBox::Start(TDesC& aFilename)
{
	TBuf<150> tmpbuf;
	iPhotosCount = 0;
	iPhotoFileIdEnd = -1;
	iIsScale = EFalse;
	
	KDirImageFolder1.Copy(aFilename);	
	
	KDirImageFolder1.Append(_L("*"));

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
		if(iIsFileOpen)
		{
			iRFsession.Close();
			iIsFileOpen = EFalse;
		}

		SetTitle();

		return -1;
	}		

	iPhotosCount = dirList->Count(); 

	if(iPhotosCount > 0)
	{
		ImageFilearray = new (ELeave) CDesCArrayFlat(iPhotosCount);
		ImageDatearray = new (ELeave) CDesCArrayFlat(iPhotosCount);			
		iSelectedPhotos = new CArrayFixFlat<TInt>(iPhotosCount);			
		iPhotoListArray = new CArrayFixFlat<TInt>(iPhotosCount);
		ImageFilePatharray = new (ELeave) CDesCArrayFlat(iPhotosCount);

		iconList = new(ELeave) CArrayPtrFlat<CGulIcon>(iPhotosCount);

		GetActualPhotoCount();
	
		iPhotosCount = iPhotoListArray->Count();
	}
	else
	{
		ImageFilearray = new (ELeave) CDesCArrayFlat(1);		
		ImageDatearray = new (ELeave) CDesCArrayFlat(1);			
		iSelectedPhotos = new CArrayFixFlat<TInt>(1);			
		iPhotoListArray = new CArrayFixFlat<TInt>(1);
		ImageFilePatharray = new (ELeave) CDesCArrayFlat(1);

		iconList = new(ELeave) CArrayPtrFlat<CGulIcon>(1);
	}
	
	for(TInt count = 0; count < iPhotosCount; count++)
	{
		iSelectedPhotos->InsertL(count, 10000);
	}

	if(iPhotosCount == 0)
	{
		//iconList->Reset();
		iconList = NULL;
		
		if(iIsFileOpen)
		{
			iRFsession.Close();
			iIsFileOpen = EFalse;
		}

		SetTitle();

		return -1;
	}

	for (TInt i=0;i<iPhotosCount;i++)
	{
		tmpbuf.Copy((*dirList)[iPhotoListArray->At(i)].iName);
		TInt pos =tmpbuf.Find(_L("."));
		tmpbuf.Delete(pos, 5);

		TTime time;
		TBuf<2> ampm;
		TBuf<160> datetime;

		time = (*dirList)[iPhotoListArray->At(i)].iModified;

		TDateTime aDateTime = time.DateTime();		
		_LIT(KFormatTxt,"%d/%d/%d %d:%d:%d ");		
				
		TInt hour = aDateTime.Hour();
		if(hour>12) 
		{
			hour=hour-12;
			ampm.Copy(_L("pm"));
		}
		else
		{
			ampm.Copy(_L("am"));
		}

		datetime.Copy(_L(""));

		datetime.Format(KFormatTxt, aDateTime.Day()+1, TInt(aDateTime.Month()+1), 
			aDateTime.Year(), hour, aDateTime.Minute(), aDateTime.Second());

		datetime.Append(ampm);

		ImageFilearray->InsertL(i, tmpbuf);
		ImageDatearray->InsertL(i, datetime);

		TBuf<128> filepathName;
		filepathName.Copy(aFilename);

		filepathName.Append((*dirList)[iPhotoListArray->At(i)].iName);
		
		ImageFilePatharray->InsertL(i, filepathName);
	}

	iPhotoFileIdEnd = 0;

	TBuf<128> path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KTickMbmFileName);

	iconList->AppendL( CEikonEnv::Static ()->CreateIconL( path,
                                         EMbmTickTick,
                                         EMbmTickTickm ) );

	iconList->AppendL( CEikonEnv::Static ()->CreateIconL( path,
                                         EMbmTickEmpty,EMbmTickEmpty ) );
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

	iAppUi.DisplayProgressBallL(R_TEXT_SCANNING_PROGRESS);

	iBitmapScaler = CMdaBitmapScaler::NewL();

	iConverter = CMdaImageFileToBitmapUtility::NewL(*this);
	
	//Start an asynchronous process to open the gif file
	iBitmap = new (ELeave) CFbsBitmap();
	iBitmap->Create(TSize(15,15),iEikonEnv->SystemGc().Device()->DisplayMode());
	fullentry.Set((*dirList)[iPhotoListArray->At(iPhotoFileIdEnd)].iName, &KDirImageFolder1, NULL);
	iConverter->OpenL(fullentry.FullName());

	return 0;
}

void CPhotoScreenListBox::ScaleBitmap()
{
	iIsScale = ETrue;
	iBitmapScaler->ScaleL(*this, *iBitmap,TSize(44,35));
}

void CPhotoScreenListBox::MiuoOpenComplete(TInt aError)
{
	switch (aError)
	{
		case KErrNone:
		{
			iIsScale = EFalse;
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

void CPhotoScreenListBox::MiuoConvertComplete(TInt aError)
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
		iConverter->Close();

		if (aError == KErrNone)
		{
			iPhotoFileIdEnd++;
			iBitmap->Resize(TSize(44,35));

			CFbsBitmap *iBitmapMask;
			iBitmapMask = iBitmap;

			ico = CGulIcon::NewL(iBitmap, iBitmapMask);
			iconList->AppendL(ico);

			if(iPhotoFileIdEnd == iPhotosCount)
			{
				//Transformation done show the final list
				iPhotoFileIdEnd = -1;
				
				SetIconsArray();

				iAppUi.DestroyProgressBall();

				SetData(0);
				
				iListbox->HandleItemAdditionL();
				iListbox->SetCurrentItemIndex(0);
				iListbox->DrawNow();
			}
			else
			{
				iBitmap = new (ELeave) CFbsBitmap();
				TParse fullentry;
				fullentry.Set((*dirList)[iPhotoListArray->At(iPhotoFileIdEnd)].iName, &KDirImageFolder1, NULL);
				iConverter->OpenL(fullentry.FullName());
			}
		}
	}
}

void CPhotoScreenListBox::MiuoCreateComplete(TInt aError)
{
}

void CPhotoScreenListBox::MarkSelectedPhoto()
{
	if  (iListbox)
	{
	// Cursor's current position
	TInt itemIndex = iListbox->CurrentItemIndex();			

	if (itemIndex >= 0) 
		{
		CTextListBoxModel* model = iListbox->Model();  // not taking ownership
		model->SetOwnershipType (ELbmOwnsItemArray);
		CDesCArray* photoArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
		
		TBuf <2000> changeItem( (*photoArray)[itemIndex] );
		
		TInt len = changeItem.Length();
		
		if ( changeItem.Right(1) == _L("0") ) // Selected -> Deselected
			{
			changeItem.Delete(len-1, 1);
			changeItem.Append(_L("1"));
			iSelectedPhotos->Delete(itemIndex);
			iSelectedPhotos->InsertL(itemIndex, 10000);
			}		
		else  // Deselected -> Selected		
			{
			changeItem.Delete(len-1, 1);
			changeItem.Append(_L("0"));
			iSelectedPhotos->Delete(itemIndex);
			iSelectedPhotos->InsertL(itemIndex, itemIndex);
			}
		photoArray->Delete(itemIndex);
		photoArray->InsertL(itemIndex, changeItem);

		iListbox->HandleItemAdditionL();
		}
	}
}

void CPhotoScreenListBox::MarkAllItems()
{
	for(TInt count = 0; count < iPhotosCount; count++)
	{
		iSelectedPhotos->Delete(count);
		iSelectedPhotos->InsertL(count, count);
	}

	iCursorPos = 0;

	SetData(1);

	iListbox->HandleItemAdditionL();
	iListbox->SetCurrentItemIndex(iCursorPos);
	iListbox->DrawNow();
}

void CPhotoScreenListBox::UnMarkAllItems()
{
	for(TInt count = 0; count < iPhotosCount; count++)
	{
		iSelectedPhotos->Delete(count);
		iSelectedPhotos->InsertL(count, 10000);
	}

	iCursorPos = 0;

	SetData(1);

	iListbox->HandleItemAdditionL();
	iListbox->SetCurrentItemIndex(iCursorPos);
	iListbox->DrawNow();
}

void CPhotoScreenListBox::MarkUnMarkSelectedItem()
{
	for(TInt count = 0; count < iPhotosCount; count++)
	{
		if(iListbox->CurrentItemIndex() == count)
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
		MarkSelectedPhoto();
	}
	else
	{
		MarkSelectedPhoto();
	}
}

void CPhotoScreenListBox::GetActualPhotoCount()
{
	TBuf<150> tmpbuf;

	_LIT(KBmp,"bmp");
	_LIT(KJpg,"jpg");
	_LIT(KJpeg,"gif");

	//for(TInt i = 0; i < iPhotosCount; i++)
	for(TInt i = iPhotosCount-1; i >=0 ; i--)
	{
		tmpbuf.Copy((*dirList)[i].iName);
		TInt pos =tmpbuf.Find(_L("."));
		tmpbuf.Delete(0,pos+1);
		tmpbuf.LowerCase();
		if(tmpbuf.Compare(KBmp) == 0 || tmpbuf.Compare(KJpg) == 0 || tmpbuf.Compare(KJpeg) == 0)
		{
			iPhotoListArray->AppendL(i);
		}
	}
}

void CPhotoScreenListBox::SendPhotoToServer()
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
				fullentry.Set((*dirList)[iPhotoListArray->At(PhotoNo)].iName, &KDirImageFolder1, NULL);
	
				imageFile.Copy(fullentry.FullName());
				iPublic_Photo=EFalse;
				iAppUi.AddPhotoRecord(imageFile, iPublic_Photo, comment,description, iNewFolder, (*dirList)[iPhotoListArray->At(PhotoNo)].iSize);//
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
	/*TInt8 choice;
	iAppUi.GetChoice(choice);
	if(choice == 0)
	{
		iAppUi.ShowWindow(KViewIdVideoList);
		iAppUi.DeleteWindow(KViewIdPhotoList);
	}
	else
	{*/
		//iAppUi.ShowWindow(KViewIdRegisterScreen);
		iAppUi.ShowWindow(KViewIdStatusScreen);
		iAppUi.ShowStatusScreen();

		iAppUi.DeleteWindow(KViewIdPhotoList);
	//}
}

TInt CPhotoScreenListBox::IsImageSelected()
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
