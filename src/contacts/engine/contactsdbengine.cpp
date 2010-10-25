/*******************************************************************

 Copyright(c) 2000-2005 

 PurpleACE Wireless Solutions Pvt. Ltd.
 www.purpleace.com
 #708, A Wing, Carlton Towers, Airport Road, Bangalore - 560017,
 India
 All rights reserved.

 This software is the confidential and proprietary information of
 PurpleACE Wireless Solutions Pvt. Ltd. ("Confidential Information").
 You shall not disclose such Confidential Information and
 shall use it only in accordance with the terms of the license
 Agreement you entered into with PurpleACE Wireless Solutions Pvt. Ltd..

 Project Name               : Ripple Vault Symbian Client
 Module Name				: ContactsDBEngine.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Contacts Database engine
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/


#include <badesca.h>    // CDesCArrayFlat
#include <s32file.h>    // CFileStore & CPermanentFileStore
#include <bautils.h>    // file helpers
#include <eikenv.h>
#include <bautils.h>
#include <in_sock.h>
//#include <cntfldst.h>
#include "ContactsDBEngine.h"
#include "RecordContactDB.h"
#include "List.h"
#include "RippleVaultAppUi.h"

const int KCustomSqlMaxLength = 1024;
const int KMaxColLength = 200;

// Table Structure for ChangesDB
_LIT(KChangesTbl, "Changes"); // table name
_LIT(KIdsTbl, "IdList");

_LIT(KIdCol, "ContactId");
_LIT(KIdCreator, "Creator"); // Local or Server?
_LIT(KIdChangeType, "ChangeType"); //add, update, delete, addupdate
_LIT(KTimeStampCol, "TimeStamp");

_LIT(KFirstNameCol, "FirstName");
_LIT(KSecondNameCol, "SecondName");
_LIT(KCompanyCol, "Company");
_LIT(KJobTitleCol, "JobTitle");

_LIT(KFaxesCol, "Faxes");
_LIT(KFaxesHomeCol, "FaxesHome");
_LIT(KFaxesOfficeCol, "FaxesOffice");

_LIT(KEmailsCol, "Emails");
_LIT(KEmailsHomeCol, "EmailsHome");
_LIT(KEmailsOfficeCol, "EmailsOffice");

_LIT(KMobilesCol, "Mobiles");
_LIT(KMobilesHomeCol, "MobileHome");
_LIT(KMobilesOfficeCol, "MobileOffice");

_LIT(KTelephonesCol, "Telephones");
_LIT(KTelephonesHomeCol, "TelephonesHome");
_LIT(KTelephonesOfficeCol, "TelephonesOffice");

_LIT(KGlobalIdCol, "GlobalId");

//_LIT(KIdListCol, "IdList");

_LIT(KMsgTypeCol, "MessageType"); // Type of transaction - sync, reg, eu
_LIT(KCurrTimeCol, "CurrTime");
_LIT(KSyncTimeCol, "SyncTime");


/*void Logger(TDes8& buf)
{
	RFs fs;
	RFile file;

	fs.Connect();

	_LIT(KLogFile, "log.txt");

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KLogFile);

	TInt err;
	if (BaflUtils::FileExists(fs, path)) {
		err = file.Open(fs, path, EFileWrite);
		if (err == KErrNone) {
			TInt size;
			file.Size(size);
			file.Seek(ESeekStart, size);
			file.Write(buf);
			file.Flush();
			file.Close();
		}
	}
	else {
		err = file.Replace(fs, path, EFileWrite);
		if (err == KErrNone) {
			file.Write(buf);
			file.Flush();
			file.Close();
		}
	}

	fs.Close();
}*/

// ---------------------------------------------------------------------------
// CChangesDB::NewL()
//
// Create instance of the ChangesDB database engine.
// ---------------------------------------------------------------------------
CChangesDB* CChangesDB::NewL()
{
    CChangesDB* tmp = new (ELeave)CChangesDB();
    CleanupStack::PushL(tmp);
    tmp->ConstructL();
    CleanupStack::Pop();
    return tmp;
}

// ---------------------------------------------------------------------------
// CChangesDB::~CChangesDB()
//
// Destructor of the ChangesDB database engine. Release resources.
// ---------------------------------------------------------------------------
CChangesDB::~CChangesDB()
{
    Close();  // Just in case, if the user does not close this explicitely
    iFsSession.Close();
}

// ---------------------------------------------------------------------------
// CChangesDB::ConstructL()
//
// Second phase construction. Leaves, if RFs session cannot be created.
// ---------------------------------------------------------------------------
void CChangesDB::ConstructL()
{
    TInt err = iFsSession.Connect();
    if(err)
        User::Leave(err);
}

// ---------------------------------------------------------------------------
// CChangesDB::CChangesDB()
//
// Constructor
// ---------------------------------------------------------------------------
CChangesDB::CChangesDB()
{
    iOpen = EFalse;
}

// ---------------------------------------------------------------------------
// CChangesDB::OpenDbL()
//
// Open existing ChangesDB database for exclusive access.
// ---------------------------------------------------------------------------
TInt CChangesDB::OpenDbL(const TFileName& aExistingChangesDBFile)
{
    Close();

    if(!BaflUtils::FileExists(iFsSession, aExistingChangesDBFile))
    {
        return KErrNotFound;
    }

    iFileStore = CPermanentFileStore::OpenL(iFsSession,
        aExistingChangesDBFile, EFileRead|EFileWrite);
    iFileStore->SetTypeL(iFileStore->Layout());       // Set file store type
    iChangesDB.OpenL(iFileStore,iFileStore->Root());

	TRAPD(g, CreateSettingsTableExtendedL());

    iOpen = ETrue;
    return KErrNone;
}

// ---------------------------------------------------------------------------
// CChangesDB::CreateDbL()
//
// Create a new database. The database will be in exclusive access mode.
// ---------------------------------------------------------------------------
TInt CChangesDB::CreateDbL(const TFileName& aNewChangesDBFile)
{
    Close();

	if (BaflUtils::FileExists(iFsSession, aNewChangesDBFile)) {
		return 1;
	}

    // Create empty database file.
    iFileStore = CPermanentFileStore::CreateL(iFsSession,
        aNewChangesDBFile, EFileRead|EFileWrite);
    iFileStore->SetTypeL(iFileStore->Layout());       // Set file store type
    TStreamId id = iChangesDB.CreateL(iFileStore);  // Create stream object
    iFileStore->SetRootL(id);            // Keep database id as root of store
    iFileStore->CommitL();               // Complete creation by commiting

    // Create ChangesDB tables and indexes
	CreateChangesTableL();
	CreateSettingsTableL();
	CreateSettingsTableExtendedL();
	CreateIdsTableL();
	CreateGlobalIdsTableL();
	CreateUploadTableL();
    iOpen = ETrue;

    return KErrNone;
}

// ---------------------------------------------------------------------------
// CChangesDB::RemoveDbL()
//
// First remove the Books table. Then remove the database file.
// ---------------------------------------------------------------------------
TInt CChangesDB::RemoveDbL(const TFileName& aExistingChangesDBFile)
{
    Close();

    if(!BaflUtils::FileExists(iFsSession, aExistingChangesDBFile))
    {
        return KErrNotFound;
    }

    OpenDbL(aExistingChangesDBFile);
    DropChangesTableL();
    Close();

    iFsSession.Delete(aExistingChangesDBFile);
    return KErrNone;
}

// ---------------------------------------------------------------------------
// CChangesDB::Close()
//
// Close the database.
// ---------------------------------------------------------------------------
TInt CChangesDB::Close()
{
    iChangesDB.Close();
    if(iFileStore)
    {
        delete iFileStore;
        iFileStore = NULL;
    }
    iOpen = EFalse;
    return KErrNone;
}

// ---------------------------------------------------------------------------
// CChangesDB::IsOpen()
//
// Return open status of the database.
// ---------------------------------------------------------------------------
TBool CChangesDB::IsOpen() const
{
    return iOpen;
}

/**
 * TODO: This implementation is just sample code. It needs to extend to
 * create proper table structure with all columns
 */
void CChangesDB::CreateChangesTableL()
{
//    TInt ret;
    // Specify columns for Books table
    TDbCol contactId(KIdCol, EDbColInt32);
    TDbCol creator(KIdCreator, EDbColInt8);
    TDbCol changetype(KIdChangeType, EDbColInt8);
	TDbCol timestamp(KTimeStampCol, EDbColText);
    TDbCol firstname(KFirstNameCol, EDbColText, KMaxColLength);
    TDbCol secondname(KSecondNameCol, EDbColText, KMaxColLength);
    TDbCol company(KCompanyCol, EDbColText, KMaxColLength);
	TDbCol jobtitle(KJobTitleCol, EDbColText, KMaxColLength);

    TDbCol telephones(KTelephonesCol, EDbColText, KMaxColLength);
    TDbCol telephoneshome(KTelephonesHomeCol, EDbColText, KMaxColLength);
    TDbCol telephonesoffice(KTelephonesOfficeCol, EDbColText, KMaxColLength);
    
    TDbCol mobiles(KMobilesCol, EDbColText, KMaxColLength);
    TDbCol mobileshome(KMobilesHomeCol, EDbColText, KMaxColLength);
    TDbCol mobilesoffice(KMobilesOfficeCol, EDbColText, KMaxColLength);

	TDbCol faxes(KFaxesCol, EDbColText, KMaxColLength);
    TDbCol faxeshome(KFaxesHomeCol, EDbColText, KMaxColLength);
    TDbCol faxesoffice(KFaxesOfficeCol, EDbColText, KMaxColLength);

	TDbCol emails(KEmailsCol, EDbColText, KMaxColLength);
    TDbCol emailshome(KEmailsHomeCol, EDbColText, KMaxColLength);
    TDbCol emailsoffice(KEmailsOfficeCol, EDbColText, KMaxColLength);

	TDbCol globalId(KGlobalIdCol, EDbColText, KMaxColLength);

	//TDbCol idlist(KIdListCol, EDbColLongText);

    // Add the columns to column set
    CDbColSet* colset = CDbColSet::NewLC();
    colset->AddL(contactId);
    colset->AddL(creator);
    colset->AddL(changetype);
	colset->AddL(timestamp);
    colset->AddL(firstname);
    colset->AddL(secondname);
    colset->AddL(company);
	colset->AddL(jobtitle);
	colset->AddL(faxeshome);
    colset->AddL(faxesoffice);
	colset->AddL(faxes);
    colset->AddL(mobileshome);
    colset->AddL(mobilesoffice);
	colset->AddL(mobiles);
    colset->AddL(telephoneshome);
    colset->AddL(telephonesoffice);
	colset->AddL(telephones);
	colset->AddL(emailshome);
    colset->AddL(emailsoffice);
	colset->AddL(emails);
	colset->AddL(globalId);
	//colset->AddL(idlist);

    // Create the Books table
    User::LeaveIfError(iChangesDB.CreateTable(KChangesTbl, *colset));
    CleanupStack::PopAndDestroy(colset);

	/*TDbCol msgtype(KMsgTypeCol, EDbColText);
	CDbColSet* cs = CDbColSet::NewLC();
	cs->AddL(msgtype);

	// Create Stats table
	User::LeaveIfError(iChangesDB.CreateTable(KStatsTbl, *cs));
    CleanupStack::PopAndDestroy(colset);
	*/
}

void CChangesDB::DropChangesTableL()
{
    _LIT(KDropTable, "DROP TABLE ");

    // Sql: DROP TABLE Books
    TBuf<KCustomSqlMaxLength> sqlStr;
    sqlStr.Append(KDropTable);
    sqlStr.Append(KChangesTbl);

    RDbIncremental incOp;
    TInt incStep = 0xFFFF;
    // Initialise Execution
    TInt incStat = incOp.Execute(iChangesDB, sqlStr, incStep);
    while (incStep>0 && incStat==KErrNone)
	{
        incStat = incOp.Next(incStep); // Do the work
    }
    incOp.Close();
}

/**
 * TODO: This implementation is just sample code. It needs to extend to
 * create proper table entry
 */
TInt CChangesDB::AddRecordFromContactsDB(CRecordContactDB* aRecordContactDB)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KChangesTbl, table.EUpdatable);
    //User::LeaveIfError(err);

	if (err != KErrNone)
	{
		return err;
	}

	TBuf<256> buffer;
	buffer.Copy(_L(""));

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.Reset();
    table.InsertL();

	/*TBuf8<30> buf;
	buf.Copy(_L("Adding: "));
	buf.AppendNum((TInt)aRecordContactDB->iContactId);
	buf.Append(_L("\r\n"));
	Logger(buf);*/

	// Contact Id
	table.SetColL(colset->ColNo(KIdCol), aRecordContactDB->iContactId);
	// Creator
	table.SetColL(colset->ColNo(KIdCreator), aRecordContactDB->iCreator);
	// Change Type
	table.SetColL(colset->ColNo(KIdChangeType), aRecordContactDB->iChangeType);

	// TimeStamp
	table.SetColL(colset->ColNo(KTimeStampCol), *(aRecordContactDB->iTimeStamp));

	// First Name
	if (aRecordContactDB->iFirstName)
		table.SetColL(colset->ColNo(KFirstNameCol), *(aRecordContactDB->iFirstName));
	
	// Last Name
	if (aRecordContactDB->iLastName)
		table.SetColL(colset->ColNo(KSecondNameCol), *(aRecordContactDB->iLastName));

	// Company Name
	if (aRecordContactDB->iCompanyName)
		table.SetColL(colset->ColNo(KCompanyCol), *(aRecordContactDB->iCompanyName));

	// Job Titles
	aRecordContactDB->GetAsString(aRecordContactDB->iJobTitles, &buffer);
	table.SetColL(colset->ColNo(KJobTitleCol), buffer);

	// Home Faxes
	aRecordContactDB->GetAsString(aRecordContactDB->iHomeFaxes, &buffer);
	table.SetColL(colset->ColNo(KFaxesHomeCol), buffer);
	// Work Faxes
	aRecordContactDB->GetAsString(aRecordContactDB->iWorkFaxes, &buffer);
	table.SetColL(colset->ColNo(KFaxesOfficeCol), buffer);
	// Faxes
	aRecordContactDB->GetAsString(aRecordContactDB->iFaxes, &buffer);
	table.SetColL(colset->ColNo(KFaxesCol), buffer);

	// Home Mobiles
	aRecordContactDB->GetAsString(aRecordContactDB->iHomeMobiles, &buffer);
	table.SetColL(colset->ColNo(KMobilesHomeCol), buffer);
	// Work Mobiles
	aRecordContactDB->GetAsString(aRecordContactDB->iWorkMobiles, &buffer);
	table.SetColL(colset->ColNo(KMobilesOfficeCol), buffer);
	// Mobiles
	aRecordContactDB->GetAsString(aRecordContactDB->iMobiles, &buffer);
	table.SetColL(colset->ColNo(KMobilesCol), buffer);

	// Home Telephones
	aRecordContactDB->GetAsString(aRecordContactDB->iHomeTelephones, &buffer);
	table.SetColL(colset->ColNo(KTelephonesHomeCol), buffer);
	// Work Telephones
	aRecordContactDB->GetAsString(aRecordContactDB->iWorkTelephones, &buffer);
	table.SetColL(colset->ColNo(KTelephonesOfficeCol), buffer);
	// Telephones
	aRecordContactDB->GetAsString(aRecordContactDB->iTelephones, &buffer);
	table.SetColL(colset->ColNo(KTelephonesCol), buffer);
	
	// Home Emails
	aRecordContactDB->GetAsString(aRecordContactDB->iHomeEmails, &buffer);
	table.SetColL(colset->ColNo(KEmailsHomeCol), buffer);
	// Work Mobiles
	aRecordContactDB->GetAsString(aRecordContactDB->iWorkEmails, &buffer);
	table.SetColL(colset->ColNo(KEmailsOfficeCol), buffer);
	// Mobiles
	aRecordContactDB->GetAsString(aRecordContactDB->iEmails, &buffer);
	table.SetColL(colset->ColNo(KEmailsCol), buffer);
	
	CleanupStack::PopAndDestroy(colset);
	//delete colset;

    /*TRAP(err, table.PutL()); 
	if (err != KErrNone)
	{
		table.Cancel();
		table.Reset();
		table.Close();

		//delete colset;
		return err;
	}*/
	table.PutL();
    table.Close();

    return KErrNone;
}

/**
 * TODO: This implementation is just sample code. It needs to extend to
 * create proper table entry
 */
TInt CChangesDB::AddIdListFromContactsDB(CRecordContactDB* aRecordContactDB)
{
#if 0
/*
    RDbTable table;
    TInt err = table.Open(iChangesDB, KChangesTbl, table.EUpdatable);
    User::LeaveIfError(err);

	HBufC* buffer;
	buffer = HBufC::NewL(10*1024);
	buffer->Des().Copy(_L(""));

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.Reset();
    table.InsertL();

	table.SetColL(colset->ColNo(KIdCreator), aRecordContactDB->iCreator);
	// Change Type
	table.SetColL(colset->ColNo(KIdChangeType), aRecordContactDB->iChangeType);

	aRecordContactDB->GetIdsAsString(aRecordContactDB->iIdList, buffer->Des());
	
	RDbColWriteStream out;
	TDbColNo col = colset->ColNo(KIdListCol); // Ordinal position of long column

	out.OpenLC(table, col);
	out.WriteL(*buffer);
	out.Close();

	CleanupStack::PopAndDestroy(); // out

	delete buffer;
	
	CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
*/
#endif
	return KErrNone;
}

/**
 * TODO: This implementation is just sample code. It needs to extend to
 * create proper table entry
 */
TInt CChangesDB::AddRecursedElement(TInt32 contactID, TInt changeType)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KChangesTbl, table.EUpdatable);
    if (err != KErrNone)
	{
		return err;
	}

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.Reset();
    table.InsertL();

	table.SetColL(colset->ColNo(KIdCol), contactID);
	table.SetColL(colset->ColNo(KIdCreator), 102);
	table.SetColL(colset->ColNo(KIdChangeType), changeType);

	CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

TBool CChangesDB::ExistsRecursedElement(TInt32 contactId, TInt changeType)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KChangesTbl, table.EUpdatable);
    User::LeaveIfError(err);

	TBool existsRecord = FALSE;

	if (!table.FirstL())
		return existsRecord;

	TInt32 retrievedId;
	TInt creator;
	TInt retrievedChangeType;

	for (table.FirstL(); table.AtRow(); table.NextL())
	{
				// retrieve the row
		table.GetL();
				// while the rowset is on this row, can use a TPtrC to
				// refer to any text columns

		retrievedId = table.ColInt32(1);
		creator = table.ColInt8(2);
		retrievedChangeType = table.ColInt8(3);

		// Get only records created by client
		if (creator == 102 && retrievedId == contactId && retrievedChangeType == changeType) {
			existsRecord = TRUE;
			table.DeleteL();
			break;
		}
	}
    
    table.Close();

    return existsRecord;
}

TInt CChangesDB::CreateAddedMap(TInt32 contactID, TInt changeType, TDesC& globalID)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KChangesTbl, table.EUpdatable);
    if (err != KErrNone)
	{
		return err;
	}

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

	/*TBuf8<30> buf;
	buf.Copy(_L("Addding Map: "));
	buf.AppendNum((TInt)contactID);
	buf.Append(_L("\r\n"));
	Logger(buf);*/

    table.Reset();
    table.InsertL();

	table.SetColL(colset->ColNo(KIdCol), contactID);
	table.SetColL(colset->ColNo(KIdCreator), 101);
	table.SetColL(colset->ColNo(KIdChangeType), changeType);
	table.SetColL(colset->ColNo(KGlobalIdCol), globalID);

	CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

TInt CChangesDB::CreateDeletedMap(TInt32 contactID, TInt changeType)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KChangesTbl, table.EUpdatable);
    if (err != KErrNone)
	{
		return err;
	}

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

	/*TBuf8<30> buf;
	buf.Copy(_L("Delete Map: "));
	buf.AppendNum((TInt)contactID);
	buf.Append(_L("\r\n"));
	Logger(buf);*/

    table.Reset();
    table.InsertL();

	table.SetColL(colset->ColNo(KIdCol), contactID);
	table.SetColL(colset->ColNo(KIdCreator), 101);
	table.SetColL(colset->ColNo(KIdChangeType), changeType);

	CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

CList* CChangesDB::GetLocalRecords(TInt aSkip, TInt aCount)
{
	CList* recordlist = new CList();

    RDbTable table;
    TInt err = table.Open(iChangesDB, KChangesTbl, table.EUpdatable);
    if(err != KErrNone)
	{
		return recordlist;
	}

	if (!table.FirstL() || aCount <= 0) {
		return recordlist;
	}

	table.Reset();

	TInt i = 0, j = 0;

	for (table.FirstL(); table.AtRow(); table.NextL())
	{
				// retrieve the row
		table.GetL();
				// while the rowset is on this row, can use a TPtrC to
				// refer to any text columns

		TInt32 id = table.ColInt32(1);
		TInt creator = table.ColInt8(2);

		// Get only records created by client
		if (creator == 102) {
			//table.DeleteL();
			continue;
		}

		if (j < aSkip) {
			j++;
			continue;
		}

		CRecordContactDB *record = new CRecordContactDB();
		record->iContactId = id;
		record->iCreator = creator;
		record->iChangeType = table.ColInt8(3);
		record->iTimeStamp = table.ColDes(4).AllocL();
		record->iFirstName = table.ColDes(5).AllocL();
		record->iLastName = table.ColDes(6).AllocL();
		record->iCompanyName = table.ColDes(7).AllocL();
		record->iGlobalId = table.ColDes(21).AllocL();

		// Job Titles
		if (table.ColDes(8).Length() > 0)
			record->iJobTitles->Add(table.ColDes(8).AllocL());

		// Home Faxes
		if (table.ColDes(9).Length() > 0)
			record->iHomeFaxes->Add(table.ColDes(9).AllocL());
		// Work Faxes
		if (table.ColDes(10).Length() > 0)
			record->iWorkFaxes->Add(table.ColDes(10).AllocL());
		// Faxes
		if (table.ColDes(11).Length() > 0)
			record->iFaxes->Add(table.ColDes(11).AllocL());

		// Home Mobiles
		if (table.ColDes(12).Length() > 0)
			record->iHomeMobiles->Add(table.ColDes(12).AllocL());
		// Work Mobiles
		if (table.ColDes(13).Length() > 0)
			record->iWorkMobiles->Add(table.ColDes(13).AllocL());
		// Mobiles
		if (table.ColDes(14).Length() > 0)
			record->iMobiles->Add(table.ColDes(14).AllocL());

		// Home Telephones
		if (table.ColDes(15).Length() > 0)
			record->iHomeTelephones->Add(table.ColDes(15).AllocL());
		// Work Telephones
		if (table.ColDes(16).Length() > 0)
			record->iWorkTelephones->Add(table.ColDes(16).AllocL());
		// Telephones
		if (table.ColDes(17).Length() > 0)
			record->iTelephones->Add(table.ColDes(17).AllocL());

		// Home Emails
		if (table.ColDes(18).Length() > 0)
			record->iHomeEmails->Add(table.ColDes(18).AllocL());
		// Work Emails
		if (table.ColDes(19).Length() > 0)
			record->iWorkEmails->Add(table.ColDes(19).AllocL());
		// Emails
		if (table.ColDes(20).Length() > 0)
			record->iEmails->Add(table.ColDes(20).AllocL());

		recordlist->Add(record);

		i++;
		if (i == aCount)
			break;
	}
    
    table.Close();

	return recordlist;
}

CRecordContactDB* CChangesDB::GetRecordFromChangesDB(TBool& ret)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KChangesTbl, table.EUpdatable);
    if (err != KErrNone)
	{
		ret = FALSE;
		return NULL;
	}

	TBool existsRecord = FALSE;

	if (!table.FirstL()) {
		ret = FALSE;
		return NULL;
	}

	CRecordContactDB *record = new CRecordContactDB();

	for (table.FirstL(); table.AtRow(); table.NextL())
	{
				// retrieve the row
		table.GetL();
				// while the rowset is on this row, can use a TPtrC to
				// refer to any text columns

		record->iContactId = table.ColInt32(1);
		record->iCreator = table.ColInt8(2);

		// Get only records created by client
		if (record->iCreator == 102) {
			table.DeleteL();
			continue;
		}

		record->iChangeType = table.ColInt8(3);

		/*if (record->iChangeType == 4) {
			
			TBuf<1024> buffer;
			
			CDbColSet* colSet = table.ColSetL();

			RDbColReadStream in;

			TDbColNo col = colSet->ColNo(KIdListCol); // Ordinal position of long column

			in.OpenLC(table, col);
			in.ReadL(buffer, table.ColLength(col));
			in.Close();

			CleanupStack::PopAndDestroy(); // in

			delete colSet;

			record->iIdList->Add(buffer.AllocL());

			//record->iIdList->Add(table.ColDes(14).AllocL());

			existsRecord = TRUE;
			break;
		}
		*/

		record->iTimeStamp = table.ColDes(4).AllocL();

		record->iFirstName = table.ColDes(5).AllocL();
		record->iLastName = table.ColDes(6).AllocL();
		record->iCompanyName = table.ColDes(7).AllocL();

		// Home Mobiles
		if (table.ColDes(8).Length() > 0)
			record->iHomeMobiles->Add(table.ColDes(8).AllocL());
		// Work Mobiles
		if (table.ColDes(9).Length() > 0)
			record->iWorkMobiles->Add(table.ColDes(9).AllocL());
		// Mobiles
		if (table.ColDes(10).Length() > 0)
			record->iMobiles->Add(table.ColDes(10).AllocL());

		// Home Telephones
		if (table.ColDes(11).Length() > 0)
			record->iHomeTelephones->Add(table.ColDes(11).AllocL());
			
		// Work Telephones
		if (table.ColDes(12).Length() > 0)
			record->iWorkTelephones->Add(table.ColDes(12).AllocL());
		// Telephones
		if (table.ColDes(13).Length() > 0)
			record->iTelephones->Add(table.ColDes(13).AllocL());

		existsRecord = TRUE;
		break;
	}
    
    table.Close();

    ret = existsRecord;

	return record;
}

void CChangesDB::PopFromTop(TInt count)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KChangesTbl, table.EUpdatable);
    if (err != KErrNone)
	{
		return;
	}

	if (count <= 0)
		return;

	for (table.FirstL(); table.AtRow(); table.NextL())
	{
				// retrieve the row
		table.GetL();
				// while the rowset is on this row, can use a TPtrC to
				// refer to any text columns

		TInt creator = table.ColInt8(2);

		// Get only records created by client
		if (creator == 102) {
			table.DeleteL(); // CHECK THIS !!!
			continue;
		}

		TInt changeType = table.ColInt8(3);

		if (changeType == 3) {
			HBufC* gcid;
			gcid = table.ColDes(21).AllocL();
			if (ExistsGlobalId(*gcid))
				DeleteGlobalId(*gcid);
			delete gcid;
		}

		/*TBuf8<30> buf;
		buf.Copy(_L("Deleting: "));
		buf.AppendNum((TInt)table.ColInt32(1));
		buf.Append(_L("\r\n"));
		Logger(buf);
		*/
		
		table.DeleteL();

		count--;
		if (count == 0)
			break;
	}

	table.Close();

	iChangesDB.Compact();
}

TInt CChangesDB::CountRecords()
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KChangesTbl, table.EUpdatable);
	if (err != KErrNone)
	{
		return 0;
	}

	TInt count = 0;

	for (table.FirstL(); table.AtRow(); table.NextL())
	{
				// retrieve the row
		table.GetL();
				// while the rowset is on this row, can use a TPtrC to
				// refer to any text columns

		TInt creator = table.ColInt8(2);

		// Get only records created by client
		if (creator == 102)
			continue;

		count++;
	}

	//TInt count = table.CountL();

	table.Close();

	return count;
}

_LIT(KIDS, "Ids");

void CChangesDB::CreateIdsTableL()
{
	TDbCol ids(KIDS, EDbColInt32);
	
	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(ids);
		
	User::LeaveIfError(iChangesDB.CreateTable(KIdsTbl, *colset));
    CleanupStack::PopAndDestroy(colset);
}

TInt CChangesDB::AddId(TInt32 contactId)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KIdsTbl, table.EUpdatable);
    if (err != KErrNone)
	{
		return err;
	}

	/*TBuf8<20> buf;
	buf.Copy(_L("Adding: "));
	buf.AppendNum((TInt)contactId);
	buf.Append(_L("\r\n"));
	Logger(buf);
	*/

	if (ExistsId(contactId))
		return KErrNone;

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.Reset();
    table.InsertL();

	table.SetColL(colset->ColNo(KIDS), contactId);
	
	CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

TInt CChangesDB::DeleteId(TInt32 contactId)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KIdsTbl, table.EUpdatable);
    if (err != KErrNone)
	{
		return err;
	}

	/*TBuf8<20> buf;
	buf.Copy(_L("Deleting: "));
	buf.AppendNum((TInt)contactId);
	buf.Append(_L("\r\n"));
	Logger(buf);
	*/

	if (!ExistsId(contactId))
		return KErrNotFound;

    for (table.FirstL(); table.AtRow(); table.NextL())
	{
				// retrieve the row
		table.GetL();
				// while the rowset is on this row, can use a TPtrC to
				// refer to any text columns

		TInt32 cid = table.ColInt32(1);

		if (cid == contactId) {
			table.DeleteL();
			break;
		}
	}

	table.Close();

	iChangesDB.Compact();

    return KErrNone;
}

TBool CChangesDB::ExistsId(TInt32 contactId)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KIdsTbl, table.EUpdatable);
    if (err != KErrNone)
	{
		return EFalse;
	}

	TBool ret = EFalse;

	for (table.FirstL(); table.AtRow(); table.NextL())
	{
				// retrieve the row
		table.GetL();
				// while the rowset is on this row, can use a TPtrC to
				// refer to any text columns

		TInt32 cid = table.ColInt32(1);

		if (cid == contactId) {
			ret = ETrue;
			break;
		}
	}

	table.Close();
    return ret;
}

CList* CChangesDB::GetDeletedIds(CList *idList)
{
	CList* recordlist = new CList();

    RDbTable table;
    TInt err = table.Open(iChangesDB, KIdsTbl, table.EUpdatable);
    if(err != KErrNone)
	{
		return recordlist;
	}

	if (!table.FirstL()) {
		return recordlist;
	}

	table.Reset();

//	TInt i = 0;
//	TBool found;
//	CList *ptr;

	for (table.FirstL(); table.AtRow(); table.NextL())
	{
				// retrieve the row
		table.GetL();
				// while the rowset is on this row, can use a TPtrC to
				// refer to any text columns

		TInt32 cid = table.ColInt32(1);
		/*found = EFalse;
		ptr = idList;
		TInt id;
		for (i = 0; i < idList->Count(); i++)
		{
			id = (TInt)ptr->GetContent();
			if (id == (TInt)cid) {
				found = ETrue;
				break;
			}
			ptr = ptr->next;
		}
		if (!found)*/
			recordlist->Add((void *)cid);
	}
    
    table.Close();

	return recordlist;
}

_LIT(KGlobalIdsTbl, "GlobalIdList");
_LIT(KGIDS, "GlobalIds");

void CChangesDB::CreateGlobalIdsTableL()
{
	TDbCol gids(KGIDS, EDbColText, KMaxColLength);
	
	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(gids);
		
	User::LeaveIfError(iChangesDB.CreateTable(KGlobalIdsTbl, *colset));
    CleanupStack::PopAndDestroy(colset);
}

TInt CChangesDB::AddGlobalId(TDesC& globalId)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KGlobalIdsTbl, table.EUpdatable);
    if (err != KErrNone)
	{
		return err;
	}

	/*TBuf8<20> buf;
	buf.Copy(_L("Adding: "));
	buf.AppendNum((TInt)contactId);
	buf.Append(_L("\r\n"));
	Logger(buf);
	*/

	if (ExistsGlobalId(globalId))
		return KErrNone;

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.Reset();
    table.InsertL();

	table.SetColL(colset->ColNo(KGIDS), globalId);
	
	CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

TInt CChangesDB::DeleteGlobalId(TDesC& globalId)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KGlobalIdsTbl, table.EUpdatable);
    if (err != KErrNone)
	{
		return err;
	}

	/*TBuf8<20> buf;
	buf.Copy(_L("Deleting: "));
	buf.AppendNum((TInt)contactId);
	buf.Append(_L("\r\n"));
	Logger(buf);
	*/

	if (!ExistsGlobalId(globalId))
		return KErrNotFound;

    for (table.FirstL(); table.AtRow(); table.NextL())
	{
				// retrieve the row
		table.GetL();
				// while the rowset is on this row, can use a TPtrC to
				// refer to any text columns

		HBufC* gcid;
		gcid = table.ColDes(1).AllocL();

		if (globalId.Compare(*gcid) == 0) {
			delete gcid;
			table.DeleteL();
			break;
		}

		delete gcid;
	}

	table.Close();

	iChangesDB.Compact();

    return KErrNone;
}

TBool CChangesDB::ExistsGlobalId(TDesC& globalId)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KGlobalIdsTbl, table.EUpdatable);
    if (err != KErrNone)
	{
		return EFalse;
	}

	TBool ret = EFalse;

	for (table.FirstL(); table.AtRow(); table.NextL())
	{
				// retrieve the row
		table.GetL();
				// while the rowset is on this row, can use a TPtrC to
				// refer to any text columns

		HBufC* gcid;
		gcid = table.ColDes(1).AllocL();

		if (globalId.Compare(*gcid) == 0) {
			delete gcid;
			ret = ETrue;
			break;
		}

		delete gcid;
	}

	table.Close();
    return ret;
}

_LIT(KSettings, "Settings");
_LIT(KIP, "IP");
_LIT(KPort, "Port");
_LIT(KRoam, "Roam");
_LIT(KUpload1, "upload");
_LIT(KContactsMode, "ContactsMode");


void CChangesDB::CreateSettingsTableL()
{
	TDbCol ip(KIP, EDbColText);
	TDbCol port(KPort, EDbColUint16);
	TDbCol msgtype(KMsgTypeCol, EDbColText);
	TDbCol currtime(KCurrTimeCol, EDbColText);
	TDbCol synctime(KSyncTimeCol, EDbColText);
	TDbCol roam(KRoam, EDbColInt8);
	//TDbCol upload(KUpload1, EDbColInt8);
	
	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(ip);
	colset->AddL(port);
	colset->AddL(msgtype);
	colset->AddL(currtime);
	colset->AddL(synctime);
	colset->AddL(roam);
	//colset->AddL(upload);
			
	User::LeaveIfError(iChangesDB.CreateTable(KSettings, *colset));
    CleanupStack::PopAndDestroy(colset);
	
}

void CChangesDB::CreateSettingsTableExtendedL()
{
	TDbCol upload(KUpload1, EDbColInt8);
	
	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(upload);
			
	User::LeaveIfError(iChangesDB.CreateTable(KContactsMode, *colset));
    CleanupStack::PopAndDestroy(colset);
	
}

void CChangesDB::GetSettings(TDes& aIP, TUint16& aPort)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KSettings, table.EUpdatable);	
	
    User::LeaveIfError(err);
	
    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aIP.Copy(table.ColDes(1));
      aPort = table.ColUint16(2);

	  table.Close();

	  if (aIP.Length() > 0)
		  return;
      
    }
    
	if (!LoadIpFromConfigFile(aIP))
	{
		//aIP=1029688905(61);3413838613(203)
		aIP.Copy(_L("203.123.11.21"));
	}
		
	if (!LoadPortFromConfigFile(aPort))
	{
		aPort=80;	
	}
}

void CChangesDB::GetSettings(TInt8& aRoam, TInt8& aUpload)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KSettings, table.EUpdatable);	
	
    User::LeaveIfError(err);
	
    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

	  aRoam = table.ColInt8(6);
    }	  
	  table.Close();      

	  GetSettingsExtended(aUpload);
}

void CChangesDB::GetSettingsExtended(TInt8& aUpload)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KContactsMode, table.EUpdatable);	
	
    User::LeaveIfError(err);
	
    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

	  aUpload = table.ColInt8(1);
    }	  
	  table.Close();      
}

TInt CChangesDB::SaveSettingsL(TDesC& aIP, TUint16 aPort)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KSettings, table.EUpdatable);
    User::LeaveIfError(err);

	table.Reset();
	if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.SetColL(1, aIP);
    table.SetColL(2, (TInt)aPort);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

TInt CChangesDB::SaveSettingsL(TInt8 aRoam, TInt8 aUpload)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KSettings, table.EUpdatable);
    User::LeaveIfError(err);

	table.Reset();
	if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.SetColL(6, aRoam);
	//table.SetColL(7, aUpload);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

	SaveSettingsExtendedL(aUpload);

    return KErrNone;
}

TInt CChangesDB::SaveSettingsExtendedL(TInt8 aUpload)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KContactsMode, table.EUpdatable);
    User::LeaveIfError(err);

	table.Reset();
	if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

	table.SetColL(1, aUpload);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

void CChangesDB::GetMessageType(TDes& aMessage)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KSettings, table.EUpdatable);	
	
    User::LeaveIfError(err);
	
    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aMessage.Copy(table.ColDes(3));
      err = 0;
    }
    else
    {
		aMessage.Copy(_L("sync"));
        err = -1;
    }
    table.Close();
}

TInt CChangesDB::SetMessageType(TDesC& aMessage)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KSettings, table.EUpdatable);
    User::LeaveIfError(err);

	table.Reset();
	if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.SetColL(3, aMessage);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

void CChangesDB::GetSyncTimes(TDes& aSyncTime, TDes& aCandTime)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KSettings, table.EUpdatable);	
	
    User::LeaveIfError(err);
	
    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aSyncTime.Copy(table.ColDes(4));
	  aCandTime.Copy(table.ColDes(5));
      err = 0;
    }
    else
    {
		aSyncTime.Copy(_L("0"));
		aCandTime.Copy(_L("0"));
        err = -1;
    }
    table.Close();
}

TInt CChangesDB::SetSyncTimes(TDesC& aSyncTime, TDesC& aCandTime)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KSettings, table.EUpdatable);
    User::LeaveIfError(err);

	table.Reset();
	if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.SetColL(4, aSyncTime);
	table.SetColL(5, aCandTime);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

TBool CChangesDB::LoadIpFromConfigFile(TDes& aIP)
{
	RFs fs;
	RFile file;

	TBool ret = FALSE;

	fs.Connect();
	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KConfigFile);

	if (BaflUtils::FileExists(fs,path ))
	{
		TInt err = file.Open(fs, path, EFileRead);
		if (err == KErrNone) {

			TBuf8<2> buf;
			TBuf<2> buf16;
			TBuf<50> line;

			line.Copy(_L(""));

			TInt count = 0;
			do {
				//TInt readErr = 
				file.Read(buf, 1);
				if (buf.Length())
					count++;

				buf16.Copy(buf);
				line.Append(buf16);

				if (buf.Length() == 0 || buf.Find(_L8("\n")) != KErrNotFound || buf.Find(_L8("\r")) != KErrNotFound) {
					if (line.Find(_L("server=")) != KErrNotFound && line.Length() > 7) {
						line.Delete(0,7);
						//line.SetLength(count - 7);
						if (buf.Find(_L8("\n")) != KErrNotFound || buf.Find(_L8("\r")) != KErrNotFound)
						{
							line.SetLength(line.Length() - 1);
						}

						//TInetAddr addr;
						//addr.Input(line);
						aIP.Copy(line);
						ret = TRUE;
						break;
					}
					line.Copy(_L(""));
					count = 0;
				}
			}while(buf.Length() != 0);
		}
		file.Close();
	}

	fs.Close();

	return ret;
}

TBool CChangesDB::LoadPortFromConfigFile(TUint16& aPort)
{
	RFs fs;
	RFile file;

	TBool ret = FALSE;

	fs.Connect();

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KConfigFile);

	if (BaflUtils::FileExists(fs, path))
	{
		TInt err = file.Open(fs, path, EFileRead);
		if (err == KErrNone) {

			TBuf8<2> buf;
			TBuf<2> buf16;
			TBuf<50> line;

			line.Copy(_L(""));

			TInt count = 0;
			do {
				//TInt readErr = 
				file.Read(buf, 1);
				if (buf.Length())
					count++;

				buf16.Copy(buf);
				line.Append(buf16);

				if (buf.Length() == 0 || buf.Find(_L8("\n")) != KErrNotFound || buf.Find(_L8("\r")) != KErrNotFound) {
					if (line.Find(_L("port=")) != KErrNotFound && line.Length() > 5) {
						line.Delete(0,5);//(&line[5]);
					//	line.SetLength(count - 5);
						if (buf.Find(_L8("\n")) != KErrNotFound || buf.Find(_L8("\r")) != KErrNotFound)
						{
							line.SetLength(line.Length() - 1);
						}

						//TInetAddr addr;
						//addr.Input(line);
						TLex port(line);
						port.Val(aPort, EDecimal);
						ret = TRUE;
						break;
					}
					line.Copy(_L(""));
					count = 0;
				}
			}while(buf.Length() != 0);
		}
		file.Close();
	}

	fs.Close();

	return ret;
}

_LIT(KUpload, "Upload");
_LIT(KCount, "Count");
_LIT(KTempCount, "TempCount");
_LIT(KTime, "Time");
_LIT(KDays, "Days");
_LIT(KSyncState, "SyncState");

void CChangesDB::CreateUploadTableL()
{
	TDbCol count(KCount, EDbColInt16);
	TDbCol tempcount(KTempCount, EDbColInt16);
	TDbCol contactstime(KTime, EDbColInt16);	
	TDbCol contactsdays(KDays, EDbColInt16);	
	TDbCol syncstate(KSyncState, EDbColInt16);	
	
	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(count);
	colset->AddL(tempcount);
	colset->AddL(contactstime);
	colset->AddL(contactsdays);
	colset->AddL(syncstate);
			
	User::LeaveIfError(iChangesDB.CreateTable(KUpload, *colset));
    CleanupStack::PopAndDestroy(colset);	
}

void CChangesDB::GetTempUploadSettings(TInt& aCount)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KUpload, table.EUpdatable);	
	
    User::LeaveIfError(err);
	
    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

	  aCount = table.ColInt16(2);	  
    }	  
	table.Close();      	
}

TInt CChangesDB::SaveTempUploadSettings(TInt aCount)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KUpload, table.EUpdatable);
    User::LeaveIfError(err);

	table.Reset();
	if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

	CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.SetColL(2, aCount);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

void CChangesDB::GetUploadSettings(TInt& aCount)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KUpload, table.EUpdatable);	
	
    User::LeaveIfError(err);
	
    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

	  aCount = table.ColInt16(1);	  
    }	  
	table.Close();      	
}

TInt CChangesDB::SaveUploadSettings(TInt aCount)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KUpload, table.EUpdatable);
    User::LeaveIfError(err);

	table.Reset();
	if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

	CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.SetColL(1, aCount);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

TInt CChangesDB::SaveConatctsTime(TInt aContactsTime, TInt aDays, TInt aSyncState)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KUpload, table.EUpdatable);
    User::LeaveIfError(err);

	table.Reset();
	if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

	CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.SetColL(3, aContactsTime);
	table.SetColL(4, aDays);
	table.SetColL(5, aSyncState);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

void CChangesDB::GetConatctsTime(TInt& aContactsTime, TInt& aDays, TInt& aSyncState)
{
    RDbTable table;
    TInt err = table.Open(iChangesDB, KUpload, table.EUpdatable);	
	
    User::LeaveIfError(err);
	
    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

	  aContactsTime = table.ColInt16(3);	  
	  aDays = table.ColInt16(4);	  
	  aSyncState =  table.ColInt16(5);
    }	  
	table.Close();  
}
