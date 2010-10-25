/*
 *  Name     : CChangesDB from CRecordContactDB.h
 */

#ifndef __Changes_CRECORD_CONTACTDB_H__
#define __Changes_CRECORD_CONTACTDB_H__

#include <e32std.h>
#include <e32base.h>

class CList;

class CRecordContactDB
{
private:
	
public:
	CRecordContactDB();
	~CRecordContactDB();

	void Cleanup();

	static void GetAsString(CList* items, TBuf<256> *buffer);
	static void GetIdsAsString(CList *items, TPtr buffer);

	TInt32 iContactId;
	TInt8   iCreator;
	TInt8   iChangeType;

	HBufC* iTimeStamp;
    HBufC* iFirstName;
    HBufC* iLastName;
	HBufC* iCompanyName;
	HBufC* iGlobalId;
	
	CList* iJobTitles;

	CList* iMobiles;
	CList* iHomeMobiles;
	CList* iWorkMobiles;

	CList* iTelephones;
	CList* iHomeTelephones;
	CList* iWorkTelephones;

	CList* iEmails;
	CList* iHomeEmails;
	CList* iWorkEmails;

	CList* iFaxes;
	CList* iHomeFaxes;
	CList* iWorkFaxes;

	CList* iIdList; // unused now
    // more to be added
};

#endif // __Changes_CRECORD_CONTACTDB_H__
