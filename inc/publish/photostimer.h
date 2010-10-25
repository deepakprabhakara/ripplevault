

#ifndef __PHOTOTIMER_H__
#define __PHOTOTIMER_H__

//#include "EngineNotifier.h"
#include "TimeOutNotifier.h"
#include "RippleVaultAppUi.h"

#include <e32svr.h>

class CTimeOutTimer;
class MUINotifier;
class CRippleVaultAppUi;

class CPhotosTimer : public CActive, public MTimeOutNotifier
{
	public: // new methods
		CRippleVaultAppUi &iAppUi;
	
		static CPhotosTimer* NewL(CRippleVaultAppUi& aAppUi);

	    static CPhotosTimer* NewLC(CRippleVaultAppUi& aAppUi);
	
		~CPhotosTimer();

public: // from MEngineNotifier

	void Start();
	
protected: // from CActive

	void DoCancel();
    
	void RunL();

private: // New methods
	CPhotosTimer(CRippleVaultAppUi& aAppUi);

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

#endif // __PHOTOSENGINE_H__

