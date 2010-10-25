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


#include "ReadContactsAO.h"
#include "FindBoxContainer.h"
 
CReadContactsAO::CReadContactsAO(CFindBoxContainer& aAppView)
: CActive(EPriorityHigh),iAppView(aAppView)
{
	CActiveScheduler::Add(this);
}

CReadContactsAO::~CReadContactsAO()
{
	Cancel();
}

void CReadContactsAO::StartL()
{
	iFlag = 0;

	iPhoneContacts = CContactDatabase::OpenL();

	cArray = (CContactIdArray*)iPhoneContacts->SortedItemsL();
	 
	iCount = cArray->Count();
	
	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
}

void CReadContactsAO::RunL()
{
	if (iCount > 0)
	{	
		ReadContactL();
		
		iFlag++;

		iCount--;

		Notify();
	}
	else
	{
		cArray->Reset();
		
		delete iPhoneContacts;
		
		iAppView.CloseReadContacts();
	}	
}

void CReadContactsAO::DoCancel()
{
}

TInt CReadContactsAO::RunError(TInt aError)
{
	return KErrNone;
}

void CReadContactsAO::ReadContactL()
{
	TBuf<150> fname;	
	TBuf<150> lname;	
	TBuf<150> mobile;
	TBuf<50> mobile1;
	TBuf<50> mobile2;
	TBuf<50> mobile3;
	TBuf<50> mobile4;
	TBuf<50> mobile5;
	TBuf<50> mobile6;

	CContactItem* contact;

	TInt cid=(*cArray)[iFlag];

	contact = iPhoneContacts->ReadContactL(cid);
	
	mobile.Copy(_L(""));
	mobile1.Copy(_L(""));
	mobile2.Copy(_L(""));
	mobile3.Copy(_L(""));
	mobile4.Copy(_L(""));
	mobile5.Copy(_L(""));
	mobile6.Copy(_L(""));

	CContactItemFieldSet& fieldSet = contact->CardFields();		   

	for (TInt j = 0; j < fieldSet.Count(); j++) 
	{				
		// Home mobile nos
		if (fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber)
			&& fieldSet[j].TextStorage()->Text().Length() > 0 
			&& fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME)
			&& fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) 
		{
			CContactItemField& textfield = fieldSet[j];
			CContactTextField* textValue = textfield.TextStorage();
			((TDes&)mobile2).Copy(textValue->Text());

			if(mobile2.Length() != 0)
			{
				mobile.Append(mobile2);
				mobile.Append(_L("\n"));
			}
			continue;
		}				
		// Work mobile nos
		if (fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[j].TextStorage()->Text().Length() > 0 
			&& fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK)
			&& fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) 
		{
			CContactItemField& textfield = fieldSet[j];
			CContactTextField* textValue = textfield.TextStorage();
			((TDes&)mobile3).Copy(textValue->Text());

			if(mobile3.Length() != 0)
			{
				mobile.Append(mobile3);
				mobile.Append(_L("\n"));
			}
			continue;
		}								
		// Mobile nos
		if (fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[j].TextStorage()->Text().Length() > 0 
			&& fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) 
		{
			CContactItemField& textfield = fieldSet[j];
			CContactTextField* textValue = textfield.TextStorage();
			((TDes&)mobile1).Copy(textValue->Text());	

			if(mobile1.Length() != 0)
			{
				mobile.Append(mobile1);
				mobile.Append(_L("\n"));
			}		
			continue;
		}
		// Home Telephone nos
		if (fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[j].TextStorage()->Text().Length() > 0 
			&& fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME)
			&& fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) 
		{
			CContactItemField& textfield = fieldSet[j];
			CContactTextField* textValue = textfield.TextStorage();
			((TDes&)mobile5).Copy(textValue->Text());	

			if(mobile5.Length() != 0)
			{
				mobile.Append(mobile5);
				mobile.Append(_L("\n"));
			}			
			continue;
		}
		// Work Telephone nos
		if (fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[j].TextStorage()->Text().Length() > 0 
			&& fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK)
			&& fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) 
		{
			CContactItemField& textfield = fieldSet[j];
			CContactTextField* textValue = textfield.TextStorage();
			((TDes&)mobile6).Copy(textValue->Text());	

			if(mobile6.Length() != 0)
			{
				mobile.Append(mobile6);
				mobile.Append(_L("\n"));
			}			
			continue;
		}
		// Telephones
		if (fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[j].TextStorage()->Text().Length() > 0 
			&& fieldSet[j].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) 
		{
			CContactItemField& textfield = fieldSet[j];
			CContactTextField* textValue = textfield.TextStorage();
			((TDes&)mobile4).Copy(textValue->Text());

			if(mobile4.Length() != 0)
			{
				mobile.Append(mobile4);
				mobile.Append(_L("\n"));
			}				
			continue;
		}
	}

	if(mobile.Length() != 0) 
	{
		GetTextFieldFromFieldSet(fieldSet, KUidContactFieldGivenName, fname);

		GetTextFieldFromFieldSet(fieldSet, KUidContactFieldFamilyName, lname);
		
		if(lname.Compare(_L("")) !=0 )
		{
			lname.Append(_L(" "));
			fname.Insert(0,lname);
		}
		
		iAppView.iContactNames->AppendL(fname);

		iAppView.iMobiles->AppendL(mobile);
		
		iAppView.iContactCount++;

		iAppView.SetFileList(3);
	}

	delete contact;
}

void CReadContactsAO::Notify()
{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	if (!IsActive())
    SetActive();
}

void CReadContactsAO::GetTextFieldFromFieldSet(CContactItemFieldSet& aFieldSet, const TUid aField,TDesC& name)
{
	TInt index = aFieldSet.Find(aField);

    if ((index >= 0) && (index < aFieldSet.Count()))
    {
   	    CContactItemField& textfield = aFieldSet[index];
        CContactTextField* textValue = textfield.TextStorage();

		((TDes&)name).Copy(textValue->Text());		
    } 
}

