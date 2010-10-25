
#ifndef __VIDEOSENGINE_H__
#define __VIDEOSENGINE_H__

#include <in_sock.h>
//#include "Sockets.hrh"
#ifndef __UIQ__
#include <aknquerydialog.h>
#endif
#include <f32file.h> 
#include <e32base.h> 
#include <s32file.h>
#include <eikmenup.h> 
#include <badesca.h>
#include "RippleVaultAppUi.h"

#ifdef __UIQ__
#include "TimeOutNotifier.h"
#endif

class CRippleVaultAppUi;

#ifdef __UIQ__
#define MODE_AUTO 0
#define MODE_PROMPT 1
#define MODE_OFF 2
class CPhotoTimeoutTimer;
class MUINotifier;
class CPhotosAppUi;
#endif
/*! 
  @class CSocketsEngine
  
  @discussion This class is the main engine part of the sockets application.
  It establishes a TCP connection using its server name and port number (performing a DNS lookup
  operation first, if appropriate).
  It creates instances of separate active objects to perform reading from, and writing to, the socket.
  */
#ifdef __UIQ__
class CVideosEngine : public CActive, public MTimeOutNotifier 
#else
class CVideosEngine : public CActive
#endif
    {
public: // new methods		

	RFs iRFsession;
	TBuf<160> comment;
	TBuf<160> description;
	TTime iCurrentTimeStamp;
#ifdef __UIQ__
	TTime TimeStamp;
	TBuf<160> imageFile;
     int PopUp;
#endif
	TBuf<100> tempp1;
	RFile TempFile;
	
	int Count;	
	TInt8 iNewFolder;
	TInt  iCount;
	TInt  iFilepos;
	TInt icompareflag1;

	CRippleVaultAppUi&		iAppUi;		
	
/*!
  @function NewL
   
  @discussion Create a CVideosEngine object
  @param aConsole console to use for ui output
  @result a pointer to the created instance of CVideosEngine
  */
	static CVideosEngine* NewL(CRippleVaultAppUi& aAppUi, TDesC& aFolderName);

/*!
  @function NewLC
   
  @discussion Create a CVideosEngine object
  @param aConsole console to use for ui output
  @result a pointer to the created instance of CVideosEngine
  */
    static CVideosEngine* NewLC(CRippleVaultAppUi& aAppUi, TDesC& aFolderName);

/*!
  @function ~CVideosEngine
  
  @discussion Destroy the object and release all memory objects
  */
	~CVideosEngine();

/*!
  @function Disconnect

  @discussion Disconnect socket
  */
	void Notify();

  //  void Disconnect();

/*!
  @function SetServerName

  @discussion Set name of server to connect to
  @param aName new server name
  */
//    void SetServerName(const TDesC& aName);

/*!
  @function ServerName

  @discussion Get server name
  @result name of server
  */
    //const TDesC& ServerName() const;

/*!
  @function SetPort

  @discussion Set port number to connect to
  @param aPort new port number
  */
    //void SetPort(TInt aPort);


/*!
  @function Port

  @discussion Get port number
  @result port number
  */
    //TInt Port() const;

	enum TSocketsEngineState 
	    {
		ESmartCharge,
		ECheckDisk,
		ECheckFile,
	    EOpenMenu,
		EFileOpen,
	    EOptionChosen,
		EDescription,
        EConnecting,
        EConnected,
		EWritten,		
		EConnectFolder,
		EProcessFile
	};

/*!
  @function Connected

  @discussion Is socket fully connected?
  @result true if socket is connected
  */
    //TBool Connected() const;

	void ChangeStatus(TSocketsEngineState aEngineStatus);
	void GetFolder();

#ifdef __UIQ__
void GetTimeStamp();
void TimerExpired();
#endif
	
protected: // from CActive
/*!
  @function DoCancel
   
  @discussion cancel any outstanding operation
  */
	void DoCancel();

/*!
  @function RunL
   
  @discussion called when operation complete
  */
    void RunL();

	TInt CheckCount();

private: // New methods
/*!
  @function CVideosEngine
  
  @discussion Perform the first phase of two phase construction 
  @param aConsole the console to use for ui output
  */
	CVideosEngine(CRippleVaultAppUi& aAppUi);

/*!
  @function ConstructL
  
  @discussion Perform the second phase construction of a CVideosEngine 
  */
    void ConstructL(TDesC& aFolderName);

	void ProcessImage();

private: // Member variables

    /*! @const The initial port number displayed to the user */
  //  static const TInt KDefaultPortNumber;

    /*! @var the actual socket */
	//RSocket                     iSocket;

    /*! @var the socket server */
	//RSocketServ                 iSocketServ;

    /*! @var The address to be used in the connection */
	//TInetAddr                   iAddress;

    /*! @var port number to connect to */
  //  TInt                        iPort;

    /*! @var server name to connect to */
//    TBuf<50>  iServerName;
    
    TSocketsEngineState iEngineStatus;
	
	TBool iPublic_Video;
	TBool iPublic_Photo;
	
#ifdef __UIQ__
CPhotoTimeoutTimer*              iTimer;
static const TInt KTimeOut;  
#endif
    };

#endif // __VIDEOSENGINE_H__

