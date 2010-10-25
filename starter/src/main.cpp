#include <e32base.h>
#include <e32cons.h>
#include <stdlib.h>
#include <apgcli.h>
#include <apgtask.h>
#include <w32std.h>
#include <apacmdln.h>
#include <f32file.h>
#include <bautils.h>

const TUid KSecureUid = { 0x102032D1 };

// This is the mainloop game loop or whatever
void mainloop(void)
{
	//CConsoleBase* console;
	//console=Console::NewL(_L("Test Consol"),TSize (KConsFullScreen,KConsFullScreen)); // Create a consol
	//CleanupStack::PushL(console);
	//console->Printf(_L("mainloop")); // write all your messages here
	//console->Getch(); // get and ignore character
	//CleanupStack::PopAndDestroy(); // close console

	_LIT(Securefile, "c:\\system\\Secure\\Secure.app"); //dummy
	
	CApaCommandLine *cmdline = CApaCommandLine::NewLC(Securefile);

	RApaLsSession ls;
	User::LeaveIfError(ls.Connect());
	CleanupClosePushL(ls);

	TThreadId context_log_threadid;

	//User::LeaveIfError( ls.StartDocument(Securefile, KSecureUid, context_log_threadid) );
	//TRAPD(error1, ls.StartApp(*cmdline));

	//console->Printf(_L("after startDocument")); // write all your messages here
	TRequestStatus context_log_status, timer_status;

	RWsSession ws;
	User::LeaveIfError(ws.Connect());
	CleanupClosePushL(ws);

	TApaTaskList tl(ws);

	TApaTask context_log_task=tl.FindApp(KSecureUid);
	if (!context_log_task.Exists()) 
	{
		TRAPD(error1, ls.StartApp(*cmdline));
	}

	//wake up on thread death
	RThread context_log_thread;
	context_log_thread.Open(context_log_threadid);
	context_log_thread.Logon(context_log_status);

	//restart every 5 sec
	RTimer timer; // The asynchronous timer and ...
	timer.CreateLocal(); // Always created for this thread.
	CleanupClosePushL(timer);

	TTime time;
	time.HomeTime();

	//TTimeIntervalHours wait_interval(1);
	TTimeIntervalSeconds wait_interval1(30);
	TTimeIntervalSeconds wait_interval(5);
	int wait_count=0; // we can only wait upto 5 sec at a time

	time += wait_interval1;
	//TBuf<30> dt;
	//time.FormatL(dt, _L("%F%Y-%M-%D %H:%T:%S\n"));
	//console->Printf(dt);

	timer.At(timer_status,time);

	bool done=false; bool restarting=false;
	int restart_count=0;

	while(!done) 
	{
		User::WaitForRequest(timer_status, context_log_status);
		if (timer_status!=KRequestPending) 
		{
			TApaTask context_log_task=tl.FindApp(KSecureUid);
			if (!context_log_task.Exists()) 
			{
				// restart
				RFs fs;	
				TRAPD(err,fs.Connect());

				_LIT(filen2, "c:\\system\\Secure\\SecureStop.dat");

				if(!(BaflUtils::FileExists(fs, filen2)))
				{
					TRAPD(error2, ls.StartApp(*cmdline));
				}

				fs.Close();	
			}
			else 
			{
				//context_log_task.SendSystemEvent(EApaSystemEventShutdown);
			}

			restarting=true; restart_count=0;
			wait_count=0;

			time.HomeTime();
			time += wait_interval;
			//time.FormatL(dt, _L("%F%Y-%M-%D %H:%T:%S\n"));
			//console->Printf(_L("%d "), timer_status);
			//console->Printf(dt);
			timer.At(timer_status, time);
		} 
		else if (context_log_status!=KRequestPending) 
		{
			//restart
			//TTimeIntervalMicroSeconds32 w(10*1000*1000);
			//User::After(w);
			TInt etype, ereason,error3;
			TExitCategoryName ecat;
			etype=context_log_thread.ExitType();
			ereason=context_log_thread.ExitReason();
			ecat=context_log_thread.ExitCategory();
			
			if ((etype!=0 || restarting)) 
			{
				RFs fs;	
				TRAPD(err,fs.Connect());

				_LIT(filen2, "c:\\system\\Secure\\SecureStop.dat");

				if(!(BaflUtils::FileExists(fs, filen2)))
				{
					TApaTask context_log_task=tl.FindApp(KSecureUid);

					if (!context_log_task.Exists()) 
						TRAPD(error3, ls.StartApp(*cmdline));
				}

				fs.Close();

				User::LeaveIfError(context_log_thread.Open(context_log_threadid));
				context_log_thread.Logon(context_log_status);
				restarting=false;
				restart_count++;
			} 
			else 
			{
				// user shutdown
				done=true;
			}
		}
		
		/*RFs fs1;	
		TRAPD(err,fs1.Connect());

		_LIT(filen3, "c:\\system\\apps\\ripplevault\\config.txt");

		if(!(BaflUtils::FileExists(fs1, filen3)))
		{
			done = true;
		}

		fs1.Close();*/

	}
	//console->Getch(); // get and ignore character
	//CleanupStack::PopAndDestroy(5); //console, ws, ls, timer, cmdline
	CleanupStack::PopAndDestroy(4); // ws, ls, timer, cmdline
}

_LIT(KTxtEPOC32EX,"Whoops!");

#ifdef __WINS__

EXPORT_C TInt InitEmulator() // mainloop function called by the emulator software
{
	__UHEAP_MARK;
	CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
	TRAPD(error,mainloop()); // more initialization, then do example
	__ASSERT_ALWAYS(!error,User::Panic(KTxtEPOC32EX,error));
	delete cleanup; // destroy clean-up stack
	__UHEAP_MARKEND;
	
	//CloseSTDLIB();
	User::Exit(0);
	
	return KErrNone;
}

int GLDEF_C E32Dll(TDllReason)
{
	return(KErrNone);
}

#else

GLDEF_C TInt E32Main() // mainloop function called by E32
{
	__UHEAP_MARK;
	CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
	TRAPD(error,mainloop()); // more initialization, then do example
	__ASSERT_ALWAYS(!error,User::Panic(KTxtEPOC32EX,error));
	delete cleanup; // destroy clean-up stack
	__UHEAP_MARKEND;
	
	//CloseSTDLIB();
	User::Exit(0);
	
	return KErrNone; // and return
}

#endif

