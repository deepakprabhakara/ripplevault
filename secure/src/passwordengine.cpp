#ifdef __UIQ__
	#include <eiktxlbx.h>
#else
	#include <AknGlobalListQuery.h>
	#include <aknlists.h> 
	#include <aknpopup.h>
	#include <aknmessagequerydialog.h>
#endif

#include <bautils.h>
#include <eikgted.h>
#include "PasswordEngine.h"
#include "SecureAppUi.h"
#include <eikenv.h>

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

_LIT(KPasswordFile,	"password.dat");
//_LIT(KDir,"c:\\system\\apps\\secure");
_LIT(KDir,"c:\\system\\secure");

CPassWordEngine* CPassWordEngine::NewL(CSecureAppUi& aAppUi)
    {
	CPassWordEngine* self = CPassWordEngine::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
    }

	
CPassWordEngine* CPassWordEngine::NewLC(CSecureAppUi& aAppUi)
    {
	CPassWordEngine* self = new (ELeave) CPassWordEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
    }


CPassWordEngine::CPassWordEngine(CSecureAppUi& aAppUi)
: CActive(EPriorityStandard),
  iAppUi(aAppUi)
    {    	
    }


CPassWordEngine::~CPassWordEngine()
{
    iRFsession.NotifyChangeCancel(iStatus);

    Cancel();	
	iRFsession.Close();
}


void CPassWordEngine::ConstructL()
{
	User::LeaveIfError(iRFsession.Connect());  
	
	TBuf<128> dir;
	dir = CSecureAppUi::ApplicationDriveAndPath();
	
	if (BaflUtils::FileExists(iRFsession, KDir))
	{
		iRFsession.NotifyChange(ENotifyWrite,iStatus, KDir);
		
		ChangeStatus(EOpenMenu);	

		iAppUi.DeleteFile();

		CActiveScheduler::Add(this); 

		SetActive();
	}
}

// from CActive
void CPassWordEngine::DoCancel()
    {
    }

void CPassWordEngine::RunL()
{
	switch(iEngineStatus)
	{
		case EOpenMenu:			
			{
				TBuf<150> path;
				path = CSecureAppUi::ApplicationDriveAndPath();					
				path.Append(KPasswordFile);
				
				RFs fs;
				fs.Connect();
				
				if (BaflUtils::FileExists(fs, path))
				{
#ifdef __UIQ__
					iAppUi.DeleteUnlock();

					iAppUi.PasswordDialogForLock();					
#else

					iAppUi.StartThreadForKeyLock();
					
					iAppUi.DeleteUnlock();
					
					TBuf<160> password;
					CAknTextQueryDialog* dlg = CAknTextQueryDialog::NewL(password, CAknQueryDialog::ENoTone );
					if(dlg->ExecuteLD(R_PASSWORD_DIALOG))
					{
						iAppUi.DeleteFile();						
						iAppUi.iPassWord.Copy(password);
						iAppUi.CheckPwdPunching();
					}
					else
					{	
						iAppUi.DeleteFile();
						iAppUi.StartThread();
					}
#endif
				}
			
				fs.Close();

				ChangeStatus(EWritten);
				Notify();
			}			
			break;
		case EWritten:
			{		 			
				ChangeStatus(EOpenMenu); 
	
				TBuf<128> dir;
				dir = CSecureAppUi::ApplicationDriveAndPath();			
				
				iRFsession.NotifyChange(ENotifyWrite, iStatus, KDir);
				SetActive();
			}
			break;
		default:
			break;
	}   	
}

void CPassWordEngine::Notify()
{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
    if (!IsActive())
    {
	    SetActive();
    }
}
   
void CPassWordEngine::ChangeStatus(TSocketsEngineState aEngineStatus)
{
	iEngineStatus=aEngineStatus;
}

