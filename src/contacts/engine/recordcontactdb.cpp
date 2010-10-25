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
 Module Name				: RecordContactDB.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Object to encapsulate contacts, defines all the supported fields like fname, lname, address, phone, mobile, etc.
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/


#include "RecordContactDB.h"
#include "List.h"

#include <cntfldst.h>
#include <cntfield.h>

CRecordContactDB::CRecordContactDB()
{
	iTimeStamp = NULL;
    iFirstName = NULL;
    iLastName = NULL;
	iCompanyName = NULL;
	iGlobalId = NULL;

	iJobTitles = new CList();

	iMobiles = new CList();
	iHomeMobiles = new CList();
	iWorkMobiles = new CList();

	iTelephones = new CList();
	iHomeTelephones = new CList();
	iWorkTelephones = new CList();

	iEmails = new CList();
	iHomeEmails = new CList();
	iWorkEmails = new CList();

	iFaxes = new CList();
	iHomeFaxes = new CList();
	iWorkFaxes = new CList();

	iIdList = new CList();
}

CRecordContactDB::~CRecordContactDB()
{
	delete iFirstName;
	delete iLastName;
	delete iCompanyName;
	delete iTimeStamp;
	delete iGlobalId;

	// if lists have allocated data, then Cleanup() shd 
	// have been called earlier
	if (iMobiles != NULL)
	{
		iJobTitles->RemoveAll();

		iMobiles->RemoveAll();
		iHomeMobiles->RemoveAll();
		iWorkMobiles->RemoveAll();

		iTelephones->RemoveAll();
		iHomeTelephones->RemoveAll();
		iWorkTelephones->RemoveAll();
		
		iEmails->RemoveAll();
		iHomeEmails->RemoveAll();
		iWorkEmails->RemoveAll();

		iFaxes->RemoveAll();
		iHomeFaxes->RemoveAll();
		iWorkFaxes->RemoveAll();
	}
	iIdList->RemoveAllAndContent();

}

void CRecordContactDB::Cleanup()
{
	iJobTitles->RemoveAllAndContent();

	iMobiles->RemoveAllAndContent();
	iHomeMobiles->RemoveAllAndContent();
	iWorkMobiles->RemoveAllAndContent();

	iTelephones->RemoveAllAndContent();
	iHomeTelephones->RemoveAllAndContent();
	iWorkTelephones->RemoveAllAndContent();
	
	iEmails->RemoveAllAndContent();
	iHomeEmails->RemoveAllAndContent();
	iWorkEmails->RemoveAllAndContent();

	iFaxes->RemoveAllAndContent();
	iHomeFaxes->RemoveAllAndContent();
	iWorkFaxes->RemoveAllAndContent();

	iMobiles = NULL;
}

void CRecordContactDB::GetAsString(CList *items, TBuf<256> *buffer)
{
	HBufC *itemBuf;

	buffer->Copy(_L(""));

	CList *ptr = items;
	for (int i = 0; i < items->Count(); i++)
	{
		CContactItemField* itemField = (CContactItemField *)ptr->GetContent();
		CContactTextField* itemValue = itemField->TextStorage();

		itemBuf = itemValue->Text().AllocLC();

		if (buffer->Length() + itemBuf->Length() >= 200) {
			CleanupStack::PopAndDestroy();
			break;
		}

		buffer->Append(*itemBuf);

		CleanupStack::PopAndDestroy();

		//if (i == (items->Count() - 1))
		//	break;

		buffer->Append(_L(","));

		ptr = ptr->next;
	}
}

void CRecordContactDB::GetIdsAsString(CList *items, TPtr buffer)
{
	buffer.Copy(_L(""));

	CList *ptr = items;
	for (int i = 0; i < items->Count(); i++)
	{
		TPtr *data = (TPtr *)ptr->GetContent();

		buffer.Append(*data);
		//if (i == (items->Count() - 1))
		//	break;
		buffer.Append(_L(","));

		ptr = ptr->next;
	}
}
