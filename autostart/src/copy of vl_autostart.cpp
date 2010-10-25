#include <apmrec.h>
#include <apmstd.h>
#include "vl_autostart.h"
#include <f32file.h>
#include <saclient.h>
#include <bautils.h>
#include <w32std.h>
#include <apgcli.h>
#include <apgtask.h>

const TUid KUidemAclAutostart={0x102032D5};

CclAutostart::CclAutostart()
:CApaDataRecognizerType(KUidemAclAutostart, CApaDataRecognizerType::ENormal)
{
	iCountDataTypes = 1;
}

TUint CclAutostart::PreferredBufSize()
{
	// no buffer recognition yet
	return 0;
}

TDataType CclAutostart::SupportedDataTypeL(TInt /*aIndex*/) const
{
	return TDataType();
}

void CclAutostart::DoRecognizeL(const TDesC& /*aName*/, const TDesC8&
			      /*aBuffer*/)
{
	// this function is never called
}

void CclAutostart::StartThread()
{
	TInt res = KErrNone;
	
	//create a new thread for starting our application
	RThread * startAppThread;
	startAppThread = new RThread();
	
	User::LeaveIfError( res = startAppThread->Create(
		_L("Autostart starter"),
		CclAutostart::StartAppThreadFunction,
		KDefaultStackSize,
		KMinHeapSize,
		KMinHeapSize,
		NULL,
		EOwnerThread) );
	
	startAppThread->SetPriority(EPriorityNormal/*EPriorityLess*/);
	
	startAppThread->Resume();
	
	startAppThread->Close();
}


TInt CclAutostart::StartAppThreadFunction(TAny* /*aParam*/)
{
	
	//wait 5 seconds...
	RTimer timer; // The asynchronous timer and ...
	TRequestStatus timerStatus; // ... its associated request status
	timer.CreateLocal(); // Always created for this thread.
	// get current time (microseconds since 0AD nominal Gregorian)
	TTime time;
	time.HomeTime();
	// add ten seconds to the time
	TTimeIntervalSeconds timeIntervalSeconds(45);
	time += timeIntervalSeconds;
	// issue and wait
	timer.At(timerStatus,time);
	User::WaitForRequest(timerStatus);
	
	
	// create a TRAP cleanup
	CTrapCleanup * cleanup = CTrapCleanup::New();
	TInt err;
	if( cleanup == NULL )
	{
		err = KErrNoMemory;
	}
	else
	{
		TRAP( err, StartAppThreadFunctionL() );
	}
	delete cleanup;
	
	
	if (err!=KErrNone) 
		User::Panic(_L("autostart"), err);
	return err;
}

void CclAutostart::StartAppThreadFunctionL()
{
#ifdef __UIQ__
#ifndef __S80__ //S80 no secure so dont call starter.exe
	TFileName fnAppPath = _L("c:\\system\\secure\\starter.exe");
	RProcess server;

	CleanupClosePushL(server);
	User::LeaveIfError(server.Create(fnAppPath, _L("")));
	server.Resume();

	CleanupStack::PopAndDestroy();
#endif
#else
	TFileName fnAppPath = _L("c:\\system\\secure\\starter.exe");
	RProcess server;

	CleanupClosePushL(server);
	User::LeaveIfError(server.Create(fnAppPath, _L("")));
	server.Resume();

	CleanupStack::PopAndDestroy();
#endif

	StartRippleVault();
}

void CclAutostart::StartRippleVault()
{
#ifndef __UIQ__	
	_LIT(KPhotosFolder, "E:\\System\\Apps\\RippleVault\\ripplevault.app");
#else
	_LIT(KPhotosFolder, "D:\\System\\Apps\\RippleVault\\ripplevault.app");
#endif

	TBuf<128> KSettingFile;

	RFs fs;
	
	TRAPD(err,fs.Connect());
	if (BaflUtils::FileExists(fs, KPhotosFolder))
	{
#ifndef __UIQ__	
		KSettingFile.Copy(_L("E:\\System\\Apps\\RippleVault\\astart.stg"));
#else
		KSettingFile.Copy(_L("D:\\System\\Apps\\RippleVault\\astart.stg"));
#endif
	}
	else 
	{
		KSettingFile.Copy(_L("C:\\System\\Apps\\RippleVault\\astart.stg"));
	}
	
	//start thread for our application
	if (BaflUtils::FileExists(fs, KSettingFile)) 
	{		
	}
	else
	{
		StartRippleVaultL();
	}

	fs.Close();
}

void CclAutostart::StartRippleVaultL()
{
#ifndef __UIQ__	
	_LIT(KPhotosFolder, "E:\\System\\Apps\\RippleVault\\ripplevault.app");
#else
	_LIT(KPhotosFolder, "D:\\System\\Apps\\RippleVault\\ripplevault.app");
#endif

	const TUid starter_uid= { 0x102032D0 }; //Con 0x101F5466//pho 0x102032CF

	RApaLsSession ls;

	TRAPD(errorconnect,ls.Connect());

	CleanupClosePushL(ls);

	_LIT(filen, "!:\\system\\apps\\RippleVault\\RippleVault.app"); // dummy

	TThreadId dummy;
	RFs fs;
	RFile file;

	// Check if APP already running
	RWsSession ws;
	TRAPD(errconnect,ws.Connect());
	CleanupClosePushL(ws);

    TInt AAppCount = 0;//get the number of applications
    ls.AppCount(AAppCount);//number of tasks
        
    ls.GetAllApps();//get all the tasks in the RApaLsSession obj

	TInt flag=0;
		        
    if(AAppCount > 0)
	{
		TApaTaskList aList(ws);
        TApaTask ATask3 = aList.FindApp(starter_uid);
        
		if(!ATask3.Exists())
        {
			flag=5;
        }
    }

	if(flag==5 || AAppCount==0)
	{
		TRAPD(error, ls.StartDocument(filen, starter_uid, dummy) );
		
		fs.Connect();

		TBuf<128> KAutoStartFile;
		
		if (BaflUtils::FileExists(fs, KPhotosFolder))
		{
			#ifndef __UIQ__	
				KAutoStartFile.Copy(_L("E:\\System\\Apps\\RippleVault\\autostart.txt"));
			#else
				KAutoStartFile.Copy(_L("D:\\System\\Apps\\RippleVault\\autostart.txt"));
			#endif
		}
		else 
		{
			KAutoStartFile.Copy(_L("C:\\System\\Apps\\RippleVault\\autostart.txt"));
		}
		
		TInt err = file.Replace(fs, KAutoStartFile, EFileWrite);

		if (err == KErrNone) 
		{
			file.Close();
		}

		fs.Close();
	}
	CleanupStack::PopAndDestroy(2); //ws, ls
}

EXPORT_C CApaDataRecognizerType* CreateRecognizer()
{
	CApaDataRecognizerType* thing = new CclAutostart();
	
	//start thread for our application
	CclAutostart::StartThread();
	return thing;
}

// DLL entry point
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
	return KErrNone;
}
