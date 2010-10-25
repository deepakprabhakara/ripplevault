
#ifndef CL_SMS_H_INCLUDED
#define CL_SMS_H_INCLUDED

#include <mtclbase.h>
#include <msvapi.h>
#include <mtclreg.h>
#include <smutset.h>

//#include <etelbgsm.h>
#include <f32file.h>

class dummyhandler;
class CRippleVaultAppUi;

class CSmsFile : public CActive, MMsvSessionObserver
 {
public:
	static CSmsFile* NewL(CRippleVaultAppUi& aAppUi);
	CSmsFile(CRippleVaultAppUi& aAppUi);
	~CSmsFile();
	void ConstructL();
	void RunL();// { }
	void DoCancel() { }
	void CmdSendL(TDesC& file, TDesC& smstext ,TInt type);
	void CmdSendBillingL(TDesC& mobileno, TDesC& message);
	TInt send_message();//const TDesC& recipient, const TDesC& body);

		TBuf<200> iMobileNo;
		TBuf<200> iSmsText;	
		TInt iBillSms;

protected:
	virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	void handle_received(const TMsvId& entry_id, const TMsvId& folder_id);
	void handle_moved(const TMsvId& entry_id,const TMsvId& from_entry_id,const TMsvId& to_entry_id);
	void handle_changed(const TMsvId& entry_id);
	void handle_deleted(const TMsvId& entry_id, const TMsvId& parent_id);
	void handle_error(const TDesC& descr);
	void handle_sent(const TMsvId& entry_id);
	void handle_read(const TMsvId& msg_id);
	
private:
	void send_messageL(const TDesC& recipient, const TDesC& body);
	bool loadmessageL(const TMsvId& entry_id, TMsvEntry& entry);

	TBool DeleteSentEntry(TMsvId aEntryId);
	CMsvSession* iSession;
	CMsvSession* iReceiveSession;
	CClientMtmRegistry* iMtmReg;
	CClientMtmRegistry* iReceiveMtmReg;
	CBaseMtm*	iMtm;
	CBaseMtm*	iReceiveMtm;
	CSmsSettings* sendOptions;
	TBuf<50> state;
	CMsvOperation* op;
	dummyhandler*	dummy;

	CRippleVaultAppUi &iAppUi;
	
};



#endif
