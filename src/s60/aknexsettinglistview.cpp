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
 Module Name				: AknExSettingListView.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-03-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  
				Settings screen for S60 - Contacts, Publish, SMS and General Settings

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

// INCLUDE FILES
#include <avkon.hrh>
#include <avkon.rsg>
#include <aknviewappui.h>
//#include <etel.h>
//#include <etelbgsm.h>
//#include <commdb.h>
#include <bautils.h>
#include <aknmessagequerydialog.h>
#include <RippleVault.rsg>
#include "RippleVault.hrh"
#include "ripplevaultappui.h"
#include "aknexsettinglistcontainer.h"
#include "aknexsettinglistlistbox.h"
#include "aknexsettinglistitemdata.h"
#include <aknnotewrappers.h> 

#include "aknexsettinglistview.h"

// ================= MEMBER FUNCTIONS =========================================
// ----------------------------------------------------------------------------
// CAknExSettingListView::CAknExSettingListView()
// Default constructor.
// ----------------------------------------------------------------------------
//
CAknExSettingListView::CAknExSettingListView()
    {
			BaseConstructL( R_NEWLIST_VIEW1 );    
    }

// ----------------------------------------------------------------------------
// CAknExSettingListView::~CAknExSettingListView()
// Destructor.
// ----------------------------------------------------------------------------
//
CAknExSettingListView::~CAknExSettingListView()
    {
    // Deletes the Container class object.
    if ( iContainer )
        {
        // Removes Container from View control stack.
        AppUi()->RemoveFromStack( iContainer );
        }

    if (iListBox)
        {
        AppUi()->RemoveFromStack(iListBox);
        }

    delete iListBox;
    delete iData;
    }

// ----------------------------------------------------------------------------
// TUid CAknExSettingListView::Id() const
// Returns ID of View.
// ----------------------------------------------------------------------------
//
TUid CAknExSettingListView::Id() const
    {
    return KViewIdSettingsScreen;
    }

// ----------------------------------------------------------------------------
// CAknExSettingListView::HandleCommandL( TInt )
// Handles the commands.
// ----------------------------------------------------------------------------
//
void CAknExSettingListView::HandleCommandL( TInt aCommand )
    {
	TInt temp;
	TInt iCurrentPos;
	TInt err;

    switch ( aCommand ) // Command is...
        {
		
		case EAbtSecure:
			{
				TBuf<300> buffer;
				//buffer.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_RIPPLE_VAULT)));
				buffer.Copy(_L("Test"));
				CAknMessageQueryDialog* verdlg = CAknMessageQueryDialog::NewL(buffer);
				verdlg->PrepareLC(R_ABOUT_QUERY);
				verdlg->RunLD();
			}
			break;

        case EAknSoftkeyBack:
            {
				// save settings
				iListBox->StoreSettingsL();
                iListBox->SaveSettingsToDB();				
				
				if(iPhotosAppUi->RegFileExists()){
					switch(iPhotosAppUi->iSettingsScreenValue){
						case 0:						
						//case 4:
							AppUi()->HandleCommandL(EMainView);	
							break;
						case 1:
						case 2:	
						case 3:
						case 4:
							AppUi()->HandleCommandL(EMainSubView);	
							break;
						default:
							break;
					}
				}
				else
					AppUi()->HandleCommandL(EUnRegView);	
				
				iPhotosAppUi->iRipplePhotos = 2;
            }
			break;
		case ESave:	
			iCurrentPos = iListBox->ListBox()->CurrentItemIndex();

		switch(iPhotosAppUi->iSettingsScreenValue){
			case 0:				
				if(iPhotosAppUi->RegFileExists())
				{
					if(iCurrentPos == 0)
					/*{
						iPhotosAppUi->ChangePasswordFromSettings();
					}
					else if(iCurrentPos == 1)//3)*/
					{
						//iListBox->SelectAccess();
						iPhotosAppUi->AccessPointSettings();
						iData->DefaultAccessPoint();
						iListBox->LoadSettingsL();
						//iPhotosAppUi->BringToForeground();				
					}
				}
				else
				{
					if(iCurrentPos == 0)
					{
						//iListBox->SelectAccess();
						iPhotosAppUi->AccessPointSettings();
						iData->DefaultAccessPoint();
						iListBox->LoadSettingsL();
						//iPhotosAppUi->BringToForeground();				
					}
				}

				break;
			case 1:	
			if(iCurrentPos == 0)
			{
				temp=iData->iContactsUpload;
				
				iListBox->PopUp();
				iListBox->StoreSettingsL();					
				
				if(iData->iContactsUpload == 0)
				{
					//TBuf<150> msg;
					//msg.Copy(_L("Contacts will be uploaded even while Roaming."));
					//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_MESSAGE1)));
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_UPLOADMESSAGE1)));
					dlg->PrepareLC(R_ROAMING_MESSAGE);					
					err = dlg->RunLD() ;
					if (err == 0)
					{
						iData->iContactsUpload = temp;     						
					}

					iListBox->LoadSettingsL();					
				}
				else if(iData->iContactsUpload == 1)
				{
					//TBuf<150> msg;
					//msg.Copy(_L("Contacts taken while roaming will be uploaded after returning to home country."));					
					//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_MESSAGE2)));
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_UPLOADMESSAGE2)));
				    dlg->PrepareLC(R_ROAMING_MESSAGE);				    
					if (dlg->RunLD())
					{
						TInt days=14;
						TInt time=0;
						TInt syncstate = 0;
						
						CAknNumberQueryDialog* dialog1 = CAknNumberQueryDialog::NewL(days);
						if(dialog1->ExecuteLD(R_CONTACTS_SETTINGS_UPLOAD))
						{							
							iPhotosAppUi->SaveConatctsTimeToDB(time, days, syncstate);
						}
						else
						{
							iPhotosAppUi->GetConatctsTimeFromDB(time, days, syncstate);
							iPhotosAppUi->SaveConatctsTimeToDB(time, days, syncstate);
						}
					}
					else
					{
						iData->iContactsUpload = temp;
					}

					iListBox->LoadSettingsL();					
				}
				else
				{
					//TBuf<150> msg;
					//msg.Copy(_L("Contacts taken while roaming will be uploaded after returning to home country."));					
					//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_MESSAGE2)));
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_UPLOADMESSAGE3)));
				    dlg->PrepareLC(R_ROAMING_MESSAGE);
				    err = dlg->RunLD();
					if (err == 0)
					{
						iData->iContactsUpload = temp;     
					}
					
					iListBox->LoadSettingsL();					
				}
            }
			else if(iCurrentPos == 1)
			{
				temp=iData->iContactsRoam;
				
				iListBox->PopUp();
				iListBox->StoreSettingsL();					
				
				if(iData->iContactsRoam == 1)
				{
					//TBuf<150> msg;
					//msg.Copy(_L("Contacts will be uploaded even while Roaming."));
					//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_MESSAGE1)));
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_MESSAGE1)));
					dlg->PrepareLC(R_ROAMING_MESSAGE);					
					err = dlg->RunLD() ;
					if (err == 0)
					{
						iData->iContactsRoam = temp;     
						iListBox->LoadSettingsL();
					}
					else
					{
						iListBox->LoadSettingsL();
					}
				}
				else {
					//TBuf<150> msg;
					//msg.Copy(_L("Contacts taken while roaming will be uploaded after returning to home country."));					
					//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_MESSAGE2)));
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_MESSAGE2)));
				    dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
				    err = dlg->RunLD();
				}
				
            }					

				break;
			case 2:	
			if(iCurrentPos == 1)
			{
				temp=iData->iDesc;
				if(iData->iMode==1)
				{				
					iListBox->PopUp();
					iListBox->StoreSettingsL();					

					if(iData->iDesc==0) //disabled
					{
						CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_MESSAGE_DESCRIPTION_OFF)));
						dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
						err = dlg->RunLD();						
					}
					else
					{ // enabled
						CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_MESSAGE_DESCRIPTION_ON)));
						dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
						err = dlg->RunLD();						
					}
				}
					
				iListBox->LoadSettingsL();
			}
			else if(iCurrentPos == 0)
			{
				temp=iData->iMode;
				
				iListBox->PopUp();
				iListBox->StoreSettingsL();					
				
				if(iData->iMode==0)
				{					
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_MESSAGE_AUTO)));
				    dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
				    err = dlg->RunLD();

					if (iListBox->SelectFolder() == -1)
					{
						iData->iMode = temp;     
						iListBox->LoadSettingsL();
					}
					iData->iDesc = 0;
				}
				else if(iData->iMode == 1){
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_MESSAGE_PROMPT)));
				    dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
				    err = dlg->RunLD();
				}
				else
				{
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_MESSAGE_MANUAL)));
				    dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
				    err = dlg->RunLD();
					iData->iDesc = 0;
				}
					
				iListBox->LoadSettingsL();
			}
			else if(iCurrentPos == 2)
			{
				temp=iData->iPublishRoam;
				
				iListBox->PopUp();
				iListBox->StoreSettingsL();					
				
				if(iData->iPublishRoam == 1)
				{
					//TBuf<150> msg;
					//msg.Copy(_L("Photos will be uploaded even while Roaming."));
					//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_MESSAGE1)));
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_MESSAGE1)));
					dlg->PrepareLC(R_ROAMING_MESSAGE);					
					err = dlg->RunLD() ;
					if (err == 0)
					{
						iData->iPublishRoam = temp;     
						iListBox->LoadSettingsL();
					}
					else
					{
						iListBox->LoadSettingsL();
					}
				}
				else {
					//TBuf<150> msg;
					//msg.Copy(_L("Photos taken while roaming will be uploaded after returning to home country."));					
					//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_MESSAGE2)));
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_MESSAGE2)));
				    dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
				    err = dlg->RunLD();
				}
				
            }					
			/*else if(iCurrentPos == 3)
			{
				temp=iData->iSecureGallery;
				
				iListBox->PopUp();
				iListBox->StoreSettingsL();					

				if(iData->iSecureGallery==0) //disabled
				{
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_SECUREGALLERY_OFF)));
					dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
					err = dlg->RunLD();						

					err = iPhotosAppUi->PasswordDialog();//CheckUserId();

					if (err != 1)
					{
						//iPhotosAppUi->ShowMessageBoxFromResource(R_TEXT_INVALID_USERID_PASSWORD);
						iData->iSecureGallery = temp;     
						iListBox->LoadSettingsL();
					}
					else
					{
						iListBox->LoadSettingsL();
					}

				}
				else
				{ // enabled
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_SECUREGALLERY_ON)));
					dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
					err = dlg->RunLD();						
				}
				
					
				iListBox->LoadSettingsL();
			}*/

				break;
			case 3:	
			if(iCurrentPos == 0)
			{
				//temp=iData->iSmsType;
				
				iListBox->PopUp();
				iListBox->StoreSettingsL();					

				if(iData->iSmsType == 0)
				{
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_SELECTIVE)));
					dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);					
					err = dlg->RunLD() ;

					iPhotosAppUi->iStoragePath = 1;
					iListBox->StoreSettingsL();
					iPhotosAppUi->SaveSmsSettingsToDB(iData->iSmsType, iData->iTime, iData->iSmsRoam);					
					iPhotosAppUi->ActivateView(KViewIdSelectiveSms);
				}				
				else if(iData->iSmsType == 1)
				{
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_AUTO)));
					dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);					
					err = dlg->RunLD() ;
				}
				else if(iData->iSmsType == 2)
				{
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_AUTO_HOURLY)));
					dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);					
					err = dlg->RunLD() ;
				}
				else
				{
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_MANUAL)));
					dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);					
					err = dlg->RunLD() ;
				}				
            }
			/*else if(iCurrentPos == 1)
			{
				CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_UPLOAD_TIME)));
				dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);					
				err = dlg->RunLD() ;

				iListBox->PopUp();				
				iListBox->StoreSettingsL();
			}*/
			else if(iCurrentPos == 1)
			{
				temp=iData->iSmsRoam;
				
				iListBox->PopUp();
				iListBox->StoreSettingsL();					
				
				if(iData->iSmsRoam == 1)
				{
					//TBuf<150> msg;
					//msg.Copy(_L("Photos will be uploaded even while Roaming."));
					//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_ROAMING_MESSAGE1)));
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_ROAMING_MESSAGE1)));
					dlg->PrepareLC(R_ROAMING_MESSAGE);					
					err = dlg->RunLD() ;
					if (err == 0)
					{
						iData->iSmsRoam = temp;     
						iListBox->LoadSettingsL();
					}
					else
					{
						iListBox->LoadSettingsL();
					}
				}
				else {
					//TBuf<150> msg;
					//msg.Copy(_L("Photos taken while roaming will be uploaded after returning to home country."));					
					//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_ROAMING_MESSAGE2)));
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_ROAMING_MESSAGE2)));
				    dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
				    err = dlg->RunLD();
				}
				
            }					
				break;
			case 4:	
			{
				if(iCurrentPos == 0)
				{
					temp=iData->iAlarmSecure;
				
					iListBox->PopUp();
					iListBox->StoreSettingsL();					
					
					if(iData->iAlarmSecure == 1)
					{
						CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_ALARM_MESSAGE1)));
						dlg->PrepareLC(R_ROAMING_MESSAGE);					
						err = dlg->RunLD() ;
						if (err == 0)
						{
							iData->iAlarmSecure = temp;     
							iListBox->LoadSettingsL();
						}
						else
						{
							iListBox->LoadSettingsL();
						}
					}
					else
					{
						CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_ALARM_MESSAGE2)));
						dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
						err = dlg->RunLD();
					}
				}
				else if(iCurrentPos == 3)
				{
					temp=iData->iAutoSecure;
				
					iListBox->PopUp();
					iListBox->StoreSettingsL();					
					
					if(iData->iAutoSecure == 1)
					{
						CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_SECURE_MESSAGE1)));
						dlg->PrepareLC(R_ROAMING_MESSAGE);					
						err = dlg->RunLD() ;
						if (err == 0)
						{
							iData->iAutoSecure = temp;     
							iListBox->LoadSettingsL();
						}
						else
						{
							iListBox->LoadSettingsL();
						}
					}
					else
					{
						CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_SECURE_MESSAGE2)));
						dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
						err = dlg->RunLD();
					}
				}
				else if(iCurrentPos == 1)
				{
					iListBox->PopUp();
					iListBox->StoreSettingsL();
					iListBox->LoadSettingsL();
				}
				else if(iCurrentPos == 2)
				{
					iListBox->PopUp();
					iListBox->StoreSettingsL();
					iListBox->LoadSettingsL();
				}
			}
				break;
			default:
				break;
		}

			break;		
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CAknExSettingListView::CreateListBoxL
// 
// ----------------------------------------------------------------------------
//
void CAknExSettingListView::CreateListBoxL(TInt aResourceId)
    {	
    if (iListBox)
        {
        AppUi()->RemoveFromStack(iListBox);
        }
    delete iListBox;
    iListBox = NULL;

    delete iData;
    iData = CAknExSettingListItemData::NewL();

    iListBox = new (ELeave) CAknExSettingListListbox;
    iListBox->SetData(iData);

	switch(iPhotosAppUi->iSettingsScreenValue){
		case 0:	iPhotosAppUi->GetSettingsFromDB(iData->iIpAddress, iData->iPort, iData->iAuto);
			break;
		case 1:	iPhotosAppUi->GetContactsSettingsFromDB(iData->iContactsRoam, iData->iContactsUpload);
			break;
		case 2:	iPhotosAppUi->GetPhotosSettingsFromDB(iData->iMode, iData->iDesc, iData->iPublishRoam, iData->iSecureGallery);
			break;
		case 3:	iPhotosAppUi->GetSmsSettingsFromDB(iData->iSmsType, iData->iTime, iData->iSmsRoam);
			break;
		case 4:	iPhotosAppUi->GetSecureSettingsFromFile(iData->iAutoSecure, iData->iSecureFriendNumber, iData->iSecureText, iData->iAlarmSecure);
			break;
		default:
			break;
	}

    //iPhotosAppUi->GetSettingsFromDB(iData->iIpAddress, iData->iPort, iData->iAuto);
	iListBox->iPhotosAppUi = iPhotosAppUi;
    iListBox->SetView(this);
    iListBox->SetMopParent(this);
    iListBox->ConstructFromResourceL(aResourceId);
    AppUi()->AddToStackL( *this, iListBox );
    iListBox->MakeVisible(ETrue);
	TRect Please(TPoint(0,45),TPoint(176,188));
    iListBox->SetRect(Please);
    iListBox->ActivateL();
    iListBox->DrawNow();    
    }

// ----------------------------------------------------------------------------
// void CAknExSettingListView::DoActivateL( const TVwsViewId&, TUid,
//  const TDesC8& )
// Creates the Container class object.
// ----------------------------------------------------------------------------
//
void CAknExSettingListView::DoActivateL(
    const TVwsViewId& /*aPrevViewId*/,
    TUid /*aCustomMessageId*/,
    const TDesC8& /*aCustomMessage*/ )
    {
    // Creates the Container class object.
	
    if ( !iContainer )
        {
		
        iContainer = new( ELeave ) CAknExSettingListContainer( this );
        iContainer->SetMopParent(this);
		iContainer->iPhotosAppUi=iPhotosAppUi;
        iContainer->ConstructL( ClientRect() );
        // Adds Container to View control stack.
        AppUi()->AddToStackL( *this, iContainer );		

		iListBox = NULL;
		switch(iPhotosAppUi->iSettingsScreenValue){
			case 0:	CreateListBoxL(R_AKNEXSETTINGLIST_SETTING_LIST);		
				break;
			case 1:	CreateListBoxL(R_AKNEXCONTACTSSETTINGLIST_SETTING_LIST);		
				break;
			case 2:	CreateListBoxL(R_AKNEXPHOTOSSETTINGLIST_SETTING_LIST);		
				break;
			case 3:	CreateListBoxL(R_AKNEXSMSSETTINGLIST_SETTING_LIST);		
				break;
			case 4:	CreateListBoxL(R_SECURE_SETTING_LIST);		
				break;
			default:
				break;
		}

        }
    }

// ----------------------------------------------------------------------------
// void CAknExSettingListView::DoDeactivate()
// Deletes the Container class object.
// ----------------------------------------------------------------------------
//
void CAknExSettingListView::DoDeactivate()
    {
    // Deletes the container class object.
    if ( iContainer )
        {
        // Removes Container from View control stack.
        AppUi()->RemoveFromStack( iContainer );
        }

    if (iListBox)
        {
        AppUi()->RemoveFromStack(iListBox);
        }

    delete iListBox;
    iListBox = NULL;

    delete iContainer;
    iContainer = NULL;
    }

void CAknExSettingListView::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
{
	if(aResourceId == R_NEWLIST_APP_MENU)
	{
		if(iPhotosAppUi->iSettingsScreenValue != 4)
			aMenuPane->SetItemDimmed(EAbtSecure, ETrue);
	}
}