//=============================================
// Filename: Phoneutils.h
//=============================================
#ifndef __GETIMSI_S80_H__
#define __GETIMSI_S80_H__

#include <etel3rdparty.h>

/* Declare the client class */
class CIMSI : public CActive
{
public:
CIMSI();

// Construction
void ConstructL();

// Destruction
~CIMSI();

static CIMSI* NewL();

// Issue request: retrieve IMSI
void GetIMSI();

// Cancel request
void DoCancel();

// Service completed request
void RunL();

TBool IsRetrieved() { return iRetrieved;}

private:
CTelephony* iTelephony; // telephony object we own
CTelephony::TSubscriberIdV1 iV1;
CTelephony::TSubscriberIdV1Pckg* iPkg;
TBool iRetrieved;

public:
TBuf<20> retrievedIMSI;
};

#endif
