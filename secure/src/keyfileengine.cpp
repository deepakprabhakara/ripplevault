
#include <bautils.h>
#include <eikgted.h>
#include "KeyFileEngine.h"
#include "SecureAppUi.h"
#include <eikenv.h>


#include <Secure.rsg>
#include "Secure.hrh"

class CSecureAppUi;

_LIT(KKeyFile,	"key.dat");

#ifdef __UIQ__
	_LIT(KDir,"c:\\system\\secure");
	
	const TInt CKeyFileEngine::KTimeOut = 2000000; //1 seconds time-out
#else 

	_LIT(KDir,"c:\\system\\secure");	
#endif

CKeyFileEngine* CKeyFileEngine::NewL(CSecureAppUi& aAppUi)
    {
	CKeyFileEngine* self = CKeyFileEngine::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
    }

	
CKeyFileEngine* CKeyFileEngine::NewLC(CSecureAppUi& aAppUi)
    {
	CKeyFileEngine* self = new (ELeave) CKeyFileEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
    }


CKeyFileEngine::CKeyFileEngine(CSecureAppUi& aAppUi)
: CActive(EPriorityStandard),
  iAppUi(aAppUi)
    {    	
    }


CKeyFileEngine::~CKeyFileEngine()
{
#ifdef __UIQ__
     delete iTimer;
#endif
    iRFsession.NotifyChangeCancel(iStatus);

    Cancel();	
	iRFsession.Close();
}


void CKeyFileEngine::ConstructL()
{
	User::LeaveIfError(iRFsession.Connect());    
	
#ifdef __UIQ__
		iTimer = CPhotoTimeoutTimer::NewL(EPriorityHigh, *this);	
#endif

	if (BaflUtils::FileExists(iRFsession,KDir))
	{
		iRFsession.NotifyChange(ENotifyWrite,iStatus,KDir);
		
		ChangeStatus(EOpenMenu);	

		CActiveScheduler::Add(this); 

		SetActive();
	}
}

// from CActive
void CKeyFileEngine::DoCancel()
    {
#ifdef __UIQ__
		iTimer->Cancel();
#endif
    }

void CKeyFileEngine::RunL()
{
	int answer;
	
	switch(iEngineStatus)
	{
		case EOpenMenu:			
			{
				TBuf<150> path;
				path = CSecureAppUi::ApplicationDriveAndPath();					
				path.Append(KKeyFile);
				
				RFs fs;
				fs.Connect();
				
				if (BaflUtils::FileExists(fs, path))
				{
					iAppUi.BringApptoForeground();

					BaflUtils::DeleteFile(fs, path);
				}
			
				fs.Close();

				ChangeStatus(EWritten);
				Notify();
			}			
			break;
		case EWritten:
			{		 			
				ChangeStatus(EOpenMenu);			
				
				iRFsession.NotifyChange(ENotifyFile, iStatus, KDir);
				SetActive();
			}
			break;
		default:
			break;
	}   	
}

void CKeyFileEngine::Notify()
{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
    if (!IsActive())
    {
	    SetActive();
    }
}
   
void CKeyFileEngine::ChangeStatus(TSocketsEngineState aEngineStatus)
{
	iEngineStatus=aEngineStatus;
}

#ifdef __UIQ__
void CKeyFileEngine::TimerExpired()
{
	Notify();
}
#endif
