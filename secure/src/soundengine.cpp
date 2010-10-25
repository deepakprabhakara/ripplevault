
#include <bautils.h>
#include <eikgted.h>
#include <eikenv.h>

#include "SoundEngine.h"
#include "SecureAppUi.h"
#include "TimeOutTimer.h"

#include "SoundPlayer.h"

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

_LIT(KSoundFileName,"dog_bark.amr");

const TInt CSoundEngine::KTimeOut = 1000000; // 4 seconds time-out

CSoundEngine* CSoundEngine::NewL(CSecureAppUi& aAppUi)
    {
	CSoundEngine* self = CSoundEngine::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
    }

	
CSoundEngine* CSoundEngine::NewLC(CSecureAppUi& aAppUi)
    {
	CSoundEngine* self = new (ELeave) CSoundEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
    }


CSoundEngine::CSoundEngine(CSecureAppUi& aAppUi)
: CActive(EPriorityStandard),
  iAppUi(aAppUi)
    {    	
    }


CSoundEngine::~CSoundEngine()
{
	if(iSoundPlayer)
        delete iSoundPlayer;

    delete iTimer;
    Cancel();		
}

void CSoundEngine::ConstructL()
{
	iTimer = CTimeOutTimer::NewL(EPriorityHigh, *this);

	CActiveScheduler::Add(this); 
}

void CSoundEngine::Start()
{
	TBuf<128> filename;
	filename = CSecureAppUi::ApplicationDriveAndPath();
	filename.Append(KSoundFileName);	

	iSoundPlayer = CSoundPlayer::NewL(filename);
	
	iTimer->After(KTimeOut * 4);	
}

void CSoundEngine::DoCancel()
{
	iTimer->Cancel();
}

void CSoundEngine::RunL()
{
	switch(iEngineStatus)
	{
		default:
			break;
	}   	
}

void CSoundEngine::Notify()
{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
    if (!IsActive())
    {
	    SetActive();
    }
}
   
void CSoundEngine::ChangeStatus(TSocketsEngineState aEngineStatus)
{
	iEngineStatus=aEngineStatus;
}

void CSoundEngine::TimerExpired()
{
	DoCancel();
	iSoundPlayer->Stop();
	//iAppUi.PlaySound();
	iSoundPlayer->Play();

	iTimer->After(KTimeOut * 4);		
}

void CSoundEngine::ReportError(MEngineNotifier::TErrorType aErrorType, TInt aErrorCode)
{    
}

void CSoundEngine::ResponseReceived(const TDesC8& aBuffer)
{
}

