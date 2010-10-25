/*
 *  Name     : CChangesDB from DBEngine.h
 */

#ifndef __CONTACTSDBENGINE_H__
#define __CONTACTSDBENGINE_H__


#include <e32std.h>
#include <badesca.h>    // CDesCArrayFlat (cannot be forward declarated)
#include <d32dbms.h>    // RDbStoreDatabase
#include <f32file.h>    // RFs


// Forward declarations
class CFileStore;

class CRecordContactDB;
class CList;

/**
 * Class:       CChangesDB
 *
 * Database definition:
 *
 *
 *              Table name: Books
 *                 Column:       Type:          Max length:
 *                 ------        -----          -----------
 */
class CChangesDB : public CBase
{
public: // Creation and destruction

    /**
     * Function:    NewL
     *
     * Description: Get instance of a CChangesDB object.
     */
    static CChangesDB* NewL();

    /**
     * Function:    ~CChangesDB
     *
     * Description: Destroy the object.
     */
    ~CChangesDB();


public: // Public API for database operations

    /**
     * Function:    OpenDbL
     *
     * Description: Open existing Changes.db database in exclusive
     *              (non-shared) mode.
     *
     * Param:       aExistingChangesDBFile is full path to 
     *              database file.
     *
     * Return:      KErrNone, if no error. KErrNotFound, if the file does
     *              not exist.
     *
     * Leaves:      With one of the system wide error codes, if the file
     *              is not a correct database file.
     */
    TInt OpenDbL(const TFileName& aExistingChangesDBFile);

    /**
     * Function:    CreateDbL
     *
     * Description: Creates and opens a new database. Creates a
     *              database file, table structure and an index. The database
     *              will be open in exclusive (non-shareable) mode. The
     *              database must be closed, when not used any more. If the
     *              database exists, it is replaced.
     *
     * Param:       aExistingChangesDBFile Name of the new database file. Is  a full
     *              path (incl. the filename). Operations following this call
     *              are performed to the new database file.
     *
     * Return:      returns always KErrNone
     *
     * Leaves:      If the file cannot be created or database initialized.
     *              Leaves with system wide error codes.
     */
    TInt CreateDbL(const TFileName& aNewChangesDBFile);

    /**
     * Function:    RemoveDbL
     *
     * Description: Removes database. Closes any open database,
     *              before dropping the database.
     *
     * Param:       aNewChangesDBFile is full path to 
	 *              database file.
     *
     * Leaves:      If the file file is not a valid database file or the
     *              database does not containt Books table, the method
     *              leaves with system wide error codes.
     */
    TInt RemoveDbL(const TFileName& aExistingChangesDBFile);

    /**
     * Function:    Close
     *
     * Description: Closes the database opened with either OpenDbL or
     *              CreateDbL. It is safe to close the database even if it
     *              is closed.
     *
     * Return:      KErrNone, if no error. KErrNotFound, if the file does
     *              not exist.
     */
    TInt Close();

    /**
     * Function:    IsOpen
     *
     * Description: Return status, whether the database is open and ready
     *              for operations.
     */
    TBool IsOpen() const;

    /**
     * Function:    AddRecordFromContactsDB()
     *
     * Description: 
     *
     * Param:       
     *
     */
    TInt AddRecordFromContactsDB(CRecordContactDB* aRecordContactDB);
	/**
     * Function:    AddIdListFromContactsDB()
     *
     * Description: 
     *
     * Param:       
     *
     */

    TInt AddIdListFromContactsDB(CRecordContactDB* aRecordContactDB);
	/**
     * Function:    AddRecursedElement()
     *
     * Description: 
     *
     * Param:       
     *
     */
	TInt AddRecursedElement(TInt32 contactID, TInt changeType);
	/**
     * Function:    ExistsRecursedElement()
     *
     * Description: 
     *
     * Param:       
     *
     */
	TBool ExistsRecursedElement(TInt32 contactId, TInt changeType);
	/**
     * Function:    GetRecordFromChangesDB()
     *
     * Description: 
     *
     * Param:       
     *
     */
    CRecordContactDB* GetRecordFromChangesDB(TBool& ret);

	// Get list of all existing records added locally skipping aSkip upto aCount
	CList* GetLocalRecords(TInt aSkip, TInt aCount);

	/**
     * Function:    PopFromTop()
     *
     * Description: 
     *
     * Param:       
     *
     */
    void PopFromTop(TInt count);
    /**
     * Function:    AddRecordFromServer(const TRecordServerXML& aRecordServer)
     *
     * Description: 
	 *
     * Param:       
     *
     */
    //TInt AddRecordFromServer(const TRecordServerXML& aRecordServer);

    /**
     * Function:    MarkRecord()
     *
     * Description: 
	 *
     * Param:       
     *
     */
    //TInt MarkRecord();

	TInt CreateAddedMap(TInt32 contactID, TInt changeType, TDesC& globalID);
	TInt CreateDeletedMap(TInt32 contactID, TInt changeType);

	TInt CountRecords();

	TInt SaveConatctsTime(TInt aContactsTime, TInt aDays, TInt aSyncState);
	void GetConatctsTime(TInt& aContactsTime, TInt& aDays, TInt& aSyncState);

	void GetTempUploadSettings(TInt& aCount);
	TInt SaveTempUploadSettings(TInt aCount);

	void GetUploadSettings(TInt& aCount);
	TInt SaveUploadSettings(TInt aCount);
	
	void GetSettings(TDes& aIP, TUint16& aPort);
	TInt SaveSettingsL(TDesC& aIP, TUint16 aPort );

	void GetSettings(TInt8& aRoam, TInt8& aUpload);
	void GetSettingsExtended(TInt8& aUpload);
	TInt SaveSettingsL(TInt8 aRoam, TInt8 aUpload);
	TInt SaveSettingsExtendedL(TInt8 aUpload);

	void GetMessageType(TDes& aMessage);
	TInt SetMessageType(TDesC& aMessage);

	void GetSyncTimes(TDes& aSyncTime, TDes& aCandTime);
	TInt SetSyncTimes(TDesC& aSyncTime, TDesC& aCandTime);

	TInt AddId(TInt32 contactId);
	TInt DeleteId(TInt32 contactId);
	TBool ExistsId(TInt32 contactId);
	CList* GetDeletedIds(CList *idList);

	TInt AddGlobalId(TDesC& globalId);
	TInt DeleteGlobalId(TDesC& globalId);
	TBool ExistsGlobalId(TDesC& globalId);

private: // Construction

    /**
     * Function:    ConstructL
     *
     * Description: Perform the second phase construction of a CChangesDB
     *              object
     */
    void ConstructL();

    /**
     * Function:    CChangesDB
     *
     * Description: Perform the first phase of two phase construction.
     */
    CChangesDB();

private: // Helpers

    void CreateChangesTableL();     /* Create the Contacts table          */
	void CreateSettingsTableL();	/* Create the Settings table          */
	void CreateSettingsTableExtendedL();
	void CreateIdsTableL();			/* Create the Id list table			  */
	void CreateGlobalIdsTableL();	/* Create the global Id list table    */
    void DropChangesTableL();       /* Drop Contacts table                */
	void CreateUploadTableL();

	TBool LoadIpFromConfigFile(TDes& aIP);
	TBool LoadPortFromConfigFile(TUint16& aPort);

private: // Member data

    RFs              iFsSession;  /* For use of iFileStore                 */
    RDbStoreDatabase iChangesDB;/* For database operations                 */
    CFileStore*      iFileStore;  /* For creating and opening database files */
    TBool            iOpen;       /* Flag indicating iChangesDB open status*/
};


#endif // __CONTACTSDBENGINE_H__
