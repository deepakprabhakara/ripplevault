
#ifndef __READCONTACTSAO_H__
#define __READCONTACTSAO_H__

#include <e32std.h>
#include <cntdb.h>
#include <cntfield.h>
#include <cntitem.h>
#include <cntfldst.h>

class CFindBoxContainer;

class CReadContactsAO : public CActive
	{
public:

	CReadContactsAO(CFindBoxContainer& aAppView);
	~CReadContactsAO();
	void StartL();

	CFindBoxContainer& iAppView;
private: // Inherited from CActive.
	void RunL();
	TInt iFlag;
	void DoCancel();
	TInt RunError(TInt aError);

	TInt iCount;

private:
	void Notify();

private:
	void ReadContactL();

    void GetTextFieldFromFieldSet(CContactItemFieldSet& aFieldSet, const TUid aField,TDesC& name);

	CContactDatabase* iPhoneContacts;

	CContactIdArray* cArray;
	};

#endif // __UPDATECONTACTSAO_H__

// End of file
