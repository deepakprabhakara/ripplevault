
#ifndef __SMSBACKUPAO_H__
#define __SMSBACKUPAO_H__

#include <e32base.h>

#include <in_sock.h>
//#include "EngineNotifier.h"
#include "TimeOutNotifier.h"
#include "RippleVaultAppUi.h"

#include <e32svr.h>
#include <etel.h>
#include <cdbcols.h>
#include <commdb.h>
#include <msvapi.h> 

//class CTimeOutTimer;
class MUINotifier;
class CRippleVaultAppUi;
class CMsvSession;
class MMsvSessionObserver;

class CSmsBackupAO : public CActive, public MMsvSessionObserver
{
	public: // new methods
		CRippleVaultAppUi &iAppUi;
	
		static CSmsBackupAO* NewL(CRippleVaultAppUi& aAppUi);

	    static CSmsBackupAO* NewLC(CRippleVaultAppUi& aAppUi);
	
		~CSmsBackupAO();

public: // from MEngineNotifier

	//void ReportError(MEngineNotifier::TErrorType aErrorType, TInt aErrorCode);

	//void ResponseReceived(const TDesC8& aBuffer);

	//void Start();
	
	//void Disconnect();
protected: // from CActive

	void DoCancel();
    
	void RunL();

private: // New methods
	CSmsBackupAO(CRippleVaultAppUi& aAppUi);

	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);

    void ConstructL();

	enum TWaitState 
	    {
        EProcess,
        EWait
	};

	void ChangeStatus(TWaitState aNewStatus);

    void Print(const TDesC& aDes);

public: // from MTimeOutNotifier

	void Start(CArrayFix<TInt>* aSelectedSms, TInt aFolderType);
	void GetSms(TInt iPos);

	CArrayFix<TInt>* iSelectedSms;

	TInt iCount;
	TInt iCurr;
	TInt smsPath;

	CMsvSession* iSession;

	CClientMtmRegistry* iMtmReg;

	CMsvEntry* parentEntry;
	
	CMsvEntrySelection* entries;

private: // Member variables

//	static const TInt	KTimeOut;

	TWaitState			iEngineStatus;

	
//	CTimeOutTimer*              iTimer;

		TBuf<120> iFileName;
		RFile iFile;
		RFs fs;

		//TInt iCount;

    };

#endif // __PHOTOSENGINE_H__

