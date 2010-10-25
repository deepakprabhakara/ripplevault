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
 Module Name				: MainSubScreenListContainer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     To Show sub-screen list for S60
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/


// INCLUDE FILES
#include "MainSubScreenListContainer.h"
#include "RippleVaultAppUi.h"
#include <akntitle.h>
#include "subicons.mbg"
#include  <ripplevault.rsg>
#include "icons.mbg"

#include <akncontext.h>
#include "rippleimage.mbg"
#include <aknnotewrappers.h> 
#include  "MailIdListForm.h"

#include <aknnavi.h>

#include <aknmessagequerydialog.h>

const TInt KAknExListGraphicGulIconIndex = 10;

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CMainSubScreenListContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------

void CMainSubScreenListContainer::ConstructL(const TRect& aRect)
{	
	CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();

	//////////////////////////////////

	TBuf<50> message;
	//TBuf<50> formatBuf;
	
/*	if(iPhotosAppUi->iMainScreenValue == 11){//contacts
		TInt ccount;
		iPhotosAppUi->GetContactsUploadSettingsFromDB(ccount);

		message.Copy(_L(""));
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_CONTACTS_UPLOADED)));
		message.Format(formatBuf, ccount);
	}
	else if(iPhotosAppUi->iMainScreenValue == 12){//photos
		TInt photos;
		TInt videos;
		TInt album;
		TInt albumshares;
		iPhotosAppUi->GetUploadSettingsFromDB(album, photos, videos, albumshares);

		message.Copy(_L(""));
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_PHOTOS_UPLOADED)));
		message.Format(formatBuf, photos);

	}
	else if(iPhotosAppUi->iMainScreenValue == 13){//videos
		TInt photos;
		TInt videos;
		TInt album;
		TInt albumshares;
		iPhotosAppUi->GetUploadSettingsFromDB(album, photos, videos, albumshares);

		message.Copy(_L(""));
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_VIDEOS_UPLOADED)));
		message.Format(formatBuf, videos);

	}
	else if(iPhotosAppUi->iMainScreenValue == 14){//sms
		TInt smscount;
		iPhotosAppUi->GetSmsUploadSettingsFromDB(smscount);
		message.Copy(_L(""));
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_SMS_UPLOADED)));
		message.Format(formatBuf, smscount);
	}
*/
	CAknNavigationControlContainer* aNaviPane = (CAknNavigationControlContainer *)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidNavi));

	CAknNavigationDecorator* aNaviDecorator = aNaviPane->CreateMessageLabelL(message);

    aNaviPane->PushL(*aNaviDecorator);

	/////////////////////////////////////////////////////

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

	if(iPhotosAppUi->iMainScreenValue == 11){
		iContactCount = 3;
		tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_TITLE_CONTACTS))); // Set the text string.
	}
	else if(iPhotosAppUi->iMainScreenValue == 12){
		iContactCount = 5;//6;
		tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_TITLE_PHOTOS))); // Set the text string.
	}
	else if(iPhotosAppUi->iMainScreenValue == 13){
		iContactCount = 5;//6;
		tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_TITLE_VIDEOS))); // Set the text string.
	}
	else if(iPhotosAppUi->iMainScreenValue == 14){
		iContactCount = 3;//4;
		tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_TITLE_MESSAGES))); // Set the text string.
	}
	else if(iPhotosAppUi->iMainScreenValue == 15){
		iContactCount = 2;//4;
		tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_LIST_MAINMENU_SECURE))); // Set the text string.
	}

	ShowListBox();	
}

// Destructor
CMainSubScreenListContainer::~CMainSubScreenListContainer()
{
	items->Reset();
	delete iListBox;
}

// ---------------------------------------------------------
// CMainSubScreenListContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------

void CMainSubScreenListContainer::SizeChanged()
    {
        // TODO: Add here control resize code etc.
	iListBox->SetExtent(TPoint(0,0),TSize(176,150));
    }

// ---------------------------------------------------------
// CMainSubScreenListContainer::CountComponentControls() const
// ---------------------------------------------------------

TInt CMainSubScreenListContainer::CountComponentControls() const
    {
    return 1; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CMainSubScreenListContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------

CCoeControl* CMainSubScreenListContainer::ComponentControl(TInt aIndex) const
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
// CMainSubScreenListContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CMainSubScreenListContainer::Draw(const TRect& aRect ) const
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
// CMainSubScreenListContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CMainSubScreenListContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }

TKeyResponse CMainSubScreenListContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,TEventCode aType)
    {
    // See if we have a selection
    TInt code = aKeyEvent.iCode;

    switch(code)
        {
		case EKeyOK:
			{
				ShowScreen();
				return iListBox->OfferKeyEventL(aKeyEvent, aType);
			}
			break;
        default:
            // Let Listbox take care of its key handling
            return iListBox->OfferKeyEventL(aKeyEvent, aType);
            break;
        }
   		return (TKeyResponse)0;
    }

void CMainSubScreenListContainer::ShowListBox()
{
	CreateWindowL();

	iListBox = new (ELeave) CAknSingleLargeStyleListBox;

	iListBox->SetContainerWindowL( *this );
	iListBox->ConstructL( this, EAknListBoxSelectionList);

	// Create the scroll indicator
	iListBox->CreateScrollBarFrameL(ETrue);
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

	iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );
	
	SetFileList();	
	
	SizeChanged();

	SetGraphicIconL(iListBox);

	iListBox->ActivateL();	

	TRect Please(TPoint(0,42),TPoint(176,186));
	SetRect(Please);		

	ActivateL();
}

void CMainSubScreenListContainer::SetGraphicIconL( CEikColumnListBox* aListBox )
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

void CMainSubScreenListContainer::GraphicIconL( CArrayPtr<CGulIcon>* aIcons )
    {
	
	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KSubIconsMbmFileName);

	if ( aIcons )
        {
			if(iPhotosAppUi->iMainScreenValue == 11)
			{
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsContactsview,EMbmSubiconsContactsviewm));
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsPhotossendtovault,EMbmSubiconsPhotossendtovaultm));
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsListsettings,EMbmSubiconsListsettingsm));
			}
			else if(iPhotosAppUi->iMainScreenValue == 12)
			{
					// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsPhotosviewgallery,EMbmSubiconsPhotosviewgallerym));                
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsPhotossendtovault,EMbmSubiconsPhotossendtovaultm));
				// Appends icon.
				//aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsPhotosgetfromvault,EMbmSubiconsPhotosgetfromvaultm));
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsPhotossharealbum,EMbmSubiconsPhotossharealbumm));                
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsPhotosunsharealbums,EMbmSubiconsPhotosunsharealbumsm));
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsListsettings,EMbmSubiconsListsettingsm));
			}
			else if(iPhotosAppUi->iMainScreenValue == 13)
			{
					// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsVideosviewgallery,EMbmSubiconsVideosviewgallerym));                
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsVideossendtovault,EMbmSubiconsVideossendtovaultm));
				// Appends icon.
				//aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsVideosgetfromvault,EMbmSubiconsVideosgetfromvaultm));
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsVideossharealbum,EMbmSubiconsVideossharealbumm));                
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsVideosunsharealbum,EMbmSubiconsVideosunsharealbumm));
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsListsettings,EMbmSubiconsListsettingsm));
			}
			else if(iPhotosAppUi->iMainScreenValue == 14)
			{
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsSmsview,EMbmSubiconsSmsviewm));
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsSmssendtovault,EMbmSubiconsSmssendtovaultm));
				// Appends icon.
				//aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsSmsgetfromvault,EMbmSubiconsSmsgetfromvaultm));
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsListsettings,EMbmSubiconsListsettingsm));                					
			}
			else if(iPhotosAppUi->iMainScreenValue == 15) // Secure
			{
				TBuf<128> mainpath;
				mainpath = CRippleVaultAppUi::ApplicationDriveAndPath();
				mainpath.Append(KIconsMbmFileName);

				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( mainpath,EMbmIconsStatus,EMbmIconsStatusm));
				// Appends icon.
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsListsettings,EMbmSubiconsListsettingsm));                					
			}
			                
        }
    }

void CMainSubScreenListContainer::SetFileList()
 {
	_LIT(KStringFolder,"%d\t");
	_LIT(KStringNil,"");

	TBuf<50> bufb(KStringNil);
	TBuf<50> bufc;
	TFileName buf1;

	// Set the listbox to use the the file list model
	items = static_cast<CDesCArray*>(iListBox->Model()->ItemTextArray());
	
    iListBox->HandleItemRemovalL();
	
	for (TInt i=0; i < iContactCount; i++)
	{
		bufc.Copy(KStringFolder);		
		
		if(iPhotosAppUi->iMainScreenValue == 11)
		{
			if(i == 0)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_CONTACTS_VIEW)));
			else if(i == 1)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_CONTACTS_SENDTOVAULT)));
			else if(i == 2)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_CONTACTS_SETTINGS)));
		}
		else if(iPhotosAppUi->iMainScreenValue == 12)
		{
			if(i == 0)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_VIEW)));
			else if(i == 1)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SENDTOVAULT)));
			//else if(i == 4)
				//bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_GETFROMVAULT)));
			else if(i == 2)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SHARE)));
			else if(i == 3)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_UNSHARE)));
			else if(i == 4)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SETTINGS)));
		}
		else if(iPhotosAppUi->iMainScreenValue == 13)
		{
			if(i == 0)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_VIEW)));
			else if(i == 1)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SENDTOVAULT)));
			//else if(i == 4)
				//bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_GETFROMVAULT)));
			else if(i == 2)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SHARE)));
			else if(i == 3)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_UNSHARE)));
			else if(i == 4)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SETTINGS)));
		}
		else if(iPhotosAppUi->iMainScreenValue == 14)
		{
			if(i == 0)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_SMS_VIEW)));
			else if(i == 1)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_SMS_SENDTOVAULT)));
			//else if(i == 4)
				//bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_SMS_GETFROMVAULT)));			
			else if(i == 2)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_SMS_SETTINGS)));
		}
		else if(iPhotosAppUi->iMainScreenValue == 15)
		{
			if(i == 0)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_SECURESTATUS)));
			else if(i == 1)
				bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_SUBMENU_SMS_SETTINGS)));
		}

		buf1.Format(bufc,i,&bufb);

		items->AppendL(buf1);	
	}
 }

void CMainSubScreenListContainer::ShowScreen()
{
	if(iPhotosAppUi->iMainScreenValue == 11)//contacts
	{
		if(iListBox->CurrentItemIndex() == 0)
		{
			//View Contacts
			CAknInformationNote* note = new(ELeave) CAknInformationNote;
			//TBuf<40> text;
			//text.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION_VIEW_CONTACTS)));
			note->ExecuteLD(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION_VIEW_CONTACTS)));
		}
		else if(iListBox->CurrentItemIndex() == 1)
		{
			//contacts send to vault
			//iPhotosAppUi->iConnectionStatus = 2;
			//iPhotosAppUi->CheckSubscriptionStatus(iPhotosAppUi->iConnectionStatus);
			iPhotosAppUi->UpdateContactsManually();
			iPhotosAppUi->ActivateView(KViewIdEmptyScreen);
			iPhotosAppUi->ActivateView(KViewIdStatusScreen);
		}
		else if(iListBox->CurrentItemIndex() == 2)
		{
			//contacts settings
			//iPhotosAppUi->ActivateView(10);
			iPhotosAppUi->iSettingsScreenValue = 1;
			iPhotosAppUi->ActivateView(KViewIdSettingsScreen );
		}		
	}
	else if(iPhotosAppUi->iMainScreenValue == 12)//photos
	{
		if(iListBox->CurrentItemIndex() == 0)
		{
			//View Gallery
			TInt ret = 0;
			
			if(iPhotosAppUi->iSecureGallery)
				//ret = iPhotosAppUi->CheckUserId();
				ret = iPhotosAppUi->PasswordDialog();
			else
				ret = 1;

			if(ret == 1)
				iPhotosAppUi->OpenBrowser();
			/*else if(ret == 0)
			{
				iPhotosAppUi->ShowMessageBoxFromResource(R_TEXT_INVALID_USERID_PASSWORD);
			}*/

		}
		else if(iListBox->CurrentItemIndex() == 1)
		{
			//photo gallery
			iPhotosAppUi->iRipplePhotos = 2;
			iPhotosAppUi->iStoragePath = PHOTOS_PHONEMEMORY;
			iPhotosAppUi->ActivateView(KViewIdPhotoList );
		}
		/*else if(iListBox->CurrentItemIndex() == 2)
		{
			//GetFromVault
			CAknInformationNote* note = new(ELeave) CAknInformationNote;
			//TBuf<40> text;
			//text.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION)));
			note->ExecuteLD(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION)));
		}*/		
		else if(iListBox->CurrentItemIndex() == 2)
		{		
			//share album
			TInt ret;
			ret = iPhotosAppUi->ListAlbums();
			if(ret)
			{
				iPhotosAppUi->MailNames.Copy(_L(""));
				iPhotosAppUi->MailId.Copy(_L(""));
				//iPhotosAppUi->albumName.Copy(_L(""));
				iPhotosAppUi->iSharingCount=0;

				CMailIdListForm* iform= new (ELeave) CMailIdListForm;
				CleanupStack::PushL( iform );
				iform->iPhotosAppUi = iPhotosAppUi;
				iform->ConstructL();
				CleanupStack::Pop();
				iform->ExecuteLD(R_SHAREALBUM_DIALOG);
			}
		}
		else if(iListBox->CurrentItemIndex() == 3)
		{
			//unshare album
			TInt ret;
			ret = iPhotosAppUi->ListAlbums();
			if(ret)
			{
				CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_UNSHARE_ALBUM_CONFIRM)));
				abtdlg->PrepareLC(R_SMART_CHARGE);
				if(abtdlg->RunLD()) 
				{
					iPhotosAppUi->GetUnSharingAlbumXML();
				}
			}
		}
		else if(iListBox->CurrentItemIndex() == 4)
		{
			//photo settings
			//iPhotosAppUi->ActivateView(11);
			iPhotosAppUi->iSettingsScreenValue = 2;
			iPhotosAppUi->ActivateView(KViewIdSettingsScreen );
		}
		
	}
	else if(iPhotosAppUi->iMainScreenValue == 13)//videos
	{
		if(iListBox->CurrentItemIndex() == 0)
		{
			//View Gallery
			TInt ret = 0;
			
			if(iPhotosAppUi->iSecureGallery)
				//ret = iPhotosAppUi->CheckUserId();
				ret = iPhotosAppUi->PasswordDialog();
			else
				ret = 1;

			if(ret == 1)
				iPhotosAppUi->OpenBrowser();
			/*else if(ret == 0)
			{
				iPhotosAppUi->ShowMessageBoxFromResource(R_TEXT_INVALID_USERID_PASSWORD);
			}*/

		}
		else if(iListBox->CurrentItemIndex() == 1)
		{
			//photo gallery
			iPhotosAppUi->iRipplePhotos = 2;
			iPhotosAppUi->iStoragePath = VIDEOS_PHONEMEMORY;
			iPhotosAppUi->ActivateView(KViewIdPhotoList );
		}
		/*else if(iListBox->CurrentItemIndex() == 2)
		{
			//GetFromVault
			CAknInformationNote* note = new(ELeave) CAknInformationNote;
			TBuf<40> text;
			text.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION)));
			note->ExecuteLD(text);

		}*/		
		else if(iListBox->CurrentItemIndex() == 2)
		{		
			//share album
			TInt ret;
			ret = iPhotosAppUi->ListAlbums();
			if(ret)
			{
				iPhotosAppUi->MailNames.Copy(_L(""));
				iPhotosAppUi->MailId.Copy(_L(""));
				//iPhotosAppUi->albumName.Copy(_L(""));
				iPhotosAppUi->iSharingCount=0;

				CMailIdListForm* iform= new (ELeave) CMailIdListForm;
				CleanupStack::PushL( iform );
				iform->iPhotosAppUi = iPhotosAppUi;
				iform->ConstructL();
				CleanupStack::Pop();
				iform->ExecuteLD(R_SHAREALBUM_DIALOG);
			}
		}
		else if(iListBox->CurrentItemIndex() == 3)
		{
			//unshare album
			TInt ret;
			ret = iPhotosAppUi->ListAlbums();
			if(ret)
			{
				CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_UNSHARE_ALBUM_CONFIRM)));
				abtdlg->PrepareLC(R_ABOUT_QUERY);
				if(abtdlg->RunLD()) 
				{
					iPhotosAppUi->GetUnSharingAlbumXML();
				}
			}

		}
		else if(iListBox->CurrentItemIndex() == 4)
		{
			//photo settings
			//iPhotosAppUi->ActivateView(11);
			iPhotosAppUi->iSettingsScreenValue = 2;
			iPhotosAppUi->ActivateView(KViewIdSettingsScreen );
		}		
	}
	else if(iPhotosAppUi->iMainScreenValue == 14)//sms
	{
		if(iListBox->CurrentItemIndex() == 0)
		{
			//View Gallery
			//iPhotosAppUi->OpenBrowser();
			CAknInformationNote* note = new(ELeave) CAknInformationNote;
			//TBuf<40> text;
			//text.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION_VIEW_MESSAGES)));
			note->ExecuteLD(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION_VIEW_MESSAGES)));

		}
		else if(iListBox->CurrentItemIndex() == 1)
		{
			if(iPhotosAppUi->PendingSMS())
			{
				iPhotosAppUi->ShowMessageBoxFromResource(R_SMS_SENDTOWEB_BUSY);

				iPhotosAppUi->ActivateView(KViewIdStatusScreen);
			}
			else{
				//manually sms backup		
				iPhotosAppUi->iStoragePath = SMS_INBOX;			
				iPhotosAppUi->ActivateView(KViewIdSmsList );
			}
		}
		/*else if(iListBox->CurrentItemIndex() == 2)
		{
			//GetFromVault
			CAknInformationNote* note = new(ELeave) CAknInformationNote;
			TBuf<40> text;
			text.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION)));
			note->ExecuteLD(text);

		}*/		
		else if(iListBox->CurrentItemIndex() == 2)
		{		
			//sms settings
			//iPhotosAppUi->ActivateView(13);
			iPhotosAppUi->iSettingsScreenValue = 3;
			iPhotosAppUi->ActivateView(KViewIdSettingsScreen );
		}		
	}	
	else if(iPhotosAppUi->iMainScreenValue == 15)//secure
	{
		if(iListBox->CurrentItemIndex() == 0)
		{//show secure
			TInt ret;
			ret = iPhotosAppUi->CheckSecureApp();
			if(ret)
			{				
				iPhotosAppUi->ShowSecure();
				iPhotosAppUi->ShowSecure();
			}
			else
			{
				CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_SECURE_STATUS)));
				abtdlg->PrepareLC(R_SMART_CHARGE);
				if(abtdlg->RunLD()) 
				{
					iPhotosAppUi->ShowSecure();
					iPhotosAppUi->ShowSecure();
				}
			}
		}
		else if(iListBox->CurrentItemIndex() == 1)
		{		
			//secure settings
			iPhotosAppUi->iSettingsScreenValue = 4;
			iPhotosAppUi->ActivateView(KViewIdSettingsScreen);
		}		
	}
}

// End of File  
