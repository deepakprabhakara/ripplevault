#ifndef __RIPPLEVAULTAPPUI_H__
#define __RIPPLEVAULTAPPUI_H__
#ifdef __UIQ__
#ifdef __S80__
	#include <eikappui.h>
	#include <e32std.h>
	#include <eikappui.h> 
	#include <apparc.h>
	#include <bautils.h>
	#include <coecntrl.h>

	#include <eikdialg.h>
	#include <eikchlst.h>
	#include <eikrted.h>
	#include <cknflopr.h>
	#include <eikbtgpc.h>
#else
	#include <cntdb.h>
	#include <coeccntx.h>
	#include <eikenv.h>
	#include <qikappui.h>
	#include <eikappui.h>
	#include <eikon.hrh>
	#include <eikdialg.h> 
	#include <eikrted.h>
	#include <eikprogi.h>
	//#include "PhotoListview_uiq.h"
	//#include "VideoListview_uiq.h"
	#include <msvapi.h>
#endif
#else
	#include <aknviewappui.h>
	#include <aknwaitdialog.h> 
#endif

#include <apgtask.h>
#include <d32dbms.h>
#include <etel.h>
#ifndef __S80__
#include <etelbgsm.h>
#endif
#include <in_sock.h>
#include <sendas.h>
#include <mtmuids.h>
#include <txtrich.h>
#include <smut.h>
#include <msvuids.h>

#include "DBEngine.h"
#include "constants.h"

class CPhotosTimer;
class CContactsTimer;
//class CSmsTimer;
class CSmsBackupTimer;
class CWriteSmsAO;
//class CSmsBackupAO;
class CPhotosEngine;
class CVideosEngine;
class CSocketsEngineWriter;
class CDNSResolver;
//class MSendAsObserver;

//deepak
#include "ContactsHandler.h"
#include "ContactsDBEngine.h"

class CContactDatabase;
class CContactChangeNotifier;
class CSMSEngine;
class CSmsSimEngine;
//class CBillingEngine;
class CCopyContactsAO;
class CChangesContactsAO;
class CUnRegView;
class CRegView;
class CHelpView;
class CDNSResolver;
class CSmsFile;
//__________________________________

#ifdef __UIQ__
#ifdef __S80__
	class CUnRegisterView;
	//class CRegisterView;
	class CPhotoListView;
	class CSmsListView;
	class CStatusScreenView;
	class CContactsView;
	class CVideoListView;
	class CEikButtonGroupContainer;
#else
	class CUnRegisterView;
	//class CMainScreenView;
	class CPhotoScreenView;
	class CVideoScreenView;
	class CSmsScreenView;
	class CStatusScreenView;
	class CContactsScreenView;

/*class CRippleVaultAppView;
	class CExampleAppView1;
	class CEmptyView;
	class CPhotoListView;
	class CVideoListView;
	class CMyListBoxAppView;*/
#endif
#else
	class CRegisterView;
#endif

// intrusion modes 
// auto: default folder, prompt: folder select, off: disable upload
#define MODE_AUTO 0
#define MODE_PROMPT 1
#define MODE_OFF 2

#define REGISTERED 1
#define NOT_REGISTERED 0
/*! 
  @class CRippleVaultAppUi
  
  @discussion An instance of class CRippleVaultAppUi is the User Interface part of the AVKON
  application framework for the RippleVault application
  */
#ifdef __UIQ__
#ifdef __S80__
class CRippleVaultAppUi : public CEikAppUi
#else
class CRippleVaultAppUi : public CQikAppUi//,MSendAsObserver
#endif
#else
class CRippleVaultAppUi : public CAknViewAppUi//,MSendAsObserver
#endif
{
public:
    void ConstructL();
    CRippleVaultAppUi();
    ~CRippleVaultAppUi();
#ifndef __UIQ__
	TUid iPrevViewId; // to use when minimized/maximimized to load correct view
#endif

	TBool iForeground;
    
  	TBool isRegistered;
	
	CPhotosEngine *iPhotosEngine1;
    CPhotosEngine *iPhotosEngine2;
    CVideosEngine *iVideosEngine1;
	CVideosEngine *iVideosEngine2;

	CPhotosTimer *iPhotosTimer;
	CContactsTimer *iContactsTimer;
	CSmsBackupTimer *iSmsBackupTimer;
	CWriteSmsAO	*iWriteSmsAO;
	//CSmsBackupAO *iSmsBackupAO;

#ifdef __UIQ__
#ifdef __S80__
	CEikDialog* iWaitDialog;
	CPhotosEngine *iPhotosEngine3;
    CPhotosEngine *iPhotosEngine4;
    CVideosEngine *iVideosEngine3;
	CVideosEngine *iVideosEngine4;
#else
	CPhotosEngine *iPhotosEngine3;
    CPhotosEngine *iPhotosEngine4;
    CVideosEngine *iVideosEngine3;
	CVideosEngine *iVideosEngine4;
	CEikDialog* iWaitDialog;
#endif
#else
	CAknWaitDialog* iWaitDialog;
#endif

#ifdef __UIQ__
#ifdef __S80__
	//void ActivateWindow();
	void SaveSelectiveSmsMailsForS80(TDesC& aNames, TDesC& aMails);
#endif
	void ShowWindow(TInt aVal);
    void DeleteWindow(TInt aVal);
	TInt ReturnScreenValue();
#endif

	

//deepak
	CContactDatabase* ContactDataBase() { return iContactsDB; }

	CCopyContactsAO *iCopyAO;

	CChangesContactsAO *iChangesAO;
	
	//On booting, check whether changes need to be stored
	TBool ChangesToBeStoredL(TTime& aCDBTime, TTime& aRippleTime);
	//Send changes to ChangesDB
//	void SendToChangesDB(TTime& lastTime);

	void AddTimeStampToFileL(TBuf<256>* timestamp);

	void CreateInitUploadFileL();

	HBufC* GetContactsTimeStamp(TTime& aTime);

	TInt CheckUserId();
	TInt CheckLockFile();
	void CreateSecureStopFile();

	void SaveUserSettingsToDB(TDes& aUserName, TDes& aPassWord, TDes& aMobileNo);
	void GetUserSettings(TDes& aUserName, TDes& aPassWord, TDes& aMobileNo);

	void SaveContactsSettingsToDB(TDesC& aIP, TUint16 aPort);
	void SaveContactsSettingsToDB(TInt8 aRoam, TInt8 iUpload);	

	void SaveContactsSettingsToDB(TInt8 iUpload);	

    void GetContactsSettingsFromDB(TDes& aIP, TInt& aPort);
    void GetContactsSettingsFromDB(TInt& aRoam, TInt& aUpload);
    
	void SavePhotosSettingsToDB(TInt8 aMode, TInt8 aDesc, TInt8 aRoam, TInt8 aSecureGallery);    
	void GetPhotosSettingsFromDB(TInt& aMode, TInt& aDesc, TInt& aRoam, TInt& aSecureGallery);	

	void SavePhotosSettingsToDB(TInt8 aMode);    
	
	void SaveSettingsToDB(TDes& aIP, TUint16 aPort, TInt8 aAuto);//, TInt8 aMode, TInt8 aDesc, TInt8 aRoam)	
	void GetSettingsFromDB(TDes& aIP, TInt& aPort, TInt& aAuto);//, TInt& aMode, TInt& aDesc, TInt& aRoam)	
//	void DisableContactsUpload();    

	void SaveSettingsToDB(TInt8 aAuto);//, TInt8 aMode, TInt8 aDesc, TInt8 aRoam)	

	void GetChoice(TInt8& aChoice);
	void SaveChoice(TInt8 aChoice);

	void SaveSmsSettingsToDB(TInt8 aSmsMode, TTime aSmsTime, TInt8 aRoam);
	void GetSmsSettingsFromDB(TInt& aSmsMode, TTime& aSmsTime, TInt& aRoam);
	
	void SaveSmsSettingsToDB(TInt8 aSmsMode);

	void SaveConatctsTimeToDB(TInt aContactsTime, TInt aDays, TInt aSyncState);
	void GetConatctsTimeFromDB(TInt& aContactsTime, TInt& aDays, TInt& aSyncState);
	
    CContactsHandler* iContactsHandler;
    
	void SaveSecureSettingsToFile(TInt8 aSecureMode, TDes& aFriendNumber, TDes& aText, TInt8 aAlarmMode);
	void GetSecureSettingsFromFile(TInt& aSecureMode, TDes& aFriendNumber, TDes& aText, TInt& aAlarmMode);
	
	void ShowSecure();

	void EnableContactsUpload(TInt aIsRegistered);
	
	void RegisterContactsChangeHandler();
	
	void LoadIPPort();

	//void InsertSms(TDesC& message);

	//TBool CapabilityOK(TUid aCapability, TInt aResponse);
	
//	void Logger(TDes8& buf);
	
	HBufC* ReadFromResource(TInt aResourceId);
	
	void ReadUserName();

	void EditConfigFile(TDes8& aValue, TDesC8& aParam);

	void GetSettingsString();

	void SaveSettingsString();

	void ShowSelectionList(TDes& aListselection);
	
private:

	/** ContactsDB **/
	CContactDatabase* iContactsDB;

	/** Listener **/
	CContactChangeNotifier* iChangeNotifier;

	/** Sms Engine **/
	CSMSEngine* iSMSEngine;

	//CBillingEngine* iBillingEngine;
	
	//send sms
	CSmsFile* iSmsFile;
		
//_______________________

public:

	CSmsSimEngine* iSmsSimEngine;

	//CDNSResolver* iDNSResolver;

//	CPhotosEngine *GetPhotosEngine(){ return iPhotosEngine1;}

    void SetDefaultFolder(TDesC& aIntr) { iDefaultFolder.Copy(aIntr); }

	TDesC& GetDefaultFolder() { return iDefaultFolder; }

#ifdef __UIQ__
		TBool isError;
		TBool isUpgrade;

		TInt iErrCode;
		TInt iStatusCode;

		TInt iPendingCountContacts;
		TInt iPendingCountPhotos;
		TInt iPendingCountVideos;
		TInt iPendingCountSms;

		TBool isQueuedInfo;
		TBool isDoingSomething;
#endif

	int Show;
	TBuf<20> FolderName;
	TBuf<150> DescName;

	TBuf<20> iUserName;
	TBuf<30> iVersion;
	TBuf<30> iOs;
	TBuf<30> iAxiomPhoneModel;
	TBuf<30> iAxiomBillingRegType;
	TBuf<30> iUserNames;
	TBuf<30> iMobileNo;
	TBuf<30> iPassword;
	TBuf<30> iNewPassword;
//	TBuf<30> iCharge;
	TBuf<60> iRegUrl;	
	TBuf<60> iPublishUrl;
	TBuf<60> iContactsUrl;
	TBuf<60> iSharingUrl;
	TBuf<60> iSmsUrl;
//	TBuf<60> iChargeClient;
//	TBuf<60> iChargePhoto;
//	TBuf<60> iChargeVideo;
	TBuf<100> iWapUrl;
	TBuf<100> iPlanUrl;
	TBuf<100> iAlbumUrl;
	TBuf<100> iStatusUrl;
	TBuf<60> iAccessPoints;
	TBuf<30> iDecodedMobileNo;
	TBuf<30> iConnectionType;
	TBuf8<1024> iRegBuffer;

	TBuf<50> iConnectionName;
//	TInt iNewUser;
	TInt iSmsValue;/*
								flag used to define sim card change events
								-1 - MSISDN validated
								0 - No change
								1 - Validating
								2 - Cancelled
								3 - Confirm Exit
								*/
	TInt iMainScreenValue;/*
								flag used to define which screen to display
								0 - 
								1 - Contacts Screen
								2 - Photos Screen
								3 - Video Screen
								4 - SMS Screen
								*/
	TInt iSettingsScreenValue;	/*
								flag used to define which settings screen is to be shown.
								0 - General
								1 - Contacts
								2 - Publish
								3 - SMS
								*/
	TInt iSetupWizardScreenValue;
	TInt iSmsBackupType;
	TInt iAutoSmsBackupType;
	TInt iConnectionStatus; /*
							flag used to define which backup/network operation is called after the status subscription.
							1 - photo/video backup
							2 - contacts backup
							3 - sms backup
							4 - change number
							*/
	TInt iContactsFlag;
	TBuf<100> iSettingsString;
	TBuf<60> iGateWay;
	//TInt iApnCount;
	TInt iSynchronize;

	TBuf<30> iImsiNumber;
	TBuf<30> iTempMbNum;
			
	TInt8 iDescription;
	TInt8 iRoaming;	
	TInt8 iAutostart;	
	TInt8 iSecureGallery;
//	TInt iPhotoCount ;
	TInt iSharingCount;
	TInt iRipplePhotos;
	TInt iContactsCount;
	TInt iSmsCount;
	TInt iAutoSmsCount;
	TInt iBillingFlag;

	//TBuf8<6000> smsBuffer;

	TInt iSmsMode;
	TTime iSmsTime;
	TInt8 iSmsRoam;	
		
	TInt iConnection;	

	// Settings
	TInt8 iMode;	
//	TBuf<30> iDefaultFolder;	

	TInt iApnCount;
	TUint32 iApnID;
	TBuf<50> iApnConnectionName;

	TInt CheckSecureApp();
	
	TInt iContactsTimerState;

	void CheckApnCountandConnect();
	void UpdateSimcard();
	void GetContactsParams(TInt &numRecords);

	void UpLoadDatas();

	void ExitApplication();

	void GetImsiNumber();

	void ImsiCheck();

	TBool ReadImsiNumber();

	TBool ReadSecureImsiNumber();

	void WriteImsiNumber();

	void HideView();

	void UploadSms();
	
	void SaveSelectiveSmsMails(TDesC& aNames, TDesC& aMails, TInt8 aCount);

	void SaveMails(TDesC& aAlbumname, TDesC& aNames, TDesC& aMails,TDesC& aMess, TInt8 aCount);

	void GetMails(TDesC& aAlbumname,TDesC& aNames, TDesC& aMails,TDesC& aMess, TInt8& aCount);

	void SaveSmsUploadSettingsToDB(TInt aCount);

	void GetSmsUploadSettingsFromDB(TInt& aCount);

	void GetSharingAlbumXML();//TDes8& buffer);

	void GetUnSharingAlbumXML();

	void GetClientSharingAlbumXML();
	
	void GetAlbumSynchingXML();
	
	void GetSharingNames(TDes& buffer);

	void GetSharingMails(TDes& buffer);

	void SaveSubStatusSyncTime(TDesC& aTime);

	void GetSubStatusSyncTime(TDesC& aTime);

	void SendBillingSMS();

	void SendSmsForClientSharing();

	TBuf<100> iMessage;

	TBuf<1024> MailNames;

	TBuf<1024> MailId;
	
	TBuf<50> albumName;	

	TInt ChooseAccessPoint();

	//void GetConnectionName();

	TInt AccessPointSettings();

	void GetCount(TInt& photocount, TInt& videocount);

	void AddPhotos();

	void AddVideos();

	//void ShowMessage(TInt flag);

	//void ShowSetupWizard(TInt val);
	
	/*void CallSetupWizardStep1();
	void CallSetupWizardStep4();
	void CallSetupWizardStep5();
	void CallSetupWizardStep9();
	void CallSetupWizardStep11();
	void CallSetupWizardStep12();
	void CallSetupWizardStep15();
	void CallSetupWizardStep17();*/
	void CallSetupWizardUploadAll();

	TBool CheckUpgradeFile();
	void CopyUpgradeFile();
	TBool IsLaterVersion(TDes& aVersion);
	TBool GetSubVersion(TDes& aVersion, TInt& subVersion);
	void OpenBrowserForUpdate();
	void StopSecure();

	TInt CheckMediaUploadExists(TDesC& aFilename);

	void UpdateContacts(TInt mode);

	void UpdateContactsManually();

	void NetworkErrorNotification(TInt aErrorcode);

	void NetworkStatusNotification(TInt aErrorcode);

	TInt GetNetworkErrorCode(TDes& aErrorMsg);

	TInt GetNetworkStatusCode(TDes& aStatusMsg);
	//void ChoicesForUpload();

//	void CheckUpgradeFile();

//	void CopyUpgradeFile();

//	TBool IsVersionUpToDate();

//	TBool IsNetworkInProgress();

//	void VersionUpToDate();
	
//	void UpgradePublish();

//	void CheckUpgrade();

//	void DownloadClient();

//	void UpGrade();

//	TBool IsLaterVersion(TDes& aVersion);

//	TBool GetSubVersion(TDes& aVersion, TInt& subVersion);

	TInt CheckNetwork();

	void UpdateScreen();
	
	void ChangePlan();

	void OpenBrowser();

	void KillBrowser();

	void RegisterDialog();

	void NewUser();
	
	void ExistingUser();

	void ChangePassword();

	void ChangePasswordFromSettings();

	void CreateHeaderXML(TDesC& msgType);

	HBufC* GetTimeStamp(TTime& aTime);
			
    void BringToForeground();    

	void SendToBackground();
	
	TBool AutoStartFileExists();

	void WritRegisteredFile();

    void WritUserDetailsToFile();

	TInt CheckSpecialChars(TDesC& aUserName,TDesC& aPassword);

	//void CheckRegFileAndUpload();

	//TBool CheckRegFile();

	void AutoRegisterRipple();

	void ConnectRegSocket();

	TInt ListAlbums();
	
	void HandleForegroundEventL(TBool aForeground);

    TDesC& PhoneImei() { return iIMEIPhone; }

	TDesC& ServerAddr() { return  iServerAddr;}
	TUint16 ServerPort() { return  iPort;}
	
	TBuf<80> iServerAddr;

    TUint16 iPort;

	TInt iStoragePath;

	HBufC8* servlet_data;

//    void Print(TDesC& line);
//    TBool Print(TDesC& line1, TDesC& line2);
//    void Print(TInt k);
//    void PrintErr();

	void ChangeMobileNumber();

    void AddPhotoRecord(TDesC& filename, TInt8 aPublic, TDesC& aComment, TDesC& aDesc, TInt8 aNew, TInt aFileSize);
    void AddVideoRecord(TDesC& aFilename, TInt8 aPublic, TDesC& aComment, TDesC& aDesc, TInt8 aNew, TInt aFileSize);

    void ScheduleToSend();

	void CloseFile();

	void DefaultAccessPoint();

	TInt ImageFolderPath();

	void ConnectToServer();

	void ConnectToServerForChangingPassword();

	//void UpdateInboxSmsTimestamp();

	void UpdateInboxSmsTimestamp(TTime aTime);

	void GetInboxSmsTimestamp(TTime& aTime);

	void UpdateSentItemsSmsTimestamp(TTime aTime);

	void GetSentItemsSmsTimestamp(TTime& aTime);

//	void ConnectToServerForCharging();

	void ConnectToServerForSms(TInt smsbackuptype);
	
	void ConnectToServerForSharing();	

	void ConnectToServerForUnSharing();

	void ConnectToServerForAlbumSynching();

	void ConnectToServerForChangingNumber();

	//void ConnectToServerForAlbumEditingSynching();

//	void ChargePublishComplete();

	void AlbumSynchingComplete();

	void ChangePasswordComplete();

	void GetSmsXML(TInt smsbackuptype);

	void UploadSmsManually(TInt smsbackuptype);

	//void GetAlbumEditingSynchingXML();

	//void EditingAlbumsComplete();

	void ConnectToServerForSubscription(TInt aStatus);

	void CheckSubscriptionStatus(TInt aStatus);

	//void CheckBillingStatus(TInt aStatus);

//	void RegChargePublish();

	void CloseSocketPassword();
	void CloseSocketPublish();
	void CloseSocketShare();
	void CloseSocketSMS();
	void CloseSocketAutoSMS();
	//void CloseSocketUpgrade();
	void CloseSocketStatus(TInt connectionstatus);
	void SendSubStatusError(TInt aStatus);

	void RoamingDisabledDialogSms();
	void RoamingDisabledDialogPublish();
	void RoamingDisabledDialogContacts();
	void CloseConnection();

	CDesCArray *GetFolderList();

	CDesCArray *GetSmsMobileList();

	CDesCArray *GetSmsNameList();

	void DeleteContactsForSms(TInt pos);

    void UserRegistered();

	//void SetTitle();
	
//	void ChargePublish(TInt publishtype);

	void SelectAccess();

	void SelectAccessSettings();

	TInt SetAccesspoint();

	TInt AutoAccesspoint();

	TBool LoadFromSecureFile(TDes& aValue, TDesC& aParam);

	TBool LoadFromConfigFile(TDes& aValue, TDesC& aParam);
	void LoadParamsFromConfigFile();
	
	//void MapAPNtoActualName();

	TInt64 ReturnPower(TInt a, TInt b);
	
	void ActivateView(TUid aViewId);
#ifdef __UIQ__

#ifdef __S80__
	void DisplayProgressBallL(TInt aResourceId);
	void DestroyProgressBall();
#endif
	//CContactDatabase* iContactsDB1;
	TBuf<100> iMobBuffer;
	TInt iImageFlag;
	void SharingListBoxDialog(TInt aCount);
	TInt SharingClientListBoxDialog();
	TInt FolderDialog();
	TInt ProcessAlbum();
	TInt DescDialog();
	TInt FolderDialog1();

	CDesCArray* numbers;

	TInt CheckPasswordField(TDesC& password);
	void SecureDialog();
	void GetTextFieldFromFieldSet(CContactItemFieldSet& aFieldSet, const TUid index,TDesC& name);
	//void GetContactNames(TInt id,TDesC& name,TDesC& mobile);
	void ShowView();
	//void ShowToolBar();
	//void ShowSettingsScreen(TInt aIndex);
	void ShowStatusScreen();
	//void SaveSelectedSmsContacts(TDesC& name, TDesC& mobile);
	TInt ShowContactsDialog(TDesC& fname);
#endif
	CDesCArray* FolderExist();
	CDesCArray* ReturnText();
	CDesCArray* ReturnTextForImage();
    
//	void SaveSettingsToDB(TDes& aIP, TUint16 aPort, TInt8 aMode, TInt8 aDesc, TInt8 aRoam, TInt8 aAutostart);

//    void GetSettingsFromDB(TInt& aMode, TInt& aDesc, TInt& aRoam);
	
//	void SaveUploadSettingsToDB(TInt8 aAlbum, TInt8 aPhoto, TInt8 aVideo, TInt8 aShares);

    void GetUploadSettingsFromDB(TInt& aAlbum, TInt& aPhoto, TInt& aVideo, TInt& aAlbumShares);
    
    void SaveContactsUploadSettingsToDB(TInt aCount);
    
    void GetContactsUploadSettingsFromDB(TInt& aCount);

	void SaveTempContactsUploadSettingsToDB(TInt aCount);
    
    void GetTempContactsUploadSettingsFromDB(TInt& aCount);

	TInt8 GetMode();

	void GetDefaultFolder(TDesC& aFolder);

	void EnableUpload(TInt aIsRegistered);

	void EnableSmsUpload(TInt aIsRegistered);

	TInt CheckAndSaveFolderToDB(TDesC& aFolder);

	void UpdatePhotosVideosCount(TInt aPhotos, TInt aVideos, TInt aAlbumShare);
	
#ifdef __LOGME__	
	void WriteLogFile(TDes8& Text);
	
	RFs iLogSession;
	RFile LogFile;
	TBuf8<1024> LogText;
#endif

	TBool RegFileExists();
	void ShowMessageBoxFromResource(TInt aResourceId); 

	/**
     * Function:    ApplicationDriveAndPath
     *
     * Description: Retrieve full path of the application.
     */
    static TFileName ApplicationDriveAndPath();

	void HandleCommandL(TInt aCommand);

	void EnablePhotosUpload();

	TInt DeleteFolderFromDB(TDesC& aFolder);

	TInt EditFolderFromDB(TDesC& aOldFolder, TDesC& aNewFolder);

	void GetUpgradeStatus(TInt8& aChoice, TDes& aVersion);
	void SaveUpgradeStatus(TInt8 aChoice, TDesC& aVersion);

	void SaveApnID(TUint32 aId);
	void GetApnID(TUint32& aId);

	void PromptForSecureExit();
	TInt PasswordDialog();

	TBool CheckIfPending();

	TBool PendingSMS();
	
	TBool LoadEncodedHpFromConfigFile(TDes& aValue, TDesC& aParam);

	void ClearMediaQueue();

	void GetAccessPointID();

//	void BringToTheFront();

	void CheckSuccessfullSms();

	void UploadSuccessfullSms();

	void CaseMinimize();

	void CaseSuspend();

#ifdef WAP_PT
	TInt GetPayloadBuffer(HBufC8** aBuffer);
//#ifdef WAP_PT
	TInt GetSMSPayloadBuffer(HBufC8** aBuffer, TInt aSmstype);
#endif

private: // from CEikAppUi
#ifdef __UIQ__
#ifdef __S80__
public:
	CEikButtonGroupContainer*	iCBA;
private:
	CCknFloatingProgressBall*	iProgressBall;

	CUnRegisterView*	iUnRegisterView;
	//CRegisterView*		iRegisterView;
	CPhotoListView*		iPhotoListView;
	CSmsListView*		iSmsListView;
	CStatusScreenView*	iStatusScreenView;
	CContactsView*		iContactsListView;
	CVideoListView*		iVideoListView;
#else
	CUnRegisterView*		iUnRegisterView;
	//CMainScreenView*		iMainScreenView;
	CPhotoScreenView*		iPhotoListView;
	CVideoScreenView*		iVideoListView;
	CSmsScreenView*			iSmsListView;
	CStatusScreenView* 		iStatusScreenView;
	CContactsScreenView* 	iContactsScreenView;

/*   CRippleVaultAppView* iAppView;
    CExampleAppView1* iAppView1;
    CEmptyView* iAppEmptyView;
	CPhotoListView *iAppPhotoListView;
	CVideoListView *iAppVideoListView;
	CMyListBoxAppView* iMainScreenAppView;*/
	//TInt iPhotoSendMode;
#endif
#else
	CRegisterView* iRegView;
#endif
    enum EMediaType
    {
        KPhoto,
        KVideo
    };    

    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);

    void GetIMEIFromPhoneL();

	//void GetProtocolDataXML(TDes8& buffer, TInt aMaxLen, TDes& albumname);
	void GetSMSProtocolDataXML(TDes8& buffer, TInt aMaxLen, TInt aSmstype);
	void GetFirstProtocolDataXML(TDes8& buffer, TDes& albumname);
    TInt GetMoreDataFromFile(TDes8& aData, TInt aMaxLen);
	void SeekDataFromFile(TInt aPacketNumber);
	TInt GetMoreSMSDataFromFile(TDes8& aData, TInt aMaxLen, TInt aSmstype);
//    void GetProtocolEndDataXML(TDes8& aData);
    void SendFileComplete(TDesC& albumnamestatus);
	void SendSmsFileComplete(TInt aSmstype);
	void SendSmsFileError(TInt aSmstype);
    void SendSharingFileComplete(TInt flag);
	void SendUnSharingFileComplete();
	void SendFileError();
    void SendSharingFileError(TInt flag);
	void MoreFilesToUpload();
	void MoreSharingFilesToUpload(TInt flag);
	TInt CheckMediaExists(TDesC& aFilename);

    void SendToServer();	

    //TBuf<MBasicGsmPhoneId::KPhoneSerialNumberSize> iIMEIPhone;
	TBuf<30> iIMEIPhone;

public:
    CPhotosDB* iPhotosDB;

	TTime iSmsNotif;
	TInt iUploadContacts;
	TInt iUploadPhotos;
	TInt iUploadVideos;
	TInt iUploadSms;

private:
    //CSocketsEngineWriter* iSocketWriter;
	
	CSocketsEngineWriter* iSocketWriterPublish;
	CSocketsEngineWriter* iSocketWriterShare;
	CSocketsEngineWriter* iSocketWriterSMS;
	CSocketsEngineWriter* iSocketWriterAutoSMS;

	//CSocketsEngineWriter* iSocketWriterUpgrade;
	CSocketsEngineWriter* iSocketWriterPublishStatus;
	CSocketsEngineWriter* iSocketWriterContactsStatus;
	CSocketsEngineWriter* iSocketWriterSmsStatus;
	CSocketsEngineWriter* iSocketWriterAutoSmsStatus;
	CSocketsEngineWriter* iSocketWriterSimChangeStatus;
	CSocketsEngineWriter* iSocketWriterPassword;
	//CSocketsEngineWriter* iSocketWriterBillingStatus;
   
	//TInt iScheduled;
	TInt isOpen;
    RFs iFsession;	
    RFile iFileDes;
	
    RFs iFSMSsession;	
    RFile iFileSMSDes;

    RFs iFAutoSMSsession;	
    RFile iFileAutoSMSDes;

    //EMediaType iMediaType;    

	// Settings
	TBuf<30> iDefaultFolder;	

    friend class CSocketsEngineWriter;
};
#ifdef __UIQ__
//Register Dialog
//class CMsvSession;
//class MMsvSessionObserver;

class CRegisterDialog : public CEikDialog
	{
public:
	CRegisterDialog(CRippleVaultAppUi& aAppUi);
	~CRegisterDialog();
private:
	CRippleVaultAppUi & iAppUi;
	//CSocketsEngineWriter *iSocketsEngine;
	void 	PreLayoutDynInitL();	
	TInt    OkToExitL(TInt aKeycode);
	//void	RegisterRipple(TDes& aUser, TDes& aPhone );
	//HBufC* GetTimeStamp(TTime& aTime);
	TBool ShutL();
	};	

//AlreadyRegister Dialog
class CAlreadyRegisterDialog : public CEikDialog
	{
public:
	CAlreadyRegisterDialog(CRippleVaultAppUi& aAppUi);
	~CAlreadyRegisterDialog();
private:
	//CSocketsEngineWriter *iSocketsEngine;
	CRippleVaultAppUi & iAppUi;
	void 	PreLayoutDynInitL();	
	TInt    OkToExitL(TInt aKeycode);
	//void	RegisterRipple(TDes& aUser, TDes& aPasswd, TDes& aPhone);
	//HBufC* GetTimeStamp(TTime& aTime);
	TBool ShutL();
	};	

//Settings Dialog
class CAdvancedDialog : public CEikDialog
	{
public:
	CAdvancedDialog(CRippleVaultAppUi& aAppUi);
	~CAdvancedDialog();
	//void GetAccessPoints();
//	void SetAccessPoints();	
private:
	CDesCArray *array;
	CRippleVaultAppUi & iAppUi;
	//TUint32 ApNum[10];
	TInt    OkToExitL(TInt aKeycode);
	void 	PreLayoutDynInitL();
	TBool ShutL();
	};

//Folder Dialog
class CFolderDialog : public CEikDialog
	{
public:
	CFolderDialog(CRippleVaultAppUi& aAppUi);
    ~CFolderDialog();	
private:	
    CRippleVaultAppUi & iAppUi;
    CDesCArray *FolderList;
	TInt    OkToExitL(TInt aKeycode);
	void 	PreLayoutDynInitL();
	TBool ShutL();
	};	

//ProcessAlbumDialog Dialog
class CProcessAlbumDialog : public CEikDialog
	{
public:
	CProcessAlbumDialog(CRippleVaultAppUi& aAppUi);
    ~CProcessAlbumDialog();	
private:	
    CRippleVaultAppUi & iAppUi;
    CDesCArray *FolderList;
	TInt    OkToExitL(TInt aKeycode);
	void 	PreLayoutDynInitL();
	TBool ShutL();
	};	

//Folder1 Dialog
class CFolderDialog1 : public CEikDialog
	{
public:
	CFolderDialog1(CRippleVaultAppUi& aAppUi);
    ~CFolderDialog1();	
private:	
    CRippleVaultAppUi & iAppUi;
    CDesCArray *FolderList;
	TInt    OkToExitL(TInt aKeycode);
	void 	PreLayoutDynInitL();
	TBool ShutL();
	};	

//Desc Dialog
class CDescDialog : public CEikDialog
	{
public:
	CDescDialog(CRippleVaultAppUi& aAppUi);
    ~CDescDialog();	
private:	
    CRippleVaultAppUi & iAppUi;    
	TInt    OkToExitL(TInt aKeycode);
	void 	PreLayoutDynInitL();
	TBool ShutL();
	};	

class CSharingListBoxDialog : public CEikDialog
	{
public:
	CSharingListBoxDialog(CRippleVaultAppUi& aAppUi);
    ~CSharingListBoxDialog();	
private:	
    CRippleVaultAppUi & iAppUi;    
	CDesCArray *FolderList;
	TInt    OkToExitL(TInt aKeycode);
	void 	PreLayoutDynInitL();
	TBool ShutL();
	};		

//Option Dialog
class CChoiceListControl : public CEikDialog
	{
public:
	CChoiceListControl(CRippleVaultAppUi& aAppUi);
	~CChoiceListControl();
private:
	CRippleVaultAppUi & iAppUi;	
	void 	PreLayoutDynInitL();	
	TInt    OkToExitL(TInt aKeycode);
	TBool ShutL();
	};	

//mobile Dialog
class CMobileDialog : public CEikDialog
{
public:
	CMobileDialog(CRippleVaultAppUi& aAppUi);
	~CMobileDialog();
private:	
	CRippleVaultAppUi & iAppUi;
	void 	PreLayoutDynInitL();	
	TInt    OkToExitL(TInt aKeycode);
	TBool ShutL();
};	

//Regmobile Dialog
class CRegMobileDialog : public CEikDialog
{
public:
	CRegMobileDialog(CRippleVaultAppUi& aAppUi);
	~CRegMobileDialog();
private:	
	CRippleVaultAppUi & iAppUi;
	void 	PreLayoutDynInitL();	
	TInt    OkToExitL(TInt aKeycode);
	TBool ShutL();
};	

//Contacts Settings Dialog
class CContactsSettingDialog : public CEikDialog
	{
public:
	CContactsSettingDialog(CRippleVaultAppUi& aAppUi);
	~CContactsSettingDialog();	
private:
	CRippleVaultAppUi & iAppUi;
	TInt    OkToExitL(TInt aKeycode);
	void 	PreLayoutDynInitL();
	TBool ShutL();
	};

//Photos Settings Dialog
class CPhotosSettingDialog : public CEikDialog
	{
public:
	CPhotosSettingDialog(CRippleVaultAppUi& aAppUi);
	~CPhotosSettingDialog();	
private:
	CRippleVaultAppUi & iAppUi;
	TInt    OkToExitL(TInt aKeycode);
	void 	PreLayoutDynInitL();
	TBool ShutL();
	};

//Videos Settings Dialog
/*class CVideosSettingDialog : public CEikDialog
	{
public:
	CVideosSettingDialog(CRippleVaultAppUi& aAppUi);
	~CVideosSettingDialog();	
private:
	CRippleVaultAppUi & iAppUi;
	TInt    OkToExitL(TInt aKeycode);
	void 	PreLayoutDynInitL();
	TBool ShutL();
	};
*/
//Videos Settings Dialog
class CSmsSettingDialog : public CEikDialog
	{
public:
	CSmsSettingDialog(CRippleVaultAppUi& aAppUi);
	~CSmsSettingDialog();	
private:
	CRippleVaultAppUi & iAppUi;
	TInt    OkToExitL(TInt aKeycode);
	void 	PreLayoutDynInitL();
	TBool ShutL();
	};

//SelectedContacts Dialog
class CSelectedContactsDialog : public CEikDialog
	{
public:
	CSelectedContactsDialog(CRippleVaultAppUi& aAppUi);
    ~CSelectedContactsDialog();	
private:	
    CRippleVaultAppUi & iAppUi;
    //CDesCArray *FolderList;

	TInt    OkToExitL(TInt aKeycode);
	void 	PreLayoutDynInitL();
	TBool ShutL();
	};	

//check sim Dialog
class CCheckSimDialog : public CEikDialog
{
public:
	CCheckSimDialog(CRippleVaultAppUi& aAppUi);
	~CCheckSimDialog();
private:	
	CRippleVaultAppUi & iAppUi;
	void 	PreLayoutDynInitL();	
	TInt    OkToExitL(TInt aKeycode);
	TBool ShutL();
};	

//change password
class CChangePassword : public CEikDialog
	{
public:
	CChangePassword(CRippleVaultAppUi& aAppUi);
	~CChangePassword();
private:
	CRippleVaultAppUi & iAppUi;	
	void 	PreLayoutDynInitL();	
	TInt    OkToExitL(TInt aKeycode);
	TBool ShutL();
	};	

//Folder1 Dialog
class CUnShareAlbumDialog : public CEikDialog
	{
public:
	CUnShareAlbumDialog(CRippleVaultAppUi& aAppUi);
    ~CUnShareAlbumDialog();	
private:	
    CRippleVaultAppUi & iAppUi;
    CDesCArray *FolderList;
	TInt    OkToExitL(TInt aKeycode);
	void 	PreLayoutDynInitL();
	TBool ShutL();
	};	

//check sim Dialog
class CSecureSettingDialog : public CEikDialog
{
public:
	CSecureSettingDialog(CRippleVaultAppUi& aAppUi);
	~CSecureSettingDialog();
private:	
	CRippleVaultAppUi & iAppUi;
	void 	PreLayoutDynInitL();	
	TInt    OkToExitL(TInt aKeycode);
	TBool ShutL();
};	

//dialog box
class CCustomMessageDialog : public CEikDialog
{
public:
    CCustomMessageDialog(TDesC& aText, TInt aResourceId);
	~CCustomMessageDialog();
private:
	//TBuf<200> iText;
	HBufC* iText;
	TInt iTitle;
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aKeycode);
	TBool ShutL();
};

class CPassWordDialog : public CEikDialog
	{
public:
	CPassWordDialog(CRippleVaultAppUi& aAppUi);
	~CPassWordDialog();
private:
	CRippleVaultAppUi & iAppUi;
	TInt    OkToExitL(TInt aKeycode);
	TBool ShutL();
	};	

//Contacts upload dialog
class CContactsUploadSettingDialog : public CEikDialog
{
public:
	CContactsUploadSettingDialog(CRippleVaultAppUi& aAppUi);
	~CContactsUploadSettingDialog();
private:	
	CRippleVaultAppUi & iAppUi;
	void 	PreLayoutDynInitL();	
	TInt    OkToExitL(TInt aKeycode);	
};
class CCheckBoxDialog : public CEikDialog
	{
public:
	CCheckBoxDialog(CRippleVaultAppUi& aAppUi, TDes& aListselection);
	~CCheckBoxDialog();
private:
	CRippleVaultAppUi & iAppUi;
	void 	PreLayoutDynInitL();
	TInt    OkToExitL(TInt aKeycode);
	TBool ShutL();
	TDes& iListSelection;
	};	
#endif	
#endif // __RIPPLEVAULTAPPUI_H__

