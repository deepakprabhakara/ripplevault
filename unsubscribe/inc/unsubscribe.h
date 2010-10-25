#ifndef __UNSUBSCRIBE_H__
#define __UNSUBSCRIBE_H__

#include <e32base.h>
//#include <e32cons.h>

_LIT(KTxtEPOC32EX,"UNSUBSCRIBE");
//_LIT(KTxtExampleCode,"Symbian OS Example Code");
//_LIT(KFormatFailed,"failed: leave code=%d");
//_LIT(KTxtOK,"ok");
//_LIT(KTxtPressAnyKey," [press any key]");

LOCAL_C void doStopAppL(); // code this function for the real example

// private
LOCAL_C void callExampleL(); // initialize with cleanup stack, then do example

GLDEF_C TInt E32Main() // main function called by E32
    {
	__UHEAP_MARK;
	CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
	TRAPD(error,callExampleL()); // more initialization, then do example
	__ASSERT_ALWAYS(!error,User::Panic(KTxtEPOC32EX,error));
	delete cleanup; // destroy clean-up stack
	__UHEAP_MARKEND;
	return 0; // and return
    }

LOCAL_C void callExampleL() // initialize and call example code under cleanup stack
    {
	TRAPD(error,doStopAppL()); // perform example function
    }

#endif