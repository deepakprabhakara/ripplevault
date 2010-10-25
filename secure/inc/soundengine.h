/* Copyright (c) 2001, Nokia. All rights reserved */

#ifndef __SOUNDENGINE_H__
#define __SOUNDENGINE_H__

#include <in_sock.h>
#include <f32file.h> 
#include <e32base.h> 
#include <s32file.h>
#include <eikmenup.h> 
#include <badesca.h>

#include "SecureAppUi.h"
#include "TimeOutNotifier.h"
#include "EngineNotifier.h"
	
class CTimeOutTimer;
class MUINotifier;
class CSoundPlayer;

class CSecureAppUi;

class CSoundEngine : public CActive, public MEngineNotifier,public MTimeOutNotifier 
{
public: // new methods

	CSecureAppUi& iAppUi;

	static CSoundEngine* NewL(CSecureAppUi& aAppUi);

    static CSoundEngine* NewLC(CSecureAppUi& aAppUi);

	void TimerExpired();
	
	~CSoundEngine();

    void Notify();

	enum TSocketsEngineState 
	    {
	    EOpenMenu,	    
		EWritten
		};

	void ChangeStatus(TSocketsEngineState aEngineStatus);

	void ReportError(MEngineNotifier::TErrorType aErrorType, TInt aErrorCode);

	void ResponseReceived(const TDesC8& aBuffer);

	void Start();

protected: // from CActive
	void DoCancel();

    void RunL();

private: // New methods
	CSoundEngine(CSecureAppUi& aAppUi);

    void ConstructL();

private: // Member variables

	CSoundPlayer*	iSoundPlayer;
	CTimeOutTimer*	iTimer;
	static const TInt KTimeOut;  

    TSocketsEngineState iEngineStatus;	
};

#endif // __PHOTOSENGINE_H__

