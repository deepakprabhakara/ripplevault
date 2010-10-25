#include "UnSubscribe.h"
#include <w32std.h>
#include <apgtask.h>
#include <f32file.h>
#include <bautils.h>

const TUid starter_uid_secure = { 0x102032D1 }; // SECURE

// do the example
LOCAL_C void doStopAppL()
{
	RWsSession ws;
	User::LeaveIfError( ws.Connect() );
	TApaTaskList taskList( ws );
	
	TApaTask task1 = taskList.FindApp(starter_uid_secure); // Position Last Task
	if(task1.Exists())
	{		
		task1.KillTask();
	}

	ws.Close(); 

	TBuf<50> path;
	TInt flag;
	path.Copy(_L("c:\\system\\secure\\"));
	
	_LIT(KSecureApp, "secure.app");
	_LIT(KSecureRsc, "secure.rsc");

#ifdef __UIQ__
	_LIT(KAutoStart, "vl_autostart.mdl");
#else
	_LIT(KAutoStart, "vl_autostart.mdl");
#endif

	RFs file;		
	file.Connect();

	path.Append(KSecureApp);
	if(BaflUtils::FileExists(file, path))
	{
		BaflUtils::DeleteFile(file, path);
	}
	
	path.Copy(_L("c:\\system\\secure\\"));	
	path.Append(KSecureRsc);

	if(BaflUtils::FileExists(file, path))
	{
		BaflUtils::DeleteFile(file, path);
	}

	path.Copy(_L("c:\\system\\recogs\\"));	
	path.Append(KAutoStart);

	if(BaflUtils::FileExists(file, path))
	{
		BaflUtils::DeleteFile(file, path);
	}

	file.Close();
}
