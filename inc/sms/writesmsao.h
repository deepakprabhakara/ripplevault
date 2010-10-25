
#ifndef __WRITESMSAO_H__
#define __WRITESMSAO_H__

#include <e32std.h>
#include <badesca.h>
//#include <aknwaitdialog.h> 
#include "ripplevaultappui.h"
//#include "ManualSmsListContainer.h"

//class CManualSmsListContainer;
class CRippleVaultAppui;

class CWriteSmsAO : public CActive
	{
public:
	CWriteSmsAO(CRippleVaultAppUi& aAppUi);
	~CWriteSmsAO();
	void StartL(TInt aInboxCount, TInt aSentItemCount, CArrayFix<TInt>* aInboxSelectedSms, CArrayFix<TInt>* aSentItemsSelectedSms, CDesCArray* aInboxSmsDatearray, CDesCArray* aSentSmsDatearray, CDesCArray* aInboxSmsDetailsarray, CDesCArray* aSentSmsDetailsarray, CDesCArray* aInboxSmsMessagearray, CDesCArray* aSentSmsMessagearray);

	//CManualSmsListContainer& iAppView;
	CRippleVaultAppUi& iAppUi;

private: // Inherited from CActive.
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

public:
	
private:
	void Notify();
	
private:
	TBuf<120> iFileName;
	RFile iFile;
	RFs fs;	

	//CAknWaitDialog* iWaitDialog;
	void WriteSmsToFile(TInt aId,TInt aSmsState);

		TInt iInboxCount;
		TInt iSentItemCount;

		CArrayFix<TInt>* iInboxSelectedSms;		
		CArrayFix<TInt>* iSentItemsSelectedSms;

		CDesCArray *InboxSmsDatearray;
		CDesCArray *InboxSmsMessagearray;
		CDesCArray *InboxSmsDetailsarray;
		
		CDesCArray *SentSmsDatearray;
		CDesCArray *SentSmsMessagearray;
		CDesCArray *SentSmsDetailsarray;

};

#endif // __WriteSmsAO_H__

// End of file
