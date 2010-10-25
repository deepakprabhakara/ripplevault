
#include "imsi.h"


CIMSI::CIMSI() : CActive(EPriorityStandard)
{
	iRetrieved = EFalse;
}

CIMSI* CIMSI::NewL()
{
	CIMSI* self =new(ELeave) CIMSI();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void CIMSI::ConstructL()
{
	iPkg = new (ELeave) CTelephony::TSubscriberIdV1Pckg(iV1);
	iTelephony = CTelephony::NewL();
	CActiveScheduler::Add(this);
}

CIMSI::~CIMSI()
{
	Cancel(); // if any request outstanding, calls DoCancel() to cleanup
	delete iTelephony;
	delete iPkg;
}

void CIMSI::GetIMSI()
{
	iTelephony->GetSubscriberId( iStatus, *iPkg );
	SetActive();
	CActiveScheduler::Start();
}

void CIMSI::RunL()
{
	if ( (iStatus == KErrNone) )
	{
		retrievedIMSI = (*iPkg)().iSubscriberId; // Read IMSI from package buffer
	}
	CActiveScheduler::Stop();

	iRetrieved = ETrue;
}

void CIMSI::DoCancel()
{
	Cancel();
} 