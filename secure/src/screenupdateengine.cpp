
#include <bautils.h>
#include <eikgted.h>
#include <eikenv.h>

#include "ScreenUpdateEngine.h"
#include "SecureAppUi.h"
#include "TimeOutTimer.h"

#ifdef __UIQ__	
	
#ifdef __S80__
	#include <secure_s80.rsg>
	#include "Secure_s80.hrh"
#else
	#include <secure_uiq.rsg>
	#include "Secure_uiq.hrh"
#endif

#else
	#include <Secure.rsg>
	#include "Secure.hrh"
#endif

class CSecureAppUi;


const TInt CScreenUpdateEngine::KTimeOut = 60000000; // 60 seconds time-out

CScreenUpdateEngine* CScreenUpdateEngine::NewL(CSecureAppUi& aAppUi)
    {
	CScreenUpdateEngine* self = CScreenUpdateEngine::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
    }

	
CScreenUpdateEngine* CScreenUpdateEngine::NewLC(CSecureAppUi& aAppUi)
    {
	CScreenUpdateEngine* self = new (ELeave) CScreenUpdateEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
    }


CScreenUpdateEngine::CScreenUpdateEngine(CSecureAppUi& aAppUi)
: CActive(EPriorityStandard),
  iAppUi(aAppUi)
    {    	
    }


CScreenUpdateEngine::~CScreenUpdateEngine()
{
    delete iTimer;
    Cancel();		
}

void CScreenUpdateEngine::ConstructL()
{
	iTimer = CTimeOutTimer::NewL(EPriorityHigh, *this);

	CActiveScheduler::Add(this); 
}

void CScreenUpdateEngine::Start()
{
	iCount = 1;
	iTimer->After(KTimeOut * 1);	
}

void CScreenUpdateEngine::DoCancel()
{
	iTimer->Cancel();
}

void CScreenUpdateEngine::RunL()
{
	switch(iEngineStatus)
	{
		default:
			break;
	}   	
}

void CScreenUpdateEngine::Notify()
{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
    if (!IsActive())
    {
	    SetActive();
    }
}
   
void CScreenUpdateEngine::ChangeStatus(TSocketsEngineState aEngineStatus)
{
	iEngineStatus=aEngineStatus;
}

void CScreenUpdateEngine::TimerExpired()
{
	DoCancel();

#ifdef __UIQ__
	iAppUi.ShowScreen();
#else
	iAppUi.BringApptoForeground();
#endif

	iAppUi.RestartSound();


	//iCount = iCount + 3;

	//if(iCount != 10)
	iTimer->After(KTimeOut * 3);
}

void CScreenUpdateEngine::ReportError(MEngineNotifier::TErrorType aErrorType, TInt aErrorCode)
{    
}

void CScreenUpdateEngine::ResponseReceived(const TDesC8& aBuffer)
{
}

