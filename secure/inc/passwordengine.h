/* Copyright (c) 2001, Nokia. All rights reserved */

#ifndef __PASSWORDENGINE_H__
#define __PASSWORDENGINE_H__

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

class CSecureAppUi;

class CPassWordEngine : public CActive
{
public: // new methods

	RFs iRFsession;	
	
	CSecureAppUi& iAppUi;

	static CPassWordEngine* NewL(CSecureAppUi& aAppUi);

    static CPassWordEngine* NewLC(CSecureAppUi& aAppUi);

	~CPassWordEngine();

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
	CPassWordEngine(CSecureAppUi& aAppUi);

    void ConstructL();	

private: // Member variables

    TSocketsEngineState iEngineStatus;		
};

#endif

