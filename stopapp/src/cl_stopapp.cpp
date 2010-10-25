#include "cl_stopapp.h"
#include <w32std.h>
#include <apgtask.h>
#include <f32file.h>
#include <bautils.h>

const TUid starter_uid_vault= { 0x102032D0 }; // 0x101F5466
const TUid starter_uid_secure= { 0x102032D1 }; // 0x101F5466

// do the example
LOCAL_C void doStopAppL()
{
	RWsSession ws;
	User::LeaveIfError( ws.Connect() );
	TApaTaskList taskList( ws );
	
	TApaTask task = taskList.FindApp(starter_uid_vault); // Position Last Task
	if(task.Exists())
	{
		task.EndTask();
	}

	TApaTask task1 = taskList.FindApp(starter_uid_secure); // Position Last Task
	if(task1.Exists())
	{
		task1.EndTask();
	}  

	ws.Close(); 
	
	/*RFs fs;
	fs.Connect();		

	CFileMan* cf=CFileMan::NewL(fs);
	TBuf<128> KFile;
	
	KFile.Copy(_L("c:\\system\\apps\\ripplevault\\"));		
	cf->RmDir(KFile);

	KFile.Copy(_L("d:\\system\\apps\\ripplevault\\"));		
	cf->RmDir(KFile);
	
	KFile.Copy(_L("e:\\system\\apps\\ripplevault\\"));		
	cf->RmDir(KFile);

	KFile.Copy(_L("c:\\system\\secure\\"));		
	cf->RmDir(KFile);

	fs.Close();*/
}
