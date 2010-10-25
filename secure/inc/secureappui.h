/*
* ============================================================================
*  Name     : CSecureAppUi from SecureAppUi.h
*  Part of  : Secure
*  Created  : 8/6/2005 by Ezhil
*  Description:
*     Declares UI class for application.
*  Version  :
*  Copyright: Purpleace
* ============================================================================
*/

#ifndef SECUREAPPUI_H
#define SECUREAPPUI_H

// INCLUDES
#include <eikapp.h>
#include <eikdoc.h>
#include <e32std.h>
#include <coeccntx.h>
#include <f32file.h> 
#include <w32std.h>

#include <msvapi.h> 

// FORWARD DECLARATIONS
#ifdef __UIQ__
#ifdef __S80__
	#include <Eikappui.h>
	#include <eikmenup.h>
	#include <eikon.hrh>
	#include <eikdialg.h>

	class CSecureAppView;
#else
	#include <qikappui.h>
	#include <eikmenup.h>
	#include <eikon.hrh>
	#include <eikdialg.h>

	class CSecureAppView;
#endif
#else
	#include <aknappui.h>

	class CSecureContainer;
#endif


class CSmsLockEngine;
class CSmsUnLockEngine;
class CSoundEngine;
class CSmsVacuumEngine;
class CSmsFile;
class CPassWordEngine;
class CScreenUpdateEngine;
class CSmsUnSubscribeEngine;

class CMsvSession;
class MMsvSessionObserver;

// CONSTANTS
//const ?type ?constant_var = ?constant;


// CLASS DECLARATION

/**
* Application UI class.
* Provides support for the following features:
* - EIKON control architecture
* 
*/
#ifdef __UIQ__
#ifdef __S80__
	class CSecureAppUi : public CEikAppUi, public MMsvSessionObserver
#else
	class CSecureAppUi : public CQikAppUi, public MMsvSessionObserver
#endif
#else
	class CSecureAppUi : public CAknAppUi, public MMsvSessionObserver
#endif
    {
    public: // // Constructors and destructor

        /**
        * EPOC default constructor.
        */      
        void ConstructL();

        /**
        * Destructor.
        */      
        ~CSecureAppUi();
        
    public: // New functions
		CSmsLockEngine* iSmsLockEngine;
		CSmsUnLockEngine* iSmsUnLockEngine;	
		CSoundEngine* iSoundEngine;
		CSmsVacuumEngine* iSmsVacuumEngine;
		CSmsFile* iSmsFile;
		CPassWordEngine *iPassWordEngine;
		CScreenUpdateEngine *iScreenUpdateEngine;
		CSmsUnSubscribeEngine *iSmsUnSubscribeEngine;

    public: // Functions from base classes
		TBuf<2000> iPassWord;
		TBuf<20> iMobileNumber;
		TBuf<300> iDisplayText;
		TBuf<20> iAutoStart;

		static TInt LockPhone(TAny * params);

#ifdef __UIQ__
		void ShowScreen();
		CSecureAppView* iAppView;
		TInt PasswordDialogForLock();
		RThread* startAppThread; 
#else
		static TInt LockKeys(TAny * params);
		void UnLockPhonePunching();
		void StartThreadForKeyLock();
		void BringApptoForeground();
		RThread* startAppThread; 
		RThread* startAppThread1;
#endif
		void UnLockPhone();
		void HandleForegroundEventL(TBool aForeground);

		void BringToForeground();
		void SendToBackground();

		void DeleteSms(TInt aSmsType);
		TInt DeleteSmsForUnLocking();
		TInt CheckSmsForLocking();

		void WriteLockPhone();
		static TFileName ApplicationDriveAndPath();

		void CheckPasswordForLocking(TInt aMode);
		void CheckPasswordForUnLocking();
		void CheckPwdPunching();

		TBool LoadFromConfigFile(TDes& aValue, TDesC& aParam, TInt val);
		TInt64 ReturnPower(TInt a, TInt b);

		void StartThread();
		void PlaySound();
		void CheckPasswordForVacuum(TInt aMode);
		void DeleteContacts();
		void DeletePhotos(TInt aMode);
		void DeleteVideos(TInt aMode);
		TInt PasswordDialog();
		void SendSms(TInt val);
		void WriteImsiNumber();
		TInt ReadImsiNumber();
		void GetImsiNumber();
		void WriteImsiIfNULL();
		void DeleteFile();
		void DeleteUnlock();
		void Deletelock(TInt aMode);
		void WriteSendersNumber(TInt aMode);
		void DeleteRegistration();
		void RestartSound();
		void CheckPasswordForUnSubscribe(TDes& aPassword,TDes& aNumber);
		void SendConfirmation();
		TInt CheckRegistration();
		void DeleteSecureStopFile();
		void CreateSecureStopFile();
		void DeleteSendersFile();

		void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* /*aArg3*/);

		void WriteLogFile(TDes8& Text);
		RFs iLogSession;	
		RFile LogFile;	
		TBuf8<300> LogText;
		TInt flag;
		TBuf<128> iImsiNumber;

		void KillBackupApp(); 

    private:
        // From MEikMenuObserver
        void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);

    private:
        /**
        * From CEikAppUi, takes care of command handling.
        * @param aCommand command to be handled
        */
        void HandleCommandL(TInt aCommand);

        /**
        * From CEikAppUi, handles key events.
        * @param aKeyEvent Event to handled.
        * @param aType Type of the key event. 
        * @return Response code (EKeyWasConsumed, EKeyWasNotConsumed). 
        */
        virtual TKeyResponse HandleKeyEventL(
            const TKeyEvent& aKeyEvent,TEventCode aType);

    private: //Data
#ifdef __UIQ__	
#else
        CSecureContainer* iAppContainer; 
#endif
    };


#ifdef __UIQ__	
////////////////////////////////////////////////////////////////////////
//
// CPassWordDialog
//
////////////////////////////////////////////////////////////////////////
class CPassWordDialog : public CEikDialog
	{
public:
	CPassWordDialog(CSecureAppUi& aAppUi);
	~CPassWordDialog();
private:
	CSecureAppUi & iAppUi;
	TInt    OkToExitL(TInt aKeycode);	
	};	
////////////////////////////////////////////////////////////////////////
//
// CPassWordDialog
//
////////////////////////////////////////////////////////////////////////
class CPassWordDialogForLock : public CEikDialog
	{
public:
	CPassWordDialogForLock(CSecureAppUi& aAppUi);
	~CPassWordDialogForLock();
private:
	CSecureAppUi & iAppUi;
	TInt    OkToExitL(TInt aKeycode);	
	};	

#endif

#endif

// End of File
