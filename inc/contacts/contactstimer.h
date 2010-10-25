/* Copyright (c) 2001, Nokia. All rights reserved */

#ifndef __CONTACTSTIMER_H__
#define __CONTACTSTIMER_H__

#include <in_sock.h>
#include "EngineNotifier.h"
#include "TimeOutNotifier.h"
#include "RippleVaultAppUi.h"

//#include <e32svr.h>
//#include <etel.h>
//#include <etelbgsm.h>
//#include <cdbcols.h>
//#include <commdb.h>

class CTimeOutTimer;
class MUINotifier;
class CRippleVaultAppUi;

class CContactsTimer : public CActive, public MEngineNotifier, public MTimeOutNotifier
{
	public: // new methods
		CRippleVaultAppUi &iAppUi;
	
		static CContactsTimer* NewL(CRippleVaultAppUi& aAppUi);

	    static CContactsTimer* NewLC(CRippleVaultAppUi& aAppUi);
	
		~CContactsTimer();

public: // from MEngineNotifier

	void ReportError(MEngineNotifier::TErrorType aErrorType, TInt aErrorCode);

	void ResponseReceived(const TDesC8& aBuffer);

	void Start();
	
	void Disconnect();
protected: // from CActive

	void DoCancel();
    
	void RunL();

private: // New methods
	CContactsTimer(CRippleVaultAppUi& aAppUi);

    void ConstructL();

	enum TWaitState 
	    {
        EProcess,
        EWait
	};

	void ChangeStatus(TWaitState aNewStatus);

    void Print(const TDesC& aDes);

public: // from MTimeOutNotifier

	void TimerExpired();

	void UploadContacts();

	//void CloseConnection();

private: // Member variables

	static const TInt	KTimeOut;

	TWaitState			iEngineStatus;

	
	CTimeOutTimer*              iTimer;

    };

#endif // __ContactsENGINE_H__

