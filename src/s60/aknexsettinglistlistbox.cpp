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
 Module Name				: AknExSettingListListbox.cpp
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
//#include <etelbgsm.h>
//#include <AgentClient.h>
//#include <cdbcols.h>
//#include <commdb.h>
//#include <nifman.h>
#include <aknlistquerydialog.h> 
#include <aknmessagequerydialog.h>
//#include <intconninit.h>			
#include <RippleVault.rsg>

#include "ripplevaultappui.h"

#include "AknExSettingListItemData.h"
#include "RippleVault.hrh"
#include "AknExSettingListView.h"

#include "AknExSettingListListbox.h"

#include <aknlists.h> 
#include <aknpopup.h> 

// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CAknExSettingListListbox::CreateSettingItemL
// Updates descripter array for list box
// ----------------------------------------------------------------------------
//
CAknSettingItem* CAknExSettingListListbox::CreateSettingItemL( TInt aIdentifier )
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
	//case EAknExSettingAuto:
		//settingItem = new (ELeave) 
            //CAknBinaryPopupSettingItem(aIdentifier, iData->iAuto);
		//break;	
	/*case ESettingPassword:
		{
			iData->iPw.Copy(_L("123"));
			
			if(iPhotosAppUi->RegFileExists())
			{
				settingItem = new (ELeave) CAknPasswordSettingItem(aIdentifier, 
					CAknPasswordSettingItem::EAlpha, iData->iPw );
			}
		}
		break;		
		*/
//contacts
	case EAknExContactsSettingRoaming:
		settingItem = new (ELeave) CAknBinaryPopupSettingItem(aIdentifier, iData->iContactsRoam ); 
		break;
	case EAknExContactsSettingUploading:
		settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem(aIdentifier, iData->iContactsUpload );        
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
		settingItem = new (ELeave) CAknBinaryPopupSettingItem(aIdentifier, iData->iSmsRoam );        
		break;		
//secure			
		case ESecureAlarmLock:
			settingItem = new (ELeave) 
				CAknBinaryPopupSettingItem(aIdentifier, iData->iAlarmSecure);
		break;
		case ESecureNumber:
			settingItem = new (ELeave) 
				CAknTextSettingItem(aIdentifier, iData->iSecureFriendNumber);
		break;
		case ESecureText:
			settingItem = new (ELeave) 
				CAknTextSettingItem(aIdentifier, iData->iSecureText);
		break;
		case ESecureAutoLock:
			settingItem = new (ELeave) 
				CAknBinaryPopupSettingItem(aIdentifier, iData->iAutoSecure);
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
// CAknExSettingListListbox::SetData
// 
// ----------------------------------------------------------------------------
//
void CAknExSettingListListbox::SetData(CAknExSettingListItemData* aData)
    {
    iData = aData;
    }

// ----------------------------------------------------------------------------
// CAknExSettingListListbox::SetView
// 
// ----------------------------------------------------------------------------
//
void CAknExSettingListListbox::SetView(CAknExSettingListView* aView)
    {
    iView = aView;
    }

// ----------------------------------------------------------------------------
// CAknExSettingListListbox::SizeChanged
// Size changed handler
// ----------------------------------------------------------------------------
//
void CAknExSettingListListbox::SizeChanged()
    {
    if (ListBox()) 
        {
        ListBox()->SetRect(Rect());
        }
    }


TKeyResponse CAknExSettingListListbox::OfferKeyEventL(
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
		case EKeyOK:{	
//			TInt folder = 0;
			iCurrentPos = iView->iListBox->ListBox()->CurrentItemIndex();

		switch(iPhotosAppUi->iSettingsScreenValue){
			case 0:	
				{
					if(iPhotosAppUi->RegFileExists())
					{
						if(iCurrentPos == 0)
						/*{
							iPhotosAppUi->ChangePasswordFromSettings();
						}
						else if(iCurrentPos == 1)//3)*/
						{
							//iView->iListBox->SelectAccess();
							iPhotosAppUi->AccessPointSettings();
							iData->DefaultAccessPoint();
							iView->iListBox->LoadSettingsL();
							//iPhotosAppUi->BringToForeground();				
						}
					}
					else
					{
						if(iCurrentPos == 0)//3)
						{
							//iView->iListBox->SelectAccess();
							iPhotosAppUi->AccessPointSettings();
							iData->DefaultAccessPoint();
							iView->iListBox->LoadSettingsL();
							//iPhotosAppUi->BringToForeground();				
						}
					}
				}
				break;
			case 1:	{
			if(iCurrentPos == 0)
			{
				temp=iData->iContactsUpload;
				
				iView->iListBox->PopUp();
				iView->iListBox->StoreSettingsL();					
				
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

					iView->iListBox->LoadSettingsL();					
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

					iView->iListBox->LoadSettingsL();					
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
					
					iView->iListBox->LoadSettingsL();					
				}
            }
			else if(iCurrentPos == 1)
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
			}	
				break;
			case 2:	{
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
			/*else if(iCurrentPos == 3)
			{
				temp=iData->iSecureGallery;
				
				iView->iListBox->PopUp();
				iView->iListBox->StoreSettingsL();					

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
						iView->iListBox->LoadSettingsL();
					}
					else
					{
						iView->iListBox->LoadSettingsL();
					}

				}
				else
				{ // enabled
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_SECUREGALLERY_ON)));
					dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);
					err = dlg->RunLD();						
				}
				
					
				iView->iListBox->LoadSettingsL();
			}*/
			
			}
				break;
			case 3:	{
			if(iCurrentPos == 0)
			{
//				temp=iData->iSmsType;
				
				iView->iListBox->PopUp();
				iView->iListBox->StoreSettingsL();					
				if(iData->iSmsType == 0)
				{
					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMS_SELECTIVE)));
					dlg->PrepareLC(R_ROAMINGOFF_MESSAGE);					
					err = dlg->RunLD() ;

					iPhotosAppUi->iStoragePath = 1;
					iView->iListBox->StoreSettingsL();
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

				iView->iListBox->PopUp();				
				iView->iListBox->StoreSettingsL();
			}*/
			else if(iCurrentPos == 1)
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
			case 4:	
			{
				if(iCurrentPos == 0)
				{
					temp=iData->iAlarmSecure;
				
					iView->iListBox->PopUp();
					iView->iListBox->StoreSettingsL();					
					
					if(iData->iAlarmSecure == 1)
					{
						CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_ALARM_MESSAGE1)));
						dlg->PrepareLC(R_ROAMING_MESSAGE);					
						err = dlg->RunLD() ;
						if (err == 0)
						{
							iData->iAlarmSecure = temp;     
							iView->iListBox->LoadSettingsL();
						}
						else
						{
							iView->iListBox->LoadSettingsL();
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
				
					iView->iListBox->PopUp();
					iView->iListBox->StoreSettingsL();					
					
					if(iData->iAutoSecure == 1)
					{
						CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_SECURE_MESSAGE1)));
						dlg->PrepareLC(R_ROAMING_MESSAGE);					
						err = dlg->RunLD() ;
						if (err == 0)
						{
							iData->iAutoSecure = temp;     
							iView->iListBox->LoadSettingsL();
						}
						else
						{
							iView->iListBox->LoadSettingsL();
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
					iView->iListBox->PopUp();
					iView->iListBox->StoreSettingsL();
					iView->iListBox->LoadSettingsL();
				}
				else if(iCurrentPos == 2)
				{
					iView->iListBox->PopUp();
					iView->iListBox->StoreSettingsL();
					iView->iListBox->LoadSettingsL();
				}
			}
				break;
			default:
				break;
			} //switch for case 3
			} // key OK case
            break;
		default:
			break;

        }	// key event switch
    return EKeyWasConsumed;
    }

TInt CAknExSettingListListbox::PopUp()
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
void CAknExSettingListListbox::SelectAccess()
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
TInt CAknExSettingListListbox::SelectFolder()
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

void CAknExSettingListListbox::SaveSettingsToDB()
{
	switch(iPhotosAppUi->iSettingsScreenValue){
		case 0:	
			iPhotosAppUi->SaveSettingsToDB(iData->iIpAddress, (TUint16)iData->iPort, iData->iAuto );
			break;
		case 1:	
			iPhotosAppUi->SaveContactsSettingsToDB(iData->iContactsRoam, iData->iContactsUpload);
			break;
		case 2:	
			iPhotosAppUi->SavePhotosSettingsToDB(iData->iMode, iData->iDesc, iData->iPublishRoam, iData->iSecureGallery);
			break;
		case 3:	
			iPhotosAppUi->SaveSmsSettingsToDB(iData->iSmsType, iData->iTime, iData->iSmsRoam);
			break;
		case 4:	
			iPhotosAppUi->SaveSecureSettingsToFile(iData->iAutoSecure, iData->iSecureFriendNumber, iData->iSecureText, iData->iAlarmSecure);
			break;
		default:
			break;
	}
    
}

// End of File
