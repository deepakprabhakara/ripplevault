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
 Module Name				: SocketsEngineWriter.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Sockets Engine that handles network connections and transfer of data to and from server.
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#include <eikgted.h>
#include <es_sock.h>
#include <charconv.h>

//#include "dnsresolver.h"
#include "SocketsEngineWriter.h"
#include "TimeOutTimer.h"
#include "RippleVault.pan"
#include "RippleVaultappui.h"

class CRippleVaultAppUi;
#ifndef __UIQ__
	#include <aknmessagequerydialog.h>
	#include <AknGlobalListQuery.h>
	#include <aknnotewrappers.h> 
	#include  <ripplevault.rsg>
#else
#ifdef __S80__
	#include  <ripplevault_series80.rsg>
#else
	#include  <ripplevault_uiq.rsg>
#endif
#endif

#include <coemain.h>
#include <eikenv.h>
#include <etel.h>
//#include <etelbgsm.h>
#include <AgentClient.h>
#include <cdbcols.h>
#include <commdb.h>
#include <utf.h>
#include <bautils.h> 

#include <badesca.h>

#ifndef __UIQ__
	#include <intconninit.h>
#endif

const TInt CSocketsEngineWriter::KTimeOut = 60000000; // 60 seconds time-out

_LIT16(KHttp,"http://");
#ifndef __SSL6__
#ifdef __SSL7__
_LIT(KTLS1,"TLS1.0");
#endif
#endif
CSocketsEngineWriter* CSocketsEngineWriter::NewL(CRippleVaultAppUi& aAppUi)
    {
	CSocketsEngineWriter* self = CSocketsEngineWriter::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
    }

	
CSocketsEngineWriter* CSocketsEngineWriter::NewLC(CRippleVaultAppUi& aAppUi)
    {
	CSocketsEngineWriter* self = new (ELeave) CSocketsEngineWriter(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
    }


CSocketsEngineWriter::CSocketsEngineWriter(CRippleVaultAppUi& aAppUi)
: CActive(EPriorityStandard),
  iAppUi(aAppUi),
  iServerName(KDefaultServerName),
  iPort(KDefaultPortNumber)
    {
#ifndef __UIQ__
	iWaitDialog = NULL;
#endif
	iRegister = 0;
	iUserStatus = 10;
	ChangeStatus(EIdle);
    }


CSocketsEngineWriter::~CSocketsEngineWriter()
    {
	Cancel();   
	delete iTimer;
	iTimer = NULL;
#ifdef WAP_PT
	iWapServ.Close();
	delete iCap;

	if (iBufCPload)
	{
		delete iBufCPload;
	}
#endif
	}


void CSocketsEngineWriter::ConstructL()
    {
    // Start a timer
	iTimer = CTimeOutTimer::NewL(EPriorityHigh, *this);
	CActiveScheduler::Add(this);
	iDoCancel = EFalse;
#ifndef WAP_PT
	iResolvedLocal = EFalse;
	//iBufCPload = NULL;
#else
	iBufCPload = NULL;
	port=0;
	iWapServ.Connect();
	iCap = CCapCodec::NewL();
	iCap->SetServerSDUSize(150000);
	iCap->SetClientSDUSize(150000);
	iCap->SetProtocolOptions(CCapCodec::EAckHeaders);
#endif
    }

#ifdef WAP_PT
TInt32 CSocketsEngineWriter::DefaultAccessPointId(TUint32& aId)
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

TInt CSocketsEngineWriter::GetGatewayAddress(TDes8& addr)
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

				//TUint32 p;
				view->ReadTextL(TPtrC(WAP_GATEWAY_ADDRESS), tmp);
				//view->ReadUintL(TPtrC(WAP_PROXY_PORT), p ); 
				//port = p;

				iAppUi.LogText.Copy(_L("Port="));
				iAppUi.WriteLogFile(iAppUi.LogText);

				iAppUi.LogText.AppendNum(port);
				iAppUi.WriteLogFile(iAppUi.LogText);

				iAppUi.LogText.Copy(_L("IP="));
				iAppUi.WriteLogFile(iAppUi.LogText);

				iAppUi.LogText.Copy(tmp);
				iAppUi.WriteLogFile(iAppUi.LogText);

				if(tmp.Compare(_L("0.0.0.0")) == 0)
				{
					iAppUi.LogText.Copy(_L("GetGatewayAddress 4\n"));
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

void CSocketsEngineWriter::MakeContentLength(TUint size, TDes8& header)
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

void CSocketsEngineWriter::SendTransaction(RWSPCOConn& aConn, RWSPCOTrans& aCOTrans, RWSPCOTrans& aTrans)
{
    // create transaction
    TBuf<100> uri;
    uri.Zero();
    uri.Copy(_L("http://"));
    uri.Append(iServerName);
  
	if (iPort != 80) {
		uri.Append(_L(":"));
		uri.AppendNum(iPort);
	}
	if (iRegister == 1)
	{
		uri.Append(iAppUi.iRegUrl); // hardcoded
	}
	else if (iRegister == 2)
	{
		//uri.Append(iAppUi.RegUrl); // hardcoded
	}
	else if (iRegister == 3 || iRegister == 9)
	{
		uri.Append(iAppUi.iSharingUrl); // hardcoded
	}
	else if (iRegister == 5)
	{
		uri.Append(iAppUi.iAlbumUrl); // hardcoded
		iAppUi.NetworkStatusNotification(NETWORK_STATUS_SYNCHING_ALBUMS);
	}
	else if (iRegister == 6)
	{
		uri.Append(iAppUi.iStatusUrl); // hardcoded
		iAppUi.NetworkStatusNotification(NETWORK_STATUS_CHECKING_ACCOUNT);
	}
	else if (iRegister == 7)
	{
		uri.Append(iAppUi.iSmsUrl); // hardcoded
		iAppUi.NetworkStatusNotification(NETWORK_STATUS_UPLOADING_SMS);
	}
	else if (iRegister == 8)
	{
		uri.Append(iAppUi.iRegUrl); // hardcoded
	}
	else
	{
		uri.Append(iAppUi.iPublishUrl);
		iAppUi.NetworkStatusNotification(NETWORK_STATUS_UPLOADING_MEDIA);
	}

	TBuf8<64> head;
    head.Zero();

    head.Append('\x91'); // content type opcode
    head.Append('\x92'); // content type value
    head.Append('\x8d'); // Content-length opcode

	if (iRegister == 1 || iRegister == 3 || iRegister == 5 || iRegister == 6 || iRegister == 8 || iRegister == 9)
	{
		iAppUi.WriteLogFile(iBuffer);

		//MakeContentLength(iAppUi.servlet_data->Des().Length(), head); // generate content length
		MakeContentLength(iBuffer.Length(), head); // generate content length

		//aConn.CreateTransaction(RWAPConn::EPost, uri, head, iAppUi.servlet_data->Des(), aTrans);
		aConn.CreateTransaction(RWAPConn::EPost, uri, head, iBuffer, aTrans);
		aConn.GetEvent(iEvent, aCOTrans, iStatus);
	}
	else if(iRegister == 7)
	{
		if (iAppUi.GetSMSPayloadBuffer(&iBufCPload, iSmsbackupType) == KErrNone)
		{
    		MakeContentLength(iBufCPload->Des().Length(), head); // generate content length

			aConn.CreateTransaction(RWAPConn::EPost, uri, head, iBufCPload->Des(), aTrans);
			aConn.GetEvent(iEvent, aCOTrans, iStatus);
    	}
	}
	else
	{
		if (iAppUi.GetPayloadBuffer(&iBufCPload) == KErrNone)
		{
			MakeContentLength(iBufCPload->Des().Length(), head); // generate content length

			aConn.CreateTransaction(RWAPConn::EPost, uri, head, iBufCPload->Des(), aTrans);
			aConn.GetEvent(iEvent, aCOTrans, iStatus);
    	}
    }
}
#endif // ifdef WAP_PT

/*
#ifndef WAP_PT
TInt CSocketsEngineWriter::ResolveLocalDNS()
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

void CSocketsEngineWriter::ConnectToServer()
{
	iAppUi.iConnection = 1;

	InitializeProgressBars();

#ifndef WAP_PT    

#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL CTS111\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	User::LeaveIfError(iSocketServ.Connect());
	ChangeStatus(EConnecting);

	if (iServerName.Find(KHttp) != KErrNotFound)
	{
		iServerName.Delete(0, 7); // length of "http://"
	}

#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL CTS222\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);

		iAppUi.LogText.Copy(_L("serverip="));
		iAppUi.LogText.Append(iServerName);
		iAppUi.LogText.Append(_L("\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);

		iAppUi.LogText.Copy(_L("RunL CTS222\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);

#endif

	TInetAddr addr;	
	if (addr.Input(iServerName) == KErrNone)// || ResolveLocalDNS() == KErrNone)
	{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL CTS333\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		Connect();
		return;
	}

	iResolvedLocal = EFalse;
	// Initiate DNS
	User::LeaveIfError(iResolver.Open(iSocketServ, KAfInet, KProtocolInetTcp));
	
	// DNS request for name resolution
	iResolver.GetByName(iServerName, iNameEntry, iStatus);
	
	ChangeStatus(ELookingUp);
	// Request time out
	iTimer->After(KTimeOut);
	SetActive();
#else
    ChangeStatus(EConnecting);

	iAppUi.NetworkStatusNotification(NETWORK_STATUS_CONNECTING);	

    _LIT8(headers, "");

    TBuf8<40> addr;
	if (GetGatewayAddress(addr) == KErrNone)
    {
		//addr.Copy(_L("100.1.200.99")); // hardcoded to Airtel
		
    	//TInt port=9201;
		//TInt port=8080;
		if(port == 0)
			port=9201;
		//port = 80;
    	User::LeaveIfError(iConnection.Open(iWapServ, addr, port, 0, EIP, EFalse));
    	TInt ret = iConnection.Connect(headers, iCap);

    	iConnection.GetEvent(iEvent, iCOTrans, iStatus);
		iTimer->After(KTimeOut * 2);
    	SetActive();
    }
    else
    {
	
		iAppUi.NetworkStatusNotification(NETWORK_STATUS_FAILED);
		iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_FAILED);
		SocketsError();

    }
#endif	
}

void CSocketsEngineWriter::Connect()
{
#ifndef WAP_PT

#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("RunL C444\n"));
#endif

//    TInt err1=0;
	//User::LeaveIfError(iSocketServ.Connect());
	ChangeStatus(EConnecting);	
	TInetAddr addr;
	if (addr.Input(iServerName) == KErrNone)// || ResolveLocalDNS() == KErrNone)
	{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL C555\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

        // Open a TCP socket
        //iSocket = new RSocket;
		//iAppUi.iServerAddr.Copy(iServerName);
        User::LeaveIfError(iSocket.Open(iSocketServ, KAfInet, KSockStream, KProtocolInetTcp));
		
#ifdef __SSL6__
   		iSocket.SetOpt( KSoSecureSocket, KSolInetSSL );
#endif
        // Set up address information
		iAddress.SetPort(iPort);
		iAddress.SetAddress(addr.Address());
        // Initiate socket connection
		iAppUi.NetworkStatusNotification(NETWORK_STATUS_CONNECTING);

		iSocket.Connect(iAddress, iStatus);

#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL C666\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		if(iRegister == 1 || iRegister == 8 || iRegister == 10)
		{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL C777\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

			ChangeStatus(ERegistering);
		}
		else if(iRegister == 2)
		{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL C888\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif
			ChangeStatus(ECharging);
		}
		else if(iRegister == 3 || iRegister == 9)
		{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL C999\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif
			ChangeStatus(ESharing);
		}
//		else if(iRegister == 4)
//		{
//			ChangeStatus(ESynchingAlbum);
//		}
		else if(iRegister == 5)
		{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL C1111\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

			ChangeStatus(EEditSynchingAlbum);
		}
		else if(iRegister == 6)
		{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL C2222\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif
			ChangeStatus(ESubStatus);
		}
		else if(iRegister == 7)
		{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL C3333\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif
			ChangeStatus(ESmsData);
		}
/*		else if(iUpGrade)
		{			
			iBuffer.Copy(_L(""));
			iData.Copy(_L(""));

#ifndef	__UIQ__			
			iWaitDialog = new(ELeave)CAknWaitDialog( (REINTERPRET_CAST(CEikDialog**,&iWaitDialog)));
			iWaitDialog->SetTone( CAknNoteDialog::EConfirmationTone ); 
			
			if(iUpGrade == 1)
				//iWaitDialog->SetTextL(_L("Checking Version"));
				iWaitDialog->SetTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_UPGRADE_CHECKING_PROGRESS)));
			else if(iUpGrade == 2)
				//iWaitDialog->SetTextL(_L("Downloading.."));
				iWaitDialog->SetTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_UPGRADE_DOWNLOAD_PROGRESS)));
			
			iWaitDialog->ExecuteLD(R_WAITNOTE_DIALOG);

#endif

			if(iUpGrade == 1)
				ChangeStatus(EUpGradeVerWrite);
			else if(iUpGrade == 2)
				ChangeStatus(EUpGradeWrite);
		}*/
		else
		{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL C4444\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif
			ChangeStatus(EWriting);
		}

#ifndef __SSL6__
#ifdef __SSL7__
		ChangeStatus(ESecureSocket);
#endif
#endif
        
		iTimer->After(KTimeOut);
		SetActive();
	}
	else
	{
		//iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_WRONGSERVERIP);
		ChangeStatus(EIdle);
		SocketsError();
	}
#endif // WAP_PT
}


void CSocketsEngineWriter::RegisterPhotos()
{    
	iRegister=1;
	
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("RegisterPhotos\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
	ConnectToServer();
}

//void CSocketsEngineWriter::ChargePublish()
//{    
//	iRegister=2;
//	ConnectToServer();
//}

void CSocketsEngineWriter::ShareFolders()
{    
	iRegister=3;
	ConnectToServer();
}

//void CSocketsEngineWriter::SynchAlbums()
//{    
//	iRegister=4;
//	ConnectToServer();
//}

void CSocketsEngineWriter::SynchAlbumsForEditing()
{    
	iRegister=5;
	ConnectToServer();
}

void CSocketsEngineWriter::SubscriptionStatus(TInt aStatus)
{    
	connectionstatus = aStatus;
		
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("SubscriptionStatus\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	if(aStatus == 3)
		iSmsbackupType = iAppUi.iSmsBackupType;
	else if(aStatus == 7)
		iSmsbackupType = iAppUi.iAutoSmsBackupType;

	iRegister=6;
	ConnectToServer();
}

void CSocketsEngineWriter::SendSmsData(TInt smsbackuptype)
{    
	iRegister=7;
	iSmsbackupType = smsbackuptype;
	ConnectToServer();
}

void CSocketsEngineWriter::ChangePassword()
{    
	iRegister=8;
	ConnectToServer();
}

void CSocketsEngineWriter::UnShareFolders()
{    
	iRegister=9;
	ConnectToServer();
}

/*void CSocketsEngineWriter::BillingStatus(TInt aStatus)
{    
	connectionstatus = aStatus;
		
	iRegister = 10;
	ConnectToServer();
}*/

void CSocketsEngineWriter::Disconnect()
{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("Disconnect 1\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

#ifndef WAP_PT
#ifndef __SSL6__
#ifdef __SSL7__
	//iSocket.CancelAll();

#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("Disconnect A\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	if(iTlsSocket != NULL)
	{
		iTlsSocket->CancelAll();
		iTlsSocket->Close();
		delete iTlsSocket;
		iTlsSocket = 0;
	}
#endif
#endif

#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("Disconnect B\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	iSocket.Close();

#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("Disconnect C\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	iSocketServ.Close();
	ChangeStatus(EIdle);

#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("Disconnect D\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

#else
/*	
	if(port == 0)
	{
		iAppUi.LogText.Copy(_L("port\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);

		return;
	}
	
	RWSPCOConn::TSessionState aState;
	iConnection.GetSessionState(aState);
	
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("Disconnect 2\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	if (aState == RWSPCOConn::EDisconnected)
		return;

#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("Disconnect 3\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	iConnection.Disconnect();
	iConnection.Close();	
	*/
/*
	TUint32 activeiap;
	CIntConnectionInitiator *iInitiator;
	iInitiator=CIntConnectionInitiator::NewL();

	if (iInitiator->GetActiveIap(activeiap)!=KErrNone){
	iInitiator->TerminateActiveConnection(); 
	}

	delete iInitiator; 
	iInitiator=0;
*/
	//iWapServ.Close();
#endif
}

// from CActive
void CSocketsEngineWriter::DoCancel()
{
    iTimer->Cancel();
	
#ifndef WAP_PT
    // Cancel appropriate request to socket
    switch (iEngineStatus)
	{
	case EIdle:
		break;
	case ELookingUp:		
		iResolver.Cancel();
        iResolver.Close();
        break;		
	case EWriting:
		iSocket.CancelConnect();
		break;
	case ERegistering:
	//case EUpGradeRecv:
	//case EUpGradeVerRecv:
		iSocket.CancelConnect();
		break;
	case ECharging:
	case ESharing:
	case ESmsData:
		iSocket.CancelConnect();
		break;
	case EGetImg:
		iSocket.CancelWrite();
		iSocket.CancelConnect();
		break;
	case EPending:
	case EGetFinalData:
		iSocket.CancelWrite();
		break;
	case EWritten:
		break;
	case EConnecting:
		iSocket.CancelConnect();
		break;
	case EReceived:
	//case EUpGradeEnd:
		iSocket.CancelRecv();
		break;
	default:
		break;
	}

    ChangeStatus(EIdle);
#else
	//iTimer->Cancel();
	
	if (iDoCancel)
		return;

	iDoCancel = ETrue;

	RWSPCOConn::TSessionState aState;
	iConnection.GetSessionState(aState);
	if (aState == RWSPCOConn::EDisconnected)
		return;

#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("DoCancel 1\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	switch (iEngineStatus)
	{
		case EConnecting:
			iConnection.CancelGetEvent();
			//break;
		case ESendRecv:
			iConnection.Disconnect();
			iConnection.Close();
			break;
		default:
			break;
	}
	
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("DoCancel 2\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif      
#endif      
}

// from CActive
void CSocketsEngineWriter::RunL()
{	
	//iAppUi.LogText.Copy(_L("RunL Start\n"));
	//iAppUi.WriteLogFile(iAppUi.LogText);

	iTimer->Cancel(); // Cancel TimeOut timer before completion   

	if (iStatus != KErrNone && iStatus != KErrEof)
	{
		DoCancel();

#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("\niStatus="));
		iAppUi.LogText.AppendNum(iStatus.Int());
		iAppUi.LogText.Append(_L("\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);

		iAppUi.LogText.Copy(_L("RunL 1\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		if(iRegister != 1){
		iAppUi.NetworkStatusNotification(NETWORK_STATUS_FAILED);
		iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_FAILED);
		}
		SocketsError();

		return;		
	}

	//iAppUi.LogText.Copy(_L("RunL 4\n"));
	//iAppUi.WriteLogFile(iAppUi.LogText);

#ifndef WAP_PT
	/*if (iStatus == KErrNone && (iEngineStatus == EUpGradeEnd ))
	{
		// Handle data		
		if(iUpGrade == 2)
		{
			iUpGrade = 0;
			ProcessFile();
		}		
		
		iUpGradeFile.Write(iBuffer);
		iUpGradeFile.Flush();

		ChangeStatus(EUpGradeRecv);

	}
	else if (iStatus == KErrEof && iEngineStatus == EUpGradeEnd)
	{
		if(iUpGrade == 2)
		{
			iUpGrade = 0;
			ProcessFile();
		}
		iUpGradeFile.Write(iBuffer);
		iUpGradeFile.Flush();
	}*/
//#endif

		//iAppUi.LogText.Copy(_L("RunL 111\n"));
		//iAppUi.WriteLogFile(iAppUi.LogText);

//#ifndef WAP_PT
	switch(iEngineStatus)
	{
	case ELookingUp:
		iResolver.Close();
		if (iStatus == KErrNone)
		{
			// DNS look up successful
			TNameRecord nameRecord;

			nameRecord = iNameEntry();
			// Extract domain name and IP address from name record
			TBuf<20> ipAddr;
			TInetAddr::Cast(nameRecord.iAddr).Output(ipAddr);
			// And connect to the IP address            
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("\nip address="));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif
#ifdef __LOGME__
		iAppUi.LogText.Copy(ipAddr);
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

			iServerName.Copy(ipAddr);
			Connect();
		}		    
		break;
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

			if(iRegister == 1 || iRegister == 8 || iRegister == 10)
			{
			iAppUi.LogText.Copy(_L("RunL C777\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);

				ChangeStatus(ERegistering);
			}
			else if(iRegister == 2)
			{
			iAppUi.LogText.Copy(_L("RunL C888\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);

				ChangeStatus(ECharging);
			}
			else if(iRegister == 3 || iRegister == 9)
			{
			iAppUi.LogText.Copy(_L("RunL C999\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);

				ChangeStatus(ESharing);
			}
			else if(iRegister == 5)
			{
			iAppUi.LogText.Copy(_L("RunL C1111\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);

				ChangeStatus(EEditSynchingAlbum);
			}
			else if(iRegister == 6)
			{
			iAppUi.LogText.Copy(_L("RunL C2222\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);

				ChangeStatus(ESubStatus);
			}
			else if(iRegister == 7)
			{
			iAppUi.LogText.Copy(_L("RunL C3333\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);

				ChangeStatus(ESmsData);
			}
			else
			{
			iAppUi.LogText.Copy(_L("RunL C4444\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);

				ChangeStatus(EWriting);
			}

			SetActive();
		break;
#endif
#endif
	case ERegistering:
	case ECharging:
	case ESharing:
	case EEditSynchingAlbum:
	case ESubStatus:
		{			
#ifdef __LOGME__			
			iAppUi.LogText.Copy(_L("ESubStatus\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);
#endif
			if(iRegister == 1)
			{
				iTimer->After(10000000);
			}
			else{
			iTimer->After(KTimeOut);
			}
//#ifndef __UIQ__
			//iBuffer.Copy(*(iAppUi.servlet_data));
//#endif
			if(iRegister == 5)
				iAppUi.NetworkStatusNotification(NETWORK_STATUS_SYNCHING_ALBUMS);
			else if(iRegister == 6)
				iAppUi.NetworkStatusNotification(NETWORK_STATUS_CHECKING_ACCOUNT);
			
#ifdef __SSL6__
			iSocket.Write(iBuffer, iStatus); // Initiate actual write
#else
#ifdef __SSL7__
			iTlsSocket->Send( iBuffer, iStatus, iDummyLength );
#else
			iSocket.Write(iBuffer, iStatus); // Initiate actual write
#endif
#endif

			ChangeStatus(EWritten);
			SetActive();
		}
		break;
	case ESmsData:
		{			
			iTimer->After(KTimeOut);
			iAppUi.GetSMSProtocolDataXML(iBuffer, 512, iSmsbackupType);
//#ifndef __UIQ__
			//iBuffer.Copy(*(iAppUi.servlet_data));
//#endif
			iAppUi.NetworkStatusNotification(NETWORK_STATUS_UPLOADING_SMS);

#ifdef __SSL6__
			iSocket.Write(iBuffer, iStatus); // Initiate actual write
#else
#ifdef __SSL7__
			iTlsSocket->Send( iBuffer, iStatus, iDummyLength );
#else
			iSocket.Write(iBuffer, iStatus); // Initiate actual write
#endif
#endif
			//ChangeStatus(EWritten);
			ChangeStatus(EGetMoreSmsData);
			SetActive();
		}		
	break;
	
	case EGetMoreSmsData:
		{		
            iAppUi.GetMoreSMSDataFromFile(iData, 300, iSmsbackupType);

			if (iData.Length() < 300)
            {
#ifdef __LOGME__
				iAppUi.LogText.Copy(_L("SMS written\n"));
				iAppUi.WriteLogFile(iAppUi.LogText);
#endif

				ChangeStatus(EWritten);
            }
			iTimer->Cancel();
			iTimer->After(KTimeOut);

#ifdef __SSL6__
			iSocket.Write(iData, iStatus); // Initiate actual write
#else
#ifdef __SSL7__
			iTlsSocket->Send( iData, iStatus, iDummyLength );
#else
			iSocket.Write(iData, iStatus); // Initiate actual write
#endif
#endif
			
			SetActive();
		}
		break;
		
	case EWriting:
		{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL CSockets: EWriting\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif
			
			//iAppUi.GetProtocolDataXML(iBuffer, 512, iAlbumnamestatus);

			iAppUi.GetFirstProtocolDataXML(iBuffer, iAlbumnamestatus);

			iCounter = 0;

#ifdef __LOGME__
			iAppUi.LogText.Copy(_L("\n\npacketnum="));
			iAppUi.LogText.AppendNum(iPacketNumber);
			iAppUi.LogText.Append(_L("\n\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);
#endif

            if (iBuffer.Length() == 0) // no file to upload
            {
                //no file to upload!
                ChangeStatus(EWritten);
            	TRequestStatus* status = &iStatus;
	            User::RequestComplete(status, KErrNone);
                SetActive();
            }
            else
            {
			    iTimer->Cancel();
			    iTimer->After(KTimeOut);

				iAppUi.NetworkStatusNotification(NETWORK_STATUS_UPLOADING_MEDIA);

#ifdef __LOGME__
				iAppUi.WriteLogFile(iBuffer);
#endif

#ifdef __SSL6__
			iSocket.Write(iBuffer, iStatus); // Initiate actual write
#else
#ifdef __SSL7__
			iTlsSocket->Send( iBuffer, iStatus, iDummyLength );
#else
			iSocket.Write(iBuffer, iStatus); // Initiate actual write
#endif
#endif

                ChangeStatus(EGetImg);
			    SetActive();
            }
			
			/*if (iAppUi.GetPayloadBuffer(&iBufCPload) == KErrNone)
			{
				iTimer->Cancel();
				iTimer->After(KTimeOut);
		
				iAppUi.NetworkStatusNotification(NETWORK_STATUS_UPLOADING_MEDIA);
				
				iSocket.Write(iBufCPload->Des(), iStatus); // Initiate actual write	
				ChangeStatus(EWritten);			
				SetActive();
			}
			else
			{
				iBuffer.Copy(_L(""));
				ChangeStatus(EWritten);
            	TRequestStatus* status = &iStatus;
	            User::RequestComplete(status, KErrNone);
                SetActive();
			}*/

		}
		break;
	case EGetImg:
		{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL CSockets: EGetImg\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif
			
            iAppUi.GetMoreDataFromFile(iData, 1024);
			iCounter++;

			if (iData.Length() < 1024 || iCounter>=PUBLISH_PACKET_SIZE)
            {
				//ChangeStatus(EGetFinalData);
				ChangeStatus(EWritten);
            }
			iTimer->Cancel();
			iTimer->After(KTimeOut);

			//iAppUi.WriteLogFile(iData);

#ifdef __SSL6__
			//iSocket.Write(temp_session, iStatus); // Initiate actual write	
			iSocket.Write(iData, iStatus); // Initiate actual write
#else
#ifdef __SSL7__
			//iTlsSocket->Send( temp_session, iStatus, iDummyLength );
			iTlsSocket->Send( iData, iStatus, iDummyLength );
#else
			//iSocket.Write(temp_session, iStatus); // Initiate actual write	
			iSocket.Write(iData, iStatus); // Initiate actual write
#endif
#endif

			
			SetActive();
		}
		break;
	case EGetFinalData:
		{
			//iAppUi.GetProtocolEndDataXML(iBuffer);

			//iAppUi.GetProtocolDataXML(iBuffer, 512, iAlbumnamestatus);

			iTimer->Cancel();
			iTimer->After(KTimeOut);
			
			//iAppUi.WriteLogFile(temp_session);
#ifdef __SSL6__				
			iSocket.Write(temp_session, iStatus); // Initiate actual write	
#else
#ifdef __SSL7__
			iTlsSocket->Send( temp_session, iStatus, iDummyLength );
#else
			iSocket.Write(temp_session, iStatus); // Initiate actual write	
#endif
#endif
			ChangeStatus(EWritten);			
			SetActive();
		}
		break;

    case EWritten:
		{
			if (/*iRegister == 1 ||*/ iRegister == 3 || iRegister == 5 || iRegister == 6 || iRegister == 7 || iRegister == 8 || iRegister == 9 || iRegister == 10) 
			{
#ifdef __LOGME__
				iAppUi.LogText.Copy(_L("EWritten\n"));
				iAppUi.WriteLogFile(iAppUi.LogText);
#endif

				iTimer->Cancel();				
#ifdef __SSL6__
				iSocket.RecvOneOrMore(iBuffer, 0, iStatus, iDummyLength);
#else
#ifdef __SSL7__
				//iTlsSocket->Recv( iBuffer, iStatus );
				iBuffer.Copy(_L8(""));
				iTlsSocket->RecvOneOrMore( iBuffer, iStatus, iDummyLength);
#else
				iBuffer.Copy(_L8(""));
				iSocket.RecvOneOrMore(iBuffer, 0, iStatus, iDummyLength);
#endif
#endif
				iTimer->After(KTimeOut);

				ChangeStatus(EReceived);
				SetActive();				
			}
			else if(iRegister == 1)
			{
#ifdef __LOGME__
				iAppUi.LogText.Copy(_L("EWritten\n"));
				iAppUi.WriteLogFile(iAppUi.LogText);
#endif
				iTimer->Cancel();				
				
#ifdef __SSL6__				
				iSocket.RecvOneOrMore(iBuffer, 0, iStatus, iDummyLength);
#else
#ifdef __SSL7__
				//iTlsSocket->Recv( iBuffer, iStatus );
				iBuffer.Copy(_L8(""));
				iTlsSocket->RecvOneOrMore( iBuffer, iStatus, iDummyLength);
#else
				iSocket.RecvOneOrMore(iBuffer, 0, iStatus, iDummyLength);
#endif
#endif
				
				iTimer->After(20000000);

				ChangeStatus(EReceived);
				SetActive();				
			}
/*			else if (iRegister == 7) 
			{
#ifndef __UIQ__			
				if (iWaitDialog != NULL)
				{
					iWaitDialog->ProcessFinishedL();
				}
#endif				

				iAppUi.SendSmsFileComplete(iSmsbackupType);

				if(iSmsbackupType == 1)				
					iAppUi.UpdateSmsTimestamp();

				//Disconnect();
				ChangeStatus(EConnecting);
				iAppUi.CloseSocketSMS();
				//iAppUi.SendSmsFileComplete(iSmsbackupType);
			}*/
			else
			{
#ifdef __LOGME__
				iAppUi.LogText.Copy(_L("Photos Written\n"));
				iAppUi.WriteLogFile(iAppUi.LogText);
#endif				
				//Disconnect();
				//ChangeStatus(EConnecting);
				//iAppUi.SendFileComplete(iAlbumnamestatus);	
				iTimer->Cancel();				

#ifdef __SSL6__				
				iSocket.RecvOneOrMore(iBuffer, 0, iStatus, iDummyLength);
#else
#ifdef __SSL7__
				//iTlsSocket->Recv( iBuffer, iStatus );
				iBuffer.Copy(_L8(""));
				iTlsSocket->RecvOneOrMore( iBuffer, iStatus, iDummyLength);
#else
				iSocket.RecvOneOrMore(iBuffer, 0, iStatus, iDummyLength);
#endif
#endif
				iTimer->After(KTimeOut*2);

				ChangeStatus(EReceived);
				SetActive();								
				
			}
		}
		break;	
	case EReceived:
		{
#ifdef __LOGME__
			iAppUi.LogText.Copy(_L("EReceived\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);
#endif
//			if(iRegister!=1)
//iBuffer.Copy(_L("<?xml version='1.0'?><!DOCTYPE html PUBLIC '-//WAPFORUM//DTD XHTML Mobile 1.0//EN''http://www.wapforum.org/DTD/xhtml-mobile10.dtd'><html xmlns='http://www.w3.org/1999/xhtml'><head><title>The request failed</title></head><body><p><big>Service temporarily unavailable.</big></p><p><i>Technical description:</i><br/>Due to technical difficulties.</p></body></html>"));
#ifdef __LOGME__
			iAppUi.WriteLogFile(iBuffer);
#endif
			ProcessServerResponse();

		}	
		break;
	default:
		break;
	};
	
#else // WAP Stack code
#ifdef __LOGME__
			iAppUi.LogText.Copy(_L("WAP RunL\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);
#endif


	switch(iEngineStatus)
	{
	case EConnecting:
        {
#ifdef __LOGME__
			iAppUi.LogText.Copy(_L("WAP RunL EConnecting\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);
#endif

			if (iEvent() == RWSPCOConn::EDisconnect_ind_s ||
				iEvent() == RWSPCOConn::EAbort_ind_t ||
				iEvent() == RWSPCOConn::EException_ind_e)
			{

				DoCancel();
				
				iAppUi.NetworkStatusNotification(NETWORK_STATUS_FAILED);
				iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_FAILED);
				SocketsError();

			}
			else if (iEvent()==RWSPCOConn::EConnect_cnf_s)
            {

				//iBuffer.Copy(_L8(""));
                ChangeStatus(ESendRecv);				
                
				SendTransaction(iConnection, iCOTrans, iTransSynch);
                iTimer->After(KTimeOut*5);
               	SetActive();
            }
			else
			{
				/*TBuf8<30> buf;
				buf.Copy(_L("Runl EConnecting"));
				buf.AppendNum(iEvent());
				buf.Append(_L("\r\n"));*/
				DoCancel();

				iAppUi.NetworkStatusNotification(NETWORK_STATUS_FAILED);
				iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_FAILED);
				SocketsError();
			}
        }
        break;
	case ESendRecv:
		{
#ifdef __LOGME__
			iAppUi.LogText.Copy(_L("WAP RunL ESendRecv\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);
#endif

    	//iBuffer.SetLength(0);
		iBuffer.Copy(_L(""));


        if (iEvent() == RWSPCOConn::EDisconnect_ind_s ||
            iEvent() == RWSPCOConn::EAbort_ind_t ||
		    iEvent() == RWSPCOConn::EException_ind_e)
        {
            //CEikonEnv::Static()->AlertWin(_L("Disconnect"));
            
			DoCancel();
			
			iAppUi.NetworkStatusNotification(NETWORK_STATUS_FAILED);
			iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_FAILED);
			SocketsError();

        }
        else if (iEvent()==RWSPCOConn::EMethodResult_ind_t)
        {

			TBuf8<1024> buffer;
            // got response
			TInt ret;

			//if(iRegister !=0 && iRegister!=7)
			if(1)
			{
				ret=iCOTrans.GetData(buffer, RWSPCOTrans::EResultBody);

				iBuffer.Copy(buffer);
				
				while ( ret == RWAPConn::EMoreData )
				{
					
					ret=iCOTrans.GetData(buffer, RWSPCOTrans::EResultBody);

					if (iBuffer.Length() + buffer.Length() > iBuffer.MaxLength())
					{
						break;
					}
					iBuffer.Append(buffer);
		
				}
				
				iCOTrans.Acknowledge(_L8(""));
			}
			iTransSynch.Release();
			


			ProcessServerResponse();

		} 
        else
        {
			iConnection.GetEvent(iEvent, iCOTrans, iStatus);
			iTimer->After(KTimeOut);
			SetActive();
        }
	}
    break;
	case EIdle:
		break;
	} // switch
#endif // WAP_PT
//	iAppUi.LogText.Copy(_L("RunL END\n"));
//	iAppUi.WriteLogFile(iAppUi.LogText);

}

void CSocketsEngineWriter::TimerExpired()
{
	if(iRegister != 1){
	iAppUi.NetworkStatusNotification(NETWORK_STATUS_FAILED);
	iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_TIMEDOUT);	
	}
	
	DoCancel();    
	
	//SocketsError();
}


void CSocketsEngineWriter::ChangeStatus(TSocketsEngineState aNewStatus)
{
    iEngineStatus = aNewStatus;
}

//void CSocketsEngineWriter::Print(const TDesC& aDes)
//{
//    // Print some text on the console
//	//    iConsole.PrintNotify(aDes, CEikGlobalTextEditor::EItalic);
//}

void CSocketsEngineWriter::SetServerName(const TDesC& aName)
{
    iServerName.Copy(aName);
}

const TDesC& CSocketsEngineWriter::ServerName() const
{
    return iServerName;
}

void CSocketsEngineWriter::SetPort(TInt aPort)
{
    iPort = aPort;
}

TInt CSocketsEngineWriter::Port() const
{
    return iPort;
}

void CSocketsEngineWriter::Register(TInt aRegistered)
{
	iRegister = TRUE;
	ConnectToServer();
}

/*
#ifdef WAP_PT
void CSocketsEngineWriter::ProcessRegisterResponse()
{
#ifndef __UIQ__
	if (iWaitDialog)
	{
		iWaitDialog->ProcessFinishedL();
	}
#endif
	_LIT8(ACK_O,"<ack>");
	_LIT8(ACK_C,"</ack>");
	_LIT8(SUCCESS,"suc_reg");
	//_LIT8(SUCCESS,"success");

	TInt start;
	TInt end;

	TBuf8<100> ack_message;
	
	start=iBuffer.Find(ACK_O);

	if (start >= 0)
	{
		temp_session.Copy(&iBuffer[start], iBuffer.Length()-start);
		end=temp_session.Find(ACK_C);

		if (start+5 >= 0)
		{
			ack_message.Copy(&iBuffer[start+5], end-5);
		}
	}
	else
	{	
		//ack_message.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_REGISTER_GPRSUNAVAILABLE)));
		iAppUi.ShowMessageBoxFromResource(R_TEXT_REGISTER_GPRSUNAVAILABLE);
	}

	if(ack_message.Compare(SUCCESS) == 0)
	{
		iAppUi.WritRegisteredFile();
		iAppUi.UserRegistered();
		iAppUi.EnableUpload(NOT_REGISTERED);

	}
	else
	{
		TBuf<200> msg;
		msg.Copy(ack_message);
		if (msg.Compare(_L("fail_reg")) == 0)
				//msg.Copy(_L("Incorrect Userid/password"));
				msg.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_INVALID_USERID_PASSWORD)));
#ifndef __UIQ__
		CAknInformationNote* note = new(ELeave) CAknInformationNote;
		note->ExecuteLD(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_INVALID_USERID_PASSWORD)));
#else
		CEikonEnv::Static ()->InfoWinL(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_INVALID_USERID_PASSWORD)));
#endif
	}
	iAppUi.SetTitle();
}
#endif // WAP_PT
*/

void CSocketsEngineWriter::EditAlbumNames()
{	
	//iBuffer.Copy(_L("<synch><existing>123</existing><existing>456</existing><edit><old>help</old><new>helpme</new><time>2.30</time></edit><edit><old>nature</old><new>Art</new><time>3.30</time></edit><del><name>123</name><time>4.30</time></del><del><name>456</name><time>5.30</time></del></synch>"));

	//iAppUi.LogText.Copy(_L("Album Buffer="));
	//iAppUi.WriteLogFile(iAppUi.LogText);

	//iAppUi.WriteLogFile(iBuffer);

	//iAppUi.LogText.Copy(_L("\n"));
	//iAppUi.WriteLogFile(iAppUi.LogText);

	_LIT8(SYNCH_O,"<synch>");
	_LIT8(SYNCH_C,"</synch>");
	
	_LIT8(ADD_O,"<existing>");
	_LIT8(ADD_C,"</existing>");

	_LIT8(EDIT_O,"<edit>");
	_LIT8(EDIT_C,"</edit>");
	
	_LIT8(OLD_O,"<old>");
	_LIT8(OLD_C,"</old>");

	_LIT8(NEW_O,"<new>");
	_LIT8(NEW_C,"</new>");

	_LIT8(TIME_O,"<time>");
	_LIT8(TIME_C,"</time>");

	_LIT8(DEL_O,"<del>");
	_LIT8(DEL_C,"</del>");

	_LIT8(NAME_O,"<name>");
	_LIT8(NAME_C,"</name>");

	_LIT8(NUMPHOTOS_O,"<numphotos>");
	_LIT8(NUMPHOTOS_C,"</numphotos>");

	_LIT8(NUMVIDEOS_O,"<numvideos>");
	_LIT8(NUMVIDEOS_C,"</numvideos>");

	_LIT8(NUMALBUMSHARE_O,"<numalbumshare>");
	_LIT8(NUMALBUMSHARE_C,"</numalbumshare>");

	_LIT8(NUMSMS_O,"<numsms>");
	_LIT8(NUMSMS_C,"</numsms>");

	TInt start;
	TInt end;

	TInt start1;
	TInt end1;					
	
	TBuf8<300> albumname;
	temp_session.Copy(iBuffer);
	
	start=temp_session.Find(SYNCH_O);
	end=temp_session.Find(SYNCH_C);
	
	TBuf8<200> tempalbumn;	
	TBuf<200> existingname;	
	TBuf<200> oldname;	
	TBuf<200> newname;	
	TBuf<200> delname;	
	TBuf<200> timebuf;	
	TBuf<20> numphotos;	
	TBuf<20> numvideos;
	TBuf<20> numalbumshare;
	TBuf<20> numsms;
		
	CnvUtfConverter converter;
	
	while (start >= 0)
	{		
///////////		
		start1=temp_session.Find(NUMPHOTOS_O);
		end1=temp_session.Find(NUMPHOTOS_C);

		while(start1 >=0)
		{
			albumname.Copy(&temp_session[start1+11], end1 - start1 - 11);
			numphotos.Copy(albumname);

			end1=11+albumname.Length()+12;

			temp_session.Delete(start1,end1);

			start1=temp_session.Find(NUMPHOTOS_O);
			end1=temp_session.Find(NUMPHOTOS_C);

		}	
		start=temp_session.Find(NUMPHOTOS_O);

		TLex nump(numphotos);
		TInt pcount;
		TInt np=nump.Val(pcount);
		if(np!=KErrNone) pcount=0;
		
////////////
		start1=temp_session.Find(NUMVIDEOS_O);
		end1=temp_session.Find(NUMVIDEOS_C);

		while(start1 >=0)
		{
			albumname.Copy(&temp_session[start1+11], end1 - start1 - 11);
			numvideos.Copy(albumname);
					
			end1=11+albumname.Length()+12;

			temp_session.Delete(start1,end1);

			start1=temp_session.Find(NUMVIDEOS_O);
			end1=temp_session.Find(NUMVIDEOS_C);
		}	

		start=temp_session.Find(NUMVIDEOS_O);

		TLex numv(numvideos);
		TInt vcount;
		TInt nv=numv.Val(vcount);
		if(nv!=KErrNone) vcount=0;

///////////////////////////////////////
		start1=temp_session.Find(NUMALBUMSHARE_O);
		end1=temp_session.Find(NUMALBUMSHARE_C);

		while(start1 >=0)
		{
			albumname.Copy(&temp_session[start1+15], end1 - start1 - 15);
			numalbumshare.Copy(albumname);
					
			end1=15+albumname.Length()+16;

			temp_session.Delete(start1,end1);
			start1=temp_session.Find(NUMALBUMSHARE_O);
			end1=temp_session.Find(NUMALBUMSHARE_C);
		}	

		start=temp_session.Find(NUMALBUMSHARE_O);

		TLex numas(numalbumshare);
		TInt ascount;
		TInt nas=numas.Val(ascount);
		if(nas!=KErrNone) ascount=0;
		
////////////////
		start1=temp_session.Find(NUMSMS_O);
		end1=temp_session.Find(NUMSMS_C);

		while(start1 >=0)
		{
			albumname.Copy(&temp_session[start1+8], end1-start1-8);
			numsms.Copy(albumname);
					
			end1=8+albumname.Length()+9;

			temp_session.Delete(start1,end1);
			start1=temp_session.Find(NUMSMS_O);
			end1=temp_session.Find(NUMSMS_C);
		}	

		start=temp_session.Find(NUMSMS_O);

		TLex nsms(numsms);
		TInt smscount;
		TInt nsms1=nsms.Val(smscount);
		if(nsms1 !=KErrNone ) smscount=0;		
		
///////////////
		start1=temp_session.Find(EDIT_O);
		end1=temp_session.Find(EDIT_C);

		while(start1 >=0)
		{
			TInt start2;
			TInt end2;					
		
			albumname.Copy(&temp_session[start1+6], end1 - start1 - 6);
			oldname.Copy(albumname);
			newname.Copy(albumname);
			timebuf.Copy(albumname);
						
			//old name
			tempalbumn.Copy(oldname);

			start2=tempalbumn.Find(OLD_O);
			end2=tempalbumn.Find(OLD_C);

			//oldname.Copy(&oldname[start2+5], end2 - start2 - 5);
			tempalbumn.Copy(&tempalbumn[start2+5], end2 - start2 - 5);
			converter.ConvertToUnicodeFromUtf8(oldname,tempalbumn);

			//new name
			tempalbumn.Copy(newname);

			start2=tempalbumn.Find(NEW_O);
			end2=tempalbumn.Find(NEW_C);

			//newname.Copy(&newname[start2+5], end2 - start2 - 5);
			tempalbumn.Copy(&tempalbumn[start2+5], end2 - start2 - 5);
			converter.ConvertToUnicodeFromUtf8(newname,tempalbumn);
		
			//time
			tempalbumn.Copy(timebuf);

			start2=tempalbumn.Find(TIME_O);
			end2=tempalbumn.Find(TIME_C);

			timebuf.Copy(&timebuf[start2+6], end2 - start2 - 6);

			end1=6+albumname.Length()+7;

			temp_session.Delete(start1,end1);

			if(iAppUi.RegFileExists()){
				iAppUi.EditFolderFromDB(oldname, newname);
			}
			start1=temp_session.Find(EDIT_O);
			end1=temp_session.Find(EDIT_C);

		}		

		start=temp_session.Find(EDIT_O);
		
		start1=temp_session.Find(DEL_O);
		end1=temp_session.Find(DEL_C);

		while(start1 >=0)
		{
			TInt start2;
			TInt end2;					
		
			albumname.Copy(&temp_session[start1+5], end1 - start1 - 5);
		
			delname.Copy(albumname);			
			timebuf.Copy(albumname);						
			
			//del name
			tempalbumn.Copy(delname);

			start2=tempalbumn.Find(NAME_O);
			end2=tempalbumn.Find(NAME_C);

			//delname.Copy(&delname[start2+6], end2 - start2 - 6);
			tempalbumn.Copy(&tempalbumn[start2+6], end2 - start2 - 6);
			converter.ConvertToUnicodeFromUtf8(delname,tempalbumn);

			//time
			tempalbumn.Copy(timebuf);

			start2=tempalbumn.Find(TIME_O);
			end2=tempalbumn.Find(TIME_C);

			timebuf.Copy(&timebuf[start2+6], end2 - start2 - 6);

			end1= 5+albumname.Length()+6;

			temp_session.Delete(start1,end1);	

			start1=temp_session.Find(DEL_O);
			end1=temp_session.Find(DEL_C);			

			if(iAppUi.RegFileExists()){
				iAppUi.DeleteFolderFromDB(delname);
			}

		}	
		start=temp_session.Find(DEL_O);


		start1=temp_session.Find(ADD_O);
		end1=temp_session.Find(ADD_C);

		while(start1 >=0)
		{
//			TInt start2;
//			TInt end2;					

			albumname.Copy(&temp_session[start1+10], end1 - start1 - 10);
			//existingname.Copy(albumname);
			converter.ConvertToUnicodeFromUtf8(existingname,albumname);
					
//			iAppUi.WriteLogFile(albumname);
			/*//old name
			tempalbumn.Copy(existingname);

			start2=tempalbumn.Find(ADD_O);
			end2=tempalbumn.Find(ADD_C);

			existingname.Copy(&existingname[start2+10], end2 - start2 - 10);*/

			/*//time
			tempalbumn.Copy(timebuf);

			start2=tempalbumn.Find(TIME_O);
			end2=tempalbumn.Find(TIME_C);

			timebuf.Copy(&timebuf[start2+6], end2 - start2 - 6);*/

			end1=10+albumname.Length()+11;

			temp_session.Delete(start1,end1);			

			if(existingname.Compare(_L("")) != 0)
			iAppUi.CheckAndSaveFolderToDB(existingname);

			start1=temp_session.Find(ADD_O);
			end1=temp_session.Find(ADD_C);
		}	
///
		start=temp_session.Find(ADD_O);
		
		//if(!iAppUi.RegFileExists())
		//{
			iAppUi.UpdatePhotosVideosCount(pcount, vcount, ascount);
			iAppUi.SaveSmsUploadSettingsToDB(smscount);
		//}
		/*else
		{
			TInt album;
			TInt photos;
			TInt videos;
			TInt albumshares;
			
			iAppUi.GetUploadSettingsFromDB(album, photos, videos, albumshares);

			iAppUi.UpdatePhotosVideosCount(photos, videos, ascount);
		}*/

	}
}

void CSocketsEngineWriter::ParseStatusXml()
{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("ProcessStatusXml\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif
	
	//iBuffer.Copy(_L("<b><sync-timestamp>45432543543</sync-timestamp><user-prof><param><value>userid=purple</value></param><param><value>user_status=suspended</value></param></user-prof><sys-prof><param><value>server=203.208.123.345</value></param><param><value>port=1280</value></param></sys-prof></b>"));
	//iBuffer.Copy(_L("<b><sync-timestamp>454</sync-timestamp><user-prof><param><value>userid=purpleace</value></param></user-prof><sys-prof></sys-prof></b>"));
	//iBuffer.Copy(_L("<b><sync-timestamp>45432543543</sync-timestamp><user-prof><param><value>user_status=suspended</value></param></user-prof><sys-prof></sys-prof></b>"));
	
	_LIT8(BODY_O,"<b>");
	_LIT8(BODY_C,"</b>");

	_LIT8(SYNCH_O,"<last-sync-timestamp>");
	_LIT8(SYNCH_C,"</last-sync-timestamp>");
	
	_LIT8(USER_O,"<user-prof>");
	_LIT8(USER_C,"</user-prof>");

	//_LIT8(SYSTEM_O,"</sys-prof>");
	//_LIT8(SYSTEM_C,"</sys-prof>");
	
	_LIT8(PARAM_O,"<param>");
	_LIT8(PARAM_C,"</param>");

	_LIT8(VALUE_O,"<value>");
	_LIT8(VALUE_C,"</value>");

	TInt start;
	TInt end;

	TInt start1;
	TInt end1;
	
	TInt start2;
	TInt end2;

	TBuf8<30> synctime;
	TBuf8<350> parambuffer;
		
	start=iBuffer.Find(BODY_O);
	end=iBuffer.Find(BODY_C);

	TInt flag = 0;

	while (start >= 0)
	{								
		flag = 1;
		start1=iBuffer.Find(SYNCH_O);
		end1=iBuffer.Find(SYNCH_C);

		if (end1-21 >= 0 && start1+21<iBuffer.Length())
		{
			synctime.Copy(&iBuffer[start1+21], end1 - start1 - 21);
			//TBuf<30> tempbuf;
			//tempbuf.Copy(synctime);
			//CEikonEnv::Static ()->AlertWin(tempbuf);

			//iAppUi.LogText.Copy(_L("synctime="));
			//iAppUi.WriteLogFile(iAppUi.LogText);

			//iAppUi.WriteLogFile(synctime);
			
			//iAppUi.LogText.Copy(_L("\n"));
			//iAppUi.WriteLogFile(iAppUi.LogText);
			
			TBuf<30> synctime1;
			synctime1.Copy(synctime);
			iAppUi.SaveSubStatusSyncTime(synctime1);
		}
		
		iBuffer.Delete(0,end1+22);

		start1=iBuffer.Find(USER_O);
		end1=iBuffer.Find(USER_C);

		while(start1 >= 0)
		{
			start2 = iBuffer.Find(PARAM_O);
			end2 = iBuffer.Find(PARAM_C);

			while (start2 >= 0 )
			{
				parambuffer.Copy(&iBuffer[start2+7], end2 - start2 - 7);

				//iAppUi.LogText.Copy(_L("parambuffer="));
				//iAppUi.WriteLogFile(iAppUi.LogText);

				//iAppUi.WriteLogFile(parambuffer);

				//iAppUi.LogText.Copy(_L("\n"));
				//iAppUi.WriteLogFile(iAppUi.LogText);

				ParseStatusParamXml(parambuffer);

				iBuffer.Delete(0,end2 + 8);

				start2 = iBuffer.Find(PARAM_O);
				end2 = iBuffer.Find(PARAM_C);
			}
			
			iBuffer.Delete(0,end1+11);
			
			start1=iBuffer.Find(USER_O);
			end1=iBuffer.Find(USER_C);
		}

		start=iBuffer.Find(BODY_O);
	}	
	if(!flag)
	{//msisdn_mismatch, imei_mismatch

		start1=iBuffer.Find(VALUE_O);
		end1=iBuffer.Find(VALUE_C);

		TBuf8<30> value;

		if (end1-7 >= 0 && start1+7<iBuffer.Length())
		{
			value.Copy(&iBuffer[start1+7], end1 - start1 - 7);

		}

		if(value.Find(_L8("msisdn_mismatch")) >= 0)
			iUserStatus = 3;
		else if(value.Find(_L8("imei_mismatch")) >= 0)
			iUserStatus = 4;

	}
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("/ProcessStatusXml\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

}

void CSocketsEngineWriter::ParseStatusParamXml(TDesC8& parambuffer)
{
	_LIT8(VALUE_O,"<value>");
	_LIT8(VALUE_C,"</value>");

	_LIT8(EQUAL,"=");

	TInt start;
	TInt end;

	TInt equal;
	
	TBuf8<150> aValue;
	//TDes8& aValue = temp_session;
	TBuf8<150> aParam;
		
	start = parambuffer.Find(VALUE_O);
	end = parambuffer.Find(VALUE_C);

	//TBuf8<30> tempbuf;

	while (start >= 0)
	{								
		if (end-7 >= 0 && start+7<parambuffer.Length())
		{
			aValue.Copy(&parambuffer[start+7], end - start - 7);

			equal = aValue.Find(EQUAL);

			aParam.Copy(&aValue[0], equal);

			aValue.Delete(0, equal+1);			

			if(aParam.Compare(_L8("user_status")) == 0)
			{
				iUserStatus = 10;

				if(aValue.Compare(_L8("valid")) == 0 || aValue.Compare(_L8("pendingconfirmation")) == 0)
					iUserStatus = 1;
				else if(aValue.Compare(_L8("probesent")) == 0)
					iUserStatus = 2;
				else
					iUserStatus = 0;
			}
			else if(aParam.Compare(_L8("version")) == 0)//for UPGRADE
			{
				TBuf<150> version_value;
				version_value.Copy(aValue);
				if(iAppUi.IsLaterVersion(version_value))
				{
					iAppUi.SaveUpgradeStatus(1, version_value);
				}
			}
			//else if(aParam.Compare(_L8("settings")) == 0)//for SETTINGS STRING
			//{
			//				ParseSettingsString();
			//}
			else
			{
				//if(aParam.Compare(_L8("hp")) == 0)
					//aValue.Copy(_L("D62DB90BF8"));

				iAppUi.EditConfigFile(aValue, aParam);
			}
		}		
		start = -1;
	}	
}
/*
void CSocketsEngineWriter::EditConfigFile(TDes8& aValue, TDesC8& aParam)
{
	RFs fs,fs1;
	RFile file,file1;
	
	//_LIT(KConfigFile,"config.txt");
	_LIT(KConfigFile1,"tempconfig.txt");

	TBuf8<150> tempbuf;

	fs.Connect();
	fs1.Connect();

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KConfigFile);

	TBuf<128> path1;
	path1 = CRippleVaultAppUi::ApplicationDriveAndPath();
	path1.Append(KConfigFile1);
	
	if (BaflUtils::FileExists(fs, path))
	{
		TInt err = file.Open(fs, path, EFileRead);
		
		TInt err1 = file1.Open(fs1, path1, EFileWrite);

		if(err1 != KErrNone)
			file1.Create(fs1, path1, EFileWrite);
		
		if (err == KErrNone) 
		{
			TBuf8<2> buf;
			TBuf<2> buf16;
			TBuf<150> line;
			TBuf<150> param;
			
			param.Copy(aParam);
			param.Append(_L("="));

			TInt len = param.Length();

			line.Copy(_L(""));

			TBool isThere;
			isThere = FALSE;

			TInt count = 0;
			do 
			{
				file.Read(buf, 1);

				if (buf.Length())
					count++;

				buf16.Copy(buf);
				line.Append(buf16);

				if (buf.Length() == 0 || buf.Find(_L8("\n")) != KErrNotFound ||
				buf.Find(_L8("\r")) != KErrNotFound) 
				{
					if (line.Find(param) == KErrNotFound)
					{
						tempbuf.Copy(line);
						file1.Write(tempbuf);
					}
					else
					{
						isThere = TRUE;
						tempbuf.Copy(_L(""));
					
						tempbuf.Append(aParam);
						tempbuf.Append(_L("="));
						tempbuf.Append(aValue);
						tempbuf.Append(_L("\n"));

						file1.Write(tempbuf);
					}

					if (line.Find(param) != KErrNotFound && line.Length() > len) 
					{
						line.Delete(0,len);
						//line.SetLength(count - 7);
						if (buf.Find(_L8("\n")) != KErrNotFound || buf.Find(_L8("\r")) != KErrNotFound)
						{
							line.SetLength(line.Length() - 1);
						}						
					}
					line.Copy(_L(""));
					count = 0;
				}
			}
			while(buf.Length() != 0);

			if(!isThere) //param not there, add it
			{
						tempbuf.Copy(_L(""));
					
						tempbuf.Append(aParam);
						tempbuf.Append(_L("="));
						tempbuf.Append(aValue);
						tempbuf.Append(_L("\n"));

						file1.Write(tempbuf);
			}


		}
	}
	
	file.Close();
	file1.Close();
	
	fs.Delete(path);	
	BaflUtils::RenameFile(fs1, path1, path,0);
	
	fs.Close();	
	fs1.Close();
}
*/
void CSocketsEngineWriter::ProcessServerResponse()
{


	if(iRegister == 1)
	{
		ProcessRegistrationResp();	
	}
	else if(iRegister == 3)
	{
		ProcessSharingResp();
	}
	else if(iRegister == 5)
	{
		ProcessAlbumSynchResp();
	}
	else if(iRegister == 6)
	{
		ProcessStatusSubResp();
	}
	else if(iRegister == 7)
	{
		ProcessSmsResp();
	}	
	else if(iRegister == 8)
	{
		ProcessChangePasswordResp();
	}
	else if(iRegister == 9)
	{
		ProcessUnsharingResp();
	}
	//else if(iRegister == 10)
	//{				
	//	ProcessBillingStatusResp();
	//}
	else
	{
		ProcessPhotosResp();
	}

}

void CSocketsEngineWriter::ProcessSharingResp()
{
	TBuf8<30> temp;

	_LIT8(ACK_O,"<ack>");
	_LIT8(ACK_C,"</ack>");
	_LIT8(SUCCESS,"success_share");
	//_LIT8(SUCCESS,"success");
	_LIT8(ALBUM_EMPTY,"album_empty");
	_LIT8(ALBUM_AlREADY_SHARED,"album_already_shared");
	_LIT8(ALBUM_INVALID_NUMBER,"album_invalid_number");
	_LIT8(ALBUM_AlREADY_SHARED_REST,"some_album_already_shared");
	_LIT8(ALBUM_INVALID_NUMBER_REST,"some_album_invalid_number");
		
	TInt start;
	TInt end;
	
	TBuf8<400> ack_message;				
	
	start=iBuffer.Find(ACK_O);

#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("ProcessSharingResp Ack="));
		iAppUi.LogText.Append(iBuffer);
		iAppUi.LogText.Append(_L("\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	if (start >= 0)
	{
		temp_session.Copy(&iBuffer[start], iBuffer.Length()-start);
		end=temp_session.Find(ACK_C);
	
		if (start+5 >= 0)
		ack_message.Copy(&iBuffer[start+5], end-5);				
		
	}
	else
	{	
		iAppUi.iPhotosDB->DeleteSharingsFirstRecord();
		iAppUi.ShowMessageBoxFromResource(R_TEXT_SHARING_GPRSUNAVAILABLE);
		iAppUi.CloseSocketShare();
	}

	#ifndef __UIQ__
		if (iWaitDialog != NULL)
		{
			iWaitDialog->ProcessFinishedL();
		}
	#else
	#ifdef __S80__
		iAppUi.DestroyProgressBall();
	#else
		CEikonEnv::Static ()->BusyMsgCancel();
	#endif
	#endif

	temp.Copy(_L("success"));

	TInt flag=0;

	if (start >= 0)
	if(ack_message.Compare(SUCCESS) == 0)
	{
		if(iAppUi.albumName.Compare(_L("")) == 0)
		{
			//msg.Copy(_L("Your invitation has been sent"));
			// success for tell a friend
		}
		else
		{
			//msg.Copy(_L("Your album has been shared"));
			flag=1;// success for album share 
		}
		
		iAppUi.SendSharingFileComplete(flag);
	}
	else
	{
		if(iAppUi.albumName.Compare(_L("")) == 0)
		{
			flag = 2;//failure for tell a friend
		}
		else
		{
			//msg.Copy(_L("Your album has been shared"));
			if(ack_message.Compare(ALBUM_EMPTY) == 0)
				flag = 3;//failure for album share if album empty 
			else if(ack_message.Compare(ALBUM_AlREADY_SHARED) == 0) 
				flag = 4;//failure for album share if album already shared 
			else if(ack_message.Compare(ALBUM_INVALID_NUMBER) == 0) 
				flag = 5;//failure for album share if mboile not valid 
			else if(ack_message.Compare(ALBUM_AlREADY_SHARED_REST) == 0) 
				flag = 6;//failure for album share if album already shared & rest 
			else if(ack_message.Compare(ALBUM_INVALID_NUMBER_REST) == 0) 
				flag = 7;//failure for album share if mboile not valid & rest
			else
				flag = 8;
		}

		iAppUi.SendSharingFileComplete(flag);
	}
}

void CSocketsEngineWriter::ProcessAlbumSynchResp()
{
	#ifndef __UIQ__
		if (iWaitDialog != NULL)
		{
			iWaitDialog->ProcessFinishedL();
		}
	#else
	#ifdef __S80__
		iAppUi.DestroyProgressBall();
	#else
		CEikonEnv::Static ()->BusyMsgCancel();
	#endif
	#endif

	//Disconnect();
	EditAlbumNames();
	//iAppUi.EditingAlbumsComplete();				
	iAppUi.AlbumSynchingComplete();
}

void CSocketsEngineWriter::ProcessStatusSubResp()
{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("ProcessStatusSubResp\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	#ifndef __UIQ__
		if (iWaitDialog != NULL)
		{
			iWaitDialog->ProcessFinishedL();
		}
	#else
	#ifdef __S80__
		iAppUi.DestroyProgressBall();
	#else
		CEikonEnv::Static ()->BusyMsgCancel();
	#endif
	#endif

		ParseStatusXml();
		//iAppUi.CloseSocketStatus(connectionstatus);//Disconnect();

		if(iUserStatus == 1)
		{
			if(connectionstatus == 1)
			{
				//iAppUi.SendToServer();
				iAppUi.GetAlbumSynchingXML();
			}
			else if(connectionstatus == 2)
			{
				iAppUi.iContactsHandler->SyncToServer();						
			}
			else if(connectionstatus == 3)
			{
				iAppUi.ConnectToServerForSms(iSmsbackupType);
			}
			else if(connectionstatus == 4)
			{
				iAppUi.UpdateSimcard();
			}
			else if (connectionstatus == 6)
			{
				iAppUi.iContactsHandler->ConnectToServer();
			}
			else if (connectionstatus == 7)
			{
				iAppUi.ConnectToServerForSms(iSmsbackupType);
			}
			
		}
		else if(iUserStatus == 2)
		{
			iAppUi.iBillingFlag = 1;
			iAppUi.SendBillingSMS();
		}
		else
		{
			iAppUi.NetworkStatusNotification(NETWORK_STATUS_FAILED);

			//TBuf<300> msg;
			if(iUserStatus == 3)
				//msg.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SUB_STATUS_MSISDNMISMATCH)));
				iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_MSISDNMISMATCH);
			else if (iUserStatus == 4){
				iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_IMEIMISMATCH);
				/*TBuf<300> formatBuf;
				formatBuf.Copy(_L(""));
				formatBuf.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SUB_STATUS_IMEIMISMATCH)));
				msg.Format(formatBuf, &iAppUi.iUserNames);*/
			}
			else if (iUserStatus == 0)
				//msg.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SUB_STATUS_EXPIRED)));
				iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_ACCSUSPENDED);
			//else
				//iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_PARSING);
			//if(iAppUi.iConnectionStatus == 4)
			if(connectionstatus == 4)
			{
				iAppUi.ImsiCheck();
			}					
		}				

#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("/ProcessStatusSubResp\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	iAppUi.CloseSocketStatus(connectionstatus);//Disconnect();

}

void CSocketsEngineWriter::ProcessSmsResp()
{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("RunL 40\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	#ifndef __UIQ__
		if (iWaitDialog != NULL)
		{
			iWaitDialog->ProcessFinishedL();
		}
	#else
	#ifdef __S80__
		iAppUi.DestroyProgressBall();
	#else
		CEikonEnv::Static ()->BusyMsgCancel();
	#endif
	#endif

	_LIT8(ACK_O,"<ack>");
	_LIT8(ACK_C,"</ack>");				

	_LIT8(NOTACK_O,"<nack>");
	_LIT8(NOTACK_C,"</nack>");				
	
	TInt start;
	TInt end;
	
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("RunL 41\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	TBuf8<100> ack_message;				

	TBuf8<10> smscount;
	TBuf<10> smscount1;
	
	start=iBuffer.Find(ACK_O);				
	
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("RunL 42\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
	
	if (start >= 0)
	{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL 43\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

/*		if(iSmsbackupType == 1)		
		{
#ifdef __LOGME__
			iAppUi.LogText.Copy(_L("RunL 44\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);
#endif
			
			iAppUi.UpdateSmsTimestamp();
		}*/

		temp_session.Copy(&iBuffer[start], iBuffer.Length()-start);
		end=temp_session.Find(ACK_C);
	
		if (start+5 >= 0)
		ack_message.Copy(&iBuffer[start+5], end-5);
		
		if(ack_message.Length()>3)
		{
			smscount.Copy(&ack_message[3], ack_message.Length()-3);

			smscount1.Copy(smscount);
			TLex nump(smscount1);
			TInt pcount;
			TInt np=nump.Val(pcount);
			if(np==KErrNone)
			{
				iAppUi.SaveSmsUploadSettingsToDB(pcount);
			}

		}
		else{
			TInt count;
			iAppUi.GetSmsUploadSettingsFromDB(count);
			if(iSmsbackupType)
				count+=iAppUi.iAutoSmsCount;
			else
				count+=iAppUi.iSmsCount;
			iAppUi.SaveSmsUploadSettingsToDB(count);
		}

		iAppUi.SendSmsFileComplete(iSmsbackupType);

	}
	else
	{			
#ifdef __LOGME__		
		iAppUi.LogText.Copy(_L("RunL 46\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);

		iAppUi.LogText.Copy(_L("RunL 50\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		start=iBuffer.Find(NOTACK_O);
		if(start>=0)
		{
			temp_session.Copy(&iBuffer[start], iBuffer.Length()-start);
			end=temp_session.Find(NOTACK_C);
		
			if (start+5 >= 0)
			ack_message.Copy(&iBuffer[start+5], end-5);
			
			if(ack_message.Length()>3)
			{
				smscount.Copy(&ack_message[3], ack_message.Length()-3);

				smscount1.Copy(smscount);
				TLex nump(smscount1);
				TInt pcount;
				TInt np=nump.Val(pcount);
				if(np==KErrNone)
				{
					iAppUi.SaveSmsUploadSettingsToDB(pcount);
				}

			}

		}
		//if(iSmsbackupType == 1)//auto sms upload
		//iAppUi.iSmsCount = 0;
		iAppUi.NetworkStatusNotification(NETWORK_STATUS_FAILED);
		iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_PARSING);
		iAppUi.SendSmsFileError(iSmsbackupType);
		//else
		//iAppUi.ShowMessageBoxFromResource(R_TEXT_SMSBACKUP_INBOXSMS);
	}

/*	iAppUi.LogText.Copy(_L("RunL 51\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);

	ChangeStatus(EConnecting);
	iAppUi.CloseSocketSMS();
*/
	//#ifndef	__UIQ__					
	//	iAppUi.SetTitle();			
	//#endif					
}

void CSocketsEngineWriter::ProcessRegistrationResp()
{
	_LIT8(ACK_O,"<ack>");
	_LIT8(ACK_C,"</ack>");
	_LIT8(SUCCESS,"suc_reg");
	_LIT(MSISDNFAIL,"fail_reg_existing_msisdn");

	//_LIT8(SUCCESS,"success");
		
	TInt start;
	TInt end;
	
	TBuf8<600> ack_message;				
	
	start=iBuffer.Find(ACK_O);

	#ifndef __UIQ__
		if (iWaitDialog != NULL)
		{
			iWaitDialog->ProcessFinishedL();
		}
	#else
	#ifdef __S80__
		iAppUi.DestroyProgressBall();
	#else
		CEikonEnv::Static ()->BusyMsgCancel();
	#endif
	#endif
	
	if (start >= 0)
	{
		temp_session.Copy(&iBuffer[start], iBuffer.Length()-start);
		end=temp_session.Find(ACK_C);
	
		if (start+5 >= 0)
		ack_message.Copy(&iBuffer[start+5], end-5);
	}
	else
	{
		ack_message.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_REGISTER_GPRSUNAVAILABLE)));
	}

	if(ack_message.Compare(SUCCESS) == 0)				
	//if(1)
	{
		if(iAppUi.iPassword.Compare(_L(""))!=0)
		{					
			//Disconnect();							
			iAppUi.iContactsFlag = 1;
			iAppUi.CloseSocketPublish();
			iAppUi.GetAlbumSynchingXML();

		}
		else{
			//Disconnect();
			iAppUi.AlbumSynchingComplete();
		}
	}
	else
	{
		//Disconnect();
		TBuf<400> msg;
		msg.Copy(ack_message);
		
		if (msg.Compare(MSISDNFAIL) == 0)
		{
			TBuf<300> formatBuf;
			formatBuf.Copy(_L(""));
			formatBuf.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_REGISTER_MSISDN_MISMATCH)));
			msg.Format(formatBuf, &iAppUi.iMobileNo);
		}
		else
			msg.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_INVALID_USERID_PASSWORD)));

		#ifndef	__UIQ__
			//CAknInformationNote* note = new(ELeave) CAknInformationNote;
			//note->ExecuteLD(msg);
			CAknMessageQueryDialog* verdlg = CAknMessageQueryDialog::NewL(msg);	
			verdlg->PrepareLC(R_ABOUT_QUERY);
			verdlg->RunLD();
		#else		
			//CEikonEnv::Static ()->InfoWinL(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),msg);
			CEikDialog* dialog;
			dialog = new (ELeave) CCustomMessageDialog(msg, R_EXAMPLE_RIPPLE_PUBLISH);
			dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
		#endif	
		
		//delete this;
		iAppUi.CloseSocketPublish();
	}
	//iAppUi.CloseSocketPublish();
	//#ifndef	__UIQ__					
	//	iAppUi.SetTitle();			
	//#endif
}

void CSocketsEngineWriter::ProcessPhotosResp()
{
#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("RunL 54\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	_LIT8(ACK_O,"<ack>");
	//_LIT8(ACK_C,"</ack>");				
		
	TInt start;
	//TInt end;

#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("RunL 55\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	//TBuf8<100> ack_message;				

	start=iBuffer.Find(ACK_O);				

#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("RunL 56\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	ChangeStatus(EConnecting);

	if (start >= 0)
	{
#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("RunL 57\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);					
#endif
		
		iAppUi.SendFileComplete(iAlbumnamestatus);	
		
		//iAppUi.LogText.Copy(_L("RunL 58\n"));
		//iAppUi.WriteLogFile(iAppUi.LogText);
	}
	else
	{			
#ifdef __LOGME__		
		iAppUi.LogText.Copy(_L("RunL 59\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);

			iAppUi.LogText.Copy(_L("RunL 62\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);
#endif
			
			iAppUi.NetworkStatusNotification(NETWORK_STATUS_FAILED);
			iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_PARSING);
			iAppUi.SendFileError();
	}
	//iAppUi.LogText.Copy(_L("RunL 63\n"));
	//iAppUi.WriteLogFile(iAppUi.LogText);				
	
}
/*
void CSocketsEngineWriter::ProcessBillingStatusResp()
{
	_LIT8(ACK_O,"<ack>");
	_LIT8(ACK_C,"</ack>");
	_LIT8(SUCCESS,	"server_updated_to_pendingconfirmation");
	_LIT8(RESEND,	"server_not_updated");
		
	TInt start;
	TInt end;
	
	TBuf8<400> ack_message;				
	
	start=iBuffer.Find(ACK_O);
	
	//iAppUi.WriteLogFile(iBuffer);

	if (start >= 0)
	{
		temp_session.Copy(&iBuffer[start], iBuffer.Length()-start);
		end=temp_session.Find(ACK_C);
	
		if (start+5 >= 0)
		ack_message.Copy(&iBuffer[start+5], end-5);				
		
	}
	else
	{
		iAppUi.NetworkErrorNotification(NETWORK_ERRCODE_PARSING);
		//ack_message.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SHARING_GPRSUNAVAILABLE)));
	}

	#ifndef __UIQ__
		if (iWaitDialog != NULL)
		{
			iWaitDialog->ProcessFinishedL();
		}
	#else
		CEikonEnv::Static ()->BusyMsgCancel();
	#endif

	iAppUi.CloseSocketStatus(connectionstatus);//Disconnect();

	if(ack_message.Compare(SUCCESS) == 0)			
	{
		if(iAppUi.iBillingFlag == 1)
			iAppUi.CheckSubscriptionStatus(iAppUi.iConnectionStatus);
	}
	else if(ack_message.Compare(RESEND) == 0)
	{
		iAppUi.CheckBillingStatus(5);
	}

}
*/
void CSocketsEngineWriter::ProcessUnsharingResp()
{
	_LIT8(ACK_O,"<ack>");
	_LIT8(ACK_C,"</ack>");
	_LIT8(SUCCESS,"success_unshare");
		
	TInt start;
	TInt end;
	
	TBuf8<400> ack_message;				
	
	start=iBuffer.Find(ACK_O);
	
	//iAppUi.WriteLogFile(iBuffer);

	if (start >= 0)
	{
		temp_session.Copy(&iBuffer[start], iBuffer.Length()-start);
		end=temp_session.Find(ACK_C);
	
		if (start+5 >= 0)
		ack_message.Copy(&iBuffer[start+5], end-5);				
		
	}
	else
	{					
		ack_message.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SHARING_GPRSUNAVAILABLE)));
	}

	#ifndef __UIQ__
		if (iWaitDialog != NULL)
		{
			iWaitDialog->ProcessFinishedL();
		}
	#else
	#ifdef __S80__
		iAppUi.DestroyProgressBall();
	#else
		CEikonEnv::Static ()->BusyMsgCancel();
	#endif
	#endif

	if(ack_message.Compare(SUCCESS) == 0)			
	{
		iAppUi.SendUnSharingFileComplete();
	}
	else
	{
		TBuf<400> msg;
		msg.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_UNSHARING_ALBUM_FAILED)));

		#ifndef	__UIQ__
			CAknInformationNote* note = new(ELeave) CAknInformationNote;
			note->ExecuteLD(msg);
		#else		
			//CEikonEnv::Static ()->InfoWinL(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),msg);
			CEikDialog* dialog;
			dialog = new (ELeave) CCustomMessageDialog(msg, R_EXAMPLE_RIPPLE_PUBLISH);
			dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
		#endif							
		
		iAppUi.CloseSocketShare();//Disconnect();				
	}

}

void CSocketsEngineWriter::ProcessChangePasswordResp()
{
	_LIT8(ACK_O,"<ack>");
	_LIT8(ACK_C,"</ack>");
	_LIT8(SUCCESS,"suc_reg");
	//_LIT8(SUCCESS,"success");
		
	TInt start;
	TInt end;
	
	TBuf8<400> ack_message;				
	
	start=iBuffer.Find(ACK_O);
	
	if (start >= 0)
	{
		temp_session.Copy(&iBuffer[start], iBuffer.Length()-start);
		end=temp_session.Find(ACK_C);
	
		if (start+5 >= 0)
		ack_message.Copy(&iBuffer[start+5], end-5);				
	}
	else
	{			
		ack_message.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_REGISTER_GPRSUNAVAILABLE)));
	}

	#ifndef __UIQ__
		if (iWaitDialog != NULL)
		{
			iWaitDialog->ProcessFinishedL();
		}
	#else
	#ifdef __S80__
		iAppUi.DestroyProgressBall();
	#else
		CEikonEnv::Static ()->BusyMsgCancel();
	#endif
	#endif

	if(ack_message.Compare(SUCCESS) == 0)				
	//if(1)
	{
		iAppUi.ChangePasswordComplete();
	}
	else
	{
		//Disconnect();
		TBuf<100> msg;
		//msg.Copy(ack_message);
		//if (msg.Compare(_L("fail_reg")) == 0)
		msg.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_INVALID_PASSWORD)));

		#ifndef	__UIQ__
			CAknInformationNote* note = new(ELeave) CAknInformationNote;
			note->ExecuteLD(msg);
		#else		
			//CEikonEnv::Static ()->InfoWinL(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),msg);
			CEikDialog* dialog;
			dialog = new (ELeave) CCustomMessageDialog(msg, R_EXAMPLE_RIPPLE_PUBLISH);
			dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
		#endif	
		
		iAppUi.CloseSocketPassword();
		//delete this;
	}
	//iAppUi.CloseSocketPublish();
	//#ifndef	__UIQ__					
	//	iAppUi.SetTitle();			
	//#endif

}

void CSocketsEngineWriter::SocketsError()
{
#ifdef __LOGME__
			iAppUi.LogText.Copy(_L("Sockets Error\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	if(!iRegister)
	{
		iAppUi.SendFileError();
	}
	else
	{

	#ifndef __UIQ__
		if (iWaitDialog != NULL)
		{
			iWaitDialog->ProcessFinishedL();
		}
	#else
	#ifdef __S80__
		iAppUi.DestroyProgressBall();
	#else
		CEikonEnv::Static ()->BusyMsgCancel();
	#endif
	#endif

		if(iRegister == 1)
		{// || (iRegister == 6 && connectionstatus == 4))

			TInt flag = 0;
			if(iAppUi.iAccessPoints.Find(_L("NOHARDCODE"))>=0)
			{
				if(iAppUi.iApnCount == -2)
					flag = 1;
			}
			else
			{
				if(iAppUi.iApnCount == 1)
					flag = 1;
			}

			if(flag == 1)
			{
				//Disconnect();
				//iAppUi.ShowMessageBoxFromResource(R_TEXT_REGISTER_GPRSUNAVAILABLE);
				#ifdef __UIQ__
					//CEikonEnv::Static()->InfoWinL(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),msg);
					iAppUi.ShowMessageBoxFromResource(R_INTERNET_APN_ERROR);
				#else
					CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_INTERNET_APN_ERROR)));
					abtdlg->PrepareLC(R_SETACCESSPOINT_ERROR);

					if (abtdlg->RunLD()) 
					{					
					}				
				#endif

				iAppUi.CloseSocketPublish();
			}
			else
			{
				iAppUi.ConnectRegSocket();
			}
		}
		else if(iRegister == 3 || iRegister == 9){
			TInt flag=0;
			if(iAppUi.albumName.Compare(_L("")) == 0){					
			}
			else{
				flag=1;
			}
            if(iRegister == 9)
               flag = 3;
			iAppUi.SendSharingFileError(flag);
		}
		else if(iRegister == 7){
			//iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_TRYAGAIN);				
			iAppUi.SendSmsFileError(iSmsbackupType);
		}
		else if(iRegister == 8){
			iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_TRYAGAIN);
			iAppUi.CloseSocketPassword();
		}

		//if(iRegister == 1 || iRegister == 5){
		else if(iRegister == 5){
			//Disconnect();
			if(!iAppUi.RegFileExists())
				iAppUi.ShowMessageBoxFromResource(R_TEXT_REGISTER_GPRSUNAVAILABLE);

			iAppUi.CloseSocketPublish();
		}
		else if (iRegister == 6)
		{
			if(connectionstatus == 4)		
				iAppUi.ShowMessageBoxFromResource(R_TEXT_REGISTER_GPRSUNAVAILABLE);
			else if (connectionstatus == 6)
				iAppUi.iContactsHandler->ResetState();

			iAppUi.SendSubStatusError(connectionstatus);
				
			iAppUi.CloseSocketStatus(connectionstatus);
		}
		else if (iRegister == 10)
		{
			//Disconnect();
			iAppUi.CloseSocketStatus(connectionstatus);				
		}
	}

}

void CSocketsEngineWriter::InitializeProgressBars()
{
#ifndef	__UIQ__
		if (iWaitDialog != NULL)
		{
			iWaitDialog->ProcessFinishedL();

			iWaitDialog = NULL;
			delete iWaitDialog;
		}
#endif
	if(iRegister == 1 || iRegister == 3 || iRegister == 5 || iRegister == 8 || iRegister == 9 )// || iRegister == 6 || iRegister == 7)
	{
//#ifndef	__UIQ__			
		if(iRegister==5 && iAppUi.RegFileExists()){
		}
/*		else if(iRegister == 6){
			if(connectionstatus == 3)
			{
				iWaitDialog = new(ELeave)CAknWaitDialog( (REINTERPRET_CAST(CEikDialog**,&iWaitDialog)));
				iWaitDialog->SetTone( CAknNoteDialog::EConfirmationTone ); 
				iWaitDialog->SetTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SUB_STATUS)));
			}
		}*/
		else{
#ifndef __UIQ__
		iWaitDialog = new(ELeave)CAknWaitDialog( (REINTERPRET_CAST(CEikDialog**,&iWaitDialog)));
		iWaitDialog->SetTone( CAknNoteDialog::EConfirmationTone ); 
#endif
		}
		if(iRegister == 3) 
		{
			if(iAppUi.albumName.Compare(_L("")) == 0){
				//iWaitDialog->SetTextL(_L("Sending Invitation"));
#ifndef __UIQ__
				iWaitDialog->SetTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SHARING_APPLICATION_PROGRESS)));
#else
#ifdef __S80__
				iAppUi.DisplayProgressBallL(R_TEXT_SHARING_APPLICATION_PROGRESS);
#else
				CEikonEnv::Static ()->BusyMsgL(*(CEikonEnv::Static ()->AllocReadResourceL(R_TEXT_SHARING_APPLICATION_PROGRESS)));
#endif
#endif
			}
			else{
				//iWaitDialog->SetTextL(_L("Sharing Album"));
#ifndef __UIQ__
				iWaitDialog->SetTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SHARING_ALBUM_PROGRESS)));
#else
#ifdef __S80__
				iAppUi.DisplayProgressBallL(R_TEXT_SHARING_ALBUM_PROGRESS);
#else
				CEikonEnv::Static ()->BusyMsgL(*(CEikonEnv::Static ()->AllocReadResourceL(R_TEXT_SHARING_ALBUM_PROGRESS)));
#endif
#endif
			}
		}
		else if(iRegister == 5){				
			if(!iAppUi.RegFileExists()){
					//iWaitDialog->SetTextL(_L("Synchronizing Albums"));
#ifndef __UIQ__
					iWaitDialog->SetTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SYNCH_ALBUM_PROGRESS)));
#else
#ifdef __S80__
					iAppUi.DisplayProgressBallL(R_TEXT_SYNCH_ALBUM_PROGRESS);
#else
					CEikonEnv::Static ()->BusyMsgL(*(CEikonEnv::Static ()->AllocReadResourceL(R_TEXT_SYNCH_ALBUM_PROGRESS)));
#endif
#endif
			}
		}		
		else if(iRegister == 8){
				//iWaitDialog->SetTextL(_L("Backup sms"));
#ifndef __UIQ__
			iWaitDialog->SetTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_CHANGE_PASSWORD_PROGRESS)));
#else
#ifdef __S80__
			iAppUi.DisplayProgressBallL(R_TEXT_CHANGE_PASSWORD_PROGRESS);
#else
			CEikonEnv::Static ()->BusyMsgL(*(CEikonEnv::Static ()->AllocReadResourceL(R_TEXT_CHANGE_PASSWORD_PROGRESS)));
#endif
#endif
		}
		else if(iRegister == 9) 
		{
			//iWaitDialog->SetTextL(_L("UnSharing Album"));
#ifndef __UIQ__
			iWaitDialog->SetTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_UNSHARING_ALBUM_PROGRESS)));
#else
#ifdef __S80__
			iAppUi.DisplayProgressBallL(R_TEXT_UNSHARING_ALBUM_PROGRESS);
#else
			CEikonEnv::Static ()->BusyMsgL(*(CEikonEnv::Static ()->AllocReadResourceL(R_TEXT_UNSHARING_ALBUM_PROGRESS)));
#endif
#endif
		}
		/*		else if(iRegister == 7){
				//iWaitDialog->SetTextL(_L("Backup sms"));
			iWaitDialog->SetTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_SMS_BACKUP_PROGRESS)));
		}*/
		else if(iRegister == 1)
		{
			TBuf<100> msg;
			msg.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_REGISTERING_PROGRESS)));
			msg.Append(_L("\n"));
			msg.Append(_L("("));
			msg.Append(iAppUi.iApnConnectionName);
			msg.Append(_L(")"));
#ifndef __UIQ__
		iWaitDialog->SetTextL(msg);
#else
#ifdef __S80__
		iAppUi.DisplayProgressBallL(R_TEXT_REGISTERING_PROGRESS);
#else
		CEikonEnv::Static ()->BusyMsgL(msg);//*(CEikonEnv::Static ()->AllocReadResourceL(R_TEXT_REGISTERING_PROGRESS)));
#endif
#endif
		}

		if(iRegister==5 && iAppUi.RegFileExists()){
		}
		/*else if(iRegister == 6){
			if(connectionstatus == 3)
			{
				iWaitDialog->ExecuteLD(R_WAITNOTE_DIALOG);
			}
		}*/
		else{
#ifndef __UIQ__
		iWaitDialog->ExecuteLD(R_WAITNOTE_DIALOG);
#endif
		}
//#endif		
	}

}

void CSocketsEngineWriter::CancelUpload()
{
	DoCancel();
}

#ifdef WAP_PT
void CSocketsEngineWriter::GetProxyAddress(TUint32 aId, TDes& aAddr)
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
	port = pPort;

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

