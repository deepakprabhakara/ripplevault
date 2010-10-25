#if !defined(__CL_AUTOSTART_H__)
#define __CL_AUTOSTART_H__

#if !defined(__APMREC_H__)
#include <apmrec.h>
#endif

#include <apgcli.h>
#include <f32file.h> 
#include <apacmdln.h>
#include <e32std.h>
#include <apmstd.h>


class CclAutostart : public CApaDataRecognizerType
{
public: // from CApaDataRecognizerType
	CclAutostart();
	TUint PreferredBufSize();
	TDataType SupportedDataTypeL(TInt aIndex) const;
	static void StartThread();
	static TInt StartAppThreadFunction(TAny* aParam);
	static void StartAppThreadFunctionL();
	
	static void StartRippleVault();
	static void StartRippleVaultL();

private: // from CApaDataRecognizerType
	void DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer);
};

#endif

