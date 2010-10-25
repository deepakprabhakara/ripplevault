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
 Module Name				: SetupWizardListListBox.cpp
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
#include <etelbgsm.h>
#include <AgentClient.h>
#include <cdbcols.h>
#include <commdb.h>
#include <nifman.h>
#include <aknlistquerydialog.h> 
#include <aknmessagequerydialog.h>
#include <intconninit.h>			
#include <RippleVault.rsg>

#include "ripplevaultappui.h"

#include "SetupWizardListItemData.h"
#include "RippleVault.hrh"
#include "SetupWizardListView.h"

#include "SetupWizardListListbox.h"

#include <aknlists.h> 
#include <aknpopup.h> 

// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CSetupWizardListListbox::CreateSettingItemL
// Updates descripter array for list box
// ----------------------------------------------------------------------------
//
CAknSettingItem* CSetupWizardListListbox::CreateSettingItemL( TInt aIdentifier )
    {

    switch (aIdentifier)
    {
    case EAknExSettingIpAddress:
        settingItem = new (ELeave) 
            CAknTextSettingItem( aIdentifier, iData->iIpAddress);
		break;
	case EAknExSettingText:
        settingItem = new (ELeave) 
            CAknTextSettingItem(aIdentifier, iData->iApn);
		break;
    case EAknExSettingPort:
        settingItem = new (ELeave) 
            CAknIntegerEdwinSettingItem(aIdentifier, iData->iPort);
		break;
	case EAknExSettingAuto:
		settingItem = new (ELeave) 
            CAknBinaryPopupSettingItem(aIdentifier, iData->iAuto);
		break;		
//contacts
	case EAknExContactsSettingRoaming:
		//settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem(aIdentifier, iData->iContactsRoam );        
		settingItem = new (ELeave) CAknBinaryPopupSettingItem(aIdentifier, iData->iContactsRoam );        
		break;

//publish
	case EAknExSettingMode:
        settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem(
            aIdentifier, iData->iMode );
		break;
	case EAknExSettingDesc:
		settingItem = new (ELeave) 
            CAknBinaryPopupSettingItem(aIdentifier, iData->iDesc);        
		break;
	case EAknExPublishSettingRoaming:
		//settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem(
		settingItem = new (ELeave) CAknBinaryPopupSettingItem(
            aIdentifier, iData->iPublishRoam );        
		break;
	case EAknExPublishSettingSecureGallery:
		settingItem = new (ELeave) CAknBinaryPopupSettingItem(
            aIdentifier, iData->iSecureGallery );        
		break;

//sms
	case EAknExSmsMode:
		settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem(aIdentifier, iData->iSmsType );        
		break;
	case EAknExSmsTime:
		settingItem = new (ELeave) CAknTimeOrDateSettingItem(aIdentifier, CAknTimeOrDateSettingItem::ETime, iData->iTime );
		break;
	case EAknExSmsSettingRoaming:
		//settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem(aIdentifier, iData->iSmsRoam );        
		settingItem = new (ELeave) CAknBinaryPopupSettingItem(aIdentifier, iData->iSmsRoam );        
		break;		

//	case EAknExSettingMode:
//        settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem(
//            aIdentifier, iData->iMode );
//		break;
//	case EAknExSettingDesc:
//		settingItem = new (ELeave) 
//            CAknBinaryPopupSettingItem(aIdentifier, iData->iDesc);        
//		break;
//	case EAknExSettingRoaming:
//		settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem(
//            aIdentifier, iData->iRoam );        
//		break;
    default:
        break;
        }
    return settingItem;
    }

// ----------------------------------------------------------------------------
// CSetupWizardListListbox::SetData
// 
// ----------------------------------------------------------------------------
//
void CSetupWizardListListbox::SetData(CSetupWizardListItemData* aData)
    {
    iData = aData;
    }

// ----------------------------------------------------------------------------
// CSetupWizardListListbox::SetView
// 
// ----------------------------------------------------------------------------
//
void CSetupWizardListListbox::SetView(CSetupWizardListView* aView)
    {
    iView = aView;
    }

// ----------------------------------------------------------------------------
// CSetupWizardListListbox::SizeChanged
// Size changed handler
// ----------------------------------------------------------------------------
//
void CSetupWizardListListbox::SizeChanged()
    {
    if (ListBox()) 
        {
        ListBox()->SetRect(Rect());
        }
    }


TKeyResponse CSetupWizardListListbox::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {    
	TInt temp;	
	TInt iCurrentPos;
	TInt err;	

	if ( aType != EEventKey ) // Is not key event?
        {
        return EKeyWasNotConsumed;
        }

//	TInt active = 0;// iTabGroup->ActiveTabIndex();
//    TInt count = 4;//iTabGroup->TabCount();

    switch ( aKeyEvent.iCode ) // The code of key event is...
        {
/*		case EKeyLeftArrow:
            if ( active > 0 )
                {
					active--;
					//iTabGroup->SetActiveTabByIndex( active );					
					iPhotosAppUi->ActivateView(2);
					iPhotosAppUi->ActivateView(11);
					iPhotosAppUi->SaveSettingsToDB(iData->iIpAddress, (TUint16)iData->iPort, iData->iAuto );
                }
            break;
		case EKeyRightArrow:
            if( (active + 1) < count )
                {
					active++;
					//iTabGroup->SetActiveTabByIndex( active );					
					iPhotosAppUi->ActivateView(2);
					iPhotosAppUi->ActivateView(11);
					iPhotosAppUi->SaveSettingsToDB(iData->iIpAddress, (TUint16)iData->iPort, iData->iAuto );
                }
            break;*/
        case EKeyUpArrow:            
			CAknSettingItemList::OfferKeyEventL(aKeyEvent, aType);
			break; 
		case EKeyDownArrow:
			CAknSettingItemList::OfferKeyEventL(aKeyEvent, aType);			
			break; 
        //default:	
		case EKeyOK:	
//			TInt folder = 0;
			iCurrentPos = iView->iListBox->ListBox()->CurrentItemIndex();

		switch(iPhotosAppUi->iSettingsScreenValue){
			case 0:	
			if(iCurrentPos == 0)// || iCurrentPos == 1 || iCurrentPos == 2)
			{
				iView->iListBox->PopUp();				
				iView->iListBox->StoreSettingsL();
            }
			else if(iCurrentPos == 1)//3)
			{
				//iView->iListBox->SelectAccess();
				iPhotosAppUi->AccessPointSettings();
		        iView->iListBox->LoadSettingsL();
				//iPhotosAppUi->BringToForeground();				
			}

				break;
			case 1:	
			if(iCurrentPos == 0)
			{
				temp=iData->iContactsRoam;
				
				iView->iListBox->PopUp();
				iView->iListBox->StoreSettingsL();					
				
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
						iView->iListBox->LoadSettingsL();
					}
					else
					{
						iView->iListBox->LoadSettingsL();
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
					iView->iListBox->PopUp();
					iView->iListBox->StoreSettingsL();					

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
					
				iView->iListBox->LoadSettingsL();
			}
			else if(iCurrentPos == 0)
			{
				temp=iData->iMode;
				
				iView->iListBox->PopUp();
				iView->iListBox->StoreSettingsL();					
				
				if(iData->iMode==0)
				{
					
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_MESSAGE_AUTO)));
				    dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
				    err = dlg->RunLD();

					if (iView->iListBox->SelectFolder() == -1)
					{
						iData->iMode = temp;     
						iView->iListBox->LoadSettingsL();
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
					
				iView->iListBox->LoadSettingsL();
			}
			else if(iCurrentPos == 2)
			{
				temp=iData->iPublishRoam;
				
				iView->iListBox->PopUp();
				iView->iListBox->StoreSettingsL();					
				
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
						iView->iListBox->LoadSettingsL();
					}
					else
					{
						iView->iListBox->LoadSettingsL();
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
				
				iView->iListBox->PopUp();
				iView->iListBox->StoreSettingsL();					

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
				
					
				iView->iListBox->LoadSettingsL();
			}

				break;
			case 3:	
			if(iCurrentPos == 0)
			{
				temp=iData->iSmsType;
				
				iView->iListBox->PopUp();
				iView->iListBox->StoreSettingsL();					
				
				if(iData->iSmsType == 1)
				{
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_AUTO)));
					dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);					
					err = dlg->RunLD() ;

					/*TBuf<150> msg;					
					msg.Copy(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_MESSAGE1)));
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(msg);
					dlg->PrepareLC(R_ROAMING_MESSAGE);					
					err = dlg->RunLD();*/
					
						//iView->iListBox->LoadSettingsL();
			
				}
				else if(iData->iSmsType == 2)
				{
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_AUTO_HOURLY)));
					dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);					
					err = dlg->RunLD() ;
				}
				else if(iData->iSmsType == 0)
				{
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_SELECTIVE)));
					dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);					
					err = dlg->RunLD() ;

					/*TBuf<150> msg;
					msg.Copy(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_MESSAGE2)));
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(msg);
				    dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
				    err = dlg->RunLD();*/
					iPhotosAppUi->iStoragePath = 1;
					iView->iListBox->StoreSettingsL();
					iPhotosAppUi->SaveSmsSettingsToDB(iData->iSmsType, iData->iTime, iData->iSmsRoam);					
					iPhotosAppUi->ActivateView(KViewIdSelectiveSms );
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

				iView->iListBox->PopUp();				
				iView->iListBox->StoreSettingsL();
			}
			else if(iCurrentPos == 2)
			{
				temp=iData->iSmsRoam;
				
				iView->iListBox->PopUp();
				iView->iListBox->StoreSettingsL();					
				
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
						iView->iListBox->LoadSettingsL();
					}
					else
					{
						iView->iListBox->LoadSettingsL();
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
			}
            break;
        }	
    return EKeyWasConsumed;
    }

TInt CSetupWizardListListbox::PopUp()
    {
	TKeyEvent aKeyEvent;
	TEventCode aType; 
	aKeyEvent.iCode=63557;
	aKeyEvent.iScanCode=167;
	aKeyEvent.iModifiers=1;
	aKeyEvent.iRepeats=0;

	aType=EEventKey;

    CAknSettingItemList::OfferKeyEventL(aKeyEvent, aType);
	return EKeyWasNotConsumed;
   }

/*
void CSetupWizardListListbox::SelectAccess()
{
	TUint32 aIAPId=0; 	

#ifndef __UIQ__	
	CIntConnectionInitiator *iInitConn; 
	iInitConn = CIntConnectionInitiator::NewL(); 
	
	iInitConn->GetActiveIap(aIAPId);
#endif
	
	if(aIAPId ==0 )
	{
		RGenericAgent netAgent;		
		
		if (netAgent.Open() == KErrNone)
		{
			CleanupClosePushL(netAgent);
			
			CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeIAP);
			CleanupStack::PushL(db);
			CCommsDbConnectionPrefTableView* 
			view = db->OpenConnectionPrefTableInRankOrderLC(ECommDbConnectionDirectionOutgoing);
		
			if(view->GotoFirstRecord()!=KErrNotFound)
			{
				view->UpdateDialogPrefL(ECommDbDialogPrefPrompt);
				TRequestStatus status1;
				netAgent.StartOutgoing(status1);
				User::WaitForRequest(status1);
				view->UpdateDialogPrefL(ECommDbDialogPrefDoNotPrompt);				
			}

			CleanupStack::PopAndDestroy(3); // view, db, netAgent
		}
	}
	iData->DefaultAccessPoint();
}
*/
TInt CSetupWizardListListbox::SelectFolder()
{
    TInt folder = -1;
	if(iData->iMode == 0)
	{
		CDesCArray* array;

		array = iPhotosAppUi->GetFolderList();

        if (array)
        {
		    CAknListQueryDialog* dlg;
		    TInt index=0;
		    dlg = new(ELeave) CAknListQueryDialog(&index);			
		    dlg->PrepareLC(R_AKNEXQUERY_LIST_SINGLE_PANE);	
		    dlg->SetItemTextArray(array);			
		    dlg->SetOwnershipType(ELbmDoesNotOwnItemArray);					
		    
		    if(dlg->RunLD()) 
		    {			
		    	TPtrC ptr1(array->MdcaPoint(index));
		    	iPhotosAppUi->SetDefaultFolder(ptr1);
                folder = 0;
		    }
		    delete array;
        }
    }
    else
    {
        folder = 0;
    }
    return folder;
	
}

void CSetupWizardListListbox::SaveSettingsToDB()
{
	switch(iPhotosAppUi->iSettingsScreenValue){
		case 0:	
			iPhotosAppUi->SaveSettingsToDB(iData->iIpAddress, (TUint16)iData->iPort, iData->iAuto );
			break;
		case 1:	
			iPhotosAppUi->SaveContactsSettingsToDB(iData->iContactsRoam);
			break;
		case 2:	
			iPhotosAppUi->SavePhotosSettingsToDB(iData->iMode, iData->iDesc, iData->iPublishRoam, iData->iSecureGallery);
			break;
		case 3:	
			iPhotosAppUi->SaveSmsSettingsToDB(iData->iSmsType, iData->iTime, iData->iSmsRoam);
			break;
		default:
			break;
	}
    
}

// End of File
