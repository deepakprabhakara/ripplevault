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
 Module Name				: SetupWizardListView.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 10-05-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Settings Screen used during Setup Wizard
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

// INCLUDE FILES
#include <avkon.hrh>
#include <avkon.rsg>
#include <aknviewappui.h>
#include <etel.h>
#include <etelbgsm.h>
#include <commdb.h>
#include <bautils.h>
#include <aknmessagequerydialog.h>
#include <RippleVault.rsg>
#include "RippleVault.hrh"
#include "ripplevaultappui.h"
#include "SetupWizardlistcontainer.h"
#include "SetupWizardlistlistbox.h"
#include "SetupWizardlistitemdata.h"
#include <aknnotewrappers.h> 

#include "SetupWizardlistview.h"

// ================= MEMBER FUNCTIONS =========================================
// ----------------------------------------------------------------------------
// CSetupWizardListView::CSetupWizardListView()
// Default constructor.
// ----------------------------------------------------------------------------
//
CSetupWizardListView::CSetupWizardListView()
    {
		BaseConstructL( R_SETUP_VIEW ); 
    }

// ----------------------------------------------------------------------------
// CSetupWizardListView::~CSetupWizardListView()
// Destructor.
// ----------------------------------------------------------------------------
//
CSetupWizardListView::~CSetupWizardListView()
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
// TUid CSetupWizardListView::Id() const
// Returns ID of View.
// ----------------------------------------------------------------------------
//
TUid CSetupWizardListView::Id() const
    {
    return KViewIdSetupWizard;
    }

// ----------------------------------------------------------------------------
// CSetupWizardListView::HandleCommandL( TInt )
// Handles the commands.
// ----------------------------------------------------------------------------
//
void CSetupWizardListView::HandleCommandL( TInt aCommand )
    {
	TInt temp;
	TInt iCurrentPos;
	TInt err;

    switch ( aCommand ) // Command is...
        {
        case EAknSoftkeyBack:
            {
				// save settings
				iListBox->StoreSettingsL();
                iListBox->SaveSettingsToDB();				
				
				if(iPhotosAppUi->RegFileExists())
				//if(1)
				{
					switch(iPhotosAppUi->iSettingsScreenValue)
					{
						case 0:						
							AppUi()->HandleCommandL(EMainView);	
							break;
						case 1:
							//iPhotosAppUi->ShowSetupWizard(1);						
							iPhotosAppUi->CallSetupWizardStep5();
							break;
						case 2:	
							//iPhotosAppUi->ShowSetupWizard(2);
							//iPhotosAppUi->ShowMessage(2);
							iPhotosAppUi->CallSetupWizardStep11();
							break;
						case 3:
							//iPhotosAppUi->ShowMessage();
							//AppUi()->HandleCommandL(EMainView);	
							//iPhotosAppUi->ShowSetupWizard(3);
							//iPhotosAppUi->ShowMessage(3);
							iPhotosAppUi->CallSetupWizardStep17();
							break;
						default:
							break;
					}
				}
				else
					AppUi()->HandleCommandL(EUnRegView);	
				
				iPhotosAppUi->iRipplePhotos = 2;
				break;
            }	
		case ESave:	
			iCurrentPos = iListBox->ListBox()->CurrentItemIndex();

		switch(iPhotosAppUi->iSettingsScreenValue)
		{
			case 0:	
			if(iCurrentPos == 0)// || iCurrentPos == 1 || iCurrentPos == 2)
			{
				iListBox->PopUp();				
				iListBox->StoreSettingsL();
            }
			else if(iCurrentPos == 1)//3)
			{
				//iListBox->SelectAccess();
				iPhotosAppUi->AccessPointSettings();
		        iListBox->LoadSettingsL();
				//iPhotosAppUi->BringToForeground();				
			}

				break;
			case 1:	
			if(iCurrentPos == 0)
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
				else if(iData->iMode==1){
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
			else if(iCurrentPos == 3)
			{
				temp=iData->iSecureGallery;
				
				iListBox->PopUp();
				iListBox->StoreSettingsL();					

				if(iData->iSecureGallery==0) //disabled
				{
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_SECUREGALLERY_OFF)));
					dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
					err = dlg->RunLD();						
				}
				else
				{ // enabled
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_SECUREGALLERY_ON)));
					dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
					err = dlg->RunLD();						
				}
				
					
				iListBox->LoadSettingsL();
			}

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
					iPhotosAppUi->ActivateView(KViewIdSelectiveSms );
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
			else if(iCurrentPos == 1)
			{
				CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_UPLOAD_TIME)));
				dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);					
				err = dlg->RunLD() ;

				iListBox->PopUp();				
				iListBox->StoreSettingsL();
			}
			else if(iCurrentPos == 2)
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
			default:
				break;
		}

			break;		
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CSetupWizardListView::CreateListBoxL
// 
// ----------------------------------------------------------------------------
//
void CSetupWizardListView::CreateListBoxL(TInt aResourceId)
    {	
    if (iListBox)
        {
        AppUi()->RemoveFromStack(iListBox);
        }
    delete iListBox;
    iListBox = NULL;

    delete iData;
    iData = CSetupWizardListItemData::NewL();

    iListBox = new (ELeave) CSetupWizardListListbox;
    iListBox->SetData(iData);

	switch(iPhotosAppUi->iSettingsScreenValue){
		case 0:	iPhotosAppUi->GetSettingsFromDB(iData->iIpAddress, iData->iPort, iData->iAuto);
			break;
		case 1:	iPhotosAppUi->GetContactsSettingsFromDB(iData->iContactsRoam);
			break;
		case 2:	iPhotosAppUi->GetPhotosSettingsFromDB(iData->iMode, iData->iDesc, iData->iPublishRoam, iData->iSecureGallery);
			break;
		case 3:	iPhotosAppUi->GetSmsSettingsFromDB(iData->iSmsType, iData->iTime, iData->iSmsRoam);
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
// void CSetupWizardListView::DoActivateL( const TVwsViewId&, TUid,
//  const TDesC8& )
// Creates the Container class object.
// ----------------------------------------------------------------------------
//
void CSetupWizardListView::DoActivateL(
    const TVwsViewId& /*aPrevViewId*/,
    TUid /*aCustomMessageId*/,
    const TDesC8& /*aCustomMessage*/ )
    {
    // Creates the Container class object.
	
    if ( !iContainer )
        {
		
        iContainer = new( ELeave ) CSetupWizardListContainer( this );
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
			default:
				break;
		}

        }
    }

// ----------------------------------------------------------------------------
// void CSetupWizardListView::DoDeactivate()
// Deletes the Container class object.
// ----------------------------------------------------------------------------
//
void CSetupWizardListView::DoDeactivate()
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

