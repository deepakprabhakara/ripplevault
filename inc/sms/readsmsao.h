
#ifndef __READSMSAO_H__
#define __READSMSAO_H__

#include <e32std.h>
#include <msvapi.h> 
#include <mtclreg.h> 
#include <msvids.h> 
#include <msvapi.h> 
#include <msvuids.h> 

#include <mtclbase.h>
#include <txtrich.h>
#include <smsclnt.h>
#include <smuthdr.h>
#include <smutset.h>
#include <smscmds.h>

#include <badesca.h>

class CMsvSession;
class MMsvSessionObserver;

#ifdef __UIQ__
#ifdef __S80__
class CMessageTextListBox;
#else
class CSmsScreenView;
#endif
#else
class CManualSmsListContainer;
#endif

class CReadSmsAO : public CActive, public MMsvSessionObserver
	{
public:

#ifdef __UIQ__
#ifdef __S80__
	CReadSmsAO(CMessageTextListBox& aAppView);
#else
	CReadSmsAO(CSmsScreenView& aAppView);
#endif
#else
	CReadSmsAO(CManualSmsListContainer& aAppView);
#endif
	~CReadSmsAO();
	void StartL();

#ifdef __UIQ__
#ifdef __S80__
	CMessageTextListBox& iAppView;
#else
	CSmsScreenView& iAppView;
#endif
#else
	CManualSmsListContainer& iAppView;
#endif

private: // Inherited from CActive.
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

public:

	void PrepareMtm(TInt aSmsType);
	void ReleaseMtm();

	TInt iInboxCount;
	TInt iSentItemCount;
	TInt iInboxFlag;
	TInt iSentItemFlag;

	TInt iInboxIndex;
	TInt iSentItemsIndex;

	TInt iSmsType;

private:
	void Notify();
	
private:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);

	CMsvSession* iSession;

	CClientMtmRegistry* iMtmReg;

	CMsvEntry* iParentEntry;

	CMsvEntrySelection* iEntries;

	TInt ReadSms();
};

#endif // __READSMSAO_H__

// End of file
