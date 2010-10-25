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
 Module Name				: DBEngine.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     Database engine, to store settings, backup cache, etc.
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : DB Engine for Asurion deployment.


***************************************************************/

#include <badesca.h>    // CDesCArrayFlat
#include <s32file.h>    // CFileStore & CPermanentFileStore
#include <bautils.h>    // file helpers
#include <in_sock.h>
#include <eikenv.h>
#include "DBEngine.h"
#include "ripplevaultappui.h"

// Table Structure for PhotosDB
_LIT(KPhotosTbl, "Photos"); // table name

_LIT(KFilePath, "Filepath"); // file path photo/video
_LIT(KPublType, "Type"); // folder type - default etc...
_LIT(KFolder, "Folder"); // remote folder name
_LIT(KMediaUploadFolder, "MediaUploadFolder"); // media upolad folder name
//_LIT(KPhotosCount, "PhotosCount");
//_LIT(KVideosCount, "VideosCount");
//_LIT(KAlbumShare, "AlbumShare");
_LIT(KDesc, "Description"); // remote Description name
_LIT(KPhotoVideo, "PhotoVideo"); // Photo or Video
_LIT(KIsNew, "IsNew");
_LIT(KStatusString, "StatusString");

_LIT(KFolderMails, "FolderMails"); // table name
_LIT(KAlbum, "Album"); // folder name
_LIT(KNames, "Names"); // storing names
_LIT(KMailId, "MailId"); // storing mailid
_LIT(KMess, "Message"); // storing frequency
_LIT(KCount, "Count"); // Count

_LIT(KSelectiveSms, "SelectiveSms"); // table name
_LIT(KSmsNames, "SmsNames"); // storing names
_LIT(KSmsmobile, "Smsmobile"); // storing mailid

// ---------------------------------------------------------------------------
// CPhotosDB::NewL()
//
// Create instance of the PhotosDB database engine.
// ---------------------------------------------------------------------------
CPhotosDB* CPhotosDB::NewL()
{
    CPhotosDB* tmp = new (ELeave)CPhotosDB();
    CleanupStack::PushL(tmp);
    tmp->ConstructL();
    CleanupStack::Pop();
    return tmp;
}

// ---------------------------------------------------------------------------
// CPhotosDB::~CPhotosDB()
//
// Destructor of the PhotosDB database engine. Release resources.
// ---------------------------------------------------------------------------
CPhotosDB::~CPhotosDB()
{
    Close();  // Just in case, if the user does not close this explicitely
    iFsSession.Close();
}

// ---------------------------------------------------------------------------
// CPhotosDB::ConstructL()
//
// Second phase construction. Leaves, if RFs session cannot be created.
// ---------------------------------------------------------------------------
void CPhotosDB::ConstructL()
{
    TInt err = iFsSession.Connect();
    if(err)
        User::Leave(err);
}

// ---------------------------------------------------------------------------
// CPhotosDB::CPhotosDB()
//
// Constructor
// ---------------------------------------------------------------------------
CPhotosDB::CPhotosDB()
{
    iOpen = EFalse;
}

// ---------------------------------------------------------------------------
// CPhotosDB::OpenDbL()
//
// Open existing PhotosDB database for exclusive access.
// ---------------------------------------------------------------------------
TInt CPhotosDB::OpenDbL(const TFileName& aExistingPhotosDBFile)
{
    Close();

    if(!BaflUtils::FileExists(iFsSession, aExistingPhotosDBFile))
    {
        return KErrNotFound;
    }

    iFileStore = CPermanentFileStore::OpenL(iFsSession,
        aExistingPhotosDBFile, EFileRead|EFileWrite);
    iFileStore->SetTypeL(iFileStore->Layout());       // Set file store type
    iPhotosDB.OpenL(iFileStore,iFileStore->Root());

	TRAPD(e, CreateSmsTimestampTableL());
	TRAPD(f, CreateMediaUploadFileTableL());
	TRAPD(g, CreateSettingsTimestampTableL());
    iOpen = ETrue;
    return KErrNone;
}

// ---------------------------------------------------------------------------
// CPhotosDB::CreateDbL()
//
// Create a new database. The database will be in exclusive access mode.
// ---------------------------------------------------------------------------
TInt CPhotosDB::CreateDbL(const TFileName& aNewPhotosDBFile)
{
    Close();

    if(!BaflUtils::FileExists(iFsSession, aNewPhotosDBFile))
    {
        // Create empty database file.
        BaflUtils::EnsurePathExistsL(iFsSession, aNewPhotosDBFile);
        iFileStore = CPermanentFileStore::ReplaceL(iFsSession,
            aNewPhotosDBFile, EFileRead|EFileWrite);
        iFileStore->SetTypeL(iFileStore->Layout());       // Set file store type
        TStreamId id = iPhotosDB.CreateL(iFileStore);  // Create stream object
        iFileStore->SetRootL(id);            // Keep database id as root of store
        iFileStore->CommitL();               // Complete creation by commiting
        
        // Create PhotosDB tables and indexes
	    CreatePhotosTableL();
		CreateFoldersTableL();
		CreateMediaUploadFileTableL();
		CreateSharingTableL();
		CreateSelectiveSmsTableL();
		CreateSettingsTableL();
		CreateStatusCountTableL();
		CreateSubscriptionStatusTableL();
		CreateStatusDisplayTableL();
		CreateSmsTimestampTableL();
		CreateSettingsTimestampTableL();
        iOpen = ETrue;
    }
    else
    {
        // assume db exists
        return -1;
    }
    return KErrNone;
}

// ---------------------------------------------------------------------------
// CPhotosDB::RemoveDbL()
//
// First remove the Books table. Then remove the database file.
// ---------------------------------------------------------------------------
TInt CPhotosDB::RemoveDbL(const TFileName& aExistingPhotosDBFile)
{
    Close();

    if(!BaflUtils::FileExists(iFsSession, aExistingPhotosDBFile))
    {
        return KErrNotFound;
    }

    OpenDbL(aExistingPhotosDBFile);
    DropPhotosTableL();
    Close();

    iFsSession.Delete(aExistingPhotosDBFile);
    return KErrNone;
}

// ---------------------------------------------------------------------------
// CPhotosDB::Close()
//
// Close the database.
// ---------------------------------------------------------------------------
TInt CPhotosDB::Close()
{
    iPhotosDB.Close();
    if(iFileStore)
    {
        delete iFileStore;
        iFileStore = NULL;
    }
    iOpen = EFalse;
    return KErrNone;
}

// ---------------------------------------------------------------------------
// CPhotosDB::IsOpen()
//
// Return open status of the database.
// ---------------------------------------------------------------------------
TBool CPhotosDB::IsOpen() const
{
    return iOpen;
}

_LIT(KSettings, "Settings");
_LIT(KIP, "IP");
_LIT(KPort, "Port");
_LIT(KMode, "Mode");
_LIT(KDescSet, "Desc");
_LIT(KRoam, "Roam");
_LIT(KAutostart, "Autostart");
_LIT(KChoice, "Choice");
_LIT(KSmsmode, "Smsmode");
_LIT(KSmstime, "Smstime");
_LIT(KSmstimestamp, "Smstimestamp");
_LIT(KSmsRoam, "SmsRoam");
_LIT(KSmsCount, "SmsCount"); // store sms count
_LIT(KSecureGallery, "SecureGallery");

_LIT(KStatusCount, "StatusCount");
_LIT(KPhotosCount, "PhotosCount");
_LIT(KVideosCount, "VideosCount");
_LIT(KAlbumShareCount, "AlbumShareCount");

_LIT(KUserId, "UserId");
_LIT(KPassWord, "PassWord");
_LIT(KMobileNo, "MobileNo");

_LIT(KStatusDisplayTable, "StatusDisplayTable");
_LIT(KNetworkStatus, "NetworkStatus");
_LIT(KErrorStatus, "ErrorStatus");
_LIT(KUpgradeStatus, "UpgradeStatus");
_LIT(KVersionStatus, "VersionStatus");
_LIT(KApnConnectionID, "ApnConnectionID");

_LIT(KSmsTimeStampTable, "SmsTimeStampTable");
_LIT(KSmsSentItemsTimestamp, "SmsSentItemsTimestamp");
_LIT(KLastBackupTime, "LastBackupTime");

void CPhotosDB::CreateStatusCountTableL()
{
	TDbCol photoscount(KPhotosCount, EDbColInt16);
	TDbCol videoscount(KVideosCount, EDbColInt16);
	TDbCol albumsharecount(KAlbumShareCount, EDbColInt16);
	TDbCol userid(KUserId, EDbColText);
	TDbCol password(KPassWord, EDbColText);
	TDbCol mobileno(KMobileNo, EDbColText);

	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(photoscount);
	colset->AddL(videoscount);
	colset->AddL(albumsharecount);
	colset->AddL(userid);
	colset->AddL(password);
	colset->AddL(mobileno);

    User::LeaveIfError(iPhotosDB.CreateTable(KStatusCount, *colset));
    CleanupStack::PopAndDestroy(colset);
	
}

void CPhotosDB::CreateSettingsTableL()
{
	TDbCol ip(KIP, EDbColText);
	TDbCol port(KPort, EDbColUint16);
	TDbCol mode(KMode, EDbColInt8);
	TDbCol deffolder(KFolder, EDbColText);
	TDbCol desc(KDescSet, EDbColInt8);
	TDbCol roam(KRoam, EDbColInt8);
	TDbCol autostart(KAutostart, EDbColInt8);
	TDbCol choice(KChoice, EDbColInt8);	
	TDbCol smsmode(KSmsmode, EDbColInt8);	
	TDbCol smstime(KSmstime, EDbColDateTime);	
	TDbCol smstimestamp(KSmstimestamp, EDbColDateTime);	
	TDbCol smsroam(KSmsRoam, EDbColInt8);
	TDbCol smscount(KSmsCount, EDbColInt16);
	TDbCol securegallery(KSecureGallery, EDbColInt8);
	//TDbCol photoscount(KPhotosCount, EDbColInt8);
	//TDbCol videoscount(KVideosCount, EDbColInt8);
	//TDbCol albumsharecount(KAlbumShareCount, EDbColInt8);

	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(ip);
	colset->AddL(port);
	colset->AddL(mode);
	colset->AddL(deffolder);
	colset->AddL(desc);
	colset->AddL(roam);
	colset->AddL(autostart);
	colset->AddL(choice);
	colset->AddL(smsmode);
	colset->AddL(smstime);
	colset->AddL(smstimestamp);
	colset->AddL(smsroam);
	colset->AddL(smscount);
	colset->AddL(securegallery);
	//colset->AddL(photoscount);
	//colset->AddL(videoscount);
	//colset->AddL(albumsharecount);

    User::LeaveIfError(iPhotosDB.CreateTable(KSettings, *colset));
    CleanupStack::PopAndDestroy(colset);
	
}

_LIT(KSubStatus, "SubStatus");
_LIT(KSyncTime, "SyncTime");

void CPhotosDB::CreateSubscriptionStatusTableL()
{
	//TDbCol lastsynctime(KSyncTime, EDbColDateTime);
	TDbCol lastsynctime(KSyncTime, EDbColText);

	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(lastsynctime);

    User::LeaveIfError(iPhotosDB.CreateTable(KSubStatus, *colset));
    CleanupStack::PopAndDestroy(colset);	
}

void CPhotosDB::CreateStatusDisplayTableL()
{
	TDbCol networkstatus(KNetworkStatus, EDbColInt16);
	TDbCol errorstatus(KErrorStatus, EDbColInt16);
	TDbCol upgradestatus(KUpgradeStatus, EDbColInt8);
	TDbCol versionstatus(KVersionStatus, EDbColText);
	TDbCol apnid(KApnConnectionID, EDbColUint32);

	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(networkstatus);
	colset->AddL(errorstatus);
	colset->AddL(upgradestatus);
	colset->AddL(versionstatus);
	colset->AddL(apnid);

    User::LeaveIfError(iPhotosDB.CreateTable(KStatusDisplayTable, *colset));
    CleanupStack::PopAndDestroy(colset);	
}

TInt CPhotosDB::SaveSettingsL(TDesC& aIP, TUint16 aPort, TInt8 aAuto)//, TInt8 aMode, TDesC& aFolder, TInt8 aDesc, TInt8 aRoam)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EUpdatable);
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
    table.SetColL(2, (TInt) aPort);
	table.SetColL(7, aAuto);
//    table.SetColL(3, aMode);
//	table.SetColL(4, aFolder);
//	table.SetColL(5, aDesc);
//	table.SetColL(6, aRoam);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

TInt CPhotosDB::SaveSettingsL(TInt8 aMode, TDesC& aFolder, TInt8 aDesc, TInt8 aRoam, TInt8 aSecureGallery)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EUpdatable);
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

//    table.SetColL(1, aIP);
//    table.SetColL(2, (TInt)aPort);
    table.SetColL(3, aMode);
	table.SetColL(4, aFolder);
	table.SetColL(5, aDesc);
	table.SetColL(6, aRoam);
	table.SetColL(14, aSecureGallery);
//	table.SetColL(7, aAutostart);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

TInt CPhotosDB::SaveSettingsL(TInt8 aSmsMode, TTime aSmsTime, TInt8 aSmsRoam)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EUpdatable);
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

//  table.SetColL(1, aIP);
//  table.SetColL(2, (TInt)aPort);
//  table.SetColL(3, aMode);
//	table.SetColL(4, aFolder);
//	table.SetColL(5, aDesc);
//	table.SetColL(6, aRoam);
//	table.SetColL(7, aAutostart);
	table.SetColL(9, aSmsMode);
	table.SetColL(10, aSmsTime);
	table.SetColL(12, aSmsRoam);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

void CPhotosDB::GetSettings(TInt8& aSmsMode, TTime& aSmsTime, TInt8& aSmsRoam)//, TInt8& aMode, TDesC& aFolder, TInt8& aDesc, TInt8& aRoam)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EUpdatable);
    User::LeaveIfError(err);
	
    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aSmsMode = table.ColInt8(9);
      aSmsTime = table.ColTime(10);
      aSmsRoam = table.ColInt8(12);

      err = 0;
    }    
    table.Close();
}



void CPhotosDB::GetSettings(TDes& aIP, TUint16& aPort, TInt8& aAuto)//, TInt8& aMode, TDesC& aFolder, TInt8& aDesc, TInt8& aRoam)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EUpdatable);
    User::LeaveIfError(err);
	
    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aIP = table.ColDes16(1);
      aPort = table.ColUint16(2);
      aAuto = table.ColInt8(7);
//      aMode = table.ColInt8(3);
//	  ((TDes&)aFolder).Copy(table.ColDes16(4));
//	  aDesc = table.ColInt8(5);
//	  aRoam = table.ColInt8(6);	  
      err = 0;
    }
    else
    {
		if (!LoadIpFromConfigFile(aIP))
		{
			//aIP=1029688905(61);3413838613(203)
			aIP.Copy(_L("203.123.11.21"));
		}
		
		if (!LoadPortFromConfigFile(aPort))
		{
			aPort=80;	
		}
        err = -1;
    }
    table.Close();
}

void CPhotosDB::GetSettings(TInt8& aMode, TDesC& aFolder, TInt8& aDesc, TInt8& aRoam, TInt8& aSecureGallery)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EUpdatable);
    User::LeaveIfError(err);
	
    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

//      aIP = table.ColDes16(1);
//      aPort = table.ColUint16(2);
      aMode = table.ColInt8(3);
	  ((TDes&)aFolder).Copy(table.ColDes16(4));
	  aDesc = table.ColInt8(5);
	  aRoam = table.ColInt8(6);
	  aSecureGallery = table.ColInt8(14);
//	  aAutostart = table.ColInt8(7);
      err = 0;
    }
    else
    {
//		if (!LoadIpFromConfigFile(aIP))
//		{
//			//aIP=1029688905(61);3413838613(203)
//			aIP.Copy(_L("203.123.11.21"));
//		}
//		
//		if (!LoadPortFromConfigFile(aPort))
//		{
//			aPort=80;	
//		}
		aMode = 1;

        err = -1;
    }
    table.Close();
}

void CPhotosDB::CreateFoldersTableL()
{
	TDbCol folder(KFolder, EDbColText);
	//TDbCol photoscount(KPhotosCount, EDbColInt8);
	//TDbCol videoscount(KVideosCount, EDbColInt8);
	//TDbCol albumshare(KAlbumShare, EDbColInt8);
	
	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(folder);
	//colset->AddL(photoscount);
	//colset->AddL(videoscount);	
	//colset->AddL(albumshare);
		
    User::LeaveIfError(iPhotosDB.CreateTable(KFolder, *colset));
    CleanupStack::PopAndDestroy(colset);
}

void CPhotosDB::CreateSharingTableL()
{
	TDbCol album(KAlbum, EDbColLongText);
	TDbCol names(KNames, EDbColLongText);
	TDbCol mailid(KMailId, EDbColLongText);
	TDbCol mess(KMess, EDbColLongText);
	TDbCol count(KCount, EDbColInt8);

	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(album);
	colset->AddL(names);
	colset->AddL(mailid);
	colset->AddL(mess);
	colset->AddL(count);

    User::LeaveIfError(iPhotosDB.CreateTable(KFolderMails, *colset));
    CleanupStack::PopAndDestroy(colset);
}

void CPhotosDB::CreateSelectiveSmsTableL()
{	
	TDbCol names(KSmsNames, EDbColText);
	TDbCol mobiles(KSmsmobile, EDbColText);

	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(names);
	colset->AddL(mobiles);

    User::LeaveIfError(iPhotosDB.CreateTable(KSelectiveSms, *colset));
    CleanupStack::PopAndDestroy(colset);
}
TInt CPhotosDB::CheckAndSaveFolder(TDesC& aFolder)
{
	if(CheckFolder(aFolder))
		return 1;

	RDbTable table;
    TInt err = table.Open(iPhotosDB, KFolder, table.EUpdatable);
    User::LeaveIfError(err);

	table.EndL();
	
	table.InsertL();
	
    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.SetColL(1, aFolder);
    
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

//void CPhotosDB::CheckAndIncreaseStatusCount(TDesC& aFolder, TInt type)
//{
/*	RDbTable table;
//	TBuf<120> FName;
    TInt err = table.Open(iPhotosDB, KFolder, table.EUpdatable);
	TInt compareflag = 0;
    User::LeaveIfError(err);

	TInt8 photoscount=0;
	
    TInt count;	
    
	count = table.CountL();
    if (count)
    {
		for(table.FirstL(); table.AtRow();table.NextL())
        {
			table.GetL();  
			compareflag=aFolder.Compare(table.ColDes16(1));
			if(compareflag==0)
			{
				table.UpdateL();          
				//increment photos or videos count for the album
				if(type==0){
				photoscount=table.ColInt8(2);
				photoscount++;
				table.SetColL(2, photoscount);
				}
				else{
				photoscount=table.ColInt8(3);
				photoscount++;
			    table.SetColL(3, photoscount);								
				}
				compareflag=1;			
				table.PutL();
				break;
			}
			else{
				compareflag=0;
			}
        }
    }
    table.Close();
*/

//}

//void CPhotosDB::UpdatePhotosVideosCount(TDesC& aFolder, TInt aCount, TInt type)
//{
/*
	RDbTable table;
//	TBuf<120> FName;
    TInt err = table.Open(iPhotosDB, KFolder, table.EUpdatable);
	TInt compareflag = 0;
    User::LeaveIfError(err);
	
    TInt count;	
    
	count = table.CountL();
    if (count)
    {
		for(table.FirstL(); table.AtRow();table.NextL())
        {
			table.GetL();  
			compareflag=aFolder.Compare(table.ColDes16(1));
			if(compareflag==0)
			{
				table.UpdateL();          
				//increment photos or videos count for the album
				if(type==0){
//				photoscount=table.ColInt8(2);
//				photoscount++;
				table.SetColL(2, aCount);
				}
				else{
//				photoscount=table.ColInt8(3);
//				photoscount++;
			    table.SetColL(3, aCount);								
				}
				compareflag=1;			
				table.PutL();
				break;
			}
			else{
				compareflag=0;
			}
        }
    }
    table.Close();
*/
  
//}

//void CPhotosDB::AlbumShareStatus(TDesC& aFolder)
//{
/*
	RDbTable table;
//	TBuf<120> FName;
    TInt err = table.Open(iPhotosDB, KFolder, table.EUpdatable);
	TInt compareflag = 0;
    User::LeaveIfError(err);
	
    TInt count;	
    
	count = table.CountL();
    if (count)
    {
		for(table.FirstL(); table.AtRow();table.NextL())
        {
			table.GetL();            
			compareflag=aFolder.Compare(table.ColDes16(1));
			if(compareflag==0)
			{
				table.UpdateL();
				table.SetColL(4, 1);
				table.PutL();
			}else{
				compareflag=0;
			}
        }
    }
    table.Close();
*/

//}

TInt CPhotosDB::CheckFolder(TDesC& aFolder)
{

	RDbTable table;
//	TBuf<120> FName;
    TInt err = table.Open(iPhotosDB, KFolder, table.EUpdatable);
	TInt compareflag = 0;
    User::LeaveIfError(err);

    TInt count;	
    
	count = table.CountL();
    if (count)
    {
		for(table.FirstL(); table.AtRow();table.NextL())
        {
			table.GetL();            
			compareflag=aFolder.Compare(table.ColDes16(1));
			if(compareflag==0)
			{
				compareflag=1;			
				break;
			}
			else
				compareflag=0;
        }
    }
    table.Close();

	return compareflag;
}

void CPhotosDB::CreatePhotosTableL()
{
    // Specify columns for Books table
#ifdef __UIQ__
    TDbCol filepath(KFilePath, EDbColLongText);
#else
    TDbCol filepath(KFilePath, EDbColText);
#endif
    TDbCol publishtype(KPublType, EDbColInt8); //
    TDbCol folder(KFolder, EDbColText);
	TDbCol desc(KDesc, EDbColText);
    TDbCol photovideo(KPhotoVideo, EDbColInt8);
	TDbCol isNew(KIsNew, EDbColInt8);
	TDbCol status(KStatusString, EDbColLongText);

    // Add the columns to column set
    CDbColSet* colset = CDbColSet::NewLC();
    colset->AddL(filepath);
    colset->AddL(publishtype);
    colset->AddL(folder);
	colset->AddL(desc);
    colset->AddL(photovideo);
	colset->AddL(isNew);
	colset->AddL(status);

    // Create the Books table
    User::LeaveIfError(iPhotosDB.CreateTable(KPhotosTbl, *colset));
    CleanupStack::PopAndDestroy(colset);
}

void CPhotosDB::DropPhotosTableL()
{
    _LIT(KDropTable, "DROP TABLE ");

    // Sql: DROP TABLE Books
    TBuf<128> sqlStr;
    sqlStr.Append(KDropTable);
    sqlStr.Append(KPhotosTbl);

    RDbIncremental incOp;
    TInt incStep = 0xFFFF;
    // Initialise Execution
    TInt incStat = incOp.Execute(iPhotosDB, sqlStr, incStep);
    while (incStep>0 && incStat==KErrNone)
	{
        incStat = incOp.Next(incStep); // Do the work
    }
    incOp.Close();
}


TInt CPhotosDB::AddRecord(TDesC& aPhoto, TInt8 aType, TDesC& aRFolder, TDesC& aDesc, TInt8 aPhotoVideo, TInt8 aNew, TDesC& aStatus)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KPhotosTbl, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.Reset();
    table.InsertL();
#ifdef __UIQ__
    // Use the stream 
    RDbColWriteStream out;
    out.OpenLC(table, 1);
    out.WriteL(aPhoto);
    out.Close();
    CleanupStack::PopAndDestroy(); // out
#else
    table.SetColL(1, aPhoto);
#endif
    table.SetColL(2, aType);
    table.SetColL(3, aRFolder);
	table.SetColL(4, aDesc);
    table.SetColL(5, aPhotoVideo);
	table.SetColL(6, aNew);
	
    RDbColWriteStream out1;
    out1.OpenLC(table, 7);
    out1.WriteL(aStatus);
    out1.Close();
    CleanupStack::PopAndDestroy(); // out


    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

TInt CPhotosDB::GetRecord(TDesC& aPhoto, TInt8& aType, TDesC& aRFolder, TDesC& aDesc, TInt8& aPhotoVideo, TInt8& aNew, TDesC& aStatus)
{/*
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KPhotosTbl, table.EUpdatable);
    User::LeaveIfError(err);

    if (!table.IsEmptyL())
    {
      table.FirstL();
      TPtr& photo = (TPtr&) aPhoto;
      TPtr& folder = (TPtr&) aRFolder;
	  TPtr& desc = (TPtr&) aDesc;
	  TPtr& status = (TPtr&) aStatus;

      table.GetL();
#ifdef  __UIQ__
      //photo.Copy(table.ColDes16(1));      
      RDbColReadStream in;
	  in.OpenLC(table, 1);
	  in.ReadL(photo, table.ColLength(1));
	  in.Close();
	  CleanupStack::PopAndDestroy(); // in  
#else
      photo.Copy(table.ColDes(1));
#endif
      aType = table.ColInt8(2);
      folder.Copy(table.ColDes(3));
	  desc.Copy(table.ColDes(4));
      aPhotoVideo = table.ColInt8(5);
	  aNew = table.ColInt8(6);

      RDbColReadStream in1;
	  in1.OpenLC(table, 7);
	  in1.ReadL(status, table.ColLength(7));
	  in1.Close();
	  CleanupStack::PopAndDestroy(); // in  

      err = 0;
    }
    else
    {
        err = -1;
    }
    table.Close();

    return err;
	*/

    RDbTable table;
    TInt err = table.Open(iPhotosDB, KPhotosTbl, table.EReadOnly);
    User::LeaveIfError(err);

	  TPtr& photo = (TPtr&) aPhoto;
	  TPtr& folder = (TPtr&) aRFolder;
	  TPtr& desc = (TPtr&) aDesc;
	  TPtr& status = (TPtr&) aStatus;

	  err = -1;

//	while(table.NextL())
    if (!table.IsEmptyL())
    {
      table.FirstL();
      table.GetL();
#ifdef  __UIQ__
      //photo.Copy(table.ColDes16(1));      
      RDbColReadStream in;
	  in.OpenLC(table, 1);
	  in.ReadL(photo, table.ColLength(1));
	  in.Close();
	  CleanupStack::PopAndDestroy(); // in  
#else
      photo.Copy(table.ColDes(1));
#endif
      aType = table.ColInt8(2);
      folder.Copy(table.ColDes(3));
	  desc.Copy(table.ColDes(4));
      aPhotoVideo = table.ColInt8(5);
	  aNew = table.ColInt8(6);

	  //sent.Copy(table.ColDes(7));
      RDbColReadStream in1;
	  in1.OpenLC(table, 7);
	  in1.ReadL(status, table.ColLength(7));
	  in1.Close();
	  CleanupStack::PopAndDestroy(); // in  

	  if(status.Find(_L("0")) != KErrNotFound){
		err = 0;
		//break;
	  }
    }
	
    //else
    //{
        //err = -1;
    //}
    table.Close();

    return err;

}

TInt CPhotosDB::DeleteFirstRecord()
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KPhotosTbl, table.EUpdatable);
    //User::LeaveIfError(err);

    if (err == 0)
    {/*
        if (table.FirstL())
            table.DeleteL();
        else
            err = -1;
	 */

	//while(table.NextL())
    if (!table.IsEmptyL())
    {
      table.FirstL();
      table.GetL();

      TBuf<1024> status;

      RDbColReadStream in1;
	  in1.OpenLC(table, 7);
	  in1.ReadL(status, table.ColLength(7));
	  in1.Close();
	  CleanupStack::PopAndDestroy(); // in1  

	  if(status.Find(_L("0")) != KErrNotFound){
		status[status.Find(_L("0"))] = '1';
	
		if(status.Find(_L("0")) != KErrNotFound){
			table.UpdateL();

			RDbColWriteStream out1;
			out1.OpenLC(table, 7);
			out1.WriteL(status);
			out1.Close();
			CleanupStack::PopAndDestroy(); // out1

			table.PutL();
		}
		else
		{
			table.DeleteL();
			err = -111;
		}
		//break;
	  }
    }
        table.Close();
    }
	iPhotosDB.Compact();
    return err;
}

TInt CPhotosDB::DeleteNonExistantRecord()
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KPhotosTbl, table.EUpdatable);
    //User::LeaveIfError(err);

    if (err == 0)
    {
        if (table.FirstL())
            table.DeleteL();
        else
            err = -1;

        table.Close();
    }

	iPhotosDB.Compact();

    return err;
}

TInt CPhotosDB::DeleteSharingsFirstRecord()
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KFolderMails, table.EUpdatable);
    //User::LeaveIfError(err);

    if (err == 0)
    {
        if (table.FirstL())
            table.DeleteL();
        else
            err = -1;

        table.Close();
    }
    return err;
}

TInt CPhotosDB::GetPublishType()
{
	TInt aPhotoVideo=-1;
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KPhotosTbl, table.EReadOnly);
    //User::LeaveIfError(err);

    if (err == 0)
    {
        if (table.FirstL())
		{
			table.GetL();
			aPhotoVideo = table.ColInt8(5);
		}            
        else
            aPhotoVideo = -1;

        table.Close();
    }
    return aPhotoVideo;
}

TInt CPhotosDB::PhotosRowCount()
{
    RDbTable table;
    TInt count = 0;
    TInt err = table.Open(iPhotosDB, KPhotosTbl, table.EReadOnly);
/*    if (err == 0)
    {
        if (!table.IsEmptyL())
			count = 1; //CountL(EEnsure);
        table.Close();
    }*/
    if (err == 0)
    {

	while(table.NextL())
    {
      //table.FirstL();
      table.GetL();

	  if(table.ColInt8(5) == 0)
		count++;
    }
        table.Close();
    }
    return count;
}

TInt CPhotosDB::VideosRowCount()
{
    RDbTable table;
    TInt count = 0;
    TInt err = table.Open(iPhotosDB, KPhotosTbl, table.EReadOnly);
/*    if (err == 0)
    {
        if (!table.IsEmptyL())
			count = 1; //CountL(EEnsure);
        table.Close();
    }*/
    if (err == 0)
    {

	while(table.NextL())
    {
      //table.FirstL();
      table.GetL();

	  if(table.ColInt8(5) == 1)
		count++;
    }
        table.Close();
    }
    return count;
}

TInt CPhotosDB::RowCount()
{
    RDbTable table;
    TInt count = 0;
    TInt err = table.Open(iPhotosDB, KPhotosTbl, table.EReadOnly);
/*    if (err == 0)
    {
        if (!table.IsEmptyL())
			count = 1; //CountL(EEnsure);
        table.Close();
    }*/
    if (err == 0)
    {

	while(table.NextL())
    {
      //table.FirstL();
      table.GetL();

      TBuf<1024> status;

      RDbColReadStream in1;
	  in1.OpenLC(table, 7);
	  in1.ReadL(status, table.ColLength(7));
	  in1.Close();
	  CleanupStack::PopAndDestroy(); // in1  

	  if(status.Find(_L("0")) != KErrNotFound){		
		count=1;
	  }
    }
        table.Close();
    }
    return count;
}

TInt CPhotosDB::RowCountForSharing()
{
    RDbTable table;
    TInt count = 0;
    TInt err = table.Open(iPhotosDB, KFolderMails, table.EReadOnly);
    if (err == 0)
    {
        if (!table.IsEmptyL())
			count = 1; //CountL(EEnsure);
        table.Close();
    }
    return count;
}

CDesCArray* CPhotosDB::GetFolderList()
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KFolder, table.EReadOnly);
    User::LeaveIfError(err);

    TInt count;
    CDesCArray *array = NULL;

    count = table.CountL();
    if (count)
    {
        array = new (ELeave) CDesCArrayFlat(count);

        for(table.FirstL(); table.AtRow();table.NextL())
        {
			table.GetL();
            array->AppendL(table.ColDes16(1));
        }
    }
    table.Close();
    return array;
}

void CPhotosDB::GetMode(TInt8& aMode)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aMode = table.ColInt8(3);
      err = 0;
    }
    else
    {
        err = -1;
    }
    table.Close();
}
TBool CPhotosDB::LoadIpFromConfigFile(TDes& aIP)
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
				/*TInt readErr = */ file.Read(buf, 1);
				if (buf.Length())
					count++;

				buf16.Copy(buf);
				line.Append(buf16);

				if (buf.Length() == 0 || buf.Find(_L8("\n")) != KErrNotFound || buf.Find(_L8("\r")) != KErrNotFound) {
					if (line.Find(_L("server=")) != KErrNotFound && line.Length() > 7) {
						line.Delete(0,7);//Copy(&line[7]);
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

TBool CPhotosDB::LoadPortFromConfigFile(TUint16& aPort)
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
				/*TInt readErr = */ file.Read(buf, 1);
				if (buf.Length())
					count++;

				buf16.Copy(buf);
				line.Append(buf16);

				if (buf.Length() == 0 || buf.Find(_L8("\n")) != KErrNotFound || buf.Find(_L8("\r")) != KErrNotFound) {
					if (line.Find(_L("port=")) != KErrNotFound && line.Length() > 5) {
						line.Delete(0,5);//Copy(&line[5]);
						//line.SetLength(count - 5);
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

TInt CPhotosDB::SaveMailsToDB(TDesC& aFolder, TDesC& aNames, TDesC& aMails, TDesC& aMess, TInt8 aCount)
{
	RDbTable table;
    TInt err = table.Open(iPhotosDB, KFolderMails, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.Reset();
    table.InsertL();

	RDbColWriteStream out;
	
	out.OpenLC(table, 1);
	out.WriteL(aFolder);
	out.Close();
	CleanupStack::PopAndDestroy();

	out.OpenLC(table, 2);
	out.WriteL(aNames);
	out.Close();
	CleanupStack::PopAndDestroy();
	
	out.OpenLC(table, 3);
	out.WriteL(aMails);
	out.Close();				
	CleanupStack::PopAndDestroy();

	out.OpenLC(table, 4);
	out.WriteL(aMess);
	out.Close();				
	CleanupStack::PopAndDestroy();

	table.SetColL(5, aCount);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();   

	return KErrNone;
}

void CPhotosDB::GetMailsFromDB(TDesC& aFolder, TDesC& aNames, TDesC& aMails, TDesC& aMess, TInt8& aCount)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KFolderMails, table.EReadOnly);
    User::LeaveIfError(err);

    if (!table.IsEmptyL())
    {
      table.FirstL();
	  TPtr& folder = (TPtr&) aFolder;
      TPtr& names = (TPtr&) aNames;
	  TPtr& mails = (TPtr&) aMails;
	  TPtr& mess  = (TPtr&) aMess;

      table.GetL();

	  RDbColReadStream in;

	  in.OpenLC(table, 1);
  	  in.ReadL(folder, table.ColLength(1));
	  in.Close();
	  CleanupStack::PopAndDestroy();

	  in.OpenLC(table, 2);
  	  in.ReadL(names, table.ColLength(2));
	  in.Close();
	  CleanupStack::PopAndDestroy();

	  in.OpenLC(table, 3);
	  in.ReadL(mails, table.ColLength(3));
	  in.Close();				
	  CleanupStack::PopAndDestroy();

	  in.OpenLC(table, 4);
	  in.ReadL(mess, table.ColLength(4));
	  in.Close();				
	  CleanupStack::PopAndDestroy();

	  aCount = table.ColInt8(5);

      err = 0;
    }
    else
    {
        err = -1;
    }
    table.Close();
}

void CPhotosDB::SaveChoice(TInt8 aChoice)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

    table.SetColL(8, aChoice);
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();
}

void CPhotosDB::GetChoice(TInt8& aChoice)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aChoice = table.ColInt8(8);
      err = 0;
    }
    else
    {
        err = -1;
    }
    table.Close();
}

TInt CPhotosDB::SaveUploadSettingsL(TInt aPhoto, TInt aVideo, TInt aAlbumShares)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KStatusCount, table.EUpdatable);
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
  
      table.SetColL(1, aPhoto);
      table.SetColL(2, aVideo);
      table.SetColL(3, aAlbumShares);
  	
      CleanupStack::PopAndDestroy(colset);
  
      table.PutL();
      table.Close();
  
      return KErrNone;
  }

void CPhotosDB::GetUploadSettings(TInt& aAlbum, TInt& aPhotoShares, TInt& aVideoShares, TInt& aAlbumShares)
{
	RDbTable table;
//	TBuf<120> FName;
    TInt err = table.Open(iPhotosDB, KFolder, table.EReadOnly);
//	TInt compareflag = 0;
    User::LeaveIfError(err);
	
    TInt count;	
    
	count = table.CountL();
	
	table.Close();	

	aAlbum=count;
	aAlbumShares=0;
	aPhotoShares=0;
	aVideoShares=0;

    RDbTable table1;
    err = table1.Open(iPhotosDB, KStatusCount, table.EUpdatable);
    User::LeaveIfError(err);
	
    if (table1.CountL())
    {
      table1.FirstL();
      table1.GetL();

      aPhotoShares = table1.ColInt16(1);
	  aVideoShares = table1.ColInt16(2);
	  aAlbumShares = table1.ColInt16(3);

      err = 0;
    }
    table1.Close();   
}

TInt CPhotosDB::CheckAndDeleteFolder(TDesC& aFolder)
{
	RDbTable table;
    TInt err = table.Open(iPhotosDB, KFolder, table.EUpdatable);
	TInt compareflag = 0;
    User::LeaveIfError(err);

    TInt count;	
    
	count = table.CountL();
    if (count)
    {
		for(table.FirstL(); table.AtRow();table.NextL())
        {
			table.GetL();            
			compareflag=aFolder.Compare(table.ColDes16(1));
			if(compareflag==0)
			{				
				table.DeleteL();
				break;
			}			
        }
    }
    table.Close();

	iPhotosDB.Compact();

	return KErrNone;
}

TInt CPhotosDB::CheckAndEditFolder(TDesC& aOldFolder, TDesC& aNewFolder)
{
	RDbTable table;
    TInt err = table.Open(iPhotosDB, KFolder, table.EUpdatable);
	TInt compareflag = 0;
    User::LeaveIfError(err);

    TInt count;	
    
	count = table.CountL();
    if (count)
    {
		for(table.FirstL(); table.AtRow();table.NextL())
        {
			table.GetL();            			
			compareflag=aOldFolder.Compare(table.ColDes16(1));
			if(compareflag==0)
			{				
				table.UpdateL();

				CDbColSet* colset = table.ColSetL();
				CleanupStack::PushL(colset);

				table.SetColL(1, aNewFolder);
    
				CleanupStack::PopAndDestroy(colset);			

				table.PutL();
				break;
			}			
        }
    }
    table.Close();

	return KErrNone;
}


TInt CPhotosDB::SaveSelectiveSmsToDB(TDesC& aNames, TDesC& aMobiles)
{
	RDbTable table;
    TInt err = table.Open(iPhotosDB, KSelectiveSms, table.EUpdatable);
    User::LeaveIfError(err);

	table.EndL();
	
	table.InsertL();
	
    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    table.SetColL(1, aNames);
	table.SetColL(2, aMobiles);
    
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

CDesCArray* CPhotosDB::GetSmsNameList()
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSelectiveSms, table.EReadOnly);
    User::LeaveIfError(err);

    TInt count;
    CDesCArray *smsname = NULL;

    count = table.CountL();
    if (count)
    {
        smsname = new (ELeave) CDesCArrayFlat(count);

        for(table.FirstL(); table.AtRow();table.NextL())
        {
			table.GetL();
            smsname->AppendL(table.ColDes16(1));
        }
    }
	else
	{
		smsname = new (ELeave) CDesCArrayFlat(1);
	}
    table.Close();
    return smsname;
}

CDesCArray* CPhotosDB::GetSmsMobileList()
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSelectiveSms, table.EReadOnly);
    User::LeaveIfError(err);

    TInt count;
    CDesCArray *smsmobile = NULL;

    count = table.CountL();
    if (count)
    {
        smsmobile = new (ELeave) CDesCArrayFlat(count);

        for(table.FirstL(); table.AtRow();table.NextL())
        {
			table.GetL();
            smsmobile->AppendL(table.ColDes16(2));
        }
    }
	else
	{
		smsmobile = new (ELeave) CDesCArrayFlat(1);
	}
    table.Close();
    return smsmobile;
}

TInt CPhotosDB::DeleteContactsForSmsFromDB(TInt pos)
{
	RDbTable table;
    TInt err = table.Open(iPhotosDB, KSelectiveSms, table.EUpdatable);
	
    User::LeaveIfError(err);

    TInt count;	
    
	count = table.CountL();
    if (count)
    {
		count=0;
		for(table.FirstL(); table.AtRow();table.NextL())
        {
			table.GetL();            			
			if(count == pos)
			{				
				table.DeleteL();
				break;
			}			
			count++;
        }
    }
    table.Close();

	iPhotosDB.Compact();

	return KErrNone;
}

void CPhotosDB::GetSmsTimestampFromDB(TTime& aTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aTime = table.ColTime(11);
      err = 0;
    }
    else
    {
		aTime = 0;
        err = -1;
    }
    table.Close();
}

/*void CPhotosDB::UpdateSmsTimestampToDB()
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}
	TTime aTime;
	aTime.HomeTime();

    table.SetColL(11, aTime);
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();
}
*/
void CPhotosDB::UpdateSmsTimestampToDB(TTime aTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

    table.SetColL(11, aTime);
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();
}

TInt CPhotosDB::SaveSmsCountL(TInt aCount)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EUpdatable);
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

    table.SetColL(13, aCount);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

void CPhotosDB::GetSmsCountL(TInt& aCount)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EReadOnly);
    User::LeaveIfError(err);
	
    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aCount = table.ColInt16(13);
      err = 0;
    }
    table.Close();
}
void CPhotosDB::GetSubStatusSyncTimeFromDB(TDesC& aTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSubStatus, table.EReadOnly);
    User::LeaveIfError(err);

	TPtr& time = (TPtr&) aTime;

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      time.Copy(table.ColDes16(1));
      err = 0;
    }
    else
    {
		//TDateTime janNineteenSeventy(1970,EJanuary,0,00,00,00,000000);
		//TTime timejan(janNineteenSeventy);
		//aTime = timejan;				
		time.Copy(_L("0"));
        err = -1;
    }
    table.Close();	
}

void CPhotosDB::SaveSubStatusSyncTimeToDB(TDesC& aTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSubStatus, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

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
    
	table.SetColL(1, aTime);
    
	CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();
}

TInt CPhotosDB::SaveUserSettingsL(TDesC& aUserName, TDesC& aPassWord, TDesC& aMobileNo)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KStatusCount, table.EUpdatable);
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
  
      table.SetColL(4, aUserName);
	  table.SetColL(5, aPassWord);
	  table.SetColL(6, aMobileNo);
      
      CleanupStack::PopAndDestroy(colset);
  
      table.PutL();
      table.Close();
  
      return KErrNone;
  }

void CPhotosDB::GetUserSettings(TDes& aUserName, TDes& aPassWord, TDes& aMobileNo)
{
	RDbTable table;
    TInt err = table.Open(iPhotosDB, KStatusCount, table.EReadOnly);
    User::LeaveIfError(err);
	
    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aUserName = table.ColDes16(4);
	  aPassWord = table.ColDes16(5);
	  aMobileNo = table.ColDes16(6);

      err = 0;
    }
    table.Close();
}



void CPhotosDB::GetNetworkErrorCode(TInt& aCode)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KStatusDisplayTable, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aCode = table.ColInt16(2);
      err = 0;
    }
    else
    {
        err = -1;
    }
    table.Close();
}

void CPhotosDB::GetNetworkStatusCode(TInt& aCode)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KStatusDisplayTable, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aCode = table.ColInt16(1);
      err = 0;
    }
    else
    {
        err = -1;
    }
    table.Close();
}

void CPhotosDB::SaveNetworkErrorCode(TInt aCode)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KStatusDisplayTable, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

    table.SetColL(2, aCode);
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

	iPhotosDB.Compact();
}

void CPhotosDB::SaveNetworkStatusCode(TInt aCode)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KStatusDisplayTable, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

    table.SetColL(1, aCode);
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

	iPhotosDB.Compact();
}

TInt CPhotosDB::CheckIfPhotoExists(TDesC& aPhoto)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KPhotosTbl, table.EReadOnly);
    User::LeaveIfError(err);

	TPtr& photo = (TPtr&) aPhoto;

	  err = 0;

	while(table.NextL())
    //if (!table.IsEmptyL())
    {
      //table.FirstL();
      table.GetL();
#ifdef  __UIQ__
      //photo.Copy(table.ColDes16(1));
	  TBuf<256> filename; 
      RDbColReadStream in;
	  in.OpenLC(table, 1);
	  in.ReadL(filename, table.ColLength(1));
	  in.Close();
	  CleanupStack::PopAndDestroy(); // in 

	  if(filename.Compare(aPhoto) == 0)
	  {
		  err = 1;
		  break;
	  }
#else
      if(photo.Compare(table.ColDes(1)) == 0)
	  {
		  err = 1;
		  break;
	  }
#endif

    }
	
    table.Close();

    return err;

}

void CPhotosDB::SaveUpgradeStatus(TInt8 aChoice, TDesC& aVersion)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KStatusDisplayTable, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

    table.SetColL(3, aChoice);
	table.SetColL(4, aVersion);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();
}

void CPhotosDB::GetUpgradeStatus(TInt8& aChoice, TDes& aVersion)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KStatusDisplayTable, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aChoice = table.ColInt8(3);
	  aVersion = table.ColDes16(4);
      err = 0;
    }
    else
    {
        err = -1;
    }
    table.Close();
}

void CPhotosDB::SaveApnID(TUint32 aId)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KStatusDisplayTable, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

    table.SetColL(5, aId);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();
}

void CPhotosDB::GetApnID(TUint32& aId)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KStatusDisplayTable, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aId = table.ColUint32(5);
      err = 0;
    }
    else
    {
        err = -1;
    }
    table.Close();
}

void CPhotosDB::CreateSmsTimestampTableL()//for sentitems
{
	TDbCol smstimestamp(KSmsSentItemsTimestamp, EDbColDateTime);	
	TDbCol backuptime(KLastBackupTime, EDbColDateTime);	

	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(smstimestamp);
	colset->AddL(backuptime);

    User::LeaveIfError(iPhotosDB.CreateTable(KSmsTimeStampTable, *colset));
    CleanupStack::PopAndDestroy(colset);	
}

void CPhotosDB::UpdateSentItemsSmsTimestampToDB(TTime aTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSmsTimeStampTable, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

    table.SetColL(1, aTime);
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();
}

void CPhotosDB::GetSentItemsSmsTimestampFromDB(TTime& aTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSmsTimeStampTable, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aTime = table.ColTime(1);
      err = 0;
    }
    else
    {
		aTime = 0;
        err = -1;
    }
    table.Close();
}

void CPhotosDB::UpdateBackupTimestampToDB(TTime aTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSmsTimeStampTable, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

    table.SetColL(2, aTime);
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();
}

void CPhotosDB::GetBackupTimestampFromDB(TTime& aTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSmsTimeStampTable, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

      aTime = table.ColTime(2);
      err = 0;
    }
    else
    {
		aTime = 0;
        err = -1;
    }
    table.Close();
}

void CPhotosDB::CreateMediaUploadFileTableL()
{
	TDbCol folder(KMediaUploadFolder, EDbColLongText);
	
	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(folder);
		
    User::LeaveIfError(iPhotosDB.CreateTable(KMediaUploadFolder, *colset));
    CleanupStack::PopAndDestroy(colset);
}

TInt CPhotosDB::CheckUploadedMediaFileName(TDesC& aFilename)
{
	RDbTable table;
    TInt err = table.Open(iPhotosDB, KMediaUploadFolder, table.EUpdatable);
	TInt compareflag = 1;
    User::LeaveIfError(err);

    TInt count;	
    
	TBuf<128> filename;
	filename.Copy(aFilename);       
			
	TPtr& photo = (TPtr&) aFilename;

	count = table.CountL();
    if (count)
    {
		for(table.FirstL(); table.AtRow();table.NextL())
        {
			table.GetL();  

			RDbColReadStream in;
			in.OpenLC(table, 1);
			in.ReadL(photo, table.ColLength(1));
			in.Close();
			CleanupStack::PopAndDestroy(); // in 

			compareflag = filename.Compare(photo);

			if(compareflag == 0)
			{
				compareflag = 2;			
				break;
			}
			else
				compareflag = 1;
        }
    }
    table.Close();

	return compareflag;
}

TInt CPhotosDB::CheckAndSaveMediaFile(TDesC& aFilename)
{
	TBuf<128> filename;
	filename.Copy(aFilename);

	if(CheckUploadedMediaFileName(aFilename) == 2)
		return 1;

	RDbTable table;
    TInt err = table.Open(iPhotosDB, KMediaUploadFolder, table.EUpdatable);
    User::LeaveIfError(err);

	table.EndL();
	
	table.InsertL();
	
    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    //table.SetColL(1, aFilename);
	RDbColWriteStream out;
    out.OpenLC(table, 1);
    out.WriteL(filename);
    out.Close();
    CleanupStack::PopAndDestroy(); // out
    
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

void CPhotosDB::GetMediaFileName(TDesC& aFilename)
{
	RDbTable table;
    TInt err = table.Open(iPhotosDB, KPhotosTbl, table.EUpdatable);
    User::LeaveIfError(err);

	TPtr& photo = (TPtr&) aFilename;
	
	if (!table.IsEmptyL())
    {
      table.FirstL();
      table.GetL();

#ifdef  __UIQ__
      RDbColReadStream in;
	  in.OpenLC(table, 1);
	  in.ReadL(photo, table.ColLength(1));
	  in.Close();
	  CleanupStack::PopAndDestroy(); // in  
#else
      photo.Copy(table.ColDes(1));
#endif
	}
	
    table.Close();
}

TInt CPhotosDB::CheckUploadingMediaFileName(TDesC& aFilename)
{
	RDbTable table;
    TInt err = table.Open(iPhotosDB, KPhotosTbl, table.EUpdatable);
	TInt compareflag = 1;
    User::LeaveIfError(err);

    TInt count;	
    
	TBuf<128> filename;
	filename.Copy(aFilename);       
			
	TPtr& photo = (TPtr&) aFilename;

	count = table.CountL();
    if (count)
    {
		for(table.FirstL(); table.AtRow();table.NextL())
        {
			table.GetL();  

#ifdef  __UIQ__
		  RDbColReadStream in;
		  in.OpenLC(table, 1);
		  in.ReadL(photo, table.ColLength(1));
		  in.Close();
		  CleanupStack::PopAndDestroy(); // in  
#else
	      photo.Copy(table.ColDes(1));
#endif

			compareflag = filename.Compare(photo);

			if(compareflag == 0)
			{
				compareflag = 3;			
				break;
			}
			else
				compareflag = 1;
        }
    }
    table.Close();

	return compareflag;
}

_LIT(KSettingsTimeStampTable, "SettingsTimeStampTable"); // table name
_LIT(KContactsSettingsOldTimestamp, "ContactsSettingsOldTimestamp"); // 
_LIT(KContactsSettingsNewTimestamp, "ContactsSettingsNewTimestamp"); // 
_LIT(KPublishSettingsOldTimestamp, "PublishSettingsOldTimestamp"); // 
_LIT(KPublishSettingsNewTimestamp, "PublishSettingsNewTimestamp"); // 
_LIT(KSmsSettingsOldTimestamp, "SmsSettingsOldTimestamp"); // 
_LIT(KSmsSettingsNewTimestamp, "SmsSettingsNewTimestamp"); // 
_LIT(KGeneralSettingsOldTimestamp, "GeneralSettingsOldTimestamp"); // 
_LIT(KGeneralSettingsNewTimestamp, "GeneralSettingsNewTimestamp"); // 
_LIT(KSecureSettingsOldTimestamp, "SecureSettingsOldTimestamp"); // 
_LIT(KSecureSettingsNewTimestamp, "SecureSettingsNewTimestamp"); // 

void CPhotosDB::CreateSettingsTimestampTableL()//for settings
{
	TDbCol contstsold(KContactsSettingsOldTimestamp, EDbColDateTime);
	TDbCol contstsnew(KContactsSettingsNewTimestamp, EDbColDateTime);	
	TDbCol pubstsold(KPublishSettingsOldTimestamp, EDbColDateTime);	
	TDbCol pubstsnew(KPublishSettingsNewTimestamp, EDbColDateTime);	
	TDbCol smsstsold(KSmsSettingsOldTimestamp, EDbColDateTime);	
	TDbCol smsstsnew(KSmsSettingsNewTimestamp, EDbColDateTime);	
	TDbCol genstsold(KGeneralSettingsOldTimestamp, EDbColDateTime);	
	TDbCol genstsnew(KGeneralSettingsNewTimestamp, EDbColDateTime);	
	TDbCol securestsold(KSecureSettingsOldTimestamp, EDbColDateTime);	
	TDbCol securestsnew(KSecureSettingsNewTimestamp, EDbColDateTime);	

	CDbColSet* colset = CDbColSet::NewLC();

	colset->AddL(contstsold);
	colset->AddL(contstsnew);
	colset->AddL(pubstsold);
	colset->AddL(pubstsnew);
	colset->AddL(smsstsold);
	colset->AddL(smsstsnew);
	colset->AddL(genstsold);
	colset->AddL(genstsnew);
	colset->AddL(securestsold);
	colset->AddL(securestsnew);

    User::LeaveIfError(iPhotosDB.CreateTable(KSettingsTimeStampTable, *colset));
    CleanupStack::PopAndDestroy(colset);	
}

void CPhotosDB::UpdateContactsSettingsTimestampToDB(TTime aOldTime, TTime aNewTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettingsTimeStampTable, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

	table.SetColL(1, aOldTime);
    table.SetColL(2, aNewTime);
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();
}

void CPhotosDB::GetContactsSettingsTimestampFromDB(TTime& aOldTime, TTime& aNewTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettingsTimeStampTable, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

	  aOldTime = table.ColTime(1);
      aNewTime = table.ColTime(2);
      err = 0;
    }
    else
    {
		aOldTime = 0;
		aNewTime = 0;
        err = -1;
    }
    table.Close();
}

void CPhotosDB::UpdatePublishSettingsTimestampToDB(TTime aOldTime, TTime aNewTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettingsTimeStampTable, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

	table.SetColL(3, aOldTime);
    table.SetColL(4, aNewTime);
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();
}

void CPhotosDB::GetPublishSettingsTimestampFromDB(TTime& aOldTime, TTime& aNewTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettingsTimeStampTable, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

	  aOldTime = table.ColTime(3);
      aNewTime = table.ColTime(4);
      err = 0;
    }
    else
    {
		aOldTime = 0;
		aNewTime = 0;
        err = -1;
    }
    table.Close();
}

void CPhotosDB::UpdateSmsSettingsTimestampToDB(TTime aOldTime, TTime aNewTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettingsTimeStampTable, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

	table.SetColL(5, aOldTime);
    table.SetColL(6, aNewTime);
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();
}

void CPhotosDB::GetSmsSettingsTimestampFromDB(TTime& aOldTime, TTime& aNewTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettingsTimeStampTable, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

	  aOldTime = table.ColTime(5);
      aNewTime = table.ColTime(6);
      err = 0;
    }
    else
    {
		aOldTime = 0;
		aNewTime = 0;
        err = -1;
    }
    table.Close();
}

void CPhotosDB::UpdateGeneralSettingsTimestampToDB(TTime aOldTime, TTime aNewTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettingsTimeStampTable, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

	table.SetColL(7, aOldTime);
    table.SetColL(8, aNewTime);
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();
}

void CPhotosDB::GetGeneralSettingsTimestampFromDB(TTime& aOldTime, TTime& aNewTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettingsTimeStampTable, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

	  aOldTime = table.ColTime(7);
      aNewTime = table.ColTime(8);
      err = 0;
    }
    else
    {
		aOldTime = 0;
		aNewTime = 0;
        err = -1;
    }
    table.Close();
}

void CPhotosDB::UpdateSecureSettingsTimestampToDB(TTime aOldTime, TTime aNewTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettingsTimeStampTable, table.EUpdatable);
    User::LeaveIfError(err);

    CDbColSet* colset = table.ColSetL();
    CleanupStack::PushL(colset);

    if (table.CountL() == 0)
    {
		table.InsertL();
	}
	else
	{		
		table.FirstL();
        table.UpdateL();
	}

	table.SetColL(9, aOldTime);
    table.SetColL(10, aNewTime);
    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();
}

void CPhotosDB::GetSecureSettingsTimestampFromDB(TTime& aOldTime, TTime& aNewTime)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettingsTimeStampTable, table.EReadOnly);
    User::LeaveIfError(err);

    if (table.CountL())
    {
      table.FirstL();
      table.GetL();

	  aOldTime = table.ColTime(9);
      aNewTime = table.ColTime(10);
      err = 0;
    }
    else
    {
		aOldTime = 0;
		aNewTime = 0;
        err = -1;
    }
    table.Close();
}

TInt CPhotosDB::SavePhotoSettingL(TInt8 aMode)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EUpdatable);
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

//    table.SetColL(1, aIP);
//    table.SetColL(2, (TInt)aPort);
    table.SetColL(3, aMode);
//	table.SetColL(7, aAutostart);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

TInt CPhotosDB::SaveGeneralSettingL(TInt8 aAuto)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EUpdatable);
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

	table.SetColL(7, aAuto);
//    table.SetColL(3, aMode);
//	table.SetColL(4, aFolder);
//	table.SetColL(5, aDesc);
//	table.SetColL(6, aRoam);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}

TInt CPhotosDB::SaveSmsSettingL(TInt8 aSmsMode)
{
    RDbTable table;
    TInt err = table.Open(iPhotosDB, KSettings, table.EUpdatable);
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

//  table.SetColL(1, aIP);
//  table.SetColL(2, (TInt)aPort);
//  table.SetColL(3, aMode);
//	table.SetColL(4, aFolder);
//	table.SetColL(5, aDesc);
//	table.SetColL(6, aRoam);
//	table.SetColL(7, aAutostart);
	table.SetColL(9, aSmsMode);

    CleanupStack::PopAndDestroy(colset);

    table.PutL();
    table.Close();

    return KErrNone;
}
