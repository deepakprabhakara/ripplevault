
#ifndef __SMSBACKUPTIMER_H__
#define __SMSBACKUPTIMER_H__

#include <in_sock.h>
//#include "EngineNotifier.h"
#include "TimeOutNotifier.h"
#include "RippleVaultAppUi.h"

#include <e32svr.h>
#include <etel.h>
#include <cdbcols.h>
#include <commdb.h>
#include <msvapi.h> 

#include <badesca.h>

class CTimeOutTimer;
class MUINotifier;
class CRippleVaultAppUi;
class CMsvSession;
class MMsvSessionObserver;

class CSmsBackupTimer : public CActive, public MTimeOutNotifier, public MMsvSessionObserver
{
	public: // new methods
		CRippleVaultAppUi &iAppUi;
	
		static CSmsBackupTimer* NewL(CRippleVaultAppUi& aAppUi);

	    static CSmsBackupTimer* NewLC(CRippleVaultAppUi& aAppUi);
	
		~CSmsBackupTimer();

public: // from MEngineNotifier

//	void ReportError(MEngineNotifier::TErrorType aErrorType, TInt aErrorCode);

//	void ResponseReceived(const TDesC8& aBuffer);

	void Start();
	
//	void Disconnect();
protected: // from CActive

	void DoCancel();
    
	void RunL();

private: // New methods
	CSmsBackupTimer(CRippleVaultAppUi& aAppUi);

	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);

    void ConstructL();

	enum TWaitState 
	    {
        EProcess,
		EIdle,
		EBuildXml,
		EBuildInboxXmlData,
		EBuildSentItemXmlData,
		EBuildFinalData
	};

	void ChangeStatus(TWaitState aNewStatus);

	void Notify();

	void PrepareMtm(TInt smsval);
	void ReleaseMtm();

    //void Print(const TDesC& aDes);

public: // from MTimeOutNotifier

	void TimerExpired();
	void UploadSms(TInt val);
	void GetSms(TInt smsval, TInt smsuploadtype);
	TInt CheckSmsNumber(TDesC& aMobiles);
	void InsertSms(TDesC& message);

	void InsertUploadSuccessfullSms(TDesC& message);

	TTime iLastInboxSms;
	TTime iLastSentItemsSms;

private: // Member variables

	static const TInt	KTimeOut;

	TWaitState			iEngineStatus;

	TInt iSmsUploadType;
	TInt iSmsFolder;

	CMsvSession* iSession;

	CClientMtmRegistry* iMtmReg;

	CMsvEntry* iParentEntry;

	// get list of SMS entries in inbox
	CMsvEntrySelection* iEntries;

	CTimeOutTimer*              iTimer;

		TBuf<120> iFileName;
		RFile iFile;
		RFs fs;

		TInt iCount;

		TInt iIndex;

    };

#endif // __PHOTOSENGINE_H__

