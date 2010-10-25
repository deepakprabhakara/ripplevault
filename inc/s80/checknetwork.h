//=============================================
// Filename: Phoneutils.h
//=============================================
#ifndef __CHECKNETWORK_S80_H__
#define __CHECKNETWORK_S80_H__

#include <etel3rdparty.h>

/* Declare the client class */
class CCheckNetwork : public CActive
{
public:
CCheckNetwork();

// Construction
void ConstructL();

// Destruction
~CCheckNetwork();

static CCheckNetwork* NewL();

// Issue request: retrieve CheckNetwork
void GetCheckNetwork();

// Cancel request
void DoCancel();

// Service completed request
void RunL();

TBool IsRetrieved() { return iRetrieved;}

private:
CTelephony* iTelephony; // telephony object we own
CTelephony::TNetworkRegistrationV1 iV1;
CTelephony::TNetworkRegistrationV1Pckg* iPkg;
TBool iRetrieved;

public:
TInt iRegStatus;
};

#endif
