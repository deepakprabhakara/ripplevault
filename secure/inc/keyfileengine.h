/* Copyright (c) 2001, Nokia. All rights reserved */

#ifndef __KEYFILEENGINE_H__
#define __KEYFILENGINE_H__

#include <in_sock.h>

#ifndef __UIQ__
	#include <aknquerydialog.h>
#endif

#include <f32file.h> 
#include <e32base.h> 
#include <s32file.h>
#include <eikmenup.h> 
#include <badesca.h>
#include "SecureAppUi.h"

#ifdef __UIQ__
	#include "TimeOutNotifier.h"
	
	class CPhotoTimeoutTimer;
	class MUINotifier;
#endif

class CSecureAppUi;

#ifdef __UIQ__
	class CKeyFileEngine : public CActive, public MTimeOutNotifier 
#else
	class CKeyFileEngine : public CActive
#endif
{
public: // new methods

	RFs iRFsession;	
	
	CSecureAppUi& iAppUi;

	static CKeyFileEngine* NewL(CSecureAppUi& aAppUi);

    static CKeyFileEngine* NewLC(CSecureAppUi& aAppUi);

#ifdef __UIQ__
	void TimerExpired();
#endif
	
	~CKeyFileEngine();

    void Notify();

	enum TSocketsEngineState 
	    {
		EWritten,		
		EOpenMenu	
	};

	void ChangeStatus(TSocketsEngineState aEngineStatus);
	
protected: // from CActive
	void DoCancel();

    void RunL();

private: // New methods
	CKeyFileEngine(CSecureAppUi& aAppUi);

    void ConstructL();	

private: // Member variables

#ifdef __UIQ__
	CPhotoTimeoutTimer*              iTimer;
	static const TInt KTimeOut;  
#endif
    TSocketsEngineState iEngineStatus;		
};

#endif

