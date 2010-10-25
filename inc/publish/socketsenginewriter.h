

#ifndef __SOCKETSENGINEWRITER_H__
#define __SOCKETSENGINEWRITER_H__

#include <in_sock.h>
#ifndef __SSL6__
#ifdef __SSL7__
#include <securesocket.h>
#endif
#endif
//#include "sockets.hrh"

#ifdef WAP_PT
#include <wapcli.h>
#include <capcodec.h>
#include <AgentClient.h>
#endif

#include "TimeOutNotifier.h"
#include "EngineNotifier.h"
//#include "RippleVault.hrh"
#ifndef __UIQ__
#include <aknwaitdialog.h> 
#endif

#include "RippleVaultAppUi.h"

class CTimeOutTimer;
class MUINotifier;
class CRippleVaultAppUi;

/*! 
  @class CSocketsEngineWriter
  
  @discussion This class is the main engine part of the sockets application.
  It establishes a TCP connection using its server name and port number (performing a DNS lookup
  operation first, if appropriate).
  It creates instances of separate active objects to perform reading from, and writing to, the socket.
  */
class CSocketsEngineWriter : public CActive , public MTimeOutNotifier 
    {
public: // new methods
	//HBufC8 *img;
	TBuf8<6000> iBuffer;
	TBuf8<6000> temp_session;
	TBuf<100> iAlbumnamestatus;
    //TInt iOffset;
	TInt iPacketNumber;
	TInt iCounter;
	TBool iDoCancel;
	CRippleVaultAppUi& iAppUi;			
/*!
  @function NewL
   
  @discussion Create a CSocketsEngineWriter object
  @param aConsole console to use for ui output
  @result a pointer to the created instance of CSocketsEngineWriter
  */
	static CSocketsEngineWriter* NewL(CRippleVaultAppUi& aAppUi);

/*!
  @function NewLC
   
  @discussion Create a CSocketsEngineWriter object
  @param aConsole console to use for ui output
  @result a pointer to the created instance of CSocketsEngineWriter
  */
    static CSocketsEngineWriter* NewLC(CRippleVaultAppUi& aAppUi);

/*!
  @function ~CSocketsEngineWriter
  
  @discussion Destroy the object and release all memory objects
  */
	~CSocketsEngineWriter();

/*!
  @function Disconnect

  @discussion Disconnect socket
  */
    void Disconnect();

/*!
  @function SetServerName

  @discussion Set name of server to connect to
  @param aName new server name
  */
    void SetServerName(const TDesC& aName);

/*!
  @function ServerName

  @discussion Get server name
  @result name of server
  */
    const TDesC& ServerName() const;

/*!
  @function SetPort

  @discussion Set port number to connect to
  @param aPort new port number
  */
    void SetPort(TInt aPort);

/*!
  @function Port

  @discussion Get port number
  @result port number
  */
    TInt Port() const;

/*!
  @function Connected

  @discussion Is socket fully connected?
  @result true if socket is connected
  */
    TBool Connected() const;

  void WriteImageToServerL(const TDesC& aDir);

public: // from MTimeOutNotifier
/*!
  @function TimerExpired

  @discussion The function to be called when a timeout occurs
  */
	void TimerExpired();

	void ProcessFile();

//	RFs iUpGradeSess;

//	RFile iUpGradeFile;

	TInt iSmsbackupType;

	TInt iUserStatus;

	TInt connectionstatus;

public:
	void ProcessServerResponse();

	void ProcessRegistrationResp();
	void ProcessSharingResp();
	void ProcessAlbumSynchResp();
	void ProcessStatusSubResp();
	void ProcessSmsResp();
	void ProcessPhotosResp();
	void ProcessChangePasswordResp();
	//void ProcessBillingStatusResp();
	void ProcessUnsharingResp();
	
	void SocketsError();
	void InitializeProgressBars();

	void ConnectToServer();	

	void Connect();

	void RegisterPhotos();	

//	void ChargePublish();
	
	void ShareFolders();

	void UnShareFolders();
	
//	void SynchAlbums();
	
	void SynchAlbumsForEditing();

	void SubscriptionStatus(TInt aStatus);

	//void BillingStatus(TInt aStatus);

	void ChangePassword();

//	void UpgradePublish();	

//	void CheckUpgrade();
    
	void Reconnect();
	
	void Register(TInt aRegistered);

	void EditAlbumNames();

	void SendSmsData(TInt smsbackuptype);
	void ParseStatusXml();

	void ParseStatusParamXml(TDesC8& parambuffer);

	//void EditConfigFile(TDes8& aValue, TDesC8& aParam);

	//void ParseSettingsString();

	void CancelUpload();

protected: // from CActive
	void DoCancel();
    void RunL();
#ifndef __UIQ__
	CAknWaitDialog* iWaitDialog;
#endif

private:
	CSocketsEngineWriter(CRippleVaultAppUi& aAppUi);

    void ConstructL();

	enum TSocketsEngineState 
	{
#ifndef WAP_PT
		EIdle,
		ELookingUp,
        EConnecting,
		EWriting,
		//EGetFirstImg,
		EGetImg,
		EPending,
		EGetFinalData,
		EWritten,		
		ERegistering,
		ESharing,
		EReceived,
		ECharging,
//		ESynchingAlbum,
//		EUpGradeWrite,
//		EUpGradeRecv,
//		EUpGradeEnd,
//		EUpGradeVerWrite,
//		EUpGradeVerRecv,
//		EUpGradeVerCheck,
//		ENoUpGrade,
//		EUpGradeExists,
		EEditSynchingAlbum,
		ESubStatus,
		ESmsData,
		EGetMoreSmsData,
		ESecureSocket
		//EGetImgNext
#else
		EIdle,
		EConnecting,
		ESendRecv
#endif
	};

/*!
  @function ChangeStatus
   
  @discussion handle a change in this object's status
  @param aNewStatus new status
  */
    void ChangeStatus(TSocketsEngineState aNewStatus);

/*!
  @function Print
   
  @discussion display text on the console
  @param aDes text to display
  */
//    void Print(const TDesC& aDes);

private: // Member variables

    /*! @const The maximum time allowed for a lookup or connect requests to complete */
    static const TInt KTimeOut;

    /*! @const The initial port number displayed to the user */
    //static const TInt KDefaultPortNumber;

    /*! @var this object's current status */
	TSocketsEngineState         iEngineStatus;

    /*! @var timer active object */
	CTimeOutTimer*              iTimer;

#ifndef WAP_PT

	RSocket                     iSocket;

    /*! @var the socket server */
	RSocketServ                 iSocketServ;

    /*! @var The address to be used in the connection */
	TInetAddr                   iAddress;

	RHostResolver           iResolver;
	
	TNameEntry              iNameEntry;

	/*! @var for reading */
	TSockXfrLength          iDummyLength;

	TInt                    iResolvedLocal; // locally resolved url

	//TInt ResolveLocalDNS();

	//HBufC8* iBufCPload;
#ifndef __SSL6__
#ifdef __SSL7__
	CSecureSocket* iTlsSocket;

	//TSockXfrLength iBytesSent;
#endif
#endif

#else
	HBufC8* iBufCPload;
	RWAPServ	iWapServ;
	CCapCodec*  iCap;
	RWSPCOConn	iConnection;
	RWSPCOTrans	iCOTrans;
	RWSPCOTrans iTransSynch;
	RWSPCOConn::TEvent iEvent;
    TInt port;

	TInt32 DefaultAccessPointId(TUint32& aId);
	TInt GetGatewayAddress(TDes8& addr);
	void SendTransaction(RWSPCOConn& aConn, RWSPCOTrans& aCOTrans, RWSPCOTrans& aTrans);
	void MakeContentLength(TUint size, TDes8& header);

	void GetProxyAddress(TUint32 aId, TDes& addr);

	//void ProcessRegisterResponse();
#endif // WAP_PT

    /*! @var server name to connect to */
    TBuf<50>  iServerName;
    /*! @var port number to connect to */
    TInt                        iPort;

    TBuf8<1024>                  iData;

	/*! @var registration */
	TInt						iRegister;
//	TInt						iUpGrade;
    };

#endif // __SOCKETSENGINEWRITER_H__

