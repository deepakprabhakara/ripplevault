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
 Module Name				: UpdateContactsAO.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Active Object for updating contacts on the phone
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/


#include "UpdateContactsAO.h"

#include <etel.h>

//#include <AgentClient.h>
//#include <cdbcols.h>
//#include <commdb.h>
#include "SocketsEngineWriter.h"
#include "RippleVaultAppUi.h"
#include "List.h"
#include "RecordContactDB.h"
#include "ContactsHandler.h"
#include "ContactsSocketsEngineWriter.h"
#include "ContactsDBEngine.h"

 
CUpdateContactsAO::CUpdateContactsAO(CRippleVaultAppUi& aAppUi, CContactsSocketsEngineWriter& aSocketsEngine)
: CActive(EPriorityIdle),
  iAppUi(aAppUi),
  iSocketsEngine(aSocketsEngine)
{
	CActiveScheduler::Add(this);
}


CUpdateContactsAO::~CUpdateContactsAO()
{
	Cancel();
}

void CUpdateContactsAO::StartL(CList *iContacts)
{
	iContactIndex = 0;
	iPtr = iContacts;
	iRecordList = iContacts;

	_LIT8(SESSION,"Set-Cookie: ");
	_LIT8(ENDOFSESSION,"\r\n");
	TInt start = iSocketsEngine.iBuffer.Find(SESSION);

	if (start != KErrNotFound) {
		iSocketsEngine.iBuffer.Copy(&iSocketsEngine.iBuffer[start]);
		TInt end = iSocketsEngine.iBuffer.Find(ENDOFSESSION);
		if (end != KErrNotFound)
			iSessionId.Copy(&iSocketsEngine.iBuffer[12], end - 12);
	}

	iSocketsEngine.iBuffer.Copy(_L("<imei>"));
	iSocketsEngine.iBuffer.Append(iAppUi.PhoneImei());
	iSocketsEngine.iBuffer.Append(_L("</imei>"));
	iSocketsEngine.iBuffer.Append(_L("<ack>"));

	if (iContacts->Count() > 0)
	{
		// Reschedule ourselves.
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, KErrNone);
		if (!IsActive())
		SetActive();
	}
	else 
	{
		TInt ccount = 0;
		iAppUi.GetTempContactsUploadSettingsFromDB(ccount);

		TInt ccount1 = 0;
		iAppUi.GetContactsUploadSettingsFromDB(ccount1);

		ccount = ccount + ccount1;

		iAppUi.SaveContactsUploadSettingsToDB(ccount);
		
		iAppUi.iContactsCount = 0;
		
		iAppUi.SaveTempContactsUploadSettingsToDB(iAppUi.iContactsCount);	
		
		EndAcks();
	}
}

void CUpdateContactsAO::RunL()
{
	if (iContactIndex < iRecordList->Count())
	{
		// Reschedule ourselves.
		Notify();
		
		// Update the next contact.
		UpdateContactL();
		iContactIndex++;
	}
	else if(iContactIndex >= iRecordList->Count())
	{
		EndAcks();
	}	
}

void CUpdateContactsAO::DoCancel()
{
}

TInt CUpdateContactsAO::RunError(TInt aError)
{
//	TBuf8<60> buf;
//	buf.Copy(_L8("CUpdateContactsAO Leave Error: "));
//	buf.AppendNum(aError);
//	buf.Append(_L("\r\n"));
//	iAppUi.WriteLogFile(buf);

	// Just return KErrNone to stop the active scheduler panicking.
	return KErrNone;
}

void CUpdateContactsAO::UpdateContactL()
{
//	CList *fieldItems;
//	TInt index;

	CRecordContactDB *record = (CRecordContactDB *)iPtr->GetContent();

	if (record->iChangeType == 0) { // add contact on server
		if (iAppUi.iContactsHandler->iChangesDB->ExistsGlobalId(*(record->iGlobalId))) {
			iPtr = iPtr->next;
			return;
		}
		TInt ccount = 0;
		iAppUi.GetTempContactsUploadSettingsFromDB(ccount);

		ccount++;
		//iAppUi.iContactsCount++;
		//iAppUi.iContactsCount = iAppUi.iContactsCount + ccount;
		//iAppUi.SaveTempContactsUploadSettingsToDB(iAppUi.iContactsCount);
		iAppUi.SaveTempContactsUploadSettingsToDB(ccount);
				
		CContactDatabase* database;
		
		database = iAppUi.ContactDataBase();
		
		TInt error;
		
		TContactItemId templateItemId = database->TemplateId();
		CContactTemplate* ct=NULL;
		TRAP(error, ct = (CContactTemplate*)database->OpenContactL(templateItemId)); 
		if (error != KErrNone)
		{
			//iContactIndex--;
			//Notify();
			iPtr = iPtr->next;
			return;
		}
		CleanupStack::PushL(ct);
		
		database->CloseContactL(templateItemId);
		
		CContactCard *contact= CContactCard::NewL(ct);
		
		CleanupStack::PushL(contact);

		/*TBuf8<60> buf;
		buf.Copy(_L("Adding s->c: "));
		buf.AppendNum((TInt)templateItemId);
		buf.Append(_L("\r\n"));
		iAppUi.Logger(buf);*/
		/*TBuf8<60> buf;
		buf.Copy(_L("Adding s->c: "));
		buf.Append(*(record->iFirstName));
		buf.Append(_L(" "));
		buf.Append(*(record->iLastName));
		buf.Append(_L("\r\n"));
		iAppUi.Logger(buf);*/
		
//		CContactItemFieldSet& fieldSet = contact->CardFields();			
		
		AddFirstName(record, (CContactItem*)contact);
		AddLastName(record, (CContactItem*)contact);
		AddCompanyName(record, (CContactItem*)contact);
		AddJobTitles(record, (CContactItem*)contact);
		
		AddHomeMobiles(record, (CContactItem*)contact);
		AddWorkMobiles(record, (CContactItem*) contact);
		AddMobiles(record, (CContactItem*) contact);
		
		AddHomeTelephones(record, (CContactItem*)contact);
		AddWorkTelephones(record, (CContactItem*)contact);
		AddTelephones(record, (CContactItem*)contact);
		
		AddHomeFaxes(record, (CContactItem*)contact);
		AddWorkFaxes(record, (CContactItem*)contact);
		AddFaxes(record, (CContactItem*)contact);

		AddHomeEmails(record, (CContactItem*)contact);
		AddWorkEmails(record, (CContactItem*)contact);
		AddEmails(record, (CContactItem*)contact);
		
		TInt id;
		id = database->AddNewContactL(*contact);

		TBuf<256> gcid;
		gcid.Copy(*(record->iGlobalId));
		
		iAppUi.iContactsHandler->iChangesDB->AddRecursedElement(id, 0);
		iAppUi.iContactsHandler->iChangesDB->CreateAddedMap(id, 3, gcid);
		iAppUi.iContactsHandler->iChangesDB->AddGlobalId(*(record->iGlobalId));
		iAppUi.iContactsHandler->iChangesDB->AddId(id);
		
		CleanupStack::PopAndDestroy(2); // ct, contact

		UpdateTimeStamp();
		
		TBuf8<10> b2;
		b2.Num(id);
		
		iSocketsEngine.iBuffer.Append(_L("<id>"));
		iSocketsEngine.iBuffer.Append(b2);
		iSocketsEngine.iBuffer.Append(_L("</id>"));
		
	}
	else if (record->iChangeType == 1) { // update

		if (record->iContactId < 0)
		{
			iSocketsEngine.iBuffer.Append(_L("<id>"));
			iSocketsEngine.iBuffer.Append(_L("</id>"));

			iPtr = iPtr->next;
			//Notify();
			return;
		}
		
		iAppUi.iContactsHandler->iChangesDB->AddRecursedElement(record->iContactId, 1);
		
		CContactDatabase* database;
		
		database = iAppUi.ContactDataBase();
		
		CContactItem* contact=NULL;
		TInt error;
		
		TRAP(error, contact = database->OpenContactL((TInt)record->iContactId));
		
		if (error == KErrInUse || error == KErrNotFound || record->iContactId == 0)
		{
			iSocketsEngine.iBuffer.Append(_L("<id>"));
			iSocketsEngine.iBuffer.Append(_L("</id>"));

//			TBuf8<60> buf;
//			buf.Copy(_L("Within update contact KErrInUse or KErrNotFound\r\n"));
//			iAppUi.WriteLogFile(buf);

			database->CloseContactL((TInt)record->iContactId);
			//delete contact;

			iPtr = iPtr->next;
			//Notify();
			return;
		}

		/*TBuf8<60> buf;
		buf.Copy(_L("Updating s->c: "));
		buf.AppendNum((TInt)record->iContactId);
		buf.Append(_L("\r\n"));
		iAppUi.Logger(buf);
		*/
		
		CContactItemFieldSet& fieldSet = contact->CardFields();

		if (&fieldSet)
			fieldSet.Reset();
		
		AddFirstName(record, contact);
		AddLastName(record, contact);
		AddCompanyName(record, contact);
		AddJobTitles(record, contact);
		
		AddHomeMobiles(record, contact);
		AddWorkMobiles(record, contact);
		AddMobiles(record, contact);
		
		AddHomeTelephones(record, contact);
		AddWorkTelephones(record, contact);
		AddTelephones(record, contact);
		
		AddHomeFaxes(record, contact);
		AddWorkFaxes(record, contact);
		AddFaxes(record, contact);

		AddHomeEmails(record, contact);
		AddWorkEmails(record, contact);
		AddEmails(record, contact);

#if 0
		UpdateJobTitle(record, contact);
		UpdateEmail(record, contact);
		UpdateFax(record, contact);

		// mobiles, phones ...
		TInt findfield = 0;
		for (int i = 0; i < fieldSet.Count(); i++)
		{
			index = fieldSet.FindNext(KUidContactFieldPhoneNumber, i);
			
			if (index >= 0)
			{
				CContactItemField& datafield = fieldSet[index];
				if ( datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapFAX) ||
					datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapPAGER)
					)
				{
					continue;
				}
				
				// home mobile
				if ( datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL) &&
					datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME) )
				{
					CContactTextField* text = datafield.TextStorage();
					fieldItems = record->iHomeMobiles;
					if (fieldItems->Count())
					{
						TPtr *data = (TPtr *)fieldItems->GetContent();
						text->SetTextL(*data);
					}
					findfield = 1;
				}
				// work mobile
				else if ( datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL) &&
					datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK) )
				{
					CContactTextField* text = datafield.TextStorage();
					fieldItems = record->iWorkMobiles;
					if (fieldItems->Count())
					{
						TPtr *data = (TPtr *)fieldItems->GetContent();
						text->SetTextL(*data);
					}
					findfield = 1;
				}
				// mobile
				else if ( datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL) &&
					!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK) &&
					!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME) )
				{
					CContactTextField* text = datafield.TextStorage();
					fieldItems = record->iMobiles;
					if (fieldItems->Count())
					{
						TPtr *data = (TPtr *)fieldItems->GetContent();
						text->SetTextL(*data);
					}
					findfield = 1;
				}
				// home telephones
				else if ( !datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL) &&
					!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK) &&
					!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapFAX) &&
					datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME) )
				{
					CContactTextField* text = datafield.TextStorage();
					fieldItems = record->iHomeTelephones;
					if (fieldItems->Count())
					{
						TPtr *data = (TPtr *)fieldItems->GetContent();
						text->SetTextL(*data);
					}
					findfield = 1;
				}
				// work telephones
				else if ( !datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL) &&
					datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK) &&
					!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapFAX) &&
					!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME) )
				{
					CContactTextField* text = datafield.TextStorage();
					fieldItems = record->iWorkTelephones;
					if (fieldItems->Count())
					{
						TPtr *data = (TPtr *)fieldItems->GetContent();
						text->SetTextL(*data);
					}
					findfield = 1;
				}
				// telephones
				else if ( !datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL) &&
					!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK) &&
					!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME) &&
					!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapTELFAX) &&
					!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapFAX) &&
					!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapPAGER) 
					)
				{
					CContactTextField* text = datafield.TextStorage();
					fieldItems = record->iTelephones;
					if (fieldItems->Count())
					{
						TPtr *data = (TPtr *)fieldItems->GetContent();
						text->SetTextL(*data);
					}
					findfield = 1;
				}
				// faxes
			}
		} // for() mobiles, phones ...
		
		if (findfield == 0) // field not avlbl in fieldset
		{
			// add field to fieldset
			AddHomeMobiles(record, contact);
			AddWorkMobiles(record, contact);
			AddMobiles(record, contact);
			
			AddHomeTelephones(record, contact);
			AddWorkTelephones(record, contact);
			AddTelephones(record, contact);
			//AddFaxes(record, contact);
			
		} // if (findfield == 0)
#endif
		
		database->CommitContactL(*contact);
		
		database->CloseContactL((TInt)record->iContactId);
		
		delete contact;

		UpdateTimeStamp();
		
		TBuf8<10> b2;
		b2.Num((TInt)record->iContactId);
		
		iSocketsEngine.iBuffer.Append(_L("<id>"));
		iSocketsEngine.iBuffer.Append(b2);
		iSocketsEngine.iBuffer.Append(_L("</id>"));
	}
	else {
		
		TInt id = (TInt)record->iContactId;
		
		CContactDatabase *cdb;
		
		cdb = iAppUi.ContactDataBase();
		
		TInt error;
		TRAP(error, cdb->DeleteContactL(id));
		if (error != KErrInUse)
		{
			cdb->CompactL();
			iAppUi.iContactsHandler->iChangesDB->AddRecursedElement(id, 2);
			iAppUi.iContactsHandler->iChangesDB->CreateDeletedMap(id, 4);
			iAppUi.iContactsHandler->iChangesDB->DeleteId(id);

			/*TBuf8<60> buf;
			buf.Copy(_L("Deleting s->c: "));
			buf.AppendNum((TInt)record->iContactId);
			buf.Append(_L("\r\n"));
			iAppUi.Logger(buf);*/
			
			UpdateTimeStamp();

			TBuf8<10> b2;
			b2.Num(id);
			
			iSocketsEngine.iBuffer.Append(_L("<id>"));
			iSocketsEngine.iBuffer.Append(b2);
			iSocketsEngine.iBuffer.Append(_L("</id>"));

			TInt ccount = 0;
			iAppUi.GetTempContactsUploadSettingsFromDB(ccount);
			
			ccount--;
			//iAppUi.iContactsCount--;
			//iAppUi.iContactsCount = iAppUi.iContactsCount + ccount;
			
			//iAppUi.SaveTempContactsUploadSettingsToDB(iAppUi.iContactsCount);
			iAppUi.SaveTempContactsUploadSettingsToDB(ccount);

		}
		
	}
	iAppUi.ContactDataBase()->CompactL();
	iPtr = iPtr->next;
	return;
}

void CUpdateContactsAO::Notify()
{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	if (!IsActive())
    SetActive();
}

void CUpdateContactsAO::AddTelephones(CRecordContactDB *record, CContactItem* contact)
{
	// Telephone Numbers
	CList* fieldItems = record->iTelephones;
	for (TInt i = 0; i < record->iTelephones->Count(); i++)
	{
		CContactItemField* telephoneField = CContactItemField::NewL(KStorageTypeText, KUidContactFieldPhoneNumber);
		CContactTextField* telephoneText = telephoneField->TextStorage();
	
		TPtr *data = (TPtr *)fieldItems->GetContent();
	
		if (data != NULL) {
			telephoneText->SetTextL(*data);
			telephoneField->SetMapping(KUidContactFieldVCardMapTEL);
			//fieldSet.AddL(*telephoneField);
			contact->AddFieldL(*telephoneField);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CUpdateContactsAO::AddHomeTelephones(CRecordContactDB *record, CContactItem* contact)
{
	// Home Telephone Numbers
	CList* fieldItems = record->iHomeTelephones;
	for (TInt i = 0; i < record->iHomeTelephones->Count(); i++)
	{
		CContactItemField* homeTelephoneField = CContactItemField::NewL(KStorageTypeText, KUidContactFieldPhoneNumber);
		CContactTextField* homeTelephoneText = homeTelephoneField->TextStorage();

		TPtr *data = (TPtr *)fieldItems->GetContent();
	
		if (data != NULL) {
			homeTelephoneText->SetTextL(*data);
			homeTelephoneField->SetMapping(KUidContactFieldVCardMapTEL);
			homeTelephoneField->AddFieldTypeL(KUidContactFieldVCardMapHOME);
			//fieldSet.AddL(*homeTelephoneField);
			contact->AddFieldL(*homeTelephoneField);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CUpdateContactsAO::AddWorkTelephones(CRecordContactDB *record, CContactItem* contact)
{
	CList* fieldItems = record->iWorkTelephones;
	for (TInt i = 0; i < record->iWorkTelephones->Count(); i++)
	{
		CContactItemField* workTelephoneField = CContactItemField::NewL(KStorageTypeText, KUidContactFieldPhoneNumber);
		CContactTextField* workTelephoneText = workTelephoneField->TextStorage();
	
		TPtr *data = (TPtr *)fieldItems->GetContent();
	
		if (data != NULL) {
			workTelephoneText->SetTextL(*data);
			workTelephoneField->SetMapping(KUidContactFieldVCardMapTEL);
			workTelephoneField->AddFieldTypeL(KUidContactFieldVCardMapWORK);
			//fieldSet.AddL(*workTelephoneField);
			contact->AddFieldL(*workTelephoneField);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CUpdateContactsAO::AddMobiles(CRecordContactDB *record, CContactItem* contact)
{
	// Mobile Numbers
	CList* fieldItems = record->iMobiles;
	for (TInt i = 0; i < record->iMobiles->Count(); i++)
	{
		CContactItemField* mobileField = CContactItemField::NewL(KStorageTypeText, KUidContactFieldPhoneNumber);
		CContactTextField* mobileText = mobileField->TextStorage();
	
		TPtr *data = (TPtr *)fieldItems->GetContent();
	
		if (data != NULL) {
			mobileText->SetTextL(*data);
			mobileField->SetMapping(KUidContactFieldVCardMapTEL);
			mobileField->AddFieldTypeL(KUidContactFieldVCardMapCELL);
			//fieldSet.AddL(*mobileField);
			contact->AddFieldL(*mobileField);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CUpdateContactsAO::AddHomeMobiles(CRecordContactDB *record, CContactItem* contact)
{
	CList *fieldItems = record->iHomeMobiles;
	for (TInt i = 0; i < record->iHomeMobiles->Count(); i++)
	{
		CContactItemField* homeMobileField = CContactItemField::NewL(KStorageTypeText, KUidContactFieldPhoneNumber);
		CContactTextField* homeMobileText = homeMobileField->TextStorage();
	
		TPtr *data = (TPtr *)fieldItems->GetContent();
	
		if (data != NULL) {
			homeMobileText->SetTextL(*data);
			homeMobileField->SetMapping(KUidContactFieldVCardMapTEL);
			homeMobileField->AddFieldTypeL(KUidContactFieldVCardMapCELL);
			homeMobileField->AddFieldTypeL(KUidContactFieldVCardMapHOME);
			//fieldSet.AddL(*homeMobileField);
			contact->AddFieldL(*homeMobileField);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CUpdateContactsAO::AddWorkMobiles(CRecordContactDB *record, CContactItem* contact)
{
	// Work Mobile Numbers
	CList* fieldItems = record->iWorkMobiles;
	for (TInt i = 0; i < record->iWorkMobiles->Count(); i++)
	{
		CContactItemField* workMobileField = CContactItemField::NewL(KStorageTypeText, KUidContactFieldPhoneNumber);
		CContactTextField* workMobileText = workMobileField->TextStorage();
	
		TPtr *data = (TPtr *)fieldItems->GetContent();
	
		if (data != NULL) {
			workMobileText->SetTextL(*data);
			workMobileField->SetMapping(KUidContactFieldVCardMapTEL);
			workMobileField->AddFieldTypeL(KUidContactFieldVCardMapCELL);
			workMobileField->AddFieldTypeL(KUidContactFieldVCardMapWORK);
			//fieldSet.AddL(*workMobileField);
			contact->AddFieldL(*workMobileField);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CUpdateContactsAO::AddFaxes(CRecordContactDB *record, CContactItem* contact)
{
	// Fax Numbers
	CList* fieldItems = record->iFaxes;
	for (TInt i = 0; i < record->iFaxes->Count(); i++)
	{
		CContactItemField* faxField = CContactItemField::NewL(KStorageTypeText, KUidContactFieldFax);
		CContactTextField* faxText = faxField->TextStorage();
	
		TPtr *data = (TPtr *)fieldItems->GetContent();
	
		if (data != NULL) {
			faxText->SetTextL(*data);
			faxField->SetMapping(KUidContactFieldVCardMapTEL);
			faxField->AddFieldTypeL(KUidContactFieldVCardMapFAX);
			//fieldSet.AddL(*faxField);
			contact->AddFieldL(*faxField);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CUpdateContactsAO::AddHomeFaxes(CRecordContactDB *record, CContactItem* contact)
{
	// Home Fax Numbers
	CList *fieldItems = record->iHomeFaxes;
	for (TInt i = 0; i < record->iHomeFaxes->Count(); i++)
	{
		CContactItemField* homeFaxField = CContactItemField::NewL(KStorageTypeText, KUidContactFieldFax);
		CContactTextField* homeFaxText = homeFaxField->TextStorage();
	
		TPtr *data = (TPtr *)fieldItems->GetContent();
	
		if (data != NULL) {
			homeFaxText->SetTextL(*data);
			homeFaxField->SetMapping(KUidContactFieldVCardMapTEL);
			homeFaxField->AddFieldTypeL(KUidContactFieldVCardMapFAX);
			homeFaxField->AddFieldTypeL(KUidContactFieldVCardMapHOME);
			//fieldSet.AddL(*homeFaxField);
			contact->AddFieldL(*homeFaxField);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CUpdateContactsAO::AddWorkFaxes(CRecordContactDB *record, CContactItem* contact)
{
	// Work Fax Numbers
	CList* fieldItems = record->iWorkFaxes;
	for (TInt i = 0; i < record->iWorkFaxes->Count(); i++)
	{
		CContactItemField* workFaxField = CContactItemField::NewL(KStorageTypeText, KUidContactFieldFax);
		CContactTextField* workFaxText = workFaxField->TextStorage();
	
		TPtr *data = (TPtr *)fieldItems->GetContent();
	
		if (data != NULL) {
			workFaxText->SetTextL(*data);
			workFaxField->SetMapping(KUidContactFieldVCardMapTEL);
			workFaxField->AddFieldTypeL(KUidContactFieldVCardMapFAX);
			workFaxField->AddFieldTypeL(KUidContactFieldVCardMapWORK);
			//fieldSet.AddL(*workFaxField);
			contact->AddFieldL(*workFaxField);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CUpdateContactsAO::AddEmails(CRecordContactDB *record, CContactItem* contact)
{
	// Emails
	CList* fieldItems = record->iEmails;
	for (TInt i = 0; i < record->iEmails->Count(); i++)
	{
		CContactItemField* emailField = CContactItemField::NewL(KStorageTypeText, KUidContactFieldEMail);
		CContactTextField* emailText = emailField->TextStorage();
	
		TPtr *data = (TPtr *)fieldItems->GetContent();
	
		if (data != NULL) {
			emailText->SetTextL(*data);
			emailField->SetMapping(KUidContactFieldVCardMapEMAILINTERNET);
			//fieldSet.AddL(*workFaxField);
			contact->AddFieldL(*emailField);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CUpdateContactsAO::AddHomeEmails(CRecordContactDB *record, CContactItem* contact)
{
	// Home Emails
	CList* fieldItems = record->iHomeEmails;
	for (TInt i = 0; i < record->iHomeEmails->Count(); i++)
	{
		CContactItemField* homeEmailField = CContactItemField::NewL(KStorageTypeText, KUidContactFieldEMail);
		CContactTextField* homeEmailText = homeEmailField->TextStorage();
	
		TPtr *data = (TPtr *)fieldItems->GetContent();
	
		if (data != NULL) {
			homeEmailText->SetTextL(*data);
			homeEmailField->SetMapping(KUidContactFieldVCardMapEMAILINTERNET);
			homeEmailField->AddFieldTypeL(KUidContactFieldVCardMapHOME);
			//fieldSet.AddL(*workFaxField);
			contact->AddFieldL(*homeEmailField);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CUpdateContactsAO::AddWorkEmails(CRecordContactDB *record, CContactItem* contact)
{
	// Work Emails
	CList* fieldItems = record->iWorkEmails;
	for (TInt i = 0; i < record->iWorkEmails->Count(); i++)
	{
		CContactItemField* workEmailField = CContactItemField::NewL(KStorageTypeText, KUidContactFieldEMail);
		CContactTextField* workEmailText = workEmailField->TextStorage();
	
		TPtr *data = (TPtr *)fieldItems->GetContent();
	
		if (data != NULL) {
			workEmailText->SetTextL(*data);
			workEmailField->SetMapping(KUidContactFieldVCardMapEMAILINTERNET);
			workEmailField->AddFieldTypeL(KUidContactFieldVCardMapWORK);
			//fieldSet.AddL(*workFaxField);
			contact->AddFieldL(*workEmailField);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CUpdateContactsAO::AddFirstName(CRecordContactDB *record, CContactItem* contact)
{
	TInt index;
	TBuf<256> contacts_buf;
	CContactItemFieldSet& fieldSet = contact->CardFields();			

	// First name
	index = fieldSet.Find(KUidContactFieldGivenName);
	
	if (index >= 0)
	{
		CContactItemField& firstNameField = fieldSet[index];
		CContactTextField* firstName = firstNameField.TextStorage();
	
		if (record->iFirstName != NULL) {
			contacts_buf.Copy(*(record->iFirstName));
			firstName->SetTextL(contacts_buf);
		}
	}
	else
	{
		CContactItemField* firstNameField = CContactItemField::NewLC(KStorageTypeText , KUidContactFieldGivenName);
		CContactTextField* firstName = firstNameField->TextStorage();
		firstNameField->SetMapping(KUidContactFieldVCardMapUnusedN);
	
		if (record->iFirstName != NULL)
		{
			contacts_buf.Copy(*(record->iFirstName));
			firstName->SetTextL(contacts_buf);
			contact->AddFieldL(*firstNameField);
		}
		CleanupStack::Pop();
	}
}

void CUpdateContactsAO::AddLastName(CRecordContactDB *record, CContactItem* contact)
{
	TInt index;
	TBuf<256> contacts_buf;
	CContactItemFieldSet& fieldSet = contact->CardFields();			

	// Last Name
	index = fieldSet.Find(KUidContactFieldFamilyName);
	
	if (index >= 0)
	{
		CContactItemField& lastNameField = fieldSet[index];
		CContactTextField* lastName = lastNameField.TextStorage();
	
		if (record->iLastName != NULL) {
			contacts_buf.Copy(*(record->iLastName));
			lastName->SetTextL(contacts_buf);
		}
	}
	else
	{
		CContactItemField* lastNameField = CContactItemField::NewLC(KStorageTypeText , KUidContactFieldFamilyName);
		CContactTextField* lastName = lastNameField->TextStorage();
		lastNameField->SetMapping(KUidContactFieldVCardMapUnusedN);
	
		if (record->iLastName != NULL)
		{
			contacts_buf.Copy(*(record->iLastName));
			lastName->SetTextL(contacts_buf);
			contact->AddFieldL(*lastNameField);
		}
		CleanupStack::Pop();
	}
}

// add the following fields
// firstname, lastname, company
void CUpdateContactsAO::AddCompanyName(CRecordContactDB *record, CContactItem* contact)
{
	TInt index;
	TBuf<256> contacts_buf;
	CContactItemFieldSet& fieldSet = contact->CardFields();			
	
	// Company Name
	index = fieldSet.Find(KUidContactFieldCompanyName);
	
	if (index >= 0)
	{
		CContactItemField& companyNameField = fieldSet[index];
		CContactTextField* companyName = companyNameField.TextStorage();
	
		if (record->iCompanyName != NULL) {
			contacts_buf.Copy(*(record->iCompanyName));
			companyName->SetTextL(contacts_buf);
		}
	}
	else
	{
		CContactItemField* companyNameField = CContactItemField::NewLC(KStorageTypeText , KUidContactFieldCompanyName);
		CContactTextField* companyName = companyNameField->TextStorage();
		companyNameField->SetMapping(KUidContactFieldVCardMapORG);
	
		if (record->iCompanyName != NULL)
		{
			contacts_buf.Copy(*(record->iCompanyName));
			companyName->SetTextL(contacts_buf);
			contact->AddFieldL(*companyNameField);
		}
		CleanupStack::Pop();
	}
}

void CUpdateContactsAO::AddJobTitles(CRecordContactDB *record, CContactItem* contact)
{
	// Job Titles
	CList* fieldItems = record->iJobTitles;
	for (TInt i = 0; i < record->iJobTitles->Count(); i++)
	{
		CContactItemField* jobTitleField = CContactItemField::NewL(KStorageTypeText, KUidContactFieldJobTitle);
		CContactTextField* jobTitleText = jobTitleField->TextStorage();
	
		TPtr *data = (TPtr *)fieldItems->GetContent();
	
		if (data != NULL) {
			jobTitleText->SetTextL(*data);
			jobTitleField->SetMapping(KUidContactFieldVCardMapTITLE);
			//fieldSet.AddL(*workFaxField);
			contact->AddFieldL(*jobTitleField);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CUpdateContactsAO::EndAcks()
{
	iSocketsEngine.iBuffer.Append(_L("</ack>"));

/*#ifndef WAP_PT
	HBufC8* content_data;
	content_data = iSocketsEngine.iBuffer.AllocL();

	TBuf8<10> content_l;
	content_l.Num(content_data->Length());
	iSocketsEngine.iBuffer.Copy(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	iSocketsEngine.iBuffer.Append(content_l);
	iSocketsEngine.iBuffer.Append(_L("\r\nCookie:"));
	iSocketsEngine.iBuffer.Append(iSessionId);
	iSocketsEngine.iBuffer.Append(_L("\r\n\r\n"));
	iSocketsEngine.iBuffer.Append(*content_data);
	delete content_data;
#endif
	iSocketsEngine.iBuffer.Append(_L("\r\n"));
*/
		
	iSocketsEngine.AcksToServer();
}

void CUpdateContactsAO::UpdateJobTitle(CRecordContactDB* record, CContactItem* contact)
{
	TInt index;
	CList *fieldItems;
	CContactItemFieldSet& fieldSet = contact->CardFields();			
	
	// Job Title
	index = fieldSet.Find(KUidContactFieldJobTitle);
	
	if (index >= 0)
	{
		CContactItemField& jobTitleField = fieldSet[index];
		CContactTextField* jobTitleName = jobTitleField.TextStorage();

		fieldItems = record->iJobTitles;
		if (fieldItems->Count())
		{
			TPtr *data = (TPtr *)fieldItems->GetContent();
			jobTitleName->SetTextL(*data);
		}
	}
	else
	{
		AddJobTitles(record, contact);
	}
}

void CUpdateContactsAO::UpdateEmail(CRecordContactDB* record, CContactItem* contact)
{
	TInt index;
	CList *fieldItems;
	CContactItemFieldSet& fieldSet = contact->CardFields();			
	
	// Emails
	TInt findfield = 0;
	for (int i = 0; i < fieldSet.Count(); i++)
	{
		index = fieldSet.FindNext(KUidContactFieldEMail, i);
		
		if (index >= 0)
		{
			CContactItemField& datafield = fieldSet[index];
						
			// home emails
			if (datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME) &&
				datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapEMAILINTERNET))
			{
				CContactTextField* text = datafield.TextStorage();
				fieldItems = record->iHomeEmails;
				if (fieldItems->Count())
				{
					TPtr *data = (TPtr *)fieldItems->GetContent();
					text->SetTextL(*data);
				}
				findfield = 1;
			}
			// work emails
			else if (datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK) &&
				datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapEMAILINTERNET))
			{
				CContactTextField* text = datafield.TextStorage();
				fieldItems = record->iWorkEmails;
				if (fieldItems->Count())
				{
					TPtr *data = (TPtr *)fieldItems->GetContent();
					text->SetTextL(*data);
				}
				findfield = 1;
			}
			// emails
			else if (!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK) &&
				!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME) &&
				datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapEMAILINTERNET))
			{
				CContactTextField* text = datafield.TextStorage();
				fieldItems = record->iEmails;
				if (fieldItems->Count())
				{
					TPtr *data = (TPtr *)fieldItems->GetContent();
					text->SetTextL(*data);
				}
				findfield = 1;
			}
		}
	} // for() mobiles, phones ...
	
	if (findfield == 0) // field not avlbl in fieldset
	{
		// add field to fieldset
		AddHomeEmails(record, contact);
		AddWorkEmails(record, contact);
		AddEmails(record, contact);
	}
}

void CUpdateContactsAO::UpdateFax(CRecordContactDB* record, CContactItem* contact)
{
	TInt index;
	CList *fieldItems;
	CContactItemFieldSet& fieldSet = contact->CardFields();			
	
	// Faxes
	TInt findfield = 0;
	for (int i = 0; i < fieldSet.Count(); i++)
	{
		index = fieldSet.FindNext(KUidContactFieldFax, i);
		
		if (index >= 0)
		{
			CContactItemField& datafield = fieldSet[index];
						
			// home emails
			if (datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME) &&
				datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapFAX))
			{
				CContactTextField* text = datafield.TextStorage();
				fieldItems = record->iHomeFaxes;
				if (fieldItems->Count())
				{
					TPtr *data = (TPtr *)fieldItems->GetContent();
					text->SetTextL(*data);
				}
				findfield = 1;
			}
			// work emails
			else if (datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK) &&
				datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapFAX))
			{
				CContactTextField* text = datafield.TextStorage();
				fieldItems = record->iWorkFaxes;
				if (fieldItems->Count())
				{
					TPtr *data = (TPtr *)fieldItems->GetContent();
					text->SetTextL(*data);
				}
				findfield = 1;
			}
			// emails
			else if (!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK) &&
				!datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME) &&
				datafield.ContentType().ContainsFieldType(KUidContactFieldVCardMapFAX))
			{
				CContactTextField* text = datafield.TextStorage();
				fieldItems = record->iFaxes;
				if (fieldItems->Count())
				{
					TPtr *data = (TPtr *)fieldItems->GetContent();
					text->SetTextL(*data);
				}
				findfield = 1;
			}
		}
	} // for() mobiles, phones ...
	
	if (findfield == 0) // field not avlbl in fieldset
	{
		// add field to fieldset
		AddHomeFaxes(record, contact);
		AddWorkFaxes(record, contact);
		AddFaxes(record, contact);
	}
}

void CUpdateContactsAO::UpdateTimeStamp()
{
	CContactDatabase *database = iAppUi.ContactDataBase();

	RFs fs;
	fs.Connect();

	TBuf<256> cdbFileName; // Contact db file name

	if (database->FindContactFile(cdbFileName)) {

		TTime cdbTime;
		fs.Modified(cdbFileName, cdbTime);

		TDateTime janNineteenSeventy
				(1970,EJanuary,0,00,00,00,000000);
		TTime timejan(janNineteenSeventy);

		TTimeIntervalMicroSeconds interval =
				cdbTime.MicroSecondsFrom(timejan);

		TInt64 cdbVal;
		cdbVal = interval.Int64()/1000;

		TBuf<256> timestamp;
		timestamp.Num(cdbVal);

		iAppUi.AddTimeStampToFileL(&timestamp);
	}

	fs.Close();
}