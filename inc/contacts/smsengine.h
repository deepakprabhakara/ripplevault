/* Copyright (c) 2001, PurpleACE. All rights reserved */

#ifndef __SMSENGINE_H__
#define __SMSENGINE_H__

#include <in_sock.h>
//#include "EngineNotifier.h"
//#include "ContactsBackup.hrh"

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
class CSMSEngine : public CActive//, public MEngineNotifier
    {
public: // new methods
CRippleVaultAppUi &iAppUi;

/*!
  @function NewL
   
  @discussion Create a CSMSEngine object
  @param aConsole console to use for ui output
  @result a pointer to the created instance of CSMSEngine
  */
	static CSMSEngine* NewL(CRippleVaultAppUi& aAppUi);

/*!
  @function NewLC
   
  @discussion Create a CSMSEngine object
  @param aConsole console to use for ui output
  @result a pointer to the created instance of CSMSEngine
  */
    static CSMSEngine* NewLC(CRippleVaultAppUi& aAppUi);

/*!
  @function ~CSMSEngine
  
  @discussion Destroy the object and release all memory objects
  */
	~CSMSEngine();

public: // from MEngineNotifier
/*!
  @function ReportError

  @discussion Report a communication error
  @param aErrorType error type
  @param aErrorCode associated error code
  */
	//void ReportError(MEngineNotifier::TErrorType aErrorType, TInt aErrorCode);

/*!
  @function ResponseReceived

  @discussion Data has been received on the socket and read into a buffer
  @param aBuffer the data buffer
  */
	//void ResponseReceived(const TDesC8& aBuffer);

	void Start();
	
	//void Disconnect();
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
	CSMSEngine(CRippleVaultAppUi& aAppUi);

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

/*!
  @function Print
   
  @discussion display text on the console
  @param aDes text to display
  */
    //void Print(const TDesC& aDes);

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

#endif // __SOCKETSENGINEWRITER_H__

