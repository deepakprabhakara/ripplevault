
#ifndef __COPYCONTACTSAO_H__
#define __COPYCONTACTSAO_H__

#include <e32std.h>
#include <cntdb.h>
#include <cntfield.h>
#include <cntitem.h>
#include <cntfldst.h>

class CList;

class MUINotifier;
class CRippleVaultAppUi;

class CCopyContactsAO : public CActive
	{
public:
//TBuf8<80000> iBuffer;

	CRippleVaultAppUi & iAppUi;

	CCopyContactsAO(CRippleVaultAppUi& aAppUi);
	~CCopyContactsAO();
	void StartL();

private: // Inherited from CActive.
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

private:
	void CopyContactL(TInt aIndex);
	void UpdateTimeStamp();

	CContactDatabase* iDatabase;
	const CContactIdArray* iContacts;

	//Add telephone numbers, mobile numbers etc to buffer
	//void AddToBuffer(TBuf8<80000>* buffer, CList *items, TPtrC type);

	TInt iContactIndex;
	
	};

#endif

// End of file
