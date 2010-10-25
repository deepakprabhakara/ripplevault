
#ifndef __UPDATECONTACTSAO_H__
#define __UPDATECONTACTSAO_H__

#include <e32std.h>
#include <cntdb.h>
#include <cntfield.h>
#include <cntitem.h>
#include <cntfldst.h>

class CRippleVaultAppUi;
class CContactsSocketsEngineWriter;
class CRecordContactDB;
class CList;

class CUpdateContactsAO : public CActive
	{
public:

	CUpdateContactsAO(CRippleVaultAppUi& aAppUi, CContactsSocketsEngineWriter& aSocketsEngine);
	~CUpdateContactsAO();
	void StartL(CList *list);

	CRippleVaultAppUi& iAppUi;
	CContactsSocketsEngineWriter& iSocketsEngine;

private: // Inherited from CActive.
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

private:
	void Notify();

	void AddFirstName(CRecordContactDB *record, CContactItem* contact);
	void AddLastName(CRecordContactDB *record, CContactItem* contact);
	void AddCompanyName(CRecordContactDB *record, CContactItem* contact);
	void AddJobTitles(CRecordContactDB *record, CContactItem* contact);

	void AddMobiles(CRecordContactDB *record, CContactItem* contact);
	void AddHomeMobiles(CRecordContactDB *record, CContactItem* contact);
	void AddWorkMobiles(CRecordContactDB *record, CContactItem* contact);

	void AddTelephones(CRecordContactDB *record, CContactItem* contact);
	void AddHomeTelephones(CRecordContactDB *record, CContactItem* contact);
	void AddWorkTelephones(CRecordContactDB *record, CContactItem* contact);

	void AddFaxes(CRecordContactDB *record, CContactItem* contact);
	void AddHomeFaxes(CRecordContactDB *record, CContactItem* contact);
	void AddWorkFaxes(CRecordContactDB *record, CContactItem* contact);

	void AddEmails(CRecordContactDB *record, CContactItem* contact);
	void AddHomeEmails(CRecordContactDB *record, CContactItem* contact);
	void AddWorkEmails(CRecordContactDB *record, CContactItem* contact);

	void UpdateJobTitle(CRecordContactDB *record, CContactItem* contact);
	void UpdateEmail(CRecordContactDB *record, CContactItem* contact);
	void UpdateFax(CRecordContactDB *record, CContactItem* contact);

	void UpdateTimeStamp();
private:
	void UpdateContactL();
	void EndAcks();

	CList *iPtr;
	CList *iRecordList;

	//CContactDatabase* iDatabase;
	//const CContactIdArray* iContacts;

	TInt iContactIndex;
	TBuf8<100> iSessionId;

	
	};

#endif // __UPDATECONTACTSAO_H__

// End of file
