

#ifndef __SMSSIMENGINE_H__
#define __SMSSIMENGINE_H__

#include <in_sock.h>
//#include "EngineNotifier.h"
//#include "RippleVault.hrh"

#ifndef __UIQ__
#include <aknwaitdialog.h> 
#include <aknprogressdialog.h> 
#else
#include <eikdialg.h> 
#include <eikprogi.h>
#endif

#include <e32svr.h>
#include <etel.h>
#include <es_sock.h>
#include <gsmumsg.h>
#include <gsmubuf.h>
//#include <gsmuset.h>
#include <smsuaddr.h> 
#include <smsustrm.h>
#include <f32file.h>
#include <commdb.h>
#include <cdbcols.h>
#include <smut.h>

class CRippleVaultAppUi;
/*! 
  @class CSocketsEngineWriter
  
  @discussion This class is the main engine part of the sockets application.
  It establishes a TCP connection using its server name and port number (performing a DNS lookup
  operation first, if appropriate).
  It creates instances of separate active objects to perform reading from, and writing to, the socket.
  */
class CSmsSimEngine : public CActive//, public MEngineNotifier
    {
public: // new methods
	CRippleVaultAppUi &iAppUi;	
	void ExitApp();

/*!
  @function NewL
   
  @discussion Create a CSMSEngine object
  @param aConsole console to use for ui output
  @result a pointer to the created instance of CSMSEngine
  */
	static CSmsSimEngine* NewL(CRippleVaultAppUi& aAppUi);

/*!
  @function NewLC
   
  @discussion Create a CSmsSimEngine object
  @param aConsole console to use for ui output
  @result a pointer to the created instance of CSMSEngine
  */
    static CSmsSimEngine* NewLC(CRippleVaultAppUi& aAppUi);

/*!
  @function ~CSMSEngine
  
  @discussion Destroy the object and release all memory objects
  */
	~CSmsSimEngine();

public: // from MEngineNotifier

	void Start();

	void Disconnect();
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

private: // New methods
/*!
  @function CSMSEngine
  
  @discussion Perform the first phase of two phase construction 
  @param aConsole the console to use for ui output
  */
	CSmsSimEngine(CRippleVaultAppUi& aAppUi);

/*!
  @function ConstructL
  
  @discussion Perform the second phase construction of a CSMSEngine 
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
	enum TWaitState 
	    {
        EDeleteSMS,
        EProcessSMS
	};

/*!
  @function ChangeStatus
   
  @discussion handle a change in this object's status
  @param aNewStatus new status
  */
    void ChangeStatus(TWaitState aNewStatus);

private: // Member variables
	RSocket		iSocket;
	//CSmsMessage*		iSmsMsg;
	TPckgBuf<TUint> 	iOctlResult;

//	MGdpPacketHandler*  iHandler;
	TInt				iRetries;	// Remaining...

	// States
	TWaitState			iEngineStatus;

	RSocketServ iSocketServer;
	RFs iFs;
    };

#ifdef __UIQ__
//Progress Dialog
class CSmsProgInfoControl : public CEikDialog
	{
public:
	CSmsProgInfoControl(CSmsSimEngine& aAppUi);
	~CSmsProgInfoControl();
private:
	CSmsSimEngine & iAppUi;	
	void 	PreLayoutDynInitL();	
	TInt    OkToExitL(TInt aKeycode);	
	};	

#endif	

#endif // __SOCKETSENGINEWRITER_H__

