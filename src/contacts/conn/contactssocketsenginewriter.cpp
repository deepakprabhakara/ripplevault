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
 Module Name				: ContactsSocketsEngineWriter.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Sockets engine for Contacts backup
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#include <eikgted.h>
#ifndef __UIQ__
#include <aknnotewrappers.h>
//#include <aknmessagequerydialog.h>
#endif

#include <f32file.h>
#include <s32file.h>
#include <d32dbms.h>

#include <etel.h>
#include <cdbcols.h>
#include <commdb.h>

#include <utf.h>
#ifndef __UIQ__
#include  <RippleVault.rsg>
#else
#ifdef __S80__
#include  <RippleVault_series80.rsg>
#else
#include  <RippleVault_uiq.rsg>
#endif
#endif
#include "ContactsDBEngine.h"
#include "TimeOutTimer.h"
#include "RippleVault.pan"
#include "RippleVaultAppUi.h"
#include "List.h"
#include "RecordContactDB.h"
//#include "dnsresolver.h"
#include "UpdateContactsAO.h"

#include "ContactsSocketsEngineWriter.h"

const TInt CContactsSocketsEngineWriter::KTimeOut = 30000000; // 30 seconds time-out

#ifndef __SSL6__
#ifdef __SSL7__
_LIT(KTLS1,"TLS1.0");
#endif
#endif

CContactsSocketsEngineWriter* CContactsSocketsEngineWriter::NewL(CRippleVaultAppUi& aAppUi, TDesC& aAddress, TUint16 aPort)
    {
	CContactsSocketsEngineWriter* self = CContactsSocketsEngineWriter::NewLC(aAppUi, aAddress, aPort);
	CleanupStack::Pop(self);
	return self;
    }

	
CContactsSocketsEngineWriter* CContactsSocketsEngineWriter::NewLC(CRippleVaultAppUi& aAppUi, TDesC& aAddress, TUint16 aPort)
    {
	CContactsSocketsEngineWriter* self = new (ELeave) CContactsSocketsEngineWriter(aAppUi, aAddress, aPort);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
    }


CContactsSocketsEngineWriter::CContactsSocketsEngineWriter(CRippleVaultAppUi& aAppUi, TDesC& aAddress, TUint16 aPort)
: CActive(EPriorityStandard),
  iAppUi(aAppUi),
  iPort(aPort),
  iServerName(aAddress)
    {
//	iRegister = 0;
//	iNewUser = 0;
	iBuffer.Copy(_L(""));
	iRecordList = NULL;
#ifndef WAP_PT
	iResolvedLocal = EFalse;
#endif
	servlet_data = NULL;
    }


CContactsSocketsEngineWriter::~CContactsSocketsEngineWriter()
    {
	/*TBuf8<60> buf;
	buf.Copy(_L8("Calling SEW destructor\r\n"));
	iAppUi.Logger(buf);*/
    Cancel();

	if (servlet_data)
		delete servlet_data;

	if (iUpdateAO != NULL)
	{
		delete iUpdateAO;
		iUpdateAO = NULL;
	}

	CleanupList();
	
    delete iTimer;
#ifdef WAP_PT
	iWapServ.Close();
	delete iCap;
#else
	iSocket.Close();
	iSocketServ.Close();	

#ifndef __SSL6__
#ifdef __SSL7__
	if(iTlsSocket != NULL)
	{
		//iTlsSocket->CancelAll();
		//iTlsSocket->Close();
		delete iTlsSocket;
		iTlsSocket = 0;
	}
#endif
#endif

	/*buf.Copy(_L8("SEW destructed\r\n"));
	iAppUi.Logger(buf);*/
#endif

    }


void CContactsSocketsEngineWriter::ConstructL()
{
    // Start a timer 
	iTimer = CTimeOutTimer::NewL(EPriorityHigh, *this);
	//iTimer = CTimeOutTimer::NewL(EPriorityIdle, *this);
	CActiveScheduler::Add(this);
	iDoCancel = EFalse;
	//iUpgradeFlag = EFalse;
	iVersionFlag = EFalse;
	iPoll = EFalse;
#ifdef WAP_PT
	iRetryCount = 0;
	port = 0;
	iWapServ.Connect();
	iCap = CCapCodec::NewL();
	iCap->SetServerSDUSize(150000);
	iCap->SetClientSDUSize(150000);
	iCap->SetProtocolOptions(CCapCodec::EAckHeaders);
#else
	iSocketServ.Connect();
#endif
}


#ifdef WAP_PT
TInt32 CContactsSocketsEngineWriter::DefaultAccessPointId(TUint32& aId)
{
	RGenericAgent netAgent1;
	TUint32 ida1 = -1;
	TUint32 ida = 0;
	TInt ret = KErrNotFound;
	
	if (netAgent1.Open() == KErrNone)
	{			
		TBuf<10> TableName;
		TableName.Copy(_L("IAP"));

		TBuf<4> ColName1;
		ColName1.Copy(_L("Id"));

		TBuf<6> ColName2;
		ColName2.Copy(_L("Name"));

		CleanupClosePushL(netAgent1);
		CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeIAP);
		CleanupStack::PushL(db);			
		
		CCommsDbConnectionPrefTableView* 
		view1 = db->OpenConnectionPrefTableViewOnRankLC(ECommDbConnectionDirectionOutgoing,1);
		CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref pref;
		if(view1->GotoFirstRecord()!=KErrNotFound)
		{
			view1->ReadConnectionPreferenceL(pref);
			ida=pref.iBearer.iIapId;			
		}
		
		CCommsDbTableView* 
		view = db->OpenTableLC(TableName);

		if(view->GotoFirstRecord()!=KErrNotFound)
		{
			do
			{
				view->ReadUintL(ColName1, ida1);
				if(ida==ida1)
				{
					aId = ida;
					ret = KErrNone;
					break;
				}
			}
			while(view->GotoNextRecord()!=KErrNotFound);
		}
		CleanupStack::PopAndDestroy(4); // view, db, netAgent			
	}
	return ret;
}

TInt CContactsSocketsEngineWriter::GetGatewayAddress(TDes8& addr)
{
	TInt ret = KErrNotFound;

	TUint32 id;
	if (DefaultAccessPointId(id) == KErrNone)
	{
		CCommsDatabase* db;
		db=CCommsDatabase::NewL(EDatabaseTypeIAP);
		CleanupStack::PushL(db);

		CCommsDbTableView *view=db->OpenTableLC(TPtrC(WAP_IP_BEARER));
		while (view->GotoNextRecord()==KErrNone)
    	{
        	TUint32 currid;
	    	view->ReadUintL(TPtrC(WAP_IAP), currid);
	    	if (currid==id)
        	{
				TBuf<100> tmp;

				view->ReadTextL(TPtrC(WAP_GATEWAY_ADDRESS), tmp);

				iAppUi.LogText.Copy(_L("Contacts gateway="));
				iAppUi.WriteLogFile(iAppUi.LogText);

				iAppUi.LogText.Copy(tmp);
				iAppUi.WriteLogFile(iAppUi.LogText);

				if(tmp.Compare(_L("0.0.0.0")) == 0)
				{
					iAppUi.LogText.Copy(_L("Not Equal"));
					iAppUi.WriteLogFile(iAppUi.LogText);
					//tmp.Copy(iAppUi.iGateWay);
					TRAPD(r, GetProxyAddress(id, tmp));
				}

            	CCnvCharacterSetConverter* conv = CCnvCharacterSetConverter::NewL();
            	RFs fs;
            	fs.Connect();
            	conv->PrepareToConvertToOrFromL(KCharacterSetIdentifierUtf8, fs);
            	conv->ConvertFromUnicode(addr, tmp);
            	fs.Close();
            	delete conv;
            	ret = KErrNone;
				break;
	    	}
	    } //while
    	CleanupStack::PopAndDestroy(2); //db, view
	}
	return ret;
}

void CContactsSocketsEngineWriter::MakeContentLength(TUint size, TDes8& header)
{
	TChar bytes;
	if (size>0xffffff) {
		bytes=4;
	} else if(size>0xffff) {
		bytes=3;
	} else if (size>0xff) {
		bytes=2;
	} else {
		bytes=1;
	}

	header.Append(bytes);

	TChar enc[4];
	TUint mask=256;
	TUint mod=0;
	TInt i=bytes;
	--i;
	while (i>=0) {
		mod=size % mask;
		size-=mod;
		if (mask>256) mod/=(mask/256);
		enc[i]=mod;
		mask*=256;
		--i;
	}
	for (i=0; i<bytes; i++) {
		header.Append(enc[i]);
	}
}

void CContactsSocketsEngineWriter::SendTransaction(RWSPCOConn& aConn, RWSPCOTrans& aCOTrans, RWSPCOTrans& aTrans)
{
    // create transaction
    TBuf<100> uri;
    uri.Zero();

	uri.Copy(_L("http://"));
    uri.Append(iServerName);

    //CEikonEnv::Static()->AlertWin(_L("SendTransaction"));
    
	if (iPort != 80) {
		uri.Append(_L(":"));
		uri.AppendNum(iPort);
	}
    //uri.Append(_L("/ripple/rippleservlet")); // hardcoded
	uri.Append(iAppUi.iContactsUrl); 

    TBuf8<64> head;
    head.Zero();

    head.Append('\x91'); // content type opcode
    head.Append('\x92'); // content type value
    head.Append('\x8d'); // Content-length opcode

    MakeContentLength(servlet_data->Length(), head); // generate content length

    // Create transaction
	//SendTransaction(iConnection, iCOTrans, iTransSynch);
    aConn.CreateTransaction(RWAPConn::EPost, uri, head, servlet_data->Des(), aTrans);
    aConn.GetEvent(iEvent, aCOTrans, iStatus);
}
#endif // ifdef WAP_PT

/*
#ifndef WAP_PT
TInt CContactsSocketsEngineWriter::ResolveLocalDNS()
{
	TInt err = KErrNotFound;
	if (!iResolvedLocal)
	{
		iServerName.LowerCase();
		// second arg is returned value - i.e. ip addr filled into iServerName
		err = iAppUi.iDNSResolver->GetIPForUrl(iServerName, iServerName);

		if (err == KErrNone)
		{
			iResolvedLocal = ETrue;
		}
	}
	return err;
}
#endif
*/

void CContactsSocketsEngineWriter::Start()
{    
	iAppUi.iConnection = 1;

#ifndef WAP_PT
	ChangeStatus(EConnecting);
	
	if (iServerName.Find(_L("http://")) != KErrNotFound)
	{
		iServerName.Delete(0, 7); // length of "http://"
	}
	TInetAddr addr;
	// try to resolve URL locally first
	if (addr.Input(iServerName) == KErrNone)// || ResolveLocalDNS() == KErrNone)
	{
		Connect();
		return;
	}

	iResolvedLocal = EFalse;
	// possibly server has moved to another IP or server URL is new to us
	// resolve with DNS server
	User::LeaveIfError(iResolver.Open(iSocketServ, KAfInet, KProtocolInetTcp));
	// DNS request for name resolution
	iResolver.GetByName(iServerName, iNameEntry, iStatus);
	
	ChangeStatus(ELookingUp);

	// Request time out
	iTimer->After(KTimeOut);
	SetActive();
#else
    ChangeStatus(EConnecting);

	//iTimer->After(KTimeOut);

    _LIT8(headers, "");

    TBuf8<40> addr;
	if (GetGatewayAddress(addr) == KErrNone)
    {
    	port=9201;
    	User::LeaveIfError(iConnection.Open(iWapServ, addr, port, 0, EIP, EFalse));
    	TInt ret = iConnection.Connect(headers, iCap);

		iAppUi.NetworkStatusNotification(NETWORK_STATUS_CONNECTING);

    	iConnection.GetEvent(iEvent, iCOTrans, iStatus);
		iTimer->After(KTimeOut * 10);
    	SetActive();
    }
    else
    {
		ErrorStatus();
    }
#endif	
}

void CContactsSocketsEngineWriter::Connect()
{
#ifndef WAP_PT
    TInetAddr addr;
    if (addr.Input(iServerName) == KErrNone)
    {
		/*TBuf8<20> buf;
		buf.Copy(_L("Connect1\r\n"));
		iAppUi.Logger(buf);*/

        // Open a TCP socket
        User::LeaveIfError(iSocket.Open(iSocketServ, KAfInet, KSockStream, KProtocolInetTcp));

#ifdef __SSL6__
   		iSocket.SetOpt( KSoSecureSocket, KSolInetSSL );
#endif

        // Set up address information
	    iAddress.SetPort(iPort);
	    iAddress.SetAddress(addr.Address());
	    
#ifndef __SSL6__
#ifdef __SSL7__
		ChangeStatus(ESecureSocket);
#else
		ChangeStatus(EConnected);
#endif
#else		
	    ChangeStatus(EConnected);
#endif
        iTimer->After(KTimeOut);

		iAppUi.NetworkStatusNotification(NETWORK_STATUS_CONNECTING);

        // Initiate socket connection
	    iSocket.Connect(iAddress, iStatus);

		/*buf.Copy(_L("Connect2\r\n"));
		iAppUi.Logger(buf);*/

		SetActive();
     }
#endif
}

void CContactsSocketsEngineWriter::Disconnect()
    {
#ifndef WAP_PT    
#ifndef __SSL6__
#ifdef __SSL7__
	//if (iEngineStatus <= ESyncRecv)
	{
		//iSocket.CancelAll();
	}
	//else
	{
		//iSocket2.CancelAll();
	}
	if(iTlsSocket != NULL)
	{
		iTlsSocket->CancelAll();
		iTlsSocket->Close();
		delete iTlsSocket;
		iTlsSocket = 0;
	}
#endif
#endif

	//if (iEngineStatus <= ESyncRecv)
	{
		iSocket.Close();
		iSocketServ.Close();
	}
	//else
	{
		//iSocket2.Close();
		//iSocketServ2.Close();
	}
	ChangeStatus(EConnecting);
#endif
    }

// from CActive
void CContactsSocketsEngineWriter::DoCancel()
    {
#ifndef WAP_PT
    iTimer->Cancel();

    // Cancel appropriate request to socket
    switch (iEngineStatus)
        {
    	case ELookingUp:		
	    iResolver.Cancel();
       	    iResolver.Close();
            break;
        case EConnecting:
            break;
        case EConnected:
            iSocket.CancelConnect();
            break;        
        case EWriting:
        case EWritten:
			iSocket.CancelWrite();
            break;
//		case EReceived:
		case ESyncRecv:
			iSocket.CancelRecv();
			break;
/*		case EWritingACK:
			iSocket2.CancelConnect();
			break;
		case EWrittenACK:
			iSocket2.CancelWrite();
			break;*/
	    }

#else
	iTimer->Cancel();
	
	if (iDoCancel)
		return;

	iDoCancel = ETrue;

	if(port == 0)
		return;

	RWSPCOConn::TSessionState aState;
	iConnection.GetSessionState(aState);
	if (aState == RWSPCOConn::EDisconnected)
		return;

	switch (iEngineStatus)
	{
		case EConnecting:
			iConnection.CancelGetEvent();
			//break;
		case EConnected:
			iConnection.Disconnect();
			iConnection.Close();
			break;
		case EAcked:
			iConnection.Disconnect();
			iConnection.Close();
			break;
		default:
			break;
	}
#endif    
    }

// from CActive
void CContactsSocketsEngineWriter::RunL()
{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL CSockets\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

#ifndef WAP_PT
	iTimer->Cancel(); // Cancel TimeOut timer before completion
	if (iStatus != KErrNone && iStatus != KErrEof) // case KErrEof stops the RecvOneOrMore looping
	{
		/*TBuf8<20> buf;
		buf.Copy(_L("Error\r\n"));
		iAppUi.Logger(buf);*/

		ErrorStatus();
		return;
	}
	else if (iStatus == KErrNone && (/*iEngineStatus == EReceived ||*/ iEngineStatus == ESyncRecv)){
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL CSockets1\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		//IssueRead();
		//SetActive();
		if (iBuffer.Length() + readBuf.Length() <= iBuffer.MaxLength())
		{
			iBuffer.Append(readBuf);
			ChangeStatus(EWritten);
		}
	}
	if (iEngineStatus == EConnected)
	{
		iEngineStatus = EWriting;
	}

	switch(iEngineStatus)
	{
#ifndef __SSL6__
#ifdef __SSL7__
	case ESecureSocket:
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL CSockets: ESecureSocket\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

			iTlsSocket = CSecureSocket::NewL( iSocket, KTLS1 );

			// Set any options before the handshake starts

			// Clears any previous options
			iTlsSocket->FlushSessionCache();
			
		/*	Note: You could here set the available ciphers with code such as the following:
			TBuf8<2> buf;
			buf.SetLength(2);
			buf[0]=0; buf[1]=10;
			iTlsSocket->SetAvailableCipherSuites( buf ); */
			
			// start the handshake 
			iTlsSocket->StartClientHandshake( iStatus );


			ChangeStatus(EConnected);

			SetActive();
		break;
#endif
#endif

	case ELookingUp:
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL CSockets: ELookingUp\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		iResolver.Close();
		if (iStatus == KErrNone)
		{
			// DNS look up successful
			TNameRecord nameRecord;

			nameRecord = iNameEntry();
			// Extract domain name and IP address from name record
			TBuf<20> ipAddr;
			TInetAddr::Cast(nameRecord.iAddr).Output(ipAddr);
			// save the url/ip mapping to our local DNS
			//iAppUi.iDNSResolver->SaveDNSEntry(iServerName, ipAddr);
			// And connect to the IP address
			iServerName.Copy(ipAddr);
			
			Connect();
		}
		break;	
	case EWriting:{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL CSockets: EWriting\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		TPtr8 data = servlet_data->Des();
		iTimer->After(KTimeOut);
		/*TBuf8<20> buf;
		buf.Copy(_L("Writing\r\n"));
		iAppUi.Logger(buf);*/
		// READ_LEN = 1024;
		if (servlet_data->Length() > READ_LEN)
		{
			readBuf.Copy(servlet_data->Ptr(), READ_LEN);
			data.Copy(&data[READ_LEN], data.Length()-READ_LEN);
			ChangeStatus(EWriting);	
		}
		else
		{
			readBuf.Copy(*servlet_data);
			ChangeStatus(EWritten);
		}
		iAppUi.NetworkStatusNotification(NETWORK_STATUS_UPLOADING_CONTACTS);
#ifdef __SSL6__
			iSocket.Write(readBuf, iStatus); // Initiate actual write	
#else		
#ifdef __SSL7__
			iTlsSocket->Send( readBuf, iStatus, iDummyLength );
#else
			iSocket.Write(readBuf, iStatus); // Initiate actual write	
#endif
#endif

		SetActive();
		}
	break;
	
	case EWritten:{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL CSockets: EWritten\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		/*TBuf8<20> buf;
		buf.Copy(_L("Written\r\n"));
		iAppUi.Logger(buf);*/

		IssueRead();
		iTimer->After(KTimeOut*20);
		SetActive();

//		if (iRegister)
//		{
//			ChangeStatus(EReceived);
//		}
//		else
		{
			ChangeStatus(ESyncRecv);
		}

	}
	break;

//	case EReceived:{
//		_LIT8(ACK_O,"<ack>");
//		_LIT8(ACK_C,"</ack>");
//		_LIT8(SUCCESS,"suc_reg");
//			
//		TInt start;
//		TInt end;
//
//		TBuf8<100> ack_message;
//
//		start=iBuffer.Find(ACK_O);
//		HBufC* message;
//
//		if (start >= 0)
//		{
//			iTempSession.Copy(&iBuffer[start], iBuffer.Length()-start);
//			end=iTempSession.Find(ACK_C);
//
//			if (start+5 >= 0)
//			ack_message.Copy(&iBuffer[start+5], end-5);				
//		}
//		else
//		{
//			message = iAppUi.ReadFromResource(R_NETWORK_ERROR);
//			ack_message.Copy(*message);
//			delete message;
//		}
//#ifndef __UIQ__
//		if (iWaitDialog)
//			iWaitDialog->ProcessFinishedL();			
//#endif
//		if(ack_message.Compare(SUCCESS) == 0)
//		{
//			iAppUi.WritRegisteredFile();
//
//			iAppUi.UserRegistered();
//		
//			iAppUi.EnableUpload(NOT_REGISTERED);
//		}
//		else
//		{
//			TBuf<100> msg;
//			msg.Copy(ack_message);
//			if (msg.Compare(_L("fail_reg")) == 0) {
//				if (iNewUser == 0)
//					message = iAppUi.ReadFromResource(R_INCORRECT_USER_NAME);
//				else
//					message = iAppUi.ReadFromResource(R_INCORRECT_USERID_PASSWORD);
//
//				msg.Copy(*message);
//				delete message;
//			}
//			//msg.Copy(_L("Incorrect Userid/password"));
//#ifndef __UIQ__
//			CAknInformationNote* note = new(ELeave) CAknInformationNote;
//			note->ExecuteLD(msg);
//#else
//			CEikonEnv::Static ()->AlertWin(msg);
//#endif
//		}
//
//		iSocket.Close();
//		iSocketServ.Close();
//		delete this;
//	}
//	break;

	case ESyncRecv:
	{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL CSockets: ESyncRecv\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		// Get XMLData and parse;
		
#ifndef __SSL6__
#ifdef __SSL7__
	if(iTlsSocket != NULL)
	{
		iTlsSocket->CancelAll();
		iTlsSocket->Close();
		delete iTlsSocket;
		iTlsSocket = 0;
	}
#endif
#endif

		iSocket.Close();
		iSocketServ.Close();

		_LIT8(ACK_O,"<ack>");
		_LIT8(ACK_C,"</ack>");
//		_LIT8(SUCCESS,"suc_reg");
			
		TInt start;
		TInt end;

		/*TBuf8<20> buf;
		buf.Copy(_L("ESyncRecv\r\n"));
		iAppUi.Logger(buf);*/

		TBuf8<100> ack_message;
		TBuf<100> ackMessage;
		//TBuf8<1024> temp_session;

#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("iBuffer=\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);

		iAppUi.WriteLogFile(readBuf);
#endif

		start=iBuffer.Find(ACK_O);

		if (start >= 0)
		{
			iTempSession.Copy(&iBuffer[start], iBuffer.Length()-start);
			end=iTempSession.Find(ACK_C);

			if (start+5 >= 0)
			    ack_message.Copy(&iBuffer[start+5], end-5);

			ackMessage.Copy(ack_message);
			ProcessAck(ackMessage);

			//delete this;
			break;
		}
		
//		if (iVersionFlag)
//		{
//			TInt lenStart = iBuffer.Find(_L8("Content-Length: "));
//			TInt lenEnd = iBuffer.Find(_L8("Date: "));
//			TBuf8<10> length;
//			length.Copy(&iBuffer[lenStart + 16], lenEnd - lenStart - 16);
//
//			TInt len;
//			TLex8 lex(length);
//			lex.Val(len);
//
//
//			TInt loc = iBuffer.Find(_L8("\r\n\r\n"));
//			if (loc + 4 < iBuffer.Length())
//				iBuffer.Copy(&iBuffer[loc + 4], len); 
//
//			//iAppUi.iWebVersion.Copy(iBuffer);
//			iAppUi.iWebVersion.Copy(_L("1.13.0"));
//			iAppUi.Upgrade(CRippleVaultAppUi::EGetSis);
//			
//			delete this;
//			break;
//		}
		
//		if (iUpgradeFlag)
//		{
//			RFs fs;
//			RFile file;
//
//			//TBuf8<1024> ts;
//			//ts.Copy(*dat);
//
//			_LIT(KFile, "contacts.sis");
//
//			TBuf<128> path;
//			path = CRippleVaultAppUi::ApplicationDriveAndPath();
//			path.Append(KFile);
//
//			fs.Connect();
//
//			TInt err = file.Replace(fs, path,EFileWrite);
//
//			//HBufC8* data;
//			//data = ts.AllocL();
//
//			TInt lenStart = iBuffer.Find(_L8("Content-Length: "));
//			TInt lenEnd = iBuffer.Find(_L8("Date: "));
//			TBuf8<10> length;
//			length.Copy(&iBuffer[lenStart + 16], lenEnd - lenStart - 16);
//
//			TInt len;
//			TLex8 lex(length);
//			lex.Val(len);
//
//
//			TInt loc = iBuffer.Find(_L8("\r\n\r\n"));
//			if (loc + 4 < iBuffer.Length())
//				iBuffer.Copy(&iBuffer[loc + 4], len); 
//
//			if (err == KErrNone) {
//				User::LeaveIfError(file.Write(iBuffer));
//				file.Flush();
//				file.Close();
//			}
//
//			//delete data;
//			//data = NULL;
//
//			fs.Close();
//
//			iAppUi.Upgrade(CRippleVaultAppUi::EUpgradeSis);
//
//			delete this;
//			break;
//		}

		// received buffer MUST have these three tags - add, update, delete
		if ( (iBuffer.Find(_L8("add")) == KErrNotFound) ||
			(iBuffer.Find(_L8("update")) == KErrNotFound) ||
			(iBuffer.Find(_L8("delete")) == KErrNotFound) )
		{
			//Disconnect();
			iAppUi.iContactsHandler->SendError();
			break;
		}

		// look for end of data
		if (iBuffer.Find(_L8("</delete>")) == KErrNotFound)
		{
			//Disconnect();
			iAppUi.iContactsHandler->SendError();
			break;
		}

		//iAppUi.iContactsHandler->SendComplete();

		CList *list = GetParsedRecords();

		iRecordList = list;
		ConvertEscapeSequences(list);

		// create a active obj, give list, iBuffer
		// define a function SocketEngineWriter::AckToServer()
		// once update is done in active object, it will call
		// the AckServer(), also perform CleanupList in the
		// AckServer() function
		//UpdateContactDB(list);

		iDownloadCount = iRecordList->Count();

		if (iDownloadCount == 0) {
			//iRecordList->RemoveAll();
			delete iRecordList;
			iRecordList = NULL;

			TBuf<30> tempstr;
			tempstr.Copy(_L("sync"));
			iAppUi.iContactsHandler->iChangesDB->SetMessageType(tempstr);
			iAppUi.iContactsHandler->SendComplete();
			//delete this;
			return;
		}

		iUpdateAO = new (ELeave) CUpdateContactsAO(iAppUi, *this);
		iUpdateAO->StartL(list);

		//iAppUi.IncrementState();
				
	}
	break;
/*
	case EWritingACK:	
	{
		ChangeStatus(EWrittenACK);
		delete servlet_data;
		servlet_data=NULL;
		servlet_data=iBuffer.AllocL();
#ifdef __SSL6__
		iSocket2.Write(*servlet_data, iStatus); // Initiate actual write
#else
#ifdef __SSL7__
		iTlsSocket->Send( *servlet_data, iStatus, iDummyLength );
#else
		iSocket2.Write(*servlet_data, iStatus); // Initiate actual write
#endif
#endif

	    iTimer->After(KTimeOut);
		SetActive();
	}
	break;

	case EWrittenACK:
	{
		iSocket2.Close();
		iSocketServ2.Close();
		iAppUi.iContactsHandler->MoreToUpload();
	}
	break;
*/
	default:
	break;
	}
#else // WAP_PT code follows
    //TBuf<2048> buf;

	iTimer->Cancel();
    /*RWSPCOConn::TSessionState s;
	iConnection.GetSessionState(s);
	*/

    if (iStatus!=KErrNone)
    {
        //CEikonEnv::Static()->AlertWin(_L("Error"));
		ErrorStatus();
        return;
    }

    switch(iEngineStatus)
    {
    case EConnecting:
        //if (s == RWSPCOConn::EConnected)
        {
			if (iEvent() == RWSPCOConn::EDisconnect_ind_s ||
			    iEvent() == RWSPCOConn::EAbort_ind_t ||
			    iEvent() == RWSPCOConn::EException_ind_e)
			{
				ErrorStatus();
			}
			else if (iEvent()==RWSPCOConn::EConnect_cnf_s)
            {
/*				TBuf8<50> buf;
				buf.Copy(_L("Connected\r\n"));
				iAppUi.WriteLogFile(buf);
*/
            	iBuffer.SetLength(0);
                ChangeStatus(EConnected);
				iAppUi.NetworkStatusNotification(NETWORK_STATUS_UPLOADING_CONTACTS);

                SendTransaction(iConnection, iCOTrans, iTransSynch);
                iTimer->After(KTimeOut*20);
               	SetActive();
            }
			else
			{
/*				TBuf8<50> buf;
				buf.Copy(_L("Event - "));
				buf.AppendNum(iEvent());
				buf.Append(_L("\r\n"));
				iAppUi.WriteLogFile(buf);
*/
				ErrorStatus();
			}
        }
        break;
    case EConnected:
    	iBuffer.SetLength(0);
        if (iEvent() == RWSPCOConn::EDisconnect_ind_s ||
            iEvent() == RWSPCOConn::EAbort_ind_t ||
		    iEvent() == RWSPCOConn::EException_ind_e)
        {
            ErrorStatus();
        }
        else if (iEvent()==RWSPCOConn::EMethodResult_ind_t)
        {
            // got response
			TInt ret;
			
/*			TBuf8<50> buf;
			buf.Copy(_L("Sent data\r\n"));
			iAppUi.WriteLogFile(buf);
*/			
			ret=iCOTrans.GetData(readBuf, RWSPCOTrans::EResultBody);

			//CEikonEnv::Static()->AlertWin(_L("Got Response"));
			iBuffer.Append(readBuf);
			
			while ( ret == RWAPConn::EMoreData )
			{
				ret=iCOTrans.GetData(readBuf, RWSPCOTrans::EResultBody);
				if (iBuffer.Length() + readBuf.Length() > iBuffer.MaxLength())
				{
					break;
				}
				iBuffer.Append(readBuf);
			}
			
			iCOTrans.Acknowledge(_L8(""));
			iTransSynch.Release();

			// Process Data based on the transaction type
//			if (iRegister)
//			{
//				iConnection.Disconnect();
//				iConnection.Close();
//
//				TBuf8<50> buf;
//				buf.Copy(_L("Process Register Response\r\n"));
//				iAppUi.WriteLogFile(buf);
//
//				ProcessRegisterResponse();
//				delete this;
//			}
//			else
			{
				//CEikonEnv::Static()->AlertWin(_L("ProcessSynchResponse"));
/*				TBuf8<50> buf;
				buf.Copy(_L("Process Synch Response\r\n"));
				iAppUi.WriteLogFile(buf);
*/
				iConnection.Disconnect();
				iConnection.Close();

				_LIT8(ACK_O,"<ack>");
				_LIT8(ACK_C,"</ack>");
					
				TInt start;
				TInt end;

				TBuf8<100> ack_message;
				TBuf<100> ackMessage;
				//TBuf8<1024> temp_session;

				start=iBuffer.Find(ACK_O);

				if (start >= 0)
				{
					iTempSession.Copy(&iBuffer[start], iBuffer.Length()-start);
					end=iTempSession.Find(ACK_C);

					if (start+5 >= 0)
						ack_message.Copy(&iBuffer[start+5], end-5);

					ackMessage.Copy(ack_message);
					ProcessAck(ackMessage);

					//delete this;
					break;
				}

				ret = ProcessSynchResponse();
				if (ret != 0)
				{
					iAppUi.iContactsHandler->SendError();
				}
			}
        }
        else
        {
			iConnection.GetEvent(iEvent, iCOTrans, iStatus);
			//iConnection.GetEvent(iEvent, iCOTrans);
			iTimer->After(KTimeOut * 4);
			//iTimer->After(KTimeOut);
			SetActive();
        }
        break;
	case EAcked:
	    if (iEvent() == RWSPCOConn::EDisconnect_ind_s ||
	        iEvent() == RWSPCOConn::EAbort_ind_t ||
			iEvent() == RWSPCOConn::EException_ind_e)
	    {
			iAppUi.iContactsHandler->SendError();
	    }
	    else if (iEvent()==RWSPCOConn::EMethodResult_ind_t)
	    {
			iCOTransAck.Acknowledge(_L8(""));
			iTransAck.Release();
			
			iConnection.Disconnect();
			iConnection.Close();

/*			TBuf8<50> buf;
			buf.Copy(_L("Acks sent\r\n"));
			iAppUi.WriteLogFile(buf);
*/
			iAppUi.iContactsHandler->MoreToUpload();
		}
		else
		{
			iConnection.GetEvent(iEvent, iCOTransAck, iStatus);
			iTimer->After(KTimeOut * 2);
			SetActive();
		}
        break;
    default:
        break;
    }
#endif // WAP_PT
}

TInt CContactsSocketsEngineWriter::ProcessSynchResponse()
{
	// received buffer MUST have end of data indication
	if (iBuffer.Find(_L8("</delete>")) == KErrNotFound)
	{
		//iAppUi.iContactsHandler->SendError();
		return -1;
	}

	//iAppUi.iContactsHandler->SendComplete();

	CList *list = GetParsedRecords();
	iRecordList = list;

	//UpdateContactDB(list);
	iDownloadCount = iRecordList->Count();

	if (iDownloadCount == 0) {
		delete iRecordList;
		iRecordList = NULL;

		TBuf<30> tempstr;
		tempstr.Copy(_L("sync"));
		iAppUi.iContactsHandler->iChangesDB->SetMessageType(tempstr);

		iAppUi.iContactsHandler->SendComplete();
		//delete this;
		return 0;
	}

	iUpdateAO = new (ELeave) CUpdateContactsAO(iAppUi, *this);
	iUpdateAO->StartL(list);
	
	// Increment Count();
	//iAppUi.IncrementState();

	return 0; // successful
}

TInt CContactsSocketsEngineWriter::RunError(TInt aError)
{
/*	TBuf8<60> buf;
	buf.Copy(_L8("CContactsSocketsEngineWriter Leave Error: "));
	buf.AppendNum(aError);
	buf.Append(_L("\r\n"));
	iAppUi.WriteLogFile(buf);
*/
	// Just return KErrNone to stop the active scheduler panicking.
	return KErrNone;
}

void CContactsSocketsEngineWriter::ErrorStatus()
{
//	TBuf8<60> buf;
//	buf.Copy(_L8("Calling DoCancel\r\n"));
//	iAppUi.WriteLogFile(buf);

	DoCancel();

	iAppUi.NetworkStatusNotification(NETWORK_STATUS_FAILED);
	iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_FAILED);

#ifndef WAP_PT
	if (iEngineStatus == EConnected && iResolvedLocal)
	{
		// resolve again from service provider dns
		iServerName.Copy(iAppUi.iServerAddr);
		
		Start();
		return;
	}

//	if (!iRegister)
	{
//		buf.Copy(_L8("Calling SendError\r\n"));
//		iAppUi.WriteLogFile(buf);

		Disconnect();
		iAppUi.iContactsHandler->SendError();
	}
//	else
//	{
//		//CEikonEnv::Static ()->AlertWin(_L("Network error"));
//#ifndef __UIQ__
//		if (iWaitDialog)
//			iWaitDialog->ProcessFinishedL();			
//#endif
//		//CEikonEnv::Static ()->AlertWin(_L("Pls retry"));
//		if (iEngineStatus == ELookingUp)
//		{
//			// show dns error
//			CEikonEnv::Static ()->AlertWin(_L("Host resolution error"));			
//		}
//		else
//		{
//			iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_TRYAGAIN);
//		}
//		Disconnect();
//		delete this;
//	}
#else
//	if (!iRegister)
	{
		iAppUi.iContactsHandler->SendError();
	}
//	else
//	{
//		//CEikonEnv::Static ()->AlertWin(_L("Network error"));
//#ifndef __UIQ__
//		if (iWaitDialog)
//			iWaitDialog->ProcessFinishedL();			
//#endif
//		//CEikonEnv::Static ()->AlertWin(_L("Pls retry"));
//		iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_TRYAGAIN);
//		delete this;
//	}
#endif
}

void CContactsSocketsEngineWriter::TimerExpired()
{
	// do not put any code to access any of
	// the data. The DoCancel immediately
	// notifies the RunL which starts immediately
	// A conflict of usage results in a crash.
/*	TBuf8<50> buf;
	buf.Copy(_L("Timer Expired\r\n"));
	iAppUi.WriteLogFile(buf);
*/
	if (iPoll) {
		iAppUi.iContactsHandler->Poll(iPollTimeOut, 1);
		return;
	}

    DoCancel();

	iAppUi.NetworkStatusNotification(NETWORK_STATUS_FAILED);
	iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_TIMEDOUT);

//	if (!iRegister)
	{
		iAppUi.iContactsHandler->SendError();
	}
//	else
//	{
//#ifndef WAP_PT
//		if ((iEngineStatus == EConnected) && iResolvedLocal)
//		{
//			// resolve again from service provider dns
//			//iServerName.Copy(iAppUi.iServerAddr);
//			//Start();
//		}
//		else
//		{
//			//CEikonEnv::Static ()->AlertWin(_L("Timed out"));
//			iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_TRYAGAIN);
//		}
//#else
//		iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_TRYAGAIN);
//#endif
//	}
}


void CContactsSocketsEngineWriter::ChangeStatus(TSocketsEngineState aNewStatus)
{
    iEngineStatus = aNewStatus;
}

//void CContactsSocketsEngineWriter::Register(TInt aRegistered)
//{
//	iRegister = TRUE;
//	iNewUser = aRegistered;
//#ifndef	__UIQ__			
//	iWaitDialog = new(ELeave)CAknWaitDialog( (REINTERPRET_CAST(CEikDialog**,&iWaitDialog)));
//	iWaitDialog->SetTone( CAknNoteDialog::EConfirmationTone ); 
//	iWaitDialog->ExecuteLD(R_WAITNOTE_DIALOG);
//#endif
//	Start();
//}

CList* CContactsSocketsEngineWriter::GetParsedRecords()
{
	HBufC *iBuf = HBufC::NewL(MAX_RECV_LEN);
	CleanupStack::PushL(iBuf);

	TPtr ptr = iBuf->Des();

	CnvUtfConverter converter;
	converter.ConvertToUnicodeFromUtf8(ptr, iBuffer);

	//ptr.Copy(iBuffer);

	TBuf<50> tagName;
	TBuf<512> tagValue;
	TInt changeType=0;

	CList *records = new CList();
	CRecordContactDB *record = NULL;

	TInt parsecomplete = 0;
	while(TRUE) {
		TInt len = ptr.Length();
		TInt startTag = ptr.Find(_L("<"));

		if (startTag == KErrNotFound)
			break;

		if (startTag + 1 == len)
			break;

		TInt endTag = ptr.Find(_L(">"));

		if (endTag == KErrNotFound)
		{
			break;
		}

		if (ptr[startTag + 1] == '/') {
			if (startTag + 2 == len)
				break;
			
			tagName.Copy(&ptr[startTag + 2], endTag - startTag - 2);

			if (startTag >= 0) {
				tagValue.Copy(&ptr[0], startTag);
				AddDataToRecord(tagName, tagValue, record);
			}
			if (tagName.Compare(_L("ct")) == 0)
			{
				records->Add(record);
				parsecomplete = 1;
			}
		}
		else {
			tagName.Copy(&ptr[startTag + 1], endTag - startTag - 1);

			if (tagName.Compare(_L("add")) == 0 || tagName.Compare(_L("update")) == 0
				 || tagName.Compare(_L("delete")) == 0) {

				if (tagName.Compare(_L("add")) == 0)
					changeType = 0;
				if (tagName.Compare(_L("update")) == 0)
					changeType = 1;
				if (tagName.Compare(_L("delete")) == 0)
					changeType = 2;
			}

			if (tagName.Compare(_L("ct")) == 0) {
				record = new CRecordContactDB();
				record->iChangeType = changeType;				
				//records->Add(record);
				parsecomplete = 0;
			}
		}

		if (endTag + 1 >= len)
			break;

		ptr.Copy(&ptr[endTag + 1], len - endTag - 1);
	}

	//delete iBuf;
	CleanupStack::PopAndDestroy(1); //iBuf

	if (!parsecomplete)
	{
		if (record)
		{
			delete record;
		}
	}
	return records;
}

void CContactsSocketsEngineWriter::AddDataToRecord(TDes& tagName, TDes& tagValue, CRecordContactDB* record)
{
	/*if (tagName.Find(_L("fn")) != KErrNotFound)
		record->iFirstName = tagValue.AllocL();
	else if (tagName.Find(_L("ln")) != KErrNotFound)
		record->iLastName = tagValue.AllocL();
	else if (tagName.Find(_L("cn")) != KErrNotFound)
		record->iCompanyName = tagValue.AllocL();
	else if (tagName.Find(_L("mblh")) != KErrNotFound)
		record->iHomeMobiles->Add(tagValue.AllocL());
	else if (tagName.Find(_L("mblb")) != KErrNotFound)
		record->iWorkMobiles->Add(tagValue.AllocL());
	else if (tagName.Find(_L("mbl")) != KErrNotFound)
		record->iMobiles->Add(tagValue.AllocL());
	else if (tagName.Find(_L("telh")) != KErrNotFound)
		record->iHomeTelephones->Add(tagValue.AllocL());
	else if (tagName.Find(_L("telb")) != KErrNotFound)
		record->iWorkTelephones->Add(tagValue.AllocL());
	else if (tagName.Find(_L("tel")) != KErrNotFound)
		record->iTelephones->Add(tagValue.AllocL());
	else if (tagName.Find(_L("fxh")) != KErrNotFound)
		record->iHomeFaxes->Add(tagValue.AllocL());
	else if (tagName.Find(_L("fxb")) != KErrNotFound)
		record->iWorkFaxes->Add(tagValue.AllocL());
	else if (tagName.Find(_L("fx")) != KErrNotFound)
		record->iFaxes->Add(tagValue.AllocL());
	else if (tagName.Find(_L("emlh")) != KErrNotFound)
		record->iHomeEmails->Add(tagValue.AllocL());
	else if (tagName.Find(_L("emlb")) != KErrNotFound)
		record->iWorkEmails->Add(tagValue.AllocL());
	else if (tagName.Find(_L("eml")) != KErrNotFound)
		record->iEmails->Add(tagValue.AllocL());
	else if (tagName.Find(_L("ttl")) != KErrNotFound)
		record->iJobTitles->Add(tagValue.AllocL());
	else if (tagName.Find(_L("gcid")) != KErrNotFound)
		record->iGlobalId = tagValue.AllocL();
	else if (tagName.Find(_L("id")) != KErrNotFound) {*/
	if (tagName.Compare(_L("fn")) == 0)
		record->iFirstName = tagValue.AllocL();
	else if (tagName.Compare(_L("ln")) == 0)
		record->iLastName = tagValue.AllocL();
	else if (tagName.Compare(_L("cn")) == 0)
		record->iCompanyName = tagValue.AllocL();
	else if (tagName.Compare(_L("mblh")) == 0)
		record->iHomeMobiles->Add(tagValue.AllocL());
	else if (tagName.Compare(_L("mblb")) == 0)
		record->iWorkMobiles->Add(tagValue.AllocL());
	else if (tagName.Compare(_L("mbl")) == 0)
		record->iMobiles->Add(tagValue.AllocL());
	else if (tagName.Compare(_L("telh")) == 0)
		record->iHomeTelephones->Add(tagValue.AllocL());
	else if (tagName.Compare(_L("telb")) == 0)
		record->iWorkTelephones->Add(tagValue.AllocL());
	else if (tagName.Compare(_L("tel")) == 0)
		record->iTelephones->Add(tagValue.AllocL());
	else if (tagName.Compare(_L("fxh")) == 0)
		record->iHomeFaxes->Add(tagValue.AllocL());
	else if (tagName.Compare(_L("fxb")) == 0)
		record->iWorkFaxes->Add(tagValue.AllocL());
	else if (tagName.Compare(_L("fx")) == 0)
		record->iFaxes->Add(tagValue.AllocL());
	else if (tagName.Compare(_L("emlh")) == 0)
		record->iHomeEmails->Add(tagValue.AllocL());
	else if (tagName.Compare(_L("emlb")) == 0)
		record->iWorkEmails->Add(tagValue.AllocL());
	else if (tagName.Compare(_L("eml")) == 0)
		record->iEmails->Add(tagValue.AllocL());
	else if (tagName.Compare(_L("ttl")) == 0)
		record->iJobTitles->Add(tagValue.AllocL());
	else if (tagName.Compare(_L("gcid")) == 0)
		record->iGlobalId = tagValue.AllocL();
	else if (tagName.Compare(_L("id")) == 0) {
		//TInt64 intVal;
		if (tagValue.Length() <= 0) {
			record->iContactId = -1;
		}
		else {
			TLex lex(tagValue);
			lex.Val(record->iContactId);
		}
	}
}

#ifndef WAP_PT
void CContactsSocketsEngineWriter::IssueRead()
{
    // Initiate a new read from socket into iBuffer
    //__ASSERT_ALWAYS(!IsActive(), User::Panic(KPanicSocketsEngineRead, ESocketsBadState));
#ifdef __SSL6__
	iSocket.RecvOneOrMore(readBuf, 0, iStatus, iDummyLength);
#else
#ifdef __SSL7__
	readBuf.Copy(_L8(""));
	//iTlsSocket->Recv( readBuf, iStatus );
	iTlsSocket->RecvOneOrMore( readBuf, iStatus, iDummyLength);
#else
	iSocket.RecvOneOrMore(readBuf, 0, iStatus, iDummyLength);
#endif
#endif

}
#endif

void CContactsSocketsEngineWriter::ReplaceTag(TDes& data, TPtrC toTag, TPtrC fromTag)
{
	if (data.Length() <= 0)
		return;

	TInt pos;

	while(1)
	{
		pos = data.Find(fromTag);
		if (pos == KErrNotFound)
			break;

		data.Delete(pos, fromTag.Length());
		data.Insert(pos, toTag);
	}	
}

void CContactsSocketsEngineWriter::ConvertXMLTags(TDes& data)
{
	ReplaceTag(data, _L("&"), _L("&amp;"));
	ReplaceTag(data, _L("<"), _L("&lt;"));
	ReplaceTag(data, _L(">"), _L("&gt;"));
	ReplaceTag(data, _L("\""), _L("&quot;"));
	ReplaceTag(data, _L("\'"), _L("&apos;"));
	//ReplaceTag(data, 128, _L("&euro;")); // or 0x20ac
}

void CContactsSocketsEngineWriter::ConvertRecordXMLTags(CList *list)
{
	CList* fieldItems = list;
	for (TInt i = 0; i < list->Count(); i++)
	{
		TPtr* data = (TPtr *)fieldItems->GetContent();
		TPtr& dat = *data;
	
		if (data != NULL) {
			ConvertXMLTags(dat);
		}
		
		fieldItems = fieldItems->next;
	}
}

void CContactsSocketsEngineWriter::ConvertEscapeSequences(CList *list)
{
	CList *ptr = list;

	for (TInt i = 0; i < list->Count(); i++)
	{
		CRecordContactDB *record = (CRecordContactDB *)ptr->GetContent();
#ifndef __WINSCW__
		if (record->iFirstName != NULL)
			ConvertXMLTags(record->iFirstName->Des());
		if (record->iLastName != NULL)
			ConvertXMLTags(record->iLastName->Des());
		if (record->iCompanyName != NULL)
			ConvertXMLTags(record->iCompanyName->Des());
#endif
		ConvertRecordXMLTags(record->iJobTitles);

		ConvertRecordXMLTags(record->iHomeTelephones);
		ConvertRecordXMLTags(record->iWorkTelephones);
		ConvertRecordXMLTags(record->iTelephones);

		ConvertRecordXMLTags(record->iHomeMobiles);
		ConvertRecordXMLTags(record->iWorkMobiles);
		ConvertRecordXMLTags(record->iMobiles);

		ConvertRecordXMLTags(record->iHomeFaxes);
		ConvertRecordXMLTags(record->iWorkFaxes);
		ConvertRecordXMLTags(record->iFaxes);

		ConvertRecordXMLTags(record->iHomeEmails);
		ConvertRecordXMLTags(record->iWorkEmails);
		ConvertRecordXMLTags(record->iEmails);

		ptr = ptr->next;
	}
}

//void CContactsSocketsEngineWriter::ProcessRegisterResponse()
//{
//#ifndef __UIQ__
//	if (iWaitDialog)
//	{
//		iWaitDialog->ProcessFinishedL();
//	}
//#endif
//	_LIT8(ACK_O,"<ack>");
//	_LIT8(ACK_C,"</ack>");
//	_LIT8(SUCCESS,"suc_reg");
//
//	TInt start;
//	TInt end;
//
//	TBuf8<100> ack_message;
//	TBuf8<1024> temp_session;
//	HBufC* message;
//
//	start=iBuffer.Find(ACK_O);
//
//	if (start >= 0)
//	{
//		temp_session.Copy(&iBuffer[start], iBuffer.Length()-start);
//		end=temp_session.Find(ACK_C);
//
//		if (start+5 >= 0)
//		{
//			ack_message.Copy(&iBuffer[start+5], end-5);
//		}
//	}
//	else
//	{
//		//ack_message.Copy(_L("Network failure"));
//		message = iAppUi.ReadFromResource(R_NETWORK_ERROR);
//		ack_message.Copy(*message);
//		delete message;
//	}
//
//	if(ack_message.Compare(SUCCESS) == 0)
//	{
//		iAppUi.WritRegisteredFile();
//		iAppUi.UserRegistered();
//		iAppUi.EnableUpload(NOT_REGISTERED);
//
//	}
//	else
//	{
//		TBuf<100> msg;
//		msg.Copy(ack_message);
//		if (msg.Compare(_L("fail_reg")) == 0) {
//			if (iNewUser == 0)
//				message = iAppUi.ReadFromResource(R_INCORRECT_USER_NAME);
//			else
//				message = iAppUi.ReadFromResource(R_INCORRECT_USERID_PASSWORD);
//
//			msg.Copy(*message);
//			delete message;
//		}
//		//if (msg.Compare(_L("fail_reg")) == 0)
//		//	msg.Copy(_L("Incorrect Userid/password"));
//
//#ifndef __UIQ__
//		CAknInformationNote* note = new(ELeave) CAknInformationNote;
//		note->ExecuteLD(msg);
//#else
//		CEikonEnv::Static ()->AlertWin(msg);
//#endif
//	}
//	iAppUi.SetTitle();
//}

void CContactsSocketsEngineWriter::ProcessAck(TDesC& ackMessage)
{
	TInt pos;
	TBuf<10> packetNum;
	TInt packNumber;
/*
	TInt ccount = 0;
	iAppUi.GetTempContactsUploadSettingsFromDB(ccount);
	
	iAppUi.iContactsCount = ccount + iAppUi.iContactsCount;

#ifdef __LOGME__
			iAppUi.LogText.Copy(_L("processackTEMPC=\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);

			iAppUi.LogText.Copy(_L(""));
			iAppUi.LogText.AppendNum(ccount);
			iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	iAppUi.SaveTempContactsUploadSettingsToDB(iAppUi.iContactsCount);
	iAppUi.iContactsCount = 0;
*/
	pos = ackMessage.Find(_L("cpr_reg"));
	if (pos != KErrNotFound) {
		packetNum.Copy(&ackMessage[7], ackMessage.Length() - 7);
		TLex lex(packetNum);
		lex.Val(packNumber);

		TBuf<30> tempstr;
		tempstr.Copy(_L("init_reg"));		
		
		iAppUi.iContactsHandler->iChangesDB->SetMessageType(tempstr);		

		iAppUi.iContactsHandler->SendCompleteI(packNumber);
		return;
	}

	pos = ackMessage.Find(_L("lpr_reg"));
	if (pos != KErrNotFound) {
		iAppUi.iContactsHandler->SendCompleteN();
		return;
	}

	pos = ackMessage.Find(_L("cpr_eu"));
	if (pos != KErrNotFound) {
		packetNum.Copy(&ackMessage[6], ackMessage.Length() - 6);
		TLex lex(packetNum);
		lex.Val(packNumber);

		TBuf<30> tempstr;
		tempstr.Copy(_L("init_eu"));		
		
		iAppUi.iContactsHandler->iChangesDB->SetMessageType(tempstr);

		iAppUi.iContactsHandler->SendCompleteI(packNumber);
		return;
	}

	pos = ackMessage.Find(_L("lpr_eu"));
	if (pos != KErrNotFound) {
		iAppUi.iContactsHandler->SendCompleteN();
		return;
	}

	pos = ackMessage.Find(_L("cpr_sync"));
	if (pos != KErrNotFound) {
		packetNum.Copy(&ackMessage[8], ackMessage.Length() - 8);
		TLex lex(packetNum);
		lex.Val(packNumber);

		TBuf<30> tempstr;
		tempstr.Copy(_L("sync"));		
		
		iAppUi.iContactsHandler->iChangesDB->SetMessageType(tempstr);

		iAppUi.iContactsHandler->SendCompleteI(packNumber);
		return;
	}

	pos = ackMessage.Find(_L("lpr_sync"));
	if (pos != KErrNotFound) {
		iAppUi.iContactsHandler->SendCompleteN();
		return;
	}

	pos = ackMessage.Find(_L("wait_time"));
	if (pos != KErrNotFound) {
		packetNum.Copy(&ackMessage[9], ackMessage.Length() - 9);
		TLex lex(packetNum);
		lex.Val(iPollTimeOut);

		iPoll = ETrue;
		iTimer->After(iPollTimeOut * 1000000);
		return;
	}

	pos = ackMessage.Find(_L("invalid_req"));
	if (pos != KErrNotFound) {
		iAppUi.iContactsHandler->SendAgain();
		return;
	}
}

void CContactsSocketsEngineWriter::CleanupList()
{
	if (iRecordList != NULL)
	{
		TInt i=0;
		CList* ptr = iRecordList;
		TInt cntrec = iRecordList->Count();

		for (i=0; i<cntrec; i++)
		{
			CRecordContactDB* record = (CRecordContactDB*)ptr->GetContent();
			record->Cleanup();
			ptr = ptr->next;
		}

		iRecordList->RemoveAllAndContent();
	}
	iRecordList = NULL;
}

void CContactsSocketsEngineWriter::AcksToServer()
{
	if (iUpdateAO != NULL)
	{
		delete iUpdateAO;
		iUpdateAO = NULL;
	}
	
	CleanupList();

	TBuf<30> tempstr;
	tempstr.Copy(_L("sync"));
	iAppUi.iContactsHandler->iChangesDB->SetMessageType(tempstr);

	iAppUi.iContactsHandler->SendComplete();
	//delete this;
/*#ifndef WAP_PT
	User::LeaveIfError(iSocketServ2.Connect());    	
    TInetAddr addr;
    if (addr.Input(iServerName) == KErrNone)
    {
		ChangeStatus(EWritingACK);
		// Open a TCP socket
		User::LeaveIfError(iSocket2.Open(iSocketServ2, KAfInet, KSockStream, KProtocolInetTcp));

		//Set up address information
		iAddress.SetPort(iPort);
		iAddress.SetAddress(addr.Address());

		// Initiate socket connection
		iSocket2.Connect(iAddress, iStatus);
		iTimer->After(KTimeOut);
		SetActive();
    }
#else
	if (servlet_data)
	{
		delete servlet_data;
	}
	servlet_data = iBuffer.AllocL();

    ChangeStatus(EAcked);
    SendTransaction(iConnection, iCOTransAck, iTransAck);

	TBuf8<50> buf;
	buf.Copy(_L("Sending acks\r\n"));
	iAppUi.Logger(buf);

    iTimer->After(KTimeOut * 20);
    SetActive();
#endif
*/
}
#ifdef WAP_PT
void CContactsSocketsEngineWriter::GetProxyAddress(TUint32 aId, TDes& aAddr)
{
iAppUi.LogText.Copy(_L("\n Default ID: "));
iAppUi.LogText.AppendNumUC(aId);
iAppUi.WriteLogFile(iAppUi.LogText);

//TBuf<100> gwAddr;
//TBuf<100> proxyService;
TBuf<100> proxyServerName;
//TBuf<100> gwAddr;
TUint32 pPort;
TUint32 proxyisp;
TUint32 wapapnid;

CCommsDatabase* commDb = CCommsDatabase::NewL(EDatabaseTypeIAP);
CleanupStack::PushL(commDb);

/*
// open commdb

//WAP_IP_BEARER table, Only works with Nokia6600, There may be something wrong, I can't 
//get PROXY gateway address for devices equipped with other Symbian 7.0s and onwards.
CCommsDbTableView* commView = commDb->OpenTableLC( _L( "WAPIPBearer" )) ; 

if (commView->GotoFirstRecord() == KErrNone)
{
do
{ 
commView->ReadTextL(TPtrC(WAP_GATEWAY_ADDRESS), gwAddr);
commView->ReadUintL(TPtrC(WAP_ACCESS_POINT_ID), wapapnid);

iAppUi.LogText.Copy(_L("WAP_GATEWAY_ADDRESS: "));
iAppUi.LogText.Append(gwAddr);
iAppUi.WriteLogFile(iAppUi.LogText);

iAppUi.LogText.Copy(_L("\n WAP_ACCESS_POINT_ID: "));
iAppUi.LogText.AppendNumUC(wapapnid);
iAppUi.WriteLogFile(iAppUi.LogText);


}while (commView->GotoNextRecord() == KErrNone);
}
CleanupStack::PopAndDestroy(commView);
*/
//sql.Format(KProxyBuf);

//PROXY table
CCommsDbTableView* commView = commDb->OpenTableLC( _L( "PROXIES" ));

if (commView->GotoFirstRecord() != KErrNotFound)
{
do
{
proxyServerName.Copy(*commView->ReadLongTextLC(TPtrC(PROXY_SERVER_NAME)));
CleanupStack::PopAndDestroy(1);

//commView->ReadTextL(TPtrC(PROXY_SERVICE_TYPE), proxyService );
commView->ReadUintL(TPtrC(PROXY_ISP), proxyisp );
commView->ReadUintL(TPtrC(PROXY_PORT_NUMBER), pPort ); 
//iAppUi.LogText.Copy(_L("ProxyServiceType: "));
//iAppUi.WriteLogFile(iAppUi.LogText);

//iAppUi.LogText.Copy(proxyService);
//iAppUi.WriteLogFile(iAppUi.LogText);

iAppUi.LogText.Copy(_L("\n PROXY_PORT_NUMBER: "));
iAppUi.LogText.AppendNumUC(pPort);
iAppUi.WriteLogFile(iAppUi.LogText);

iAppUi.LogText.Copy(_L("\n PROXY_ISP: "));
iAppUi.LogText.AppendNumUC(proxyisp);
iAppUi.WriteLogFile(iAppUi.LogText);

iAppUi.LogText.Copy(_L("\n PROXY_SERVER_ADDRESS: "));
iAppUi.LogText.Append(proxyServerName);
iAppUi.WriteLogFile(iAppUi.LogText);

if(aId == proxyisp)
{
iAppUi.LogText.Copy(_L("\n FOUND: "));
iAppUi.WriteLogFile(iAppUi.LogText);

	aAddr.Copy(proxyServerName);

	break;	
}

}while (commView->GotoNextRecord() != KErrNotFound);
}
CleanupStack::PopAndDestroy(2);//commView); 

//CleanupStack::PopAndDestroy(commDb);

iAppUi.LogText.Copy(_L("\n </GETPROXYADDRESS> "));
iAppUi.WriteLogFile(iAppUi.LogText);

//return proxyServerName;
}
#endif
