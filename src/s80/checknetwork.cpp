
#include "CheckNetwork.h"


CCheckNetwork::CCheckNetwork() : CActive(EPriorityStandard)
{
	iRetrieved = EFalse;
}

CCheckNetwork* CCheckNetwork::NewL()
{
	CCheckNetwork* self =new(ELeave) CCheckNetwork();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void CCheckNetwork::ConstructL()
{
	iPkg = new (ELeave) CTelephony::TNetworkRegistrationV1Pckg(iV1);
	iTelephony = CTelephony::NewL();
	CActiveScheduler::Add(this);
}

CCheckNetwork::~CCheckNetwork()
{
	Cancel(); // if any request outstanding, calls DoCancel() to cleanup
	delete iTelephony;
	delete iPkg;
}

void CCheckNetwork::GetCheckNetwork()
{
	iTelephony->GetNetworkRegistrationStatus( iStatus, *iPkg );
	SetActive();
	CActiveScheduler::Start();
}

void CCheckNetwork::RunL()
{
	if ( (iStatus == KErrNone) )
	{
		iRegStatus = (*iPkg)().iRegStatus; // Read CheckNetwork from package buffer
	}
	CActiveScheduler::Stop();

	iRetrieved = ETrue;
}

void CCheckNetwork::DoCancel()
{
	Cancel();
} 