/* Copyright (c) 2000-2005, PurpleACE. All rights reserved */

#ifndef __CONTACTSSOCKETSENGINEWRITER_H__
#define __CONTACTSSOCKETSENGINEWRITER_H__

#include <in_sock.h>
#ifndef __SSL6__
#ifdef __SSL7__
#include <securesocket.h>
#endif
#endif
#include "TimeOutNotifier.h"
#include "EngineNotifier.h"
//#include "RippleVault.hrh"
#ifndef __UIQ__
#include <aknwaitdialog.h> 
#endif

#include "constants.h"

#ifdef WAP_PT
#include <wapcli.h>
#include <capcodec.h>
#include <AgentClient.h>
#endif

class CTimeOutTimer;
class MUINotifier;
class CRippleVaultAppUi;
class CList;
class CRecordContactDB;
class CContactItem;
class CList;
class CUpdateContactsAO;

/*! 
  @class CContactsSocketsEngineWriter
  
  @discussion This class is the main engine part of the sockets application.
  It establishes a TCP connection using its server name and port number (performing a DNS lookup
  operation first, if appropriate).
  It creates instances of separate active objects to perform reading from, and writing to, the socket.
  */
class CContactsSocketsEngineWriter : public CActive, public MTimeOutNotifier
    {
public: // new methods
	HBufC8* servlet_data;
	CRippleVaultAppUi & iAppUi;

	static CContactsSocketsEngineWriter* NewL(CRippleVaultAppUi& aAppUi,TDesC& aAddress,TUint16 aPort );

    static CContactsSocketsEngineWriter* NewLC(CRippleVaultAppUi& aAppUi,TDesC& aAddress,TUint16 aPort );

	~CContactsSocketsEngineWriter();

    void Disconnect();

//	void Register(TInt aRegistered);

public: // from MTimeOutNotifier

	void TimerExpired();

public:
	void Start();
	
private: // from CActive
	void DoCancel();

    void RunL();

	TInt RunError(TInt aError);
	
#ifndef __UIQ__
	CAknWaitDialog* iWaitDialog;
#endif
	
private: // New methods
/*!
  @function CContactsSocketsEngineWriter
  
  @discussion Perform the first phase of two phase construction 
  @param aConsole the console to use for ui output
  */
	CContactsSocketsEngineWriter(CRippleVaultAppUi& aAppUi,TDesC& aAddress,TUint16 aPort );

/*!
  @function ConstructL
  
  @discussion Perform the second phase construction of a CContactsSocketsEngineWriter 
  */
    void ConstructL();

/*!
  @enum TSocketsEngineState

  @discussion Tracks the state of this object through the connection process
  @value ENotConnected The initial (idle) state
  @value EConnecting A connect request is pending with the socket server
  @value EConnected A connection has been established
  @value ELookingUp A DNS lookup request is pending with the socket server
  */
	enum TSocketsEngineState 
	    {
#ifndef WAP_PT
	    ELookingUp,
        EConnecting,
        EConnected,
        EWriting,
		EWritten,
//		EReceived,
		ESyncRecv,
		EWritingACK,
		EWrittenACK,
		ESecureSocket
#else
			EConnecting,
			EConnected,
			EAcked
#endif
	};

    void ChangeStatus(TSocketsEngineState aNewStatus);

public:
	void AcksToServer();
private: // Member variables
	void Connect();
	CList* GetParsedRecords();
	
	void AddDataToRecord(TDes& tagName, TDes& tagValue, CRecordContactDB* record);

	void IssueRead();

	void CleanupList();

	void ErrorStatus();

	void ProcessAck(TDesC& ackMessage);

public:
	void ReplaceTag(TDes& data, TPtrC toTag, TPtrC fromTag);
	void ConvertXMLTags(TDes& data);
	void ConvertRecordXMLTags(CList *list);
	void ConvertEscapeSequences(CList *list);
	
private:

    static const TInt KTimeOut;

    //static const TInt KDefaultPortNumber;

	TInetAddr                   iAddress;
	TInt                        iPort;
	TBuf<128>                   iServerName;
//	TInt                        iRegister;
//	TInt						iNewUser;
	CTimeOutTimer*              iTimer;
	TSocketsEngineState         iEngineStatus;

#ifndef WAP_PT
	RSocket                     iSocket;
	//RSocket                     iSocket2;
	//RSocketServ                 iSocketServ2;
	TSockXfrLength              iDummyLength;
	RSocketServ                 iSocketServ;
	RHostResolver               iResolver;
	TNameEntry                  iNameEntry;
	TInt                        iResolvedLocal; // locally resolved url
	//TInt ResolveLocalDNS();

#ifndef __SSL6__
#ifdef __SSL7__
	CSecureSocket* iTlsSocket;

	//TSockXfrLength iBytesSent;
#endif
#endif
	
#else
	RWAPServ	iWapServ;
	CCapCodec*  iCap;
	RWSPCOConn	iConnection;
	RWSPCOTrans	iCOTrans;
	RWSPCOTrans iTransSynch;
	RWSPCOTrans	iCOTransAck;
	RWSPCOTrans iTransAck;
	RWSPCOConn::TEvent iEvent;

	TInt		iRetryCount;
	TInt        port;
	//RGenericAgent iAgent;
	TInt32 DefaultAccessPointId(TUint32& aId);
	TInt GetGatewayAddress(TDes8& addr);
	void SendTransaction(RWSPCOConn& aConn, RWSPCOTrans& aCOTrans, RWSPCOTrans& aTrans);
	void MakeContentLength(TUint size, TDes8& header);

	void GetProxyAddress(TUint32 aId, TDes& addr);
#endif // ifndef WAP_PT

//	void ProcessRegisterResponse();
	TInt ProcessSynchResponse();

public:
	/*! @var read buffer */
	TBuf8<MAX_RECV_LEN>			iBuffer;

	TBuf8<READ_LEN> readBuf;

//	TBool						iUpgradeFlag;
	TBool						iVersionFlag;
	TBool						iPoll;

	TInt						iPollTimeOut;
private:

	TBuf8<1024>					iTempSession;
	
	CList*                      iRecordList;
	CUpdateContactsAO			*iUpdateAO;
	TInt						iDownloadCount;
	TBool						iDoCancel;
    };

#endif // __SOCKETSENGINEWRITER_H__

