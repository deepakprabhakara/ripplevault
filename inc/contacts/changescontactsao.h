
#ifndef __CHANGESCONTACTSAO_H__
#define __CHANGESCONTACTSAO_H__

#include <e32std.h>
#include <cntdb.h>
#include <cntfield.h>
#include <cntitem.h>
#include <cntfldst.h>

class CList;

class MUINotifier;
class CRippleVaultAppUi;

class CChangesContactsAO : public CActive
	{
public:
//TBuf8<80000> iBuffer;

	CRippleVaultAppUi & iAppUi;

	CChangesContactsAO(CRippleVaultAppUi& aAppUi);
	~CChangesContactsAO();
	void StartL(TTime aCDBTime, TTime aRippleTime);

private: // Inherited from CActive.
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

private:

	enum TChangesAOState 
	{
	    EAddUpdate,
        EDelete
	};

    void ChangeStatus(TChangesAOState aNewStatus);

	void CompareContactL(TInt aIndex);

	CContactDatabase* iDatabase;
	const CContactIdArray* iContacts;

	//Add telephone numbers, mobile numbers etc to buffer
	//void AddToBuffer(TBuf8<80000>* buffer, CList *items, TPtrC type);

	TChangesAOState iEngineStatus;

	TInt iDeletedIndex;

	CList *iDeletedIds;
	CList *iDeletedPtr;

	TInt iContactIndex;

	CList *iIdList;
	TTime iRippleTime;
	TTime iCDBTime;
	
	};

#endif

// End of file
