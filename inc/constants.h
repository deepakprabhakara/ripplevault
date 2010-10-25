#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

const TInt KDefaultPortNumber = 80;
_LIT(KDefaultServerName, "203.208.254.132");
//_LIT(KUpGradeFile, "ripplevault.sis");

const TUid KUidRippleVaultApp = {0x102032D0}; // App UID

#define MAX_RETRIES 1

#define KIndiaCountryCode 404

#define ITEM_SELECTED 10000

#ifdef WAP_PT
#define MAX_BUFFER_LEN (10*1024)
#else
#define MAX_BUFFER_LEN (50*1024)
#endif

// Max no of contacts to be sent in one buffer
#ifdef WAP_PT
#define MAX_CONTACTS 20
#else
#define MAX_CONTACTS 50 // 50
#endif

#define MAX_RECV_LEN (100*1024)
#define READ_LEN 1024


#ifndef WAP_PT

#ifdef __NOKIA_SYM8__
#define PUBLISH_PACKET_SIZE 10 //40Kb packet chunk for sym8 (fast phones)
#else
#define PUBLISH_PACKET_SIZE 10 //20Kb packet chunk
#endif

#else
#define PUBLISH_PACKET_SIZE 10 //10Kb packet chunk for wap
#endif

#define SMS_PACKET_SIZE (10*1024) //40Kb packet chunk for sym8 (fast phones)

_LIT(KContactsDBFile,"changesdb.dat");
_LIT(KAutoStartFile, "autostart.txt");
_LIT(KRegPath,       "RegRippleVault.dat");
_LIT(KConfigFile,    "config.txt");
_LIT(KDatabaseName, "photos.db");
_LIT(KTempFile, "Log.txt");
_LIT(KTimeStampFile, "timestamp.txt");
_LIT(KRegPath1, "RegDetails.dat");	
_LIT(KAutostartSettings, "astart.stg");
_LIT(KInitUploadFile, "initupload.tag");
_LIT(KRegStep1File, "reg_step1.txt");
_LIT(KIMSIFile, "Imsi.txt");
_LIT(KSecureFile, "secure.txt");
_LIT(KSecureStopFile, "SecureStop.dat");
_LIT(KContactSetupWizard, "contactsetup.dat");
_LIT(KResumeFile, "resume.dat");
_LIT(KInitSmsFile, "initsms.dat");

_LIT(KUpgradeFilesPath, "c:\\system\\ripple_upg\\");

#ifdef __UIQ__
#ifdef __S80__
	_LIT(KDirPhonePhoto,"c:");
	_LIT(KDirMMCPhoto,"d:");
	_LIT(KDirPhoneVideo,"c:");
	_LIT(KDirMMCVideo,"d:");
	
	_LIT(KFolderPhonePhoto,"c:\\");
	_LIT(KFolderMMCPhoto,"d:\\");
	_LIT(KFolderPhoneVideo,"c:\\");
	_LIT(KFolderMMCVideo,"d:\\");
	
	_LIT(KFilesPhonePhoto,"c:\\*");
	_LIT(KFilesMMCPhoto,"d:\\*");
	_LIT(KFilesPhoneVideo,"c:\\*");
	_LIT(KFilesMMCVideo,"d:\\*");

	_LIT(KDirPhonePhotoSony,"c:\\my files\\images");
	_LIT(KDirMMCPhotoSony,"d:\\my files\\images");
	_LIT(KDirPhoneVideoSony,"c:\\my files\\video");
	_LIT(KDirMMCVideoSony,"d:\\my files\\video");

	_LIT(KFilesPhonePhotoSony,"c:\\my files\\images\\*");
	_LIT(KFilesMMCPhotoSony,"d:\\my files\\images\\*");
	_LIT(KFilesPhoneVideoSony,"c:\\my files\\video\\*");
	_LIT(KFilesMMCVideoSony,"d:\\my files\\video\\*");
#else
	//P800 Photo Path
	_LIT(KDirPhonePhoto,"c:\\documents\\media files\\image\\unfiled");
	_LIT(KDirMMCPhoto,"d:\\media files\\image\\unfiled");
	_LIT(KDirPhoneVideo,"c:\\documents\\media files\\video\\unfiled");
	_LIT(KDirMMCVideo,"d:\\media files\\video\\unfiled");
	
	_LIT(KFolderPhonePhoto,"c:\\documents\\media files\\image\\unfiled\\");
	_LIT(KFolderMMCPhoto,"d:\\media files\\image\\unfiled\\");
	_LIT(KFolderPhoneVideo,"c:\\documents\\media files\\video\\unfiled\\");
	_LIT(KFolderMMCVideo,"d:\\media files\\video\\unfiled\\");
	
	_LIT(KFilesPhonePhoto,"c:\\documents\\media files\\image\\unfiled\\*");
	_LIT(KFilesMMCPhoto,"d:\\media files\\image\\unfiled\\*");
	_LIT(KFilesPhoneVideo,"c:\\documents\\media files\\video\\unfiled\\*");
	_LIT(KFilesMMCVideo,"d:\\media files\\video\\unfiled\\*");

	_LIT(KDirPhonePhotoSony,"c:\\documents\\media files\\image\\Sony Ericsson");
	_LIT(KDirMMCPhotoSony,"d:\\media files\\image\\Sony Ericsson");
	_LIT(KDirPhoneVideoSony,"c:\\documents\\media files\\video\\Sony Ericsson");
	_LIT(KDirMMCVideoSony,"d:\\media files\\video\\Sony Ericsson");

	_LIT(KFilesPhonePhotoSony,"c:\\documents\\media files\\image\\Sony Ericsson\\*");
	_LIT(KFilesMMCPhotoSony,"d:\\media files\\image\\Sony Ericsson\\*");
	_LIT(KFilesPhoneVideoSony,"c:\\documents\\media files\\video\\Sony Ericsson\\*");
	_LIT(KFilesMMCVideoSony,"d:\\media files\\video\\Sony Ericsson\\*");
#endif
#else 
	_LIT(KDirPhonePhoto,"c:\\Nokia\\Images");
	_LIT(KDirMMCPhoto,"e:\\Images");
	_LIT(KDirPhoneVideo,"c:\\Nokia\\Videos");
	_LIT(KDirMMCVideo,"e:\\Videos");
	
	_LIT(KFolderPhonePhoto,"c:\\Nokia\\Images\\");
	_LIT(KFolderMMCPhoto,"e:\\Images\\");
	_LIT(KFolderPhoneVideo,"c:\\Nokia\\Videos\\");
	_LIT(KFolderMMCVideo,"e:\\Videos\\");

	_LIT(KFilesPhonePhoto,"c:\\Nokia\\Images\\*");
	_LIT(KFilesMMCPhoto,"e:\\Images\\*");
	_LIT(KFilesPhoneVideo,"c:\\Nokia\\Videos\\*");
	_LIT(KFilesMMCVideo,"e:\\Videos\\*");
#endif

	_LIT(KTickMbmFileName, "tick.mbm");
	_LIT(KImageMbmFileName, "Images.mbm");
	_LIT(KVideoMbmFileName, "video.mbm");
	_LIT(KSubIconsMbmFileName, "subicons.mbm");
	_LIT(KIconsMbmFileName, "icons.mbm");

	_LIT(KSyncMbmFileName,"Sync.mbm");
	_LIT(KStatusMbmFileName,"statustick.mbm");
	_LIT(KStatusMbmFileName1,"statustick1.mbm");
#ifdef __UIQ__
	_LIT(KLogoMbmFileName,"logo_uiq.mbm");
	_LIT(KUiqSmsMbmFileName, "Uiqsms.mbm");
	_LIT(KS80SmsMbmFileName, "S80sms.mbm");
#else //S60
	_LIT(KLogoMbmFileName,"logo.mbm");
	_LIT(KSmsMbmFileName, "sms.mbm");
	_LIT(KSettingsMbmFileName, "settings.mbm");
	_LIT(KRippleImageMbmFileName, "rippleimage.mbm");
	_LIT(KPhotoTickMbmFileName, "phototick.mbm");
	_LIT(KMarkingMbmFileName, "marking.mbm");
	_LIT(KEmptyMbmFileName, "empty.mbm");
#endif

#ifndef __UIQ__
//view ids
	//const TUid KViewIdSetupWizard = { 17 }; // UID of View.
	//const TUid KViewIdSubScreen = {16}; //sub screen
	//const TUid KViewIdMainScreen = {15}; //main screen
	//const TUid KViewIdMainScreen = {4}; //main screen
	const TUid KViewIdSelectiveSms = {14}; //selective sms
	const TUid KViewIdSmsList = {12}; //sms list
	const TUid KViewIdMailId = {8}; // MaildID
	const TUid KViewIdHelp = {7}; //Help
	const TUid KViewIdFindBox = {6}; // FindBoxView
	const TUid KViewIdPhotoList = {5}; //photo gallery
	const TUid KViewIdStatusScreen = {4}; //status
	const TUid KViewIdUnregisterScreen = {3}; //unregistered
	const TUid KViewIdEmptyScreen = {2}; //empty view
	const TUid KViewIdSettingsScreen = {1}; // Settings panel.
#else
#ifdef __S80__
	const TInt KViewIdUnregisterScreen = 0; //unregistered
	//const TInt KViewIdRegisterScreen = 1;
	const TInt KViewIdPhotoList = 2; //photo gallery
	const TInt KViewIdSmsList = 3; //sms list
	const TInt KViewIdStatusScreen = 4; //status
	const TInt KViewIdContactsViewScreen = 5; //Contacts View
	const TInt KViewIdVideoList = 6; //photo gallery
#else
	const TInt KViewIdUnregisterScreen = 0; //unregistered
	//const TInt KViewIdRegisterScreen = 1;
	const TInt KViewIdPhotoList = 2; //photo gallery
	const TInt KViewIdSmsList = 4; //sms list
	const TInt KViewIdStatusScreen = 5; //status
	const TInt KViewIdContactsViewScreen = 6; //Contacts View
	const TInt KViewIdVideoList = 3; //photo gallery
#endif
#endif

#define NETWORK_ERRCODE_PARSING				100
#define NETWORK_ERRCODE_FAILED				101
#define NETWORK_ERRCODE_TIMEDOUT			102
#define NETWORK_ERRCODE_MSISDNMISMATCH		103
#define NETWORK_ERRCODE_IMEIMISMATCH		104
#define NETWORK_ERRCODE_ACCSUSPENDED		105

#define NETWORK_STATUS_CONNECTING			200
#define NETWORK_STATUS_UPLOADING_MEDIA		201
#define NETWORK_STATUS_UPLOADING_SMS		202
#define NETWORK_STATUS_UPLOADING_CONTACTS	203
#define NETWORK_STATUS_FAILED				204
#define NETWORK_STATUS_SYNCHING_ALBUMS		205
#define NETWORK_STATUS_RUNNING				206
#define NETWORK_STATUS_CHECKING_ACCOUNT		207
#define NETWORK_STATUS_SUSPENDUPLOAD		222


//flags moved out as constants
#define PHOTOSBACKUP_AUTO					0
#define PHOTOSBACKUP_PROMPT					1
#define PHOTOSBACKUP_MANUAL					2

#define SMSBACKUP_SELECTIVE					0
#define SMSBACKUP_SCHEDULED					1
#define SMSBACKUP_SCHEDULED_HOURLY			2
#define SMSBACKUP_MANUAL					3

#define DISABLED							0
#define ENABLED								1

#ifdef __S80__
#define NETWORK_ROAMING						7
#else
#define NETWORK_ROAMING						5
#endif

#define PHOTOS_PHONEMEMORY					0
#define PHOTOS_MMC							1
#define VIDEOS_PHONEMEMORY					2
#define VIDEOS_MMC							3

#define SMS_INBOX							0
#define SMS_SENTITEMS						1

#endif
