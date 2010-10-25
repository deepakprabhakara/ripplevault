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
 Module Name				: PhotoListContainer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 17-06-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     To Show Thumbnails Photo List
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/

// INCLUDE FILES
#include "PhotoListContainer.h"
#include "RippleVaultAppUi.h"
#include <eikclbd.h>
#include <gulicon.h>
#include <akniconarray.h>
#include <palbimageutil.h> 
#include <palbimagedata.h>
#include <bautils.h>
#include "phototick.mbg"
#include "statustick.mbg"
#include "statustick1.mbg"
#include "empty.mbg"

#include <akntitle.h>
#include <akncontext.h>
#include "rippleimage.mbg"

#include  <ripplevault.rsg>

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPhotoListContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------

void CPhotoListContainer::ConstructL(const TRect& aRect)
{
	DrawStausPane();

	User::LeaveIfError(iRFsession.Connect());

	iCursorPos=0;
	iPhoneIconList = NULL;	
	iMmcIconList = NULL;
	iListBox = NULL;
	iPhoneCount = 0;
	iMmcCount = 0;
	iFlag = 0;

	iSelectedPhonePhotos = new CArrayFixFlat<TInt>(1);
	iSelectedMmcPhotos = new CArrayFixFlat<TInt>(1);

	ShowListBox();

	GetPhoneImage();	
	
	SetFileList(0);

	TRect ListWindow(TPoint(0,44),TPoint(176,188));
	SetRect(ListWindow);
	ActivateL();
}

// Destructor
CPhotoListContainer::~CPhotoListContainer()
{
	 delete iBitmap;
	 
	 iRFsession.Close();

	 delete iNaviDeco ;	

	 //if(iListBox)
		 //delete iListBox;
}

// ---------------------------------------------------------
// CPhotoListContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------

void CPhotoListContainer::SizeChanged()
{
    // TODO: Add here control resize code etc.
	iListBox->SetExtent(TPoint(0,0),TSize(176,160));
}

// ---------------------------------------------------------
// CPhotoListContainer::CountComponentControls() const
// ---------------------------------------------------------

TInt CPhotoListContainer::CountComponentControls() const
{
	return 1; // return nbr of controls inside this container
}

// ---------------------------------------------------------
// CPhotoListContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------

CCoeControl* CPhotoListContainer::ComponentControl(TInt aIndex) const
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
// CPhotoListContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CPhotoListContainer::Draw(const TRect& aRect ) const
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
// CPhotoListContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CPhotoListContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
{
	// TODO: Add your control event handler code here
}

TKeyResponse CPhotoListContainer::OfferKeyEventL(
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

					if( iPhotosAppUi->iStoragePath == PHOTOS_MMC)
						iPhotosAppUi->iStoragePath = PHOTOS_PHONEMEMORY;
					else
						iPhotosAppUi->iStoragePath = VIDEOS_PHONEMEMORY;

					CFormattedCellListBoxData* colData = iListBox->ItemDrawer()->ColumnData();

					if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
					{	
						colData->SetIconArray(iPhoneIconList);
					}
					else
					{				
						colData->SetIconArray(iMmcIconList);
					}

					//iPhotosAppUi->ActivateView(KViewIdEmptyScreen );
					//iPhotosAppUi->ActivateView(KViewIdPhotoList );
					SetFileList(1);
                }
            break;
		case EKeyRightArrow:
            if( (active + 1) < count )
                {
					active++;	
					
					iTabGroup->SetActiveTabById(1);
					
					if( iPhotosAppUi->iStoragePath == PHOTOS_PHONEMEMORY)
						iPhotosAppUi->iStoragePath = PHOTOS_MMC;
					else
						iPhotosAppUi->iStoragePath = VIDEOS_MMC;

					//iPhotosAppUi->ActivateView(KViewIdEmptyScreen );
					//iPhotosAppUi->ActivateView(KViewIdPhotoList );
					if(iFlag == 0)
					{
						GetMmcImage();
					}

					iFlag = 1;

					CFormattedCellListBoxData* colData = iListBox->ItemDrawer()->ColumnData();

					if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
					{	
						colData->SetIconArray(iPhoneIconList);
					}
					else
					{				
						colData->SetIconArray(iMmcIconList);
					}

					SetFileList(1);
                }
            break;
		case EKeyOK:
			{
				TInt count;
				if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
					count = iPhoneCount;
				else
					count = iMmcCount;

				if(count > 0)
					MarkUnMarkSelectedItem();
			}
			break;
        default:
            // Let Listbox take care of its key handling
            return iListBox->OfferKeyEventL(aKeyEvent, aType);
            break;
        }
   		return (TKeyResponse)0;
    }

void CPhotoListContainer::ShowListBox()
{
	CreateWindowL();
		
	iListBox = new (ELeave) CAknDoubleLargeStyleListBox;
	
	iListBox->SetContainerWindowL( *this );
	iListBox->ConstructL( this, EAknListBoxMarkableList );
	
	// Create the scroll indicator
	iListBox->CreateScrollBarFrameL(ETrue);
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
	
	iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );
	
	if(iPhotosAppUi->iStoragePath == PHOTOS_PHONEMEMORY)
	{
		iListBox->View()->SetListEmptyTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_WEBUPLOAD_NOPHOTOIMAGE)));
	}
	else
	{
		iListBox->View()->SetListEmptyTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_WEBUPLOAD_NOVIDEOIMAGE)));
	}
	
	iListBox->ActivateL();
	iListBox->SetFocus(ETrue, EDrawNow);
}

void CPhotoListContainer::AppendThumnails()
{
	TBuf<150> pathName;
	TInt count;
	
	if( iPhotosAppUi->ImageFolderPath() == 0)
	{
		pathName.Copy(KDirPhonePhoto);
		count = iPhoneCount;
	}
	else if( iPhotosAppUi->ImageFolderPath() == 1)
	{
		pathName.Copy(KDirMMCPhoto);
		count = iMmcCount;
	}
	else if( iPhotosAppUi->ImageFolderPath() == 2)
	{
		pathName.Copy(KDirPhoneVideo);
		count = iPhoneCount;
	}
	else if( iPhotosAppUi->ImageFolderPath() == 3)
	{
		pathName.Copy(KDirMMCVideo);
		count = iMmcCount;
	}

	CPAlbImageUtil* imageAlbum = CPAlbImageUtil::NewL();
	CPAlbImageData* imageData = CPAlbImageData::NewL();
	
	CGulIcon* ico;
	TBuf<150> tmpbuf;

	if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
	{
		iPhoneIconList = new(ELeave) CArrayPtrFlat<CGulIcon>(count);
		CleanupStack::PushL(iPhoneIconList);
	}
	else
	{
		iMmcIconList = new(ELeave) CArrayPtrFlat<CGulIcon>(count);
		CleanupStack::PushL(iMmcIconList);
	}

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KPhotoTickMbmFileName);

	if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
	{
		iPhoneIconList->AppendL( iEikonEnv->CreateIconL(path,
                                             EMbmPhototickMarked_add,
                                             EMbmPhototickMarked_add_m ) );
	}
	else
	{
		iMmcIconList->AppendL( iEikonEnv->CreateIconL(path,
                                             EMbmPhototickMarked_add,
                                             EMbmPhototickMarked_add_m ) );
	}

	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KEmptyMbmFileName);

	if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
	{
		iPhoneIconList->AppendL( iEikonEnv->CreateIconL(path,
                                             EMbmAEmpty,
                                             EMbmAEmpty ) );
	}
	else
	{
		iMmcIconList->AppendL( iEikonEnv->CreateIconL(path,
                                             EMbmAEmpty,
                                             EMbmAEmpty ) );
	}

	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KStatusMbmFileName);

	if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
	{
		iPhoneIconList->AppendL( iEikonEnv->CreateIconL(path,
                                             EMbmStatustickStatustick,
                                             EMbmStatustickStatustickm ) );
	}
	else
	{
		iMmcIconList->AppendL( iEikonEnv->CreateIconL(path,
                                             EMbmStatustickStatustick,
                                             EMbmStatustickStatustickm ) );
	}

	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KStatusMbmFileName1);

	if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
	{
		iPhoneIconList->AppendL( iEikonEnv->CreateIconL(path,
                                             EMbmStatustick1Statustick1,
                                             EMbmStatustick1Statustick1m ) );
	}
	else
	{
		iMmcIconList->AppendL( iEikonEnv->CreateIconL(path,
                                             EMbmStatustick1Statustick1,
                                             EMbmStatustick1Statustick1m ) );
	}

#if 1
	if(iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 1)//Images
	{
		for (TInt i = count-1; i>=0; i--)
		{
			//iPhotosAppUi->LogText.Copy((*dirList)[i].iName);
			//iPhotosAppUi->LogText.Append(_L("\n"));
			//iPhotosAppUi->WriteLogFile(iPhotosAppUi->LogText);

			imageData->SetImageNameL((*dirList)[i].iName);

#ifdef __NOKIA6600__
			imageData->SetSubFolderL(pathName);
#endif
			TRAPD(e, iBitmap = imageAlbum->GetThumbnailL(*imageData));

			if(iBitmap == NULL)
			{
				iBitmap = new (ELeave) CFbsBitmap();

				TBuf<128> path;
				path = CRippleVaultAppUi::ApplicationDriveAndPath();
				path.Append(KImageMbmFileName);

				iBitmap->Load(path);
			}

			//iPhotosAppUi->LogText.Copy(_L("AppendThumbnails 1\n"));
			//iPhotosAppUi->WriteLogFile(iPhotosAppUi->LogText);

			//30x40, 36x44, 40x30, 40x48, 44x36
			//aBitmap->Resize(TSize(48,30));
			ico = CGulIcon::NewL(iBitmap, NULL);

			if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
			{
				
				iPhoneIconList->AppendL(ico);
			}
			else
			{				
				iMmcIconList->AppendL(ico);
			}

			tmpbuf.Copy((*dirList)[i].iName);
			TInt pos =tmpbuf.Find(_L("."));
			if(pos != -1)
				tmpbuf.Delete(pos, 5);
			
			TBuf<128> filepathName;

			if( iPhotosAppUi->ImageFolderPath() == 0)
			{
				filepathName.Copy(KFolderPhonePhoto);
			}
			else if( iPhotosAppUi->ImageFolderPath() == 1)
			{
				filepathName.Copy(KFolderMMCPhoto);
			}
			else if( iPhotosAppUi->ImageFolderPath() == 2)
			{
				filepathName.Copy(KFolderPhoneVideo);
			}
			else if( iPhotosAppUi->ImageFolderPath() == 3)
			{
				filepathName.Copy(KFolderMMCVideo);
			}

			filepathName.Append((*dirList)[i].iName);
			
			if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
			{
				ImageDataPhonearray->InsertL(count - 1 - i, tmpbuf);
				ImageDataPhoneFilearray->InsertL(count - 1 - i, filepathName);
			}
			else
			{
				ImageDataMmcarray->InsertL(count - 1 - i, tmpbuf);
				ImageDataMmcFilearray->InsertL(count - 1 - i, filepathName);
			}

			TBuf<50> tmptime;
			
			TTime time;
			time = (*dirList)[i].iModified;

			TDateTime aDateTime = time.DateTime();
			
			//dd/mm/yyyy hh:mm:ss am
			_LIT(KFormatTxt,"%d/%d/%d-%d:%d:%d ");
				
			TBuf<2> ampm;

			TInt hour = aDateTime.Hour();
				if(hour>12) {
					hour=hour-12;
					ampm.Copy(_L("pm"));
				}
				else{
					ampm.Copy(_L("am"));
				}

			tmptime.Format(KFormatTxt,
                   aDateTime.Day()+1,
                   TInt(aDateTime.Month()+1), 
                       // Format the month as a TInt to preserve locale independence
                   aDateTime.Year(), 
                       // Day and month ranges begin at zero (0-30 and 0-11), 
                       // so add one when formatting
                   hour, aDateTime.Minute(), aDateTime.Second());

			tmptime.Append(ampm);
		
		if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
			ImageDatePhonearray->InsertL(count - 1 - i, tmptime);
		else
			ImageDateMmcarray->InsertL(count - 1 - i, tmptime);			
		}
	}
	else//videos
	{
		iBitmap = new (ELeave) CFbsBitmap();

		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KVideoMbmFileName);

		iBitmap->Load(path);
				
		//for (TInt i=0;i<dirList->Count();i++)
		for (TInt i= count-1;i>=0;i--)
		{
			ico = CGulIcon::NewL(iBitmap, NULL);

			if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
			{
				iPhoneIconList->AppendL(ico);
			}
			else
			{				
				iMmcIconList->AppendL(ico);
			}

			tmpbuf.Copy((*dirList)[i].iName);
			TInt pos =tmpbuf.Find(_L("."));
			if(pos != -1)
				tmpbuf.Delete(pos, 5);
						
			TBuf<128> filepathName;

			if( iPhotosAppUi->ImageFolderPath() == 0)
			{
				filepathName.Copy(KFolderPhonePhoto);
			}
			else if( iPhotosAppUi->ImageFolderPath() == 1)
			{
				filepathName.Copy(KFolderMMCPhoto);
			}
			else if( iPhotosAppUi->ImageFolderPath() == 2)
			{
				filepathName.Copy(KFolderPhoneVideo);
			}
			else if( iPhotosAppUi->ImageFolderPath() == 3)
			{
				filepathName.Copy(KFolderMMCVideo);
			}

			filepathName.Append((*dirList)[i].iName);
			
			if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
			{
				ImageDataPhonearray->InsertL(count - 1 - i, tmpbuf);
				ImageDataPhoneFilearray->InsertL(count - 1 - i, filepathName);
			}
			else
			{
				ImageDataMmcarray->InsertL(count - 1 - i, tmpbuf);
				ImageDataMmcFilearray->InsertL(count - 1 - i, filepathName);
			}

			TBuf<50> tmptime;
			
			TTime time;
			time = (*dirList)[i].iModified;

			TDateTime aDateTime = time.DateTime();
			
			//dd/mm/yyyy hh:mm:ss am
			_LIT(KFormatTxt,"%d/%d/%d-%d:%d:%d ");
				
			TBuf<2> ampm;

			TInt hour = aDateTime.Hour();
				if(hour>12) {
					hour=hour-12;
					ampm.Copy(_L("pm"));
				}
				else{
					ampm.Copy(_L("am"));
				}

			tmptime.Format(KFormatTxt,
                   aDateTime.Day()+1,
                   TInt(aDateTime.Month()+1), 
                       // Format the month as a TInt to preserve locale independence
                   aDateTime.Year(), 
                       // Day and month ranges begin at zero (0-30 and 0-11), 
                       // so add one when formatting
                   hour, aDateTime.Minute(), aDateTime.Second());

			tmptime.Append(ampm);
		
			if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
				ImageDatePhonearray->InsertL(count - 1 - i, tmptime);
			else
				ImageDateMmcarray->InsertL(count - 1 - i, tmptime);
		}		
	}	
#else
	count = 6;
	CFbsBitmap *Bitmap;
	Bitmap = new (ELeave) CFbsBitmap();

	TBuf<128> path11;
	path11 = CRippleVaultAppUi::ApplicationDriveAndPath();
	path11.Append(KRippleImageMbmFileName);

	Bitmap->Load(path11);
	ico = CGulIcon::NewL(Bitmap, NULL);

	iPhoneIconList->AppendL(ico);
	iPhoneIconList->AppendL(ico);
	iPhoneIconList->AppendL(ico);
	iPhoneIconList->AppendL(ico);
	iPhoneIconList->AppendL(ico);
	iPhoneIconList->AppendL(ico);
	iPhoneIconList->AppendL(ico);
#endif	

	CleanupStack::Pop();
}

void CPhotoListContainer::SetFileList(TInt aIsmark)
 {
	//SetIconsArray();
	TBuf<300> buffer;

	if(aIsmark == 0)
	{
		// Set the listbox to use the the file list model
	    items = static_cast<CDesCArray*>(iListBox->Model()->ItemTextArray());
	}
	else
	{
		items->Reset();
		items = static_cast<CDesCArray*>(iListBox->Model()->ItemTextArray());
	}

    iListBox->HandleItemRemovalL();

	TInt count;
	if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
		count = iPhoneCount;
	else
		count = iMmcCount;

	for (TInt i=4;i<count+4;i++)
	{
		if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
			buffer.Append((*ImageDatePhonearray)[i-4]);
		else
			buffer.Append((*ImageDateMmcarray)[i-4]);

		TInt ret = 1;

		TBuf<150> filearray;

		if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
			filearray.Copy((*ImageDataPhoneFilearray)[i-4]);
		else
			filearray.Copy((*ImageDataMmcFilearray)[i-4]);

		//CAknMessageQueryDialog* verdlg1 = CAknMessageQueryDialog::NewL(filearray);	
		//verdlg1->PrepareLC(R_ABOUT_QUERY);
		//verdlg1->RunLD();
		
		ret = CheckMediaExists(filearray);

		if( aIsmark == 0)
		{
			//bufc.AppendNum(ret);
			//bufc.Append(_L("\t"));
			//bufc.Append(bufb1);

			if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
				buffer.Format(_L("%d\t%S\t%S\t%d\t%d"),i , &(*ImageDataPhonearray)[i-4], &(*ImageDatePhonearray)[i-4], 1, ret); 
			else
				buffer.Format(_L("%d\t%S\t%S\t%d\t%d"),i , &(*ImageDataMmcarray)[i-4], &(*ImageDateMmcarray)[i-4], 1, ret);
		}
		else
		{
			if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
			{
				if( iSelectedPhonePhotos->At(i-4) == ITEM_SELECTED)
				{
					//bufc.AppendNum(ret);
					//bufc.Append(_L("\t"));					
					//bufc.Append(bufb1);
					
					buffer.Format(_L("%d\t%S\t%S\t%d\t%d"),i , &(*ImageDataPhonearray)[i-4], &(*ImageDatePhonearray)[i-4],1, ret); 
				}
				else
				{
					//bufc.AppendNum(ret);
					//bufc.Append(_L("\t"));					
					//bufc.Append(bufb0);
					
					buffer.Format(_L("%d\t%S\t%S\t%d\t%d"),i , &(*ImageDataPhonearray)[i-4], &(*ImageDatePhonearray)[i-4], 0, ret); 
				}
			}
			else
			{
				if( iSelectedMmcPhotos->At(i-4) == ITEM_SELECTED)
				{
					//bufc.AppendNum(ret);
					//bufc.Append(_L("\t"));					
					//bufc.Append(bufb1);
					
					buffer.Format(_L("%d\t%S\t%S\t%d\t%d"),i , &(*ImageDataMmcarray)[i-4], &(*ImageDateMmcarray)[i-4], 1, ret);
				}
				else
				{
					//bufc.AppendNum(ret);
					//bufc.Append(_L("\t"));					
					//bufc.Append(bufb0);
					
					buffer.Format(_L("%d\t%S\t%S\t%d\t%d"),i , &(*ImageDataMmcarray)[i-4], &(*ImageDateMmcarray)[i-4], 0, ret);
				}
			}
		}
		
		//buf1.Format(bufc,i,&tfilename);

		items->AppendL(buffer);	
	}	
	
	iListBox->HandleItemAdditionL();
	iListBox->SetCurrentItemIndex(iCursorPos);
	iListBox->DrawNow();
}

TInt CPhotoListContainer::IsImageSelected()
{
	TInt Ret=0;
	
	for(TInt i = 0; i < iPhoneCount; i++)
	{
		if(	iSelectedPhonePhotos->At(i) != ITEM_SELECTED)
		{
			Ret = 1;
			break;
		}
	}

	if(Ret == 0)
	{
		for(TInt i = 0; i < iMmcCount; i++)
		{
			if(	iSelectedMmcPhotos->At(i) != ITEM_SELECTED)
			{
				Ret = 1;
				break;
			}
		}
	}

	return Ret;
}

void CPhotoListContainer::MarkSelectedPhoto()
{
	if  (iListBox)
	{
	// Cursor's current position
	TInt itemIndex = iListBox->CurrentItemIndex();			

	if (itemIndex >= 0) 
		{
		CTextListBoxModel* model = iListBox->Model();  // not taking ownership
		model->SetOwnershipType (ELbmOwnsItemArray);
		CDesCArray* photoArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
		
		TBuf <512> changeItem( (*photoArray)[itemIndex] );
		
		TInt len = changeItem.Length();
		//TInt pos = changeItem.Mid(changeItem.Locate('\t') + 1).Locate('\t');
		//TInt pos = changeItem.Mid(changeItem.Mid(changeItem.Locate('\t') + 1).Locate('\t') + 1).Locate('\t');
		TInt tabPosition1 = changeItem.Locate('\t');
		TInt tabPosition2 = changeItem.Mid(tabPosition1 + 1).Locate('\t');
		tabPosition2 += tabPosition1 + 1;
		TInt pos = changeItem.Mid(tabPosition2 + 1).Locate('\t');
		pos += tabPosition2 + 1;

		TBuf<2> text;
		text.Copy(_L(""));
		text.Append(changeItem.Mid(pos + 1,1));
		
		//CEikonEnv::Static ()->AlertWin(text);

		// Compare index to see which is the case.		
		if ( text.Right(1) == _L("1") ) // Deselected -> Selected		
			{
				changeItem.Replace(pos+1, 1, _L("0"));

				if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
				{
					iSelectedPhonePhotos->Delete(itemIndex);
					iSelectedPhonePhotos->InsertL(itemIndex, itemIndex);
				}
				else
				{
					iSelectedMmcPhotos->Delete(itemIndex);
					iSelectedMmcPhotos->InsertL(itemIndex, itemIndex);
				}
			}		
		else if ( text.Right(1) == _L("0") ) // Selected -> Deselected
			{
				changeItem.Replace(pos+1, 1, _L("1"));

				if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
				{
					iSelectedPhonePhotos->Delete(itemIndex);
					iSelectedPhonePhotos->InsertL(itemIndex, ITEM_SELECTED);
				}
				else
				{
					iSelectedMmcPhotos->Delete(itemIndex);
					iSelectedMmcPhotos->InsertL(itemIndex, ITEM_SELECTED);
				}
			}
			
		photoArray->Delete(itemIndex);
		photoArray->InsertL(itemIndex, changeItem);

		iListBox->HandleItemAdditionL();
		}
	}
}

void CPhotoListContainer::MarkSelectedItem()
{
	if  (iListBox)
	{
	// Cursor's current position
	TInt itemIndex = iListBox->CurrentItemIndex();			

	if (itemIndex >= 0) 
		{
		CTextListBoxModel* model = iListBox->Model();  // not taking ownership
		model->SetOwnershipType (ELbmOwnsItemArray);
		CDesCArray* photoArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
		
		TBuf <512> changeItem( (*photoArray)[itemIndex] );
		
		TInt len = changeItem.Length();
		//TInt pos = changeItem.Mid(changeItem.Locate('\t') + 1).Locate('\t');
		//TInt pos = changeItem.Mid(changeItem.Mid(changeItem.Locate('\t') + 1).Locate('\t') + 1).Locate('\t');
		TInt tabPosition1 = changeItem.Locate('\t');
		TInt tabPosition2 = changeItem.Mid(tabPosition1 + 1).Locate('\t');
		tabPosition2 += tabPosition1 + 1;
		TInt pos = changeItem.Mid(tabPosition2 + 1).Locate('\t');
		pos += tabPosition2 + 1;

		TBuf<2> text;
		text.Copy(_L(""));
		text.Append(changeItem.Mid(pos + 1,1));
		
		//CEikonEnv::Static ()->AlertWin(text);

		// Compare index to see which is the case.		
		if ( text.Right(1) == _L("1") ) // Deselected -> Selected		
			{
				changeItem.Replace(pos+1, 1, _L("0"));

				if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
				{
					iSelectedPhonePhotos->Delete(itemIndex);
					iSelectedPhonePhotos->InsertL(itemIndex, itemIndex);
				}
				else
				{
					iSelectedMmcPhotos->Delete(itemIndex);
					iSelectedMmcPhotos->InsertL(itemIndex, itemIndex);
				}
			}
			
		photoArray->Delete(itemIndex);
		photoArray->InsertL(itemIndex, changeItem);

		iListBox->HandleItemAdditionL();
		}
	}
}

void CPhotoListContainer::UnMarkSelectedPhoto()
{
	TInt count = iListBox->CurrentItemIndex();
	
	if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
	{
		iSelectedPhonePhotos->Delete(count);
		iSelectedPhonePhotos->InsertL(count, ITEM_SELECTED);
	}
	else
	{
		iSelectedMmcPhotos->Delete(count);
		iSelectedMmcPhotos->InsertL(count, ITEM_SELECTED);
	}

	iCursorPos = count;
	
	SetFileList(1);
	
	iListBox->HandleItemAdditionL();
	iListBox->SetCurrentItemIndex(iCursorPos);
	iListBox->DrawNow();
}

void CPhotoListContainer::MarkAllItems()
{	
	TInt count;
	if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
		count = iPhoneCount;
	else
		count = iMmcCount;

	for(TInt i = 0; i < count; i++)
	{
		if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
		{
			iSelectedPhonePhotos->Delete(i);
			iSelectedPhonePhotos->InsertL(i, i);
		}
		else
		{
			iSelectedMmcPhotos->Delete(i);
			iSelectedMmcPhotos->InsertL(i, i);
		}
	}

	iCursorPos = 0;
	
	SetFileList(1);
	
	iListBox->HandleItemAdditionL();
	iListBox->SetCurrentItemIndex(iCursorPos);
	iListBox->DrawNow();
}

void CPhotoListContainer::UnMarkAllItems()
{
	TInt count;
	if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
		count = iPhoneCount;
	else
		count = iMmcCount;

	for(TInt i = 0; i < count; i++)
	{
		if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
		{	
			iSelectedPhonePhotos->Delete(i);
			iSelectedPhonePhotos->InsertL(i, ITEM_SELECTED);
		}
		else
		{
			iSelectedMmcPhotos->Delete(i);
			iSelectedMmcPhotos->InsertL(i, ITEM_SELECTED);
		}
	}

	iCursorPos = 0;

	SetFileList(1);

	iListBox->HandleItemAdditionL();
	iListBox->SetCurrentItemIndex(iCursorPos);
	iListBox->DrawNow();
}

void CPhotoListContainer::MarkUnMarkSelectedItem()
{
	TInt count = iListBox->CurrentItemIndex();

	if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
	{
		if(iSelectedPhonePhotos->At(count) != ITEM_SELECTED)
		{
			iIsMark = EFalse;
		}
		else
		{
			iIsMark = ETrue;
		}
	}
	else
	{
		if(iSelectedMmcPhotos->At(count) != ITEM_SELECTED)
		{
			iIsMark = EFalse;
		}
		else
		{
			iIsMark = ETrue;
		}
	}

	if(iIsMark)
		MarkSelectedPhoto();
	else
		MarkSelectedPhoto();
}

void CPhotoListContainer::DrawStausPane()
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

	iNaviPane = static_cast<CAknNavigationControlContainer*>(statusPane->ControlL(TUid::Uid(EEikStatusPaneUidNavi))); 

	iNaviDeco = iNaviPane->CreateTabGroupL(); 
	iNaviPane->PushL(static_cast<CAknNavigationDecorator &>(*iNaviDeco)); 
	
	iTabGroup = static_cast<CAknTabGroup*> (iNaviDeco->DecoratedControl()); 

	iTabGroup->AddTabL(  0, *(iEikonEnv->AllocReadResourceL(R_TAB_PHONE)) ); 

	iTabGroup->AddTabL(  1, *(iEikonEnv->AllocReadResourceL(R_TAB_MEMORY)) ); 

	if( iPhotosAppUi->iStoragePath == PHOTOS_PHONEMEMORY || iPhotosAppUi->iStoragePath == VIDEOS_PHONEMEMORY )
		iTabGroup->SetActiveTabById(0); 
	else 
		iTabGroup->SetActiveTabById(1); 
}

void CPhotoListContainer::SetIconsArray()
{
	CFormattedCellListBoxData* colData = iListBox->ItemDrawer()->ColumnData();

	if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
	{	
		colData->SetIconArray(iPhoneIconList);
	}
	else
	{				
		colData->SetIconArray(iMmcIconList);
	}

	colData->SetGraphicsSubCellL(0, ETrue);
	
	colData->SetSubCellSizeL(0,TSize(35, 65) );
	colData->SetSubCellAlignmentL(0, CGraphicsContext::ECenter);

	colData->SetGraphicsSubCellL(1, EFalse);

	colData->SetSubCellSizeL(1,TSize(35, 80));
	
	colData->SetGraphicsSubCellL(2, EFalse);

	colData->SetSubCellSizeL(2,TSize(35, 50));
	colData->SetSubCellAlignmentL(2, CGraphicsContext::ELeft);
}

void CPhotoListContainer::GetPhoneImage()
{
	TBuf<50> pathName;
	TBuf<50> pathName1;
	TInt path;
	TInt count;
	
	path = iPhotosAppUi->ImageFolderPath();

	if( path == 0)
	{
		pathName.Copy(KFilesPhonePhoto);
		pathName1.Copy(KFolderPhonePhoto);
	}
	else if( path == 2)
	{
		pathName.Copy(KFilesPhoneVideo);
		pathName1.Copy(KFolderPhoneVideo);
	}

	if (BaflUtils::FolderExists(iRFsession, pathName1))
	{
		iWaitDialog = new(ELeave)CAknWaitDialog( (REINTERPRET_CAST(CEikDialog**,&iWaitDialog)));
		iWaitDialog->SetTone( CAknNoteDialog::EConfirmationTone ); 
		iWaitDialog->SetTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SCANNING_PROGRESS)));
		iWaitDialog->ExecuteLD(R_WAITNOTE_SCAN_DIALOG);
		
		TInt err = iRFsession.GetDir(pathName, KEntryAttNormal, ESortByDate, dirList);
		CleanupStack::PushL(dirList);
		
		if (err != KErrNone)
		{
			return;
		}
		
		count = dirList->Count();

		if(count > 0)
		{
			ImageDataPhonearray = new (ELeave) CDesCArrayFlat(count);
			ImageDatePhonearray = new (ELeave) CDesCArrayFlat(count);
			ImageDataPhoneFilearray = new (ELeave) CDesCArrayFlat(count);
			iSelectedPhonePhotos->Reset();
			iSelectedPhonePhotos = new CArrayFixFlat<TInt>(count);
		}
		else
		{
			ImageDataPhonearray = new (ELeave) CDesCArrayFlat(1);
			ImageDatePhonearray = new (ELeave) CDesCArrayFlat(1);
			ImageDataPhoneFilearray = new (ELeave) CDesCArrayFlat(1);
			iSelectedPhonePhotos->Reset();
			iSelectedPhonePhotos = new CArrayFixFlat<TInt>(1);
		}
		
		for(TInt i = 0; i < count; i++)
		{
			iSelectedPhonePhotos->InsertL(i, ITEM_SELECTED);
		}
		
		iPhoneCount = count;

		if(count != 0)
		{
			AppendThumnails();
		}

		CleanupStack::PopAndDestroy();

		if(iWaitDialog != NULL)
			iWaitDialog->ProcessFinishedL();
	}	

	SetIconsArray();
}

void CPhotoListContainer::GetMmcImage()
{
	TBuf<50> pathName;
	TBuf<50> pathName1;
	TInt path;
	TInt count;
	
	path = iPhotosAppUi->ImageFolderPath();

	if( path == 1)
	{
		pathName.Copy(KFilesMMCPhoto);
		pathName1.Copy(KFolderMMCPhoto);
	}
	else if( path == 3)
	{
		pathName.Copy(KFilesMMCVideo);
		pathName1.Copy(KFolderMMCVideo);
	}

	if (BaflUtils::FolderExists(iRFsession, pathName1))
	{
		iWaitDialog = new(ELeave)CAknWaitDialog( (REINTERPRET_CAST(CEikDialog**,&iWaitDialog)));
		iWaitDialog->SetTone( CAknNoteDialog::EConfirmationTone ); 
		iWaitDialog->SetTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SCANNING_PROGRESS)));
		iWaitDialog->ExecuteLD(R_WAITNOTE_SCAN_DIALOG);
		
		TInt err = iRFsession.GetDir(pathName, KEntryAttNormal, ESortByDate, dirList);
		CleanupStack::PushL(dirList);
		
		if (err != KErrNone)
		{
			return;
		}
		
		count = dirList->Count();

		if(count > 0)
		{
			ImageDataMmcarray = new (ELeave) CDesCArrayFlat(count);
			ImageDateMmcarray = new (ELeave) CDesCArrayFlat(count);
			ImageDataMmcFilearray = new (ELeave) CDesCArrayFlat(count);
			iSelectedMmcPhotos->Reset();
			iSelectedMmcPhotos = new CArrayFixFlat<TInt>(count);
		}
		else
		{
			ImageDataMmcarray = new (ELeave) CDesCArrayFlat(1);
			ImageDateMmcarray = new (ELeave) CDesCArrayFlat(1);
			ImageDataMmcFilearray = new (ELeave) CDesCArrayFlat(1);
			iSelectedMmcPhotos->Reset();
			iSelectedMmcPhotos = new CArrayFixFlat<TInt>(1);
		}
		
		for(TInt i = 0; i < count; i++)
		{
			iSelectedMmcPhotos->InsertL(i, ITEM_SELECTED);
		}
		
		iMmcCount = count;

		if(count != 0)
		{
			AppendThumnails();
		}

		CleanupStack::PopAndDestroy();

		if(iWaitDialog != NULL)
			iWaitDialog->ProcessFinishedL();
	}
}

TInt CPhotoListContainer::CheckMediaExists(TDesC& aFilename)
{
	return iPhotosAppUi->CheckMediaUploadExists(aFilename);
}
// End of File  
