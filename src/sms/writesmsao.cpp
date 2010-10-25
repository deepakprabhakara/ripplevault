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
 Module Name				: WriteSmsAO.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Active Object for updating Sms on the phone
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/


#include "WriteSmsAO.h"
#include "Constants.h"
//#include <ripplevault.rsg>
#include <utf.h>


CWriteSmsAO::CWriteSmsAO(CRippleVaultAppUi& aAppUi)
: CActive(EPriorityStandard),iAppUi(aAppUi)
{
	CActiveScheduler::Add(this);
}

CWriteSmsAO::~CWriteSmsAO()
{
	Cancel();
}

void CWriteSmsAO::StartL(TInt aInboxCount, TInt aSentItemCount, CArrayFix<TInt>* aInboxSelectedSms, CArrayFix<TInt>* aSentItemsSelectedSms, CDesCArray* aInboxSmsDatearray, CDesCArray* aSentSmsDatearray, CDesCArray* aInboxSmsDetailsarray, CDesCArray* aSentSmsDetailsarray, CDesCArray* aInboxSmsMessagearray, CDesCArray* aSentSmsMessagearray)
{	
	iInboxCount = aInboxCount;
	iSentItemCount = aSentItemCount;

	TInt count;
	if(iInboxCount == 0)
		count = 1;
	else
		count = iInboxCount;

	iInboxSelectedSms = new CArrayFixFlat<TInt>(count);
	InboxSmsDatearray = new (ELeave) CDesCArrayFlat(count);
	InboxSmsMessagearray = new (ELeave) CDesCArrayFlat(count);
	InboxSmsDetailsarray = new (ELeave) CDesCArrayFlat(count);

	TInt j;
	for (j=0;j<iInboxCount; ++j){
		iInboxSelectedSms->AppendL(aInboxSelectedSms->At(j));
		InboxSmsDatearray->InsertL(j,(*aInboxSmsDatearray)[j]);
		InboxSmsMessagearray->InsertL(j,(*aInboxSmsMessagearray)[j]);
		InboxSmsDetailsarray->InsertL(j,(*aInboxSmsDetailsarray)[j]);
	}

	if(iSentItemCount == 0)
		count = 1;
	else
		count = iSentItemCount;

	iSentItemsSelectedSms = new CArrayFixFlat<TInt>(count);
	SentSmsDatearray = new (ELeave) CDesCArrayFlat(count);
	SentSmsMessagearray = new (ELeave) CDesCArrayFlat(count);
	SentSmsDetailsarray = new (ELeave) CDesCArrayFlat(count);

	for (j=0;j<iSentItemCount; ++j){
		iSentItemsSelectedSms->AppendL(aSentItemsSelectedSms->At(j));
		SentSmsDatearray->InsertL(j,(*aSentSmsDatearray)[j]);
		SentSmsMessagearray->InsertL(j,(*aSentSmsMessagearray)[j]);
		SentSmsDetailsarray->InsertL(j,(*aSentSmsDetailsarray)[j]);
	}

/*	TInt8 choice;
	iAppUi.GetChoice(choice);
	if(!choice)
			{
		choice = 1;
		iAppUi.SaveChoice(choice);
		iAppUi.ActivateView(KViewIdMainScreen);	
			}
			else
			{
		iAppUi.ActivateView(KViewIdEmptyScreen);
		iAppUi.ActivateView(KViewIdStatusScreen);
			}
*/
	/*iWaitDialog = NULL;
	
    if(iWaitDialog == NULL)
	{
		iWaitDialog = new(ELeave)CAknWaitDialog( (REINTERPRET_CAST(CEikDialog**,&iWaitDialog)));
		iWaitDialog->SetTone( CAknNoteDialog::EConfirmationTone );
		iWaitDialog->SetTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SMS_BACKUP_PROGRESS)));
		
        iWaitDialog->ExecuteLD(R_WAITNOTE_SCAN_DIALOG);
	}*/

	CnvUtfConverter converter;
	TBuf8<500> data;
	TBuf<500> tempdata;

	iFileName = CRippleVaultAppUi::ApplicationDriveAndPath();
	iFileName.Append(_L("smsbackup.man"));
	
	fs.Connect();
	
	iFile.Replace(fs, iFileName, EFileWrite | EFileStream);

	tempdata.Copy(_L("<root><h><msgt>smsbackup</msgt><msg></msg><uid>"));
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
	
	tempdata.Copy(iAppUi.iUserNames);
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

	tempdata.Copy(_L("</uid><hp>"));
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
	
	tempdata.Copy(iAppUi.iMobileNo);
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

	tempdata.Copy(_L("</hp><did>"));
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

    tempdata.Copy(iAppUi.PhoneImei()); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
	
	tempdata.Copy(_L("</did><currec></currec><totrec></totrec><loc>en_us</loc><ctime></ctime><lstime>0</lstime><synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype></h><b>"));
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
}

void CWriteSmsAO::RunL()
{
	/*if(iAppView.iFlag == 1)
	{
		Notify();
	}
	else */
	if (iInboxCount > 0)
	{	
		iInboxCount--;

		if(iInboxSelectedSms->At(iInboxCount) != ITEM_SELECTED)
			WriteSmsToFile(iInboxCount, SMS_INBOX);

		Notify();
	}
	else if (iSentItemCount > 0)
	{	
		iSentItemCount--;
	
		if(iSentItemsSelectedSms->At(iSentItemCount) != ITEM_SELECTED)
			WriteSmsToFile(iSentItemCount, SMS_SENTITEMS);

		Notify();
	}
	else
	{			
		CnvUtfConverter converter;
		TBuf8<50> data;
		TBuf<50> tempdata;

		tempdata.Copy(_L("</b></root>"));
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		iFile.Write(data);

		iFile.Close();
		fs.Close();
		
		/*if (iWaitDialog != NULL)
		{
			iWaitDialog->ProcessFinishedL();
			iWaitDialog = NULL;
		}*/

		//iAppUi.CloseWriteSms();
		TInt smsbackuptype = 0;
		iAppUi.UploadSmsManually(smsbackuptype);

		//delete this;

	}	
}

void CWriteSmsAO::DoCancel()
{
}

TInt CWriteSmsAO::RunError(TInt aError)
{
	return KErrNone;
}

void CWriteSmsAO::Notify()
{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);

	if (!IsActive())
		SetActive();
}

void CWriteSmsAO::WriteSmsToFile(TInt aId,TInt aSmsState)
{
	CnvUtfConverter converter;
	TBuf8<2000> data;
	TBuf<500> tempdata;

	tempdata.Copy(_L("<sms>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

	tempdata.Copy(_L("<folder>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
	
	if( aSmsState == SMS_INBOX)
	{
		converter.ConvertFromUnicodeToUtf8(data,_L("Inbox"));		
		iFile.Write(data);
	}
	else if( aSmsState == SMS_SENTITEMS)
	{
		converter.ConvertFromUnicodeToUtf8(data,_L("SentItems"));		
		iFile.Write(data);
	}

	tempdata.Copy(_L("</folder>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
//from address
	tempdata.Copy(_L("<from>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
	
	iFile.Write(_L8("<![CDATA["));
				
	if( aSmsState == SMS_INBOX)
	{
		converter.ConvertFromUnicodeToUtf8(data,(*InboxSmsDetailsarray)[aId]);
		iFile.Write(data);
	}
	else if( aSmsState == SMS_SENTITEMS)
	{
		converter.ConvertFromUnicodeToUtf8(data,iAppUi.iMobileNo);
		iFile.Write(data);
	}

	iFile.Write(_L8("]]>"));

	tempdata.Copy(_L("</from>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

//to address
	tempdata.Copy(_L("<to>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

	iFile.Write(_L8("<![CDATA["));	
	
	if( aSmsState == SMS_INBOX)
	{
		converter.ConvertFromUnicodeToUtf8(data,iAppUi.iMobileNo);
		iFile.Write(data);
	}
	else if( aSmsState == SMS_SENTITEMS)
	{
		converter.ConvertFromUnicodeToUtf8(data,(*SentSmsDetailsarray)[aId]);
		iFile.Write(data);
	}

	iFile.Write(_L8("]]>"));

	tempdata.Copy(_L("</to>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

//sms time
	tempdata.Copy(_L("<time>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
			
	//message.Copy(_L(""));
	
	if( aSmsState == SMS_INBOX)
		converter.ConvertFromUnicodeToUtf8(data,(*InboxSmsDatearray)[aId]);
	else if( aSmsState == SMS_SENTITEMS)
		converter.ConvertFromUnicodeToUtf8(data,(*SentSmsDatearray)[aId]);

	iFile.Write(data);

	tempdata.Copy(_L("</time>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

//sms text
	tempdata.Copy(_L("<text>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);
				
	TBuf<2000> message;
	message.Copy(_L("<![CDATA["));
	
	if( aSmsState == SMS_INBOX)
		message.Append((*InboxSmsMessagearray)[aId]);
	else if( aSmsState == SMS_SENTITEMS)
		message.Append((*SentSmsMessagearray)[aId]);
	
	message.Append(_L("]]>"));

	converter.ConvertFromUnicodeToUtf8(data,message);
	iFile.Write(data);

	tempdata.Copy(_L("</text>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);

	tempdata.Copy(_L("</sms>")); 
	converter.ConvertFromUnicodeToUtf8(data,tempdata);
	iFile.Write(data);			
}
