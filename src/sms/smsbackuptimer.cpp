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
 Module Name				: SmsBackupTimer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :      Timer for SMS backup ie. Scheduler
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/


#include <eikgted.h>
#include <eikmenup.h>
#include <eikgted.h>
#include <eikenv.h>
#include <bautils.h>
#include <badesca.h> 
#include <msvstd.h> 
#include <mtclbase.h> 
#include <msvids.h> 
#include <smut.h> 
#include <smuthdr.h> 
#include <mtclreg.h> 
#include <txtrich.h> 
#include <smsclnt.h>
#include <gsmupdu.h>
#include <utf.h>
#include <f32file.h>

#ifndef __UIQ__
	#include <avkon.hrh>
	#include <RippleVault.rsg>
#else
#ifdef __S80__
	#include  <ripplevault_series80.rsg>
#else
	#include  <ripplevault_uiq.rsg>
#endif
#endif

#include "SmsBackuptimer.h"
#include "timeouttimer.h"

#include "constants.h"

const TInt CSmsBackupTimer::KTimeOut = 60000000; // 60 seconds time-out

class CRippleVaultAppUi;

CSmsBackupTimer* CSmsBackupTimer::NewL(CRippleVaultAppUi& aAppUi)
{
	CSmsBackupTimer* self = CSmsBackupTimer::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
}

	
CSmsBackupTimer* CSmsBackupTimer::NewLC(CRippleVaultAppUi& aAppUi)
{
	CSmsBackupTimer* self = new (ELeave) CSmsBackupTimer(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CSmsBackupTimer::CSmsBackupTimer(CRippleVaultAppUi& aAppUi) 
: CActive(EPriorityIdle),
iAppUi(aAppUi)
{
}


CSmsBackupTimer::~CSmsBackupTimer()
{
    Cancel();
	delete iTimer;
}

void CSmsBackupTimer::ConstructL()
{
	iFileName = CRippleVaultAppUi::ApplicationDriveAndPath();
	iFileName.Append(_L("smsbackup.aut"));

	iCount=0;
	iIndex = 0;
	iEngineStatus = EIdle;
	iSmsUploadType = 0;
	iSmsFolder = 0;

	//iTimer = CTimeOutTimer::NewL(EPriorityHigh, *this);
	iTimer = CTimeOutTimer::NewL(EPriorityIdle, *this);
    	
	CActiveScheduler::Add(this); 
}

void CSmsBackupTimer::Start()
{
	iTimer->After(KTimeOut * 1);	
}

// from CActive
void CSmsBackupTimer::DoCancel()
{
	iTimer->Cancel();
}

// from CActive
void CSmsBackupTimer::RunL()
{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("RunL\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	switch(iEngineStatus)
	{
		case EProcess:
			{
#ifdef __LOGME__
				iAppUi.LogText.Copy(_L8("EProcess\n"));
				iAppUi.WriteLogFile(iAppUi.LogText);
#endif

				SetActive();
			}
			break;
		case EIdle:
			{	
#ifdef __LOGME__
				iAppUi.LogText.Copy(_L8("EIdle\n"));
				iAppUi.WriteLogFile(iAppUi.LogText);
#endif

				SetActive();
			}
			break;
		case EBuildInboxXmlData:
			{
#ifdef __LOGME__
				iAppUi.LogText.Copy(_L8("EBuildInboxXmlData\n"));
				iAppUi.WriteLogFile(iAppUi.LogText);
#endif

				Notify();

				//smsval=0;//Inbox
				if(iIndex <= iEntries->Count() - 1)
					GetSms(0, iSmsUploadType);
				else
				{
					iSmsFolder = 1;
					iIndex = 0;
					ChangeStatus(EBuildFinalData);
				}
			}
			break;
		case EBuildSentItemXmlData:
			{
#ifdef __LOGME__
				iAppUi.LogText.Copy(_L8("EBuildSentItemXmlData\n"));
				iAppUi.WriteLogFile(iAppUi.LogText);
#endif

				Notify();

				//smsval=1;//Sent items
				if(iIndex <= iEntries->Count() - 1)
					GetSms(1, iSmsUploadType);
				else
					ChangeStatus(EBuildFinalData);
			}
			break;
		case EBuildFinalData:
			{
				
				ReleaseMtm();

#ifdef __LOGME__
				iAppUi.LogText.Copy(_L8("EBuildFinalData\n"));
				iAppUi.WriteLogFile(iAppUi.LogText);
#endif

				TBuf<500> tempdata;
				TBuf8<500> data;
				CnvUtfConverter converter;

				tempdata.Copy(_L("</b></root>"));
				converter.ConvertFromUnicodeToUtf8(data,tempdata);
				//iFile.Write(_L8("</b></root>"));		
				iFile.Write(data);

				iFile.Close();
				fs.Close();

				//_LIT8(KPlus,"<sms>");//check if no sms then connect
				//TInt pos = iAppUi.smsBuffer.Find(KPlus);
				if(iCount)//pos != KErrNotFound)
				{
					ChangeStatus(EProcess);

					TInt smsbackuptype = 1;

					if(iSmsUploadType == 0)
					{
						smsbackuptype = 2;
					}
					else if(iSmsUploadType == 5)
					{
						smsbackuptype = 5;
					}

					iAppUi.GetSmsXML(smsbackuptype);
					
					/*TInt network;	
					network = iAppUi.CheckNetwork();

					if(network == NETWORK_ROAMING)
					{
						if(iAppUi.iSmsRoam == ENABLED)
						{
							iAppUi.GetSmsXML(smsbackuptype);
						}
					}
					else
						iAppUi.GetSmsXML(smsbackuptype);*/
				}
				else
				{
					ChangeStatus(EIdle);
				}
							
			}
			break;

		default:
			break;
	}
}

void CSmsBackupTimer::TimerExpired()
{
	//User::CompressAllHeaps();

	DoCancel();

	TInt val=0;

	TInt network;	
	network = iAppUi.CheckNetwork();

	TTime smstime;
	TTime hometime;
	TDateTime smsdatetime;
	TDateTime homedatetime;

	TInt smsuploadtype, smsroam;
	iAppUi.GetSmsSettingsFromDB(smsuploadtype, smstime, smsroam);


	hometime.HomeTime();

	smsdatetime = smstime.DateTime();
	homedatetime = hometime.DateTime();

	//iAppUi.WriteLogFile(_L8("timerexpired\n"));
	if(iAppUi.iSmsMode != SMSBACKUP_MANUAL)
	{
		if(network == NETWORK_ROAMING)
			{
			if(iAppUi.iSmsRoam == ENABLED)
				{
						if(iAppUi.iSmsMode == SMSBACKUP_SCHEDULED || smsuploadtype == SMSBACKUP_SELECTIVE)
						{
							if( (smsdatetime.Hour() == homedatetime.Hour()) && (smsdatetime.Minute() == homedatetime.Minute()))
								val = 1;
						}
						else if(iAppUi.iSmsMode == SMSBACKUP_SCHEDULED_HOURLY)
						{
							if(homedatetime.Minute() == 0)
								val = 1;
						}
					//iAppUi.WriteLogFile(_L8("if\n"));
						if(val == 1)
						{
							iIndex = 0;
							UploadSms(val);
						}
				}
			}
		else
			{
						if(smsuploadtype == SMSBACKUP_SCHEDULED || smsuploadtype == SMSBACKUP_SELECTIVE)
						{
							if( (smsdatetime.Hour() == homedatetime.Hour()) && (smsdatetime.Minute() == homedatetime.Minute()))
								val = 1;
						}
						else if(smsuploadtype == SMSBACKUP_SCHEDULED_HOURLY)
						{
							if(homedatetime.Minute() == 0)
								val = 1;
						}
					//iAppUi.WriteLogFile(_L8("if\n"));
						if(val == 1)
						{
							iIndex = 0;
							UploadSms(val);
						}

			}
	}
	//iAppUi.WriteLogFile(_L8("donetimerexpired\n"));
//	UploadSms(val);
	//CEikonEnv::Static ()->AlertWin(_L("123"));

	iTimer->After(KTimeOut * 1);	
}

void CSmsBackupTimer::ChangeStatus(TWaitState aNewStatus)
{
    iEngineStatus = aNewStatus;
}

void CSmsBackupTimer::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* /*aArg3*/)
{
}

void CSmsBackupTimer::UploadSms(TInt val)
{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("\nUploadSms="));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	if(val == 1)
	{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L8("\nval=1\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	}

	if(iEngineStatus == EIdle)
	{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L8("EIdle\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif
	}
	else
	{
	}

	if(val == 1 && iEngineStatus != EIdle)
		return;

	iCount = 0;
	//iAppUi.WriteLogFile(_L8("UploadSms\n"));

	TInt smsval;
//	TInt minute;
	CnvUtfConverter converter;
	TBuf8<500> data;
	TBuf<500> tempdata;

	smsval = 0;

	TInt smsuploadtype;
	smsuploadtype = iAppUi.iSmsMode;

	if(val == 3)
		val = 1;

	if(val == 2)
	{
		val = 1;
		smsuploadtype = 5;
	}

	if(val == 1)
	{
		//iAppUi.WriteLogFile(_L8("val=1\n"));

		iAppUi.iAutoSmsCount=0;

		fs.Connect();
		iFile.Replace(fs, iFileName, EFileWrite | EFileStream);

		tempdata.Copy(_L("<root><h><msgt>smsbackup</msgt><msg></msg><uid>"));
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		//iFile.Write(_L8("<root><h><msgt>smsbackup</msgt><msg></msg><uid>"));
		iFile.Write(data);

		//TBuf8<50> temp;
		//temp.Copy(iAppUi.iUserNames);
		tempdata.Copy(iAppUi.iUserNames);
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		//iFile.Write(temp);//(iAppUi.iUserNames);
		iFile.Write(data);

		tempdata.Copy(_L("</uid><hp>"));
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		//iFile.Write(_L8("</uid><hp>"));
		iFile.Write(data);
		
		//temp.Copy(iAppUi.iMobileNo);
		tempdata.Copy(iAppUi.iMobileNo);
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		//iFile.Write(temp);//iAppUi.iMobileNo);	
		iFile.Write(data);

		tempdata.Copy(_L("</hp><did>"));
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		//iFile.Write(_L8("</hp><did>"));
		iFile.Write(data);

        //temp.Copy(iAppUi.PhoneImei()); 
		tempdata.Copy(iAppUi.PhoneImei()); 
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		//iFile.Write(temp);//iAppUi.PhoneImei());	
		iFile.Write(data);
		
		tempdata.Copy(_L("</did><currec></currec><totrec></totrec><loc>en_us</loc><ctime></ctime><lstime>0</lstime><synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype></h><b>"));
		converter.ConvertFromUnicodeToUtf8(data,tempdata);
		//iFile.Write(_L8("</did><currec></currec><totrec></totrec><loc>en_us</loc><ctime></ctime><lstime>0</lstime><synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype></h><b>"));
		iFile.Write(data);

		if(iSmsFolder == 0)
			ChangeStatus(EBuildInboxXmlData);
		else
			ChangeStatus(EBuildSentItemXmlData);

		PrepareMtm(iSmsFolder);

		if(iEntries->Count() == 0)
		{
			if(iSmsFolder == 0)
			{
				iSmsFolder = 1;
				ChangeStatus(EBuildSentItemXmlData);
				ReleaseMtm();
				PrepareMtm(iSmsFolder);
			}
			else
			{
				ChangeStatus(EBuildFinalData);
			}
		}

		if(iIndex < iEntries->Count())
		{
			Notify();

			//smsval=0;//Inbox
			iSmsUploadType = smsuploadtype;
			GetSms(iSmsFolder, smsuploadtype);
		}
		else
		{
			ChangeStatus(EBuildFinalData);
			
			Notify();
		}
	}

#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("/UploadSms\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
}

void CSmsBackupTimer::GetSms(TInt smsval,TInt smsuploadtype)
{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("GetSms 1\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	CnvUtfConverter converter;
	//TMsvId smstype;	
	TBuf8<2000> data;
	TInt smsflag;
	TBuf<500> tempdata;

	TTime time;

	smsflag = 0;

	TInt ilen;
	// go through all entries in the Inbox
	//for(TInt i = iIndex; i < entries->Count(); i++)
	{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("iindex="));
	iAppUi.LogText.AppendNum(iIndex);
	iAppUi.LogText.Append(_L8("\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		TMsvId id = iEntries->At(iIndex);
		CMsvEntry* entry = iSession->GetEntryL(id);
		TMsvEntry msvEntry = entry->Entry();

#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("GetSms 2\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		iIndex++;

		CSmsClientMtm* smsMtm = STATIC_CAST(CSmsClientMtm*, iMtmReg->NewMtmL(msvEntry.iMtm));
		
		smsMtm->SwitchCurrentEntryL(id);
		TUid type = smsMtm->Type();
		
		if(type == KUidMsgTypeSMS)
		{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("GetSms 3\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

			smsMtm->LoadMessageL();
			CSmsHeader& smsHeader = smsMtm->SmsHeader();
			//here we are getting the size of SMS
			ilen=smsMtm->Body().Read(0).Length();

			TTime timestamp;
			if(smsval == 0)
				iAppUi.GetInboxSmsTimestamp(timestamp);
			else 
				iAppUi.GetSentItemsSmsTimestamp(timestamp);

			TTime smstime;
			smstime = smsHeader.Message().Time();

#ifdef __LOGME__
		TBuf<160> message;
		
		TDateTime aDateTime = smstime.DateTime();		
		_LIT(KFormatTxt,"%d/%d/%d %d:%d:%d ");		

		TBuf<2> ampm;		
		TInt hour = aDateTime.Hour();
		if(hour>12) 
		{
			hour=hour-12;
			ampm.Copy(_L("pm"));
		}
		else
		{
			ampm.Copy(_L("am"));
		}

		message.Copy(_L(""));

		message.Format(KFormatTxt, aDateTime.Day()+1, TInt(aDateTime.Month()+1), 
			aDateTime.Year(), hour, aDateTime.Minute(), aDateTime.Second());

		message.Append(ampm);

		iAppUi.LogText.Copy(_L("Sms Time="));
		iAppUi.LogText.Append(message);
		iAppUi.LogText.Append(_L("\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);

		aDateTime = timestamp.DateTime();		
		//_LIT(KFormatTxt,"%d/%d/%d %d:%d:%d ");		

		//TBuf<2> ampm;		
		hour = aDateTime.Hour();
		if(hour>12) 
		{
			hour=hour-12;
			ampm.Copy(_L("pm"));
		}
		else
		{
			ampm.Copy(_L("am"));
		}

		message.Copy(_L(""));

		message.Format(KFormatTxt, aDateTime.Day()+1, TInt(aDateTime.Month()+1), 
			aDateTime.Year(), hour, aDateTime.Minute(), aDateTime.Second());

		message.Append(ampm);

		iAppUi.LogText.Copy(_L("Sync Time="));
		iAppUi.LogText.Append(message);
		iAppUi.LogText.Append(_L("\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

			if(smstime > timestamp)
			{
				if(smsuploadtype == 1 || smsuploadtype == 2 || smsuploadtype == 5)
					smsflag = 1;
				else
				{
					TBuf<30> num;				
					
					num.Copy(smsHeader.FromAddress());					
						
					smsflag = CheckSmsNumber(num);
				}

				if(smsflag == 1)
				{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("GetSms 4\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

					iAppUi.iAutoSmsCount++;
					//iAppUi.UpdateScreen();

					//folder type
					//iFile.Write(_L8("<sms>"));
					tempdata.Copy(_L("<sms>")); 
					converter.ConvertFromUnicodeToUtf8(data,tempdata);
					iFile.Write(data);

					iCount=1;
					//iFile.Write(_L8("<folder>"));
					tempdata.Copy(_L("<folder>")); 
					converter.ConvertFromUnicodeToUtf8(data,tempdata);
					iFile.Write(data);
					
					if( smsval == 0)
					{
						converter.ConvertFromUnicodeToUtf8(data,_L("Inbox"));
						iFile.Write(data);
					}
					else if( smsval == 1)
					{
						converter.ConvertFromUnicodeToUtf8(data,_L("SentItems"));
						iFile.Write(data);
					}

					//iFile.Write(_L8("</folder>"));
					tempdata.Copy(_L("</folder>")); 
					converter.ConvertFromUnicodeToUtf8(data,tempdata);
					iFile.Write(data);
					//from address
					//iFile.Write(_L8("<from>"));
					tempdata.Copy(_L("<from>")); 
					converter.ConvertFromUnicodeToUtf8(data,tempdata);
					iFile.Write(data);

					iFile.Write(_L8("<![CDATA["));

					if( smsval == 0)
					{
						converter.ConvertFromUnicodeToUtf8(data,smsHeader.FromAddress());
						iFile.Write(data);
					}
					else if( smsval == 1)
					{
						converter.ConvertFromUnicodeToUtf8(data,iAppUi.iMobileNo);
						iFile.Write(data);
					}
					iFile.Write(_L8("]]>"));

					//iFile.Write(_L8("</from>"));
					tempdata.Copy(_L("</from>")); 
					converter.ConvertFromUnicodeToUtf8(data,tempdata);
					iFile.Write(data);
					
					//to address
					//iFile.Write(_L8("<to>"));
					tempdata.Copy(_L("<to>")); 
					converter.ConvertFromUnicodeToUtf8(data,tempdata);
					iFile.Write(data);
					
					iFile.Write(_L8("<![CDATA["));

					if( smsval == 0)
					{
						converter.ConvertFromUnicodeToUtf8(data,iAppUi.iMobileNo);
						iFile.Write(data);
					}
					else if( smsval == 1)
					{
						converter.ConvertFromUnicodeToUtf8(data,smsHeader.FromAddress());
						iFile.Write(data);
					}
					iFile.Write(_L8("]]>"));
					//iFile.Write(_L8("</to>"));
					tempdata.Copy(_L("</to>")); 
					converter.ConvertFromUnicodeToUtf8(data,tempdata);
					iFile.Write(data);

					//sms time
					//iFile.Write(_L8("<time>"));
					tempdata.Copy(_L("<time>")); 
					converter.ConvertFromUnicodeToUtf8(data,tempdata);
					iFile.Write(data);
					
					TBuf<500> message1;
					message1.Copy(_L(""));

					time = smsHeader.Message().Time();

					TDateTime aDateTime = time.DateTime();
					//_LIT(KFormatTxt,"%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S%:3%+B");
					_LIT(KFormatTxt,"%d/%d/%d %d:%d:%d ");
					//dd/mm/yyyy hh:mm:ss am

					TBuf<2> ampm;
					//time.FormatL(message, KFormatTxt);
					TInt hour = aDateTime.Hour();
					if(hour>12) {
						hour=hour-12;
						ampm.Copy(_L("pm"));
					}
					else{
						ampm.Copy(_L("am"));
					}

					message1.Format(KFormatTxt,
					   aDateTime.Day()+1,
					   TInt(aDateTime.Month()+1), 
						   // Format the month as a TInt to preserve locale independence
					   aDateTime.Year(), 
						   // Day and month ranges begin at zero (0-30 and 0-11), 
						   // so add one when formatting
					   hour, aDateTime.Minute(), aDateTime.Second());

					message1.Append(ampm);

					converter.ConvertFromUnicodeToUtf8(data,message1);
					iFile.Write(data);

					//iFile.Write(_L8("</time>"));
					tempdata.Copy(_L("</time>")); 
					converter.ConvertFromUnicodeToUtf8(data,tempdata);
					iFile.Write(data);

					//sms text
					//iFile.Write(_L8("<text>"));
					tempdata.Copy(_L("<text>")); 
					converter.ConvertFromUnicodeToUtf8(data,tempdata);
					iFile.Write(data);
					
					TBuf<2000> message;
					smsMtm->Body().Extract(message,0);
					
					message.Insert(0,_L("<![CDATA["));					
					message.Append(_L("]]>"));
					
					/*message.Copy(_L("<![CDATA["));
					message.Append(smsMtm->Body().Read(0,ilen-1));
					message.Append(_L("]]>"));*/

					converter.ConvertFromUnicodeToUtf8(data,message);
					iFile.Write(data);
					//iFile.Write(_L8("</text>"));
					tempdata.Copy(_L("</text>")); 
					converter.ConvertFromUnicodeToUtf8(data,tempdata);
					iFile.Write(data);

					//iFile.Write(_L8("</sms>"));				
					tempdata.Copy(_L("</sms>")); 
					converter.ConvertFromUnicodeToUtf8(data,tempdata);
					iFile.Write(data);

					//if(i%10 == 0) //dont read more than 10 sms at a time
					//	break;

				}
			}
			/*else //timestamp reached, stop
			{
				if(iSmsUploadType == 0)
					ChangeStatus(EBuildSentItemXmlData);
				else
					ChangeStatus(EBuildFinalData);
			}*/
		}
					TInt len;
					iFile.Size(len);
					if(len>SMS_PACKET_SIZE || (iIndex == iEntries->Count())){
#ifdef __LOGME__
						iAppUi.LogText.Copy(_L8("GETSMS 5\n"));
						iAppUi.WriteLogFile(iAppUi.LogText);
#endif

						if(smsval == 0)
							iLastInboxSms = time;
						else
							iLastSentItemsSms = time;
						
						if(iIndex == iEntries->Count()){
#ifdef __LOGME__
						iAppUi.LogText.Copy(_L8("GETSMS 6\n"));
						iAppUi.WriteLogFile(iAppUi.LogText);
#endif

							iIndex = 0;
							if(smsval == 0){
#ifdef __LOGME__
						iAppUi.LogText.Copy(_L8("GETSMS 7\n"));
						iAppUi.WriteLogFile(iAppUi.LogText);
#endif

								iSmsFolder = 1;
								//ChangeStatus(EBuildSentItemXmlData);
								ChangeStatus(EBuildFinalData);
							}
							else
								ChangeStatus(EBuildFinalData);
						}
						else //SMS Packet size reached
						{
#ifdef __LOGME__
								iAppUi.LogText.Copy(_L8("GETSMS 8\n"));
								iAppUi.WriteLogFile(iAppUi.LogText);
#endif

								ChangeStatus(EBuildFinalData);
						}
						
						//break;
					}

		delete smsMtm;
		delete entry;
	}
	//CleanupStack::PopAndDestroy(2); // entries, parentEntry
	//delete iSession;
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("/GetSms\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
}

TInt CSmsBackupTimer::CheckSmsNumber(TDesC& aMobiles)
{
	TBuf<30> mobile;
	TBuf<30> listmobile;
	TInt smsflag;	
	_LIT(KPlus,"+");
	_LIT(KZero,"0");
	
	smsflag = 0;
	
	CDesCArray* smsmobiles = iAppUi.GetSmsMobileList();

	for(int i = 0 ; i < smsmobiles->Count() ; i++)
	{		
		mobile.Copy(aMobiles);
		listmobile.Copy(smsmobiles->MdcaPoint(i));

		TInt pos = mobile.Find(KPlus);
		if(pos != KErrNotFound)
			mobile.Delete(pos,1);

		pos = listmobile.Find(KPlus);
		if(pos != KErrNotFound)
			listmobile.Delete(pos,1);

		pos = mobile.Find(KZero);
		if(pos == 0)
			mobile.Delete(pos,1);

		pos = listmobile.Find(KZero);
		if(pos == 0)
			listmobile.Delete(pos,1);
		
		//if(mobile.Compare(listmobile) == 0)
		if(mobile.Find(listmobile) >= 0 || listmobile.Find(mobile) >=0)
		{
			smsflag = 1;
			break;
		}
	}
	return smsflag;
}

void CSmsBackupTimer::InsertSms(TDesC& message)
{
	TBuf<160> aAddress;
	TBuf<500> aMessage;
	TBuf<500> aDescription;

	aAddress.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)));
	aMessage.Copy(message);
	aDescription.Copy(message);

	CMsvSession* iSession = CMsvSession::OpenSyncL(*this );

	CClientMtmRegistry* iMtmRegistry = CClientMtmRegistry::NewL(*iSession);

	CSmsClientMtm* iSmsMtm = STATIC_CAST( CSmsClientMtm*, iMtmRegistry->NewMtmL(KUidMsgTypeSMS));

	iSmsMtm->SwitchCurrentEntryL(KMsvGlobalInBoxIndexEntryId); //inbox
	//iSmsMtm->SwitchCurrentEntryL(KMsvDraftEntryId);  //draft

	iSmsMtm->CreateMessageL(KUidMsgTypeSMS.iUid);
	iSmsMtm->AddAddresseeL(aAddress); //to address if need
   
	CSmsHeader& iHeader = iSmsMtm->SmsHeader();
	iHeader.SetFromAddressL(aAddress);

	CRichText& body = iSmsMtm->Body();
	body.Reset();
	body.InsertL(0, aMessage);
  
	TMsvEntry entry = iSmsMtm->Entry().Entry(); 
	entry.SetInPreparation(EFalse); 
	entry.SetVisible(ETrue);
	entry.iDate.HomeTime();
	entry.iDescription.Set(aDescription);
	entry.iDetails.Set(aAddress);
	entry.SetUnread(ETrue);

	iSmsMtm->Entry().ChangeL(entry); 
	iSmsMtm->SaveMessageL();   
	
	delete iSession;
}

void CSmsBackupTimer::Notify()
{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("Notify\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	if (!IsActive())
		SetActive();

	//User::WaitForAnyRequest();

	/*RTimer timer; // The asynchronous timer and ...
	timer.CreateLocal(); // Always created for this thread.
	TTime timertime;

		timertime.HomeTime();
		timertime+=TTimeIntervalMicroSeconds(100000);
		timer.At(iStatus,timertime);
		User::WaitForRequest(iStatus);*/
}

void CSmsBackupTimer::PrepareMtm(TInt smsval)
{
	TInt smstype;

	if( smsval == 0)
		smstype = KMsvGlobalInBoxIndexEntryId;
	else if( smsval == 1)
		smstype = KMsvSentEntryId;

	iSession = CMsvSession::OpenSyncL(*this );

	iMtmReg = CClientMtmRegistry::NewL(*iSession);

	TMsvSelectionOrdering sort(KMsvGroupByType, EMsvSortByDate, ETrue);
	//sort.SetShowInvisibleEntries(ETrue);

	iParentEntry  = CMsvEntry::NewL(*iSession, smstype, sort);	

	// get list of SMS entries in inbox
	iEntries  = iParentEntry->ChildrenL();

#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("Count="));
	iAppUi.LogText.AppendNum(iEntries->Count());
	iAppUi.LogText.Append(_L8("\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
}

void CSmsBackupTimer::ReleaseMtm()
{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("ReleaseMtm 1\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	delete iEntries;
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("ReleaseMtm 2\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
	delete iParentEntry;
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("ReleaseMtm 3\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
	delete iMtmReg;
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("ReleaseMtm 4\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
	delete iSession;
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L8("ReleaseMtm 5\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
}

void CSmsBackupTimer::InsertUploadSuccessfullSms(TDesC& message)
{
	TBuf<160> aAddress;
	TBuf<500> aMessage;
	TBuf<500> aDescription;

	aAddress.Copy(_L("+"));
	aAddress.Append(iAppUi.iMobileNo);
	aMessage.Copy(message);
	aDescription.Copy(message);

	CMsvSession* iSession = CMsvSession::OpenSyncL(*this );

	CClientMtmRegistry* iMtmRegistry = CClientMtmRegistry::NewL(*iSession);

	CSmsClientMtm* iSmsMtm = STATIC_CAST( CSmsClientMtm*, iMtmRegistry->NewMtmL(KUidMsgTypeSMS));

	iSmsMtm->SwitchCurrentEntryL(KMsvGlobalInBoxIndexEntryId); //inbox
	//iSmsMtm->SwitchCurrentEntryL(KMsvDraftEntryId);  //draft

	iSmsMtm->CreateMessageL(KUidMsgTypeSMS.iUid);
	iSmsMtm->AddAddresseeL(aAddress); //to address if need
   
	CSmsHeader& iHeader = iSmsMtm->SmsHeader();
	iHeader.SetFromAddressL(aAddress);

	CRichText& body = iSmsMtm->Body();
	body.Reset();
	body.InsertL(0, aMessage);
  
	TMsvEntry entry = iSmsMtm->Entry().Entry(); 
	entry.SetInPreparation(EFalse); 
	entry.SetVisible(ETrue);
	entry.iDate.HomeTime();
	entry.iDescription.Set(aDescription);
	entry.iDetails.Set(aAddress);
	entry.SetUnread(ETrue);

	iSmsMtm->Entry().ChangeL(entry); 
	iSmsMtm->SaveMessageL();   
	
	delete iSession;
}