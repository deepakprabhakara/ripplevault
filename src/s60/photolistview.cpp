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
 Module Name				: PhotoListView.cpp
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
 
//INCLUDE FILES
#include  <aknlistquerydialog.h> 
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <RippleVault.rsg>
#include  <aknmessagequerydialog.h>
#include  "PhotoListView.h"
#include  "PhotoListContainer.h" 
#include  "RippleVault.hrh"
#include  <bautils.h>

#include "constants.h"
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPhotoListView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------

void CPhotoListView::ConstructL()
    {
		BaseConstructL( R_PHOTOLIST_VIEW );
    }

// ---------------------------------------------------------
// CPhotoListView::~CPhotoListView()
// ?implementation_description
// ---------------------------------------------------------

CPhotoListView::~CPhotoListView()
    {
		if ( iContainer )
			{
			AppUi()->RemoveFromViewStack( *this, iContainer );
			}

		delete iContainer;
    }

// ---------------------------------------------------------
// TUid CPhotoListView::Id()
// ?implementation_description
// ---------------------------------------------------------

TUid CPhotoListView::Id() const
    {
    return KViewIdPhotoList;
    }

// ---------------------------------------------------------
// CPhotoListView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------

void CPhotoListView::HandleCommandL(TInt aCommand)
    {   
	TInt path;
	TInt count;

    switch ( aCommand )
        {
		case EMarkSel:
			{
				TInt count;
				if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
					count = iContainer->iPhoneCount;
				else
					count = iContainer->iMmcCount;

				if(count > 0)
					iContainer->MarkUnMarkSelectedItem();
			}
			break;
		case EMarkAll:
			{
				TInt count;
				if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
					count = iContainer->iPhoneCount;
				else
					count = iContainer->iMmcCount;

				if(count > 0)
					iContainer->MarkAllItems();
			}
			break;
		case EUnMarkAll:
			{
				TInt count;
				if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
					count = iContainer->iPhoneCount;
				else
					count = iContainer->iMmcCount;

				if(count > 0)
					iContainer->UnMarkAllItems();
			}
			break;
        //case EAknSoftkeyDone:
		case ESendSel:
            {
				TInt count;
				if( iPhotosAppUi->ImageFolderPath() == 0 || iPhotosAppUi->ImageFolderPath() == 2)
					count = iContainer->iPhoneCount;
				else
					count = iContainer->iMmcCount;

				if(count > 0)
					iContainer->MarkSelectedItem();

				iPhotoSendMode = iContainer->IsImageSelected();
				if(iPhotoSendMode == 1)
				{
					TBuf<150> msg;
					TInt path=0;	
					
					count = GetPhotoCount();

					msg.Copy(_L(""));

					msg.AppendNum(count);

					path = iPhotosAppUi->ImageFolderPath();
					if(path == 0 || path == 1)			
						//msg.Append(_L(" Photos"));
						msg.Append(*(iEikonEnv->AllocReadResourceL(R_IMAGES_CONFIRM_PHOTOS)));
					else if(path == 2 || path == 3)
						//msg.Append(_L(" Videos"));		
						msg.Append(*(iEikonEnv->AllocReadResourceL(R_IMAGES_CONFIRM_VIDEOS)));

					//msg.Append(_L(" will be sent to your Ripple Vault. Confirm?"));
					msg.Append(*(iEikonEnv->AllocReadResourceL(R_IMAGES_CONFIRM)));

					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(msg);
					dlg->PrepareLC(R_UPLOAD_ALLPHOTOS);
					if(dlg->RunLD())
					{
						SendPhotoToServer();
						
						iPhotosAppUi->ActivateView(KViewIdStatusScreen);
						
						/*
						TInt8 choice;
						iPhotosAppUi->GetChoice(choice);
						if(choice){
							//iPhotosAppUi->ActivateView(KViewIdSubScreen);
							iPhotosAppUi->ActivateView(KViewIdEmptyScreen);
							iPhotosAppUi->ActivateView(KViewIdStatusScreen);
						}
						else
						{
							if(iPhotosAppUi->iStoragePath==PHOTOS_PHONEMEMORY || iPhotosAppUi->iStoragePath==PHOTOS_MMC){
								iPhotosAppUi->iStoragePath = VIDEOS_PHONEMEMORY;									
								iPhotosAppUi->ActivateView(KViewIdEmptyScreen );
								iPhotosAppUi->ActivateView(KViewIdPhotoList );
							}
							else{
								iPhotosAppUi->CallSetupWizardStep12();
							}
						}
						*/
					}
				}
				else
				{					
						path = iPhotosAppUi->ImageFolderPath();

						CAknMessageQueryDialog* abtdlg;
						
						//abtdlg= CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_HELP_RIPPLE_VAULT)));
						//abtdlg->SetHeaderTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)));

					if(path == 0 || path == 1)
						//iPhotosAppUi->ShowMessageBoxFromResource(R_EXAMPLE_TEXT_NOTSEL);
						abtdlg= CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_TEXT_NOTSEL)));
					else if(path == 2 || path == 3)
						//iPhotosAppUi->ShowMessageBoxFromResource(R_EXAMPLE_TEXT_NOTSEL1);
						abtdlg= CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_TEXT_NOTSEL1)));

							abtdlg->PrepareLC(R_UPGRADE_QUERY);

							if (abtdlg->RunLD()) 
							{
							}
							else
							{

								iPhotosAppUi->ActivateView(KViewIdStatusScreen);
								/*
								TInt8 choice;
								iPhotosAppUi->GetChoice(choice);
								if(choice)
									iPhotosAppUi->ActivateView(KViewIdSubScreen );
								else
								{
									if(iPhotosAppUi->iStoragePath==PHOTOS_PHONEMEMORY || iPhotosAppUi->iStoragePath==PHOTOS_MMC){
										iPhotosAppUi->iStoragePath = VIDEOS_PHONEMEMORY;
										iPhotosAppUi->ActivateView(KViewIdEmptyScreen );
										iPhotosAppUi->ActivateView(KViewIdPhotoList );
									}
									else{
										iPhotosAppUi->CallSetupWizardStep12();
									}
								}*/
							}

				}
				break;
            }
		case ESendAll:
			{
				iPhotoSendMode = 1000;
				SendPhotoToServer();
				break;
			}
		//case ESendSel:
		case EAknSoftkeyBack:
            {
				if(iContainer->IsImageSelected())
				{
					CAknMessageQueryDialog* abtdlg;
					abtdlg= CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_CANCEL_BACKUP)));
					abtdlg->PrepareLC(R_GALLERY_INFO);
					if (abtdlg->RunLD()) 
					{
						iPhotosAppUi->ActivateView(KViewIdStatusScreen);
					}
				}
				else
					iPhotosAppUi->ActivateView(KViewIdStatusScreen);
				
				/*
				TInt8 choice;
				iPhotosAppUi->GetChoice(choice);
				if(choice)
					iPhotosAppUi->ActivateView(KViewIdSubScreen );
				else
				{
					if(iPhotosAppUi->iStoragePath==PHOTOS_PHONEMEMORY || iPhotosAppUi->iStoragePath==PHOTOS_MMC){
						iPhotosAppUi->iStoragePath = VIDEOS_PHONEMEMORY;									
						iPhotosAppUi->ActivateView(KViewIdEmptyScreen );
						iPhotosAppUi->ActivateView(KViewIdPhotoList );
					}
					else{
						iPhotosAppUi->CallSetupWizardStep12();
					}
				}
				*/

				break;
            }
        default:
            {       
			AppUi()->HandleCommandL(aCommand);
            break;
            }
        }
    }

// ---------------------------------------------------------
// CPhotoListView::HandleClientRectChange()
// ---------------------------------------------------------

void CPhotoListView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CPhotoListView::DoActivateL(...)
// ?implementation_description
// ---------------------------------------------------------

void CPhotoListView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CPhotoListContainer;
        iContainer->SetMopParent(this);
		iContainer->iPhotosAppUi=iPhotosAppUi;
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );		
        }
   }

// ---------------------------------------------------------
// CPhotoListView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------

void CPhotoListView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }

void CPhotoListView::SendPhotoToServer()
{
//	int answer=0;
	TInt8 intrusionmode;
	TInt Index;	
	TInt ret=1;	
	
	iFolderStatus=-2;
	iNewFolder = 0;
	//iPhotosAppUi->BringToForeground();
	User::LeaveIfError(iRFsession.Connect());

#ifdef __LOGME__
	iPhotosAppUi->LogText.Copy(_L("SendPhotoToServer 1\n"));
	iPhotosAppUi->WriteLogFile(iPhotosAppUi->LogText);
#endif

	intrusionmode = iPhotosAppUi->GetMode();
	
	if(iPhotosAppUi->iRipplePhotos == 1)
	{
		iFolderStatus=0;
		//comment.Copy(_L("Backup"));		
		comment.Copy(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));
		ReadandSendData();		
	}
	else if(intrusionmode==MODE_PROMPT || intrusionmode==MODE_AUTO || intrusionmode==2)
	{
		//CAknQueryDialog* dlg = CAknQueryDialog::NewL(CAknQueryDialog::ENoTone );
		//TInt ret;
		//ret=( dlg->ExecuteLD( R_SMART_CHARGE ) );
		
		/*TBuf<150> msg;

		msg.Copy(_L("Do you want to Publish this "));

		path = iPhotosAppUi->ImageFolderPath();
		if(path == 0 || path == 1)			
			msg.Append(_L("Photos"));
		else if(path == 2 || path == 3)
			msg.Append(_L("Videos"));

		msg.Append(_L("? You would be charged "));		
		msg.Append(temp);
		msg.Append(_L(" pesos."));

		CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(msg);
		dlg->PrepareLC(R_SMART_CHARGE);
		
		ret = dlg->RunLD();*/

		if(ret)
		{				
			iNewFolder = 0;
			iPublic_Photo = EFalse;			

			if(intrusionmode==MODE_PROMPT || intrusionmode==2)
			{		
				GetFolder();
				// if iFolderStatus == -1, no folder is selected
				// i.e. dont want to send
				if (iFolderStatus != -1) // want to upload?
				{
					if(iFolderStatus==Count) 
						{
							iFolderStatus=-2;
							Index=0;
							iFolderStatus=0;
							iPublic_Photo=EFalse;
							StoreFolder();
						}
						else
						{
							iFolderStatus=0;
							//ChangeStatus(EConnecting);
							  ReadandSendData();
						}
					}
					else // no folder selected
					{
						DataWritten();
					}
				}
				else if(intrusionmode==MODE_AUTO)// non-intrusive mode
				{	
					Count=-1;	
					iFolderStatus=0;
					comment.Copy(iPhotosAppUi->GetDefaultFolder());
					ReadandSendData();
				}
				iRFsession.Close();
		}
	}
#ifdef __LOGME__
	iPhotosAppUi->LogText.Copy(_L("SendPhotoToServer 2\n"));
	iPhotosAppUi->WriteLogFile(iPhotosAppUi->LogText);
#endif

}

void CPhotoListView::StoreFolder()
{
	int answer;
	TInt Index;
	if(iFolderStatus>=0)
	{
		TInt compareflag;
	    TBuf<160> folderName;
	
	    folderName.FillZ();
		folderName.Zero();
		folderName.SetLength(0);
		CAknTextQueryDialog* dlg = CAknTextQueryDialog::NewL(folderName, CAknQueryDialog::ENoTone );
		answer=( dlg->ExecuteLD( R_FOLDER_NAME_QUERY ) ); // new folder name
		if(answer)
		{
			iFolderStatus=0;			
			
			compareflag=iPhotosAppUi->CheckAndSaveFolderToDB(folderName);
	
			if(compareflag!=1)
			{
				comment.Copy(folderName);
				iNewFolder = 1;
				ReadandSendData();
			}
			else
			{
				CDesCArray* folderexist = iPhotosAppUi->FolderExist();
				//CEikonEnv::Static ()->AlertWin (folderexist->MdcaPoint(0)) ;
				TBuf<100> msg;
				msg.Copy(folderexist->MdcaPoint(0));
				CAknMessageQueryDialog* verdlg = CAknMessageQueryDialog::NewL(msg);	
				verdlg->PrepareLC(R_ABOUT_QUERY);
				verdlg->RunLD();				
				folderexist->Reset();
			}
		}
		else
		{
			Index=0;				
			iFolderStatus=0;
			iPublic_Photo=EFalse;

		}
	}
}	

void CPhotoListView::ReadandSendData()
{
	TBuf<50> pathName;
	TBuf<50> pathName1;
	TBuf<50> pathName2;
	TBuf<50> pathName3;
	TBuf<150> desc;
	TInt path;
	
	path = iPhotosAppUi->ImageFolderPath();

	if( path == 0 || path == 1)
	{
		pathName.Copy(KFilesPhonePhoto);
		pathName1.Copy(KFilesMMCPhoto);
		pathName2.Copy(KFolderPhonePhoto);
		pathName3.Copy(KFolderMMCPhoto);
	}
	else if( path == 2 || path == 3)
	{
		pathName.Copy(KFilesPhoneVideo);
		pathName1.Copy(KFilesMMCVideo);
		pathName2.Copy(KFolderPhoneVideo);
		pathName3.Copy(KFolderMMCVideo);
	}

	if(iFolderStatus>=0)
	{
		if (BaflUtils::FolderExists(iRFsession, pathName2))
		{
			CDir *dirList;
			// get the dirs contents
			TInt err;
			err = iRFsession.GetDir(pathName, KEntryAttNormal, ESortByDate, dirList);
			CleanupStack::PushL(dirList);
			if (err != KErrNone)		// if error, just return, dir must not exist
				return;			
			TParse fullentry;
		
			//////
			TBuf<160> imageFile;
			if(iPhotoSendMode == 1000)
			{
				//for (TInt PhotoNo =0; PhotoNo< dirList->Count(); PhotoNo++)
				for (TInt PhotoNo = dirList->Count()-1; PhotoNo>=0; PhotoNo--)
				{
					fullentry.Set((*dirList)[PhotoNo].iName, &pathName, NULL);
					imageFile.Copy(fullentry.FullName());
					iPublic_Photo=EFalse;
					if( path == 0 || path ==1)
						iPhotosAppUi->AddPhotoRecord(imageFile, iPublic_Photo, comment, desc, iNewFolder, (*dirList)[PhotoNo].iSize);
					else if( path == 2 || path ==3)
						iPhotosAppUi->AddVideoRecord(imageFile, iPublic_Photo, comment, desc, iNewFolder, (*dirList)[PhotoNo].iSize);
				}
			}
			else
			{
				TInt count;
				count = dirList->Count()-1;
				for (TInt PhotoNo =0; PhotoNo< dirList->Count(); PhotoNo++)
				{
					if(iContainer->iSelectedPhonePhotos->At(PhotoNo) != ITEM_SELECTED)
					{

						fullentry.Set((*dirList)[count - PhotoNo].iName, &pathName, NULL);
						imageFile.Copy(fullentry.FullName());
						iPublic_Photo=EFalse;
						if( path == 0 || path ==1)
						{
							iPhotosAppUi->AddPhotoRecord(imageFile, iPublic_Photo, comment, desc, iNewFolder, (*dirList)[count - PhotoNo].iSize);
						}
						else if( path == 2 || path ==3)
							iPhotosAppUi->AddVideoRecord(imageFile, iPublic_Photo, comment, desc, iNewFolder, (*dirList)[count - PhotoNo].iSize);

					//iPhotosAppUi->LogText.Copy(_L("\nFile="));
					//iPhotosAppUi->LogText.Append(imageFile);
					//iPhotosAppUi->WriteLogFile(iPhotosAppUi->LogText);
					}
				}
			}			
			
			CleanupStack::PopAndDestroy();// delete dirList
		}
		
		if(iContainer->iFlag == 1)
		if (BaflUtils::FolderExists(iRFsession, pathName3))
		{
			CDir *dirList1;
			
			TInt err;
			err = iRFsession.GetDir(pathName1, KEntryAttNormal, ESortByDate, dirList1);
			CleanupStack::PushL(dirList1);
			
			if (err != KErrNone)		// if error, just return, dir must not exist
				return;			
			
			TParse fullentry1;
		
			//////
			TBuf<160> imageFile1;
			if(iPhotoSendMode == 1000)
			{
				//for (TInt PhotoNo =0; PhotoNo< dirList->Count(); PhotoNo++)
				for (TInt PhotoNo1 = dirList1->Count()-1; PhotoNo1>=0; PhotoNo1--)
				{
					fullentry1.Set((*dirList1)[PhotoNo1].iName, &pathName1, NULL);
					imageFile1.Copy(fullentry1.FullName());
					iPublic_Photo=EFalse;
					if( path == 0 || path ==1)
						iPhotosAppUi->AddPhotoRecord(imageFile1, iPublic_Photo, comment, desc, iNewFolder, (*dirList1)[PhotoNo1].iSize);
					else if( path == 2 || path ==3)
						iPhotosAppUi->AddVideoRecord(imageFile1, iPublic_Photo, comment, desc, iNewFolder, (*dirList1)[PhotoNo1].iSize);
				}
			}
			else
			{
				TInt count;
				count = dirList1->Count()-1;
				for (TInt PhotoNo1 =0; PhotoNo1< dirList1->Count(); PhotoNo1++)
				{
					if(iContainer->iSelectedMmcPhotos->At(PhotoNo1) != ITEM_SELECTED)
					{

						fullentry1.Set((*dirList1)[count - PhotoNo1].iName, &pathName1, NULL);
						imageFile1.Copy(fullentry1.FullName());
						iPublic_Photo=EFalse;
						if( path == 0 || path ==1)
						{
							iPhotosAppUi->AddPhotoRecord(imageFile1, iPublic_Photo, comment, desc, iNewFolder, (*dirList1)[count - PhotoNo1].iSize);
						}
						else if( path == 2 || path ==3)
							iPhotosAppUi->AddVideoRecord(imageFile1, iPublic_Photo, comment, desc, iNewFolder, (*dirList1)[count - PhotoNo1].iSize);
					
						//iPhotosAppUi->LogText.Copy(_L("\nFile="));
						//iPhotosAppUi->LogText.Append(imageFile1);
						//iPhotosAppUi->WriteLogFile(iPhotosAppUi->LogText);

					}
				}
			}			
			
			CleanupStack::PopAndDestroy();// delete dirList
		}
	
		iPhotosAppUi->ScheduleToSend();

		DataWritten();		
	}
}

void CPhotoListView::DataWritten()
{
}

void CPhotoListView::GetFolder()
{
	TInt PopValue;	
	//TBuf<150> msg;
	TInt count=0;	
	
	CDesCArray* items = iPhotosAppUi->GetFolderList();	
    CDesCArray* foldernames = iPhotosAppUi->ReturnText();

	if (items == NULL)
	{
		Count = 0;
		items = new (ELeave) CDesCArrayFlat(1);
		items->AppendL(foldernames->MdcaPoint(0));
	}
	else
	{
		Count = items->MdcaCount();	
		items->AppendL(foldernames->MdcaPoint(0));
	}
	foldernames->Reset();

	CEikTextListBox* list = new( ELeave ) CAknSinglePopupMenuStyleListBox;
    CleanupStack::PushL( list );

    CAknPopupList* popupList = CAknPopupList::NewL(
       list,
       R_AVKON_SOFTKEYS_SELECT_CANCEL,
       AknPopupLayouts::EMenuWindow);

    CleanupStack::PushL( popupList );
    
    popupList->SetMaximumHeight(2);

    list->ConstructL( popupList, CEikListBox::ELeftDownInViewRect );
    list->CreateScrollBarFrameL( ETrue );
    list->ScrollBarFrame()->SetScrollBarVisibilityL(
		   CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );

    CTextListBoxModel* model = list->Model();
    model->SetItemTextArray( items );
    model->SetOwnershipType( ELbmOwnsItemArray );
	
	count = GetPhotoCount();

//	TInt price;
//	TLex lex(iPhotosAppUi->iCharge);
//	lex.Val(price);
//	count = count * price;

//	TBuf<30> temp;
//	temp.AppendNum(count);	

//	msg.Copy(_L("Charge "));
//	msg.Append(temp);
//	msg.Append(_L(" pesos"));

	//msg.Copy(_L("Select Album:"));
	//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_IMAGES_SELECT_ALBUM)));
	
	/*count = count / 3;
	temp.Copy(_L(""));
	temp.AppendNum(count);

	path = iPhotosAppUi->ImageFolderPath();
	if(path == 0 || path == 1)			
		msg.Append(_L(" Photos)"));
	else if(path == 2 || path == 3)
		msg.Append(_L(" Videos)"));*/

	popupList->SetTitleL(*(iEikonEnv->AllocReadResourceL(R_IMAGES_SELECT_ALBUM)));

	PopValue = popupList->ExecuteLD();		
	
	if(PopValue==0)
	{
		iFolderStatus=-1;
	}
	else if(PopValue==1)
	{
		PopValue=list->CurrentItemIndex();		
		iFolderStatus=PopValue;		
		TPtrC ptr1(items->MdcaPoint(PopValue));
		comment.Copy(ptr1);		
	}
	
	items->Reset();
	CleanupStack::Pop(); // popupList
    CleanupStack::PopAndDestroy(); // list	

}

TInt CPhotoListView::GetPhotoCount()
{
	TInt count=0;
	TInt PhotoNo;
	
	for (PhotoNo =0; PhotoNo< iContainer->iSelectedPhonePhotos->Count(); PhotoNo++)
	{
		if(iContainer->iSelectedPhonePhotos->At(PhotoNo) != ITEM_SELECTED)
			count++;
	}

	for (PhotoNo =0; PhotoNo< iContainer->iSelectedMmcPhotos->Count(); PhotoNo++)
	{
		if(iContainer->iSelectedMmcPhotos->At(PhotoNo) != ITEM_SELECTED)
			count++;
	}

	return count;
}
// End of File 