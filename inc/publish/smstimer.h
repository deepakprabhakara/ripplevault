

#ifndef __SMSTIMER_H__
#define __SMSTIMER_H__

//#include "EngineNotifier.h"
#include "TimeOutNotifier.h"
#include "RippleVaultAppUi.h"

#include <e32svr.h>

class CTimeOutTimer;
class MUINotifier;
class CRippleVaultAppUi;

class CSmsTimer : public CActive, public MTimeOutNotifier
{
	public: // new methods
		CRippleVaultAppUi &iAppUi;
	
		static CSmsTimer* NewL(CRippleVaultAppUi& aAppUi);

	    static CSmsTimer* NewLC(CRippleVaultAppUi& aAppUi);
	
		~CSmsTimer();

public: // from MEngineNotifier

	void Start();
	
protected: // from CActive

	void DoCancel();
    
	void RunL();

private: // New methods
	CSmsTimer(CRippleVaultAppUi& aAppUi);

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

private: // Member variables

	static const TInt	KTimeOut;

	TWaitState			iEngineStatus;

	
	CTimeOutTimer*              iTimer;

    };

#endif // __SMSENGINE_H__

