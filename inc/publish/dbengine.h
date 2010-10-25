/*
 *  Name     : CPhotosDB from DBEngine.h
 */

#ifndef __PHOTOSDBENGINE_H__
#define __PHOTOSDBENGINE_H__


#include <e32std.h>
#include <badesca.h>    // CDesCArrayFlat (cannot be forward declarated)
#include <d32dbms.h>    // RDbStoreDatabase
#include <f32file.h>    // RFs

// Forward declarations
class CFileStore;
/**
 *  Maximum length of individual item in CDesCArrayFlat (see GetAllBooksL &
 *  GetBooksByKeyL methods).
 */
//const int KDBItemMaxLength = 1024;

/*struct TRecordPhotos
{
    HBufC8 *photofile; //filename with complete path
    TInt8 type; // folder type
    HBufC8 *comment; // description
};
*/
/**
 * Class:       CPhotosDB
 *
 * Database definition:
 *
 *
 *              Table name: Books
 *                 Column:       Type:          Max length:
 *                 ------        -----          -----------
 */
class CPhotosDB : public CBase
{
public: // Creation and destruction

    /**
     * Function:    NewL
     *
     * Description: Get instance of a CPhotosDB object.
     */
    static CPhotosDB* NewL();

    /**
     * Function:    CPhotosDB
     *
     * Description: Perform the first phase of two phase construction.
     */
    CPhotosDB();

    /**
     * Function:    ~CPhotosDB
     *
     * Description: Destroy the object.
     */
    ~CPhotosDB();


public: // Public API for database operations

    /**
     * Function:    OpenDbL
     *
     * Description: Open existing Changes.db database in exclusive
     *              (non-shared) mode.
     *
     * Param:       aExistingPhotosDBFile is full path to 
     *              database file.
     *
     * Return:      KErrNone, if no error. KErrNotFound, if the file does
     *              not exist.
     *
     * Leaves:      With one of the system wide error codes, if the file
     *              is not a correct database file.
     */
    TInt OpenDbL(const TFileName& aExistingPhotosDBFile);

    /**
     * Function:    CreateDbL
     *
     * Description: Creates and opens a new database. Creates a
     *              database file, table structure and an index. The database
     *              will be open in exclusive (non-shareable) mode. The
     *              database must be closed, when not used any more. If the
     *              database exists, it is replaced.
     *
     * Param:       aExistingPhotosDBFile Name of the new database file. Is  a full
     *              path (incl. the filename). Operations following this call
     *              are performed to the new database file.
     *
     * Return:      returns always KErrNone
     *
     * Leaves:      If the file cannot be created or database initialized.
     *              Leaves with system wide error codes.
     */
    TInt CreateDbL(const TFileName& aNewPhotosDBFile);

    /**
     * Function:    RemoveDbL
     *
     * Description: Removes database. Closes any open database,
     *              before dropping the database.
     *
     * Param:       aNewPhotosDBFile is full path to 
	 *              database file.
     *
     * Leaves:      If the file file is not a valid database file or the
     *              database does not containt Books table, the method
     *              leaves with system wide error codes.
     */
    TInt RemoveDbL(const TFileName& aExistingPhotosDBFile);

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
     * Function:    AddRecord()
     *
     * Description: 
     *
     * Param:       
     *
     */	

    TInt AddRecord(TDesC& aPhoto, TInt8 aType, TDesC& aRFolder, TDesC& aDesc, TInt8 aPhotoVideo, TInt8 aNew, TDesC& aStatus);

    TInt GetRecord(TDesC& aPhoto, TInt8& aType, TDesC& aRFolder, TDesC& aDesc, TInt8& aPhotoVideo, TInt8& aNew, TDesC& aStatus);

    TInt DeleteFirstRecord();

	TInt DeleteNonExistantRecord();

	TInt DeleteSharingsFirstRecord();	

	TInt GetPublishType();

	TInt PhotosRowCount();

	TInt VideosRowCount();

    TInt RowCount();

	TInt RowCountForSharing();

	TInt SaveUserSettingsL(TDesC& aUserName, TDesC& aPassWord, TDesC& aMobileNo);

	void GetUserSettings(TDes& aUserName, TDes& aPassWord, TDes& aMobileNo);

	void GetChoice(TInt8& aChoice);

	void SaveChoice(TInt8 aChoice);

	TInt SaveSettingsL(TInt8 aSmsMode, TTime aSmsTime, TInt8 aSmsRoam);

	TInt SaveSmsSettingL(TInt8 aSmsMode);

	TInt SaveSettingsL(TDesC& aIP, TUint16 aPort, TInt8 aAuto);

	TInt SaveGeneralSettingL(TInt8 aAuto);
	
	TInt SaveSettingsL(TInt8 aMode, TDesC& aFolder, TInt8 aDesc, TInt8 aRoam, TInt8 aSecureGallery);

	TInt SavePhotoSettingL(TInt8 aMode);

	void GetSettings(TInt8& aSmsMode, TTime& aSmsTime, TInt8& aSmsRoam);

	void GetSettings(TDes& aIP, TUint16& aPort, TInt8& aAuto);
	
	void GetSettings(TInt8& aMode,TDesC& aFolder, TInt8& aDesc, TInt8& aRoam, TInt8& aSecureGallery);
	
	TInt SaveSmsCountL(TInt aCount);

	void GetSmsCountL(TInt& aCount);

	CDesCArray* GetSmsMobileList();
	
	CDesCArray* GetSmsNameList();

	TInt DeleteContactsForSmsFromDB(TInt pos);

	TInt SaveSelectiveSmsToDB(TDesC& aNames, TDesC& aMobiles);

	TInt SaveMailsToDB(TDesC& aFolder, TDesC& aNames, TDesC& aMails, TDesC& aMess, TInt8 aCount);

	void GetMailsFromDB(TDesC& aFolder, TDesC& aNames, TDesC& aMails, TDesC& aMess, TInt8& aCount);

	TInt SaveUploadSettingsL(TInt aPhoto, TInt aVideo, TInt aAlbumShares);

	void GetUploadSettings(TInt& aAlbum, TInt& aPhoto, TInt& aVideo, TInt& aAlbumShares);

	TInt CheckAndSaveFolder(TDesC& aFolder);

	TInt CheckUploadedMediaFileName(TDesC& aFilename);

	TInt CheckUploadingMediaFileName(TDesC& aFilename);

	TInt CheckAndSaveMediaFile(TDesC& aFilename);

	void GetMediaFileName(TDesC& aFilename);

	//void CheckAndIncreaseStatusCount(TDesC& aFolder, TInt type);
	
	//void UpdatePhotosVideosCount(TDesC& aFolder, TInt aCount, TInt type);
	
	//void AlbumShareStatus(TDesC& aFolder);
	
	CDesCArray* GetFolderList();

	void GetMode(TInt8& aMode);

	TInt CheckFolder(TDesC& aFolder);

	TBool LoadIpFromConfigFile(TDes& aIP);

	TBool LoadPortFromConfigFile(TUint16& aPort);

	TInt CheckAndDeleteFolder(TDesC& aFolder);

	TInt CheckAndEditFolder(TDesC& aOldFolder, TDesC& aNewFolder);

	//void UpdateSmsTimestampToDB();

	void UpdateSmsTimestampToDB(TTime aTime);

	void GetSmsTimestampFromDB(TTime& aTime);

	void SaveSubStatusSyncTimeToDB(TDesC& aTime);

	void GetSubStatusSyncTimeFromDB(TDesC& aTime);

	void GetNetworkErrorCode(TInt& aCode);

	void GetNetworkStatusCode(TInt& aCode);

	void SaveNetworkErrorCode(TInt aCode);

	void SaveNetworkStatusCode(TInt aCode);

	TInt CheckIfPhotoExists(TDesC& aPhoto);

	void GetUpgradeStatus(TInt8& aChoice, TDes& aVersion);

	void SaveUpgradeStatus(TInt8 aChoice, TDesC& aVersion);

	void SaveApnID(TUint32 aId);

	void GetApnID(TUint32& aId);

	void UpdateSentItemsSmsTimestampToDB(TTime aTime);
	
	void GetSentItemsSmsTimestampFromDB(TTime& aTime);

	void UpdateBackupTimestampToDB(TTime aTime);

	void GetBackupTimestampFromDB(TTime& aTime);

	void UpdateContactsSettingsTimestampToDB(TTime aOldTime, TTime aNewTime);

	void GetContactsSettingsTimestampFromDB(TTime& aOldTime, TTime& aNewTime);

	void UpdatePublishSettingsTimestampToDB(TTime aOldTime, TTime aNewTime);

	void GetPublishSettingsTimestampFromDB(TTime& aOldTime, TTime& aNewTime);

	void UpdateSmsSettingsTimestampToDB(TTime aOldTime, TTime aNewTime);

	void GetSmsSettingsTimestampFromDB(TTime& aOldTime, TTime& aNewTime);

	void UpdateGeneralSettingsTimestampToDB(TTime aOldTime, TTime aNewTime);

	void GetGeneralSettingsTimestampFromDB(TTime& aOldTime, TTime& aNewTime);

	void UpdateSecureSettingsTimestampToDB(TTime aOldTime, TTime aNewTime);

	void GetSecureSettingsTimestampFromDB(TTime& aOldTime, TTime& aNewTime);

private: // Construction

    /**
     * Function:    ConstructL
     *
     * Description: Perform the second phase construction of a CPhotosDB
     *              object
     */
    void ConstructL();


private: // Helpers

    void CreatePhotosTableL();     /* Create the Changes table          */
	void CreateFoldersTableL();
	void CreateSharingTableL();
	void CreateMediaUploadFileTableL();
	void CreateSettingsTableL();
	void CreateStatusCountTableL();
	void CreateSelectiveSmsTableL();
	void CreateSubscriptionStatusTableL();
	void CreateStatusDisplayTableL();
	void CreateSmsTimestampTableL();
	void CreateSettingsTimestampTableL();
    void DropPhotosTableL();       /* Drop Contacts table                */


private: // Member data

    RFs              iFsSession;  /* For use of iFileStore                 */
    RDbStoreDatabase iPhotosDB;/* For database operations                 */
    CFileStore*      iFileStore;  /* For creating and opening database files */
    TBool            iOpen;       /* Flag indicating iPhotosDB open status*/
};


#endif // __PHOTOSDBENGINE_H__

