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
 Module Name				: RippleVaultAppUi.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-01-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Main file for RippleVault APP
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/


/*********************
MACRO __UIQ__ is for UIQ builds, otherwise by default S60 build
WAP_PT is for WAP stack client, otherwise Internet APN build by default
NOKIA6600 is for Symbian 7 specific build. Videosengine6600 is specifically for Symbian 7 & 8 phones due to the difference in the way videos are saved on these phones.

The above MACRO's are defined in the .mmp files in group folder

*/

#ifndef __UIQ__ // series 60
	#include <avkon.hrh>
	#include <bautils.h>
	#include <akntitle.h>
	#include <apselect.h>
	#include <eikmenup.h>
	#include <intconninit.h>
	#include <RippleVault.rsg>
	#include <aplistitemlist.h>
	#include <aknmessagequerydialog.h>
	#include <aknnotewrappers.h> 

	#include "HelpView.h"
	#include "FindBoxView.h"
	#include "RegisterView.h"
	#include "TestFileView2.h"
	#include "RippleVault.hrh"
	#include "PhotoListView.h"	
	#include "UnRegisterView.h"
	#include "ManualSmsListView.h"

	//#include "MainScreenListView.h"
	//#include "MainSubScreenListView.h"

	#include "AknExSettingListView.h"
	//#include "SetupWizardListView.h"
	#include "SelectiveSmsListView.h"

	//#include <apdatahandler.h>
	//#include <CommDbConnPref.h>			
	//#include <apaccesspointitem.h> 		
#else
#ifdef __S80__
	#include "UnRegisterView.h"
	//#include "RegisterView.h"
	#include "SmsListView.h"
	#include "PhotoListView.h"
	#include "StatusScreenView.h"
	#include "ContactsView.h"
	#include "VideoListView.h"

	#include "imsi.h"
	#include "CheckNetwork.h"

	//#include <eikenv.h>
	//#include <eikon.rsg>
	//#include <apgtask.h>
	#include <eikcmbox.h>
	#include<eikmfne.h>
	#include <plpvariant.h>
	#include <AgentClient.h>
	#include <ckninfo.h>
	#include <eikchkbx.h>
	#include<eikbutb.h>

	#include <RippleVault_series80.rsg>
	#include "ripplevault_series80.hrh"
	#include <etel3rdparty.h> 

	#include <eikhopbt.h>
	#include <eikmfne.h>
	#include <eikmenub.h>
	#include <cknconf.h>
#else //UIQ
	#include <commdb.h>	
	#include <badesca.h>	
	#include <cdbover.h>		
	#include <cntitem.h>
	#include <bautils.h>
	#include <cntfldst.h>
	#include <eikcmbox.h>
	#include <eiktxlbm.h>
	#include <eikmenub.h>		
	#include <eikchlst.h>
	#include <cdbstore.h>	
	#include <plpvariant.h>
	#include <QikIpeditor.h>
	#include <AgentClient.h>	
	#include <QikNumberEditor.h>
	#include <QikTimeEditor.h>
	#include <RippleVault_uiq.rsg>
	#include <qikvertoptionbuttonlist.h>

	#include "RippleVault_uiq.hrh"
	#include <qikon.hrh>
	
	#include <mtclreg.h>//for sms
	#include <smsclnt.h>
	#include <txtrich.h>
	#include <smuthdr.h>

	#include <eiklabel.h>
	#include <eikchkbx.h>
	#include<eikbutb.h>

	#include "UnRegisterView_Uiq.h"
	//#include "MainScreenView_Uiq.h"
	#include "MainScreenListBox.h"
	#include "PhotoScreenView_Uiq.h"
	#include "MediaScreenListBox.h"
	#include "VideoScreenView_Uiq.h"
	#include "SmsScreenView_Uiq.h"
	#include "StatusScreenView_Uiq.h"
	#include "ContactsScreenView_Uiq.h"
#endif
#endif

#ifndef __UIQ__
#include <e32base.h>
#include <gulicon.h>
#include <akniconarray.h>
#include <AVKON.mbg>
#include "marking.mbg"
#endif

#include "RippleVault.pan"
#include "RippleVaultAppUi.h"
//#include "SmsTimer.h"
#include "PhotosEngine.h"
#include "PhotosTimer.h"

#include "ContactsTimer.h"
#include "SmsBackupTimer.h"
#include "WriteSmsAO.h"
//#include "SmsBackupAO.h"
#include "VideosEngine.h"
//#include "dnsresolver.h"
#include "socketsenginewriter.h"
#include <AgentClient.h>
#include <cdbcols.h>
#include <commdb.h>
#include <cdbover.h>
#include <cdbstore.h>
#include <eikspane.h>
#include <f32file.h>
#include <s32file.h>
#include <d32dbms.h>
#include <etel.h>
#include <cdbcols.h>
#include <apparc.h>
#include <eikenv.h>
#include <eikapp.h>
#include <cdbcols.h>
#include <apgcli.h>
#ifndef __S80__
#include <etelagsm.h>
#endif
#include <etelmm.h>
#include <utf.h>

//deepak
#include "CopyContactsAO.h"
#include "ChangesContactsAO.h"
#include "SMSEngine.h"
#include "SmsSimEngine.h"
//#include "BillingEngine.h"
#include "smsfile.h"
#include "List.h"
#include "RecordContactDB.h"
#include <cntdb.h>
//_____________________

#ifdef __WINSCW__
	#define EMULATOR
#endif

/*
#ifdef __UIQ__
	//P800 Photo Path
	_LIT(KDir1,"c:\\documents\\media files\\image\\unfiled");
	_LIT(KDir2,"d:\\media files\\image\\unfiled");
	_LIT(KDir3,"c:\\documents\\media files\\video\\unfiled");
	_LIT(KDir4,"d:\\media files\\video\\unfiled");
	
	_LIT(KDir5,"c:\\documents\\media files\\image\\Sony Ericsson");
	_LIT(KDir6,"d:\\media files\\image\\Sony Ericsson");
	_LIT(KDir7,"c:\\documents\\media files\\video\\Sony Ericsson");
	_LIT(KDir8,"d:\\media files\\video\\Sony Ericsson");

	_LIT(KFolder1,"c:\\documents\\media files\\image\\unfiled\\");
	_LIT(KFolder2,"d:\\media files\\image\\unfiled\\");
	_LIT(KFolder3,"c:\\documents\\media files\\video\\unfiled\\");
	_LIT(KFolder4,"d:\\media files\\video\\unfiled\\");
	_LIT(KImageFolder1,"c:\\documents\\media files\\image\\unfiled\\*");
	_LIT(KImageFolder2,"d:\\media files\\image\\unfiled\\*");
	_LIT(KVideoFolder1,"c:\\documents\\media files\\video\\unfiled\\*");
	_LIT(KVideoFolder2,"d:\\media files\\video\\unfiled\\*");
#else 
	_LIT(KDir1,"c:\\Nokia\\Images");
	_LIT(KDir2,"e:\\Images");
	_LIT(KDir3,"c:\\Nokia\\Videos");
	_LIT(KDir4,"e:\\Videos");
	_LIT(KImageFolder1,"c:\\Nokia\\Images\\*");
	_LIT(KImageFolder2,"e:\\Images\\*");
	_LIT(KVideoFolder1,"c:\\Nokia\\Videos\\*");
	_LIT(KVideoFolder2,"e:\\Videos\\*");
	_LIT(KFolder1,"c:\\Nokia\\Images\\");
	_LIT(KFolder2,"e:\\Images\\");
	_LIT(KFolder3,"c:\\Nokia\\Videos\\");
	_LIT(KFolder4,"e:\\Videos\\");
#endif
*/

class CPhotosTimer;
class CContactsTimer;
class CSmsBackupTimer;
class CWriteSmsAO;
//class CSmsBackupAO;
class CPhotosEngine;
class CVideosEngine;

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CRippleVaultAppUi::GetIMEIFromPhoneL()
// Retreives IMEI number
// ---------------------------------------------------------

void CRippleVaultAppUi::GetIMEIFromPhoneL()
{
#ifdef __WINS__
    iIMEIPhone.Copy(_L("1234567890"));
#elif __UIQ__
	TPlpVariantMachineId aId;
	PlpVariant::GetMachineIdL(aId);

	iIMEIPhone.Copy(aId);
#else //Series 60
    TBuf<255> tsyName;

	// Read the TSY name from CommsDB
	CCommsDatabase* db = CCommsDatabase::NewL(EDatabaseTypeUnspecified);
	CleanupStack::PushL(db);
	CCommsDbTableView* table = db->OpenTableLC(TPtrC(MODEM));
	table->GotoFirstRecord();
	table->ReadTextL(TPtrC(MODEM_TSY_NAME),tsyName);
	// Cleanup - CommsDB no longer needed
	CleanupStack::PopAndDestroy(2); // table,db

	if (!tsyName.Length())
    {
		User::Leave(KErrNotFound);
	}

	// Connect to the ETel server
	RTelServer telServer;
	User::LeaveIfError(telServer.Connect());
	CleanupClosePushL(telServer);

	// Make sure our TSY is loaded
	User::LeaveIfError(telServer.LoadPhoneModule(tsyName));

	// Get the number of registered phones
	TInt numberOfPhones;
	User::LeaveIfError(telServer.EnumeratePhones(numberOfPhones));

    TInt aPhoneId=0;
	// Get the phone name
	RTelServer::TPhoneInfo phoneInfo;
	User::LeaveIfError(telServer.GetPhoneInfo(aPhoneId,phoneInfo));

	// Open the phone by name
	RBasicGsmPhone phone;
	User::LeaveIfError(phone.Open(telServer,phoneInfo.iName));
	CleanupClosePushL(phone);

	// Get Phone Info
	RBasicGsmPhone::TId id;
	User::LeaveIfError(phone.GetGsmPhoneId(id));

	// Close the Phone handle, no longer needed
	CleanupStack::PopAndDestroy(); // phone

	// Return the retrieved IMEI Number
	iIMEIPhone.Copy(id.iSerialNumber);

	// Cleanup
	telServer.UnloadPhoneModule(tsyName);
	CleanupStack::PopAndDestroy(); // telServer
#endif
}

void CRippleVaultAppUi::ConstructL()
{
#ifdef __NOKIA6600__
	BaseConstructL(EAknEnableSkin); // support themes on sym7 onwards
#else
    BaseConstructL();
#endif

	TBuf<128> name;
    name = CRippleVaultAppUi::ApplicationDriveAndPath();
    name.Append(KTempFile);

#ifdef __LOGME__	
	iLogSession.Connect();

	TInt err=LogFile.Open(iLogSession,name,EFileStreamText|EFileWrite|EFileShareAny);
	if (err==KErrNotFound) 
		err=LogFile.Create(iLogSession,name,EFileStreamText|EFileWrite|EFileShareAny);
	else
	{
		LogFile.Seek(ESeekEnd,0);
	}
#endif

#ifdef __LOGME__	
	LogText.Copy(_L("Log Started\n\n\n"));
	WriteLogFile(LogText);
#endif
////////////////////////////////
	//for packets photo

    GetIMEIFromPhoneL();

	iPhotosEngine1 = NULL;
    iPhotosEngine2 = NULL;
	iVideosEngine1 = NULL;
	iVideosEngine2 = NULL;
	iPhotosTimer   = NULL;
	iContactsTimer   = NULL;
	iSmsBackupTimer = NULL;
	iWriteSmsAO = NULL;
//	iSmsBackupAO = NULL;
	iWaitDialog = NULL;
 
#ifdef __UIQ__
//#ifndef __S80__
	iPhotosEngine3 = NULL;
    iPhotosEngine4 = NULL;
	
	iVideosEngine3 = NULL;
	iVideosEngine4 = NULL;
//#endif
#endif

	iChangeNotifier = NULL;
	iSMSEngine = NULL;
	iSmsSimEngine = NULL;
//	iBillingEngine = NULL;
	iCopyAO = NULL;
	iChangesAO = NULL;
	iContactsHandler = NULL;
	iContactsDB = NULL;

	iRipplePhotos = 0;

	iApnCount = -1;
	
#ifdef __LOGME__	
	LogText.Copy(_L("Log Started2\n\n\n"));
	WriteLogFile(LogText);
#endif

	//UPGRADE
	TBool upgflag = CheckUpgradeFile();

	RegisterContactsChangeHandler();

	iPhotosDB = CPhotosDB::NewL();

    //TBuf<128> name;
	name.Copy(_L(""));
    name = CRippleVaultAppUi::ApplicationDriveAndPath();
    name.Append(KDatabaseName);

    if (iPhotosDB->CreateDbL(name) != KErrNone)
    {
        // assume db exists 
        iPhotosDB->OpenDbL(name);
    }

	iVersion.Copy(_L(""));
	iOs.Copy(_L(""));
	iUserNames.Copy(_L(""));
	iMobileNo.Copy(_L(""));
	iPassword.Copy(_L(""));
//	iCharge.Copy(_L(""));
	
	iRegUrl.Copy(_L(""));
	iPublishUrl.Copy(_L(""));
	iContactsUrl.Copy(_L(""));
	iSmsUrl.Copy(_L(""));
//	iChargeClient.Copy(_L(""));
//	iChargePhoto.Copy(_L(""));
//	iChargeVideo.Copy(_L(""));
	iWapUrl.Copy(_L(""));
	iPlanUrl.Copy(_L(""));
	iAlbumUrl.Copy(_L(""));
	iStatusUrl.Copy(_L(""));
	iGateWay.Copy(_L(""));
	iAccessPoints.Copy(_L(""));
	
	LoadParamsFromConfigFile();

	GetImsiNumber();

	CheckLockFile();

	if(upgflag)
		iPhotosDB->SaveUpgradeStatus(0, iVersion);
	
//	iDNSResolver = new CDNSResolver;
	
//	TBuf<80> path;	
//	path.Copy(ApplicationDriveAndPath());
//	path.Append(_L("dns.txt"));
//	iDNSResolver->Init(path);

	TInt ports;
	TInt modes;
	TInt desc;
	TInt roam;
	TInt autostart;
	//TInt ccount;
	TInt securegallery;
	TInt upload;
	
#ifdef __LOGME__	
	LogText.Copy(_L("Log Started10\n\n\n"));
	WriteLogFile(LogText);
#endif

	GetContactsSettingsFromDB(roam,upload);

	iContactsHandler->iContactsRoaming = roam;
	iContactsHandler->iContactsUpload = upload;

	GetSettingsFromDB(iServerAddr, ports, autostart);
	GetPhotosSettingsFromDB(modes, desc, roam, securegallery);	

    TInt smstype;
	TTime time;
	TInt smsroam;
	
	GetSmsSettingsFromDB(smstype, time, smsroam);

	//GetContactsUploadSettingsFromDB(ccount);	
	
	iPort=ports;
	iMode=modes;
	iDescription=desc;
	iRoaming=roam;
	iAutostart = autostart;
	//iContactsCount = ccount;
	iContactsCount = 0;
	iConnection = 0;
	iSecureGallery = securegallery;
	
	iSmsMode = smstype;
	iSmsTime = time;
	iSmsRoam = smsroam;	

	iSmsValue = 0;
	iContactsTimerState = 0;
	//iApnCount = 2;

	iSmsFile = CSmsFile::NewL(*this);

	iSmsNotif.HomeTime();
	TTimeIntervalHours hours(6);

	iSmsNotif - hours;

#ifdef __UIQ__

#ifdef __S80__
	iUnRegisterView		= NULL;
	//iRegisterView		= NULL;
	iSmsListView		= NULL;
	iPhotoListView		= NULL;
	iStatusScreenView	= NULL;
	iProgressBall		= NULL;
	iContactsListView	= NULL;
	iVideoListView		= NULL;
#else
	iUnRegisterView		= NULL;
	//iMainScreenView		= NULL;
	iPhotoListView		= NULL;
	iVideoListView		= NULL;
	iSmsListView		= NULL;
	iStatusScreenView	= NULL;
	iContactsScreenView	= NULL;
#endif
	iMainScreenValue = 0;

#ifdef __S80__
	iCBA = CEikButtonGroupContainer::Current();
#endif

	if(RegFileExists())
	{
		DefaultAccessPoint();

		ReadUserName();

		//DeleteFolderFromDB(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM_OTHER)));
		//CheckAndSaveFolderToDB(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));

		ShowWindow(KViewIdStatusScreen); //mainscreen view

		ImsiCheck();
	}
	else
	{
		ShowWindow(KViewIdUnregisterScreen); //unregisterview
		
		TInt Reg;
		Reg = ChooseAccessPoint();
		
		AutoRegisterRipple();
	}	

#else // Series 60 code	
	
	//Show=0;
	Show=1;
	CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
	statusPane->SwitchLayoutL(R_AVKON_STATUS_PANE_LAYOUT_USUAL);
	
	StatusPane()->MakeVisible(ETrue);

	CAknTitlePane* tp=(CAknTitlePane*)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidTitle)); 
    tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH))); // Set the text string.

	CUnRegisterView* UnRegView = new (ELeave) CUnRegisterView;
    CleanupStack::PushL( UnRegView );
    UnRegView->iPhotosAppUi = this;
	UnRegView->ConstructL();
    AddViewL( UnRegView );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    

	CRegisterView* RegView = new (ELeave) CRegisterView;
    CleanupStack::PushL( RegView );
	RegView->iPhotosAppUi = this;
	iRegView = RegView;
    RegView->ConstructL();
    AddViewL( RegView );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    

	CAknExSettingListView* view = new( ELeave ) CAknExSettingListView;
    CleanupStack::PushL( view );
	view->iPhotosAppUi = this;
    AddViewL( view ); // transfer ownership to CAknViewAppUi
    CleanupStack::Pop(); // view
/*
	CSetupWizardListView* setupview = new( ELeave ) CSetupWizardListView;
    CleanupStack::PushL( setupview );
	setupview->iPhotosAppUi = this;
    AddViewL( setupview ); // transfer ownership to CAknViewAppUi
    CleanupStack::Pop(); // view

	CAknExContactsSettingListView* contactsview = new( ELeave ) CAknExContactsSettingListView;
    CleanupStack::PushL( contactsview );
	contactsview->iContactsAppUi = this;
    AddViewL( contactsview ); // transfer ownership to CAknViewAppUi
    CleanupStack::Pop(); // view

	CAknExPhotosSettingListView* photosview = new( ELeave ) CAknExPhotosSettingListView;
    CleanupStack::PushL( photosview );
	photosview->iContactsAppUi = this;
    AddViewL( photosview );// transfer ownership to CAknViewAppUi
    CleanupStack::Pop();   // view

	CAknExSmsSettingListView* smsview = new( ELeave ) CAknExSmsSettingListView;
    CleanupStack::PushL( smsview );
	smsview->iContactsAppUi = this;
    AddViewL( smsview ); // transfer ownership to CAknViewAppUi
    CleanupStack::Pop(); // view
*/
	CTestFileView2* view2 = new (ELeave) CTestFileView2;
    CleanupStack::PushL( view2 );
    view2->ConstructL();
    AddViewL( view2 );     // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();   // view2

	CPhotoListView* photolist = new (ELeave) CPhotoListView;
    CleanupStack::PushL( photolist );
	photolist->iPhotosAppUi = this;
    photolist->ConstructL();
    AddViewL( photolist ); // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();   // photolist


	CFindBoxView* findview = new (ELeave) CFindBoxView;
    CleanupStack::PushL( findview );
	findview->iPhotosAppUi = this;
    findview->ConstructL();
    AddViewL( findview );  // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();

	CHelpView* help = new (ELeave) CHelpView;
    CleanupStack::PushL( help );
	help->iPhotosAppUi = this;
    help->ConstructL();
    AddViewL( help );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    

	CManualSmsListView* manualsms = new (ELeave) CManualSmsListView;
    CleanupStack::PushL( manualsms );
	manualsms->iPhotosAppUi = this;
    manualsms->ConstructL();
    AddViewL( manualsms );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();
    
	CSelectiveSmsListView* selectivesms = new (ELeave) CSelectiveSmsListView;
    CleanupStack::PushL( selectivesms );
	selectivesms->iPhotosAppUi = this;
    selectivesms->ConstructL();
    AddViewL( selectivesms );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();

	/*CMainScreenListView* mainScreen = new (ELeave) CMainScreenListView;
    CleanupStack::PushL( mainScreen );
	mainScreen->iPhotosAppUi = this;
    mainScreen->ConstructL();
    AddViewL( mainScreen );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();

	CMainSubScreenListView* mainSubScreen = new (ELeave) CMainSubScreenListView;
    CleanupStack::PushL( mainSubScreen );
	mainSubScreen->iPhotosAppUi = this;
    mainSubScreen->ConstructL();
    AddViewL( mainSubScreen );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();
*/

	if(RegFileExists())	
	{
		DefaultAccessPoint();

		ReadUserName();

		//DeleteFolderFromDB(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM_OTHER)));
		//CheckAndSaveFolderToDB(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));

		iPrevViewId = KViewIdStatusScreen;
		SetDefaultViewL(*RegView);

	}
	else
	{

		iPrevViewId = KViewIdUnregisterScreen;
		SetDefaultViewL(*UnRegView);
	}

	iMainScreenValue = 1;

#endif // ifdef __UIQ__  

	iContactsFlag = 0;

	if(ReadImsiNumber())
	{
		if (AutoStartFileExists() && RegFileExists())
			Show=2;
		else
			Show=1;
	}
}

CRippleVaultAppUi::CRippleVaultAppUi()
{
    //iScheduled = 0;
}

CRippleVaultAppUi::~CRippleVaultAppUi()
{
#ifdef __UIQ__
#ifdef __S80__
	if (iUnRegisterView)
    {
        RemoveFromStack(iUnRegisterView);
        delete iUnRegisterView;
        iUnRegisterView = NULL;
    }
    
/*	if (iRegisterView)
	{
		RemoveFromStack(iRegisterView);
		delete iRegisterView;
        iRegisterView = NULL;
	}
*/
	if (iStatusScreenView)
    {
        RemoveFromStack(iStatusScreenView);
        delete iStatusScreenView;
        iStatusScreenView = NULL;
    }

	if (iSmsListView)
    {
		RemoveFromStack(iSmsListView);
        delete iSmsListView;
        iSmsListView = NULL;
    }

	if (iPhotoListView)
    {
		RemoveFromStack(iPhotoListView);
        delete iPhotoListView;
        iPhotoListView = NULL;
    }

	if(iProgressBall != NULL)
	{
		delete iProgressBall;
		iProgressBall = NULL;
	}
    
	if (iContactsListView)
	{
		RemoveFromStack(iContactsListView);
		delete iContactsListView;
        iContactsListView = NULL;
	}

	if (iVideoListView)
    {
		RemoveFromStack(iVideoListView);
        delete iVideoListView;
        iVideoListView = NULL;
    }

	if( iPhotosEngine3 != NULL)
	{
		delete iPhotosEngine3;
   		iPhotosEngine3=NULL;
	}
	if( iPhotosEngine4 != NULL)
	{
		delete iPhotosEngine4;
   		iPhotosEngine4=NULL;
	}
	if( iVideosEngine3 != NULL)
	{
   		delete iVideosEngine3;
   		iVideosEngine3=NULL;
	}

	if( iVideosEngine4 != NULL)
	{
   		delete iVideosEngine4;
   		iVideosEngine4=NULL;
	}
#else
	if( iPhotosEngine3 != NULL)
	{
		delete iPhotosEngine3;
   		iPhotosEngine3=NULL;
	}
	if( iPhotosEngine4 != NULL)
	{
		delete iPhotosEngine4;
   		iPhotosEngine4=NULL;
	}
	if( iVideosEngine3 != NULL)
	{
   		delete iVideosEngine3;
   		iVideosEngine3=NULL;
	}

	if( iVideosEngine4 != NULL)
	{
   		delete iVideosEngine4;
   		iVideosEngine4=NULL;
	}

	if (iUnRegisterView)
    {
        RemoveFromStack(iUnRegisterView);
        delete iUnRegisterView;
        iUnRegisterView = NULL;
    }

	/*if (iMainScreenView)
    {
        RemoveFromStack(iMainScreenView->ListBox());
        RemoveFromStack(iMainScreenView);
        delete iMainScreenView;
        iMainScreenView = NULL;
    }*/

	if (iPhotoListView)
	{
		RemoveFromStack(iPhotoListView->ListBox());
		RemoveFromStack(iPhotoListView);
		delete iPhotoListView;
		iPhotoListView = NULL;
	}

	if (iVideoListView)
	{
		RemoveFromStack(iVideoListView->ListBox());
		RemoveFromStack(iVideoListView);
		delete iVideoListView;
		iVideoListView = NULL;
	}

	if (iSmsListView)
	{
		RemoveFromStack(iSmsListView->ListBox());
		RemoveFromStack(iSmsListView);
		delete iSmsListView;
		iSmsListView = NULL;
	}

	if (iStatusScreenView)
	{
		RemoveFromStack(iStatusScreenView);
		delete iStatusScreenView;
		iStatusScreenView = NULL;
	}

	if (iContactsScreenView)
	{
		RemoveFromStack(iContactsScreenView);
		delete iContactsScreenView;
		iContactsScreenView = NULL;
	}
#endif
#endif
	if( iSocketWriterPublish != NULL)
	{
		delete iSocketWriterPublish;
		iSocketWriterPublish = NULL;
	}
	if( iSocketWriterShare != NULL)
	{
		delete iSocketWriterShare;
		iSocketWriterShare = NULL;
	}
	if( iSocketWriterSMS != NULL)
	{
		delete iSocketWriterSMS;
		iSocketWriterSMS = NULL;
	}
	/*if( iSocketWriterUpgrade != NULL)
	{
		delete iSocketWriterUpgrade;
		iSocketWriterUpgrade = NULL;
	}*/

   	if( iPhotosEngine1 != NULL)
	{
		delete iPhotosEngine1;
   		iPhotosEngine1=NULL;
	}
	if( iPhotosEngine2 != NULL)
	{
		delete iPhotosEngine2;
   		iPhotosEngine2=NULL;
	}

	if( iPhotosTimer != NULL)
	{
		delete iPhotosTimer;
   		iPhotosTimer=NULL;
	}

	if( iContactsTimer != NULL)
	{
		delete iContactsTimer;
   		iContactsTimer=NULL;
	}	

	if( iSmsBackupTimer != NULL)
	{
		delete iSmsBackupTimer;
   		iSmsBackupTimer=NULL;
	}

	if( iWriteSmsAO != NULL)
	{
		delete iWriteSmsAO;
   		iWriteSmsAO=NULL;
	}

	/*if( iSmsBackupAO != NULL)
	{
		delete iSmsBackupAO;
   		iSmsBackupAO=NULL;
	}*/

	if( iVideosEngine1 != NULL)
	{
   		delete iVideosEngine1;
   		iVideosEngine1=NULL;
	}

	if( iVideosEngine2 != NULL)
	{
   		delete iVideosEngine2;
   		iVideosEngine2=NULL;
	}

	if( iPhotosDB != NULL)
	{
		delete iPhotosDB;
		iPhotosDB = NULL;
	}

	if(iSmsFile) 
	{
		delete iSmsFile;
	}

	if(iSmsSimEngine != NULL)
	{
		iSmsSimEngine->Disconnect();		
		delete iSmsSimEngine;
		iSmsSimEngine = NULL;
	}

/*	if(iBillingEngine != NULL)
	{
		iBillingEngine->Disconnect();		
		delete iBillingEngine;
		iBillingEngine = NULL;
	}
*/

#ifdef __LOGME__	
	LogFile.Close();
	iLogSession.Close();
#endif

}

void CRippleVaultAppUi::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
{
#ifdef __UIQ__
//#ifdef __S80__
//#else
	//if(aResourceId == R_STATUSSCREEN_MENU)
	if(aResourceId == R_CASCADE_OTHER_LIST)
	{
/*		TBool pending;

		pending = EFalse;

		if(((iPendingCountContacts + iPendingCountPhotos +
			iPendingCountVideos) > 0) && (iStatusCode == NETWORK_STATUS_RUNNING || iStatusCode == NETWORK_STATUS_FAILED))
			pending = ETrue;

		if(!isQueuedInfo)
			pending = EFalse;
*/
		//aMenuPane->SetItemDimmed(ESendToWeb, !pending);

		//aMenuPane->SetItemDimmed(EAboutErrorCode, !isError);

		aMenuPane->SetItemDimmed(EUpgradeClient, !isUpgrade);

		TBool queued;

		if(iPhotosDB->PhotosRowCount() == 0 && iPhotosDB->VideosRowCount() == 0)
			queued = ETrue;
		else
			queued = EFalse;

		aMenuPane->SetItemDimmed(EQueueList, queued );

		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KResumeFile);

		RFs fs1;

		fs1.Connect();
		
		if (BaflUtils::FileExists(fs1, path)) //suspended
			aMenuPane->SetItemTextL(ESuspend, R_TEXT_SUSPEND);
		else
			aMenuPane->SetItemTextL(ESuspend, R_TEXT_RESUME);

		fs1.Close();

		//aMenuPane->SetItemDimmed(EShowUploadCount, !isQueuedInfo);

		//aMenuPane->SetItemDimmed(EClearMediaQueue, !(iPendingCountPhotos+iPendingCountVideos) || !isQueuedInfo);
	}
//#endif
#endif
}

void CRippleVaultAppUi::HandleCommandL(TInt aCommand)
{
#ifdef __UIQ__
    TInt err;
	CEikDialog* dialog;	
	switch (aCommand)
	{
		case EAdvanced: 													
			dialog = new (ELeave) CAdvancedDialog(*this);
			err = dialog->ExecuteLD(R_ADVANCED_DIALOG);						
			if(err!=0)
			{
				if(!RegFileExists())
			 	{
			 		break;
			 	}
   			}
			break;
		case EAccessPointSetting:
			{
				AccessPointSettings(); 
			}
			break;
		case ESharingClient:
			{
				MailNames.Copy(_L(""));
				MailId.Copy(_L(""));
				albumName.Copy(_L(""));
				iMessage.Copy(_L(""));
				iStoragePath = 3;

				ShowWindow(KViewIdContactsViewScreen);
				DeleteWindow(KViewIdStatusScreen);
			}
			break;
		case EShareAlbum:
			{
				MailNames.Copy(_L(""));
				MailId.Copy(_L(""));
				albumName.Copy(_L(""));
				iMessage.Copy(_L(""));
				iStoragePath = 2;

				ShowWindow(KViewIdContactsViewScreen); 
				DeleteWindow(KViewIdStatusScreen);
			}
			break;
		case TBack:
		case TScreenBack:
			{
				//ShowWindow(0);
				//DeleteWindow(1);
				//SendToBackground();
				CaseMinimize();
			}
			break;	
		case EChangePassword:
			{
				dialog = new (ELeave) CChangePassword(*this);
				err = dialog->ExecuteLD(R_CHANGE_PASSWORD);
				break;
			}
		case TRVHelp:
			{/*				
				HBufC* myTitle = iEikonEnv->AllocReadResourceLC(R_HELP_DIALOG_TITLE);				
				TBuf<1024> text;				
				text.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_RIPPLE_VAULT)));
				iEikonEnv->InfoWinL(*myTitle ,text );			
				CleanupStack::PopAndDestroy(1);*/
				
#ifdef __S80__
				CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_HELP_DIALOG_TITLE)), *(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_RIPPLE_VAULT)));
#else
				
				//iEikonEnv
				CEikDialog* dialog;
				dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_RIPPLE_VAULT)), R_HELP_DIALOG_TITLE);
				dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif
			}
			break;
		case TCBHelp:
			{/*
				HBufC* myTitle = iEikonEnv->AllocReadResourceLC(R_HELP_DIALOG_TITLE);				
				TBuf<1024> text;				
				text.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_CONTACTSBACKUP)));
				iEikonEnv->InfoWinL(*myTitle ,text );			
				CleanupStack::PopAndDestroy(1);*/

#ifdef __S80__
				CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_HELP_DIALOG_TITLE)), *(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_CONTACTSBACKUP)));
#else
				CEikDialog* dialog;
				dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_CONTACTSBACKUP)), R_HELP_DIALOG_TITLE);
				dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif
			}
			break;
		case TRCHelp:
			{
				/*HBufC* myTitle = iEikonEnv->AllocReadResourceLC(R_HELP_DIALOG_TITLE);				
				TBuf<1024> text;				
				text.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_RESTORECONTACTS)));
				iEikonEnv->InfoWinL(*myTitle ,text );			
				CleanupStack::PopAndDestroy(1);*/
#ifdef __S80__
				CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_HELP_DIALOG_TITLE)), *(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_RESTORECONTACTS)));
#else
				CEikDialog* dialog;
				dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_RESTORECONTACTS)), R_HELP_DIALOG_TITLE);
				dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif
			}
			break;
		case TPBHelp:
			{
				/*HBufC* myTitle = iEikonEnv->AllocReadResourceLC(R_HELP_DIALOG_TITLE);				
				TBuf<1024> text;				
				text.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_PUBLISHBACKUP)));
				iEikonEnv->InfoWinL(*myTitle ,text );			
				CleanupStack::PopAndDestroy(1);*/
#ifdef __S80__
				CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_HELP_DIALOG_TITLE)), *(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_PUBLISHBACKUP)));
#else
				CEikDialog* dialog;
				dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_PUBLISHBACKUP)), R_HELP_DIALOG_TITLE);
				dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif
			}
			break;
		case TSMSHelp:
			{
				/*HBufC* myTitle = iEikonEnv->AllocReadResourceLC(R_HELP_DIALOG_TITLE);				
				TBuf<1024> text;				
				text.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_SMSBACKUP)));
				iEikonEnv->InfoWinL(*myTitle ,text );			
				CleanupStack::PopAndDestroy(1);*/
#ifdef __S80__
				CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_HELP_DIALOG_TITLE)), *(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_SMSBACKUP)));
#else
				CEikDialog* dialog;
				dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_SMSBACKUP)), R_HELP_DIALOG_TITLE);
				dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif
			}
			break;
		case TVPVHelp:
			{
				/*HBufC* myTitle = iEikonEnv->AllocReadResourceLC(R_HELP_DIALOG_TITLE);				
				TBuf<1024> text;				
				text.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_VIEWPHOTOSVIDEOS)));
				iEikonEnv->InfoWinL(*myTitle ,text );			
				CleanupStack::PopAndDestroy(1);*/
#ifdef __S80__
				CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_HELP_DIALOG_TITLE)), *(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_VIEWPHOTOSVIDEOS)));
#else
				CEikDialog* dialog;
				dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_TEXT_HELP_VIEWPHOTOSVIDEOS)), R_HELP_DIALOG_TITLE);
				dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif
			}
			break;
/*		case ESendToWeb:
			{
				if(iPendingCountContacts>0)
				{
					UpdateContactsManually();
				}
				if(iPendingCountPhotos>0 || iPendingCountVideos>0)
				{
					ScheduleToSend();

				}
				
			}
			break;
		case EAboutErrorCode:
			{
				if(isError)
				{
					switch(iErrCode)
					{
						case NETWORK_ERRCODE_PARSING:
							ShowMessageBoxFromResource(R_TEXT_NETWORK_ERRCODE_PARSING);
							break;
						case NETWORK_ERRCODE_FAILED:
							ShowMessageBoxFromResource(R_TEXT_NETWORK_ERRCODE_FAILED);
							break;
						case NETWORK_ERRCODE_TIMEDOUT:
							ShowMessageBoxFromResource(R_TEXT_NETWORK_ERRCODE_TIMEDOUT);
							break;
						case NETWORK_ERRCODE_MSISDNMISMATCH:
							ShowMessageBoxFromResource(R_TEXT_NETWORK_ERRCODE_MSISDNMISMATCH);
							break;
						case NETWORK_ERRCODE_IMEIMISMATCH:
							ShowMessageBoxFromResource(R_TEXT_NETWORK_ERRCODE_IMEIMISMATCH);
							break;
						case NETWORK_ERRCODE_ACCSUSPENDED:
							ShowMessageBoxFromResource(R_TEXT_NETWORK_ERRCODE_ACCSUSPENDED);
							break;
						default:
							break;
					}			
				}
			}
			break;
		case EHelp:
			{
				ShowMessageBoxFromResource(R_STATUSTEXT_HELP);
			}
			break;
		case EShowUploadCount:
			{
				TBuf<250> uploadcount;

				TBuf<50> message;
				TBuf<50> formatBuf;
				TInt album;
				TInt photos;
				TInt videos;
				TInt albumshares;
				//TInt photoshares;
				//TInt videoshares;
				TInt ccount;
				TInt smscount;
				
				GetUploadSettingsFromDB(album, photos, videos, albumshares);
				GetContactsUploadSettingsFromDB(ccount);
				GetSmsUploadSettingsFromDB(smscount);

				message.Copy(_L(""));
				formatBuf.Copy(_L(""));
				formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_CONTACTS_UPLOADED)));
				message.Format(formatBuf, ccount);
				
				uploadcount.Append(message);
				uploadcount.Append(_L("\n"));

				message.Copy(_L(""));
				formatBuf.Copy(_L(""));
				formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_SMS_UPLOADED)));
				message.Format(formatBuf, smscount);

				uploadcount.Append(message);
				uploadcount.Append(_L("\n"));
				
				message.Copy(_L(""));
				formatBuf.Copy(_L(""));
				formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_ALBUMS)));
				message.Format(formatBuf, album, albumshares);

				uploadcount.Append(message);
				uploadcount.Append(_L("\n"));

				message.Copy(_L(""));
				formatBuf.Copy(_L(""));
				formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_PHOTOS_UPLOADED)));
				message.Format(formatBuf, photos);

				uploadcount.Append(message);
				uploadcount.Append(_L("\n"));

				message.Copy(_L(""));
				formatBuf.Copy(_L(""));
				formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_VIDEOS_UPLOADED)));
				message.Format(formatBuf, videos);

				uploadcount.Append(message);
				//uploadcount.Append(_L("\n"));

				//CEikonEnv::Static()->InfoWinL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),uploadcount);
				CEikDialog* dialog;
				dialog = new (ELeave) CCustomMessageDialog(uploadcount, R_EXAMPLE_RIPPLE_PUBLISH);
				dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
			}
			break;*/
		case EUpgradeClient:
			{
				CopyUpgradeFile();
			}
			break;
		case EClearMediaQueue:
			{
				ClearMediaQueue();
			}
			break;
#ifdef __S80__
		case EEikCmdExit:
		case TStop:
			//Exit();
			User::Exit(EEikCmdExit);
			//PromptForSecureExit();
			break;
			//iImageFlag = 1;
			//ShowSettingsDialog();
			//ShowWindow(1);
			//break;
		case ERegister:
			AutoRegisterRipple();
			//ShowRegisterDialog();
			//break;
			break;
		/*case EMainScreenOpen:
			{
				ActivateWindow();				
			}
			break;			*/
		case ESmsScreenBack:
			{
				ShowWindow(KViewIdStatusScreen);
				DeleteWindow(KViewIdSmsList);

				SaveChoice(1);
			}
			break;
		case EPhotoScreenBack:
			{	
				/*TInt8 choice=0;
				GetChoice(choice);
				*/
				if(iPhotoListView != NULL)
				{/*
					if(choice == 0)
					{
						ShowWindow(KViewIdVideoList);
						DeleteWindow(KViewIdPhotoList);
					}
					else
					{*/
						ShowWindow(KViewIdStatusScreen);
						DeleteWindow(KViewIdPhotoList);
					//}
				}
				else if(iVideoListView != NULL)
				{
					ShowWindow(KViewIdStatusScreen);
					DeleteWindow(KViewIdVideoList);

					//if(choice == 0)
					//{
					//	CallSetupWizardStep12();
					//}
				}
			}
			break;
		case EPhotoSelect:
			{
				if(iPhotoListView != NULL)
					iPhotoListView->MarkUnMarkSelectedItem();
				else if(iVideoListView != NULL)
					iVideoListView->MarkUnMarkSelectedItem();
			}
			break;
		case EPhotoSelectAll:
			{
				if(iPhotoListView != NULL)
					iPhotoListView->MarkAllItems();
				else if(iVideoListView != NULL)
					iVideoListView->MarkAllItems();
			}
			break;
		case EPhotoUnSelectAll:
			{
				if(iPhotoListView != NULL)
					iPhotoListView->UnMarkAllItems();
				else if(iVideoListView != NULL)
					iVideoListView->UnMarkAllItems();
			}
			break;
		case EPhotoSelectFolder:
			{
				if(iPhotoListView != NULL)
					iPhotoListView->SelectFolder();
				else if(iVideoListView != NULL)
					iVideoListView->SelectFolder();
			}
			break;
		case ESmsSelect:
			{
				if(iSmsListView != NULL)
					iSmsListView->MarkUnMarkSelectedItem();
			}
			break;
		case ESmsSelectAll:
			{
				if(iSmsListView != NULL)
					iSmsListView->MarkAllItems();
			}
			break;
		case ESmsUnSelectAll:
			{
				if(iSmsListView != NULL)
					iSmsListView->UnMarkAllItems();
			}
			break;
		case ESendSel:
			{
				if(iSmsListView != NULL)
					iSmsListView->SendSelected();
				else if(iPhotoListView != NULL)
				{
					TInt PhotoSendMode;				
					PhotoSendMode = iPhotoListView->IsImageSelected();
					
					if(PhotoSendMode == 1)
					{
						iPhotoListView->SendPhotoToServer();
					}
					else
					{
						ShowMessageBoxFromResource(R_EXAMPLE_TEXT_NOTSEL);
					}
				}
				else if(iVideoListView != NULL)
				{
					TInt PhotoSendMode;
					PhotoSendMode = iVideoListView->IsImageSelected();

					if(PhotoSendMode == 1)
					{
						iVideoListView->SendVideoToServer();
					}
					else
					{
						ShowMessageBoxFromResource(R_EXAMPLE_TEXT_NOTSEL1);
					}
				}
			}
			break;
		case EContactsAdd:
			{
				if(iContactsListView != NULL)
					iContactsListView->AddContacts();
			}
			break;
		case EContactsDel:
			{
				if(iContactsListView != NULL)
					iContactsListView->DeleteContacts();
			}
			break;
		case EContactsBack:
			{
				ShowWindow(KViewIdStatusScreen);
				DeleteWindow(KViewIdContactsViewScreen);
			}
			break;
		case EContactsSend:
			{
				if(iContactsListView != NULL)
					iContactsListView->ShareContacts();
			}
			break;
//// NEW UI
		case EBackupPhoneBook:
			{
				UpdateContactsManually();
			}
			break;
		case EBackupPhotos:
			{
				ShowWindow(KViewIdPhotoList);
				DeleteWindow(KViewIdStatusScreen); //main screen
			}
			break;
		case EBackupVideos:
			{
				ShowWindow(KViewIdVideoList);
				DeleteWindow(KViewIdStatusScreen); //main screen
			}
			break;
		case EBackupSMS:
			{
				if(PendingSMS())
				{
					ShowMessageBoxFromResource(R_SMS_SENDTOWEB_BUSY);
				}
				else{
					//manually sms backup		
					ShowWindow(KViewIdSmsList);
					DeleteWindow(KViewIdStatusScreen); //main screen
				}
			}
			break;
		case ESettingsPhoneBook:
			{
				dialog = new (ELeave) CContactsSettingDialog(*this);
				err = dialog->ExecuteLD(R_CONTACTS_SETTINGS);
				if(err)
				{
					if(iSettingsScreenValue == 1) 
					{
						dialog = new (ELeave) CContactsUploadSettingDialog(*this);
						err = dialog->ExecuteLD(R_CONTACTS_UPLOAD_SETTINGS);
					}
				}
			}
			break;
		case ESettingsPhotos:
			{
				dialog = new (ELeave) CPhotosSettingDialog(*this);
				err = dialog->ExecuteLD(R_PHOTOS_SETTINGS);	
			}
			break;
		case ESettingsSMS:
			{
				dialog = new (ELeave) CSmsSettingDialog(*this);
				err = dialog->ExecuteLD(R_SMS_SETTINGS);	

				if(err)
				if(iSmsMode == 0)
				{
					MailNames.Copy(_L(""));
					MailId.Copy(_L(""));
					albumName.Copy(_L(""));
					iMessage.Copy(_L(""));
					iStoragePath = 1;

					ShowWindow(KViewIdContactsViewScreen); 
					DeleteWindow(KViewIdStatusScreen); 
				}
			}
			break;
		case ESettingsAntitheft:
			{
				//iPhotosAppUi->iSettingsScreenValue = 4;
				//iPhotosAppUi->ActivateView(KViewIdSettingsScreen);
				ShowMessageBoxFromResource(R_ANTITHEFT_INTEREST); 
			}
			break;
		case ESettingsGeneral:
			{
				dialog = new (ELeave) CAdvancedDialog(*this);
				err = dialog->ExecuteLD(R_ADVANCED_DIALOG);						
				if(err!=0)
				{
					if(!RegFileExists())
			 		{
			 			break;
			 		}
   				}
				break;
			}
		case ESuspend:
			{
				CaseSuspend();
			}
			break;
#else
		case EEikCmdExit:
			User::Exit(EEikCmdExit);
			//PromptForSecureExit();
			//Exit();
			break;
		case EPhotoSelId: 
			{
				if(iImageFlag == 1)
				{
					iImageFlag = 2;
					iPhotoListView->ResetData();

					CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
					appUi->SetToolbarL(R_PHOTOLIST_TOOLBAR_MEMORY);

					TInt err;
					err = iPhotoListView->Start(KDirMMCPhoto);
					if(err == -1)
						ShowMessageBoxFromResource(R_WEBUPLOAD_NOPHOTOIMAGE);
				}
				else if(iImageFlag == 2)
				{
					iImageFlag = 1;
					iPhotoListView->ResetData();
			
					CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
					appUi->SetToolbarL(R_PHOTOLIST_TOOLBAR_PHONE);

					TInt err;
					err = iPhotoListView->Start(KDirPhonePhoto);
					if(err == -1)
						ShowMessageBoxFromResource(R_WEBUPLOAD_NOPHOTOIMAGE);
				}
				else if(iImageFlag == 3)
				{
					iImageFlag = 4;
					iVideoListView->ResetData();

					CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
					appUi->SetToolbarL(R_PHOTOLIST_TOOLBAR_MEMORY);

					TInt err;
					err = iVideoListView->Start(KDirMMCVideo);
					if(err == -1)
						ShowMessageBoxFromResource(R_WEBUPLOAD_NOVIDEOIMAGE);
				}
				else if(iImageFlag == 4)
				{
					iImageFlag = 3;
					iVideoListView->ResetData();
			
					CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
					appUi->SetToolbarL(R_PHOTOLIST_TOOLBAR_PHONE);

					TInt err;
					err = iVideoListView->Start(KDirPhoneVideo);
					if(err == -1)
						ShowMessageBoxFromResource(R_WEBUPLOAD_NOVIDEOIMAGE);
				}
			}
			break;	
		case ESmsSelId: 
			{
				if(iImageFlag == 5)
				{
					iImageFlag = 6;

					CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
					appUi->SetToolbarL(R_SMSLIST_TOOLBAR_MEMORY);

					iSmsListView->ChangeScreen();

					iSmsListView->ResetData();

					/*if(iSmsListView->iSentItemsFlag == EFalse)
					{
						iSmsListView->iSentItemsFlag = ETrue;

						iSmsListView->Start(1);
					}
					else
					{						
						iSmsListView->ResetData();
					}*/
				}
				else if(iImageFlag == 6)
				{
					iImageFlag = 5;
			
					CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
					appUi->SetToolbarL(R_SMSLIST_TOOLBAR_PHONE);

					iSmsListView->ChangeScreen();

					iSmsListView->ResetData();

					/*if(iSmsListView->iInboxItemsFlag == EFalse)
					{
						iSmsListView->iInboxItemsFlag = ETrue;
						iSmsListView->Start(0);
					}
					else
					{						
						iSmsListView->ResetData();
					}*/
				}
			}
			break;			
		case TListBack:
			{
				TInt val = 0;
				if(iImageFlag == 1 || iImageFlag == 2)
				{
					val = iPhotoListView->IsImageSelected();
				}
				else if(iImageFlag == 3 || iImageFlag == 4)
				{
					val = iVideoListView->IsImageSelected();
				}
				else if(iImageFlag == 5 || iImageFlag == 6)
				{
					val = iSmsListView->IsSmsSelected();
				}

				if (val == 1)
				{
					if(CEikonEnv::Static()->QueryWinL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),*(iEikonEnv->AllocReadResourceL(R_TEXT_CANCEL_BACKUP))))
						ShowView();
				}
				else
					ShowView();
			}
			break;
		case EMarkSel:
			{
				if(iImageFlag == 1 || iImageFlag == 2)
					iPhotoListView->MarkUnMarkSelectedItem();
				else if(iImageFlag == 3 || iImageFlag == 4)
					iVideoListView->MarkUnMarkSelectedItem();
				else if(iImageFlag == 5 || iImageFlag == 6)
					iSmsListView->MarkUnMarkSelectedItem();
			}
			break;
		case EMarkAll:
			{
				if(iImageFlag == 1 || iImageFlag == 2)
					iPhotoListView->MarkAllItems();
				else if(iImageFlag == 3 || iImageFlag == 4)
					iVideoListView->MarkAllItems();
				else if(iImageFlag == 5 || iImageFlag == 6)
					iSmsListView->MarkAllItems();
			}
			break;
		case EUnMarkAll:
			{
				if(iImageFlag == 1 || iImageFlag == 2)
					iPhotoListView->UnMarkAllItems();
				else if(iImageFlag == 3 || iImageFlag == 4)
					iVideoListView->UnMarkAllItems();
				else if(iImageFlag == 5 || iImageFlag == 6)
					iSmsListView->UnMarkAllItems();
			}
			break;
		case ESendSel:
			{
				TInt PhotoSendMode;				
				if(iImageFlag == 1 || iImageFlag == 2)
				{
					if(iPhotoListView->GetCount() == 0)
						iPhotoListView->MarkSelectedPhoto();

					PhotoSendMode = iPhotoListView->IsImageSelected();
					
					if(PhotoSendMode == 1)
					{
						TBuf<150> msg;
						
						TInt count = iPhotoListView->GetCount();

						msg.Copy(_L(""));
						msg.AppendNum(count);

						if(iImageFlag == 1 || iImageFlag == 2)			
							msg.Append(*(iEikonEnv->AllocReadResourceL(R_IMAGES_CONFIRM_PHOTOS)));
						else if(iImageFlag == 3 || iImageFlag == 4)
							msg.Append(*(iEikonEnv->AllocReadResourceL(R_IMAGES_CONFIRM_VIDEOS)));

						msg.Append(*(iEikonEnv->AllocReadResourceL(R_IMAGES_CONFIRM)));

						if(CEikonEnv::Static()->QueryWinL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),msg))
							iPhotoListView->SendPhotoToServer();
					}
					else
					{
						ShowMessageBoxFromResource(R_EXAMPLE_TEXT_NOTSEL);
					}
				}
				else if(iImageFlag == 3 || iImageFlag == 4)
				{
					if(iVideoListView->GetCount() == 0)
						iVideoListView->MarkSelectedVideo();

					PhotoSendMode = iVideoListView->IsImageSelected();

					if(PhotoSendMode == 1)
					{
						TBuf<150> msg;
						
						TInt count = iVideoListView->GetCount();

						msg.Copy(_L(""));
						msg.AppendNum(count);

						if(iImageFlag == 1 || iImageFlag == 2)			
							msg.Append(*(iEikonEnv->AllocReadResourceL(R_IMAGES_CONFIRM_PHOTOS)));
						else if(iImageFlag == 3 || iImageFlag == 4)
							msg.Append(*(iEikonEnv->AllocReadResourceL(R_IMAGES_CONFIRM_VIDEOS)));

						msg.Append(*(iEikonEnv->AllocReadResourceL(R_IMAGES_CONFIRM)));

						if(CEikonEnv::Static()->QueryWinL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),msg))
							iVideoListView->SendVideoToServer();
					}
					else
					{
						ShowMessageBoxFromResource(R_EXAMPLE_TEXT_NOTSEL1);
					}
				}
				else if(iImageFlag == 5 || iImageFlag == 6)
				{
					if(iSmsListView->GetCount() == 0)
						iSmsListView->MarkSelectedSms();

					PhotoSendMode = iSmsListView->IsSmsSelected();

					if(PhotoSendMode == 1)
					{
						TBuf<150> msg;
						
						TInt count = iSmsListView->GetCount();

						msg.Copy(_L(""));
						msg.AppendNum(count);

						msg.Append(*(iEikonEnv->AllocReadResourceL(R_SMS_CONFIRM)));
						msg.Append(*(iEikonEnv->AllocReadResourceL(R_IMAGES_CONFIRM)));
						
						if(CEikonEnv::Static()->QueryWinL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),msg))
							iSmsListView->SendSmsToServer();
					}
					else
					{
						ShowMessageBoxFromResource(R_TEXT_SELECT_MESSAGE_FIRST);
					}
				}
				break;
			}
			break;
		/*case TScreenBack:
			{
				if(iMainScreenValue == 1 || iMainScreenValue == 2 || iMainScreenValue == 3
					|| iMainScreenValue == 4 || iMainScreenValue == 5)
				{
					iMainScreenValue = 0;
					iMainScreenView->UpdateListBoxL(iMainScreenValue);
					
					CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
					appUi->SetToolbarL(R_REGISTER_TOOLBAR);
				}
				else if(iMainScreenValue == 6)
				{
					iMainScreenValue = 0;
					ShowWindow(KViewIdRegisterScreen); //main screen
					DeleteWindow(KViewIdStatusScreen); //status screen 
				}
			}
			break;	*/
		case TRegister: 													
		  	dialog = new (ELeave) CRegisterDialog(*this);
			err = dialog->ExecuteLD(R_REGISTER_DIALOG);
			if(err!=0)
			{				
			}	
			break;
		case TAlreadyRegister:
			{
				AutoRegisterRipple();
				//AlbumSynchingComplete();
			}
			break;	
		case EViewAlbum:
			OpenBrowser();
			break;
/*		case EChangePlan:
			{
				ChangePlan();				
			}
			break;*/
		case ESharing:
			if(iContactsScreenView != NULL)
			{
				TInt err;					
				if(iStoragePath == 4)
				{
					err = iContactsScreenView->AddNewDatatoDB();
					if(err == 1)
					{						
						iStoragePath = 1;								
			
						CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
						appUi->SetToolbarL(R_SELECTIVECONTACTSLIST_TOOLBAR);

						iContactsScreenView->ResetData();
					}
				}
				else
				{
					if(iStoragePath == 1)
					{
						if(iContactsScreenView->GetStoredContactsCount() == 0)
						{
							iEikonEnv->InfoMsg(*(iEikonEnv->AllocReadResourceL(R_SELECT_RECEPIENT)));
						}
						else
						{
						//TInt8 choice=0;
						//GetChoice(choice);
						
						/*if(choice == 0)
						{
							err = 0;
							ShowWindow(KViewIdRegisterScreen);
							DeleteWindow(KViewIdContactsViewScreen);
							CallSetupWizardStep17();
						}
						else{*/
							err = 1;
							ShowWindow(KViewIdStatusScreen);
							DeleteWindow(KViewIdContactsViewScreen);

						//}
					}
					}
					else
						err = iContactsScreenView->SendSharingXml();
					
/*					if(err == 1)
					{
						ShowWindow(1);
						DeleteWindow(6);
					}*/
				}
			}
			break;
		case ECancel:
			if(iContactsScreenView != NULL)
			{
				if(iStoragePath == 1)
				{
					iSmsMode = 1;
				    SaveSmsSettingsToDB(iSmsMode,iSmsTime,iSmsRoam);

					/*TInt8 choice=0;
					GetChoice(choice);
					
					if(choice == 0)
					{
						err = 0;
						ShowWindow(KViewIdRegisterScreen);
						DeleteWindow(KViewIdContactsViewScreen);
						CallSetupWizardStep17();
					}
					else{*/
						err = 1;
						ShowWindow(KViewIdStatusScreen);
						DeleteWindow(KViewIdContactsViewScreen);

					//}
				}
				else if(iStoragePath == 4)
				{
					iStoragePath = 1;
					iContactsScreenView->ResetData();								
			
					CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
					appUi->SetToolbarL(R_SELECTIVECONTACTSLIST_TOOLBAR);
				}
				else
				{
					ShowWindow(KViewIdStatusScreen);
					DeleteWindow(KViewIdContactsViewScreen);
				}
			}
			break;
		case ESelectAdd:
			if(iContactsScreenView != NULL)
			{				
				iStoragePath = 4;
				iContactsScreenView->AddContacts();								
			
				CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
				appUi->SetToolbarL(R_CONTACTSLIST_TOOLBAR);
			}
			break;
		case ESelectDelete:
			if(iContactsScreenView != NULL)
			{
				iContactsScreenView->DeleteContacts();
			}
			break;
		case TStop:
			//User::Exit(EEikCmdExit);
#ifdef __S80__
			User::Exit(EEikCmdExit);
#else
			PromptForSecureExit();
#endif
			break;
//// NEW UI
		case EBackupPhoneBook:
			{
				UpdateContactsManually();
			}
			break;
		case EBackupPhotos:
			{
				iImageFlag = 1;
				ShowWindow(KViewIdPhotoList);   //photo screen
				DeleteWindow(KViewIdStatusScreen); //main screen
			}
			break;
		case EBackupVideos:
			{
				iImageFlag = 3;
				ShowWindow(KViewIdVideoList);   //video screen
				DeleteWindow(KViewIdStatusScreen); //main screen
			}
			break;
		case EBackupSMS:
			{
				if(PendingSMS())
				{
					ShowMessageBoxFromResource(R_SMS_SENDTOWEB_BUSY);
				}
				else{
					//manually sms backup		
					iImageFlag = 5;
					ShowWindow(KViewIdSmsList);   //sms screen
					DeleteWindow(KViewIdStatusScreen); //main screen
				}
			}
			break;
		case ESettingsPhoneBook:
			{
				dialog = new (ELeave) CContactsSettingDialog(*this);
				err = dialog->ExecuteLD(R_CONTACTS_SETTINGS);
				if(err)
				{
					if(iSettingsScreenValue == 1) 
					{
						dialog = new (ELeave) CContactsUploadSettingDialog(*this);
						err = dialog->ExecuteLD(R_CONTACTS_UPLOAD_SETTINGS);
					}
				}
			}
			break;
		case ESettingsPhotos:
			{
				dialog = new (ELeave) CPhotosSettingDialog(*this);
				err = dialog->ExecuteLD(R_PHOTOS_SETTINGS);	
			}
			break;
		case ESettingsSMS:
			{
				dialog = new (ELeave) CSmsSettingDialog(*this);
				err = dialog->ExecuteLD(R_SMS_SETTINGS);	

				if(err)
				if(iSmsMode == 0)
				{
					MailNames.Copy(_L(""));
					MailId.Copy(_L(""));
					albumName.Copy(_L(""));
					iMessage.Copy(_L(""));
					iStoragePath = 1;

					ShowWindow(KViewIdContactsViewScreen); 
					DeleteWindow(KViewIdStatusScreen); 
				}
			}
			break;
		case ESettingsAntitheft:
			{
				//iPhotosAppUi->iSettingsScreenValue = 4;
				//iPhotosAppUi->ActivateView(KViewIdSettingsScreen);
				ShowMessageBoxFromResource(R_ANTITHEFT_INTEREST); 
			}
			break;
		case ESettingsGeneral:
			{
				dialog = new (ELeave) CAdvancedDialog(*this);
				err = dialog->ExecuteLD(R_ADVANCED_DIALOG);						
				if(err!=0)
				{
					if(!RegFileExists())
			 		{
			 			break;
			 		}
   				}
				break;
			}
		case ESuspend:
			{
				CaseSuspend();
			}
			break;
#endif
		default:
            break;
	}
#else // Series 60 code
    switch(aCommand)
        {
		case EEikCmdExit:
#ifdef __LOGME__
				 LogText.Copy(_L("EEikCmdExit"));
				 WriteLogFile(LogText);
#endif
#ifndef __NOKIA_SYM8__
			User::Exit(EEikCmdExit);
#endif
			//Exit();
			break;
		case EExit:
        case EAknSoftkeyExit://Personal Photo
			//Exit();
			PromptForSecureExit();
            break;
		case ESettings:
			iRipplePhotos = 2;
			iSettingsScreenValue = 0;
			iPrevViewId = KViewIdSettingsScreen;
			ActivateLocalViewL(KViewIdSettingsScreen);
			break;
		case EUnRegHide:
			SendToBackground();
			break;
        case EHide:
			iRipplePhotos = 2;			
			//iPrevViewId = KViewIdEmptyScreen;
			ActivateLocalViewL(KViewIdEmptyScreen);			
			SendToBackground();
			break;
		case ERegView:
			iPrevViewId = KViewIdStatusScreen;
			ActivateLocalViewL(KViewIdStatusScreen);			
			break;
		case EUnRegView:
			iPrevViewId = KViewIdUnregisterScreen ;
			ActivateLocalViewL(KViewIdUnregisterScreen);			
			break;
//		case ERecommend:
//			//ActivateLocalViewL(TUid::Uid(6));
//			break;
		case EFindView:
			iPrevViewId = KViewIdFindBox;
			ActivateLocalViewL(KViewIdFindBox);
			break;
		case Esmsbackup:
			iPrevViewId = KViewIdSmsList;
			ActivateLocalViewL(KViewIdSmsList);
			break;
//		case EMailView:
//			ActivateLocalViewL(TUid::Uid(8));
//			break;
		case EMainView:
			iPrevViewId = KViewIdStatusScreen;
			ActivateLocalViewL(KViewIdStatusScreen);
			break;
		case EMainSubView:
			iPrevViewId = KViewIdStatusScreen;
			ActivateLocalViewL(KViewIdStatusScreen);
			break;
		case EHelp:
			iPrevViewId = KViewIdHelp;
			ActivateLocalViewL(KViewIdHelp);
			break;
/*		case EAbout: 
			{			
				TBuf<80> msg;
				msg.Copy(_L("Version "));
				msg.Append(iVersion);
				CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(msg);
				abtdlg->PrepareLC(R_ABOUT_QUERY);
				if (abtdlg->RunLD()) 
				{					
				}
			break;
			}
		case EChangePlan:
			{
				ChangePlan();				
			}
			break;*/
//		case EUpgrade:
//			{
//				if(IsNetworkInProgress())					
//					CEikonEnv::Static ()->AlertWin(_L("Network Busy"));					
//				else
//					CheckUpgrade();
//			}
//			break;
		default:
            break;
        }
#endif
}

void CRippleVaultAppUi::BringToForeground()
{
#ifdef __LOGME__	
	LogText.Copy(_L("\nBringToForeground\n"));
	WriteLogFile(LogText);
#endif

    Show=0;
#ifdef __UIQ__
//	TUid id = { 3 };
//	ActivateViewL(iAppEmptyView->ViewId());
//	CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
//	appUi->SetToolbarL(R_EXAMPLE_TOOLBAR1);	
//	iAppView1->MakeVisible(EFalse);
    // Construct en empty TApaTask object
    // giving it a reference to the Window Server session
    TApaTask task(iEikonEnv->WsSession( ));
                   
    // Initialise the object with the window group id of 
    // our application (so that it represent our app)
    task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
    
    // Request window server to bring our application
    // to foreground
    task.BringToForeground();
#else
	HideView();
	ActivateLocalViewL(KViewIdEmptyScreen);

    TApaTask task(iEikonEnv->WsSession( ));
                   
    // Initialise the object with the window group id of 
    // our application (so that it represent our app)
    task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
    
    // Request window server to bring our application
    // to foreground
    task.BringToForeground();

#endif
}

void CRippleVaultAppUi::SendToBackground()
{
#ifdef __LOGME__	
	LogText.Copy(_L("\nSendToBackground\n"));
	WriteLogFile(LogText);
#endif

    Show=1;

#ifndef __UIQ__
	CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
	statusPane->SwitchLayoutL(R_AVKON_STATUS_PANE_LAYOUT_EMPTY);
#else
	//TUid id = { 3 };
	//ActivateViewL(iAppView1->ViewId());
#endif
    // Construct en empty TApaTask object
    // giving it a reference to the Window Server session
    TApaTask task(iEikonEnv->WsSession( ));
                   
    // Initialise the object with the window group id of 
    // our application (so that it represent our app)
    task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
    
    task.SendToBackground();	
}

TBool CRippleVaultAppUi::AutoStartFileExists()
{
	RFs Autofs;
	TBool ret;

	Autofs.Connect();
	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KAutoStartFile);
	if (BaflUtils::FileExists(Autofs, path)) 
	{
		BaflUtils::DeleteFile(Autofs, path);
		ret = TRUE;
	}
	else
		ret = FALSE;

	Autofs.Close();

	return ret;
}

void CRippleVaultAppUi::HandleForegroundEventL(TBool aForeground)
{
#ifndef __UIQ__
	CAknAppUi::HandleForegroundEventL(aForeground);
	
	 if(aForeground)
	 {
		 if(Show==1)
		 {
			 			 
			 CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
			 statusPane->SwitchLayoutL(R_AVKON_STATUS_PANE_LAYOUT_USUAL);		 
			 			 
			 if(RegFileExists())
			 {
				 iRipplePhotos = 0;
				 //ActivateLocalViewL(TUid::Uid(2));
		         //ActivateLocalViewL(TUid::Uid(4));
				 //ActivateLocalViewL(TUid::Uid(15));
				 //ActivateLocalViewL(KViewIdEmptyScreen);
				TInt8 choice=0;
				GetChoice(choice);

				 if(/*(iPrevViewId == KViewIdMainScreen || iPrevViewId == KViewIdSubScreen) &&*/ CheckIfPending() && choice == 1)
					ActivateLocalViewL(KViewIdStatusScreen);
				 else
					ActivateLocalViewL(iPrevViewId);

			 }
			 else{
				iPrevViewId = KViewIdUnregisterScreen;
				ActivateLocalViewL(KViewIdUnregisterScreen);
			 }
		 }	
		 else if(Show==2)
		 {	
			ActivateLocalViewL(TUid::Uid(2));
			SendToBackground();
		 }
		 else if(Show==0)
		 {
			 //HideView();
			 //ActivateLocalViewL(KViewIdEmptyScreen);
			 //HideView();
		 }
	 }
#else
	 /*if(aForeground)
	 {
	 	TInt val = ReturnScreenValue();
		if((val == KViewIdRegisterScreen) && CheckIfPending())
		{
			//CEikonEnv::Static ()->AlertWin(_L("1"));	

			iMainScreenValue = 6;
			ShowWindow(KViewIdStatusScreen); //status screen
			DeleteWindow(val);
			ShowStatusScreen();
  
		}
	 }*/
#endif
}


CDesCArray* CRippleVaultAppUi::ReturnText()
{
    CDesCArray* textArray = 
        iCoeEnv->ReadDesCArrayResourceL(R_AKNEXQUERY_SUB_POPUP_FOLDER); 
    return textArray;
}

CDesCArray* CRippleVaultAppUi::ReturnTextForImage()
{
    CDesCArray* textArray = 
        iCoeEnv->ReadDesCArrayResourceL(R_SUB_POPUP_FOLDER); 
    return textArray;
}

CDesCArray* CRippleVaultAppUi::FolderExist()
{
    CDesCArray* textArray = 
        iCoeEnv->ReadDesCArrayResourceL(R_AKNEXQUERY_FOLDER_EXIST); 
    return textArray;
}

void CRippleVaultAppUi::AddPhotoRecord(TDesC& aFilename, TInt8 aPublic, TDesC& aComment, TDesC& aDesc, TInt8 aNew, TInt aFileSize)
{
#ifdef __LOGME__
	LogText.Copy(_L("filename="));
	LogText.Append(aFilename);
	LogText.Append(_L("\n"));
	WriteLogFile(LogText);
#endif

if(!iPhotosDB->CheckIfPhotoExists(aFilename))
{
#ifdef __LOGME__
	LogText.Copy(_L("filesize="));
	LogText.AppendNum(aFileSize);
	LogText.Append(_L("\n"));
	WriteLogFile(LogText);
#endif

    // public/private type to be passed
	TInt num_packets;
	if (aFileSize % (PUBLISH_PACKET_SIZE*1024) == 0)
		num_packets = aFileSize/(PUBLISH_PACKET_SIZE*1024);
	else
		num_packets = (aFileSize/(PUBLISH_PACKET_SIZE*1024)) + 1;

	TBuf<1024> status;
	status.Copy(_L(""));

	for(TInt i=0; i< num_packets && i<1024;++i)
	{
		status.Append(_L("0"));
	}

	iPhotosDB->AddRecord(aFilename, aPublic, aComment, aDesc, KPhoto, aNew, status);
}
}

void CRippleVaultAppUi::AddVideoRecord(TDesC& aFilename, TInt8 aPublic, TDesC& aComment, TDesC& aDesc, TInt8 aNew, TInt aFileSize)
{
if(!iPhotosDB->CheckIfPhotoExists(aFilename))
{
#ifdef __LOGME__
	LogText.Copy(_L("filename="));
	LogText.Append(aFilename);
	LogText.Append(_L("\n"));
	WriteLogFile(LogText);
#endif

    // public/private type to be passed
	TInt num_packets;
	if (aFileSize % (PUBLISH_PACKET_SIZE*1024) == 0)
		num_packets = aFileSize/(PUBLISH_PACKET_SIZE*1024);
	else
		num_packets = (aFileSize/(PUBLISH_PACKET_SIZE*1024)) + 1;

	TBuf<1024> status;
	status.Copy(_L(""));

	for(TInt i=0; i< num_packets && i<1024;++i)
	{
		status.Append(_L("0"));
	}

	iPhotosDB->AddRecord(aFilename, aPublic, aComment, aDesc, KVideo, aNew, status);
}
}

void CRippleVaultAppUi::SendToServer()
{
#ifdef __LOGME__
	LogText.Copy(_L("SendToServer 1\n"));
	WriteLogFile(LogText);
#endif

	if(iSocketWriterPublish == NULL)
	{
		iSocketWriterPublish = CSocketsEngineWriter::NewL(*this);
		iSocketWriterPublish->SetServerName(iServerAddr);
		iSocketWriterPublish->SetPort(iPort);
		iSocketWriterPublish->ConnectToServer();	
#ifdef __LOGME__
		LogText.Copy(_L("SendToServer 2\n"));
		WriteLogFile(LogText);
#endif

	}
}

void CRippleVaultAppUi::ScheduleToSend()
{
#ifdef __LOGME__
	LogText.Copy(_L("ScheduleToSend 1\n"));
	WriteLogFile(LogText);
#endif

	TInt network;		
	network = CheckNetwork();
	if(network == NETWORK_ROAMING)
	{
		if(iRoaming == DISABLED)
		{
			RoamingDisabledDialogPublish();
			return;
		}
		
	}

	if( iPhotosTimer == NULL)
	{
		iPhotosTimer = CPhotosTimer::NewL(*this);
		iPhotosTimer->Start();
	}

	if (iSocketWriterPublishStatus!=NULL || iSocketWriterPublish!=NULL)//iScheduled)
    {
		return;
    }

    // Initialize send
    //iScheduled = 1;

    // get the photo/video entry from the file.
    // start sending once connected
   
	//SendToServer();

#ifdef __LOGME__
	LogText.Copy(_L("ScheduleToSend 2\n"));
	WriteLogFile(LogText);
#endif

	if(iPhotosDB->RowCount() > 0)
	{
	iConnectionStatus = 1;
	CheckSubscriptionStatus(iConnectionStatus);
	}
}

void CRippleVaultAppUi::MoreFilesToUpload()
{
	TInt count;

	count = iPhotosDB->RowCount();

#ifdef __LOGME__
	LogText.Copy(_L("MoreFilesToUpload 1\n"));
	WriteLogFile(LogText);
#endif

	while(count > 0)
	{
		TInt8 type;
		TInt8 photo;
		TInt8 isNew;	
	
		TBuf<128> afileName;
		TBuf<128> folderName;
		TBuf<128> descName;
		TBuf<1024> status;	

#ifdef __LOGME__
		LogText.Copy(_L("MoreFilesToUpload 2\n"));
		WriteLogFile(LogText);
#endif

		TInt err;
		err = iPhotosDB->GetRecord(afileName, type, folderName, descName, photo, isNew, status);

#ifdef __LOGME__
		LogText.Copy(_L("MFTU FileName="));
		LogText.Append(afileName);
		LogText.Append(_L("\n"));
		WriteLogFile(LogText);
#endif
		
		if(err == 0)
		{
			if(CheckMediaExists(afileName))
			{
#ifdef __LOGME__
				LogText.Copy(_L("Media Exists\n"));
				WriteLogFile(LogText);
#endif			
				count = -1;				
			}
			else
			{
#ifdef __LOGME__
				LogText.Copy(_L("Media Not Exists\n"));
				WriteLogFile(LogText);
#endif
				iPhotosDB->DeleteNonExistantRecord();
				count = iPhotosDB->RowCount();
			}
#ifdef __LOGME__
			LogText.Copy(_L("Media Checked\n"));
			WriteLogFile(LogText);
#endif
		}
		else
		{
#ifdef __LOGME__
			LogText.Copy(_L("File not Opened\n"));
			WriteLogFile(LogText);
#endif
			count = 0;
		}
#ifdef __LOGME__
		LogText.Copy(_L("MoreFilesToUpload 3\n"));
		WriteLogFile(LogText);
#endif
	}	

	if(iSocketWriterPublish != NULL)
	{
		iSocketWriterPublish->Disconnect();
		delete iSocketWriterPublish;		
		iSocketWriterPublish = NULL;
	}	
#ifdef __LOGME__
	LogText.Copy(_L("MoreFilesToUpload 4\n"));
	WriteLogFile(LogText);
#endif
	if(count == -1)
	{
		SendToServer();
	}
	else
	{
		CloseConnection();
		//GetAlbumSynchingXML();
	}
}

TInt CRippleVaultAppUi::CheckMediaExists(TDesC& aFilename)
{
	RFs file;
	file.Connect();
	
	if(BaflUtils::FileExists(file,aFilename))
	{
#ifdef __LOGME__
		LogText.Copy(_L("CheckMediaExists 1\n"));
		WriteLogFile(LogText);
#endif

		file.Close();
		return 1;
	}
	else
	{
#ifdef __LOGME__
		LogText.Copy(_L("CheckMediaExists 2\n"));
		WriteLogFile(LogText);
#endif

		file.Close();
		return 0;
	}
	
}

// iSocketWriter will call this first time when protocol header is
// sent completely. Calls with buffer and length. Sends the buffer
// Once send is complete, calls this function again.
// When this function returns 0, it means end of image/video file.
// Call next - GetProtocolEndDataXML, send it.
TInt CRippleVaultAppUi::GetMoreDataFromFile(TDes8& aData, TInt aMaxLen)
{
    //if (iFileDes.
    // read data from filedesc into aData

    // return the length of data read

		TInt pos = 0;
		iFileDes.Seek(ESeekCurrent,pos);

		//LogText.Copy(_L("\n\nbefore reading pos="));
		//LogText.AppendNum(pos);
		//LogText.Append(_L("\n\n"));
		//WriteLogFile(LogText);

    TInt ret = iFileDes.Read(aData, aMaxLen);
	if(ret != KErrNone || ret == KErrOverflow || ret == KErrGeneral)
	{
		return 0;
	}
    if (ret == 0)
    {
		return aData.Length();
    }
	return 0;
}

void CRippleVaultAppUi::SendBillingSMS()
{
	//From: MSISDN
	//To: short code
	//I=Subscription keyword (will be an 8 digit number) B (billing type)=MO T (type)=S (subscribe) IMSI=666666
	//	Billing Short Code: "billing_short_code="
	//	Billing Subscription Keyword: "billing_sub_keyword="
	//	Billing Type: "billing_bill_type="
	///	Billing Registration Type: "billing_reg_type="
	TBuf<30> ShortCode;
	TBuf<30> SubscriptionKeyword;
	TBuf<30> BillingType;
	TBuf<30> BillingRegType;

	ShortCode.Copy(_L(""));
	SubscriptionKeyword.Copy(_L(""));
	BillingType.Copy(_L(""));
	BillingRegType.Copy(_L(""));

	TBuf<60> tmpstr;
	tmpstr.Copy(_L("billing_short_code"));	
	LoadFromConfigFile(ShortCode, tmpstr);

	tmpstr.Copy(_L("billing_sub_keyword"));	
	LoadFromConfigFile(SubscriptionKeyword, tmpstr);

	tmpstr.Copy(_L("billing_bill_type"));	
	LoadFromConfigFile(BillingType, tmpstr);

	tmpstr.Copy(_L("billing_reg_type"));	
	LoadFromConfigFile(BillingRegType, tmpstr);

	TBuf<160> BillingSMSMessage;
	BillingSMSMessage.Copy(_L("I="));
	BillingSMSMessage.Append(SubscriptionKeyword);
	BillingSMSMessage.Append(_L(" B="));
	BillingSMSMessage.Append(BillingType);
	BillingSMSMessage.Append(_L(" T="));
	BillingSMSMessage.Append(BillingRegType);
	BillingSMSMessage.Append(_L(" IMSI="));
	BillingSMSMessage.Append(iImsiNumber);

	iSmsFile->CmdSendBillingL(ShortCode, BillingSMSMessage);
}

void CRippleVaultAppUi::SeekDataFromFile(TInt aPacketNumber)
{

		TInt pos;

		pos = (aPacketNumber)*1024*PUBLISH_PACKET_SIZE;

	    TInt ret = iFileDes.Seek(ESeekStart, pos);

		pos = 0;
		ret = iFileDes.Seek(ESeekCurrent,pos);

#ifdef __LOGME__
		LogText.Copy(_L("\n\nseek pos="));
		LogText.AppendNum(pos);
		LogText.Append(_L("\n\n"));
		WriteLogFile(LogText);
#endif
}

// called after tail end of protocol data is sent out
void CRippleVaultAppUi::SendFileComplete(TDesC& albumnamestatus)
{

	NetworkErrorNotification(0);
	//iPhotosDB->CheckAndIncreaseStatusCount(albumname, publishtype);

	// delete the first record from the db    
	TInt publishtype = iPhotosDB->GetPublishType();

	TBuf<128> fileName;
	iPhotosDB->GetMediaFileName(fileName);
	TInt check = iPhotosDB->DeleteFirstRecord();

#ifdef __LOGME__
	LogText.Copy(_L("\n\ndelete flag="));
	LogText.AppendNum(check);
	LogText.Append(_L("\n\n"));
	WriteLogFile(LogText);
#endif

	if(check == -111)
	{
		iPhotosDB->CheckAndSaveMediaFile(fileName);

		TBuf<100> albumname;
		albumname.Copy(albumnamestatus);

		TInt album;
		TInt photos;
		TInt videos;
		TInt albumshares;

		GetUploadSettingsFromDB(album, photos, videos, albumshares);		

		if(publishtype==0){
			iUploadPhotos++;
			photos++;
		}
		else
		{
			videos++;
			iUploadVideos++;
		}

		UpdatePhotosVideosCount(photos, videos, albumshares);
	}

	if(isOpen==1)
	{
		isOpen=0;
		iFileDes.Close();
	}
	iFsession.Close();
	

	//iSocketWriter->Disconnect();
	//delete iSocketWriter;
	//iSocketWriter = NULL;

	//ChargePublish(publishtype);

#ifdef __LOGME__
	LogText.Copy(_L("SendFileComplete\n"));
	WriteLogFile(LogText);
#endif

	TTime btime;
	btime.HomeTime();

	iPhotosDB->UpdateBackupTimestampToDB(btime);
	//UpdateScreen();

	MoreFilesToUpload();
}

void CRippleVaultAppUi::SendFileError()
{

	//iScheduled = 0;
	if(isOpen==1)
	{
		isOpen=0;
		iFileDes.Close();
	}
	iFsession.Close();

	if(iSocketWriterPublish != NULL)
	{
		iSocketWriterPublish->Disconnect();
		delete iSocketWriterPublish;		
		iSocketWriterPublish = NULL;
	}
}

void CRippleVaultAppUi::SendSharingFileError(TInt flag)
{
	iPhotosDB->DeleteSharingsFirstRecord();

	CloseSocketShare();

	if(flag == 3)
		ShowMessageBoxFromResource(R_EXAMPLE_TEXT_NETWORKERROR);
	else
		ShowMessageBoxFromResource(R_TEXT_SHARING_GPRSUNAVAILABLE);				

	#ifndef __UIQ__	
		if(flag == 1)
			HandleCommandL(EMainSubView);	
		else
			HandleCommandL(EMainView);	
	#endif

}

void CRippleVaultAppUi::CloseSocketPassword()
{
	// called by registration path only	
	if (iSocketWriterPassword != NULL)
	{		
		iSocketWriterPassword->Disconnect();
		delete iSocketWriterPassword;
		//delete servlet_data;
		iSocketWriterPassword = NULL;
	}	

	CloseConnection();
}

void CRippleVaultAppUi::CloseSocketPublish()
{
	// called by registration path only	
	if (iSocketWriterPublish != NULL)
	{		
		iSocketWriterPublish->Disconnect();
		delete iSocketWriterPublish;
		//delete servlet_data;
		iSocketWriterPublish = NULL;
	}	
	CloseConnection();
}

void CRippleVaultAppUi::CloseSocketShare()
{
	// called by registration path only	
	if (iSocketWriterShare != NULL)
	{		
		iSocketWriterShare->Disconnect();
		delete iSocketWriterShare;
		//delete servlet_data;
		iSocketWriterShare = NULL;
	}
	CloseConnection();
}

void CRippleVaultAppUi::CloseSocketSMS()
{
	// called by registration path only	
	if (iSocketWriterSMS != NULL)
	{		
		iSocketWriterSMS->Disconnect();
		delete iSocketWriterSMS;
		//delete servlet_data;
		iSocketWriterSMS = NULL;
	}
	CloseConnection();
}

void CRippleVaultAppUi::CloseSocketStatus(TInt connectionstatus)
{
	LoadParamsFromConfigFile();

	if(connectionstatus == 1)
	{
		if (iSocketWriterPublishStatus != NULL)
		{		
			iSocketWriterPublishStatus->Disconnect();
			delete iSocketWriterPublishStatus;
			//delete servlet_data;
			iSocketWriterPublishStatus = NULL;
		}
	}
	else if(connectionstatus == 2 || connectionstatus == 6)
	{
		if (iSocketWriterContactsStatus != NULL)
		{		
			iSocketWriterContactsStatus->Disconnect();
			delete iSocketWriterContactsStatus;
			//delete servlet_data;
			iSocketWriterContactsStatus = NULL;
		}
	}
	else if(connectionstatus == 3)
	{
		if (iSocketWriterSmsStatus != NULL)
		{		
			iSocketWriterSmsStatus->Disconnect();
			delete iSocketWriterSmsStatus;
			//delete servlet_data;
			iSocketWriterSmsStatus = NULL;
		}
	}
	else if(connectionstatus == 4)
	{
		if (iSocketWriterSimChangeStatus != NULL)
		{		
			iSocketWriterSimChangeStatus->Disconnect();
			delete iSocketWriterSimChangeStatus;
			//delete servlet_data;
			iSocketWriterSimChangeStatus = NULL;
		}
	}
	else if(connectionstatus == 7)
	{
		if (iSocketWriterAutoSmsStatus != NULL)
		{		
			iSocketWriterAutoSmsStatus->Disconnect();
			delete iSocketWriterAutoSmsStatus;
			//delete servlet_data;
			iSocketWriterAutoSmsStatus = NULL;
		}
	}
	
	CloseConnection();
	
	/*else if(connectionstatus == 5)
	{
		if (iSocketWriterBillingStatus != NULL)
		{		
			iSocketWriterBillingStatus->Disconnect();
			delete iSocketWriterBillingStatus;			
			iSocketWriterBillingStatus = NULL;
		}
	}*/	
}

/*void CRippleVaultAppUi::CloseSocketUpgrade()
{
	// called by registration path only	
	if (iSocketWriterUpgrade != NULL)
	{		
		iSocketWriterUpgrade->Disconnect();
		delete iSocketWriterUpgrade;
		delete servlet_data;
		iSocketWriterUpgrade = NULL;
	}
	CloseConnection();
}
*/
#ifdef WAP_PT
TInt CRippleVaultAppUi::GetPayloadBuffer(HBufC8** aBuffer)
{
    // get the record from db
//    HBufC* filename = HBufC::New(128);
    TInt8 type;
//    HBufC* folder = HBufC::New(128);
    TInt8 photo;
	TInt8 isNew;

	TInt packetnum;
	//Anupam
	TBuf<128> fileName;
	TBuf<128> folderName;
	TBuf<128> descName;
	TBuf8<128> data;
	TBuf<1024> status;
	CnvUtfConverter converter;
	
	TInt err;
	TInt retval = -1;
	err = iPhotosDB->GetRecord(fileName, type, folderName, descName, photo, isNew, status);

#ifdef __LOGME__
	LogText.Copy(_L("GetPayloadBuffer 1\n"));
	WriteLogFile(LogText);
#endif

	iFsession.Connect();

	if (err == 0)
	{
		TInt len = 0;
        TInt ret;		
		
		ret = iFileDes.Open(iFsession, fileName, EFileRead);
        if (ret == KErrNone)
        {
			isOpen=1;
		    iFileDes.Size(len);
			iFileDes.Seek(ESeekStart, 0);
        }		
		
		if (len)
		{
#ifdef __LOGME__
			LogText.Copy(_L("GetPayloadBuffer 2\n"));
			WriteLogFile(LogText);
#endif

			TBuf8<300> file_head;
			TBuf<10> imglen;

			packetnum = status.Find(_L("0"));

			if(packetnum == status.Length() - 1){
				len = len % (PUBLISH_PACKET_SIZE*1024);
				imglen.Num(len);			
			}
			else{
				len = PUBLISH_PACKET_SIZE*1024;
				imglen.Num(len);
			}
			
			//converter.ConvertFromUnicodeToUtf8(data,imglen);				
			//file_head.Copy(data);
			//file_head.Append(_L("-"));

			TBuf8<600> start_head;
			start_head.Copy(_L("<root><h><msgt>"));
			
			if (photo == KPhoto)
			{
				start_head.Append(_L("photopacketbackup"));
			}
			else
			{
				start_head.Append(_L("videopacketbackup"));
			}

			start_head.Append(_L("</msgt>"));
			
			start_head.Append(_L("<msg></msg>"));			

			start_head.Append(_L("<uid>"));
			start_head.Append(iUserNames);
			start_head.Append(_L("</uid>"));

			start_head.Append(_L("<hp>"));
			start_head.Append(iMobileNo);
			start_head.Append(_L("</hp>"));

			start_head.Append(_L("<did>"));
			start_head.Append(iIMEIPhone);
			start_head.Append(_L("</did>"));

			start_head.Append(_L("<currec>"));

			TInt pos =fileName.Find(_L("."));
			if(pos != -1)
				fileName.Delete(pos + 1, 4);
			else
				fileName.Append(_L("."));

			pos = fileName.LocateReverse('\\');
			if(pos!=-1)
				fileName.Delete(1, pos);

			start_head.Append(fileName);
			start_head.AppendNum(status.Find(_L("0")) + 1);
			start_head.Append(_L("</currec>"));
			
			start_head.Append(_L("<totrec>"));
			start_head.AppendNum(status.Length());
			start_head.Append(_L("</totrec>"));

			start_head.Append(_L("<loc>en_us</loc><ctime></ctime><lstime>0</lstime><synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype>"));
			
			start_head.Append(_L("</h>"));

			start_head.Append(_L("<b>"));

			//start_head.Append(_L("<object>"));

			start_head.Append(_L("<folder>"));
			start_head.Append(_L("<name>"));

			if(folderName.Compare(_L("")) == 0)
				folderName.Copy(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));
			
			converter.ConvertFromUnicodeToUtf8(data,folderName);
			start_head.Append(data);
			
			start_head.Append(_L("</name>"));
			start_head.Append(_L("</folder>"));

			start_head.Append(_L("<description>"));
			if(iDescription)
			{
				
				converter.ConvertFromUnicodeToUtf8(data,descName);				
				start_head.Append(data);				

			}

			start_head.Append(_L("</description>"));

			start_head.Append(_L("</b></root>"));

#ifdef __LOGME__
			LogText.Copy(_L("GetPayloadBuffer 3\n"));
			WriteLogFile(LogText);
#endif
//			GetFirstProtocolDataXML(start_head,300,folderName);
/*
			start_head.Copy(_L("<imei>"));
			start_head.Append(iIMEIPhone);
			start_head.Append(_L("</imei>"));
			start_head.Append(_L("<object>"));
			
			start_head.Append(_L("<folder>"));		
			start_head.Append(_L("<name>"));
			start_head.Append(folderName);
			start_head.Append(_L("</name>"));
            if (isNew)
            {
                TBuf8<4> typebuf;
                start_head.Append(_L("<type>"));
                typebuf.Num(type);
                start_head.Append(typebuf);
                start_head.Append(_L("</type>"));
            }
			start_head.Append(_L("</folder>"));

			start_head.Append(_L("<data length = \""));
			
			start_head.Append(imglen);
			start_head.Append(_L("\">"));
*/

			imglen.Num(start_head.Length());
			converter.ConvertFromUnicodeToUtf8(data,imglen);
			file_head.Copy(data);
			imglen.Copy(_L("-"));
			converter.ConvertFromUnicodeToUtf8(data,imglen);
			file_head.Append(data);

			TBuf8<1024> buffer;
#ifndef WAP_PT
				TBuf8<20> content_l;
				content_l.Num(len+start_head.Length()+file_head.Length()); // 0 for footer
			
				buffer.Copy(_L("GET "));			
				buffer.Append(iPublishUrl);
				//buffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));

				buffer.Append(_L(" HTTP/1.0\r\nHOST:"));
				buffer.Append(iServerAddr);
				buffer.Append(_L("\r\nPORT:"));
				buffer.AppendNum(iPort);

				buffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
				
				buffer.Append(content_l);
				buffer.Append(_L("\r\n\r\n"));
#else
				buffer.Copy(_L(""));
#endif

			HBufC8* data = HBufC8::NewL(len+start_head.Length()+file_head.Length()+buffer.Length());
			if (data)
			{
				*aBuffer = data;
				//data->Des().Copy(buffer);
				//data->Des().Append(file_head);
			
				data->Des().Copy(buffer);
				data->Des().Append(file_head);
				data->Des().Append(start_head);

				SeekDataFromFile(packetnum);

				TBuf8<1024> tembuff;
				TInt i=0;
				while(i<PUBLISH_PACKET_SIZE)
				{
				    TInt ret = iFileDes.Read(tembuff, tembuff.MaxLength());
					if(ret != KErrNone || ret == KErrOverflow || ret == KErrGeneral)
					{
						break;
					}
					if (tembuff.Length())
					{
						data->Des().Append(tembuff);
						i++;
					}
					else
					{
						break;
					}
				}


				//TInt ret = iFileDes.Read(data->Des(), len);

#ifdef __LOGME__
				LogText.Copy(_L("\nEXTRACTEDlen="));
				LogText.AppendNum(data->Des().Size());
				WriteLogFile(LogText);
#endif
				//data->Des().Insert(0, buffer);
				//data->Des().Insert(buffer.Length(), file_head);

				/*while(1)
				{
				    TInt ret = iFileDes.Read(data->Des(), len);
					if(ret != KErrNone || ret == KErrOverflow || ret == KErrGeneral)
					{
						break;
					}
					if (file_head.Length())
					{
						data->Des().Append(file_head);
					}
					else
					{
						break;
					}
				}*/

				//data->Des().Append(start_head);				

				retval = KErrNone;
			}
#ifdef __LOGME__
			LogText.Copy(_L("GetPayloadBuffer 4\n"));
			WriteLogFile(LogText);

			LogText.Copy(_L("\n\nlen="));
			LogText.AppendNum(len);
			WriteLogFile(LogText);

			LogText.Copy(_L("\n\nbuffer="));
			LogText.AppendNum(buffer.Length());
			WriteLogFile(LogText);

			LogText.Copy(_L("\n\nfile_head="));
			LogText.AppendNum(file_head.Length());
			WriteLogFile(LogText);

			LogText.Copy(_L("\n\nstart_head="));
			LogText.AppendNum(start_head.Length());
			WriteLogFile(LogText);
#endif
//			LogText.Copy(_L("\n\ncontent_l="));
//			LogText.Append(content_l);
//			WriteLogFile(LogText);


		} // if (len
		iFileDes.Close();
	} // if (err
//	delete folder;
//    delete filename;
    
    iFsession.Close();
	
#ifdef __LOGME__	
	LogText.Copy(_L("GetPayloadBuffer 5\n"));
	WriteLogFile(LogText);
#endif
    
	return retval;
}
#endif

#ifdef WAP_PT
TInt CRippleVaultAppUi::GetSMSPayloadBuffer(HBufC8** aBuffer, TInt aSmstype)
{
    // get the record from db
//    HBufC* filename = HBufC::New(128);
//    TInt8 type;
//    HBufC* folder = HBufC::New(128);
//    TInt8 photo;
//	TInt8 isNew;
	//Anupam
	TBuf<128> fileName;

	fileName = CRippleVaultAppUi::ApplicationDriveAndPath();
	if(aSmstype)
		fileName.Append(_L("smsbackup.aut"));
	else
		fileName.Append(_L("smsbackup.man"));
	
	TInt err=0;
	TInt retval = -1;
	//if (err == 0)
	//{
		TInt len = 0;
        TInt ret;

		if(aSmstype)
			iFAutoSMSsession.Connect();
		else
			iFSMSsession.Connect();
		
		if(aSmstype)
			ret = iFileAutoSMSDes.Open(iFAutoSMSsession, fileName, EFileRead);
		else
			ret = iFileSMSDes.Open(iFSMSsession, fileName, EFileRead);

        if (ret == KErrNone)
        {
			if(aSmstype)
				iFileAutoSMSDes.Size(len);
			else
				iFileSMSDes.Size(len);
        }		
		
		if (len)
		{
			TBuf8<300> start_head;

			HBufC8* data = HBufC8::NewL(len);
			if (data)
			{
				*aBuffer = data;
				//data->Des().Copy(start_head);
				
				while(1)
				{
					if(aSmstype)
						TInt ret = iFileAutoSMSDes.Read(start_head, start_head.MaxLength());
					else
						TInt ret = iFileSMSDes.Read(start_head, start_head.MaxLength());
					if(ret != KErrNone || ret == KErrOverflow || ret == KErrGeneral)
					{
						break;
					}
					if (start_head.Length())
					{
						data->Des().Append(start_head);
					}
					else
					{
						break;
					}
				}
				retval = KErrNone;
			}
		} // if (len
		//iFileSMSDes.Close();
	//} // if (err
	//delete folder;
    //delete filename;
    
    //iFSMSsession.Close();
    return retval;
}
#endif
/*
void CRippleVaultAppUi::GetProtocolDataXML(TDes8& buffer, TInt aMaxLen, TDes& albumname)
{
    // get the record from db
    HBufC* filename = HBufC::New(128);
    TInt8 type;
    HBufC* folder = HBufC::New(128);
    TInt8 photo;
	TInt8 isNew;
	
	CnvUtfConverter converter;
	TBuf<128> fileName;
	TBuf<128> folderName;
	TBuf<128> descName;	
	TBuf8<128> data;

	buffer.Copy(_L(""));
    //TInt err = iPhotosDB->GetRecord(filename->Des(), type, folder->Des(), photo, isNew);
	TInt err = iPhotosDB->GetRecord(fileName, type, folderName, descName, photo, isNew);


	LogText.Copy(_L("\nfilename1="));
	WriteLogFile(LogText);


	LogText.Copy(fileName);
	WriteLogFile(LogText);

	LogText.Copy(_L("\n"));
	WriteLogFile(LogText);

	albumname.Copy(folderName);

	if (err == 0)
	{
		//TInt len = 0;
//        TInt ret;

		//len = 1;
		//if (len)
		//{
			TBuf8<600> start_head;
			//TBuf8<10> imglen;			
			//imglen.Num(len);

			start_head.Copy(_L("<root><h><msgt>"));
			
			if (photo == KPhoto)
			{
				start_head.Append(_L("photosbackup"));
			}
			else
			{
				start_head.Append(_L("videosbackup"));
			}

			start_head.Append(_L("</msgt>"));
			
			start_head.Append(_L("<msg></msg>"));			

			start_head.Append(_L("<uid>"));
			start_head.Append(iUserNames);
			start_head.Append(_L("</uid>"));

			start_head.Append(_L("<hp>"));
			start_head.Append(iMobileNo);
			start_head.Append(_L("</hp>"));

			start_head.Append(_L("<did>"));
			start_head.Append(iIMEIPhone);
			start_head.Append(_L("</did>"));

			start_head.Append(_L("<currec></currec><totrec></totrec><loc>en_us</loc><ctime></ctime><lstime>0</lstime><synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype>"));
			
			start_head.Append(_L("</h>"));

			start_head.Append(_L("<b>"));

			//start_head.Append(_L("<object>"));

			start_head.Append(_L("<folder>"));
			start_head.Append(_L("<name>"));
			
			converter.ConvertFromUnicodeToUtf8(data,folderName);
			start_head.Append(data);
			
			start_head.Append(_L("</name>"));
			start_head.Append(_L("</folder>"));

			start_head.Append(_L("<description>"));
			if(iDescription)
			{
				
				converter.ConvertFromUnicodeToUtf8(data,descName);				
				start_head.Append(data);				

			}

			start_head.Append(_L("</description>"));

			start_head.Append(_L("</b></root>"));

			buffer.Copy(start_head);			
		//}
	}
	delete folder;
    delete filename;
}

*/
void CRippleVaultAppUi::GetFirstProtocolDataXML(TDes8& buffer, TDes& albumname)
{
    // get the record from db
    TInt8 type;
    TInt8 photo;
	TInt8 isNew;

	TInt packetnum;
	//Anupam
	TBuf<128> fileName;
	TBuf<128> folderName;
	TBuf<128> descName;
	TBuf8<128> data;
	TBuf<1024> status;
	CnvUtfConverter converter;
	
	TBuf<10> extension;

	TInt err;
	//TInt retval = -1;
	err = iPhotosDB->GetRecord(fileName, type, folderName, descName, photo, isNew, status);

	albumname.Copy(folderName);

#ifdef __LOGME__
	LogText.Copy(_L("GetFirstProtocolDataXML 1\n"));
	WriteLogFile(LogText);
#endif

	iFsession.Connect();

	if (err == 0)
	{
		TInt len = 0;
        TInt ret;		
		
		ret = iFileDes.Open(iFsession, fileName, EFileRead);
        if (ret == KErrNone)
        {
			isOpen=1;
		    iFileDes.Size(len);
			iFileDes.Seek(ESeekStart, 0);
        }		
		
		if (len)
		{
#ifdef __LOGME__
			LogText.Copy(_L("GetFirstProtocolDataXML 2\n"));
			WriteLogFile(LogText);
#endif

			TBuf8<300> file_head;
			TBuf<10> imglen;

			packetnum = status.Find(_L("0"));

			if(packetnum == status.Length() - 1){
				len = len % (PUBLISH_PACKET_SIZE*1024);
				imglen.Num(len);			
			}
			else{
				len = PUBLISH_PACKET_SIZE*1024;
				imglen.Num(len);
			}
			
			TBuf8<1024> aXmlData;
			aXmlData.Copy(_L("<root><h><msgt>"));
			
			if (photo == KPhoto)
			{
				aXmlData.Append(_L("photopacketbackup"));
			}
			else
			{
				aXmlData.Append(_L("videopacketbackup"));
			}

			aXmlData.Append(_L("</msgt>"));
			
			aXmlData.Append(_L("<msg></msg>"));			

			aXmlData.Append(_L("<uid>"));
			aXmlData.Append(iUserNames);
			aXmlData.Append(_L("</uid>"));

			aXmlData.Append(_L("<hp>"));
			aXmlData.Append(iMobileNo);
			aXmlData.Append(_L("</hp>"));

			aXmlData.Append(_L("<did>"));
			aXmlData.Append(iIMEIPhone);
			aXmlData.Append(_L("</did>"));

			aXmlData.Append(_L("<currec>"));

			//TInt pos =fileName.Find(_L("."));
			TInt pos =fileName.LocateReverse('.');
			if(pos != -1)
			{
				//extension.Copy(fileName.Right(4));
				//fileName.Delete(pos + 1, 4);
				extension.Copy(fileName.Right(fileName.Length()-pos));
				fileName.Delete(pos + 1, fileName.Length()-pos);
			}
			else
				fileName.Append(_L("."));

			pos = fileName.LocateReverse('\\');
			if(pos!=-1)
				fileName.Delete(1, pos);

			aXmlData.Append(fileName);
			aXmlData.AppendNum(status.Find(_L("0")) + 1);
			aXmlData.Append(extension);
			aXmlData.Append(_L("</currec>"));

			aXmlData.Append(_L("<totrec>"));
			aXmlData.AppendNum(status.Length());
			aXmlData.Append(_L("</totrec>"));

			aXmlData.Append(_L("<loc>en_us</loc><ctime></ctime><lstime>0</lstime><synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype>"));
			
			aXmlData.Append(_L("</h>"));

			aXmlData.Append(_L("<b>"));

			//aXmlData.Append(_L("<object>"));

			aXmlData.Append(_L("<folder>"));
			aXmlData.Append(_L("<name>"));
			
			if(folderName.Compare(_L("")) == 0)
				folderName.Copy(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));

			converter.ConvertFromUnicodeToUtf8(data,folderName);
			aXmlData.Append(data);
			
			aXmlData.Append(_L("</name>"));
			aXmlData.Append(_L("</folder>"));

			aXmlData.Append(_L("<description>"));
			if(iDescription)
			{
				
				converter.ConvertFromUnicodeToUtf8(data,descName);				
				aXmlData.Append(data);				

			}

			aXmlData.Append(_L("</description>"));

			aXmlData.Append(_L("</b></root>"));

			imglen.Num(aXmlData.Length());
			converter.ConvertFromUnicodeToUtf8(data,imglen);
			file_head.Copy(data);
			imglen.Copy(_L("-"));
			converter.ConvertFromUnicodeToUtf8(data,imglen);
			file_head.Append(data);

#ifdef __LOGME__
			LogText.Copy(_L("GetFirstProtocolDataXML 3\n"));
			WriteLogFile(LogText);
#endif
//			GetFirstProtocolDataXML(start_head,300,folderName);

			//TBuf8<1024> buffer;
#ifndef WAP_PT
				TBuf8<20> content_l;
				content_l.Num(len+aXmlData.Length()+file_head.Length()); // 0 for footer
			
				buffer.Copy(_L("GET "));			
				buffer.Append(iPublishUrl);
				//buffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));

				buffer.Append(_L(" HTTP/1.0\r\nHOST:"));
				buffer.Append(iServerAddr);
				buffer.Append(_L("\r\nPORT:"));
				buffer.AppendNum(iPort);

				buffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
				
				buffer.Append(content_l);
				buffer.Append(_L("\r\n\r\n"));

				buffer.Append(file_head);
				buffer.Append(aXmlData);
#else
				buffer.Copy(_L(""));
				buffer.Append(file_head);
#endif

				SeekDataFromFile(packetnum);

				/*TBuf8<1024> tembuff;
				TInt i=0;
				while(i<PUBLISH_PACKET_SIZE)
				{
				    TInt ret = iFileDes.Read(tembuff, tembuff.MaxLength());
					if(ret != KErrNone || ret == KErrOverflow || ret == KErrGeneral)
					{
						break;
					}
					if (tembuff.Length())
					{
						data->Des().Append(tembuff);
						i++;
					}
					else
					{
						break;
					}
				}*/

#ifdef __LOGME__
			LogText.Copy(_L("GetFirstProtocolDataXML 4\n"));
			WriteLogFile(LogText);

			LogText.Copy(_L("\n\nlen="));
			LogText.AppendNum(len);
			WriteLogFile(LogText);

			LogText.Copy(_L("\n\nbuffer="));
			LogText.AppendNum(buffer.Length());
			WriteLogFile(LogText);

			LogText.Copy(_L("\n\nfile_head="));
			LogText.AppendNum(file_head.Length());
			WriteLogFile(LogText);

			LogText.Copy(_L("\n\naXmlData="));
			LogText.AppendNum(aXmlData.Length());
			WriteLogFile(LogText);
#endif
			//remove this
			//WriteLogFile(data->Des());

//			LogText.Copy(_L("\n\ncontent_l="));
//			LogText.Append(content_l);
//			WriteLogFile(LogText);


		} // if (len
		//iFileDes.Close();
	} // if (err
//	delete folder;
//    delete filename;
    
    //iFsession.Close();
#ifdef __LOGME__
	LogText.Copy(_L("GetFirstProtocolDataXML 5\n"));
	WriteLogFile(LogText);
#endif    
	//return retval;
}

CDesCArray* CRippleVaultAppUi::GetFolderList()
{
    return iPhotosDB->GetFolderList();
}

void CRippleVaultAppUi::EnableSmsUpload(TInt aIsRegistered){
	if (aIsRegistered == SMSBACKUP_MANUAL)//NOT_REGISTERED)
	{
		/*if( iSmsBackupTimer != NULL)
		{
			delete iSmsBackupTimer;
   			iSmsBackupTimer=NULL;
		}*/
	}
	else{
		if( iSmsBackupTimer == NULL)
		{
			iSmsBackupTimer = CSmsBackupTimer::NewL(*this);
			iSmsBackupTimer->Start();
		}
	}
}

void CRippleVaultAppUi::EnableUpload(TInt aIsRegistered)
{	
	if (aIsRegistered == 2)//NOT_REGISTERED || iMode == MODE_OFF)
	{
		if (iPhotosEngine1 != NULL)
		{
			delete iPhotosEngine1;
			iPhotosEngine1 = NULL;
		}	
		if (iPhotosEngine2 != NULL)
		{
			delete iPhotosEngine2;
			iPhotosEngine2 = NULL;
		}
		if( iPhotosTimer != NULL)
		{
			delete iPhotosTimer;
   			iPhotosTimer=NULL;
		}
		if (iVideosEngine1 != NULL)
		{
			delete iVideosEngine1;
			iVideosEngine1 = NULL;
		}
		if( iVideosEngine2 != NULL)
		{
   			delete iVideosEngine2;
   			iVideosEngine2=NULL;
		}

#ifdef __UIQ__
//#ifndef __S80__
		if (iPhotosEngine3 != NULL)
		{
			delete iPhotosEngine3;
			iPhotosEngine3 = NULL;
		}	
		if (iPhotosEngine4 != NULL)
		{
			delete iPhotosEngine4;
			iPhotosEngine4 = NULL;
		}
		if (iVideosEngine3 != NULL)
		{
			delete iVideosEngine3;
			iVideosEngine3 = NULL;
		}
		if( iVideosEngine4 != NULL)
		{
   			delete iVideosEngine4;
   			iVideosEngine4=NULL;
		}
//#endif
#endif
	}
	else
	{	TBuf <120> dirname;
		if (iPhotosEngine1 == NULL)
		{
			dirname.Copy(KDirPhonePhoto);
			iPhotosEngine1=CPhotosEngine::NewL(*this, dirname);				
		}
		if (iPhotosEngine2 == NULL)
		{
			dirname.Copy(KDirMMCPhoto);
			iPhotosEngine2=CPhotosEngine::NewL(*this, dirname);				
		}
		if( iPhotosTimer == NULL)
		{
			iPhotosTimer = CPhotosTimer::NewL(*this);
			iPhotosTimer->Start();
		}
		if (iVideosEngine1 == NULL)
		{
			dirname.Copy(KDirPhoneVideo);
			iVideosEngine1=CVideosEngine::NewL(*this, dirname);
		}
		if (iVideosEngine2 == NULL)
		{
			dirname.Copy(KDirMMCVideo);
			iVideosEngine2=CVideosEngine::NewL(*this, dirname);
		}
		/*if(iBillingEngine == NULL)
		{
			iBillingEngine = CBillingEngine::NewL(*this);
			iBillingEngine->Start();
		}*/

#ifdef __UIQ__
//#ifndef __S80__
		if (iPhotosEngine3 == NULL)
		{
			dirname.Copy(KDirPhonePhotoSony);
			iPhotosEngine3=CPhotosEngine::NewL(*this, dirname);				
		}
		if (iPhotosEngine4 == NULL)
		{
			dirname.Copy(KDirMMCPhotoSony);
			iPhotosEngine4=CPhotosEngine::NewL(*this, dirname);				
		}
		if (iVideosEngine3 == NULL)
		{
			dirname.Copy(KDirPhoneVideoSony);
			iVideosEngine3=CVideosEngine::NewL(*this, dirname);
		}
		if (iVideosEngine4 == NULL)
		{
			dirname.Copy(KDirMMCVideoSony);
			iVideosEngine4=CVideosEngine::NewL(*this, dirname);
		}
//#endif
#endif
	}

}

TInt8 CRippleVaultAppUi::GetMode()
{
	return iMode;
}

TInt CRippleVaultAppUi::CheckAndSaveFolderToDB(TDesC& aFolder)
{
	TInt ret=0;
	if(!aFolder.Compare(_L(""))==0){
	//TInt ret;
	ret =  iPhotosDB->CheckAndSaveFolder(aFolder);    
	}
	return ret;
}

void CRippleVaultAppUi::UpdatePhotosVideosCount(TInt aPhotosCount, TInt aVideosCount, TInt aAlbumShareCount){
	iPhotosDB->SaveUploadSettingsL(aPhotosCount, aVideosCount, aAlbumShareCount);
}

#ifdef __LOGME__	
void CRippleVaultAppUi::WriteLogFile(TDes8& Text)
{   
//#ifdef __LOGME__
	TInt size;
	LogFile.Size(size);
	if(size>50*1024)
	{
		LogFile.Close();

		TBuf<128> name;
		name = CRippleVaultAppUi::ApplicationDriveAndPath();
		name.Append(KTempFile);

		iLogSession.Delete(name);

		LogFile.Create(iLogSession,name,EFileStreamText|EFileWrite|EFileShareAny);
		
	}

 LogFile.Write(Text);
//#endif
}
#endif

void CRippleVaultAppUi::ConnectToServer()
{
	if(iSocketWriterPublish == NULL)
	{
	    iSocketWriterPublish = CSocketsEngineWriter::NewL(*this);
	    iSocketWriterPublish->SetServerName(iServerAddr);
	    iSocketWriterPublish->SetPort(iPort);
		iSocketWriterPublish->iBuffer.Copy(*servlet_data);
	    iSocketWriterPublish->RegisterPhotos();	
		delete servlet_data;
	}
}

void CRippleVaultAppUi::ConnectToServerForChangingPassword()
{
	if(iSocketWriterPassword == NULL)
	{
		iSocketWriterPassword = CSocketsEngineWriter::NewL(*this);
		iSocketWriterPassword->SetServerName(iServerAddr);
		iSocketWriterPassword->SetPort(iPort);
		iSocketWriterPassword->iBuffer.Copy(*servlet_data);
		iSocketWriterPassword->ChangePassword();	
		delete servlet_data;	
	}
}

//void CRippleVaultAppUi::ConnectToServerForCharging()
//{
//	iSocketWriter = CSocketsEngineWriter::NewL(*this);
//    iSocketWriter->SetServerName(iServerAddr);
//    iSocketWriter->SetPort(iPort);
//    iSocketWriter->ChargePublish();
//}

void CRippleVaultAppUi::WritRegisteredFile()
{
	WriteImsiNumber();

	RFs fs;
	fs.Connect();

	RFile file;
	TBuf8<20> name;
	TInt err;

	name.Copy(iUserName);
	TBuf<128> path1;
	path1 = CRippleVaultAppUi::ApplicationDriveAndPath();
	path1.Append(KRegPath1);
	err = file.Open(fs, path1, EFileWrite);

	if(err != KErrNone)
	{
		file.Create(fs, path1, EFileWrite);
		file.Write(name);
	}

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KRegPath);
	err = file.Open(fs, path, EFileWrite);

	if(err != KErrNone)
	{
		file.Create(fs, path, EFileWrite);		
	}

	file.Close();
	fs.Close();	
}

/*
TBool CRippleVaultAppUi::CheckRegFile()
{
	_LIT(KRegPath1, "RegDetails.dat");
	
	RFs fs;
	fs.Connect();

	RFile file;
	TBuf8<20> name;
	TInt err;
	TBuf<150> path1;

	path1 = CRippleVaultAppUi::ApplicationDriveAndPath();
	path1.Delete(15, 13);
	path1.Append(_L("Contactsbackup\\"));
	path1.Append(KRegPath1);

	err = file.Open(fs, path1, EFileRead);

	if(err == KErrNone)
	{
		file.Read(name);
		iUserName.Copy(name);
		err = 1;
	}
	else
		err = -1;

	file.Close();
	fs.Close();	

	return err;
}

void CRippleVaultAppUi::CheckRegFileAndUpload()
{
	TInt err;

	err = CheckRegFile();

	if(err != -1)
	{
		//CEikonEnv::Static ()->AlertWin(iUserName);
		WritRegisteredFile();
		UserRegistered();
		EnableUpload(REGISTERED);
	}
}
*/

TBool CRippleVaultAppUi::RegFileExists()
{
	RFs fs;
	TBool ret;

	fs.Connect();

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KRegPath);
	if (BaflUtils::FileExists(fs, path))
	{
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}

	fs.Close();
	return ret;
}

#ifdef __UIQ__
CMobileDialog::CMobileDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
{	
}

CMobileDialog::~CMobileDialog()
{
}

CRegMobileDialog::CRegMobileDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
{	
}

CRegMobileDialog::~CRegMobileDialog()
{
}

CCheckSimDialog::CCheckSimDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
{	
}

CCheckSimDialog::~CCheckSimDialog()
{
}

CRegisterDialog::CRegisterDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
{
	//iSocketsEngine =NULL;
}

CRegisterDialog::~CRegisterDialog()
{
}

CChangePassword::CChangePassword(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
{	
}

CChangePassword::~CChangePassword()
{
}

CAlreadyRegisterDialog::CAlreadyRegisterDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)	
{
	//iSocketsEngine = NULL;
}

CAlreadyRegisterDialog::~CAlreadyRegisterDialog()
{
}

CAdvancedDialog::CAdvancedDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
{
}

CAdvancedDialog::~CAdvancedDialog()
{
}

CSmsSettingDialog::CSmsSettingDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
{
}

CSmsSettingDialog::~CSmsSettingDialog()
{
}
/*
CVideosSettingDialog::CVideosSettingDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
{
}

CVideosSettingDialog::~CVideosSettingDialog()
{
}
*/
CPhotosSettingDialog::CPhotosSettingDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
{
}

CPhotosSettingDialog::~CPhotosSettingDialog()
{
}

CContactsSettingDialog::CContactsSettingDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
{
}

CContactsSettingDialog::~CContactsSettingDialog()
{
}

CContactsUploadSettingDialog::CContactsUploadSettingDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
{
}

CContactsUploadSettingDialog::~CContactsUploadSettingDialog()
{
}

CSecureSettingDialog::CSecureSettingDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
{
}

CSecureSettingDialog::~CSecureSettingDialog()
{
}

CCustomMessageDialog::CCustomMessageDialog(TDesC& aText, TInt aResourceId)
{
	//iText.Copy(aText);
	iText= aText.AllocL();
	iTitle = aResourceId;
}

CCustomMessageDialog::~CCustomMessageDialog()
{
}

CFolderDialog::CFolderDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
	{
	}
	
CFolderDialog::~CFolderDialog()
	{
	}

CProcessAlbumDialog::CProcessAlbumDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
	{
	}	
CProcessAlbumDialog::~CProcessAlbumDialog()
	{
	}

CDescDialog::CDescDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
	{
	}
	
CDescDialog::~CDescDialog()
	{
	}

CSharingListBoxDialog::CSharingListBoxDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
	{
	}
	
CSharingListBoxDialog::~CSharingListBoxDialog()
	{
	}

CFolderDialog1::CFolderDialog1(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
	{
	}

CUnShareAlbumDialog::CUnShareAlbumDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
	{
	}

CFolderDialog1::~CFolderDialog1()
	{
	}

CUnShareAlbumDialog::~CUnShareAlbumDialog()
	{
	}

CChoiceListControl::CChoiceListControl(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)	
{	
}

CChoiceListControl::~CChoiceListControl()
{
}

CSelectedContactsDialog::CSelectedContactsDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
	{
	}

CSelectedContactsDialog::~CSelectedContactsDialog()
	{
	}

void CSelectedContactsDialog::PreLayoutDynInitL()
    {
	  iAppUi.iSettingsScreenValue  = -1;
      
	  CEikChoiceList* FolderEditor = STATIC_CAST(CEikChoiceList*,Control(EContactsList));
	  FolderEditor->SetArrayL(iAppUi.numbers);
    }

TBool CSelectedContactsDialog::ShutL()
{
	return EFalse;
}

TInt CSelectedContactsDialog::OkToExitL(TInt aKeycode)
{
#ifdef __S80__
	if (aKeycode == EDone1 || aKeycode == EDone2)
	{
		return EFalse;
	}
#endif

#ifdef __S80__
	if (aKeycode == EDone3)
#endif
	{
		CEikChoiceList* FolderEditor = STATIC_CAST(CEikChoiceList*,Control(EContactsList));
		if (iAppUi.numbers)
		{
			TInt aListPos = FolderEditor->CurrentItem();
			iAppUi.iSettingsScreenValue  = aListPos;

			TPtrC ptr1(iAppUi.numbers->MdcaPoint(aListPos));			
			iAppUi.iMobBuffer.Copy(ptr1);			
		}			
	}
	return ETrue;
}

void CSmsSettingDialog::PreLayoutDynInitL()
{
    TInt smstype;
	TTime time;
	TInt smsroam;
	
	iAppUi.GetSmsSettingsFromDB(smstype, time, smsroam);

	if(smstype == 1)
		smstype = 0;
	else if(smstype == 3)
		smstype = 1;

	CCoeControl* myControlPtr = this->Control(EUpLoad);
	CEikChoiceList* myVertOptionButtonList = static_cast<CEikChoiceList*>(myControlPtr);
	myVertOptionButtonList->SetCurrentItem(smstype);			
	
	/*
#ifdef __S80__
	CCoeControl* myTimePtr = this->Control(EMyTimeEditorId);
	CEikTimeEditor* myTimeEditor = static_cast<CEikTimeEditor*>(myTimePtr);
	myTimeEditor->SetTime(time);
#else
	CCoeControl* myTimePtr = this->Control(EMyTimeEditorId);
    CQikTimeEditor* myTimeEditor = static_cast<CQikTimeEditor*>(myTimePtr);
	myTimeEditor->SetTimeL(time);
#endif
	*/

	CCoeControl* myRoamPtr = this->Control(EPhotosRoamListControlId);
	CEikChoiceList* myRoamButtonList = static_cast<CEikChoiceList*>(myRoamPtr);
	myRoamButtonList->SetCurrentItem(smsroam);

}

TBool CSmsSettingDialog::ShutL()
{
	return EFalse;
}

TInt CSmsSettingDialog::OkToExitL(TInt aKeycode)
{
	//TBuf<200> msg;		
	TInt smstype;
	TTime time;
	TInt smsroam;

#ifdef __S80__
	if (aKeycode == EDone1 || aKeycode == EDone2)
	{
		return EFalse;
	}
#else
	if (aKeycode == EEikBidCancel)
	{
		return -1;
	}
#endif

	CCoeControl* myControlPtr = this->Control(EUpLoad);
	CEikChoiceList* myVertOptionButtonList = static_cast<CEikChoiceList*>(myControlPtr);
	smstype = myVertOptionButtonList->CurrentItem();

	if(smstype == 0)
		smstype = 1;
	else if(smstype == 1)
		smstype = 3;

	/*
#ifdef __S80__
	CCoeControl* myTimePtr = this->Control(EMyTimeEditorId);
	CEikTimeEditor* myTimeEditor = static_cast<CEikTimeEditor*>(myTimePtr);
    time = myTimeEditor->Time();
#else
	CCoeControl* myTimePtr = this->Control(EMyTimeEditorId);
    CQikTimeEditor* myTimeEditor = static_cast<CQikTimeEditor*>(myTimePtr);
    time = myTimeEditor->Time();
#endif
	*/

	CCoeControl* myRoamPtr = this->Control(EPhotosRoamListControlId);
	CEikChoiceList* myRoamButtonList = static_cast<CEikChoiceList*>(myRoamPtr);
	smsroam = myRoamButtonList->CurrentItem();

#ifdef __S80__
	if (aKeycode == EDone3)
#endif
	{    
	iAppUi.SaveSmsSettingsToDB((TInt8)smstype, time, (TInt8)smsroam);
	
	if(smsroam == 1)
		iAppUi.ShowMessageBoxFromResource(R_SMS_MESSAGE1);
		//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_SMS_MESSAGE1)));
	else
		iAppUi.ShowMessageBoxFromResource(R_SMS_MESSAGE2);
		//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_SMS_MESSAGE2)));

	//iEikonEnv->InfoWinL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)), msg);

	/*TInt8 choice=0;
	iAppUi.GetChoice(choice);
	if(choice == 0)
		iAppUi.CallSetupWizardStep16();*/
	}
	return ETrue;
}

/*
void CVideosSettingDialog::PreLayoutDynInitL()
{
    TInt iMode;
	TInt desc;
	TInt photoroam;
	TInt securegallery;
	
	iAppUi.GetPhotosSettingsFromDB(iMode, desc, photoroam, securegallery);

#ifdef __S80__
	if(iMode == 2)
		iMode = 1;
#endif

	CCoeControl* myControlPtr = this->Control(EUpLoad);
	CEikChoiceList* myVertOptionButtonList = static_cast<CEikChoiceList*>(myControlPtr);
	myVertOptionButtonList->SetCurrentItem(iMode);			

	CCoeControl* myDescPtr = this->Control(EDescListControlId);
	CEikChoiceList* myDescButtonList = static_cast<CEikChoiceList*>(myDescPtr);
	myDescButtonList->SetCurrentItem(desc);

	CCoeControl* myRoamPtr = this->Control(EPhotosRoamListControlId);
	CEikChoiceList* myRoamButtonList = static_cast<CEikChoiceList*>(myRoamPtr);
	myRoamButtonList->SetCurrentItem(photoroam);	

	CCoeControl* mySecurePtr = this->Control(EPhotosSecureGalleryListControlId);
	CEikChoiceList* mySecureButtonList = static_cast<CEikChoiceList*>(mySecurePtr);
	mySecureButtonList->SetCurrentItem(securegallery);	
}

TBool CVideosSettingDialog::ShutL()
{
	return EFalse;
}

TInt CVideosSettingDialog::OkToExitL(TInt aKeycode)
{
	//TBuf<200> msg;		
	TInt iMode;	
	TInt iDescription;	
	TInt iPhotoRoaming;	
	TInt iSecureGallery;

#ifdef __S80__
	if (aKeycode == EDone1 || aKeycode == EDone2)
	{
		return EFalse;
	}
#else	
	if (aKeycode == EEikBidCancel)
	{
		return -1;
	}
#endif

    CCoeControl* myControlPtr = this->Control(EUpLoad);
	CEikChoiceList* myVertOptionButtonList = static_cast<CEikChoiceList*>(myControlPtr);
	iMode = myVertOptionButtonList->CurrentItem();

#ifdef __S80__
	if(iMode == 1)
		iMode = 2;
#endif

	CCoeControl* myDescPtr = this->Control(EDescListControlId);
	CEikChoiceList* myDescButtonList = static_cast<CEikChoiceList*>(myDescPtr);
	iDescription = myDescButtonList->CurrentItem();

	CCoeControl* myRoamPtr = this->Control(EPhotosRoamListControlId);
	CEikChoiceList* myRoamButtonList = static_cast<CEikChoiceList*>(myRoamPtr);
	iPhotoRoaming = myRoamButtonList->CurrentItem();

	CCoeControl* mySecurePtr = this->Control(EPhotosSecureGalleryListControlId);
	CEikChoiceList* mySecureButtonList = static_cast<CEikChoiceList*>(mySecurePtr);
	iSecureGallery = mySecureButtonList->CurrentItem();

#ifdef __S80__
	if (aKeycode == EDone3)
#endif
	{
		if( iMode == 0)
		{
			CDesCArray* array;
			array = iAppUi.GetFolderList();
			if(array != NULL )
			{
				if(array->Count() != 0)
				{
					int answer;
					answer=iAppUi.FolderDialog1();	
					if(answer)
					{
						iAppUi.SetDefaultFolder(iAppUi.FolderName);
					}
					else
					{
						return EFalse;
					}
				}
				else
				{
					//array don't have any element
					iMode = iAppUi.GetMode();				
					iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_PROMPTMODE);
				}
			}
			else
			{
				//array is NULL case
				myVertOptionButtonList->SetCurrentItem(1);
				iMode = myVertOptionButtonList->CurrentItem();

	#ifdef __S80__
				if(iMode == 1)
					iMode = 2;
	#endif

				iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_FOLDERNOTEXIST);
			}
		}
    
		if(iMode == 0 || iMode == 2)
			iDescription = 0;

		iAppUi.SavePhotosSettingsToDB((TInt8)iMode, (TInt8)iDescription, (TInt8)iPhotoRoaming, (TInt8)iSecureGallery);
		
		if(iPhotoRoaming == 1)
			iAppUi.ShowMessageBoxFromResource(R_VIDEOS_MESSAGE1);
			//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_VIDEOS_MESSAGE1)));
		else
			iAppUi.ShowMessageBoxFromResource(R_VIDEOS_MESSAGE2);
			//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_VIDEOS_MESSAGE2)));

		//iEikonEnv->InfoWinL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)), msg);
	}
	return ETrue;
}
*/

void CPhotosSettingDialog::PreLayoutDynInitL()
{
    TInt iMode;
	TInt desc;
	TInt photoroam;
	TInt securegallery;
	
	iAppUi.GetPhotosSettingsFromDB(iMode, desc, photoroam, securegallery);
#ifdef __S80__
	if(iMode == 2)
		iMode = 1;
#endif

	CCoeControl* myControlPtr = this->Control(EUpLoad);
	CEikChoiceList* myVertOptionButtonList = static_cast<CEikChoiceList*>(myControlPtr);
	myVertOptionButtonList->SetCurrentItem(iMode);			

	CCoeControl* myDescPtr = this->Control(EDescListControlId);
	CEikChoiceList* myDescButtonList = static_cast<CEikChoiceList*>(myDescPtr);
	myDescButtonList->SetCurrentItem(desc);

	CCoeControl* myRoamPtr = this->Control(EPhotosRoamListControlId);
	CEikChoiceList* myRoamButtonList = static_cast<CEikChoiceList*>(myRoamPtr);
	myRoamButtonList->SetCurrentItem(photoroam);	

	//CCoeControl* mySecurePtr = this->Control(EPhotosSecureGalleryListControlId);
	//CEikChoiceList* mySecureButtonList = static_cast<CEikChoiceList*>(mySecurePtr);
	//mySecureButtonList->SetCurrentItem(securegallery);	

	//CCoeControl* myPacketPtr = this->Control(EPhotosPacketSizeListControlId);
	//CEikChoiceList* myPacketButtonList = static_cast<CEikChoiceList*>(myPacketPtr);
	//myPacketButtonList->SetCurrentItem(size);	
}

TBool CPhotosSettingDialog::ShutL()
{
	return EFalse;
}

TInt CPhotosSettingDialog::OkToExitL(TInt aKeycode)
{
	//TBuf<200> msg;		
	TInt iMode;	
	TInt iDescription;	
	TInt iPhotoRoaming;	
	TInt iSecureGallery;
#ifdef __S80__
	if (aKeycode == EDone1 || aKeycode == EDone2)
	{
		return EFalse;
	}
#else	
	if (aKeycode == EEikBidCancel)
	{
		return -1;
	}
#endif

    CCoeControl* myControlPtr = this->Control(EUpLoad);
	CEikChoiceList* myVertOptionButtonList = static_cast<CEikChoiceList*>(myControlPtr);
	iMode = myVertOptionButtonList->CurrentItem();

#ifdef __S80__
	if(iMode == 1)
		iMode = 2;
#endif

	CCoeControl* myDescPtr = this->Control(EDescListControlId);
	CEikChoiceList* myDescButtonList = static_cast<CEikChoiceList*>(myDescPtr);
	iDescription = myDescButtonList->CurrentItem();

	CCoeControl* myRoamPtr = this->Control(EPhotosRoamListControlId);
	CEikChoiceList* myRoamButtonList = static_cast<CEikChoiceList*>(myRoamPtr);
	iPhotoRoaming = myRoamButtonList->CurrentItem();

	//CCoeControl* mySecurePtr = this->Control(EPhotosSecureGalleryListControlId);
	//CEikChoiceList* mySecureButtonList = static_cast<CEikChoiceList*>(mySecurePtr);
	//iSecureGallery = mySecureButtonList->CurrentItem();

	//CCoeControl* myPacketPtr = this->Control(EPhotosPacketSizeListControlId);
	//CEikChoiceList* myPacketButtonList = static_cast<CEikChoiceList*>(myPacketPtr);
	//iPacketSize = myPacketButtonList->CurrentItem();

#ifdef __S80__
	if (aKeycode == EDone3)
#endif
	{
		if( iMode == 0)
		{
			CDesCArray* array;
			array = iAppUi.GetFolderList();
			if(array != NULL )
			{
				if(array->Count() != 0)
				{
					int answer;
					answer=iAppUi.FolderDialog1();	
					if(answer)
					{
						iAppUi.SetDefaultFolder(iAppUi.FolderName);
					}
					else
					{
						return EFalse;
					}
				}
				else
				{
					//array don't have any element
					iMode = iAppUi.GetMode();				
					iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_PROMPTMODE);
				}
			}
			else
			{
				//array is NULL case
				myVertOptionButtonList->SetCurrentItem(1);
				iMode = myVertOptionButtonList->CurrentItem();
	#ifdef __S80__
				if(iMode == 1)
					iMode = 2;
	#endif
				iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_FOLDERNOTEXIST);
			}
		}
    
		if(iMode == 0 || iMode == 2)
			iDescription = 0;

		iAppUi.SavePhotosSettingsToDB((TInt8)iMode, (TInt8)iDescription, (TInt8)iPhotoRoaming, (TInt8)iSecureGallery);

		if(iPhotoRoaming == 1)
			iAppUi.ShowMessageBoxFromResource(R_PHOTOS_MESSAGE1);
			//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_MESSAGE1)));
		else
			iAppUi.ShowMessageBoxFromResource(R_PHOTOS_MESSAGE2);
			//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_PHOTOS_MESSAGE2)));

		//iEikonEnv->InfoWinL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)), msg);

		/*TInt8 choice=0;
		iAppUi.GetChoice(choice);
		if(choice == 0)
			iAppUi.CallSetupWizardStep10();*/
	}
	return ETrue;
}

void CSecureSettingDialog::PreLayoutDynInitL()
{
    TInt secureStart;
	TBuf<20>friendNumber;
	TBuf<200> secureText;
	TInt alarm;

	iAppUi.GetSecureSettingsFromFile(secureStart, friendNumber, secureText, alarm);

	CCoeControl* mySecurePtr = this->Control(ESecureAutoId);
	CEikChoiceList* mySecureList = static_cast<CEikChoiceList*>(mySecurePtr);
	mySecureList->SetCurrentItem(secureStart);	
	
	CEikRichTextEditor* R1;
    R1 = STATIC_CAST(CEikRichTextEditor*,Control(ESecureMobileId));
	R1->SetTextL(&friendNumber);
	
	CEikRichTextEditor* R2;
    R2 = STATIC_CAST(CEikRichTextEditor*,Control(ESecureTextId));
	R2->SetTextL(&secureText);
}

TBool CSecureSettingDialog::ShutL()
{
	return EFalse;
}

TInt CSecureSettingDialog::OkToExitL(TInt aKeycode)
{
	TInt secureStart;
	TBuf<20>friendNumber;
	TBuf<200> secureText;
	//TBuf<300> msg;	

#ifdef __S80__
	if (aKeycode == EDone1 || aKeycode == EDone2)
	{
		return EFalse;
	}
#endif

	CCoeControl* mySecurePtr = this->Control(ESecureAutoId);
	CEikChoiceList* mySecureList = static_cast<CEikChoiceList*>(mySecurePtr);
	secureStart = mySecureList->CurrentItem();

#ifdef __S80__
	if (aKeycode == EDone3)
#endif
	{
		CEikRichTextEditor* R1=STATIC_CAST(CEikRichTextEditor*,Control(ESecureMobileId));
		R1->GetText(friendNumber);	

		CEikRichTextEditor* R2=STATIC_CAST(CEikRichTextEditor*,Control(ESecureTextId));	
		R2->GetText(secureText);	

		if(secureStart == 1)		
			iAppUi.ShowMessageBoxFromResource(R_SECURE_MESSAGE1);
			//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_SECURE_MESSAGE1)));
		else
			iAppUi.ShowMessageBoxFromResource(R_SECURE_MESSAGE2);
			//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_SECURE_MESSAGE2)));

		//iEikonEnv->InfoWinL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)), msg);

		iAppUi.SaveSecureSettingsToFile(secureStart, friendNumber, secureText, 1);
	}
	return ETrue;
}

void CCustomMessageDialog::PreLayoutDynInitL()
{
	this->SetTitleL(iTitle);

	CEikLabel* label = (CEikLabel*)Control(ECustomMessageDialog);
	label->SetTextL(iText->Des());
}

TBool CCustomMessageDialog::ShutL()
{
	return EFalse;
}

TInt CCustomMessageDialog::OkToExitL(TInt aKeycode)
{
	return ETrue;
}

void CContactsSettingDialog::PreLayoutDynInitL()
{
    TInt contactroam;
    TInt contactupload;
	
	iAppUi.iSettingsScreenValue = 0;

	iAppUi.GetContactsSettingsFromDB(contactroam, contactupload);

	iAppUi.iContactsHandler->iContactsRoaming = contactroam;
	iAppUi.iContactsHandler->iContactsUpload = contactupload;
	
	CCoeControl* myControlPtr = this->Control(EContactsUploadControlId);
	CEikChoiceList* myVertOptionButtonList = static_cast<CEikChoiceList*>(myControlPtr);
	myVertOptionButtonList->SetCurrentItem(contactupload);

	CCoeControl* myControlPtr1 = this->Control(EContactsRoamListControlId);
	CEikChoiceList* myVertOptionButtonList1 = static_cast<CEikChoiceList*>(myControlPtr1);
	myVertOptionButtonList1->SetCurrentItem(contactroam);
}

TBool CContactsSettingDialog::ShutL()
{
	return EFalse;
}

TInt CContactsSettingDialog::OkToExitL(TInt aKeycode)
{
	TInt iContactRoaming;
	//TBuf<200> msg;	
#ifdef __S80__
	if (aKeycode == EDone1 || aKeycode == EDone2)
	{
		return EFalse;
	}
#else	
	if (aKeycode == EEikBidCancel)
	{
		return -1;
	}
#endif
	
	CCoeControl* myControlPtr = this->Control(EContactsUploadControlId);
	CEikChoiceList* myVertOptionButtonList = static_cast<CEikChoiceList*>(myControlPtr);
	iAppUi.iContactsHandler->iContactsUpload = myVertOptionButtonList->CurrentItem();

	CCoeControl* myControlPtr1 = this->Control(EContactsRoamListControlId);
	CEikChoiceList* myVertOptionButtonList1 = static_cast<CEikChoiceList*>(myControlPtr1);
	iAppUi.iContactsHandler->iContactsRoaming = myVertOptionButtonList1->CurrentItem();
	
#ifdef __S80__
	if (aKeycode == EDone3)
#endif
	{
	//iAppUi.SaveContactsSettingsToDB((TInt8)iContactRoaming);
		
	if(iContactRoaming == 1)
		iAppUi.ShowMessageBoxFromResource(R_CONTACTS_MESSAGE1);
		//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_MESSAGE1)));
	else
		iAppUi.ShowMessageBoxFromResource(R_CONTACTS_MESSAGE2);
		//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_CONTACTS_MESSAGE2)));

	//iEikonEnv->InfoWinL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)), msg);

		if(iAppUi.iContactsHandler->iContactsUpload == 1)
		{
			iAppUi.iSettingsScreenValue = 1;
		}
		else
		{
			iAppUi.SaveContactsSettingsToDB(iAppUi.iContactsHandler->iContactsRoaming,iAppUi.iContactsHandler->iContactsUpload);
		}
	}
	return ETrue;
}

void CChoiceListControl::PreLayoutDynInitL()
{	
	iAppUi.iSettingsScreenValue = 0;

	CCoeControl* myControlPtr = this->Control(EChoiceListControlId);

#ifdef __S80__
	CEikChoiceList* myVertOptionButtonList = static_cast<CEikChoiceList*>(myControlPtr);
	myVertOptionButtonList->SetCurrentItem(0);
#else
	CQikVertOptionButtonList* myVertOptionButtonList = static_cast<CQikVertOptionButtonList*>(myControlPtr);
	myVertOptionButtonList->SetButtonById(0);
#endif

}

TBool CChoiceListControl::ShutL()
{
	return EFalse;
}

// ---------------------------------------------------------
// CChoiceListControl::OkToExitL(TInt /*aKeycode*/)
// 
// ---------------------------------------------------------

TInt CChoiceListControl::OkToExitL(TInt aKeycode)
{
#ifdef __S80__
	if (aKeycode == EDone1 || aKeycode == EDone2)
	{
		return EFalse;
	}
#endif

#ifdef __S80__
	CCoeControl* myControlPtr = this->Control(EChoiceListControlId);
	CEikChoiceList* myVertOptionButtonList = static_cast<CEikChoiceList*>(myControlPtr);

	TInt iModelData = myVertOptionButtonList->CurrentItem();
#else
	CCoeControl* myControlPtr = this->Control(EChoiceListControlId);
	CQikVertOptionButtonList* myVertOptionButtonList = static_cast<CQikVertOptionButtonList*>(myControlPtr);

	TInt iModelData = myVertOptionButtonList->LabeledButtonId();
#endif

#ifdef __S80__
	if (aKeycode == EDone3)
#endif
	{
		if(iModelData == 0)
		{		
			iAppUi.iSettingsScreenValue = 1;
		}
		else if(iModelData == 1)
		{
			iAppUi.iSettingsScreenValue = 2;
		}
	}
	return ETrue;
}

// ---------------------------------------------------------
// CMobileDialog::PreLayoutDynInitL()
// 
// ---------------------------------------------------------

void CMobileDialog::PreLayoutDynInitL()
{
	TBuf<20> buf1;
	
	buf1.Zero();
		
	buf1.Copy(_L(""));

	CEikRichTextEditor* R1;
    R1 = STATIC_CAST(CEikRichTextEditor*,Control(EMobileNumber));
	R1->SetTextL(&buf1);	    
	
}

TBool CMobileDialog::ShutL()
{
	return EFalse;
}
void CContactsUploadSettingDialog::PreLayoutDynInitL()
{
    TInt days;
	TInt contactstime;
	TInt syncstate = 0;
	
	iAppUi.GetConatctsTimeFromDB(contactstime, days, syncstate);

	if(days == 0)
		days = 14;
#ifdef __S80__
	CCoeControl* myControlPtr = this->Control(EContactsUpload);
	CEikNumberEditor* myNumberEditor = static_cast<CEikNumberEditor*>(myControlPtr);
	myNumberEditor->SetNumber(days); 
#else
	CCoeControl* myControlPtr = this->Control(EContactsUpload);
	CQikNumberEditor* myNumberEditor = static_cast<CQikNumberEditor*>(myControlPtr);
	myNumberEditor->SetValueL(days); 
#endif

}

TInt CContactsUploadSettingDialog::OkToExitL(TInt aKeycode)
{
	TInt days=0;
	TInt time=0;
	TInt syncstate = 0;

#ifdef __S80__
	// Get a pointer to the control.
    CCoeControl* myControlPtr = this->Control(EContactsUpload);
    CEikNumberEditor* myNumberEditor = static_cast<CEikNumberEditor*>(myControlPtr);
    days = myNumberEditor->Number();
#else	
	// Get a pointer to the control.
    CCoeControl* myControlPtr = this->Control(EContactsUpload);
    CQikNumberEditor* myNumberEditor = static_cast<CQikNumberEditor*>(myControlPtr);
    days = myNumberEditor->Value();
#endif

	iAppUi.SaveConatctsTimeToDB(time, days, syncstate);
	
	iAppUi.SaveContactsSettingsToDB(iAppUi.iContactsHandler->iContactsRoaming,iAppUi.iContactsHandler->iContactsUpload);

	return ETrue;
}

// ---------------------------------------------------------
// CMobileDialog::OkToExitL(TInt /*aKeycode*/)
// 
// ---------------------------------------------------------

TInt CMobileDialog::OkToExitL(TInt /*aKeycode*/)
{	
	TBuf<20> mobilenumber;	
	
	//get the phonenumber
	CEikRichTextEditor* richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(EMobileNumber));	
	richTextEditor->GetText(mobilenumber);	

	iAppUi.iTempMbNum.Copy(mobilenumber);	

	return ETrue;
}

// ---------------------------------------------------------
// CRegMobileDialog::PreLayoutDynInitL()
// 
// ---------------------------------------------------------

void CRegMobileDialog::PreLayoutDynInitL()
{
	TBuf<20> buf1;
	
	buf1.Zero();
		
	buf1.Copy(_L(""));

	CEikRichTextEditor* R1;
    R1 = STATIC_CAST(CEikRichTextEditor*,Control(EMobileNumber));
	R1->SetTextL(&buf1);	    
	
}

TBool CRegMobileDialog::ShutL()
{
	return EFalse;
}

// ---------------------------------------------------------
// CRegMobileDialog::OkToExitL(TInt /*aKeycode*/)
// 
// ---------------------------------------------------------

TInt CRegMobileDialog::OkToExitL(TInt /*aKeycode*/)
{	
	TBuf<20> mobilenumber;	
	
	//get the phonenumber
	CEikRichTextEditor* richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(EMobileNumber));	
	richTextEditor->GetText(mobilenumber);	

	iAppUi.iMobileNo.Copy(mobilenumber);	

	return ETrue;
}

// ---------------------------------------------------------
// CCheckSimDialog::PreLayoutDynInitL()
// 
// ---------------------------------------------------------

void CCheckSimDialog::PreLayoutDynInitL()
{
	TBuf<20> buf1;
	
	buf1.Zero();
		
	buf1.Copy(_L(""));

	CEikRichTextEditor* R1;
    R1 = STATIC_CAST(CEikRichTextEditor*,Control(EUserName));
	R1->SetTextL(&buf1);

	iAppUi.iSettingsScreenValue = 0;
}

TBool CCheckSimDialog::ShutL()
{
	return EFalse;
}

// ---------------------------------------------------------
// CCheckSimDialog::OkToExitL(TInt /*aKeycode*/)
// 
// ---------------------------------------------------------

TInt CCheckSimDialog::OkToExitL(TInt /*aKeycode*/)
{	
	TBuf<20> userid;
	TBuf<20> password;
	
	//get the userid
	CEikRichTextEditor* richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(EUserName));	
	richTextEditor->GetText(userid);	

	//get the phonenumber
	CEikRichTextEditor* richTextEditor1=STATIC_CAST(CEikRichTextEditor*,Control(EPhoneNumber));	
	richTextEditor1->GetText(password);	

	//iAppUi.GetUserSettings(pword);
	//iAppUi.GetUserSettings(iUserNames, iPassword, iMobileNo);
	
	if(userid.Compare(iAppUi.iUserNames)==0 && password.Compare(iAppUi.iPassword)==0)
		iAppUi.iSettingsScreenValue = 1;
	else 
		iAppUi.iSettingsScreenValue = 0;

	return ETrue;
}

// ---------------------------------------------------------
// CChangePassword::PreLayoutDynInitL()
// 
// ---------------------------------------------------------

void CChangePassword::PreLayoutDynInitL()
{	
}

TBool CChangePassword::ShutL()
{
	return EFalse;
}

// ---------------------------------------------------------
// CChangePassword::OkToExitL(TInt /*aKeycode*/)
// 
// ---------------------------------------------------------

TInt CChangePassword::OkToExitL(TInt /*aKeycode*/)
{	
	TBuf<20> newpassword;	
	TBuf<20> oldpassword;	
	
	CEikRichTextEditor* richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(EUserName));	
	richTextEditor->GetText(oldpassword);	

	//get the phonenumber
	CEikRichTextEditor* richTextEditor1=STATIC_CAST(CEikRichTextEditor*,Control(EPhoneNumber));	
	richTextEditor1->GetText(newpassword);	
	
	if (newpassword.Length() < 6)
	{
		iAppUi.ShowMessageBoxFromResource(R_TEXT_PASSWORD_WRONGLENGTH);
		return EFalse;
	}
	
	if(iAppUi.CheckPasswordField(newpassword))
	{
		iAppUi.ShowMessageBoxFromResource(R_TEXT_INVALID_PASSWORD);
		return EFalse;
	}

	if (oldpassword.Length() && newpassword.Length())
	{
		if(oldpassword.Compare(iAppUi.iPassword) == 0)
		{
			iAppUi.iNewPassword.Copy(newpassword);
			iAppUi.ChangePassword();
		}
		else
		{
			iAppUi.ShowMessageBoxFromResource(R_TEXT_INVALID_PASSWORD);
		}
	}
	else
	{
		return EFalse;
	}
	
	return ETrue;	
}

// ---------------------------------------------------------
// CRegisterDialog::PreLayoutDynInitL()
// 
// ---------------------------------------------------------

void CRegisterDialog::PreLayoutDynInitL()
{
	TBuf<30> buf1;
	
	buf1.Copy(iAppUi.iUserNames);

	CEikRichTextEditor* R1;
    R1 = STATIC_CAST(CEikRichTextEditor*,Control(EUserName));
	R1->SetTextL(&buf1);	    
	
}

TBool CRegisterDialog::ShutL()
{
	return EFalse;
}

// ---------------------------------------------------------
// CRegisterDialog::OkToExitL(TInt /*aKeycode*/)
// 
// ---------------------------------------------------------

TInt CRegisterDialog::OkToExitL(TInt /*aKeycode*/)
{	
	TBuf<20> username1;	
	TBuf<20> phonenumber1;	
	
	//get the username
	CEikRichTextEditor* richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(EUserName));
	richTextEditor->GetText(username1);
		
	//get the phonenumber
	richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(EPhoneNumber));	
	richTextEditor->GetText(phonenumber1);
	
	if (phonenumber1.Length() < 6)
	{
		iAppUi.ShowMessageBoxFromResource(R_TEXT_PASSWORD_WRONGLENGTH);
		return EFalse;
	}

	if(iAppUi.CheckPasswordField(phonenumber1))
	{
		iAppUi.ShowMessageBoxFromResource(R_TEXT_INVALID_PASSWORD);
		return EFalse;
	}

	if (username1.Length() && phonenumber1.Length())
	{
		if(iAppUi.CheckSpecialChars(username1,phonenumber1))
		{
			//RegisterRipple(username1, phonenumber1);
			username1.LowerCase();
			iAppUi.iUserNames.Copy(username1);
			iAppUi.iUserName.Copy(username1);
			iAppUi.iPassword.Copy(phonenumber1);		
			
			if(iAppUi.iSettingsScreenValue == 1)
				iAppUi.NewUser();
			else if(iAppUi.iSettingsScreenValue == 2)
				iAppUi.ExistingUser();
		}
		else
			return EFalse;
	}
	else
	{
		return EFalse;
	}
	
	return ETrue;	
}

// ---------------------------------------------------------
// CAlreadyRegisterDialog::PreLayoutDynInitL()
// 
// ---------------------------------------------------------

void CAlreadyRegisterDialog::PreLayoutDynInitL()
{
	TBuf<20> buf1;
	TBuf<20> buf2;
	TBuf<20> buf3;

	buf1.Zero();
	buf2.Zero();
	buf3.Zero();
    	
	buf1.Copy(iAppUi.iUserNames);

	CEikRichTextEditor* R1;
    R1 = STATIC_CAST(CEikRichTextEditor*,Control(EUserName));
	R1->SetTextL(&buf1);	    
	    
	CEikRichTextEditor* R2;
    R2 = STATIC_CAST(CEikRichTextEditor*,Control(EPhoneNumber));
	R2->SetTextL(&buf2);	    
	    
	//CEikRichTextEditor* R3;
    //R3 = STATIC_CAST(CEikRichTextEditor*,Control(EPassword));
	//R3->SetTextL(&buf3);	    
}

TBool CAlreadyRegisterDialog::ShutL()
{
	return EFalse;
}

// ---------------------------------------------------------
// CAlreadyRegisterDialog::OkToExitL(TInt /*aKeycode*/)
// 
// ---------------------------------------------------------

TInt CAlreadyRegisterDialog::OkToExitL(TInt /*aKeycode*/)
{
	TBuf<20> username1;	
	TBuf<20> phonenumber1;	
	TBuf<20> password1;	
	
	//get the username
	CEikRichTextEditor* richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(EUserName));
	richTextEditor->GetText(username1);
	
	//get the phonenumber
	//richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(EPhoneNumber));	
	//richTextEditor->GetText(phonenumber1);
	
	//get the phonenumber
	richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(EPhoneNumber));	
	richTextEditor->GetText(password1);

	if (password1.Length() < 6)
	{
		iAppUi.ShowMessageBoxFromResource(R_TEXT_PASSWORD_WRONGLENGTH);
		return EFalse;
	}

	if(iAppUi.CheckPasswordField(password1))
	{
		iAppUi.ShowMessageBoxFromResource(R_TEXT_INVALID_PASSWORD);
		return EFalse;
	}

	if (username1.Compare(_L("")) == 0 || password1.Compare(_L("")) == 0)
	{
		return EFalse;
	}
	if (1)//username1.Compare(password1) == 0)
	{
		iAppUi.iUserNames.LowerCase();
		iAppUi.iUserName.Copy(iAppUi.iUserNames);								
		iAppUi.iPassword.Copy(password1);		
		
		iAppUi.ExistingUser();
	
		//RegisterRipple(username1, password1, phonenumber1);
		return ETrue;
	}
	else
	{
		CEikonEnv::Static ()->AlertWin(*(iEikonEnv->AllocReadResourceL(R_TEXT_PASSWORD_INCORRECT)));
		return EFalse;
	}
	
	return ETrue;
}

// ---------------------------------------------------------
// CAdvancedDialog::PreLayoutDynInitL()
// 
// ---------------------------------------------------------

void CAdvancedDialog::PreLayoutDynInitL()
{
    TInt iPort;    
	TBuf<80> ip;	
	TInt autostart;
	
	iAppUi.GetSettingsFromDB(ip, iPort, autostart);

	CCoeControl* myAutoPtr = this->Control(EAutoListControlId);
	CEikChoiceList* myAutoButtonList = static_cast<CEikChoiceList*>(myAutoPtr);
	myAutoButtonList->SetCurrentItem(autostart);
}

TBool CAdvancedDialog::ShutL()
{
	return EFalse;
}

// ---------------------------------------------------------
// CAdvancedDialog::OkToExitL(TInt aKeycode)
// 
// ---------------------------------------------------------

TInt CAdvancedDialog::OkToExitL(TInt aKeycode)
{
	TBuf<80> ipAddress;	
	TInt portValue;	

	TInt iAutostart;
#ifdef __S80__
	if (aKeycode == EDone1 || aKeycode == EDone2)
	{
		return EFalse;
	}
#else	
	if (aKeycode == EEikBidCancel)
	{
		return -1;
	}
#endif

	iAppUi.GetSettingsFromDB(ipAddress, portValue, iAutostart);

	CCoeControl* myAutoPtr = this->Control(EAutoListControlId);
	CEikChoiceList* myAutoButtonList = static_cast<CEikChoiceList*>(myAutoPtr);
	iAutostart = myAutoButtonList->CurrentItem();

#ifdef __S80__
	if (aKeycode == EDone3)
#endif
	{
		iAppUi.SaveSettingsToDB(ipAddress, (TUint16)portValue, (TInt8)iAutostart);		
	}	

	return ETrue;
}

// ---------------------------------------------------------
// CAlreadyRegisterDialog::RegisterRipple(TDes& aUser, TDes& aPasswd, TDes& aPhone)
// 
// ---------------------------------------------------------
/*
void CAlreadyRegisterDialog::RegisterRipple(TDes& aUser, TDes& aPasswd, TDes& aPhone)
{
	//Already Registered
	TBuf8<1024> iRBuffer;

	iAppUi.iUserName.Copy(aUser);					
	iAppUi.iUserNames.Copy(aUser);
	iAppUi.iMobileNo.Copy(aPhone);
	iAppUi.iPassword.Copy(aPasswd);
	
	iRBuffer.Copy(_L("<root>"));
	
	HBufC* currTime;	
	TTime ctime;

	ctime.HomeTime();
	currTime = GetTimeStamp(ctime);
	
	iRBuffer.Append(_L("<h>"));
	iRBuffer.Append(_L("<msgt>"));
	iRBuffer.Append(_L("eu"));
	iRBuffer.Append(_L("</msgt>"));

	iRBuffer.Append(_L("<msg></msg>"));

	iRBuffer.Append(_L("<uid>"));
	iRBuffer.Append(aUser);
	iRBuffer.Append(_L("</uid>"));

	iRBuffer.Append(_L("<hp>"));
	iRBuffer.Append(iAppUi.iMobileNo);
	iRBuffer.Append(_L("</hp>"));

	iRBuffer.Append(_L("<did>"));
	iRBuffer.Append(iAppUi.PhoneImei());
	iRBuffer.Append(_L("</did>"));

	iRBuffer.Append(_L("<currec>"));
	iRBuffer.AppendNum(1);
	iRBuffer.Append(_L("</currec>"));

	iRBuffer.Append(_L("<totrec>"));
	iRBuffer.AppendNum(1);
	iRBuffer.Append(_L("</totrec>"));

	iRBuffer.Append(_L("<loc>en_us</loc>"));
	
	iRBuffer.Append(_L("<ctime>"));
	iRBuffer.Append(*currTime);
	iRBuffer.Append(_L("</ctime>"));

	iRBuffer.Append(_L("<lstime>"));
	iRBuffer.Append(_L("0"));
	iRBuffer.Append(_L("</lstime>"));

	iRBuffer.Append(_L("<synctype><mod>cs</mod><gran>contactlevel</gran>\
<adde>true</adde><upde>true</upde><dele>true</dele></synctype>"));

	iRBuffer.Append(_L("</h>"));

	iRBuffer.Append(_L("<b>"));
	iRBuffer.Append(_L("<hp>"));
	iRBuffer.Append(aPhone);
	iRBuffer.Append(_L("</hp>"));
	iRBuffer.Append(_L("<username>"));
	iRBuffer.Append(aUser);
	iRBuffer.Append(_L("</username>"));
	iRBuffer.Append(_L("<password>"));
	iRBuffer.Append(aPasswd);
	iRBuffer.Append(_L("</password>"));
	iRBuffer.Append(_L("</b>"));
	iRBuffer.Append(_L("</root>"));

	HBufC8* content_data;
	content_data=iRBuffer.AllocL();

	TBuf8<10> content_l;
	content_l.Num(content_data->Length());
	//iRBuffer.Copy(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	iRBuffer.Append(_L(" HTTP/1.0\r\nHOST:"));
	iRBuffer.Append(iAppUi.iServerAddr);
	iRBuffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));

	iRBuffer.Append(content_l);
	iRBuffer.Append(_L("\r\n\r\n"));
	iRBuffer.Append(*content_data);

	if (iSocketsEngine != NULL)
	{
		iSocketsEngine->Disconnect();
		delete iSocketsEngine;
		iSocketsEngine = NULL;
	}
	// servername and port from iContactsAppUi

	iSocketsEngine = CSocketsEngineWriter::NewL(iAppUi);
	iSocketsEngine->SetServerName(iAppUi.ServerAddr());
	iSocketsEngine->SetPort(iAppUi.ServerPort());
	iSocketsEngine->iBuffer.Copy(iRBuffer);
	iSocketsEngine->Register(1);
	delete content_data;
}
*/
// ---------------------------------------------------------
// CRegisterDialog::RegisterRipple(TDes& aUser, TDes& aPhone )
// 
// ---------------------------------------------------------
/*
void CRegisterDialog::RegisterRipple(TDes& aUser, TDes& aPhone )
{
	//New User
	TBuf8<1024> iRBuffer;
	
	iAppUi.iUserName.Copy(aUser);						
	iAppUi.iUserNames.Copy(aUser);
	iAppUi.iMobileNo.Copy(aPhone);
	
	iRBuffer.Copy(_L("<root>"));

	HBufC* currTime;	
	TTime ctime;

	ctime.HomeTime();
	currTime = GetTimeStamp(ctime);
	
	iRBuffer.Append(_L("<h>"));
	iRBuffer.Append(_L("<msgt>"));
	iRBuffer.Append(_L("reg"));
	iRBuffer.Append(_L("</msgt>"));

	iRBuffer.Append(_L("<msg></msg>"));

	iRBuffer.Append(_L("<uid>"));
	iRBuffer.Append(aUser);
	iRBuffer.Append(_L("</uid>"));

	iRBuffer.Append(_L("<hp>"));
	iRBuffer.Append(iAppUi.iMobileNo);
	iRBuffer.Append(_L("</hp>"));

	iRBuffer.Append(_L("<did>"));
	iRBuffer.Append(iAppUi.PhoneImei());
	iRBuffer.Append(_L("</did>"));

	iRBuffer.Append(_L("<currec>"));
	iRBuffer.AppendNum(1);
	iRBuffer.Append(_L("</currec>"));

	iRBuffer.Append(_L("<totrec>"));
	iRBuffer.AppendNum(1);
	iRBuffer.Append(_L("</totrec>"));

	iRBuffer.Append(_L("<loc>en_us</loc>"));
	
	iRBuffer.Append(_L("<ctime>"));
	iRBuffer.Append(*currTime);
	iRBuffer.Append(_L("</ctime>"));

	iRBuffer.Append(_L("<lstime>"));
	iRBuffer.Append(_L("0"));
	iRBuffer.Append(_L("</lstime>"));

	iRBuffer.Append(_L("<synctype><mod>cs</mod><gran>contactlevel</gran>\
<adde>true</adde><upde>true</upde><dele>true</dele></synctype>"));

	iRBuffer.Append(_L("</h>"));

	iRBuffer.Append(_L("<b>"));
	iRBuffer.Append(_L("<hp>"));
	iRBuffer.Append(aPhone);
	iRBuffer.Append(_L("</hp>"));
	iRBuffer.Append(_L("<username>"));
	iRBuffer.Append(aUser);
	iRBuffer.Append(_L("</username>"));
	//iRBuffer.Append(_L("</registration>"));
	iRBuffer.Append(_L("</b>"));
	iRBuffer.Append(_L("</root>"));

	HBufC8* content_data;
	content_data=iRBuffer.AllocL();

	TBuf8<10> content_l;
	content_l.Num(content_data->Length());
	//iRBuffer.Copy(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	iRBuffer.Append(_L(" HTTP/1.0\r\nHOST:"));
	iRBuffer.Append(iAppUi.iServerAddr);
	iRBuffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));

	iRBuffer.Append(content_l);
	iRBuffer.Append(_L("\r\n\r\n"));
	iRBuffer.Append(*content_data);

	if (iSocketsEngine != NULL)
	{
		iSocketsEngine->Disconnect();
		delete iSocketsEngine;
		iSocketsEngine = NULL;
	}
	// servername and port from iPhotosAppUi

	iSocketsEngine = CSocketsEngineWriter::NewL(iAppUi);
	iSocketsEngine->SetServerName(iAppUi.ServerAddr());
	iSocketsEngine->SetPort(iAppUi.ServerPort());
	iSocketsEngine->iBuffer.Copy(iRBuffer);
	iSocketsEngine->Register(0);
	delete content_data;
}
*/
// ---------------------------------------------------------
// CFolderDialog::PreLayoutDynInitL()
// 
// ---------------------------------------------------------

void CFolderDialog::PreLayoutDynInitL()
    {    
      //FolderList = new (ELeave) CDesCArrayFlat(20);
      
      FolderList=iAppUi.GetFolderList();
      
      /*CEikChoiceList* FolderEditor = STATIC_CAST(CEikChoiceList*,Control(EFolderList));
	  FolderEditor->SetArrayL(FolderList);

	  CEikRichTextEditor* richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(EFolderName));
	  richTextEditor->MakeVisible(EFalse);*/

	  CCoeControl* myControlPtr = this->Control(EFolderList);    
	  CEikComboBox* myComboBox = static_cast<CEikComboBox*>(myControlPtr);
      // Set the control with the array identified by the specified id.
      myComboBox->SetArray(FolderList);
    }

TBool CFolderDialog::ShutL()
{
	return EFalse;
}

// ---------------------------------------------------------
// CFolderDialog::OkToExitL(TInt aKeycode)
// 
// ---------------------------------------------------------

TInt CFolderDialog::OkToExitL(TInt aKeycode)
	{	
//		TBuf<20> Name;
		TBuf<20> Name1;
#ifdef __S80__
	if (aKeycode == EDone1 || aKeycode == EDone2)
	{
		return EFalse;
	}
#endif
   
	    //get the fodlername
		CEikComboBox* FolderEditor = STATIC_CAST(CEikComboBox*,Control(EFolderList));
		FolderEditor->GetText(Name1);		

#ifdef __S80__
		if(aKeycode == EDone3 && Name1.Length()==0)
			return EFalse;
#else
		if(Name1.Length()==0)
			return EFalse;
#endif

		iAppUi.FolderName.Copy(Name1);

		/*CEikRichTextEditor* richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(EFolderName));
		richTextEditor->GetText(Name);

		CEikChoiceList* FolderEditor = STATIC_CAST(CEikChoiceList*,Control(EFolderList));
		if (FolderList)
		{
			TInt aListPos = FolderEditor->CurrentItem();
			if ( aListPos  > 0)
			{
				Name1.Copy((*FolderList)[aListPos]);
			}
			else
			{
				Name1.Copy((*FolderList)[0]);
			}
		}*/

		/*if(iAppUi.GetMode() == 0)
		{
	    	CDesCArray* array;
			TInt aPos;
			TInt flg = 0;
			array = iAppUi.GetFolderList();

			if(array != NULL)
			{
				if(array->Find(Name1, aPos) == 0)
				{
					iAppUi.FolderName.Copy(Name1);
					flg = 0;
				}
				else
				{
					iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_FOLDERNOTEXIST);
					flg = 1;
				}
			}
			else
			{
				iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_FOLDERNOTEXIST);
				flg = 1;
			}
			if( flg == 1)
			{
				return EFalse;
			}
		}
		else
		{
			if (Name.Length())
			{
				iAppUi.FolderName.Copy(Name);
			}
			else
			{
				if (FolderList)
				{
					TInt item = FolderEditor->CurrentItem();
					if ( item > 0)
					{
						iAppUi.FolderName.Copy((*FolderList)[item]);
					}
					else
					{
						iAppUi.FolderName.Copy((*FolderList)[0]);
					}
				}
				else
				{
					return EFalse;
				}
			}
		}*/

		return ETrue;
	}

void CProcessAlbumDialog::PreLayoutDynInitL()
{    
  iAppUi.iSettingsScreenValue  = -1;
}

TBool CProcessAlbumDialog::ShutL()
{
	return EFalse;
}

// ---------------------------------------------------------
// CFolderDialog::OkToExitL(TInt aKeycode)
// 
// ---------------------------------------------------------
TInt CProcessAlbumDialog::OkToExitL(TInt aKeycode)
{	
#ifdef __S80__
	if (aKeycode == EDone1 || aKeycode == EDone2)
	{
		return EFalse;
	}
#endif
   
	CCoeControl* myAutoPtr = this->Control(EProcessImageListId);
	CEikChoiceList* myAutoButtonList = static_cast<CEikChoiceList*>(myAutoPtr);
	iAppUi.iSettingsScreenValue = myAutoButtonList->CurrentItem();

	return ETrue;
}

// ---------------------------------------------------------
// CFolderDialog1::PreLayoutDynInitL()
// 
// ---------------------------------------------------------
	
void CFolderDialog1::PreLayoutDynInitL()
    {    
      //FolderList = new (ELeave) CDesCArrayFlat(20);
      
      FolderList=iAppUi.GetFolderList();
      
      CEikChoiceList* FolderEditor = STATIC_CAST(CEikChoiceList*,Control(EFolderList));
	  FolderEditor->SetArrayL(FolderList);
    }

TBool CFolderDialog1::ShutL()
{
	return EFalse;
}

// ---------------------------------------------------------
// CFolderDialog1::OkToExitL(TInt aKeycode)
// 
// ---------------------------------------------------------

TInt CFolderDialog1::OkToExitL(TInt aKeycode)
	{	
		TBuf<20> Name1;
#ifdef __S80__
	if (aKeycode == EDone1 || aKeycode == EDone2)
	{
		return EFalse;
	}
#endif
		CEikChoiceList* FolderEditor = STATIC_CAST(CEikChoiceList*,Control(EFolderList));
		if (FolderList)
		{
			TInt aListPos = FolderEditor->CurrentItem();
			if ( aListPos  > 0)
			{
				Name1.Copy((*FolderList)[aListPos]);
			}
			else
			{
				Name1.Copy((*FolderList)[0]);
			}
		}
#ifdef __S80__
	if (aKeycode == EDone3)
#endif
	{
		if(iAppUi.GetMode() == 0)
		{
	    	CDesCArray* array;
			TInt aPos;
			TInt flg = 0;
			array = iAppUi.GetFolderList();

			if(array != NULL)
			{
				if(array->Find(Name1, aPos) == 0)
				{
					iAppUi.FolderName.Copy(Name1);
					flg = 0;
				}
				else
				{
					iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_FOLDERNOTEXIST);
					flg = 1;
				}
			}
			else
			{
				iAppUi.ShowMessageBoxFromResource(R_EXAMPLE_TEXT_FOLDERNOTEXIST);
				flg = 1;
			}
			if( flg == 1)
			{
				return EFalse;
			}
		}
	}
		return ETrue;
	}

// ---------------------------------------------------------
// CUnShareAlbumDialog::PreLayoutDynInitL()
// 
// ---------------------------------------------------------

void CUnShareAlbumDialog::PreLayoutDynInitL()
    {    
      //FolderList = new (ELeave) CDesCArrayFlat(20);
      
      FolderList=iAppUi.GetFolderList();
      
      CEikChoiceList* FolderEditor = STATIC_CAST(CEikChoiceList*,Control(EFolderList));
	  FolderEditor->SetArrayL(FolderList);
    }

TBool CUnShareAlbumDialog::ShutL()
{
	return EFalse;
}

// ---------------------------------------------------------
// CUnShareAlbumDialog::OkToExitL(TInt aKeycode)
// 
// ---------------------------------------------------------

TInt CUnShareAlbumDialog::OkToExitL(TInt aKeycode)
	{	
#ifdef __S80__
	if (aKeycode == EDone1 || aKeycode == EDone2)
	{
		return EFalse;
	}
#endif

#ifdef __S80__
	if (aKeycode == EDone3)
#endif
	{
		CEikChoiceList* FolderEditor = STATIC_CAST(CEikChoiceList*,Control(EFolderList));
		if (FolderList)
		{
			TInt aListPos = FolderEditor->CurrentItem();
			if ( aListPos  > 0)
			{
				iAppUi.albumName.Copy((*FolderList)[aListPos]);
			}
			else
			{
				iAppUi.albumName.Copy((*FolderList)[0]);
			}

#ifdef __S80__
			if(CCknConfirmationDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),*(iEikonEnv->AllocReadResourceL(R_TEXT_UNSHARE_ALBUM_CONFIRM))))
				iAppUi.GetUnSharingAlbumXML();
#else
			CEikDialog* dialog;
			dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_TEXT_UNSHARE_ALBUM_CONFIRM)), R_EXAMPLE_RIPPLE_PUBLISH);
			if(dialog->ExecuteLD(R_EIK_DIALOG_YESNO_INFO))
				iAppUi.GetUnSharingAlbumXML();
#endif
		}
	}
		return ETrue;
	}

// ---------------------------------------------------------
// CSharingListBoxDialog::PreLayoutDynInitL()
// 
// ---------------------------------------------------------

void CSharingListBoxDialog::PreLayoutDynInitL()
{          
	FolderList=iAppUi.GetFolderList();
      
    CEikChoiceList* FolderEditor = STATIC_CAST(CEikChoiceList*,Control(EFolderList));
	FolderEditor->SetArrayL(FolderList);
}

TBool CSharingListBoxDialog::ShutL()
{
	return EFalse;
}

// ---------------------------------------------------------
// CSharingListBoxDialog::OkToExitL(TInt aKeycode)
// 
// ---------------------------------------------------------

TInt CSharingListBoxDialog::OkToExitL(TInt aKeycode)
{	
#ifdef __S80__
	if (aKeycode == EDone1 || aKeycode == EDone2)
	{
		return EFalse;
	}
#endif
#ifdef __S80__
	if (aKeycode == EDone3)
#endif
	{
	CEikChoiceList* FolderEditor = STATIC_CAST(CEikChoiceList*,Control(EFolderList));
	if (FolderList)
	{
		TInt aListPos = FolderEditor->CurrentItem();
		if ( aListPos  > 0)
		{
			iAppUi.albumName.Copy((*FolderList)[aListPos]);
		}
		else
		{
			iAppUi.albumName.Copy((*FolderList)[0]);
		}
	}

	CEikRichTextEditor* richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(ESendNames));
	richTextEditor->GetText(iAppUi.iMessage);	
	}
	return ETrue;
}

// ---------------------------------------------------------
// CDescDialog::PreLayoutDynInitL()
// 
// ---------------------------------------------------------

void CDescDialog::PreLayoutDynInitL()
    {          
    }

TBool CDescDialog::ShutL()
{
	return EFalse;
}

// ---------------------------------------------------------
// CDescDialog::OkToExitL(TInt aKeycode)
// 
// ---------------------------------------------------------

TInt CDescDialog::OkToExitL(TInt aKeycode)
	{	
		TBuf<150> Name;
		
	    //get the descname
		CEikRichTextEditor* richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(EDescName));
		richTextEditor->GetText(Name);

		iAppUi.DescName.Copy(Name);
		
		return ETrue;
	}

// ---------------------------------------------------------
// CAdvancedDialog::SetAccessPoints()
// 
// ---------------------------------------------------------

//void CAdvancedDialog::SetAccessPoints()
//{
//	RGenericAgent netAgent1;
//	if (netAgent1.Open() == KErrNone)
//	{
//		
//		CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref pref;
//
//		pref.iRanking = 1;
//		pref.iDirection = ECommDbConnectionDirectionOutgoing;
//		pref.iDialogPref = ECommDbDialogPrefDoNotPrompt;
//
//		CCommsDbConnectionPrefTableView::TCommDbIapBearer bearer;
//		bearer.iBearerSet = ECommDbBearerGPRS;
//		
//		bearer.iIapId = 12;
//		pref.iBearer = bearer;
//
//		CCommDbOverrideSettings* aSettings;
//		
//		aSettings = CCommDbOverrideSettings::NewL(CCommDbOverrideSettings::EParamListFull);
//		aSettings->SetConnectionPreferenceOverride(pref);
//	
//	}
//}

// ---------------------------------------------------------
// CAdvancedDialog::GetAccessPoints()
// 
// ---------------------------------------------------------
/*
void CAdvancedDialog::GetAccessPoints()
{
	int count=1;
	int val;
	RGenericAgent netAgent;
	if (netAgent.Open() == KErrNone)
	{
		TBuf<50> Name;
		TBuf<50> test;
		TUint32 ida=0;
		TUint32 ida1=0;
		TBuf<20> TableName;
		TableName.Copy(_L("IAP"));

		TBuf<20> ColName1;
		ColName1.Copy(_L("Id"));
		
		TBuf<20> ColName2;
		ColName2.Copy(_L("Name"));
			
		CleanupClosePushL(netAgent);	
		CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeIAP);
		CleanupStack::PushL(db);
		
		
		CCommsDbConnectionPrefTableView* 
		view1 = db->OpenConnectionPrefTableViewOnRankLC(ECommDbConnectionDirectionOutgoing,1);
		CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref pref;
		if(view1->GotoFirstRecord()!=KErrNotFound)
		{
			view1->ReadConnectionPreferenceL(pref);
			ida=pref.iBearer.iIapId;			
		}
		
		
		CCommsDbTableView* 
			view = db->OpenTableLC(TableName);		
		if(view->GotoFirstRecord()!=KErrNotFound)
		{
			do
			{								
				view->ReadUintL(ColName1, ida1);
				view->ReadTextL(ColName2, Name);
				if(ida==ida1)
				{				
					
					array->InsertL(0,Name);
					ApNum[0]=ida1;

					val=ApNum[0];
					test.AppendNum(val);		
				}
				else
				{	
					array->AppendL(Name);
					ApNum[count]=ida1;	
					
					val=ApNum[count];
					test.AppendNum(val);
					count++;							
				}				
			}
			while(view->GotoNextRecord()!=KErrNotFound);
		}
		
		CleanupStack::PopAndDestroy(4); // view, db, netAgent			
	}
}
*/
// ---------------------------------------------------------
// CRippleVaultAppUi::FolderDialog()
// 
// ---------------------------------------------------------

TInt CRippleVaultAppUi::FolderDialog()
{
	CEikDialog* folderdialog;	
	TInt err;	
	//TBuf<30> temp;
	folderdialog = new (ELeave) CFolderDialog(*this);	
	//err = dialog->ExecuteLD(R_FOLDER_DIALOG);
	folderdialog->PrepareLC(R_FOLDER_DIALOG);
	
	//iPhotoCount = iPhotoCount * 3;
//	TInt price;
//	TLex lex(iCharge);
//	lex.Val(price);
	
//	iPhotoCount = iPhotoCount * price;

//	temp.Copy(_L("Charge "));
//	temp.AppendNum(iPhotoCount);
//	temp.Append(_L(" Pesos"));

	//temp.Copy(_L("Select Album:"));
	
	//folderdialog->SetTitleL(temp);
	err = folderdialog->RunLD();

	return err;
}

TInt CRippleVaultAppUi::ProcessAlbum()
{
	CEikDialog* folderdialog;	
	TInt err;	
	folderdialog = new (ELeave) CProcessAlbumDialog(*this);	
	folderdialog->PrepareLC(R_PROCESS_IMAGE_DIALOG);
	
	err = folderdialog->RunLD();

	return iSettingsScreenValue;
}
// ---------------------------------------------------------
// CRippleVaultAppUi::FolderDialog1()
// 
// ---------------------------------------------------------

TInt CRippleVaultAppUi::FolderDialog1()
{
	CEikDialog* folderdialog1;	
	TInt err;
	
	folderdialog1 = new (ELeave) CFolderDialog1(*this);
	err = folderdialog1->ExecuteLD(R_FOLDER_DIALOG1);

	return err;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SharingListBoxDialog()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::SharingListBoxDialog(TInt aCount)
{
	CEikDialog* sharelistdialog;	
	TInt err;
	
	sharelistdialog = new (ELeave) CSharingListBoxDialog(*this);
	err = sharelistdialog->ExecuteLD(R_SHARING_LISTBOX_DIALOG);
	if(err)
	{				
		SaveMails(albumName,MailNames,MailId,iMessage,aCount);
		GetSharingAlbumXML();
	}
}

// ---------------------------------------------------------
// CRippleVaultAppUi::DescDialog()
// 
// ---------------------------------------------------------

TInt CRippleVaultAppUi::DescDialog()
{
	CEikDialog* descdialog;	
	TInt err;
	
	descdialog = new (ELeave) CDescDialog(*this);
	err = descdialog->ExecuteLD(R_DESC_DIALOG);

	return err;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetTimeStamp(TTime& aTime)
// 
// ---------------------------------------------------------
/*
HBufC* CRegisterDialog::GetTimeStamp(TTime& aTime)
{
	TBuf<256> timestamp;

	TDateTime janNineteenSeventy(1970,EJanuary,0,00,00,00,000000);

	TTime timejan(janNineteenSeventy);

	TTimeIntervalMicroSeconds interval = aTime.MicroSecondsFrom(timejan);
	TInt64 k;
	k=interval.Int64()/1000;
	timestamp.Num(k);

	HBufC* ts = NULL;
	ts = timestamp.AllocLC();
	CleanupStack::Pop();

	return (ts);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetTimeStamp(TTime& aTime)
// 
// ---------------------------------------------------------

HBufC* CAlreadyRegisterDialog::GetTimeStamp(TTime& aTime)
{
	TBuf<256> timestamp;

	TDateTime janNineteenSeventy(1970,EJanuary,0,00,00,00,000000);

	TTime timejan(janNineteenSeventy);

	TTimeIntervalMicroSeconds interval = aTime.MicroSecondsFrom(timejan);
	TInt64 k;
	k=interval.Int64()/1000;
	timestamp.Num(k);

	HBufC* ts = NULL;
	ts = timestamp.AllocLC();
	CleanupStack::Pop();

	return (ts);
}
*/
// ---------------------------------------------------------
// CRippleVaultAppUi::GetTextFieldFromFieldSet(CContactItemFieldSet& aFieldSet, const TUid aField,TDesC& name)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetTextFieldFromFieldSet(CContactItemFieldSet& aFieldSet, const TUid aField,TDesC& name)
{
	TInt index = aFieldSet.Find(aField);

    if ((index >= 0) && (index < aFieldSet.Count()))
    {
   	    CContactItemField& textfield = aFieldSet[index];
        CContactTextField* textValue = textfield.TextStorage();

		((TDes&)name).Copy(textValue->Text());		
    } 
}
#endif

// ---------------------------------------------------------
// CRippleVaultAppUi::SelectAccess()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::SelectAccess()
{
	TUint32 aIAPId=0; 	
/*
#ifndef __UIQ__	
	CIntConnectionInitiator *iInitConn; 
	iInitConn = CIntConnectionInitiator::NewL(); 
	
	iInitConn->GetActiveIap(aIAPId);
#endif
*/	
	if(aIAPId ==0 )
	{
		//TBuf<150> msg;
		//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_VAULT_MESSAGE)));		
#ifndef __UIQ__			
		CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_VAULT_MESSAGE)));
		dlg->PrepareLC(R_ACCESS_POINT);
		dlg->RunLD();
#else
		//TBuf<150> msgtitle;
		//msgtitle.Copy(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)));		
		//iEikonEnv->InfoWinL(*(iEikonEnv->AllocReadResourceL(R_TEXT_SETUP_WIZARD)), *(iEikonEnv->AllocReadResourceL(R_VAULT_MESSAGE)));
#ifdef __S80__
		CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_TEXT_SETUP_WIZARD)), *iEikonEnv->AllocReadResourceL(R_VAULT_MESSAGE));
#else
		CEikDialog* dialog;
		dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_VAULT_MESSAGE)), R_TEXT_SETUP_WIZARD);
		dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif		
#endif

#ifndef __UIQ__			
		CAknMessageQueryDialog* dlg1 = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_VAULT_MESSAGE1)));
		dlg1->PrepareLC(R_ACCESS_POINT);
		dlg1->SetHeaderTextL(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_1)));
		dlg1->RunLD();
#else
		//TBuf<150> msgtitle;
		//msgtitle.Copy(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)));		
		//iEikonEnv->InfoWinL(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_1)), *(iEikonEnv->AllocReadResourceL(R_VAULT_MESSAGE1)));
#ifdef __S80__
		CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_1)), *iEikonEnv->AllocReadResourceL(R_VAULT_MESSAGE1));
#else		
		CEikDialog* dialog1;
		dialog1 = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_VAULT_MESSAGE1)), R_TEXT_SETUP_WIZARD);
		dialog1->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif
#endif		
		
//		TRequestStatus status;
		RGenericAgent netAgent;		
		
		if (netAgent.Open() == KErrNone)
		{
			CleanupClosePushL(netAgent);
			
			//KillBrowser();

			if(netAgent.Stop()==KErrNone)//;
			{	
			CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeIAP);
			CleanupStack::PushL(db);
			CCommsDbConnectionPrefTableView* 
//			view = db->OpenConnectionPrefTableViewOnRankLC(ECommDbConnectionDirectionOutgoing,0);
			view = db->OpenConnectionPrefTableInRankOrderLC(ECommDbConnectionDirectionOutgoing);
			
			
			if(view->GotoFirstRecord()!=KErrNotFound)
			{
//				TBuf8<40> log;
//				log.Copy(_L("startaccess"));
//				WriteLogFile(log);
				/*view->UpdateDialogPrefL(ECommDbDialogPrefPrompt);
				TRequestStatus status1;
				netAgent.StartOutgoing(status1);
				User::WaitForRequest(status1);*/
				
				view->UpdateDialogPrefL(ECommDbDialogPrefDoNotPrompt);				
			}

			CleanupStack::PopAndDestroy(3); // view, db, netAgent
			}
			else{
			CleanupStack::PopAndDestroy(); // netAgent
			}
		}
	}
}

void CRippleVaultAppUi::SelectAccessSettings()
{
	TUint32 aIAPId=0; 	
	if(aIAPId ==0 )
	{
		RGenericAgent netAgent;		
		
		if (netAgent.Open() == KErrNone)
		{
			CleanupClosePushL(netAgent);
			
			if(netAgent.Stop()==KErrNone)//;
			{	
			CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeIAP);
			CleanupStack::PushL(db);
			CCommsDbConnectionPrefTableView* 
//			view = db->OpenConnectionPrefTableViewOnRankLC(ECommDbConnectionDirectionOutgoing,0);
			view = db->OpenConnectionPrefTableInRankOrderLC(ECommDbConnectionDirectionOutgoing);
			
			
			if(view->GotoFirstRecord()!=KErrNotFound)
			{
//				TBuf8<40> log;
//				log.Copy(_L("startaccess"));
//				WriteLogFile(log);
				view->UpdateDialogPrefL(ECommDbDialogPrefPrompt);
				TRequestStatus status1;
				netAgent.StartOutgoing(status1);
				User::WaitForRequest(status1);
				
				view->UpdateDialogPrefL(ECommDbDialogPrefDoNotPrompt);				
			}

			CleanupStack::PopAndDestroy(3); // view, db, netAgent
			}
			else{
			CleanupStack::PopAndDestroy(); // netAgent
			}

		}
	}
}

// ---------------------------------------------------------
// CRippleVaultAppUi::UserRegistered()
// Called after registration is complete
// ---------------------------------------------------------

void CRippleVaultAppUi::UserRegistered()
{
	TTime btime;
	btime.HomeTime();

	iPhotosDB->UpdateBackupTimestampToDB(btime);

	iSetupWizardScreenValue = 1;

	CreateInitUploadFileL();

	SaveApnID(iApnID);

#ifdef __UIQ__				   
	
#ifdef __S80__	
	iMode = MODE_OFF;
#else	
	iMode = MODE_PROMPT;
#endif

	iDescription=0;
	iRoaming=0;
	iSecureGallery=1;

	//GetSmsSettingsFromDB(iSmsMode,iSmsTime,iSmsRoam);
	TInt smstype;
	TTime time;
	TInt smsroam;
	
	TInt iContactRoaming=0;
	TInt iContactsUpload=0;
	
	GetSmsSettingsFromDB(smstype, time, smsroam);
	
	iSmsMode = smstype;
	iSmsTime = time;
	iSmsRoam = smsroam;

	iSmsMode = 1;
	iSmsRoam = 0;

	iContactRoaming = 0;

	_LIT(KTimeString,"12:00.00am");

	TInt returnvalue=iSmsTime.Parse(KTimeString); 

	SaveSettingsToDB(iServerAddr, iPort, iAutostart);
	SaveContactsSettingsToDB((TInt8)iContactRoaming, (TInt8)iContactsUpload);
	SavePhotosSettingsToDB(iMode, iDescription, iRoaming, iSecureGallery);
	SaveSmsSettingsToDB(iSmsMode,iSmsTime,iSmsRoam);

	/*TInetAddr Addr;	
	TInt port;
	TInt autostart;

	GetSettingsFromDB(iServerAddr, port, autostart);
		
	iPort = port;
	Addr.Input(iServerAddr);	
	iAutostart = autostart;
	
	SaveSettingsToDB(iServerAddr, iPort, iAutostart);*/

	ShowWindow(KViewIdStatusScreen); //main screen
	DeleteWindow(KViewIdUnregisterScreen); //unregisterview

	/*iSettingsString.Copy(_L(""));
	TBuf<60> tmpstr;
	tmpstr.Copy(_L("settings"));
	LoadFromConfigFile(iSettingsString,tmpstr );

	if(iSettingsString.Length() == 0){
		CallSetupWizardStep1();
		
	}
	else{
		SaveChoice(1);
		SaveSettingsString();
	}*/

	CallSetupWizardUploadAll();
	//SaveChoice(1);
#else
	//iPrevViewId = TUid::Uid(15);
	iPrevViewId = KViewIdStatusScreen;

	//TInetAddr Addr;	
	//TInt port;
	//TInt mode;
	TInt desc=1;
	TInt roam=0;
	TInt autostart = 1;
	TInt securegallery = 1;

#ifdef __LOGME__
		LogText.Copy(_L("serverip1="));
		LogText.Append(iServerAddr);
		LogText.Append(_L("\n"));
		WriteLogFile(LogText);
#endif

//	GetSettingsFromDB(iServerAddr, port, mode, desc, roam, autostart);
	//GetSettingsFromDB(iServerAddr, port, autostart);
	//GetPhotosSettingsFromDB(mode, desc, roam, securegallery);		
#ifdef __LOGME__
		LogText.Copy(_L("serverip2="));
		LogText.Append(iServerAddr);
		LogText.Append(_L("\n"));
		WriteLogFile(LogText);
#endif
	//iPort = port;
	//Addr.Input(iServerAddr);
	iMode = MODE_PROMPT;
	iDescription=desc;
	iRoaming=roam;
	iAutostart = autostart;
	iSecureGallery = securegallery;

	iSmsMode = 1;
	iSmsRoam = 0;

	_LIT(KTimeString,"12:00.00am");

	TInt returnvalue=iSmsTime.Parse(KTimeString); 

#ifdef __LOGME__
		LogText.Copy(_L("serverip3="));
		LogText.Append(iServerAddr);
		LogText.Append(_L("\n"));
		WriteLogFile(LogText);
#endif

	SaveSettingsToDB(iServerAddr, iPort, iAutostart);
	SavePhotosSettingsToDB(iMode, iDescription, iRoaming, iSecureGallery);
	SaveSmsSettingsToDB(iSmsMode,iSmsTime,iSmsRoam);	

	ActivateLocalViewL(KViewIdStatusScreen);
	//iPrevViewId = TUid::Uid(15);
	//ActivateLocalViewL(TUid::Uid(15));
#endif
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SetTitle()
// set the app title
// ---------------------------------------------------------
/*
void CRippleVaultAppUi::SetTitle()
{
#ifndef __UIQ__	
	CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
	statusPane->SwitchLayoutL(R_AVKON_STATUS_PANE_LAYOUT_USUAL);	
	CAknTitlePane* tp=(CAknTitlePane*)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidTitle)); 	

	//tp->SetTextL(_L("Ripple Vault")); 
	tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH))); 
#endif
}
*/
// ---------------------------------------------------------
// CRippleVaultAppUi::ActivateView(TInt aViewId)
// active view given an id
// ---------------------------------------------------------

void CRippleVaultAppUi::ActivateView(TUid aViewId)
{
#ifndef __UIQ__
	if(aViewId != KViewIdEmptyScreen)
		iPrevViewId = aViewId;//TUid::Uid(aViewId);
	//ActivateLocalViewL(TUid::Uid(aViewId));
	ActivateLocalViewL(aViewId);
#endif
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ShowMessageBoxFromResource(TInt aResourceId)
// show message box from resource
// ---------------------------------------------------------

void CRippleVaultAppUi::ShowMessageBoxFromResource(TInt aResourceId)
{
	//HBufC*  message;
	//message = iEikonEnv->AllocReadResourceL(aResourceId);
#ifndef __UIQ__			
	CAknMessageQueryDialog* verdlg = CAknMessageQueryDialog::NewL(*iEikonEnv->AllocReadResourceL(aResourceId));
	verdlg->PrepareLC(R_ABOUT_QUERY);
	verdlg->RunLD();
#else
	//CEikonEnv::Static()->InfoWinL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),*iEikonEnv->AllocReadResourceL(aResourceId));
#ifdef __S80__
	CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)), *iEikonEnv->AllocReadResourceL(aResourceId));
#else
	CEikDialog* dialog;
	dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(aResourceId)), R_EXAMPLE_RIPPLE_PUBLISH);
	dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif
#endif
//	CEikonEnv::Static ()->InfoWinL(_L("Ripple Vault:"),*message);
	//delete message;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CheckPasswordField(TDesC& password)
// check if password is numeric
// ---------------------------------------------------------

#ifdef __UIQ__
TInt CRippleVaultAppUi::CheckPasswordField(TDesC& password)
{
	TInt length;
	TInt flag=0;
	
	length = password.Length();

	for(int i=0; i<length; i++)
	{		
		if(password[i] == '1' || password[i] == '2' || password[i] == '3' || password[i] == '4' || password[i] == '5'
			|| password[i] == '6' || password[i] == '7' || password[i] == '8' || password[i] == '9' || password[i] == '0')
		{
			flag = 0;
		}
		else
		{
			flag = 1;
			break;
		}
	}

	if(flag == 0)
	{
		return 0;
	}
	else
		return 0;
}

#endif

// ---------------------------------------------------------
// CRippleVaultAppUi::ImageFolderPath()
// Return iStoragePath
// ---------------------------------------------------------

TInt CRippleVaultAppUi::ImageFolderPath()
{		
	return iStoragePath;
}


// ---------------------------------------------------------------------------
// CRippleVaultAppUi::ApplicationDriveAndPath()
//
// Get the application path and drive. It must be done differently in the
// development environment and in the device.
// ---------------------------------------------------------------------------
//
TFileName CRippleVaultAppUi::ApplicationDriveAndPath()
{	
#if 1
	//#define EMULATOR //For testing
    TFileName appfullname(CEikonEnv::Static()->EikAppUi()->Application()->AppFullName());
    TParse parse;
#ifdef __UIQ__
#ifdef __WINSCW__

    // On development environment the AppFullName points to z drive.
    // Replace it to point to C drive, which is writable by our application.
    parse.Set(_L("c:"), &appfullname, NULL);

#else // In device use the application fullname directly.
    parse.Set(appfullname, NULL, NULL);
#endif
#else
	//series 60 code.
#ifdef EMULATOR

    // On development environment the AppFullName points to z drive.
    // Replace it to point to C drive, which is writable by our application.
    parse.Set(_L("c:"), &appfullname, NULL);

#else // In device use the application fullname directly.

    parse.Set(appfullname, NULL, NULL);

#endif
#endif//__UIQ__
    TFileName fn = parse.DriveAndPath();
    // Make sure the path exists (create if not). This is needed in EMULATOR.

    BaflUtils::EnsurePathExistsL(CCoeEnv::Static()->FsSession(), fn);
	
#else	
	TFileName fn;
	fn.Copy(_L("c:\\system\\apps\\RippleVault\\"));
#endif
		
	return fn;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::LoadFromConfigFile(TDes& aValue, TDesC& aParam)
// Load Parameter value from Config File
// ---------------------------------------------------------

TBool CRippleVaultAppUi::LoadFromConfigFile(TDes& aValue, TDesC& aParam)
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
		TInt err = file.Open(fs, path, EFileRead|EFileShareAny);
		if (err == KErrNone) 
		{
			TBuf8<2> buf;
			TBuf<2> buf16;
			TBuf<150> line;
			TBuf<150> param;

			param.Copy(aParam);
			param.Append(_L("="));

			TInt len = param.Length();

			line.Copy(_L(""));

			TInt count = 0;
			do 
			{
				/*TInt readErr = */ file.Read(buf, 1);
				if (buf.Length())
				count++;

				buf16.Copy(buf);
				line.Append(buf16);

				if (buf.Length() == 0 || buf.Find(_L8("\n")) != KErrNotFound ||
				buf.Find(_L8("\r")) != KErrNotFound) 
				{
					if (line.Find(param) != KErrNotFound && line.Length() > len) 
					{
						line.Delete(0,len);
						//line.SetLength(count - 7);
						if (buf.Find(_L8("\n")) != KErrNotFound || buf.Find(_L8("\r")) != KErrNotFound)
						{
							line.SetLength(line.Length() - 1);
						}
						
						//decode if hp or userid iDecodedMobileNo	
						TInt64 decode=0;
						if(param.Find(_L("hp"))!= KErrNotFound || param.Find(_L("charge")) != KErrNotFound )
						{
							TBuf<10> digits;
							digits.Copy(_L("0123456789"));

							if(param.Find(_L("hp"))!= KErrNotFound )
							{
								iDecodedMobileNo.Copy(line);
							}
						
							decode=0;
							for(TInt i=0;i<line.Length();++i)
							{
								TBuf<1> t;
								t.Copy(&line[i],1);
												
								TInt64 power=ReturnPower(16, line.Length()-1-i);			 	

								if(digits.Find(t) != KErrNotFound)
								{
									decode=decode+((TInt64)digits.Find(t)*power);
								}
								if(t.Find(_L("A"))!= KErrNotFound||t.Find(_L("a"))!=KErrNotFound){
									decode=decode+((TInt64)10*power);
								}
								if(t.Find(_L("B"))!= KErrNotFound||t.Find(_L("b"))!=KErrNotFound){
									decode=decode+((TInt64)11*power);
								}
								if(t.Find(_L("C"))!= KErrNotFound||t.Find(_L("c"))!=KErrNotFound){
									decode=decode+((TInt64)12*power);
								}
								if(t.Find(_L("D"))!= KErrNotFound||t.Find(_L("d"))!=KErrNotFound){
									decode=decode+((TInt64)13*power);
								}
								if(t.Find(_L("E"))!= KErrNotFound||t.Find(_L("e"))!=KErrNotFound){
									decode=decode+((TInt64)14*power);
								}
								if(t.Find(_L("F"))!= KErrNotFound||t.Find(_L("f"))!=KErrNotFound){
									decode=decode+((TInt64)15*power);
								}
							}
						}	
						if(param.Find(_L("hp"))!= KErrNotFound)
						{
							decode = decode - (TInt64)10101010;	
							aValue.Num(decode);
						}
/*						else if(param.Find(_L("charge"))!= KErrNotFound)
						{
							TInt64 number;
							decode = decode - (TInt64)10101010;
							TLex lex(iMobileNo);
							TRAPD(err, lex.Val(number));

							decode=	decode/number;	
							TInt charge = decode.GetTInt();
							aValue.Num(charge);
						}*/
						else
						{
							aValue.Copy(line);
						}
						ret = TRUE;
						break;
					}
					line.Copy(_L(""));
					count = 0;
				}
			}
			while(buf.Length() != 0);
		}
		file.Close();
	}
	fs.Close();
	return ret;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::MapAPNtoActualName()
// 
// ---------------------------------------------------------
/*
void CRippleVaultAppUi::MapAPNtoActualName()
{
_LIT(smart_wap, "smart_wap");
_LIT(airtel_internet, "airtel_internet");
_LIT(singtel_internet, "singtel_internet");
_LIT(starhub_wap, "starhub_wap");
_LIT(starhub_wap_prepaid, "starhub_wap_prepaid");
_LIT(NO_HARD_CODE, "NOHARDCODE");
	
	if(iAccessPoints.Compare(smart_wap)==0){
		iAccessPoints.Copy(_L("SmartGPRS"));
	}
	else if(iAccessPoints.Compare(airtel_internet)==0){
		iAccessPoints.Copy(_L("Mobile Office"));
	}
	else if(iAccessPoints.Compare(singtel_internet)==0){
		iAccessPoints.Copy(_L("IDEAS Internet (GPRS)"));
	}
	else if(iAccessPoints.Compare(starhub_wap)==0){
		iAccessPoints.Copy(_L("Gee! GPRS"));
	}
	else if(iAccessPoints.Compare(starhub_wap_prepaid)==0){
		iAccessPoints.Copy(_L("Prepaid Gee!GPRS"));
	}
	else if(iAccessPoints.Compare(NO_HARD_CODE)==0){

	}
	else{
		iAccessPoints.Copy(_L(""));
	}
}
*/
// ---------------------------------------------------------
// CRippleVaultAppUi::ReturnPower(TInt a, TInt b)
// 
// ---------------------------------------------------------

TInt64 CRippleVaultAppUi::ReturnPower(TInt a, TInt b){
	TInt64 pow=1;
	for(TInt j=0;j<b;++j){
		pow=pow*a;
	}	
	return pow;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::RegisterDialog()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::RegisterDialog()
{		
#ifdef __UIQ__
	TInt err;
	err = 0;
	TBuf<50> tmpstr;
	TBuf<50> userId;
	tmpstr.Copy( _L("userid"));	
	LoadFromConfigFile(userId,tmpstr);

//	iNewUser = 1;
	iUserName.Copy(iUserNames);		  	
	CEikDialog* regdialog;	
	if(userId.Length()>0)
	{
		regdialog = new (ELeave) CAlreadyRegisterDialog(*this);
		regdialog->ExecuteLD(R_ALREADYREGISTER_DIALOG);					
	}
	else
	{
		CEikDialog* choicelistdialog = new(ELeave) CChoiceListControl(*this);		
		if(choicelistdialog->ExecuteLD(R_CHOICE_LIST_DIALOG))
		{
			err = 1;
		}
		else
			err = 0;

		if(err)
		{
			iMobileNo.Copy(_L(""));

			tmpstr.Copy(_L("hp"));	
			LoadFromConfigFile(iMobileNo,tmpstr );

			if(iMobileNo.Length() > 0 )
			{
				err = 1;
			}
			else
			{
				CEikDialog* mobregdialog;	
				mobregdialog = new (ELeave) CRegMobileDialog(*this);
				if(mobregdialog->ExecuteLD(R_MOBILE_DIALOG))
				{
					err = 1;
				}
				else
					err = 0;
			}
		}

		if(err)
		{
			if(iSettingsScreenValue == 1)
			{
				regdialog = new (ELeave) CRegisterDialog(*this);
				regdialog->ExecuteLD(R_REGISTER_DIALOG);	
			}
			else if(iSettingsScreenValue == 2)
			{
				regdialog = new (ELeave) CAlreadyRegisterDialog(*this);
				regdialog->ExecuteLD(R_REGISTER_DIALOG);		
			}
		}
	}
#else
	TBuf<20> password;
	TBuf<20> phone_number;
	TBool iRegistrationSuccess;
	TBool check;
	iRegistrationSuccess=EFalse;
	
	//BringToForeground();

	if(!iRegistrationSuccess)
	{   
		do
		{	
			TBuf<50> tmpstr;
			TBuf<50> userId;
			//TBuf<50> number
			tmpstr.Copy( _L("userid"));	
			LoadFromConfigFile(userId,tmpstr);

			if(userId.Length() > 0 )
			{
				iRegistrationSuccess = 2;
				phone_number.Copy(userId);
				//TInt number;
				//number = 0;					
				do
				{
					CAknMultiLineDataQueryDialog* dialog =	CAknMultiLineDataQueryDialog::NewL(phone_number, password);
					check=dialog->ExecuteLD(R_CONTACTS_DIALOG_ALREADY_REGISTERED);
					if(check)
					{
						//password.Copy(_L(""));
						//password.AppendNum(number);
						if(password.Length() >= 6)
						{							
							iRegistrationSuccess = 1;
							iUserNames.Copy(phone_number);
							iUserNames.LowerCase();
							iUserName.Copy(iUserNames);
							iPassword.Copy(password);
								
							ExistingUser();						
						}
					}
					else
						iRegistrationSuccess = 1;
				}
				while(iRegistrationSuccess == 2);
			}
			else
			{
				TInt index;
		
				CAknListQueryDialog* listquery = new (ELeave) CAknListQueryDialog(&index);
				if(listquery->ExecuteLD(R_SELECT_MAIN_OPTION))			
				{
				}
				
				iMobileNo.Copy(_L(""));

				tmpstr.Copy(_L("hp"));	
				LoadFromConfigFile(iMobileNo,tmpstr );

				if(iMobileNo.Length() > 0 )
				{
					iRegistrationSuccess = 2;
				}
				else
				{					
					CAknTextQueryDialog* phonedialog = CAknTextQueryDialog::NewL(iMobileNo);
					check = phonedialog->ExecuteLD(IMAGE_NAME);
					if(check)
					{
						iRegistrationSuccess = 2;
					}
					else
						iRegistrationSuccess = 1;
				}

				if(iRegistrationSuccess == 2)
				{
					phone_number.Copy(_L(""));
					//TInt number;
					//number=0;
					//CAknMultiLineDataQueryDialog* dialog =	CAknMultiLineDataQueryDialog::NewL(phone_number, password);
					do
					{
						CAknMultiLineDataQueryDialog* dialog =	CAknMultiLineDataQueryDialog::NewL(phone_number, password);
						check=dialog->ExecuteLD(R_CONTACTS_DIALOG_REGISTERED);
						if(check)
						{					
							//password.Copy(_L(""));
							//password.AppendNum(number);
							if(CheckSpecialChars(phone_number,password))
							{
								if(password.Length() >= 6)
								{
									iRegistrationSuccess = 1;
									iUserNames.Copy(phone_number);
									iUserNames.LowerCase();
									iUserName.Copy(iUserNames);
									iPassword.Copy(password);

									if(index == 0)
										NewUser();
									else if(index == 1)
										ExistingUser();
								}
							}
						}
						else
							iRegistrationSuccess = 1;
					}
					while(iRegistrationSuccess == 2);
				}
			}
		}
		while(!iRegistrationSuccess);		
	}
#endif
}

// ---------------------------------------------------------
// CRippleVaultAppUi::NewUser()
// Get New User XML
// ---------------------------------------------------------

/*
void CRippleVaultAppUi::NewUser()
{
	iRegBuffer.Copy(_L("<root>"));

	TBuf<30> tempbuf;
		
	tempbuf.Copy(_L("init_reg"));	
	iContactsHandler->iChangesDB->SetMessageType(tempbuf);
	
	tempbuf.Copy(_L("reg"));					
	CreateHeaderXML(tempbuf);
	
	iRegBuffer.Append(_L("<b>"));
	iRegBuffer.Append(_L("<hp>"));
	iRegBuffer.Append(iMobileNo);
	iRegBuffer.Append(_L("</hp>"));
	iRegBuffer.Append(_L("<username>"));
	iRegBuffer.Append(iUserNames);
	iRegBuffer.Append(_L("</username>"));
	//iRegBuffer.Append(_L("</registration>"));
	iRegBuffer.Append(_L("</b>"));
	iRegBuffer.Append(_L("</root>"));

	HBufC8* content_data;
	content_data=iRegBuffer.AllocL();

	TBuf8<10> content_l;
	content_l.Num(content_data->Length());
	//iRegBuffer.Copy(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	iRegBuffer.Copy(_L("GET "));
	iRegBuffer.Append(iRegUrl);
	iRegBuffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));

	iRegBuffer.Append(content_l);
	iRegBuffer.Append(_L("\r\n\r\n"));
	iRegBuffer.Append(*content_data);
	
	servlet_data = iRegBuffer.AllocL();
	ConnectToServer();
}
*/

void CRippleVaultAppUi::NewUser()
{
	iRegBuffer.Copy(_L("<root>"));
	TBuf<30> tempbuf;
	
	tempbuf.Copy(_L("init_reg"));	
	iContactsHandler->iChangesDB->SetMessageType(tempbuf);

	//tempbuf.Copy(_L("eu"));
	tempbuf.Copy(_L("reg"));
	CreateHeaderXML(tempbuf);
	
	iRegBuffer.Append(_L("<b>"));
	iRegBuffer.Append(_L("<hp>"));
	iRegBuffer.Append(iMobileNo);
	iRegBuffer.Append(_L("</hp>"));
	iRegBuffer.Append(_L("<username>"));
	iRegBuffer.Append(iUserNames);
	iRegBuffer.Append(_L("</username>"));
	iRegBuffer.Append(_L("<password>"));
	iRegBuffer.Append(iPassword);
	iRegBuffer.Append(_L("</password>"));
	iRegBuffer.Append(_L("<phone_model>"));
	iRegBuffer.Append(iAxiomPhoneModel);
	iRegBuffer.Append(_L("</phone_model>"));
	iRegBuffer.Append(_L("<billing_reg_type>"));
	iRegBuffer.Append(iAxiomBillingRegType);
	iRegBuffer.Append(_L("</billing_reg_type>"));
	iRegBuffer.Append(_L("</b>"));
	iRegBuffer.Append(_L("</root>"));

#ifndef WAP_PT
	HBufC8* content_data;
	content_data=iRegBuffer.AllocL();

	TBuf8<10> content_l;
	content_l.Num(content_data->Length());
	//iRegBuffer.Copy(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	iRegBuffer.Copy(_L("GET "));
	iRegBuffer.Append(iRegUrl);
	//iRegBuffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	iRegBuffer.Append(_L(" HTTP/1.0\r\nHOST:"));
	iRegBuffer.Append(iServerAddr);
	iRegBuffer.Append(_L("\r\nPORT:"));
	iRegBuffer.AppendNum(iPort);
	iRegBuffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));


	iRegBuffer.Append(content_l);
	iRegBuffer.Append(_L("\r\n\r\n"));
	iRegBuffer.Append(*content_data);
#endif

	servlet_data = iRegBuffer.AllocL();
	ConnectToServer();
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ExistingUser()
// Get existing user XML
// ---------------------------------------------------------

void CRippleVaultAppUi::ExistingUser()
{
	iRegBuffer.Copy(_L("<root>"));
	TBuf<30> tempbuf;
	
	tempbuf.Copy(_L("init_eu"));
	//tempbuf.Copy(_L("init_reg"));	
	iContactsHandler->iChangesDB->SetMessageType(tempbuf);				
	
	tempbuf.Copy(_L("eu"));				
	CreateHeaderXML(tempbuf);
	
	iRegBuffer.Append(_L("<b>"));
	iRegBuffer.Append(_L("<hp>"));
	iRegBuffer.Append(iMobileNo);
	iRegBuffer.Append(_L("</hp>"));
	iRegBuffer.Append(_L("<username>"));
	iRegBuffer.Append(iUserNames);
	iRegBuffer.Append(_L("</username>"));
	//iRegBuffer.Append(_L("<oldpassword>"));
	//iRegBuffer.Append(iPassword);
	//iRegBuffer.Append(_L("</oldpassword>"));
	iRegBuffer.Append(_L("<password>"));
	iRegBuffer.Append(iPassword);
	iRegBuffer.Append(_L("</password>"));
	iRegBuffer.Append(_L("<phone_model>"));
	iRegBuffer.Append(iAxiomPhoneModel);
	iRegBuffer.Append(_L("</phone_model>"));
	iRegBuffer.Append(_L("<billing_reg_type>"));
	iRegBuffer.Append(iAxiomBillingRegType);
	iRegBuffer.Append(_L("</billing_reg_type>"));
	iRegBuffer.Append(_L("</b>"));
	iRegBuffer.Append(_L("</root>"));

#ifndef WAP_PT
	HBufC8* content_data;
	content_data=iRegBuffer.AllocL();

	TBuf8<10> content_l;
	content_l.Num(content_data->Length());
	//iRegBuffer.Copy(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	iRegBuffer.Copy(_L("GET "));
	iRegBuffer.Append(iRegUrl);
	//iRegBuffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	iRegBuffer.Append(_L(" HTTP/1.0\r\nHOST:"));
	iRegBuffer.Append(iServerAddr);
	iRegBuffer.Append(_L("\r\nPORT:"));
	iRegBuffer.AppendNum(iPort);
	iRegBuffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));


	iRegBuffer.Append(content_l);
	iRegBuffer.Append(_L("\r\n\r\n"));
	iRegBuffer.Append(*content_data);
#endif
	servlet_data = iRegBuffer.AllocL();
	ConnectToServer();

}

// ---------------------------------------------------------
// CRippleVaultAppUi::ChangePassword()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::ChangePassword()
{
	iRegBuffer.Copy(_L("<root>"));
	TBuf<30> tempbuf;
	
	//tempbuf.Copy(_L("init_eu"));
	//tempbuf.Copy(_L("init_reg"));	
	//iContactsHandler->iChangesDB->SetMessageType(tempbuf);				
	
	tempbuf.Copy(_L("change_pw"));				
	CreateHeaderXML(tempbuf);
	
	iRegBuffer.Append(_L("<b>"));
	iRegBuffer.Append(_L("<hp>"));
	iRegBuffer.Append(iMobileNo);
	iRegBuffer.Append(_L("</hp>"));
	iRegBuffer.Append(_L("<username>"));
	iRegBuffer.Append(iUserNames);
	iRegBuffer.Append(_L("</username>"));
	iRegBuffer.Append(_L("<newpassword>"));
	iRegBuffer.Append(iNewPassword);
	iRegBuffer.Append(_L("</newpassword>"));
	iRegBuffer.Append(_L("<password>"));
	iRegBuffer.Append(iPassword);
	iRegBuffer.Append(_L("</password>"));
	iRegBuffer.Append(_L("</b>"));
	iRegBuffer.Append(_L("</root>"));

	HBufC8* content_data;
	content_data=iRegBuffer.AllocL();

	TBuf8<10> content_l;
	content_l.Num(content_data->Length());
	//iRegBuffer.Copy(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	iRegBuffer.Copy(_L("GET "));
	iRegBuffer.Append(iRegUrl);
	//iRegBuffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	iRegBuffer.Append(_L(" HTTP/1.0\r\nHOST:"));
	iRegBuffer.Append(iServerAddr);
	iRegBuffer.Append(_L("\r\nPORT:"));
	iRegBuffer.AppendNum(iPort);
	iRegBuffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));


	iRegBuffer.Append(content_l);
	iRegBuffer.Append(_L("\r\n\r\n"));
	iRegBuffer.Append(*content_data);

	servlet_data = iRegBuffer.AllocL();
	ConnectToServerForChangingPassword();
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CreateHeaderXML(TDesC& msgType)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::CreateHeaderXML(TDesC& msgType)
{
	HBufC* currTime;
	
	TTime ctime;

	ctime.HomeTime();
	currTime = GetTimeStamp(ctime);
	
	iRegBuffer.Append(_L("<h>"));
	iRegBuffer.Append(_L("<msgt>"));
	iRegBuffer.Append(msgType);
	iRegBuffer.Append(_L("</msgt>"));

	iRegBuffer.Append(_L("<msg></msg>"));

	iRegBuffer.Append(_L("<uid>"));
	iRegBuffer.Append(iUserNames);
	iRegBuffer.Append(_L("</uid>"));

	iRegBuffer.Append(_L("<hp>"));
	iRegBuffer.Append(iMobileNo);
	iRegBuffer.Append(_L("</hp>"));

	iRegBuffer.Append(_L("<did>"));
	iRegBuffer.Append(PhoneImei());
	iRegBuffer.Append(_L("</did>"));

	iRegBuffer.Append(_L("<currec>"));
	iRegBuffer.AppendNum(1);
	iRegBuffer.Append(_L("</currec>"));

	iRegBuffer.Append(_L("<totrec>"));
	iRegBuffer.AppendNum(1);
	iRegBuffer.Append(_L("</totrec>"));

	iRegBuffer.Append(_L("<loc>en_us</loc>"));
	
	iRegBuffer.Append(_L("<ctime>"));
	iRegBuffer.Append(*currTime);
	iRegBuffer.Append(_L("</ctime>"));

	iRegBuffer.Append(_L("<lstime>"));
	iRegBuffer.Append(_L("0"));
	iRegBuffer.Append(_L("</lstime>"));

	iRegBuffer.Append(_L("<synctype><mod>cs</mod><gran>contactlevel</gran>\
<adde>true</adde><upde>true</upde><dele>true</dele></synctype>"));

	iRegBuffer.Append(_L("</h>"));
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetTimeStamp(TTime& aTime)
// 
// ---------------------------------------------------------

HBufC* CRippleVaultAppUi::GetTimeStamp(TTime& aTime)
{
	TBuf<256> timestamp;

	TDateTime janNineteenSeventy(1970,EJanuary,0,00,00,00,000000);

	TTime timejan(janNineteenSeventy);

	TTimeIntervalMicroSeconds interval = aTime.MicroSecondsFrom(timejan);
	TInt64 k;
	k=interval.Int64()/1000;
	timestamp.Num(k);

	HBufC* ts = NULL;
	ts = timestamp.AllocLC();
	CleanupStack::Pop();

	return (ts);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ChargePublish(TInt publishtype)
// 
// ---------------------------------------------------------

//void CRippleVaultAppUi::ChargePublish(TInt publishtype)
//{
//	TBuf8<300> chargebuf;
//	
//	chargebuf.Copy(_L(""));
//
//	HBufC8* content_data;
//	content_data=chargebuf.AllocL();
//
//	TBuf8<10> content_l;
//	content_l.Num(content_data->Length());
//
//	if(publishtype == 0)		
//	{
//		//chargebuf.Copy(_L("GET /rpc/chargerp/300/uploadphoto HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
//		chargebuf.Copy(_L("GET "));
//		chargebuf.Append(iChargePhoto);
//		chargebuf.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
//	}
//	else if(publishtype == 1)
//	{
//		//chargebuf.Copy(_L("GET /rpc/chargerp/300/uploadvideo HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));	
//		chargebuf.Copy(_L("GET "));
//		chargebuf.Append(iChargeVideo);
//		chargebuf.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
//	}
//
//	chargebuf.Append(content_l);
//	chargebuf.Append(_L("\r\n\r\n"));
//	chargebuf.Append(*content_data);
//	
//	servlet_data = chargebuf.AllocL();
//	ConnectToServerForCharging();
//}
// ---------------------------------------------------------
// CRippleVaultAppUi::RegChargePublish()
// 
// ---------------------------------------------------------

//
//void CRippleVaultAppUi::RegChargePublish()
//{
//	TBuf8<300> chargebuf;
//	
//	chargebuf.Copy(_L(""));
//
//	HBufC8* content_data;
//	content_data=chargebuf.AllocL();
//
//	TBuf8<10> content_l;
//	content_l.Num(content_data->Length());
//
//	//chargebuf.Copy(_L("GET /rpc/chargerpc/300/downloadclient HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
//	chargebuf.Copy(_L("GET "));
//	chargebuf.Append(iChargeClient);
//	chargebuf.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
//
//	chargebuf.Append(content_l);
//	chargebuf.Append(_L("\r\n\r\n"));
//	chargebuf.Append(*content_data);
//	
//	servlet_data = chargebuf.AllocL();
//	ConnectToServerForCharging();
//}
//
//void CRippleVaultAppUi::ChargePublishComplete()
//{
//	delete servlet_data;
//	MoreFilesToUpload();
//}

// ---------------------------------------------------------
// CRippleVaultAppUi::AutoRegisterRipple()
// automatically register without prompt if userid/password known
// ---------------------------------------------------------

void CRippleVaultAppUi::AutoRegisterRipple()
{
/*    TBuf<128> KRegStep1;
    KRegStep1 = CRippleVaultAppUi::ApplicationDriveAndPath();
	KRegStep1.Append(_L("reg_step1.txt"));

	RFs fs;
	
	fs.Connect();
	
	if (BaflUtils::FileExists(fs, KRegStep1)) 
	{	
		GetAlbumSynchingXML();
	}
	else
	{	
		TInt setaccess=0;
		if(iAccessPoints.Compare(_L("NOHARDCODE"))==0)
		{
			//SelectAccess();
			setaccess = 10;			
		}
		else
		{
			setaccess = SetAccesspoint();
		}	
		if(setaccess==10)
		{	
			TInt Reg;
			TBuf<100> Temp;

			iUserName.Copy(iUserNames);

			Reg = CheckRegFile();
			if(Reg == -1)
			{
				Reg=0;*/
				TBuf<50> tmpstr;
				TBuf<50> userId;
				TBuf<50> mobileNo;
				TBuf<50> passWord;
				
				tmpstr.Copy( _L("userid"));	
				LoadFromConfigFile(userId,tmpstr);

				tmpstr.Copy( _L("hp"));
				LoadFromConfigFile(mobileNo,tmpstr);

				tmpstr.Copy(_L("password"));
				LoadFromConfigFile(passWord,tmpstr);

				if(userId.Length() > 0 && mobileNo.Length() > 0  && passWord.Length() > 0)
				{
					ExistingUser();
				}
				else if(userId.Length() > 0 && mobileNo.Length() > 0)
				{					
					//NewUser();
					RegisterDialog();
				}
				else
				{
					//NewUser();
					RegisterDialog();
				}
			/*}
			else
			{
				//Temp.Copy(_L("User Name:"));
				HBufC*  message;
				message = iEikonEnv->AllocReadResourceL(R_EXAMPLE_USER_NAME);				
				Temp.Copy(message->Des());
				delete message;
				
				Temp.Append(iUserName);
				CEikonEnv::Static ()->AlertWin(Temp);

				WritRegisteredFile();
				UserRegistered();
				EnableUpload(REGISTERED);
			}
		}
		else
		{			
			//display message
			TBuf<200> msg;
	//		msg.Copy(_L("Please configure your phone for \""));
			msg.Copy(_L("Please configure your phone for SmartGPRS settings. Send \"SET GPRS n6600\" to 211, if you have a Nokia 6600, to configure SmartGPRS."));
	//		msg.Append(iAccessPoints);
	//		msg.Append(_L("\" settings!"));
	#ifdef __UIQ__
			CEikonEnv::Static()->InfoWinL(_L("Ripple Vault"),msg);		
	#else
			CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(msg);
			abtdlg->PrepareLC(R_SETACCESSPOINT_ERROR);

			if (abtdlg->RunLD()) 
			{					
			}				
	#endif		
		}	
	}	
	fs.Close();*/
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ChooseAccessPoint()
// Choose Access Point, either manually or automatically
// ---------------------------------------------------------

TInt CRippleVaultAppUi::ChooseAccessPoint() // if NOHARDCODE then manual selection, otherwise automatic
{
	KillBrowser(); // otherwise browser has locked the internet apn and our app will be unable to connect

	TInt setaccess=0;
    TBuf<128> KRegStep1;
    KRegStep1 = CRippleVaultAppUi::ApplicationDriveAndPath();
	KRegStep1.Append(KRegStep1File);

	RFs fs;
	
	fs.Connect();
	
	if (0)//BaflUtils::FileExists(fs, KRegStep1)) 
	{	
		GetAlbumSynchingXML();
	}
	else
	{			
		if(iAccessPoints.Find(_L("NOHARDCODE"))>=0)
		{
			DefaultAccessPoint();
			//TRAPD(err,SelectAccess());
			iApnCount = 0;
			//SelectAccess();
			setaccess = 10;			
		}
		else if(iAccessPoints.Compare(_L(""))==0)
		{
			setaccess = 0;
		}
		else
		{
			setaccess = SetAccesspoint();
		}	
		if(setaccess==10)
		{	
		}
		else
		{			
			//display message
			//TBuf<200> msg;	
			//msg.Copy(_L("Please configure your phone for InternetGPRS settings."));	
	#ifdef __UIQ__
			//CEikonEnv::Static()->InfoWinL(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),msg);
			ShowMessageBoxFromResource(R_INTERNET_APN_ERROR);
	#else
			CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_INTERNET_APN_ERROR)));
			abtdlg->PrepareLC(R_SETACCESSPOINT_ERROR);

			if (abtdlg->RunLD()) 
			{					
			}				
	#endif		
		}	
	}	
	fs.Close();

	//GetConnectionName();

	return setaccess;
}

TInt CRippleVaultAppUi::AccessPointSettings() // if NOHARDCODE then manual selection, otherwise automatic
{

	TInt setaccess=0;
	
		if(iAccessPoints.Find(_L("NOHARDCODE"))>=0)
		{
			//TRAPD(err,SelectAccess());
			SelectAccessSettings();
			GetAccessPointID();
			SaveApnID(iApnID);
			setaccess = 10;			
		}
		else if(iAccessPoints.Compare(_L(""))==0)
		{
			setaccess = 0;
		}
		else
		{
			setaccess = SetAccesspoint();
		}	
		if(setaccess==10)
		{	
		}
		else
		{			
			//display message
			//TBuf<200> msg;	
			//msg.Copy(_L("Please configure your phone for InternetGPRS settings."));	
	#ifdef __UIQ__
			//CEikonEnv::Static()->InfoWinL(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),msg);
			ShowMessageBoxFromResource(R_INTERNET_APN_ERROR);
	#else
			CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_INTERNET_APN_ERROR)));
			abtdlg->PrepareLC(R_SETACCESSPOINT_ERROR);

			if (abtdlg->RunLD()) 
			{					
			}				
	#endif		
		}	
	return setaccess;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CheckNetwork()
// Check Network status to check if on Home or Roaming Network
// ---------------------------------------------------------

TInt CRippleVaultAppUi::CheckNetwork() // for roaming
{
	TInt regstatus;
#ifdef __S80__	
	CCheckNetwork* cn = CCheckNetwork::NewL();
	cn->GetCheckNetwork();

	do
	{
	} while (!cn->IsRetrieved());

	regstatus = cn->iRegStatus;

	delete cn;
	cn = NULL;
#else
	TBuf<255> tsyName;

	// Read the TSY name from CommsDB
	CCommsDatabase* db = CCommsDatabase::NewL(EDatabaseTypeUnspecified);
	CleanupStack::PushL(db);
	CCommsDbTableView* table = db->OpenTableLC(TPtrC(MODEM));
	table->GotoFirstRecord();
	table->ReadTextL(TPtrC(MODEM_TSY_NAME),tsyName);
	// Cleanup - CommsDB no longer needed
	CleanupStack::PopAndDestroy(2); // table,db

	if (!tsyName.Length())
    {
		User::Leave(KErrNotFound);
	}

	// Connect to the ETel server
	RTelServer telServer;
	User::LeaveIfError(telServer.Connect());
	CleanupClosePushL(telServer);

#ifdef __UIQ__  //For UIQ the TSY filename returned by the above method is wrong. So the file has been hardcoded for Sony Ericsson phones.
	tsyName.Copy(_L("erigsm.tsy"));
#endif

	// Make sure our TSY is loaded
	User::LeaveIfError(telServer.LoadPhoneModule(tsyName));

	// Get the number of registered phones
	TInt numberOfPhones;
	User::LeaveIfError(telServer.EnumeratePhones(numberOfPhones));

	TInt aPhoneId=0;
	// Get the phone name
	RTelServer::TPhoneInfo phoneInfo;
	User::LeaveIfError(telServer.GetPhoneInfo(aPhoneId,phoneInfo));

	// Open the phone by name
	RBasicGsmPhone phone;
	User::LeaveIfError(phone.Open(telServer,phoneInfo.iName));
	CleanupClosePushL(phone);

	// Get Phone Info
	RBasicGsmPhone::TRegistrationStatus aRegStatus;
	User::LeaveIfError(phone.GetNetworkRegistrationStatus(aRegStatus));

	// Close the Phone handle, no longer needed
	CleanupStack::PopAndDestroy(); // phone

	regstatus = aRegStatus;

	// Cleanup
	telServer.UnloadPhoneModule(tsyName);
	CleanupStack::PopAndDestroy(); // telServer
#endif

	return regstatus;

}

// ---------------------------------------------------------
// CRippleVaultAppUi::KillBrowser()
// Kill Browser if active
// ---------------------------------------------------------

void CRippleVaultAppUi::KillBrowser()
{
#ifndef __UIQ__				
	// Wap Browser's constants UId	
	const TUid KWmlBrowserUid = { 0x10008D39 }; 
	//const TUid KSMSMessageUid = { 0x100058C5 };

	RWsSession ws;
	User::LeaveIfError( ws.Connect() );
	TApaTaskList taskList( ws );
		
	/*TApaTask task = taskList.FindApp(KSMSMessageUid); // Position Last Task
	if(task.Exists())
	{
		task.KillTask();
	} */ 

	TApaTask task1 = taskList.FindApp(KWmlBrowserUid); // Position Last Task
	if(task1.Exists())
	{		
		task1.KillTask();
	}  
	ws.Close(); 

#else
/*	const TInt KWmlBrowserUid = { 0x100066DE };
	//const TUid KSMSMessageUid = { 0x100058C5 };

	RWsSession ws;
	User::LeaveIfError( ws.Connect() );
	TApaTaskList taskList( ws );

	//TApaTask task = taskList.FindApp(KSMSMessageUid); // Position Last Task
	//if(task.Exists())
	//{
		//task.KillTask();
	//}  

	
	TApaTask task1 = taskList.FindApp(KWmlBrowserUid); // Position Last Task
	if(task1.Exists())
	{		
		task1.KillTask();
	}  
	ws.Close(); 
*/
#endif
}

// ---------------------------------------------------------
// CRippleVaultAppUi::OpenBrowser()
// Open WAP browser seeded with a URL
// ---------------------------------------------------------

void CRippleVaultAppUi::OpenBrowser()
{
	HBufC* param = HBufC::NewLC( 120 ); 
	
#ifdef __UIQ__	
	param->Des().Copy(_L("http://")); 
	param->Des().Append(iServerAddr); 
	param->Des().Append(_L(":"));
	param->Des().AppendNum((TInt)iPort); 	
	param->Des().Append(_L("/"));
	param->Des().Append(iWapUrl);
	param->Des().Append(_L("?userId="));
	param->Des().Append(iUserNames);
	
	/*param->Des().Append(_L("?msisdn="));
	param->Des().Append(iDecodedMobileNo); 

	param->Des().Copy(_L("")); 
	param->Des().Append(iWapUrl);
	param->Des().Append(_L("?userId="));
	param->Des().Append(iUserNames);*/

#ifdef __S80__
	TFileName AppToLaunch;
	
	AppToLaunch = _L("z:\\System\\Apps\\opera\\opera.app");
	
	CApaCommandLine* cmdLine = CApaCommandLine::NewLC();
	cmdLine->SetLibraryNameL(AppToLaunch);

	cmdLine->SetDocumentNameL(param->Des());
	
	cmdLine->SetCommandL(EApaCommandOpen);
	RApaLsSession ls;
	User::LeaveIfError(ls.Connect());

	TInt handle;
	TInt initialAppCount;

	handle = ls.Handle();
	ls.EmbeddableAppCount( initialAppCount );
	User::LeaveIfError(ls.StartApp(*cmdLine));
	CleanupStack::PopAndDestroy(1);
#else
	TUid KWebAppUid = { 0x100066de };

	RApaLsSession appArcSession; 
	appArcSession.Connect(); // connect to AppArc server 
	TApaAppInfo aInfo; 
	CApaCommandLine* aCommandLine = CApaCommandLine::NewL(); 
    
	appArcSession.GetAppInfo(aInfo, KWebAppUid); 
    
	aCommandLine->SetLibraryNameL(aInfo.iFullName); 
	aCommandLine->SetDocumentNameL(param->Des()); 
	aCommandLine->SetCommandL(EApaCommandCreate); 

	appArcSession.StartApp(*aCommandLine);

	SendToBackground();
#endif
#else			
	//param->Des().Copy( _L( "4 http://wap.yahoo.com/" ) ); 	
	param->Des().Copy(_L("4 http://")); 
	param->Des().Append(iServerAddr); 
	param->Des().Append(_L(":"));
	param->Des().AppendNum((TInt)iPort); 
	param->Des().Append(_L("/"));
	param->Des().Append(iWapUrl);
	param->Des().Append(_L("?userId="));
	param->Des().Append(iUserNames);

	/*param->Des().Copy(_L("4 ")); 
	param->Des().Append(iWapUrl);
	param->Des().Append(_L("?userId="));
	param->Des().Append(iUserNames);*/

	/*TBuf<200> msg;
	msg.Copy(param->Des());
	CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(msg);
	abtdlg->PrepareLC(R_SETACCESSPOINT_ERROR);
	if (abtdlg->RunLD()) 
	{					
	}*/

	// Wap Browser's constants UId	
	const TInt KWmlBrowserUid = 0x10008D39; 

	TUid id( TUid::Uid( KWmlBrowserUid ) ); 
	
	TApaTaskList taskList( CEikonEnv::Static()->WsSession() );
	TApaTask task = taskList.FindApp( id );
	
	if ( task.Exists() )
	{
		HBufC8* param8 = HBufC8::NewLC( param->Length() ); 
		param8->Des().Append( *param ); 
		task.SendMessage( TUid::Uid( 0 ), *param8 ); 		// Uid is not used 
		CleanupStack::PopAndDestroy(); 
	}
	else 
	{ 
		RApaLsSession appArcSession; 
		User::LeaveIfError(appArcSession.Connect()); // connect to AppArc server 
		TThreadId id; 
		appArcSession.StartDocument( *param, TUid::Uid( KWmlBrowserUid ), id ); 
		appArcSession.Close(); 
	} 
#endif
	CleanupStack::PopAndDestroy(); // param
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SaveMails(TDesC& aAlbumname, TDesC& aNames, TDesC& aMails,TDesC& aMess, TInt8 aCount)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::SaveMails(TDesC& aAlbumname, TDesC& aNames, TDesC& aMails,TDesC& aMess, TInt8 aCount)
{
    iPhotosDB->SaveMailsToDB(aAlbumname,aNames, aMails, aMess, aCount);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetMails(TDesC& aAlbumname, TDesC& aNames, TDesC& aMails, TDesC& aMess, TInt8& aCount)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetMails(TDesC& aAlbumname, TDesC& aNames, TDesC& aMails, TDesC& aMess, TInt8& aCount)
{   
    iPhotosDB->GetMailsFromDB(aAlbumname,aNames, aMails, aMess, aCount);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetUnSharingAlbumXML()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetUnSharingAlbumXML()//TDes8& RBuffer)
{
	TBuf8<1500> RBuffer;
	CnvUtfConverter converter;
	TBuf8<150> utf8buf;
	
	RBuffer.Copy(_L("<root><h><msgt>"));
	RBuffer.Append(_L("unsharealbum"));
	RBuffer.Append(_L("</msgt>"));

	RBuffer.Append(_L("<msg></msg>"));
	
	RBuffer.Append(_L("<uid>"));
	RBuffer.Append(iUserNames);
	RBuffer.Append(_L("</uid>"));
	
	RBuffer.Append(_L("<hp>"));
	RBuffer.Append(iMobileNo);
	RBuffer.Append(_L("</hp>"));
	
	RBuffer.Append(_L("<did>"));
	RBuffer.Append(PhoneImei());
	RBuffer.Append(_L("</did>"));

	RBuffer.Append(_L("<currec></currec><totrec></totrec><loc>en_us</loc><ctime></ctime><lstime>0</lstime><synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype>"));

	RBuffer.Append(_L("</h><b>"));

	RBuffer.Append(_L("<albumid>"));
	converter.ConvertFromUnicodeToUtf8(utf8buf,albumName);//Album Name
	RBuffer.Append(utf8buf);
	RBuffer.Append(_L("</albumid>"));	
	
	RBuffer.Append(_L("</b></root>"));

#ifndef WAP_PT
	HBufC8* content_data;
	content_data=RBuffer.AllocL();

	TBuf8<10> content_l;
	content_l.Num(content_data->Length());
	RBuffer.Copy(_L("GET "));
	RBuffer.Append(iSharingUrl);
	//RBuffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));

	RBuffer.Append(_L(" HTTP/1.0\r\nHOST:"));
	RBuffer.Append(iServerAddr);
	RBuffer.Append(_L("\r\nPORT:"));
	RBuffer.AppendNum(iPort);
	RBuffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));

	RBuffer.Append(content_l);
	RBuffer.Append(_L("\r\n\r\n"));
	RBuffer.Append(*content_data);
#endif

	servlet_data = RBuffer.AllocL();
	ConnectToServerForUnSharing();
	//WriteLogFile(RBuffer);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSharingAlbumXML()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetSharingAlbumXML()//TDes8& RBuffer)
{
	TBuf8<1500> RBuffer;
	TBuf<150> sharingName;
	TBuf<150> mess;
	TInt8 sharingCount;
	CnvUtfConverter converter;
	TBuf8<150> utf8buf;

	GetMails(albumName,MailNames,MailId, mess, sharingCount);

	RBuffer.Copy(_L("<root><h><msgt>"));
	RBuffer.Append(_L("sharealbum"));
	RBuffer.Append(_L("</msgt>"));

	RBuffer.Append(_L("<msg></msg>"));
	
	RBuffer.Append(_L("<uid>"));
	RBuffer.Append(iUserNames);
	RBuffer.Append(_L("</uid>"));
	
	RBuffer.Append(_L("<hp>"));
	RBuffer.Append(iMobileNo);
	RBuffer.Append(_L("</hp>"));
	
	RBuffer.Append(_L("<did>"));
	RBuffer.Append(PhoneImei());
	RBuffer.Append(_L("</did>"));

	RBuffer.Append(_L("<currec></currec><totrec></totrec><loc>en_us</loc><ctime></ctime><lstime>0</lstime><synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype>"));

	RBuffer.Append(_L("</h><b>"));

	RBuffer.Append(_L("<share>"));

	RBuffer.Append(_L("<albumid>"));
	converter.ConvertFromUnicodeToUtf8(utf8buf,albumName);
	RBuffer.Append(utf8buf);//Album Name
	RBuffer.Append(_L("</albumid>"));
	
	RBuffer.Append(_L("<message>"));
	converter.ConvertFromUnicodeToUtf8(utf8buf,mess);
	RBuffer.Append(utf8buf);	
	RBuffer.Append(_L("</message>"));
	
	RBuffer.Append(_L("<recipients>"));
	
	//while(MailId.Length() > 0 || MailNames.Length() > 0)		
	for(int i=0; i< sharingCount ;i++)
	{
		RBuffer.Append(_L("<object>"));

		RBuffer.Append(_L("<name>"));
		
		GetSharingNames(sharingName);		
		converter.ConvertFromUnicodeToUtf8(utf8buf,sharingName);
		RBuffer.Append(utf8buf);//Album Name
		
		RBuffer.Append(_L("</name>"));

		RBuffer.Append(_L("<mobile>"));
		
		GetSharingMails(sharingName);		
		RBuffer.Append(sharingName);//Friend Id's
		
		RBuffer.Append(_L("</mobile>"));

		/*RBuffer.Append(_L("<email>"));
		
		GetSharingNames(sharingName);		
		RBuffer.Append(sharingName);//Friend Id's
		
		RBuffer.Append(_L("</email>"));*/

		RBuffer.Append(_L("</object>"));
	}
	
	RBuffer.Append(_L("</recipients>"));
	RBuffer.Append(_L("</share>"));
	RBuffer.Append(_L("</b></root>"));

#ifndef WAP_PT
	HBufC8* content_data;
	content_data=RBuffer.AllocL();

	TBuf8<10> content_l;
	content_l.Num(content_data->Length());
	//RBuffer.Copy(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	RBuffer.Copy(_L("GET "));
	RBuffer.Append(iSharingUrl);
	//RBuffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	RBuffer.Append(_L(" HTTP/1.0\r\nHOST:"));
	RBuffer.Append(iServerAddr);
	RBuffer.Append(_L("\r\nPORT:"));
	RBuffer.AppendNum(iPort);
	RBuffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));


	RBuffer.Append(content_l);
	RBuffer.Append(_L("\r\n\r\n"));
	RBuffer.Append(*content_data);
#endif

	servlet_data = RBuffer.AllocL();
	ConnectToServerForSharing();

}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetClientSharingAlbumXML()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetClientSharingAlbumXML()//TDes8& RBuffer)
{
	TBuf8<1500> RBuffer;
	TBuf<150> sharingName;
	TBuf<150> mess;
	TInt8 sharingCount;
	CnvUtfConverter converter;
	TBuf8<150> utf8buf;

	GetMails(albumName,MailNames,MailId, mess, sharingCount);

	/*RBuffer.Copy(_L("<imei>"));
	RBuffer.Append(PhoneImei());
	RBuffer.Append(_L("</imei>"));

	RBuffer.Append(_L("<share>"));
	
	RBuffer.Append(_L("<uid>"));
	RBuffer.Append(iUserNames);
	RBuffer.Append(_L("</uid>"));
	
	RBuffer.Append(_L("<albumid>"));
	RBuffer.Append(_L("PURPLEACESMARTRIPPLEPUBLISHCLIENT"));
	RBuffer.Append(_L("</albumid>"));

	RBuffer.Append(_L("<recipients>"));*/

	RBuffer.Copy(_L("<root><h><msgt>"));
	RBuffer.Append(_L("shareapplication"));
	RBuffer.Append(_L("</msgt>"));

	RBuffer.Append(_L("<msg></msg>"));
	
	RBuffer.Append(_L("<uid>"));
	RBuffer.Append(iUserNames);
	RBuffer.Append(_L("</uid>"));
	
	RBuffer.Append(_L("<hp>"));
	RBuffer.Append(iMobileNo);
	RBuffer.Append(_L("</hp>"));
	
	RBuffer.Append(_L("<did>"));
	RBuffer.Append(PhoneImei());
	RBuffer.Append(_L("</did>"));

	RBuffer.Append(_L("<currec></currec><totrec></totrec><loc>en_us</loc><ctime></ctime><lstime>0</lstime><synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype>"));

	RBuffer.Append(_L("</h><b>"));

	RBuffer.Append(_L("<share>"));
	
	RBuffer.Append(_L("<recipients>"));
	
	for(int i=0; i< sharingCount ;i++)
	{
		RBuffer.Append(_L("<object>"));

		RBuffer.Append(_L("<name>"));
		
		GetSharingNames(sharingName);		
		converter.ConvertFromUnicodeToUtf8(utf8buf,sharingName);
		RBuffer.Append(utf8buf);//Album Name

		RBuffer.Append(_L("</name>"));

		RBuffer.Append(_L("<mobile>"));
		
		GetSharingMails(sharingName);		
		RBuffer.Append(sharingName);//Friend Id's
		
		RBuffer.Append(_L("</mobile>"));

		/*RBuffer.Append(_L("<email>"));
		
		GetSharingNames(sharingName);		
		RBuffer.Append(sharingName);//Friend Id's
		
		RBuffer.Append(_L("</email>"));*/

		RBuffer.Append(_L("</object>"));
	}
	
	RBuffer.Append(_L("</recipients>"));
	RBuffer.Append(_L("</share>"));
	RBuffer.Append(_L("</b></root>"));

#ifndef WAP_PT
	HBufC8* content_data;
	content_data=RBuffer.AllocL();

	TBuf8<10> content_l;
	content_l.Num(content_data->Length());
	//RBuffer.Copy(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	RBuffer.Copy(_L("GET "));
	RBuffer.Append(iSharingUrl);
	//RBuffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	RBuffer.Append(_L(" HTTP/1.0\r\nHOST:"));
	RBuffer.Append(iServerAddr);
	RBuffer.Append(_L("\r\nPORT:"));
	RBuffer.AppendNum(iPort);
	RBuffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));


	RBuffer.Append(content_l);
	RBuffer.Append(_L("\r\n\r\n"));
	RBuffer.Append(*content_data);
#endif

	servlet_data = RBuffer.AllocL();
	albumName.Copy(_L(""));
	ConnectToServerForSharing();

}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetAlbumSynchingXML()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetAlbumSynchingXML()//TDes8& RBuffer)
{
	//CloseSocketPublish();

	if(!RegFileExists())
	{
		TBuf<128> KRegStep1;
		KRegStep1 = CRippleVaultAppUi::ApplicationDriveAndPath();
	KRegStep1.Append(KRegStep1File);

		RFs fs;
		RFile file;

		fs.Connect();
		
		TInt err = file.Replace(fs, KRegStep1, EFileStreamText|EFileWrite|EFileShareAny);

		if (err == KErrNone) 
		{
			file.Close();
		}		

		fs.Close();		
	}
	
	TBuf8<1024> RBuffer;
	
	RBuffer.Copy(_L("<root><h><msgt>"));
	RBuffer.Append(_L("albumsynch"));
	RBuffer.Append(_L("</msgt>"));

	RBuffer.Append(_L("<msg></msg>"));
	
	RBuffer.Append(_L("<uid>"));
	RBuffer.Append(iUserNames);
	RBuffer.Append(_L("</uid>"));
	
	RBuffer.Append(_L("<hp>"));
	RBuffer.Append(iMobileNo);
	RBuffer.Append(_L("</hp>"));
	
	RBuffer.Append(_L("<did>"));
	RBuffer.Append(PhoneImei());
	RBuffer.Append(_L("</did>"));

	RBuffer.Append(_L("<currec></currec><totrec></totrec><loc>en_us</loc><ctime></ctime><lstime>0</lstime><synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype>"));

	RBuffer.Append(_L("</h><b></b></root>"));

#ifndef WAP_PT	
	HBufC8* content_data;
	content_data=RBuffer.AllocL();

	TBuf8<10> content_l;
	content_l.Num(content_data->Length());
	//RBuffer.Copy(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	RBuffer.Copy(_L("GET "));
	//RBuffer.Append(_L("/ripple/publishreghomework"));
	RBuffer.Append(iAlbumUrl);
	//RBuffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	RBuffer.Append(_L(" HTTP/1.0\r\nHOST:"));
	RBuffer.Append(iServerAddr);
	RBuffer.Append(_L("\r\nPORT:"));
	RBuffer.AppendNum(iPort);
	RBuffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));


	RBuffer.Append(content_l);
	RBuffer.Append(_L("\r\n\r\n"));
	RBuffer.Append(*content_data);

	//delete servlet_data;
	//delete iSocketWriterPublish;
#endif

	servlet_data = RBuffer.AllocL();
	ConnectToServerForAlbumSynching();	
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSharingNames(TDes& buffer)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetSharingNames(TDes& buffer)
{
	TInt pos;

	pos = MailId.Locate(';');	
	
#ifndef __UIQ__	
	buffer.Copy(&MailId[0],pos-3);
#else
	buffer.Copy(&MailId[0],pos);
#endif		

	pos++; 
	if(pos<MailId.Length())	
		MailId.Delete(0,pos);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSharingMails(TDes& buffer)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetSharingMails(TDes& buffer)
{
	TInt pos;

	pos = MailNames.Locate(';');	
	
	if(pos == KErrNotFound)
		return;

	buffer.Copy(&MailNames[0],pos);

	pos++; 
	if(pos<=MailNames.Length())
		MailNames.Delete(0,pos);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::MoreSharingFilesToUpload(TInt flag)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::MoreSharingFilesToUpload(TInt flag)
{
	if(flag == 0)// success for tell a friend
	{				
		ShowMessageBoxFromResource(R_TEXT_SHARING_APPLICATION_SUCCESS);
	}
	else if(flag == 1)// success for album share 
	{
		ShowMessageBoxFromResource(R_TEXT_SHARING_ALBUM_SUCCESS);
	}
	else if(flag == 2)//failure for tell a friend
	{
		//ShowMessageBoxFromResource(R_TEXT_SHARING_ALBUM_SUCCESS);
	}
	else if(flag == 3)//failure for album share if album empty 
	{
		ShowMessageBoxFromResource(R_TEXT_SHARING_ALBUM_EMPTY);
	}
	else if(flag == 4)//failure for album share if album already shared 
	{
		ShowMessageBoxFromResource(R_TEXT_SHARING_ALBUM_ALREADY_SHARED);
	}
	else if(flag == 5)//failure for album share if mboile not valid 
	{
		ShowMessageBoxFromResource(R_TEXT_SHARING_ALBUM_INVALID_NUMBER);
	}
	else if(flag == 6)//failure for album share if album already shared & rest 
	{
		ShowMessageBoxFromResource(R_TEXT_SHARING_ALBUM_ALREADY_SHARED_REST);
	}
	else if(flag == 7)//failure for album share if mboile not valid & rest
	{
		ShowMessageBoxFromResource(R_TEXT_SHARING_ALBUM_INVALID_NUMBER_REST);
	}
	else//failure for album share if junk data 
	{
		ShowMessageBoxFromResource(R_TEXT_SHARING_ALBUM_FAILED);
	}

	if (iPhotosDB->RowCountForSharing())
    {
		if(iSocketWriterShare != NULL)
		{   	
			iSocketWriterShare->Disconnect();
			delete iSocketWriterShare;
			iSocketWriterShare = NULL;
		}
        

		CloseConnection();
		//SendToServer();
		//ConnectToServerForSharing();
		TBuf<150> mess;
		TInt8 sharingCount;

		GetMails(albumName,MailNames,MailId, mess, sharingCount);
		if(albumName.Compare(_L("")) == 0)
		{							
			GetClientSharingAlbumXML();				
		}
		else
		{			
			GetSharingAlbumXML();
		}
		//iConnectionStatus = 2;
		//CheckSubscriptionStatus(iConnectionStatus);
    }
    else
    {
#ifdef __LOGME__
		LogText.Copy(_L("moresharing 1\n"));
		WriteLogFile(LogText);
#endif

		//iScheduled = 0;
		if(iSocketWriterShare != NULL)
		{	
		    iSocketWriterShare->Disconnect();
			delete iSocketWriterShare;
		    iSocketWriterShare = NULL;
		}

		CloseConnection();

#ifdef __LOGME__
		LogText.Copy(_L("moresharing 2\n"));
		WriteLogFile(LogText);
#endif
		#ifndef __UIQ__	
			if(flag == 1 || flag == 3)
				HandleCommandL(EMainSubView);	
			else
				HandleCommandL(EMainView);	
		#else
			ShowWindow(KViewIdStatusScreen);
			DeleteWindow(KViewIdContactsViewScreen);
		#endif

		//GetAlbumSynchingXML();
    }
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SendSharingFileComplete(TInt flag)
// 
// ---------------------------------------------------------

// called after tail end of protocol data is sent out
void CRippleVaultAppUi::SendSharingFileComplete(TInt flag)
{
/*#ifndef __UIQ__	
	if(flag == 1)
		HandleCommandL(EMainSubView);	
	else
		HandleCommandL(EMainView);	
#endif*/
	iPhotosDB->DeleteSharingsFirstRecord();

	//iPhotosDB->AlbumShareStatus(albumName);

	MoreSharingFilesToUpload(flag);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SendUnSharingFileComplete()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::SendUnSharingFileComplete()
{
	ShowMessageBoxFromResource(R_TEXT_UNSHARING_ALBUM_SUCCESS);
	
	iSocketWriterShare->Disconnect();
	delete iSocketWriterShare;
    iSocketWriterShare = NULL;

	CloseConnection();

	#ifndef __UIQ__	
		HandleCommandL(EMainSubView);	
	#endif
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ChangePasswordComplete()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::ChangePasswordComplete()
{
	CloseSocketPassword();

	SaveUserSettingsToDB(iUserNames, iNewPassword, iMobileNo);

	TBuf8<150> aValue;
	TBuf8<150> aParam;
	
	aParam.Copy(_L("password"));
	aValue.Copy(iNewPassword);	
	EditConfigFile(aValue, aParam);

	ShowMessageBoxFromResource(R_TEXT_CHANGE_PASSWORD_SUCCESS);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::AlbumSynchingComplete()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::AlbumSynchingComplete()
{
	if(!RegFileExists())
	{
    TBuf<128> KRegStep1;
    KRegStep1 = CRippleVaultAppUi::ApplicationDriveAndPath();
	KRegStep1.Append(KRegStep1File);

	RFs fs1;

	fs1.Connect();
	
	//delete file
	//fs1.Delete(KRegStep1);
	BaflUtils::DeleteFile(fs1, KRegStep1);

	fs1.Close();

	SaveUserSettingsToDB(iUserNames, iPassword, iMobileNo);

	WritRegisteredFile();

	WritUserDetailsToFile();

//	EnableUpload(iMode);
//	EnableSmsUpload(iSmsMode);

//	EnableContactsUpload(NOT_REGISTERED);

	UserRegistered();

	//UpdateSmsTimestamp();

	CloseSocketPublish();
	}
	else
	{
		CloseSocketPublish();

		//SendToServer();
		MoreFilesToUpload();
	}

	/*
	if(iSocketWriterPublish)
	{
        iSocketWriterPublish->Disconnect();
		delete iSocketWriterPublish;
        iSocketWriterPublish=NULL;
	}*/
	
//	RegChargePublish();
}

void CRippleVaultAppUi::WritUserDetailsToFile()
{
	TBuf<60> tmpstr;	
	TBuf<60> pwd;	

	tmpstr.Copy( _L("password"));	

	LoadFromConfigFile(pwd,tmpstr);

	if(pwd.Compare(_L("")) == 0)
	{
		//_LIT(KConfigFile,"config.txt");

		RFs fs;
		fs.Connect();

		RFile file;
		TBuf8<20> name;
		TInt err;

		name.Copy(_L("\npassword="));
		name.Append(iPassword);
		name.Append(_L("\n"));

		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KConfigFile);

		err = file.Open(fs, path, EFileWrite);

		if(err == KErrNone)
		{
			file.Seek(ESeekEnd,0);
			file.Write(name);
		}

		file.Close();
		fs.Close();
	}
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ConnectToServerForSharing()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::ConnectToServerForSharing()
{
	iSocketWriterShare = CSocketsEngineWriter::NewL(*this);
    iSocketWriterShare->SetServerName(iServerAddr);
    iSocketWriterShare->SetPort(iPort);
	iSocketWriterShare->iBuffer.Copy(*servlet_data);
    iSocketWriterShare->ShareFolders();
	delete servlet_data;
}
// ---------------------------------------------------------
// CRippleVaultAppUi::ConnectToServerForUnSharing()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::ConnectToServerForUnSharing()
{
	iSocketWriterShare = CSocketsEngineWriter::NewL(*this);
    iSocketWriterShare->SetServerName(iServerAddr);
    iSocketWriterShare->SetPort(iPort);
	iSocketWriterShare->iBuffer.Copy(*servlet_data);
    iSocketWriterShare->UnShareFolders();
	delete servlet_data;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ConnectToServerForAlbumSynching()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::ConnectToServerForAlbumSynching()
{
	iSocketWriterPublish = CSocketsEngineWriter::NewL(*this);
    iSocketWriterPublish->SetServerName(iServerAddr);
    iSocketWriterPublish->SetPort(iPort);
	iSocketWriterPublish->iBuffer.Copy(*servlet_data);
	iSocketWriterPublish->SynchAlbumsForEditing();
	delete servlet_data;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ConnectToServerForChangingNumber()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::ConnectToServerForChangingNumber()
{
	LogText.Copy(_L("ConnectToServerForChangingNumber\n"));
	WriteLogFile(LogText);

	if(iSmsSimEngine != NULL)
	{
		iSmsSimEngine->Disconnect();		
		delete iSmsSimEngine;
		iSmsSimEngine = NULL;
	}

	UpdateSimcard();

	//iConnectionStatus = 4;
	//CheckSubscriptionStatus(iConnectionStatus);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ListAlbums()
// 
// ---------------------------------------------------------

#ifndef __UIQ__	
TInt CRippleVaultAppUi::ListAlbums()
{   	
	CDesCArray* array;
	array = GetFolderList();

	if (array)
    {
		CAknListQueryDialog* dlg;
		TInt index=0;
		dlg = new(ELeave) CAknListQueryDialog(&index);			
		dlg->PrepareLC(R_AKNEXQUERY_LIST_SINGLE_PANE);	
		dlg->SetItemTextArray(array);			
		dlg->SetOwnershipType(ELbmDoesNotOwnItemArray);					
		
		if(dlg->RunLD()) 
		{			
		    TPtrC ptr1(array->MdcaPoint(index));
			albumName.Copy(ptr1);			
			
			//iPhotosAppUi->GetMails(iPhotosAppUi->albumName,iPhotosAppUi->MailNames,iPhotosAppUi->MailId, message);
			//AppUi()->HandleCommandL(EMailView);
			iStoragePath = 3;			
			//HandleCommandL(EFindView);
			delete array;
			return 1;
		}
		else
		{
			delete array;
			return 0;
		}
    }    
	else
	{
		ShowMessageBoxFromResource(R_TEXT_NOALBUMS);
		return 0;
	}
}
#endif

// ---------------------------------------------------------
// CRippleVaultAppUi::SetAccesspoint()
// Set the active access point, given the APN Name
// ---------------------------------------------------------

TInt CRippleVaultAppUi::SetAccesspoint()
{
	TInt flag=1;
	//RGenericAgent netAgent;
	//if (netAgent.Open() == KErrNone)
	{
		TBuf<50> Name;
		TBuf<50> Name2;
		TBuf<50> apnname;
		TUint32 ida1=0;
		TBuf<20> TableName;
		TBuf<20> IAPTableName;

		//netAgent.Stop();

		TableName.Copy(_L("OutgoingGPRS"));
		IAPTableName.Copy(_L("IAP"));

		TBuf<20> ColName1;
		ColName1.Copy(_L("Id"));
		
		TBuf<20> ColName2;
		ColName2.Copy(_L("APN"));

		TBuf<20> ColName3;
		ColName3.Copy(_L("Name"));
			
		//CleanupClosePushL(netAgent);	
		CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeIAP);
		CleanupStack::PushL(db);

		CCommsDbTableView* view = db->OpenTableLC(TableName);		
		
		if(iApnCount == -1)
		{
			iApnCount = 0;

			if(view->GotoFirstRecord()!=KErrNotFound)
			{
				do
				{								
					view->ReadTextL(ColName2, apnname);
					view->ReadTextL(ColName3, Name);

					if(apnname.Compare(iAccessPoints)==0) //match
					{						
						iApnCount++;
					}
				}
				while(view->GotoNextRecord()!=KErrNotFound);			
			}		
		}
			
		TInt apncount =0;

		if(view->GotoFirstRecord()!=KErrNotFound)
		{
			do
			{								
				view->ReadTextL(ColName2, apnname);
				view->ReadTextL(ColName3, Name);

				if(apnname.Compare(iAccessPoints)==0) //match
				{
					apncount++;

					if(apncount == iApnCount)
					{

		//LogText.Copy(Name);
		//WriteLogFile(LogText);

						flag=25;									
						break;
					}
				}
			}
			while(view->GotoNextRecord()!=KErrNotFound);			
		}		

		if(flag==25)
		{	
			CCommsDbTableView* view3 = db->OpenTableLC(IAPTableName);		
			if(view3->GotoFirstRecord()!=KErrNotFound)
			{
				do
				{								
					view3->ReadUintL(ColName1, ida1);
					view3->ReadTextL(ColName3, Name2);

					if(Name.Compare(Name2)==0) //match
					{					
						flag=2;									
						iApnConnectionName.Copy(Name);						
						break;
					}
				}
				while(view3->GotoNextRecord()!=KErrNotFound);			
			}		
			CleanupStack::PopAndDestroy();
		}
		
		if(flag==2)
		{		
			CCommsDbConnectionPrefTableView* 
			view1 = db->OpenConnectionPrefTableViewOnRankLC(ECommDbConnectionDirectionOutgoing,1);
			CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref pref;
			if(view1->GotoFirstRecord()!=KErrNotFound)
			{																
					view1->ReadConnectionPreferenceL(pref);

					if(pref.iBearer.iIapId==ida1)// default access point same as the one we want so do nothing
					{
						iApnID = ida1;
						flag = 10;
						view1->UpdateDialogPrefL(ECommDbDialogPrefDoNotPrompt);
					}
					else
					{
						iApnID = ida1;
						CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref aPref;
						aPref.iBearer.iIapId = ida1;
						aPref.iDirection = ECommDbConnectionDirectionOutgoing; 
						aPref.iDialogPref = ECommDbDialogPrefDoNotPrompt; 
						aPref.iBearer.iBearerSet = ECommDbBearerGPRS; 
						aPref.iRanking = 1; 		
				
						TRAPD(leaveCode,view1->ChangeConnectionPreferenceRankL(0)); // call a function
						TRAPD(leaveCode1,view1->InsertConnectionPreferenceL(aPref, EFalse)); // call a function						
							flag = 10;
				}
			}
			else
			{
				iApnID = ida1;
				CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref aPref;
				aPref.iBearer.iIapId = ida1;
				aPref.iDirection = ECommDbConnectionDirectionOutgoing; 
				aPref.iDialogPref = ECommDbDialogPrefDoNotPrompt; 
				aPref.iBearer.iBearerSet = ECommDbBearerGPRS; 
				aPref.iRanking = 1; 		
				
				TRAPD(leaveCode1,view1->InsertConnectionPreferenceL(aPref, EFalse)); // call a function						

				flag = 10;				
			}
			CleanupStack::PopAndDestroy();			
		}
		CleanupStack::PopAndDestroy(2); // view, db, netAgent			
	}	
	return flag;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetCount(TInt& photocount, TInt& videocount)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetCount(TInt& photocount, TInt& videocount)
{
	CDir *dirList;
	TInt err;
	RFs session;	
	
	session.Connect();

	if(BaflUtils::FileExists(session, KFolderPhonePhoto))
	{		
		err = session.GetDir(KFilesPhonePhoto, KEntryAttNormal, ESortByDate, dirList); 			
		photocount = dirList->Count();			
	}

	if(BaflUtils::FileExists(session, KFolderMMCPhoto))
	{
		err = session.GetDir(KFilesMMCPhoto, KEntryAttNormal, ESortByDate, dirList); 
		photocount = photocount + dirList->Count();		
	}	

	if(BaflUtils::FileExists(session, KFolderPhoneVideo))
	{
		err = session.GetDir(KFilesPhoneVideo, KEntryAttNormal, ESortByDate, dirList); 
		videocount = dirList->Count();
	}		

	if(BaflUtils::FileExists(session, KFolderMMCVideo))
	{		
		err = session.GetDir(KFilesMMCVideo, KEntryAttNormal, ESortByDate, dirList); 
		videocount = videocount + dirList->Count();	
	}	

	session.Close();
}

// ---------------------------------------------------------
// CRippleVaultAppUi::AddPhotos()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::AddPhotos()
{
	TInt err=1;	
	RFs session;	
	TBuf<100> filepath;
	TBuf<100> filepath1;
	TBuf<30> foldername;
	TBuf<30> description;

	CheckAndSaveFolderToDB(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));
	foldername.Copy(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));	

	if(err)
	{		
		session.Connect();

		for(int j=0; j<2 ; j++)
		{
			if(j==0)
			{
				filepath.Copy(KFilesPhonePhoto);
				filepath1.Copy(KFolderPhonePhoto);
			}
			else if(j==1)
			{
				filepath.Copy(KFilesMMCPhoto);
				filepath1.Copy(KFolderMMCPhoto);
			}

			if(BaflUtils::FileExists(session, filepath1))
			{
				CDir *dirList;

				err = session.GetDir(filepath, KEntryAttNormal, ESortByDate, dirList);
				
				CleanupStack::PushL(dirList);
				
				if (err != KErrNone) // if error, just return, dir must not exist
					return;			
				
				for(int i=0; i<dirList->Count() ; i++)
				{
					TParse fullentry;
					//////
					TBuf<160> imageFile;
					fullentry.Set((*dirList)[i].iName, &filepath, NULL);
					
					imageFile.Copy(fullentry.FullName());				
			
					//if(j==0 || j==1)
						AddPhotoRecord(imageFile, EFalse, foldername,description, 0, (*dirList)[i].iSize);
					//else if(j==2 || j==3)
					//	AddVideoRecord(imageFile, EFalse, foldername,description, 0, (*dirList)[i].iSize);
					
				}			
				CleanupStack::PopAndDestroy();		// delete dirList			
			}
		}	
		session.Close();
		
		//ScheduleToSend();
	}
}

void CRippleVaultAppUi::AddVideos()
{
	TInt err=1;	
	RFs session;	
	TBuf<100> filepath;
	TBuf<100> filepath1;
	TBuf<30> foldername;
	TBuf<30> description;

	CheckAndSaveFolderToDB(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));
	foldername.Copy(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));	

	if(err)
	{		
		session.Connect();

		for(int j=0; j<2 ; j++)
		{
			if(j==0)
			{
				filepath.Copy(KFilesPhoneVideo);
				filepath1.Copy(KFolderPhoneVideo);
			}
			else if(j==1)
			{
				filepath.Copy(KFilesMMCVideo);
				filepath1.Copy(KFolderMMCVideo);
			}

			if(BaflUtils::FileExists(session, filepath1))
			{
				CDir *dirList;

				err = session.GetDir(filepath, KEntryAttNormal, ESortByDate, dirList);
				
				CleanupStack::PushL(dirList);
				
				if (err != KErrNone) // if error, just return, dir must not exist
					return;			
				
				for(int i=0; i<dirList->Count() ; i++)
				{
					TParse fullentry;
					//////
					TBuf<160> imageFile;
					fullentry.Set((*dirList)[i].iName, &filepath, NULL);
					
					imageFile.Copy(fullentry.FullName());				
			
					//if(j==0 || j==1)
					//	AddPhotoRecord(imageFile, EFalse, foldername,description, 0, (*dirList)[i].iSize);
					//else if(j==2 || j==3)
						AddVideoRecord(imageFile, EFalse, foldername,description, 0, (*dirList)[i].iSize);
					
				}			
				CleanupStack::PopAndDestroy();		// delete dirList			
			}
		}	
		session.Close();
		
		//ScheduleToSend();
	}
}

// ---------------------------------------------------------
// CRippleVaultAppUi::AddTimeStampToFileL(TBuf<256>* timestamp)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::AddTimeStampToFileL(TBuf<256>* timestamp)
{
	RFs fs;
	RFile file;

	TBuf8<256> ts;
	ts.Copy(*timestamp);

	fs.Connect();

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KTimeStampFile);

	TInt err = file.Replace(fs,path ,EFileWrite);

	HBufC8* data;
	data = ts.AllocL();

	if (err == KErrNone) {
		User::LeaveIfError(file.Write(*data));
		file.Flush();
		file.Close();
	}

	delete data;
	data = NULL;

	fs.Close();
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ChangesToBeStoredL(TTime& aCDBTime, TTime& aRippleTime)
// 
// ---------------------------------------------------------

//--------------------------------------------------
//On booting, check whether changes need to be stored
//--------------------------------------------------
TBool CRippleVaultAppUi::ChangesToBeStoredL(TTime& aCDBTime, TTime& aRippleTime)
{
	TBool returnStatus = EFalse;
	RFs fs;
	RFile file;

	fs.Connect();

	// If time stamp file exists check whether it is newer
	// than that of Contacts DB
	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KTimeStampFile);

	if (BaflUtils::FileExists(fs, path)) {
		TInt err = file.Open(fs, path, EFileRead);
		if (err == KErrNone) {
			// Modification time of Ripple
			TBuf8<256> rippleTimeStamp;
			User::LeaveIfError(file.Read(rippleTimeStamp));

			TInt64 rippleVal;
			TBuf<50> str;
			str.Copy(rippleTimeStamp);
			TLex lex(str);
			lex.Val(rippleVal);

			//RFile cdbFile;		// Contact database file
			CContactDatabase *database;	// Contact database
			TBuf<256> cdbFileName; // Contact db file name

			database = CContactDatabase::OpenL();

			if (database->FindContactFile(cdbFileName)) {

				TTime cdbTime;
				fs.Modified(cdbFileName, cdbTime);

				TDateTime janNineteenSeventy
						(1970,EJanuary,0,00,00,00,000000);
				TTime timejan(janNineteenSeventy);

				TTimeIntervalMicroSeconds interval =
						cdbTime.MicroSecondsFrom(timejan);

				TInt64 cdbVal;
				cdbVal = interval.Int64()/1000;

				TTimeIntervalMicroSeconds rms=
						rippleVal * 1000;

				// Modification time of timestamp file
				// should always be greater than or equal
				// to that of contacts DB
				if (rippleVal < cdbVal) {
					aRippleTime = TTime(timejan) + rms;
					aCDBTime = cdbTime;
					returnStatus = ETrue;
				}

				//cdbFile.Close();
				//}
			}
			delete database;
			database = NULL;
			//returnStatus = ETrue;
		}
		file.Close();
	}

	fs.Close();

	//Print(_L("Exiting ChangesToBeStoredL"));

	return returnStatus;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::RegisterContactsChangeHandler()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::RegisterContactsChangeHandler()
{
	TRAPD(leaveCode, iContactsDB = CContactDatabase::OpenL());
	if (leaveCode != KErrNotFound)
	{
		iContactsHandler = CContactsHandler::NewL(this);
		LoadIPPort();
	}
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ReadUserName()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::ReadUserName()
{
	GetUserSettings(iUserNames, iPassword, iMobileNo);
	iUserName.Copy(iUserNames);

	/*_LIT(KRegPath1, "RegDetails.dat");

	RFs fs;
	RFile file;

	fs.Connect();

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KRegPath1);

	TInt err = file.Open(fs, path, EFileRead);
	if (err == KErrNone) {
		TBuf8<20> userName;
		User::LeaveIfError(file.Read(userName));
		iUserName.Copy(userName);
		iUserNames.Copy(userName);
	}

	file.Close();
	fs.Close();*/
}

// ---------------------------------------------------------
// CRippleVaultAppUi::EnableContactsUpload(TInt aIsRegistered)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::EnableContactsUpload(TInt aIsRegistered)
{

#ifndef __WINS__
	if(iSMSEngine == NULL)
	{
		iSMSEngine = CSMSEngine::NewL(*this);
		iSMSEngine->Start();		
	}
#endif
	if(iCopyAO == NULL && aIsRegistered == NOT_REGISTERED) // new user
	{
		iCopyAO = new (ELeave) CCopyContactsAO(*this);
		iCopyAO->StartL();
	}

	if(iChangeNotifier== NULL)
	{
		iChangeNotifier = CContactChangeNotifier::NewL(*iContactsDB, iContactsHandler);
	}
}

// ---------------------------------------------------------
// CRippleVaultAppUi::LoadIPPort()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::LoadIPPort()
{
	//TInetAddr ipAddr;
	TInt portNo;

	//iListBox->SaveSettingsToDB();
	GetContactsSettingsFromDB(iServerAddr , portNo);

	iPort = portNo;
	//ipAddr.Output(iServerAddr);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetUploadSettingsFromDB(TInt& aAlbum, TInt& aPhoto, TInt& aVideo, TInt& aAlbumShares)
// 
// ---------------------------------------------------------

//DB
void CRippleVaultAppUi::GetUploadSettingsFromDB(TInt& aAlbum, TInt& aPhoto, TInt& aVideo, TInt& aAlbumShares)
{
    TInt album = 0;
	TInt photo = 0;
	TInt video = 0;
	TInt albumshares = 0;
	
    iPhotosDB->GetUploadSettings(album, photo, video, albumshares);
	
    aAlbum = album;
    aPhoto = photo;
	aVideo = video;	
	aAlbumShares = albumshares;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSettingsFromDB(TDes& aIP, TInt& aPort, TInt& aMode, TInt& aDesc, TInt& aRoam, TInt& aAutostart)
// 
// ---------------------------------------------------------

//void CRippleVaultAppUi::GetSettingsFromDB(TDes& aIP, TInt& aPort, TInt& aMode, TInt& aDesc, TInt& aRoam, TInt& aAutostart)
//{
////    TUint32 ip = 0;
//    TUint16 port = 0;
//    TInt8 mode = 0;
//	TInt8 desc = 1;
//	TInt8 roam = 0;
//	TInt8 autostart=1;
//
//    iPhotosDB->GetSettings(aIP, port, mode, iDefaultFolder, desc, roam, autostart);
//	
//    aPort = port;
//    aMode = mode;
//	aDesc = desc;
//	aRoam = roam;
//	aAutostart = autostart;
//}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetContactsSettingsFromDB(TDes& aIP, TInt& aPort )
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetContactsSettingsFromDB(TDes& aIP, TInt& aPort )
{
//    TUint32 ip = 0;
    TUint16 port = 0;

	if(iContactsHandler != NULL)
		iContactsHandler->iChangesDB->GetSettings(aIP, port );

	aPort = port;

	/*
    TBuf<16> buf;
    buf.NumUC(ip);
    aIP.Input(buf);
    */
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetContactsSettingsFromDB(TInt& aRoam)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetContactsSettingsFromDB(TInt& aRoam, TInt& aUpload)
{
	TInt8 roam = 0;
	TInt8 upload=0;
	
    if(iContactsHandler != NULL)
		iContactsHandler->iChangesDB->GetSettings(roam, upload);

    aRoam = roam;
	aUpload = upload;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SaveContactsSettingsToDB(TDesC& aIP, TUint16 aPort )
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::SaveContactsSettingsToDB(TDesC& aIP, TUint16 aPort )
{
    iContactsHandler->iChangesDB->SaveSettingsL(aIP, aPort);
    // save server config
    /*
    TInetAddr addr;
    TBuf<16> buf;

    buf.NumUC(aIP);
    addr.Input(buf);
    addr.Output(buf);
    */
//	iServerAddr.Copy(aIP);
//    iPort = aPort;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SaveContactsSettingsToDB(TInt8 aDesc)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::SaveContactsSettingsToDB(TInt8 aDesc, TInt8 aUpload)
{
	iContactsHandler->iContactsUpload = aUpload;
	iContactsHandler->iContactsRoaming = aDesc;
    iContactsHandler->iChangesDB->SaveSettingsL(aDesc, aUpload);

	TTime oldtime;
	TTime newtime;

	iPhotosDB->GetContactsSettingsTimestampFromDB(oldtime, newtime);

	oldtime = newtime;
	newtime.UniversalTime();
	iPhotosDB->UpdateContactsSettingsTimestampToDB(oldtime, newtime);

}

// ---------------------------------------------------------
// CRippleVaultAppUi::SaveSettingsToDB(TDes& aIP, TUint16 aPort, TInt8 aAutostart)//, TInt8 aMode, TInt8 aDesc, TInt8 aRoam)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::SaveSettingsToDB(TDes& aIP, TUint16 aPort, TInt8 aAutostart)//, TInt8 aMode, TInt8 aDesc, TInt8 aRoam)
{
    iPhotosDB->SaveSettingsL(aIP, aPort, aAutostart);//, aMode, iDefaultFolder, aDesc, aRoam);
    iContactsHandler->iChangesDB->SaveSettingsL(aIP, aPort);

    TBuf<128> KAutostartSetting;
    KAutostartSetting = CRippleVaultAppUi::ApplicationDriveAndPath();
	KAutostartSetting.Append(KAutostartSettings);
//	_LIT(KAutostartSetting, "C:\\System\\Apps\\RippleVault\\astart.txt");

	//Create AutostartSetting.cfg or delete it according to iAutostart variable
	// if exists dont autostart else do
	RFs fs;
	RFile file;

	fs.Connect();
	
	if(aAutostart==0){

	TInt err = file.Replace(fs, KAutostartSetting, EFileStreamText|EFileWrite|EFileShareAny);

	if (err == KErrNone) 
	{
		file.Close();
	}		
	}
	else{
		//delete file
		fs.Delete(KAutostartSetting);
	}

	fs.Close();		

	iServerAddr.Copy(aIP);    
	iPort = aPort;
	iAutostart = aAutostart;
//	iMode = aMode;
//	iDescription = aDesc;
//	iRoaming = aRoam;

	TTime oldtime;
	TTime newtime;

	iPhotosDB->GetGeneralSettingsTimestampFromDB(oldtime, newtime);

	oldtime = newtime;
	newtime.UniversalTime();
	iPhotosDB->UpdateGeneralSettingsTimestampToDB(oldtime, newtime);
	
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SavePhotosSettingsToDB(TInt8 aMode, TInt8 aDesc, TInt8 aRoam, TInt8 aSecureGallery)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::SavePhotosSettingsToDB(TInt8 aMode, TInt8 aDesc, TInt8 aRoam, TInt8 aSecureGallery)
{
    iPhotosDB->SaveSettingsL(aMode, iDefaultFolder, aDesc, aRoam, aSecureGallery);

//	iServerAddr.Copy(aIP);    
//	iPort = aPort;
	iMode = aMode;
	iDescription = aDesc;
	iRoaming = aRoam;
	iSecureGallery = aSecureGallery;

	EnableUpload(iMode);

	TTime oldtime;
	TTime newtime;

	iPhotosDB->GetPublishSettingsTimestampFromDB(oldtime, newtime);

	oldtime = newtime;
	newtime.UniversalTime();
	iPhotosDB->UpdatePublishSettingsTimestampToDB(oldtime, newtime);

}

// ---------------------------------------------------------
// CRippleVaultAppUi::SaveSmsSettingsToDB(TInt8 aSmsMode, TTime aSmsTime, TInt8 aSmsRoam)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::SaveSmsSettingsToDB(TInt8 aSmsMode, TTime aSmsTime, TInt8 aSmsRoam)
{
    iPhotosDB->SaveSettingsL(aSmsMode, aSmsTime, aSmsRoam);

	iSmsMode = aSmsMode;
	iSmsTime = aSmsTime;
	iSmsRoam = aSmsRoam;

	EnableSmsUpload(iSmsMode);

	TTime oldtime;
	TTime newtime;

	iPhotosDB->GetSmsSettingsTimestampFromDB(oldtime, newtime);

	oldtime = newtime;
	newtime.UniversalTime();
	iPhotosDB->UpdateSmsSettingsTimestampToDB(oldtime, newtime);

}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSettingsFromDB(TDes& aIP, TInt& aPort, TInt& aAuto)//, TInt& aMode, TInt& aDesc, TInt& aRoam)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetSettingsFromDB(TDes& aIP, TInt& aPort, TInt& aAuto)//, TInt& aMode, TInt& aDesc, TInt& aRoam)
{
//  TUint32 ip = 0;
    TUint16 port = 0;
    TInt8 autostart = 1;
//  TInt8 mode = 0;
//	TInt8 desc = 0;
//	TInt8 roam = 0;
    iPhotosDB->GetSettings(aIP, port, autostart);//, mode, iDefaultFolder, desc, roam);
//	GetContactsSettingsFromDB(aIP, port);
	
    aPort = port;
    aAuto = autostart;
//  aMode = mode;
//	aDesc = desc;
//	aRoam = roam;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetPhotosSettingsFromDB(TInt& aMode, TInt& aDesc, TInt& aRoam, TInt& aSecureGallery)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetPhotosSettingsFromDB(TInt& aMode, TInt& aDesc, TInt& aRoam, TInt& aSecureGallery)
{
//    TUint32 ip = 0;
//    TUint16 port = 0;
    TInt8 mode = 0;
	TInt8 desc = 1;
	TInt8 roam = 0;
	TInt8 securegallery = 1;
//	TInt8 autostart = 1;
    iPhotosDB->GetSettings(mode, iDefaultFolder, desc, roam, securegallery);
	
//    aPort = port;
    aMode = mode;
	aDesc = desc;
	aRoam = roam;
	aSecureGallery = securegallery;
//	aStart = autostart;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSmsSettingsFromDB(TInt& aSmsMode, TTime& aSmsTime, TInt& aSmsRoam)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetSmsSettingsFromDB(TInt& aSmsMode, TTime& aSmsTime, TInt& aSmsRoam)
{
    TInt8 smsMode = 1;
	TTime smsTime;
	TInt8 roam = 0;

    iPhotosDB->GetSettings(smsMode, smsTime, roam);
	
    aSmsMode = smsMode;
	aSmsTime = smsTime;
	aSmsRoam = roam;	
}

void CRippleVaultAppUi::SaveTempContactsUploadSettingsToDB(TInt aCount)
{
	iContactsHandler->iChangesDB->SaveTempUploadSettings(aCount);

	//UpdateScreen();
}
    
void CRippleVaultAppUi::GetTempContactsUploadSettingsFromDB(TInt& aCount)
{
	TInt count = 0;
	iContactsHandler->iChangesDB->GetTempUploadSettings(count);
	aCount = count;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SaveContactsUploadSettingsToDB(TInt aCount)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::SaveContactsUploadSettingsToDB(TInt aCount)
{
	iContactsHandler->iChangesDB->SaveUploadSettings(aCount);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetContactsUploadSettingsFromDB(TInt& aCount)
// 
// ---------------------------------------------------------
    
void CRippleVaultAppUi::GetContactsUploadSettingsFromDB(TInt& aCount)
{
	TInt count = 0;
	iContactsHandler->iChangesDB->GetUploadSettings(count);
	aCount = count;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SaveSmsUploadSettingsToDB(TInt aCount)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::SaveSmsUploadSettingsToDB(TInt aCount)
{
	iPhotosDB->SaveSmsCountL(aCount);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSmsUploadSettingsFromDB(TInt& aCount)
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::GetSmsUploadSettingsFromDB(TInt& aCount)
{
	TInt count = 0;
	iPhotosDB->GetSmsCountL(count);
	aCount = count;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::HideView()
// 
// ---------------------------------------------------------

void CRippleVaultAppUi::HideView()
{
#ifndef __UIQ__
 CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
 statusPane->SwitchLayoutL(R_AVKON_STATUS_PANE_LAYOUT_EMPTY);   
#endif 
}

#ifdef __UIQ__						

// ---------------------------------------------------------
// CRippleVaultAppUi::ShowContactsDialog(TDesC& fname)
// show the contacts list for UIQ
// ---------------------------------------------------------

TInt CRippleVaultAppUi::ShowContactsDialog(TDesC& fname)
	{
		CEikDialog* selectedcontactsdialog;	
		TInt err;	
		
		selectedcontactsdialog = new (ELeave) CSelectedContactsDialog(*this);
		//err = selectedcontactsdialog->ExecuteLD(R_SELECTEDCONTACTS_DIALOG);
		selectedcontactsdialog->PrepareLC(R_SELECTEDCONTACTS_DIALOG);
		selectedcontactsdialog->SetTitleL(fname);
		err = selectedcontactsdialog->RunLD();
		return err;
	}

#endif

// ---------------------------------------------------------
// CRippleVaultAppUi::GetImsiNumber()
// Get the IMSI number
// ---------------------------------------------------------

void CRippleVaultAppUi::GetImsiNumber()
{	
#ifdef __UIQ__
#ifdef __S80__
	CIMSI* imsi = CIMSI::NewL();
	imsi->GetIMSI();

	do
	{
	} while (!imsi->IsRetrieved());
	
	iImsiNumber.Copy(imsi->retrievedIMSI);

	delete imsi;
	imsi = NULL;
#else
	RFs fs;
	fs.Connect();
	RFile file;

	_LIT(KImsiFileName,"C:\\System\\data\\imsi.txt");

	TInt res = file.Open(fs,KImsiFileName,
	 EFileShareReadersOnly|EFileStreamText);

	if(res != KErrNone)
	{
	 //gConsole->Printf(_L("Open failed: %d\n"),res);
	}
	else
	{
	 TBuf8<128> buf; 
	 file.Read(buf);
	 file.Close();
	 fs.Close();

	 iImsiNumber.Copy(buf);
	}
#endif
#else
	TBuf<255> KTsyName;

	// Read the TSY name from CommsDB
	CCommsDatabase* db = CCommsDatabase::NewL(EDatabaseTypeUnspecified);
	CleanupStack::PushL(db);
	CCommsDbTableView* table = db->OpenTableLC(TPtrC(MODEM));
	table->GotoFirstRecord();
	table->ReadTextL(TPtrC(MODEM_TSY_NAME),KTsyName);
	// Cleanup - CommsDB no longer needed
	CleanupStack::PopAndDestroy(2); // table,db

	if (!KTsyName.Length())
	{
		User::Leave(KErrNotFound);
	}

	RTelServer server;
	CleanupClosePushL(server); 
	User::LeaveIfError(server.Connect());
	
	//Load in the phone device driver 				
	User::LeaveIfError(server.LoadPhoneModule(KTsyName));
	
	//Find the number of phones available from the tel server				
	TInt numberPhones; 
	User::LeaveIfError(server.EnumeratePhones(numberPhones));
	
	//Check there are available phones 
	if (numberPhones < 1)
		User::Leave(KErrNotFound); 

	//Get info about the first available phone 
	RTelServer::TPhoneInfo info; 
	User::LeaveIfError(server.GetPhoneInfo(0, info));

#ifdef __NOKIA6600__		
	RMobilePhone phone;
	CleanupClosePushL(phone); 
	User::LeaveIfError(phone.Open(server, info.iName));

	TRequestStatus status;
	RMobilePhone::TMobilePhoneSubscriberId aIMSI;			
	
	phone.GetSubscriberId(status,aIMSI);
	User::WaitForRequest(status);
	
	iImsiNumber.Copy(aIMSI);	
#else
	//Use this info to open a connection to the phone, the phone is identified by its name 
	RAdvGsmPhone phone; 
	CleanupClosePushL(phone); 
	User::LeaveIfError(phone.Open(server, info.iName));

	TRequestStatus satus; 
	RAdvGsmPhone::TSubscriberId imsiNumber; 
	phone.GetSubscriberId(satus, imsiNumber); 
	User::WaitForRequest(satus);

	// imsiNumber is a TBuf which contains the IMSI number
	iImsiNumber.Copy(imsiNumber);	
#endif	
	CleanupStack::PopAndDestroy();//phone, line 
					
	//Unload the phone device driver 
	server.UnloadPhoneModule(KTsyName); 

	//Close the connection to the tel server and remove it from the cleanup stack 
	CleanupStack::PopAndDestroy();
#endif
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ImsiCheck()
// Check is SIM card has been changed
// ---------------------------------------------------------

void CRippleVaultAppUi::ImsiCheck()
{
	TBuf<50> file;

	if(ReadImsiNumber())
	{
		iSmsValue = -1;
		UpLoadDatas();
	}
	else
	{
		//TInt ret = CheckUserId();
		TInt ret = 1;

		if(ret == 1)
		{
			TBuf<600> msg;
			msg.Copy(*(iEikonEnv->AllocReadResourceL(R_RIPPLE_SIM)));
			msg.Append(iMobileNo);			
			msg.Append(*(iEikonEnv->AllocReadResourceL(R_RIPPLE_SIM1)));
#ifndef __UIQ__
			CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(msg);
			abtdlg->PrepareLC(R_SIM_INFO);
			if (abtdlg->RunLD())
#else
#ifdef __S80__
			if(CCknConfirmationDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),msg))
#else
			CEikDialog* dialog;
			dialog = new (ELeave) CCustomMessageDialog(msg, R_EXAMPLE_RIPPLE_PUBLISH);
			if(dialog->ExecuteLD(R_EIK_DIALOG_YESNO_INFO))
#endif
#endif
			{
				//sendsms
				file.Copy(iMobileNo);
				iTempMbNum.Copy(iMobileNo);

				if(iSmsSimEngine == NULL)
				{
					iSmsSimEngine = CSmsSimEngine::NewL(*this);
					iSmsSimEngine->Start();
				}
				iSmsFile->CmdSendL(file,file,0);
#ifndef __UIQ__
				iWaitDialog = new(ELeave)CAknWaitDialog( (REINTERPRET_CAST(CEikDialog**,&(iWaitDialog))));
				iWaitDialog->SetTone( CAknNoteDialog::EConfirmationTone ); 	
				iWaitDialog->ExecuteLD(R_SMS_DIALOG);
#endif
			}			
			else
			{

			//TBuf<600> msg;
			//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_MSISDN_CHANGE_MESSAGE)));
#ifndef __UIQ__
			CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_MSISDN_CHANGE_MESSAGE)));
			abtdlg->PrepareLC(R_MESSAGE_INFO);
			if (abtdlg->RunLD())
#else
#ifdef __S80__
			//if(CCknConfirmationDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),*(iEikonEnv->AllocReadResourceL(R_MSISDN_CHANGE_MESSAGE))))
			CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)), *iEikonEnv->AllocReadResourceL(R_MSISDN_CHANGE_MESSAGE));
#else
			CEikDialog* dialog;
			//dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_MSISDN_CHANGE_MESSAGE)), R_EXAMPLE_RIPPLE_PUBLISH);
			//if(dialog->ExecuteLD(R_EIK_DIALOG_YESNO_INFO));
			dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_MSISDN_CHANGE_MESSAGE)), R_EXAMPLE_RIPPLE_PUBLISH);
			dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif
#endif
			{

			}

			StopSecure();
			User::Exit(EEikCmdExit);

			//ExitApplication();
/*#ifndef __UIQ__
				TBuf<30> number;
				number.Copy(_L(""));
				CAknTextQueryDialog* dialog1 = CAknTextQueryDialog::NewL(number);
				if(dialog1->ExecuteLD(R_MOBILE_NUMBER))			
				{
					//sendsms
					file.Copy(number);
					iTempMbNum.Copy(number);					

					if(iSmsSimEngine == NULL)
					{
						iSmsSimEngine = CSmsSimEngine::NewL(*this);
						iSmsSimEngine->Start();
					}
					iSmsFile->CmdSendL(file,file,0);
				}			
				else
				{
					ImsiCheck();
				}
#else
				CEikDialog* mobdialog;	
				mobdialog = new (ELeave) CMobileDialog(*this);
				if(mobdialog->ExecuteLD(R_MOBILE_DIALOG))
				{
					//sendsms
					file.Copy(iTempMbNum);					

					if(iSmsSimEngine == NULL)
					{
						iSmsSimEngine = CSmsSimEngine::NewL(*this);
						iSmsSimEngine->Start();
					}
					iSmsFile->CmdSendL(file,file,0);
				}
				else
				{
					ImsiCheck();
				}
#endif			*/
			}
		}
		else if(ret == 0)
		{
			//TBuf<100> msg;
			//msg.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_INVALID_USERID_PASSWORD)));
			#ifndef __UIQ__
				/*CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(msg);
				abtdlg->PrepareLC(R_ABOUT_QUERY);
				abtdlg->RunLD(); */
				ShowMessageBoxFromResource(R_TEXT_INVALID_USERID_PASSWORD);
				HandleCommandL(EExit);
			#else				
				//CEikonEnv::Static ()->InfoWinL(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),msg);
				ShowMessageBoxFromResource(R_TEXT_INVALID_USERID_PASSWORD);
				HandleCommandL(TStop);				
			#endif
		}
		else
		{			
			#ifndef __UIQ__
				HandleCommandL(EExit);
			#else							
				HandleCommandL(TStop);				
			#endif
		}
	}	
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ReadSecureImsiNumber()
// Check IMSI number in Scure Folder with IMSI number of SIM
// ---------------------------------------------------------

TBool CRippleVaultAppUi::ReadSecureImsiNumber()
{
	RFs fs;
	fs.Connect();

	RFile file;
	TBuf8<20> name;
	TBuf<20> imsi;
	TInt err;
	TBuf<150> path1;

	path1.Copy(_L("c:\\system\\secure\\"));	
	path1.Append(KIMSIFile);

	err = file.Open(fs, path1, EFileRead|EFileShareAny);

	//imsi.Copy(iImsiNumber);

	if(err == KErrNone)
	{
		file.Read(name);
		imsi.Copy(name);
	}
	
	file.Close();
	fs.Close();	

	if(imsi.Compare(_L("")) == 0)
		err = 2;
	else
	{	
		//if(imsi.Compare(iImsiNumber) == 0)
		if((imsi.Find(iImsiNumber) >= 0 || iImsiNumber.Find(imsi) >= 0) && imsi.Length() >= 10 && iImsiNumber.Length() >= 10) //new check
			err = 1;
		else
			err = 0;
	}

	return err;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ReadImsiNumber()
// Check IMSI number in DB with IMSI number of SIM
// ---------------------------------------------------------

TBool CRippleVaultAppUi::ReadImsiNumber()
{
//	_LIT(KRegPath1, "Imsi.txt");
	
	RFs fs;
	fs.Connect();

	RFile file;
	TBuf8<20> name;
	TBuf<20> imsi;
	TInt err;
	TBuf<150> path1;

	path1 = CRippleVaultAppUi::ApplicationDriveAndPath();	
	path1.Append(KIMSIFile);

	err = file.Open(fs, path1, EFileRead|EFileShareAny);

	if(err == KErrNone)
	{
		file.Read(name);
		imsi.Copy(name);
	}
	
	file.Close();
	fs.Close();	

	//if(imsi.Compare(iImsiNumber) == 0)
	if((imsi.Find(iImsiNumber) >= 0 || iImsiNumber.Find(imsi) >= 0) && imsi.Length() >= 10 && iImsiNumber.Length() >= 10) //new check
		err = 1;
	else
		err = 0;

	return err;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::WriteImsiNumber()
// Write IMSI number to DB
// ---------------------------------------------------------

void CRippleVaultAppUi::WriteImsiNumber()
{
	//_LIT(KRegPath1, "Imsi.txt");
	
	RFs fs;
	fs.Connect();

	RFile file;	
	TInt err;
	TBuf<150> path1;
	TBuf8<50> name;

	name.Copy(iImsiNumber);

	path1 = CRippleVaultAppUi::ApplicationDriveAndPath();	
	path1.Append(KIMSIFile);

	err = file.Open(fs, path1, EFileWrite);

	if (err == KErrNotFound) 
		file.Create(fs, path1, EFileWrite);

	file.Write(name);
	
	file.Close();
	fs.Close();		
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ExitApplication()
// Exits application after confirm message
// ---------------------------------------------------------

void CRippleVaultAppUi::ExitApplication()
{
#ifndef __UIQ__
			CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_MSISDN_CHANGE_MESSAGE)));
			abtdlg->PrepareLC(R_MESSAGE_INFO);
			if (abtdlg->RunLD())
#else
#ifdef __S80__
			//if(CCknConfirmationDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),*(iEikonEnv->AllocReadResourceL(R_MSISDN_CHANGE_MESSAGE))))
			CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)), *iEikonEnv->AllocReadResourceL(R_MSISDN_CHANGE_MESSAGE));
#else
			CEikDialog* dialog;
			//dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_MSISDN_CHANGE_MESSAGE)), R_EXAMPLE_RIPPLE_PUBLISH);
			//if(dialog->ExecuteLD(R_EIK_DIALOG_YESNO_INFO));
			dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_MSISDN_CHANGE_MESSAGE)), R_EXAMPLE_RIPPLE_PUBLISH);
			dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif
#endif
			{

			}

			if(iSmsSimEngine != NULL)
			{
				iSmsSimEngine->Disconnect();
				delete iSmsSimEngine;
				iSmsSimEngine = NULL;
			}

			//StopSecure();
			User::Exit(EEikCmdExit);

	//TBuf<100> msg;	

	//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_RIPPLE_CLOSE)));
		
	/*
#ifndef __UIQ__
	CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_RIPPLE_CLOSE)));
	abtdlg->PrepareLC(R_SIM_INFO);
	if (abtdlg->RunLD())
#else
#ifdef __S80__
	if(CCknConfirmationDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),*(iEikonEnv->AllocReadResourceL(R_RIPPLE_CLOSE))))
#else
	CEikDialog* dialog;
	dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_RIPPLE_CLOSE)), R_EXAMPLE_RIPPLE_PUBLISH);
	if(dialog->ExecuteLD(R_EIK_DIALOG_YESNO_INFO))
#endif
#endif
	{	
		if(iSmsSimEngine != NULL)
			{
				iSmsSimEngine->Disconnect();		
				delete iSmsSimEngine;
				iSmsSimEngine = NULL;
			}
		#ifndef __UIQ__
			if(iWaitDialog == NULL)
				HandleCommandL(EExit);
		#else				
			HandleCommandL(TStop);				
		#endif
	}
	else
	{
	#ifndef __UIQ__				
		if(iSmsSimEngine != NULL)
		{
			iSmsSimEngine->Disconnect();
			delete iSmsSimEngine;
			iSmsSimEngine = NULL;
		}
	#endif
		ImsiCheck();
	}
	*/
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ShowView()
// Photos/Videos Gallery for UIQ
// ---------------------------------------------------------

#ifdef __UIQ__				
void CRippleVaultAppUi::ShowView()
{
	TInt8 choice=0;
	GetChoice(choice);
	
	if(iImageFlag == 1 || iImageFlag == 2)
	{
		/*if(choice == 0)
		{			
			iImageFlag = 3;
			ShowWindow(KViewIdVideoList); //video screen
			DeleteWindow(KViewIdPhotoList); //photo screen
		}
		else
		{*/
			ShowWindow(KViewIdStatusScreen); //main screen
			DeleteWindow(KViewIdPhotoList); //photo screen
		//}
	}
	else if(iImageFlag == 3 || iImageFlag == 4)
	{
		ShowWindow(KViewIdStatusScreen); //main screen
		DeleteWindow(KViewIdVideoList); //video screen

		//if(choice == 0)
		//{			
		//	CallSetupWizardStep12();
		//}
	}
	else if(iImageFlag == 5 || iImageFlag == 6)
	{
		ShowWindow(KViewIdStatusScreen); //main screen
		DeleteWindow(KViewIdSmsList); //sms screen

		//SaveChoice(1);
	}
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ShowToolBar()
// Shows sub screen for UIQ
// ---------------------------------------------------------
/*
void CRippleVaultAppUi::ShowToolBar()
{
#ifndef __S80__
	CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
	appUi->SetToolbarL(R_SCREENLIST_TOOLBAR);

	//iMainScreenView->UpdateListBoxL(iMainScreenValue);
#endif
}
*/
void CRippleVaultAppUi::SecureDialog()
{
	CEikDialog* dialog;		
	dialog = new (ELeave) CSecureSettingDialog(*this);
	dialog->ExecuteLD(R_SECURE_SETTINGS);		
}

/*
void CRippleVaultAppUi::ShowSettingsScreen(TInt aIndex)
{
#ifndef __S80__
	TInt err;
	CEikDialog* dialog;	
	if( iMainScreenValue == 1)//contacts
	{
		if( aIndex == 1)//send to vault
		{
			//iConnectionStatus = 2;
			//CheckSubscriptionStatus(iConnectionStatus);						
		}
		else if( aIndex == 2)//settings
		{
			dialog = new (ELeave) CContactsSettingDialog(*this);
			err = dialog->ExecuteLD(R_CONTACTS_SETTINGS);	
		}
	}
	else if( iMainScreenValue == 2)//photos
	{
		if( aIndex == 1)//send to vault
		{
			iImageFlag = 1;
			ShowWindow(KViewIdPhotoList);   //photo screen
			DeleteWindow(KViewIdStatusScreen); //main screen
		}		
		else if( aIndex == 3)//sharing
		{
			MailNames.Copy(_L(""));
			MailId.Copy(_L(""));
			albumName.Copy(_L(""));
			iMessage.Copy(_L(""));
			iStoragePath = 2;

			ShowWindow(KViewIdContactsViewScreen); 
			DeleteWindow(KViewIdStatusScreen);
		}
		else if( aIndex == 4)//unsharing
		{			
			dialog = new (ELeave) CUnShareAlbumDialog(*this);
			err = dialog->ExecuteLD(R_FOLDER_DIALOG1);
		}
		else if( aIndex == 5)//settings
		{
			dialog = new (ELeave) CPhotosSettingDialog(*this);
			err = dialog->ExecuteLD(R_PHOTOS_SETTINGS);	
		}
	}
	else if( iMainScreenValue == 3)//Videos
	{
		if( aIndex == 1)//send to vault
		{
			iImageFlag = 3;
			ShowWindow(KViewIdVideoList);   //video screen
			DeleteWindow(KViewIdStatusScreen); //main screen
		}	
		else if( aIndex == 3)//sharing
		{
			MailNames.Copy(_L(""));
			MailId.Copy(_L(""));
			albumName.Copy(_L(""));
			iMessage.Copy(_L(""));
			iStoragePath = 2;

			ShowWindow(KViewIdContactsViewScreen); 
			DeleteWindow(KViewIdStatusScreen); 			
		}
		else if( aIndex == 4)//unsharing
		{
			dialog = new (ELeave) CUnShareAlbumDialog(*this);
			dialog->ExecuteLD(R_FOLDER_DIALOG1);
		}
		else if( aIndex == 5)//settings
		{
			dialog = new (ELeave) CVideosSettingDialog(*this);
			err = dialog->ExecuteLD(R_VIDEOS_SETTINGS);	
		}		
	}
	else if( iMainScreenValue == 4)//sms
	{
		if( aIndex == 1)//send to vault
		{				
			iImageFlag = 5;
			ShowWindow(KViewIdSmsList);   //sms screen
			DeleteWindow(KViewIdStatusScreen); //main screen
		}
		else if( aIndex == 3)//settings
		{
			dialog = new (ELeave) CSmsSettingDialog(*this);
			err = dialog->ExecuteLD(R_SMS_SETTINGS);	

			if(err)
			if(iSmsMode == 0)
			{
				MailNames.Copy(_L(""));
				MailId.Copy(_L(""));
				albumName.Copy(_L(""));
				iMessage.Copy(_L(""));
				iStoragePath = 1;

				ShowWindow(KViewIdContactsViewScreen); 
				DeleteWindow(KViewIdStatusScreen); 
			}
		}
	}
#endif
}
*/
// ---------------------------------------------------------
// CRippleVaultAppUi::ShowStatusScreen()
// Show Status Screen for UIQ
// ---------------------------------------------------------

void CRippleVaultAppUi::ShowStatusScreen()
{
	TBuf<50> iLabelTitle;          // example label      
	TBuf<50> iLabelPendingItems;          // example label
	TBuf<50> iLabelContactsCount;          // example label
	TBuf<50> iLabelPhotosCount;          // example label
	TBuf<50> iLabelVersion;          // example label      
	TBuf<50> iLabelAppStatus;          // example label      
	TBuf<50> iLabelErrStatus;          // example label      

	TBuf<50> iLabelUploadedItems;          // example label
	TBuf<50> iLabelUploadedContactsCount;          // example label
	TBuf<50> iLabelUploadedPhotosCount;          // example label

	isDoingSomething = EFalse;

	isQueuedInfo = EFalse;

	TBuf<50> message;
	TBuf<50> formatBuf;
	TInt ccounttemp;
	TInt pendingitems=0;
	
	GetTempContactsUploadSettingsFromDB(ccounttemp); //get temp contacts count
	GetContactsParams(pendingitems); // get total contacts count
	iPendingCountContacts = pendingitems - ccounttemp;

	ccounttemp = 0;
	//pendingitems = iPhotosAppUi->iPhotosDB->RowCount(); // get media pending count
	
	iPendingCountPhotos = iPhotosDB->PhotosRowCount();

	iPendingCountVideos = iPhotosDB->VideosRowCount();
	//pendingitems -= ccounttemp; // subtract sent items

	if(iPendingCountContacts<0) // negative items make no sense:-)
		iPendingCountContacts = 0;

	//iPendingCount = pendingitems;

	iPendingCountSms = iSmsCount + iAutoSmsCount;

    iLabelTitle.Copy(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RUNNING)));

	TBuf<50> dum;

	TInt code;

	code = GetNetworkStatusCode(dum);

	if((iPendingCountContacts + iPendingCountPhotos + iPendingCountVideos + iPendingCountSms > 0) || code != NETWORK_STATUS_RUNNING)
	{
		isQueuedInfo = ETrue;

	message.Copy(_L(""));
	//formatBuf.Copy(_L(""));
	message.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_ITEMS_PENDING)));
	//message.Format(formatBuf, pendingitems);

    iLabelPendingItems.Copy(message);
//-------------------------
		message.Copy(_L(""));
		iLabelContactsCount.Copy(message);

		if(iPendingCountContacts>0 || (iPendingCountContacts == 0 && !isQueuedInfo)){
		ccounttemp++;
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_CONTACTS_UPLOADED)));
		message.Format(formatBuf, iPendingCountContacts);

		iLabelContactsCount.Append(message);
		}

		message.Copy(_L(""));
		iLabelPhotosCount.Copy(message);

		if(iPendingCountSms>0  || (iPendingCountSms == 0 && !isQueuedInfo)){
		ccounttemp++;
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_SMS_UPLOADED)));
		message.Format(formatBuf, iPendingCountSms);

//#ifdef __S80__
		if(ccounttemp == 2)
			iLabelContactsCount.Append(_L(", "));

		iLabelContactsCount.Append(message);
/*#else
		if(ccounttemp == 1)
			iLabelContactsCount.Append(message);
		else
			iLabelPhotosCount.Append(message);
#endif*/
		}

		message.Copy(_L(""));

		if(iPendingCountPhotos>0 || (iPendingCountPhotos == 0 && !isQueuedInfo)){
		ccounttemp++;
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_PHOTOS_UPLOADED)));
		message.Format(formatBuf, iPendingCountPhotos);

//#ifdef __S80__
		if(ccounttemp == 2)
			iLabelContactsCount.Append(_L(", "));

		if(ccounttemp > 2)
			iLabelPhotosCount.Append(message);
		else
			iLabelContactsCount.Append(message);
/*#else
		if(ccounttemp > 2)
			iLabelPhotosCount.Append(_L(", "));

		if(ccounttemp > 1)
			iLabelPhotosCount.Append(message);
		else
			iLabelContactsCount.Append(message);
#endif*/
		}

		message.Copy(_L(""));
		if(iPendingCountVideos>0 || (iPendingCountVideos == 0 && !isQueuedInfo)){
		ccounttemp++;
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_VIDEOS_UPLOADED)));
		message.Format(formatBuf, iPendingCountVideos);

//#ifdef __S80__
		if(ccounttemp == 2)
			iLabelContactsCount.Append(_L(", "));
		else if (ccounttemp == 4)
			iLabelPhotosCount.Append(_L(", "));
			
		if(ccounttemp > 2)
			iLabelPhotosCount.Append(message);
		else
			iLabelContactsCount.Append(message);
/*#else
		if(ccounttemp > 2)
			iLabelPhotosCount.Append(_L(", "));
			
		if(ccounttemp > 1)
			iLabelPhotosCount.Append(message);
		else
			iLabelContactsCount.Append(message);
#endif*/
		}

		//}
		if((iPendingCountContacts + iPendingCountPhotos + iPendingCountVideos + iPendingCountSms == 0) && isQueuedInfo)	
		{
			iLabelContactsCount.Copy(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_ZEROITEMS)));
		}


		TInt album;
		TInt albumshares;
		
		GetUploadSettingsFromDB(album, iPendingCountPhotos, iPendingCountVideos, albumshares);
		GetContactsUploadSettingsFromDB(iPendingCountContacts);
		GetSmsUploadSettingsFromDB(iPendingCountSms);

//-------------------------

	}
	else
	{
		isQueuedInfo = EFalse;

		TInt album;
		TInt albumshares;
		
		GetUploadSettingsFromDB(album, iPendingCountPhotos, iPendingCountVideos, albumshares);
		GetContactsUploadSettingsFromDB(iPendingCountContacts);
		GetSmsUploadSettingsFromDB(iPendingCountSms);

		//message.Copy(_L(""));
		//formatBuf.Copy(_L(""));
		//message.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_ITEMS_UPLOADED)));
		//message.Format(formatBuf, pendingitems);
		
		iLabelTitle.Copy(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_LASTSYNCH)));
	}


	iLabelUploadedItems.Copy(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_ITEMS_UPLOADED)));

	//{
	message.Copy(_L(""));
	iLabelUploadedContactsCount.Copy(message);

	ccounttemp = 0;

	if(iPendingCountContacts>0 || (iPendingCountContacts == 0 && !isQueuedInfo)){
	ccounttemp++;
	formatBuf.Copy(_L(""));
	formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_CONTACTS_UPLOADED)));
	message.Format(formatBuf, iPendingCountContacts);

	iLabelUploadedContactsCount.Append(message);
	}

	message.Copy(_L(""));
	if(iPendingCountSms>0 || (iPendingCountSms == 0 && !isQueuedInfo)){
	ccounttemp++;
	formatBuf.Copy(_L(""));
	formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_SMS_UPLOADED)));
	message.Format(formatBuf, iPendingCountSms);

//#ifdef __S80__
		if(ccounttemp == 2)
			iLabelUploadedContactsCount.Append(_L(", "));

		iLabelUploadedContactsCount.Append(message);
/*#else
		if(ccounttemp == 1)
			iLabelUploadedContactsCount.Append(message);
		else
			iLabelUploadedPhotosCount.Append(message);
#endif*/
	}

	message.Copy(_L(""));
	iLabelUploadedPhotosCount.Copy(message);

	if(iPendingCountPhotos>0 || (iPendingCountPhotos == 0 && !isQueuedInfo)){
	ccounttemp++;
	formatBuf.Copy(_L(""));
	formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_PHOTOS_UPLOADED)));
	message.Format(formatBuf, iPendingCountPhotos);

//#ifdef __S80__
		if(ccounttemp == 2)
			iLabelUploadedContactsCount.Append(_L(", "));

		if(ccounttemp > 2)
			iLabelUploadedPhotosCount.Append(message);
		else
			iLabelUploadedContactsCount.Append(message);
/*#else
		if(ccounttemp > 2)
			iLabelUploadedPhotosCount.Append(_L(", "));

		if(ccounttemp > 1)
			iLabelUploadedPhotosCount.Append(message);
		else
			iLabelUploadedContactsCount.Append(message);
#endif*/

	}

	message.Copy(_L(""));
	if(iPendingCountVideos>0 || (iPendingCountVideos == 0 && !isQueuedInfo)){
	ccounttemp++;
	formatBuf.Copy(_L(""));
	formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_VIDEOS_UPLOADED)));
	message.Format(formatBuf, iPendingCountVideos);

//#ifdef __S80__
		if(ccounttemp == 2)
			iLabelUploadedContactsCount.Append(_L(", "));
		else if (ccounttemp == 4)
			iLabelUploadedPhotosCount.Append(_L(", "));
			
		if(ccounttemp > 2)
			iLabelUploadedPhotosCount.Append(message);
		else
			iLabelUploadedContactsCount.Append(message);
/*#else
		if(ccounttemp > 2)
			iLabelUploadedPhotosCount.Append(_L(", "));
			
		if(ccounttemp > 1)
			iLabelUploadedPhotosCount.Append(message);
		else
			iLabelUploadedContactsCount.Append(message);
#endif*/

	}

	//}
	//else
	if((iPendingCountContacts + iPendingCountPhotos + iPendingCountVideos + iPendingCountSms == 0) && isQueuedInfo)	
	{
		iLabelUploadedContactsCount.Copy(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_ZEROITEMS)));
	}

	message.Copy(_L("v"));
	message.Append(iVersion);
	
    iLabelVersion.Copy(message);

	message.Copy(_L("")); // Application status
	iStatusCode = GetNetworkStatusCode(message);
	if(iStatusCode != NETWORK_STATUS_FAILED && iStatusCode != NETWORK_STATUS_RUNNING)
	{
		isDoingSomething = ETrue;
	}
	if(message.Compare(_L("")) == 0){
		message.Copy(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_RUNNING)));
	}

	if(!ReadImsiNumber()){
		isDoingSomething = EFalse;
		message.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_NETWORK_STATUS_SUSPENDED_SIMCHANGE)));
	}
	
    iLabelAppStatus.Copy(message);

	if(!isQueuedInfo)
	{
		isDoingSomething = EFalse;

		TBuf<160> message;

		TTime btime;
		iPhotosDB->GetBackupTimestampFromDB(btime);

		TDateTime aDateTime = btime.DateTime();		
		_LIT(KFormatTxt,"%d/%d/%d %S:%S:%S ");

		TBuf<2> ampm;		
		TInt hour = aDateTime.Hour();
		if(hour>12) 
		{
			hour=hour-12;
			ampm.Copy(_L("pm"));
		}
		else if(hour == 12)
		{
			ampm.Copy(_L("pm"));
		}
		else if(hour == 0)
		{
			hour = 12;
			ampm.Copy(_L("am"));
		}
		else
		{
			ampm.Copy(_L("am"));
		}

		message.Copy(_L(""));

		TInt min = aDateTime.Minute();
		TInt sec = aDateTime.Second();

		TBuf<2> hourbuf;
		TBuf<2> minbuf;
		TBuf<2> secbuf;

		hourbuf.Copy(_L(""));
		minbuf.Copy(_L(""));
		secbuf.Copy(_L(""));

		if(hour < 10)
		{			
			hourbuf.Append(_L("0"));
			hourbuf.AppendNum(hour);
		}
		else
			hourbuf.AppendNum(hour);

		if(min < 10)
		{			
			minbuf.Append(_L("0"));
			minbuf.AppendNum(min);
		}
		else
			minbuf.AppendNum(min);
		

		if(sec < 10)
		{			
			secbuf.Append(_L("0"));
			secbuf.AppendNum(sec);
		}
		else
			secbuf.AppendNum(sec);

		message.Format(KFormatTxt, aDateTime.Day()+1, TInt(aDateTime.Month()+1), 
			aDateTime.Year(), &hourbuf, &minbuf, &secbuf);

		message.Append(ampm);

#ifdef __LOGME__
		LogText.Copy(_L("\nBackup Time="));
		LogText.Append(message);
		LogText.Append(_L("\n"));
		WriteLogFile(LogText);
#endif

		iLabelAppStatus.Copy(message);
	}

	isError = EFalse;

	message.Copy(_L(""));
	iErrCode = GetNetworkErrorCode(message);

	if(iErrCode!=0)
		isError = ETrue;

	isUpgrade = EFalse;

	TInt8 choice;
	TBuf<50> vers;

	GetUpgradeStatus(choice, vers);
	if(choice == 1)
	{
		isUpgrade = ETrue;
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_TEXT_UPGRADE_AVAILABLE)));
		message.Copy(_L(""));
		message.Format(formatBuf, &vers);
	}

	iLabelErrStatus.Copy(message);
		
	//ShowWindow(5); //status screen
	//DeleteWindow(1); //main screen			
	iStatusScreenView->ApplicationStatus(iLabelTitle, iLabelVersion, 
		iLabelAppStatus, iLabelPendingItems, iLabelContactsCount, iLabelPhotosCount,
		iLabelErrStatus, isDoingSomething, isQueuedInfo, iLabelUploadedItems, iLabelUploadedContactsCount, iLabelUploadedPhotosCount);

		/*TBool pending;

		pending = EFalse;

		if(((iPendingCountContacts + iPendingCountPhotos +
			iPendingCountVideos) > 0) && (iStatusCode == NETWORK_STATUS_RUNNING || iStatusCode == NETWORK_STATUS_FAILED))
			pending = ETrue;

		if(!isQueuedInfo)
			pending = EFalse;
*/
//		aMenuPane->SetItemDimmed(ESendToWeb, !pending);

//		aMenuPane->SetItemDimmed(EAboutErrorCode, !isError);

#ifdef __S80__
	iCBA->SetCommandSetL(R_STATUSSCREEN_CBA);
	//iCBA->DimCommand(ESendToWeb, !pending);
	//iCBA->DimCommand(EAboutErrorCode, !isError);
	//iCBA->DimCommand(EShowUploadCount, !isQueuedInfo);
	iCBA->DrawNow();
#endif
}
#endif

// ---------------------------------------------------------
// CRippleVaultAppUi::EnablePhotosUpload()
// checks DB and connects to server for photos/videos upload
// ---------------------------------------------------------

void CRippleVaultAppUi::EnablePhotosUpload()
{
	//CEikonEnv::Static ()->AlertWin(_L("helpme"));	

	if (iPhotosDB->RowCount())
    {
		if(iSocketWriterPublish == NULL)
		{
			//SendToServer();
			iConnectionStatus = 1;
			CheckSubscriptionStatus(iConnectionStatus);
		}        
    }    
}

// ---------------------------------------------------------
// CRippleVaultAppUi::DeleteFolderFromDB(TDesC& aFolder)
// Delete album
// ---------------------------------------------------------

TInt CRippleVaultAppUi::DeleteFolderFromDB(TDesC& aFolder)
{
	TInt ret;
	ret =  iPhotosDB->CheckAndDeleteFolder(aFolder);
    return ret;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::EditFolderFromDB(TDesC& aOldFolder, TDesC& aNewFolder)
// Edit album
// ---------------------------------------------------------

TInt CRippleVaultAppUi::EditFolderFromDB(TDesC& aOldFolder, TDesC& aNewFolder)
{
	TInt ret;
	ret =  iPhotosDB->CheckAndEditFolder(aOldFolder, aNewFolder);
    return ret;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CheckBillingStatus(TInt aStatus)
// Gets Billing Status XML
// ---------------------------------------------------------
/*
void CRippleVaultAppUi::CheckBillingStatus(TInt aStatus)
{    
	TBuf8<1024> RBuffer;
	HBufC* synctime;
	TBuf<30> time;
	TInt err;
		
	RBuffer.Copy(_L("<root><h><msgt>"));
	RBuffer.Append(_L("mo_sent"));
	RBuffer.Append(_L("</msgt>"));

	RBuffer.Append(_L("<msg></msg>"));
	
	RBuffer.Append(_L("<uid>"));
	RBuffer.Append(iUserNames);
	RBuffer.Append(_L("</uid>"));
	
	RBuffer.Append(_L("<hp>"));
	RBuffer.Append(iMobileNo);
	RBuffer.Append(_L("</hp>"));
	
	RBuffer.Append(_L("<did>"));
	RBuffer.Append(PhoneImei());
	RBuffer.Append(_L("</did>"));

	RBuffer.Append(_L("<currec></currec><totrec></totrec><loc>en_us</loc><ctime></ctime><lstime>0</lstime><synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype>"));

	RBuffer.Append(_L("</h><b>"));

	RBuffer.Append(_L("<last-sync-timestamp>"));
	
	GetSubStatusSyncTime(time);
	
	RBuffer.Append(time);
	
	RBuffer.Append(_L("</last-sync-timestamp>"));
	
	RBuffer.Append(_L("</b></root>"));

	HBufC8* content_data;
	content_data=RBuffer.AllocL();

	TBuf8<10> content_l;
	content_l.Num(content_data->Length());

	RBuffer.Copy(_L("GET "));
	RBuffer.Append(iStatusUrl);
	//RBuffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	RBuffer.Append(_L(" HTTP/1.0\r\nHOST:"));
	RBuffer.Append(iServerAddr);
	RBuffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));


	RBuffer.Append(content_l);
	RBuffer.Append(_L("\r\n\r\n"));
	RBuffer.Append(*content_data);

	servlet_data = RBuffer.AllocL();
	
	ConnectToServerForSubscription(aStatus);

	//WriteLogFile(RBuffer);	
}
*/
void CRippleVaultAppUi::UpdateContacts(TInt mode)
{
	TInt network;	
	network = CheckNetwork();

	if(network == NETWORK_ROAMING)
	{
#ifdef __LOGME__
		LogText.Copy(_L("UpdateContacts 1\n"));
		WriteLogFile(LogText);
#endif
		if(iContactsHandler->iContactsRoaming == 1)
		{
#ifdef __LOGME__
			LogText.Copy(_L("UpdateContacts 2\n"));
			WriteLogFile(LogText);
#endif
			if(iContactsHandler->iSocketsEngine == NULL )
			{
#ifdef __LOGME__
				LogText.Copy(_L("UpdateContacts 3\n"));
				WriteLogFile(LogText);
#endif
				if(mode == 0){
#ifdef __LOGME__
					LogText.Copy(_L("status sub for contacts\n"));
					WriteLogFile(LogText);
#endif

					iConnectionStatus = 2;
				}
				else{
#ifdef __LOGME__
					LogText.Copy(_L("status sub for incremental contacts change\n"));
					WriteLogFile(LogText);
#endif
					iConnectionStatus = 6;
				}

				CheckSubscriptionStatus(iConnectionStatus);
			}
		}
	}
	else
	{
#ifdef __LOGME__
		LogText.Copy(_L("UpdateContacts 4\n"));
		WriteLogFile(LogText);
#endif

		if(iContactsHandler->iSocketsEngine == NULL )
		{
#ifdef __LOGME__
			LogText.Copy(_L("UpdateContacts 5\n"));
			WriteLogFile(LogText);
#endif
				if(mode == 0){
#ifdef __LOGME__
					LogText.Copy(_L("status sub for contacts\n"));
					WriteLogFile(LogText);
#endif
					iConnectionStatus = 2;
				}
				else{
#ifdef __LOGME__
					LogText.Copy(_L("status sub for incremental contacts change\n"));
					WriteLogFile(LogText);
#endif
					iConnectionStatus = 6;
				}

			CheckSubscriptionStatus(iConnectionStatus);
		}
	}
}

void CRippleVaultAppUi::UpdateContactsManually()
{
	TInt network;	
	network = CheckNetwork();

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KInitUploadFile);

	RFs fs1;

	fs1.Connect();
	
	//delete file
	//fs1.Delete(KRegStep1);

	if(network == NETWORK_ROAMING)
	{
#ifdef __LOGME__
		LogText.Copy(_L("UpdateContacts 1\n"));
		WriteLogFile(LogText);
#endif
		if(iContactsHandler->iContactsRoaming == 1)
		{
#ifdef __LOGME__
			LogText.Copy(_L("UpdateContacts 4\n"));
			WriteLogFile(LogText);
#endif

			if(!BaflUtils::FileExists(fs1, path))
			{
			iConnectionStatus = 2;

			CheckSubscriptionStatus(iConnectionStatus);
			}

			ShowMessageBoxFromResource(R_TEXT_CONTACTS_SENDTOWEB);
		}
		else
		{
			RoamingDisabledDialogContacts();
		}
	}
	else
	{
#ifdef __LOGME__
		LogText.Copy(_L("UpdateContacts 4\n"));
		WriteLogFile(LogText);
#endif
		if(!BaflUtils::FileExists(fs1, path))
		{
		iConnectionStatus = 2;

		CheckSubscriptionStatus(iConnectionStatus);
		}

		ShowMessageBoxFromResource(R_TEXT_CONTACTS_SENDTOWEB);
	}

	fs1.Close();
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CheckSubscriptionStatus(TInt aStatus)
// Gets Subscription Status XML
// ---------------------------------------------------------

void CRippleVaultAppUi::CheckSubscriptionStatus(TInt aStatus)
{    
#ifdef __LOGME__	
	LogText.Copy(_L("CheckSubscriptionStatus 1\n"));
	WriteLogFile(LogText);
#endif

	TBuf8<1024> RBuffer;
	//HBufC* synctime;
	//TTime time;
	TBuf<30> time;
	TInt err;
		
	RBuffer.Copy(_L("<root><h><msgt>"));
	RBuffer.Append(_L("prof_update"));
	RBuffer.Append(_L("</msgt>"));

	RBuffer.Append(_L("<msg></msg>"));
	
	RBuffer.Append(_L("<uid>"));
	RBuffer.Append(iUserNames);
	RBuffer.Append(_L("</uid>"));
	
	RBuffer.Append(_L("<hp>"));
	RBuffer.Append(iMobileNo);
	RBuffer.Append(_L("</hp>"));
	
	RBuffer.Append(_L("<did>"));
	RBuffer.Append(PhoneImei());
	RBuffer.Append(_L("</did>"));

	RBuffer.Append(_L("<currec></currec><totrec></totrec><loc>en_us</loc><ctime></ctime><lstime>0</lstime><synctype><mod>cs</mod><gran>contactlevel</gran><adde>true</adde><upde>true</upde><dele>true</dele></synctype>"));

	RBuffer.Append(_L("</h><b>"));

	RBuffer.Append(_L("<last-sync-timestamp>"));
	
	GetSubStatusSyncTime(time);
	//TBuf<30> tempbuf;
	//tempbuf.Copy(time);
	//CEikonEnv::Static ()->AlertWin(tempbuf);
	//synctime = GetTimeStamp(time);
	//RBuffer.Append(*synctime);
	RBuffer.Append(time);
	
	RBuffer.Append(_L("</last-sync-timestamp>"));

	RBuffer.Append(_L("<user-prof>"));

	RBuffer.Append(_L("<param name=\"payment-type\" value=\""));
	RBuffer.Append(iConnectionType);
	RBuffer.Append(_L("\"/>"));

	err = ReadImsiNumber();
	
	if(!err)
	{
		RBuffer.Append(_L("<param name=\"imsi\" value=\""));
		RBuffer.Append(iImsiNumber);
		RBuffer.Append(_L("\"/>"));
	
		RBuffer.Append(_L("<param name=\"msisdn\" value=\""));
		//RBuffer.Append(iMobileNo);
		RBuffer.Append(iTempMbNum);		
		RBuffer.Append(_L("\"/>"));
	}

	GetSettingsString();
	RBuffer.Append(_L("<param name=\"settings\" value=\""));
	RBuffer.Append(iSettingsString);
	RBuffer.Append(_L("\"/>"));
	
	RBuffer.Append(_L("</user-prof>"));

	RBuffer.Append(_L("<sys-prof>"));
	
	RBuffer.Append(_L("</sys-prof>"));
	
	RBuffer.Append(_L("</b></root>"));

#ifndef WAP_PT
	HBufC8* content_data;
	content_data=RBuffer.AllocL();

	TBuf8<10> content_l;
	content_l.Num(content_data->Length());

	RBuffer.Copy(_L("GET "));
	RBuffer.Append(iStatusUrl);
	//RBuffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
	RBuffer.Append(_L(" HTTP/1.0\r\nHOST:"));
	RBuffer.Append(iServerAddr);
	RBuffer.Append(_L("\r\nPORT:"));
	RBuffer.AppendNum(iPort);
	RBuffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));


	RBuffer.Append(content_l);
	RBuffer.Append(_L("\r\n\r\n"));
	RBuffer.Append(*content_data);
#endif

	servlet_data = RBuffer.AllocL();
	
#ifdef __LOGME__	
	LogText.Copy(_L("\nCheckSubscriptionStatus 2. iServerAddr="));
	WriteLogFile(LogText);

	LogText.Copy(iServerAddr);
	WriteLogFile(LogText);

#endif

	ConnectToServerForSubscription(aStatus);

	//WriteLogFile(RBuffer);

	//LogText.Copy(_L("\n"));
	//WriteLogFile(LogText);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ConnectToServerForSubscription(TInt aStatus)
// Connects to server for Status Subscription
// ---------------------------------------------------------

void CRippleVaultAppUi::ConnectToServerForSubscription(TInt aStatus)
{
#ifdef __LOGME__
	LogText.Copy(_L("ConnectToServerForSubscription\n"));
	WriteLogFile(LogText);
#endif

	if(aStatus == 1)
	{
#ifdef __LOGME__
		LogText.Copy(_L("iSocketWriterPublishStatus 1\n"));
		WriteLogFile(LogText);
#endif
		if(iSocketWriterPublishStatus == NULL)
		{
			iSocketWriterPublishStatus = CSocketsEngineWriter::NewL(*this);
			iSocketWriterPublishStatus->SetServerName(iServerAddr);
			iSocketWriterPublishStatus->SetPort(iPort);
			iSocketWriterPublishStatus->iBuffer.Copy(*servlet_data);
			iSocketWriterPublishStatus->SubscriptionStatus(aStatus);		
			delete servlet_data;
		}
		//LogText.Copy(_L("iSocketWriterPublishStatus 2\n"));
		//WriteLogFile(LogText);
	}
	else if(aStatus == 2 || aStatus == 6)
	{
#ifdef __LOGME__		
		LogText.Copy(_L("iSocketWriterContactsStatus 1\n"));
		WriteLogFile(LogText);
#endif

		if(iSocketWriterContactsStatus == NULL)
		{
			iSocketWriterContactsStatus = CSocketsEngineWriter::NewL(*this);
			iSocketWriterContactsStatus->SetServerName(iServerAddr);
			iSocketWriterContactsStatus->SetPort(iPort);
			iSocketWriterContactsStatus->iBuffer.Copy(*servlet_data);
			iSocketWriterContactsStatus->SubscriptionStatus(aStatus);		
			delete servlet_data;
		}
		//LogText.Copy(_L("iSocketWriterContactsStatus 2\n"));
		//WriteLogFile(LogText);
	}
	else if(aStatus == 3)
	{
#ifdef __LOGME__
		LogText.Copy(_L("iSocketWriterSmsStatus 1\n"));
		WriteLogFile(LogText);
#endif

		if(iSocketWriterSmsStatus == NULL)
		{
			iSocketWriterSmsStatus = CSocketsEngineWriter::NewL(*this);
			iSocketWriterSmsStatus->SetServerName(iServerAddr);
			iSocketWriterSmsStatus->SetPort(iPort);
			iSocketWriterSmsStatus->iBuffer.Copy(*servlet_data);
			iSocketWriterSmsStatus->SubscriptionStatus(aStatus);		
			delete servlet_data;
		}
		//LogText.Copy(_L("iSocketWriterSmsStatus 2\n"));
		//WriteLogFile(LogText);
	}
	else if(aStatus == 4)
	{
#ifdef __LOGME__
		LogText.Copy(_L("iSocketWriterSimChangeStatus 1\n"));
		WriteLogFile(LogText);
#endif

		if(iSocketWriterSimChangeStatus == NULL)
		{
			iSocketWriterSimChangeStatus = CSocketsEngineWriter::NewL(*this);
			iSocketWriterSimChangeStatus->SetServerName(iServerAddr);
			iSocketWriterSimChangeStatus->SetPort(iPort);
			iSocketWriterSimChangeStatus->iBuffer.Copy(*servlet_data);
			iSocketWriterSimChangeStatus->SubscriptionStatus(aStatus);		
			delete servlet_data;
		}
	}
	else if(aStatus == 7)
	{
#ifdef __LOGME__
		LogText.Copy(_L("iSocketWriterAutoSmsStatus 1\n"));
		WriteLogFile(LogText);
#endif

		if(iSocketWriterAutoSmsStatus == NULL)
		{
			iSocketWriterAutoSmsStatus = CSocketsEngineWriter::NewL(*this);
			iSocketWriterAutoSmsStatus->SetServerName(iServerAddr);
			iSocketWriterAutoSmsStatus->SetPort(iPort);
			iSocketWriterAutoSmsStatus->iBuffer.Copy(*servlet_data);
			iSocketWriterAutoSmsStatus->SubscriptionStatus(aStatus);		
			delete servlet_data;
		}
		//LogText.Copy(_L("iSocketWriterSmsStatus 2\n"));
		//WriteLogFile(LogText);
	}
	/*else if(aStatus == 5)
	{
		if(iSocketWriterBillingStatus == NULL)
		{
			iSocketWriterBillingStatus = CSocketsEngineWriter::NewL(*this);
			iSocketWriterBillingStatus->SetServerName(iServerAddr);
			iSocketWriterBillingStatus->SetPort(iPort);
			iSocketWriterBillingStatus->iBuffer.Copy(*servlet_data);
			iSocketWriterBillingStatus->BillingStatus(aStatus);		
			delete servlet_data;
		}
	}*/

}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSmsXML(TInt smsbackuptype)
// gets the SMS backup XML
// ---------------------------------------------------------

void CRippleVaultAppUi::GetSmsXML(TInt smsbackuptype)
{
	TInt network;		
	network = CheckNetwork();
	if(network == NETWORK_ROAMING)
	{
		if(iSmsRoam == DISABLED)
		{
			return;
		}
		
	}

	iAutoSmsBackupType = smsbackuptype;
	//ConnectToServerForSms(smsbackuptype);
	iConnectionStatus = 7;
	CheckSubscriptionStatus(iConnectionStatus);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::UploadSmsManually(TInt smsbackuptype)
// gets the SMS backup XML
// ---------------------------------------------------------

void CRippleVaultAppUi::UploadSmsManually(TInt smsbackuptype)
{
	if(iWriteSmsAO != NULL)
	{
		delete iWriteSmsAO;
		iWriteSmsAO = NULL;
	}

	TInt network;		
	network = CheckNetwork();
	if(network == NETWORK_ROAMING)
	{
		if(iSmsRoam == DISABLED)
		{
			RoamingDisabledDialogSms();
			return;
		}
		
	}

	iSmsBackupType = smsbackuptype;
	//ConnectToServerForSms(smsbackuptype);
	iConnectionStatus = 3;
	CheckSubscriptionStatus(iConnectionStatus);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::ConnectToServerForSms(TInt smsbackuptype)
// Connects to server for SMS upload
// ---------------------------------------------------------

void CRippleVaultAppUi::ConnectToServerForSms(TInt smsbackuptype)
{
if(smsbackuptype)
{
	if(iSocketWriterAutoSMS == NULL)
	{	
		iSocketWriterAutoSMS = CSocketsEngineWriter::NewL(*this);
	    iSocketWriterAutoSMS->SetServerName(iServerAddr);
	    iSocketWriterAutoSMS->SetPort(iPort);
	    iSocketWriterAutoSMS->SendSmsData(smsbackuptype);
	}
}
else
{
	if(iSocketWriterSMS == NULL)
	{	
		iSocketWriterSMS = CSocketsEngineWriter::NewL(*this);
	    iSocketWriterSMS->SetServerName(iServerAddr);
	    iSocketWriterSMS->SetPort(iPort);
	    iSocketWriterSMS->SendSmsData(smsbackuptype);
	}
}
}

// ---------------------------------------------------------
// CRippleVaultAppUi::UpdateSmsTimestamp()
// Updates SMS last synch timestamp
// ---------------------------------------------------------

/*void CRippleVaultAppUi::UpdateInboxSmsTimestamp()
{
	iPhotosDB->UpdateSmsTimestampToDB();
}*/

void CRippleVaultAppUi::UpdateInboxSmsTimestamp(TTime aTime)
{
	iPhotosDB->UpdateSmsTimestampToDB(aTime);
}

void CRippleVaultAppUi::UpdateSentItemsSmsTimestamp(TTime aTime)
{
	iPhotosDB->UpdateSentItemsSmsTimestampToDB(aTime);
}

void CRippleVaultAppUi::GetSentItemsSmsTimestamp(TTime& aTime)
{
	iPhotosDB->GetSentItemsSmsTimestampFromDB(aTime);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSmsTimestamp(TTime& aTime)
// Gets SMS last synch timestamp
// ---------------------------------------------------------

void CRippleVaultAppUi::GetInboxSmsTimestamp(TTime& aTime)
{
	iPhotosDB->GetSmsTimestampFromDB(aTime);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SaveSubStatusSyncTime(TDesC& aTime)
// Saves Subscription Status last synch time to DB
// ---------------------------------------------------------

void CRippleVaultAppUi::SaveSubStatusSyncTime(TDesC& aTime)
{
	iPhotosDB->SaveSubStatusSyncTimeToDB(aTime);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSubStatusSyncTime(TDesC& aTime)
// Gets Subscription Status last synch time from DB
// ---------------------------------------------------------

void CRippleVaultAppUi::GetSubStatusSyncTime(TDesC& aTime)
{
	iPhotosDB->GetSubStatusSyncTimeFromDB(aTime);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SaveSelectiveSmsMails(TDesC& aNames, TDesC& aMails, TInt8 aCount)
// saves recipients for Selective SMS
// ---------------------------------------------------------

void CRippleVaultAppUi::SaveSelectiveSmsMails(TDesC& /*aNames*/, TDesC& /*aMails*/, TInt8 aCount)
{
	TBuf<150> names;
	TBuf<150> mobileno;

	for(int i=0; i< aCount ;i++)
	{
		GetSharingNames(names);		
		
		GetSharingMails(mobileno);				

		iPhotosDB->SaveSelectiveSmsToDB(names, mobileno);
	}    
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSmsMobileList()
// gets list of mobile numbers for Selective SMS
// ---------------------------------------------------------

CDesCArray* CRippleVaultAppUi::GetSmsMobileList()
{
    return iPhotosDB->GetSmsMobileList();
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSmsNameList()
// get list of recipients for Selective SMS
// ---------------------------------------------------------

CDesCArray* CRippleVaultAppUi::GetSmsNameList()
{
    return iPhotosDB->GetSmsNameList();
}

// ---------------------------------------------------------
// CRippleVaultAppUi::DeleteContactsForSms(TInt pos)
// deletes recipients for Selective SMS
// ---------------------------------------------------------

void CRippleVaultAppUi::DeleteContactsForSms(TInt pos)
{
	iPhotosDB->DeleteContactsForSmsFromDB(pos);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::UploadSms()
// Uploads SMS ie. automatic backup after timer expires
// ---------------------------------------------------------

void CRippleVaultAppUi::UploadSms()
{
	TInt val=1;
	if(iSmsBackupTimer != NULL)
	{
		iSmsBackupTimer->UploadSms(val);
	}
	else
	{
		iSmsBackupTimer = CSmsBackupTimer::NewL(*this);
		iSmsBackupTimer->UploadSms(val);
	}
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSMSProtocolDataXML(TDes8& buffer, TInt aMaxLen, TInt aSmstype)
// Gets SMS XML
// ---------------------------------------------------------

void CRippleVaultAppUi::GetSMSProtocolDataXML(TDes8& buffer, TInt /*aMaxLen*/, TInt aSmstype)
{

		TInt len = 0;
        TInt ret;

		if(aSmstype)
			iFAutoSMSsession.Connect();
		else
			iFSMSsession.Connect();

		TBuf<50> FileName;
		FileName = CRippleVaultAppUi::ApplicationDriveAndPath();
		if(aSmstype)
			FileName.Append(_L("smsbackup.aut"));
		else
			FileName.Append(_L("smsbackup.man"));

		//ret = iFileDes.Open(iFsession, filename->Des(), EFileRead|EFileStream);
		if(aSmstype)
			ret = iFileAutoSMSDes.Open(iFAutoSMSsession, FileName, EFileRead|EFileStream);
		else
			ret = iFileSMSDes.Open(iFSMSsession, FileName, EFileRead|EFileStream);

		if (ret == KErrNone)
		{
			if(aSmstype)
				iFileAutoSMSDes.Size(len);
			else
				iFileSMSDes.Size(len);
			TBuf8<10> imglen;			
			imglen.Num(len);
			//TBuf<200> smstempbuf;
			//RBuffer.Copy(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
			buffer.Copy(_L("GET "));
			buffer.Append(iSmsUrl);
		//	buffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
			buffer.Append(_L(" HTTP/1.0\r\nHOST:"));
			buffer.Append(iServerAddr);
			buffer.Append(_L("\r\nPORT:"));
			buffer.AppendNum(iPort);
			buffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));

			buffer.Append(imglen);
			buffer.Append(_L("\r\n\r\n"));
			//smsBuffer.Append();

			//buffer = smsBuffer.AllocL();

		}
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetMoreSMSDataFromFile(TDes8& aData, TInt aMaxLen)
// gets data from the SMS backup file
// ---------------------------------------------------------

TInt CRippleVaultAppUi::GetMoreSMSDataFromFile(TDes8& aData, TInt aMaxLen, TInt aSmsType)
{
    //if (iFileDes.
    // read data from filedesc into aData

    // return the length of data read
	TInt ret;

	if(aSmsType)
		ret = iFileAutoSMSDes.Read(aData, aMaxLen);
	else
		ret = iFileSMSDes.Read(aData, aMaxLen);
	if(ret != KErrNone || ret == KErrOverflow || ret == KErrGeneral)
	{
		return 0;
	}
    if (ret == 0)
    {
		return aData.Length();
    }
	return 0;
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SendSmsFileComplete(TInt aSmstype)
// Sms Upload completed successfully
// ---------------------------------------------------------

void CRippleVaultAppUi::SendSmsFileComplete(TInt aSmstype)
{
		if(aSmstype != 0)
		{
#ifdef __LOGME__
			LogText.Copy(_L("SendSmsFileComplete 44\n"));
			WriteLogFile(LogText);
#endif
			
			UpdateInboxSmsTimestamp(iSmsBackupTimer->iLastInboxSms);
			UpdateSentItemsSmsTimestamp(iSmsBackupTimer->iLastSentItemsSms);
		}

	NetworkErrorNotification(0);
	//iPhotosDB->CheckAndIncreaseStatusCount(albumname, publishtype);
/*	if(aSmstype != 2)
	{
		TInt count;
		GetSmsUploadSettingsFromDB(count);
		count+=iSmsCount;
		SaveSmsUploadSettingsToDB(count);
	}*/

	iUploadSms += (iAutoSmsCount + iSmsCount);

if(aSmstype)
	iAutoSmsCount = 0;
else	
	iSmsCount=0;

	TTime btime;
	btime.HomeTime();

	iPhotosDB->UpdateBackupTimestampToDB(btime);

	//UpdateScreen();

		TBuf<50> FileName;
		FileName = CRippleVaultAppUi::ApplicationDriveAndPath();
		if(aSmstype){
			FileName.Append(_L("smsbackup.aut"));
			iFileAutoSMSDes.Close();
			iFAutoSMSsession.Delete(FileName);

			iFAutoSMSsession.Close();
		}
		else{
			FileName.Append(_L("smsbackup.man"));
			iFileSMSDes.Close();
			iFSMSsession.Delete(FileName);

			iFSMSsession.Close();

		}

if(aSmstype)
{
	if (iSocketWriterAutoSMS != NULL)
	{
		iSocketWriterAutoSMS->Disconnect();	
		delete iSocketWriterAutoSMS;
		iSocketWriterAutoSMS = NULL;
	}
}
else{
	if (iSocketWriterSMS != NULL)
	{
		iSocketWriterSMS->Disconnect();	
		delete iSocketWriterSMS;
		iSocketWriterSMS = NULL;
	}
}
	CloseConnection();

	if(aSmstype == 2)		
		iSmsBackupTimer->UploadSms(3); //selective sms
	else if(aSmstype == 1 || aSmstype == 5)
		iSmsBackupTimer->UploadSms(2);
	/*if(iSmsBackupAO != NULL)
	{
		delete iSmsBackupAO;
		iSmsBackupAO = NULL;
	}*/
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SendSmsFileError(TInt aSmstype)
// Sms Upload failed due to an error
// ---------------------------------------------------------

void CRippleVaultAppUi::SendSmsFileError(TInt aSmstype)
{
		TBuf<50> FileName;
		FileName = CRippleVaultAppUi::ApplicationDriveAndPath();
		if(aSmstype){
			FileName.Append(_L("smsbackup.aut"));
			iFileAutoSMSDes.Close();
			iFAutoSMSsession.Delete(FileName);

			iFAutoSMSsession.Close();
			
			iAutoSmsCount = 0;
		}
		else{
			FileName.Append(_L("smsbackup.man"));
			iFileSMSDes.Close();
			iFSMSsession.Delete(FileName);

			iFSMSsession.Close();
			iSmsCount = 0;

		}

if(aSmstype)
{
	if (iSocketWriterAutoSMS != NULL)
	{
		iSocketWriterAutoSMS->Disconnect();
		
		delete iSocketWriterAutoSMS;
		iSocketWriterAutoSMS = NULL;
	}
}
else
{
	if (iSocketWriterSMS != NULL)
	{
		iSocketWriterSMS->Disconnect();
		
		delete iSocketWriterSMS;
		iSocketWriterSMS = NULL;
	}
}
	/*if(iSmsBackupAO != NULL)
	{
		delete iSmsBackupAO;
		iSmsBackupAO = NULL;
	}*/

	if(!aSmstype || aSmstype == 5)
	{
		if( iSmsBackupTimer == NULL)
		{
			iSmsBackupTimer = CSmsBackupTimer::NewL(*this);
			iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMSBACKUP_INBOXSMS)));
			
			delete iSmsBackupTimer;
			iSmsBackupTimer = NULL;
		}
		else
		{
			iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMSBACKUP_INBOXSMS)));
		}

		//UpdateScreen();
	}

}

// ---------------------------------------------------------
// CRippleVaultAppUi::SendSubStatusError(TInt aStatus)
// Subscription Status failed due to an error
// ---------------------------------------------------------

void CRippleVaultAppUi::SendSubStatusError(TInt aStatus)
{
/*	if(aStatus == 1)
	{
		//InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_PUBLISHBACKUP_INBOXSMS)));	
		if( iSmsBackupTimer == NULL)
		{
			iSmsBackupTimer = CSmsBackupTimer::NewL(*this);
			iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_PUBLISHBACKUP_INBOXSMS)));
			
			delete iSmsBackupTimer;
			iSmsBackupTimer == NULL;
		}
		else
		{
			iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMSBACKUP_INBOXSMS)));
		}
	}
	else if(aStatus == 2)
	{
		//InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_CONTACTSBACKUP_INBOXSMS)));	
		if( iSmsBackupTimer == NULL)
		{
			iSmsBackupTimer = CSmsBackupTimer::NewL(*this);
			iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_CONTACTSBACKUP_INBOXSMS)));
			
			delete iSmsBackupTimer;
			iSmsBackupTimer == NULL;
		}
		else
		{
			iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMSBACKUP_INBOXSMS)));
		}
	}*/
	//else 
	if(aStatus == 3 && iSmsBackupType == 0)
	{
		iSmsCount = 0;

		//InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMSBACKUP_INBOXSMS)));	
		if( iSmsBackupTimer == NULL)
		{
			iSmsBackupTimer = CSmsBackupTimer::NewL(*this);
			iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMSBACKUP_INBOXSMS)));
			
			delete iSmsBackupTimer;
			iSmsBackupTimer = NULL;
		}
		else
		{
			iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMSBACKUP_INBOXSMS)));
		}

		/*if( iSmsBackupAO != NULL)
		{
			delete iSmsBackupAO;
   			iSmsBackupAO=NULL;
		}*/
		//UpdateScreen();
	}
	else if(aStatus == 7)
	{
		iAutoSmsCount = 0;
		if(iAutoSmsBackupType == 5)
		{
			if( iSmsBackupTimer == NULL)
			{
				iSmsBackupTimer = CSmsBackupTimer::NewL(*this);
				iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMSBACKUP_INBOXSMS)));
				
				delete iSmsBackupTimer;
				iSmsBackupTimer = NULL;
			}
			else
			{
				iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_SMSBACKUP_INBOXSMS)));
			}
		}
	}
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetChoice(TInt8& aChoice)
// Gets flag which tells if setup wizard has been completed
// ---------------------------------------------------------

void CRippleVaultAppUi::GetChoice(TInt8& aChoice)
{
	iPhotosDB->GetChoice(aChoice);
	/*TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KSetupWizardFile);

	RFs fs1;

	fs1.Connect();
	
	if(BaflUtils::FileExists(fs1, path)) //setup wizard
	{	
		aChoice = 0;
	}
	else
	{
		aChoice = 1;
	}

	fs1.Close();
*/
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SaveChoice(TInt8 aChoice)
// Saves flag after setup wizard is complete
// ---------------------------------------------------------

void CRippleVaultAppUi::SaveChoice(TInt8 aChoice)
{
	iPhotosDB->SaveChoice(aChoice);
	/*TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KSetupWizardFile);

	RFs fs1;

	fs1.Connect();
	
	if(BaflUtils::FileExists(fs1, path)) //setup wizard
	{	
		if(aChoice)
			BaflUtils::DeleteFile(fs1, path);
	}

	fs1.Close();*/
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CallSetupWizardStep1(){
// Setup Wizard
// ---------------------------------------------------------
/*
void CRippleVaultAppUi::CallSetupWizardStep1(){

	iSetupWizardScreenValue = 1;
		//TBuf<1000> msg;
		//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_CONTACTS)));

		//TBuf<60> tembuf;
		//tembuf.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_SETUP_WIZARD)));
		//tembuf.Append(_L(" 1/3"));
#ifdef __UIQ__
		//CEikonEnv::Static()->InfoWinL(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_3)),*(iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_CONTACTS)));
#ifdef __S80__
		CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_3)), *iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_CONTACTS));
#else		
		CEikDialog* dialog;
		dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_CONTACTS)), R_SETUP_WIZARD_STEP_3);
		dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif
		//{
		CallSetupWizardStep4();
		//}
#else
		CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_CONTACTS)));
		abtdlg->PrepareLC(R_MESSAGE_INFO);
		abtdlg->SetHeaderTextL(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_3))); 

		if (abtdlg->RunLD()){
			CallSetupWizardStep4();
		}
#endif
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CallSetupWizardStep4()
// Setup Wizard
// ---------------------------------------------------------

void CRippleVaultAppUi::CallSetupWizardStep4(){
#ifdef __UIQ__	
	CEikDialog* contactsdialog;	
	contactsdialog = new (ELeave) CContactsSettingDialog(*this);
	contactsdialog->ExecuteLD(R_CONTACTS_SETTINGS);	

	CallSetupWizardStep5();
#else
	iSettingsScreenValue = 1;
	ActivateView(KViewIdEmptyScreen);
	ActivateView(KViewIdSetupWizard);
#endif
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CallSetupWizardStep5()
// Setup Wizard
// ---------------------------------------------------------

void CRippleVaultAppUi::CallSetupWizardStep5(){
	
	if(iSetupWizardScreenValue < 5)
	{
		RFs fs;
		fs.Connect();
		
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KContactSetupWizard);
		
		if (BaflUtils::FileExists(fs, path)) 
		{
			EnableContactsUpload(NOT_REGISTERED);

			BaflUtils::DeleteFile(fs, path);
		}

		fs.Close();
	}

	iSetupWizardScreenValue = 5;

		//TBuf<1000> msg;
		//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_CONTACTS_4)));

		//TBuf<60> tembuf;
		//tembuf.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_SETUP_WIZARD)));
		//tembuf.Append(_L(" 1/3"));
#ifdef __UIQ__
		//CEikonEnv::Static()->InfoWinL(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_4)),*(iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_PUBLISH)));
#ifdef __S80__
		CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_4)), *iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_PUBLISH));
#else		
		CEikDialog* dialog;
		dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_PUBLISH)), R_SETUP_WIZARD_STEP_4);
		dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif
		CallSetupWizardStep9();
#else
		CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_PUBLISH)));
		abtdlg->PrepareLC(R_MESSAGE_INFO);
		abtdlg->SetHeaderTextL(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_4))); 

		if (abtdlg->RunLD()){
			CallSetupWizardStep9();
		}
		else{
			//CallSetupWizardStep3();
		}
#endif		
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CallSetupWizardStep9()
// Setup Wizard
// ---------------------------------------------------------

void CRippleVaultAppUi::CallSetupWizardStep9(){

	CheckAndSaveFolderToDB(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));

#ifdef __UIQ__	
	CEikDialog* photosdialog;	
	photosdialog = new (ELeave) CPhotosSettingDialog(*this);
	photosdialog->ExecuteLD(R_PHOTOS_SETTINGS);	

	CallSetupWizardStep11();
#else

	iSettingsScreenValue = 2;
	ActivateView(KViewIdEmptyScreen);
	ActivateView(KViewIdSetupWizard);
#endif
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CallSetupWizardStep11()
// Setup Wizard
// ---------------------------------------------------------

void CRippleVaultAppUi::CallSetupWizardStep11(){
		//TBuf<50> tempbuf;
		//tempbuf.Copy(_L("Backup"));							
		//CheckAndSaveFolderToDB(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));		
#ifdef __UIQ__	
#ifdef __S80__
	if(CCknConfirmationDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_5)),*(iEikonEnv->AllocReadResourceL(R_TEXT_UPLOAD_PUBLISH))))
#else
	CEikDialog* dialog;
	dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_TEXT_UPLOAD_PUBLISH)), R_SETUP_WIZARD_STEP_5);
	if(dialog->ExecuteLD(R_EIK_DIALOG_YESNO_INFO))
#endif
	{
		iSettingsScreenValue = 0;
		iImageFlag = 1;
		ShowWindow(KViewIdPhotoList); //photo screen
		DeleteWindow(KViewIdRegisterScreen); //main screen
	}
	else
	{
		CallSetupWizardStep12();
	}
#else
	CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_TEXT_UPLOAD_PUBLISH)));
	abtdlg->PrepareLC(R_GALLERY_INFO);		
	abtdlg->SetHeaderTextL(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_5))); 

	if (abtdlg->RunLD())
	{
		iRipplePhotos = 1;
				
		iStoragePath = 0;									
		ActivateView(KViewIdPhotoList);
	}
	else
		CallSetupWizardStep12();
#endif
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CallSetupWizardStep12()
// Setup Wizard
// ---------------------------------------------------------

void CRippleVaultAppUi::CallSetupWizardStep12(){
	iSetupWizardScreenValue = 12;
		//TBuf<1000> msg;
		//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_SMS)));

		//TBuf<60> tembuf;
		//tembuf.Copy(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_6)));
		//tembuf.Append(_L(" 3/3"));
#ifdef __UIQ__
		//CEikonEnv::Static()->InfoWinL(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_6)),*(iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_SMS)));
#ifdef __S80__
		CCknInfoDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_6)), *iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_SMS));
#else		
		CEikDialog* dialog;
		dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_SMS)), R_SETUP_WIZARD_STEP_6);
		dialog->ExecuteLD(R_EIK_DIALOG_CONTINUE_INFO);
#endif		
		CallSetupWizardStep15();
#else
		CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_SETUPMESSAGE_SMS)));
		abtdlg->PrepareLC(R_MESSAGE_INFO);
		abtdlg->SetHeaderTextL(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_6))); 

		if (abtdlg->RunLD()){
			CallSetupWizardStep15();
		}
		else{
			//CallSetupWizardStep10();
		}
#endif
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CallSetupWizardStep15()
// Setup Wizard
// ---------------------------------------------------------

void CRippleVaultAppUi::CallSetupWizardStep15(){
#ifdef __UIQ__	
	CEikDialog* smssettingdialog;	
	smssettingdialog = new (ELeave) CSmsSettingDialog(*this);
	TInt err = smssettingdialog->ExecuteLD(R_SMS_SETTINGS);
	if(iSmsMode == 0)
	{
		MailNames.Copy(_L(""));
		MailId.Copy(_L(""));
		albumName.Copy(_L(""));
		iMessage.Copy(_L(""));
		iStoragePath = 1;

		ShowWindow(KViewIdContactsViewScreen); 
		DeleteWindow(KViewIdRegisterScreen);
	}
	else{
		CallSetupWizardStep17();
	}
#else
	iSettingsScreenValue = 3;
	ActivateView(KViewIdEmptyScreen);
	ActivateView(KViewIdSetupWizard);
#endif
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CallSetupWizardStep17()
// Setup Wizard
// ---------------------------------------------------------

void CRippleVaultAppUi::CallSetupWizardStep17(){
	TInt rid;

	if(iSmsMode == SMSBACKUP_SELECTIVE)
		rid = R_SETUPWIZARD_SMSBACKUPALL_SELECTIVE;
	else if(iSmsMode == SMSBACKUP_SCHEDULED || iSmsMode == SMSBACKUP_SCHEDULED_HOURLY)
		rid = R_SETUPWIZARD_SMSBACKUPALL_AUTO;
	else
		rid = R_TEXT_UPLOAD_SMS;

#ifdef __UIQ__
#ifdef __S80__
	if(CCknConfirmationDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_7)),*(iEikonEnv->AllocReadResourceL(rid))))
#else
	CEikDialog* dialog;
	dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(rid)), R_SETUP_WIZARD_STEP_7);
	if(dialog->ExecuteLD(R_EIK_DIALOG_YESNO_INFO))
#endif
	{
		iSettingsScreenValue = 0;
		//TTime aTime;
		//aTime.HomeTime();

		//UpdateInboxSmsTimestamp(aTime);
		//UpdateSentItemsSmsTimestamp(aTime);

		if(iSmsMode == SMSBACKUP_SELECTIVE || iSmsMode == SMSBACKUP_SCHEDULED || iSmsMode == SMSBACKUP_SCHEDULED_HOURLY)
		{
			TInt val=1;
			iSmsBackupTimer->UploadSms(val);
			SaveChoice(1);
			iMainScreenValue = 6;
#ifdef __S80__
			ShowWindow(KViewIdStatusScreen); //status screen
			if(iRegisterView != NULL)
			{
				iRegisterView->ShowStatusScreen();
			}
#else
			ShowWindow(KViewIdStatusScreen); //status screen
			DeleteWindow(KViewIdRegisterScreen);
			ShowStatusScreen();
#endif
		}
		else
		{
			iImageFlag = 5;
			ShowWindow(KViewIdSmsList); //sms screen
			DeleteWindow(KViewIdRegisterScreen); //main screen
		}

	}
	else
	{
		if(iSmsMode == SMSBACKUP_SELECTIVE || iSmsMode == SMSBACKUP_SCHEDULED || iSmsMode == SMSBACKUP_SCHEDULED_HOURLY)
		{
			TTime aTime;
			aTime.HomeTime();

			UpdateInboxSmsTimestamp(aTime);
			UpdateSentItemsSmsTimestamp(aTime);

		}
		
		SaveChoice(1);
		iMainScreenValue = 6;
#ifdef __S80__
		ShowWindow(KViewIdStatusScreen); //status screen
		if(iRegisterView != NULL)
		{
			iRegisterView->ShowStatusScreen();
		}
#else
		ShowWindow(KViewIdStatusScreen); //status screen
		DeleteWindow(KViewIdRegisterScreen);
		ShowStatusScreen();
#endif
	}
#else
	CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(rid)));
	abtdlg->PrepareLC(R_GALLERY_INFO);		
	abtdlg->SetHeaderTextL(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_7))); 

	if (abtdlg->RunLD())
	{
		//TTime aTime;
		//aTime.HomeTime();

		//UpdateInboxSmsTimestamp(aTime);
		//UpdateSentItemsSmsTimestamp(aTime);

		if(iSmsMode == SMSBACKUP_SELECTIVE || iSmsMode == SMSBACKUP_SCHEDULED || iSmsMode == SMSBACKUP_SCHEDULED_HOURLY)
		{
			TInt val=1;
			iSmsBackupTimer->UploadSms(val);
			SaveChoice(1);
			ActivateView(KViewIdStatusScreen);
			//ActivateView(KViewIdMainScreen);
		}
		else
		{
			iStoragePath = 0;
			ActivateView(KViewIdEmptyScreen);
			ActivateView(KViewIdSmsList);
		}
	}
	else
	{
		if(iSmsMode == SMSBACKUP_SELECTIVE || iSmsMode == SMSBACKUP_SCHEDULED || iSmsMode == SMSBACKUP_SCHEDULED_HOURLY)
		{
			TTime aTime;
			aTime.HomeTime();

			UpdateInboxSmsTimestamp(aTime);
			UpdateSentItemsSmsTimestamp(aTime);

		}
		SaveChoice(1);
		//ActivateView(KViewIdMainScreen);
		ActivateView(KViewIdStatusScreen);
	}
#endif
}
*/

// ---------------------------------------------------------
// CRippleVaultAppUi::LoadParamsFromConfigFile()
// Loads parameters from the config file
// ---------------------------------------------------------

void CRippleVaultAppUi::LoadParamsFromConfigFile()
{
	TBuf<60> tmpstr;
	
	tmpstr.Copy( _L("version"));	
	LoadFromConfigFile(iVersion,tmpstr);
	
	tmpstr.Copy(_L("os"));	
	LoadFromConfigFile(iOs, tmpstr);
	
	tmpstr.Copy(_L("phone_model"));	
	LoadFromConfigFile(iAxiomPhoneModel, tmpstr);

	tmpstr.Copy(_L("billing_reg_type"));	
	LoadFromConfigFile(iAxiomBillingRegType, tmpstr);

	tmpstr.Copy( _L("userid"));	
	LoadFromConfigFile(iUserNames,tmpstr);
	
	tmpstr.Copy(_L("hp"));	
	LoadFromConfigFile(iMobileNo,tmpstr );
	
	tmpstr.Copy(_L("password"));
	LoadFromConfigFile(iPassword,tmpstr );

	//tmpstr.Copy(_L("charge"));
	//LoadFromConfigFile(iCharge,tmpstr );

	tmpstr.Copy(_L("registration_url"));	
	LoadFromConfigFile(iRegUrl,tmpstr );

	tmpstr.Copy(_L("media_pkt_upload_url"));	
	LoadFromConfigFile(iPublishUrl,tmpstr );

	tmpstr.Copy(_L("sharing_url"));	
	LoadFromConfigFile(iSharingUrl,tmpstr );

	tmpstr.Copy(_L("contacts_url"));	
	LoadFromConfigFile(iContactsUrl,tmpstr );

	tmpstr.Copy(_L("sms_backup_url"));	
	LoadFromConfigFile(iSmsUrl,tmpstr );

	tmpstr.Copy(_L("apn"));	
	LoadFromConfigFile(iAccessPoints,tmpstr );	

	tmpstr.Copy(_L("sub_update_url"));	
	LoadFromConfigFile(iPlanUrl,tmpstr );	

	tmpstr.Copy(_L("album_synch_url"));	
	LoadFromConfigFile(iAlbumUrl,tmpstr );	
	
	tmpstr.Copy(_L("sub_status_check"));	
	LoadFromConfigFile(iStatusUrl,tmpstr );	

	tmpstr.Copy(_L("connection_type"));	
	LoadFromConfigFile(iConnectionType,tmpstr );	

	//MapAPNtoActualName();
	
//	tmpstr.Copy(_L("chargeclient"));	
//	LoadFromConfigFile(iChargeClient,tmpstr );
//	iChargeClient.Copy(_L("/rpc/chargerpc/3000/downloadclient"));
	
//	tmpstr.Copy(_L("chargephoto"));	
	//LoadFromConfigFile(iChargePhoto,tmpstr );
//	iChargePhoto.Copy(_L("/rpc/chargerp/"));
//	iChargePhoto.Append(iCharge);
//	iChargePhoto.Append(_L("00/uploadphoto"));

//	tmpstr.Copy(_L("chargevideo"));	
//	LoadFromConfigFile(iChargeVideo,tmpstr );
//	iChargeVideo.Copy(_L("/rpc/chargerp/"));
//	iChargeVideo.Append(iCharge);
//	iChargeVideo.Append(_L("00/uploadvideo"));

	tmpstr.Copy(_L("media_gallery_url"));	
	LoadFromConfigFile(iWapUrl,tmpstr );

	tmpstr.Copy(_L("gateway"));	
	LoadFromConfigFile(iGateWay,tmpstr );

	tmpstr.Copy(_L("settings"));
	LoadFromConfigFile(iSettingsString,tmpstr );

	iUserName.Copy(iUserNames);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SendSmsForClientSharing()
// Sends SMS for "Tell a Friend" recommendation
// ---------------------------------------------------------

void CRippleVaultAppUi::SendSmsForClientSharing()
{
	TBuf<150> sharingName;
	TBuf<150> mess;
	TInt8 sharingCount;

	TBuf<300> smstext;
	TBuf<300> formatBuf;
	
	//formatBuf.Copy(iUserNames);
	smstext.Append(*(iEikonEnv->AllocReadResourceL(R_TEXT_TELL_FRIEND)));

	formatBuf.Format(smstext, &iUserNames);
	smstext.Copy(formatBuf);

	GetMails(albumName,MailNames,MailId, mess, sharingCount);

	//for(int i=0; i< sharingCount ;i++)
	//{
		GetSharingMails(sharingName);		
		
		iSmsFile->CmdSendL(sharingName,smstext,1);
	//}	
	
	//iPhotosDB->DeleteSharingsFirstRecord();
}

// ---------------------------------------------------------
// CRippleVaultAppUi::EditConfigFile(TDes8& aValue, TDesC8& aParam)
// edits config file
// ---------------------------------------------------------

void CRippleVaultAppUi::EditConfigFile(TDes8& aValue, TDesC8& aParam)
{
	RFs fs,fs1;
	RFile file,file1;
	
	//_LIT(KConfigFile,"config.txt");
	_LIT(KConfigFile1,"tempconfig.txt");

	TBuf8<150> tempbuf;

	fs.Connect();
	fs1.Connect();

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KConfigFile);

	TBuf<128> path1;
	path1 = CRippleVaultAppUi::ApplicationDriveAndPath();
	path1.Append(KConfigFile1);

	if (BaflUtils::FileExists(fs, path))
	{
		TInt err = file.Open(fs, path, EFileRead);
		
		TInt err1 = file1.Open(fs1, path1, EFileWrite);

		if(err1 != KErrNone)
			file1.Create(fs1, path1, EFileWrite);
		
		if (err == KErrNone) 
		{
			TBuf8<2> buf;
			TBuf<2> buf16;
			TBuf<150> line;
			TBuf<150> param;
			
			param.Copy(aParam);
			param.Append(_L("="));

			TInt len = param.Length();

			line.Copy(_L(""));

			TInt count = 0;
			do 
			{
				file.Read(buf, 1);

				if (buf.Length())
					count++;

				buf16.Copy(buf);
				line.Append(buf16);

				if (buf.Length() == 0 || buf.Find(_L8("\n")) != KErrNotFound ||
				buf.Find(_L8("\r")) != KErrNotFound) 
				{
					if (line.Find(param) == KErrNotFound)
					{
						tempbuf.Copy(line);
						file1.Write(tempbuf);
					}
					else
					{
						tempbuf.Copy(_L(""));
					
						tempbuf.Append(aParam);
						tempbuf.Append(_L("="));
						tempbuf.Append(aValue);
						tempbuf.Append(_L("\n"));

						file1.Write(tempbuf);
					}

					if (line.Find(param) != KErrNotFound && line.Length() > len) 
					{
						line.Delete(0,len);
						//line.SetLength(count - 7);
						if (buf.Find(_L8("\n")) != KErrNotFound || buf.Find(_L8("\r")) != KErrNotFound)
						{
							line.SetLength(line.Length() - 1);
						}						
					}
					line.Copy(_L(""));
					count = 0;
				}
			}
			while(buf.Length() != 0);
		}
	}
	
	file.Close();
	file1.Close();
	
	fs.Delete(path);	
	BaflUtils::RenameFile(fs1, path1, path,0);
	
	fs.Close();	
	fs1.Close();
}

// ---------------------------------------------------------
// CRippleVaultAppUi::UpLoadDatas()
// Enables contacts, photos/videos and sms upload
// ---------------------------------------------------------

void CRippleVaultAppUi::UpLoadDatas()
{
	if(iSmsValue == -1)
	{
		//GetConnectionName();

		EnableUpload(iMode);
		EnableSmsUpload(iSmsMode);

		if(iContactsFlag == 0)
		{
			TTime aCDBTime, aRippleTime;

			//added by Deepak
				TBuf<128> path;
				path = CRippleVaultAppUi::ApplicationDriveAndPath();
				path.Append(KInitUploadFile);

				RFs fs1;

				fs1.Connect();
				
				//delete file
				//fs1.Delete(KRegStep1);

			if(BaflUtils::FileExists(fs1, path))
			{
				EnableContactsUpload(NOT_REGISTERED);
			}
			else if (ChangesToBeStoredL(aCDBTime, aRippleTime))
			{
				iChangesAO = new (ELeave) CChangesContactsAO(*this);
				iChangesAO->StartL(aCDBTime, aRippleTime);
			}else{		
				if(iContactsHandler->iContactsUpload == 0)
				{
					//iContactsHandler->SyncToServer();			
					iConnectionStatus = 2;
					CheckSubscriptionStatus(iConnectionStatus);
				}
			}

			EnableContactsUpload(REGISTERED);
		}
	}
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SaveUserSettingsToDB(TDes& aUserName, TDes& aPassWord, TDes& aMobileNo)
// Saves username, password and phone number to DB
// ---------------------------------------------------------

void CRippleVaultAppUi::SaveUserSettingsToDB(TDes& aUserName, TDes& aPassWord, TDes& aMobileNo)
{
	iUserName.Copy(aUserName);
	iUserNames.Copy(iUserName);
	iPassword.Copy(aPassWord);
	iMobileNo.Copy(aMobileNo);
    iPhotosDB->SaveUserSettingsL(aUserName, aPassWord, aMobileNo);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetUserSettings(TDes& aUserName, TDes& aPassWord, TDes& aMobileNo)
// Gets username, password and phone number from DB
// ---------------------------------------------------------

void CRippleVaultAppUi::GetUserSettings(TDes& aUserName, TDes& aPassWord, TDes& aMobileNo)
{
    iPhotosDB->GetUserSettings(aUserName, aPassWord, aMobileNo);
}

// ---------------------------------------------------------
// CRippleVaultAppUi::CheckUserId()
// Checks userid/password details
// ---------------------------------------------------------

TInt CRippleVaultAppUi::CheckUserId()
{	
	TBuf<50> userid;
	TBuf<50> password;
	
	userid.Copy(_L(""));
	password.Copy(_L(""));

#ifndef __UIQ__
	CAknMultiLineDataQueryDialog* dialog =	CAknMultiLineDataQueryDialog::NewL(userid, password);
	if(dialog->ExecuteLD(R_CONTACTS_DIALOG_REGISTERED))
	{
		if(userid.Compare(iUserNames)==0 && password.Compare(iPassword)==0)  
			return 1; 
		else 
			return 0;
	}
	return -1;
#else
	CEikDialog* simdialog;	
	simdialog = new (ELeave) CCheckSimDialog(*this);
	if(simdialog->ExecuteLD(R_REGISTER_DIALOG))
	{
		return iSettingsScreenValue;
	}
	else
		return -1;
	
#endif	
}

// ---------------------------------------------------------
// CRippleVaultAppUi::UpdateSimcard()
// Updates SIM card details after SIM card change validation
// ---------------------------------------------------------

void CRippleVaultAppUi::UpdateSimcard()
{
	iMobileNo.Copy(iTempMbNum);

	SaveUserSettingsToDB(iUserNames, iPassword, iMobileNo);

	WriteImsiNumber();
	
	UpLoadDatas();
	
	TBuf8<150> aValue;
	TBuf8<150> aParam;
	
	aParam.Copy(_L("userid"));
	aValue.Copy(iUserNames);	
	EditConfigFile(aValue, aParam);

	aParam.Copy(_L("hp"));	
	
	TBuf<50> tgt;	
	tgt.Copy(iTempMbNum);

	TInt64 num;	
	TLex lex(tgt);
	lex.Val(num);

	num = num + 10101010;
	tgt.NumUC(num,EHex); 	
	aValue.Copy(tgt);

	EditConfigFile(aValue, aParam);

	/*TBuf<60> tmpstr;	
	tmpstr.Copy( _L("userid"));	

	LoadFromConfigFile(iUserNames,tmpstr);
				
	iUserName.Copy(iUserNames);

	if(iUserName.Compare(_L("")) == 0)
		ReadUserName();

	tmpstr.Copy(_L("hp"));	
	LoadFromConfigFile(iMobileNo,tmpstr );*/
}

// ---------------------------------------------------------
// CRippleVaultAppUi::GetSettingsString()
// Prepares the Settings string to send to server
// ---------------------------------------------------------

void CRippleVaultAppUi::GetSettingsString()
{
	iSettingsString.Copy(_L(""));

	TInt ports;
	TInt autostart;
	TInt contactsroam;
	TInt modes;
	TInt desc;
	TInt roam;
	TInt securegallery;
    TInt smstype;
	TTime time;
	TInt smsroam;
	TInt upload;

	GetSettingsFromDB(iServerAddr, ports, autostart);
	GetContactsSettingsFromDB(contactsroam, upload);
	GetPhotosSettingsFromDB(modes, desc, roam, securegallery);	
	GetSmsSettingsFromDB(smstype, time, smsroam);

	iSettingsString.AppendNum(autostart);
	iSettingsString.AppendNum(contactsroam);
	iSettingsString.AppendNum(modes);
	iSettingsString.AppendNum(desc);
	iSettingsString.AppendNum(roam);
	iSettingsString.AppendNum(securegallery);
	iSettingsString.AppendNum(smstype);	
	iSettingsString.AppendNum(smsroam);

	iSettingsString.Append(_L("|"));
	TBuf<30> dateString;
	_LIT(KDateString5,"%-B%:0%J%:1%T%:2%S%:3%+B");
	time.FormatL(dateString,KDateString5);	
		iSettingsString.Append(dateString);
	iSettingsString.Append(_L("|"));
}

// ---------------------------------------------------------
// CRippleVaultAppUi::SaveSettingsString()
// Saves from Settings String to DB
// ---------------------------------------------------------

void CRippleVaultAppUi::SaveSettingsString()
{
	CheckAndSaveFolderToDB(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));

	if(iSettingsString.Length() == 0)
		return;
/*
		RFs fs;
		fs.Connect();
		
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KContactSetupWizard);
		
		if (BaflUtils::FileExists(fs, path)) 
		{
			EnableContactsUpload(NOT_REGISTERED);

			BaflUtils::DeleteFile(fs, path);
		}

		fs.Close();
*/
	//ShowMessageBoxFromResource(R_SECUREWIZARD_SKIP);

	TBuf<1> t;
	t.Copy(&iSettingsString[0],1);

	if(t.Find(_L("0"))!=KErrNotFound)
	{
		iAutostart = 0;
	}
	else
	{
		iAutostart = 1;
	}

	t.Copy(&iSettingsString[1],1);

	if(t.Find(_L("0"))!=KErrNotFound)
	{
		iContactsHandler->iContactsRoaming = 0;
	}
	else
	{
		iContactsHandler->iContactsRoaming = 1;
	}

	t.Copy(&iSettingsString[2],1);

	if(t.Find(_L("0"))!=KErrNotFound)
	{
		iMode = 0;
		SetDefaultFolder(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));
	}
	else if(t.Find(_L("1"))!=KErrNotFound)
	{
		iMode = 1;
	}
	else
	{ 
		iMode = 2;
	}

	t.Copy(&iSettingsString[3],1);

	if(t.Find(_L("0"))!=KErrNotFound)
	{
		iDescription = 0;
	}
	else
	{
		iDescription = 1;
	}

	t.Copy(&iSettingsString[4],1);

	if(t.Find(_L("0"))!=KErrNotFound)
	{
		iRoaming = 0;
	}
	else
	{
		iRoaming = 1;
	}

	t.Copy(&iSettingsString[5],1);

	if(t.Find(_L("0"))!=KErrNotFound)
	{
		iSecureGallery = 0;
	}
	else
	{
		iSecureGallery = 1;
	}

	t.Copy(&iSettingsString[6],1);

	if(t.Find(_L("0"))!=KErrNotFound)
	{
		iSmsMode = 0;
	}
	else if(t.Find(_L("1"))!=KErrNotFound)
	{
		iSmsMode = 1;
	}
	else if(t.Find(_L("2"))!=KErrNotFound)
	{ 
		iSmsMode = 2;
	}
	else
	{ 
		iSmsMode = 3;
	}

	t.Copy(&iSettingsString[7],1);

	if(t.Find(_L("0"))!=KErrNotFound)
	{
		iSmsRoam = 0;
	}
	else
	{
		iSmsRoam = 1;
	}

	TBuf<30> dateString;
	dateString.Copy(&iSettingsString[9], iSettingsString.Length() - 10);

	TInt returnvalue=iSmsTime.Parse(dateString); 


	SaveSettingsToDB(iServerAddr, iPort, iAutostart);
	SaveContactsSettingsToDB(iContactsHandler->iContactsRoaming, iContactsHandler->iContactsUpload);
	SavePhotosSettingsToDB(iMode, iDescription, iRoaming, iSecureGallery);
	SaveSmsSettingsToDB(iSmsMode,iSmsTime,iSmsRoam);

}

void CRippleVaultAppUi::RoamingDisabledDialogSms()
{
	ShowMessageBoxFromResource(R_TEXT_ROAMINGDISABLED_SMS);
}

void CRippleVaultAppUi::RoamingDisabledDialogPublish()
{
	ShowMessageBoxFromResource(R_TEXT_ROAMINGDISABLED_PUBLISH);
}

void CRippleVaultAppUi::RoamingDisabledDialogContacts()
{
	ShowMessageBoxFromResource(R_TEXT_ROAMINGDISABLED_CONTACTS);
}

void CRippleVaultAppUi::CloseConnection()
{
#ifdef __LOGME__
	LogText.Copy(_L("CloseConnection\n"));
	WriteLogFile(LogText);
#endif

#ifdef WAP_PT
	//LogText.Copy(_L("CloseConnection\n"));
	//WriteLogFile(LogText);

	if( iSocketWriterPublish == NULL && iSocketWriterShare == NULL
		&& iSocketWriterSMS == NULL	&& iSocketWriterPublishStatus == NULL && iSocketWriterContactsStatus == NULL 
		&& iSocketWriterSmsStatus == NULL && iSocketWriterSimChangeStatus == NULL
		&& iSocketWriterAutoSMS == NULL && iSocketWriterAutoSmsStatus == NULL
		&& iContactsHandler->iSocketsEngine == NULL && iSocketWriterPassword == NULL )
	{
		CheckSuccessfullSms();

		if(iConnection == 1)
		{
			RGenericAgent netAgent;		
			TBool active;
				
			if (netAgent.Open() == KErrNone)
			{
				CleanupClosePushL(netAgent);
				
				netAgent.NetworkActive(active);

				if(active)
				{
					//LogText.Copy(_L("Stopped Connection\n"));
					//WriteLogFile(LogText);

					iConnection = 0;

					TRAPD(err, netAgent.Stop());
					if(err==KErrNone)
					{
					}
					else{
					}

				}		
				CleanupStack::PopAndDestroy(); // netAgent
			}	
		}
	}
#endif

	if( iSocketWriterPublish == NULL /*&& iSocketWriterShare == NULL*/ 
		&& iSocketWriterSMS == NULL	&& iSocketWriterPublishStatus == NULL && iSocketWriterContactsStatus == NULL 
		&& iSocketWriterSmsStatus == NULL /*&& iSocketWriterSimChangeStatus == NULL*/
		&& iSocketWriterAutoSMS == NULL && iSocketWriterAutoSmsStatus == NULL
		&& iContactsHandler->iSocketsEngine == NULL /*&& iSocketWriterPassword == NULL*/ )
	{
		CheckSuccessfullSms();
		NetworkStatusNotification(NETWORK_STATUS_RUNNING);
	}
	else if(iContactsHandler->iSocketsEngine != NULL || iSocketWriterContactsStatus != NULL)
	{
		NetworkStatusNotification(NETWORK_STATUS_UPLOADING_CONTACTS);
	}
	else if(iSocketWriterPublish != NULL || iSocketWriterPublishStatus != NULL)
	{
		NetworkStatusNotification(NETWORK_STATUS_UPLOADING_MEDIA);
	}
	else if(iSocketWriterSMS != NULL || iSocketWriterSmsStatus != NULL || iSocketWriterAutoSMS != NULL || iSocketWriterAutoSmsStatus != NULL)
	{
		NetworkStatusNotification(NETWORK_STATUS_UPLOADING_SMS);
	}

}


void CRippleVaultAppUi::SaveSecureSettingsToFile(TInt8 aSecureMode, TDes& aFriendNumber, TDes& aText, TInt8 aAlarmMode)
{
#ifndef __S80__
	RFs fs;
	fs.Connect();

	RFile file;
	TBuf8<200> name;
	//TInt err;

	TBuf<128> path;

	path.Copy(_L("c:\\system\\secure\\secure.txt"));

	name.Copy(_L(""));

	name.Copy(_L("autolock="));
	name.AppendNum(aSecureMode);
	name.Append(_L("\n"));

	name.Append(_L("fpn="));
	name.Append(aFriendNumber);
	name.Append(_L("\n"));

	name.Append(_L("text="));
	name.Append(aText);
	name.Append(_L("\n"));

	name.Append(_L("alarm="));
	name.AppendNum(aAlarmMode);
	name.Append(_L("\n"));

	if (BaflUtils::FileExists(fs, path)) 
	{
		BaflUtils::DeleteFile(fs, path);
	}

	file.Create(fs, path, EFileWrite);
	file.Write(name);

	file.Close();
	fs.Close();	

	TTime oldtime;
	TTime newtime;

	iPhotosDB->GetSecureSettingsTimestampFromDB(oldtime, newtime);

	oldtime = newtime;
	newtime.UniversalTime();
	iPhotosDB->UpdateSecureSettingsTimestampToDB(oldtime, newtime);
#endif
}

void CRippleVaultAppUi::GetSecureSettingsFromFile(TInt& aSecureMode, TDes& aFriendNumber,  TDes& aText, TInt& aAlarmMode)
{
	TBuf<60> tmpstr;
	TBuf<20> mode;	

	tmpstr.Copy(_L("autolock"));			
	LoadFromSecureFile(mode,tmpstr );
	
	if(mode.Compare(_L("1")) == 0)
		aSecureMode = 1;
	else
		aSecureMode = 0;	

	tmpstr.Copy(_L("fpn"));			
	LoadFromSecureFile(aFriendNumber,tmpstr );

	tmpstr.Copy(_L("text"));			
	LoadFromSecureFile(aText,tmpstr );

	tmpstr.Copy(_L("alarm"));			
	LoadFromSecureFile(mode,tmpstr );
	
	if(mode.Compare(_L("1")) == 0)
		aAlarmMode = 1;
	else
		aAlarmMode = 0;	
}

TBool CRippleVaultAppUi::LoadFromSecureFile(TDes& aValue, TDesC& aParam)
{
	RFs fs;
	RFile file;

	TBool ret = FALSE;

	fs.Connect();
	TBuf<128> path;

	path.Copy(_L("c:\\system\\secure\\secure.txt"));

	if (BaflUtils::FileExists(fs, path))
	{
		TInt err = file.Open(fs, path, EFileRead);
		if (err == KErrNone) 
		{
			TBuf8<2> buf;
			TBuf<2> buf16;
			TBuf<150> line;
			TBuf<150> param;

			param.Copy(aParam);
			param.Append(_L("="));

			TInt len = param.Length();

			line.Copy(_L(""));

			TInt count = 0;
			do 
			{
				/*TInt readErr = */ file.Read(buf, 1);
				if (buf.Length())
				count++;

				buf16.Copy(buf);
				line.Append(buf16);

				if (buf.Length() == 0 || buf.Find(_L8("\n")) != KErrNotFound ||
				buf.Find(_L8("\r")) != KErrNotFound) 
				{
					if (line.Find(param) != KErrNotFound && line.Length() > len) 
					{
						line.Delete(0,len);
						//line.SetLength(count - 7);
						if (buf.Find(_L8("\n")) != KErrNotFound || buf.Find(_L8("\r")) != KErrNotFound)
						{
							line.SetLength(line.Length() - 1);
						}
						
						aValue.Copy(line);
						
						ret = TRUE;
						break;
					}
					line.Copy(_L(""));
					count = 0;
				}
			}
			while(buf.Length() != 0);
		}
		file.Close();
	}
	fs.Close();
	return ret;
}

TInt CRippleVaultAppUi::CheckLockFile()
{	
	RFs fs;
	fs.Connect();
	TInt val = 0;

	TBuf<128> path;

	path.Copy(_L("c:\\system\\secure\\lock.dat"));

	if (BaflUtils::FileExists(fs, path)) 
	{
		val = 1;

		User::Exit(EEikCmdExit);
	}
	else
	{
		TInt secureStart;
		TInt alarmStart;
		TBuf<20>friendNumber;
		TBuf<200> secureText;
	
		GetSecureSettingsFromFile(secureStart, friendNumber, secureText, alarmStart);

		TInt ret = ReadSecureImsiNumber();
		//TInt ret1 = BaflUtils::FileExists(fs, path);

		if(ret != 2)
		if(secureStart == 1 && ret == 0)
		{
			val = 1;

			User::Exit(EEikCmdExit);
		}
	}

	fs.Close();
	
	return val;
}

#ifdef __UIQ__
#ifdef __S80__

// ---------------------------------------------------------
// CRippleVaultAppUi::SaveSelectiveSmsMails(TDesC& aNames, TDesC& aMails, TInt8 aCount)
// saves recipients for Selective SMS
// ---------------------------------------------------------

void CRippleVaultAppUi::SaveSelectiveSmsMailsForS80(TDesC& aNames, TDesC& aMails)
{
	iPhotosDB->SaveSelectiveSmsToDB(aNames, aMails);    
}

void CRippleVaultAppUi::ShowWindow(TInt view)
{
	if(view == 0)//unregister view
	{
		TRect rect = ClientRect();	
		rect.iBr.iX = rect.iTl.iX + rect.Width();

		if(iUnRegisterView == NULL)
		{
			iUnRegisterView = CUnRegisterView::NewL( rect, *this);	
			AddToStackL(iUnRegisterView);

			iCBA->SetCommandSetL(R_RIPPLEVAULT_CBA);
			iCBA->DrawNow();

			CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
			menuBar->ChangeMenuBarL(0, R_UNREG_MENUBAR, EFalse);
		}
	}
/*	else if(view == 1)//Register view
	{
		if(iRegisterView == NULL)
		{
			iRegisterView = CRegisterView::NewL(ClientRect(), *this);
			AddToStackL(iRegisterView);

			CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
			menuBar->ChangeMenuBarL(0, R_REGISTER_MENUBAR, EFalse);
			
			iCBA->SetCommandSetL(R_REGISTER_CBA);
			iCBA->DrawNow();
		}
	}*/
	else if(view == 2)//photolist view
	{
		if(iPhotoListView == NULL)
		{
			iPhotoListView = CPhotoListView::NewL(ClientRect(), *this);
			AddToStackL(iPhotoListView);
		
			iCBA->SetCommandSetL(R_PHOTO_CBA);
			iCBA->DrawNow();

			CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
			menuBar->ChangeMenuBarL(0, R_PHOTOLIST_MENUBAR, EFalse);
		}
	}
	else if(view == 3)//sms listview
	{
		if(iSmsListView == NULL)
		{
			iSmsListView = CSmsListView::NewL(ClientRect(), *this);
			AddToStackL(iSmsListView);
		
			iCBA->SetCommandSetL(R_SMS_CBA);
			iCBA->DrawNow();

			CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
			menuBar->ChangeMenuBarL(0, R_SMSLIST_MENUBAR, EFalse);
		}
	}
	else if(view == 4)//status screen view
	{
//		TRect rect = ClientRect();	
//		rect.iTl.iX = rect.iTl.iX + rect.Width()/3;
		
//		rect.iTl.iY = rect.iTl.iY + 5;

		TRect rect = ClientRect();	
		rect.iBr.iX = rect.iTl.iX + rect.Width();

		if( iStatusScreenView == NULL)
		{
			iStatusScreenView = CStatusScreenView::NewL( rect, *this);
			AddToStackL(iStatusScreenView);
			
			//iRegisterView->ShowStatusScreen();
			ShowStatusScreen();

			CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
			menuBar->ChangeMenuBarL(0, R_STATUSSCREEN_MENUBAR, EFalse);
		}
	}
	else if(view == 5)//contacts screen view
	{
		if(iContactsListView == NULL)
		{
			iContactsListView = CContactsView::NewL(ClientRect(), *this);
			AddToStackL(iContactsListView);
		
			iCBA->SetCommandSetL(R_CONTACTS_CBA);
			
			iCBA->DimCommand(EContactsDel, ETrue);
			iCBA->DimCommand(EContactsAdd, EFalse);

			if(iStoragePath == 1)
				iCBA->DimCommand(EContactsSend, ETrue);

			iCBA->DrawNow();
		}
	}
	else if(view == 6)//videolist view
	{
		if(iVideoListView == NULL)
		{
			iVideoListView = CVideoListView::NewL(ClientRect(), *this);
			AddToStackL(iVideoListView);
		
			iCBA->SetCommandSetL(R_PHOTO_CBA);
			iCBA->DrawNow();

			CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
			menuBar->ChangeMenuBarL(0, R_PHOTOLIST_MENUBAR, EFalse);
		}
	}
}
/*
void CRippleVaultAppUi::ActivateWindow()
{
	TInt err;
	CEikDialog* dialog;	

	TInt view;
	view = iRegisterView->GetMainScreenCurrentItemIndex();

	TInt view1;
	view1 = iRegisterView->GetMainSubScreenCurrentItemIndex();

	if(view1 == -1) //focus is on mainscreen
	{
		iRegisterView->SelectSubScreen();
	}
	else if(view == 0 && view1 == 0)//show contacts view contacts
	{
		ShowMessageBoxFromResource(R_TEXT_FEATURE_CONSTRUCTION_VIEW_CONTACTS);
	}
	else if (view == 0 && view1 == 1) //show send to web
	{
		iConnectionStatus = 2;
		CheckSubscriptionStatus(iConnectionStatus);
		ShowMessageBoxFromResource(R_TEXT_CONTACTS_SENDTOWEB);
		//iRegisterView->ShowStatusScreen();
		ShowWindow(KViewIdStatusScreen);
		//DeleteWindow(1);
	}
	else if (view == 0 && view1 == 2) //show contacts settings
	{
		dialog = new (ELeave) CContactsSettingDialog(*this);
		err = dialog->ExecuteLD(R_CONTACTS_SETTINGS);			
	}
	else if(view == 1 && view1 == 0)//show photo view gallery
	{
		TInt ret=0;
		
		if(iSecureGallery)
			//ret = CheckUserId();
			ret = PasswordDialog();
		else
			ret = 1;

		if(ret == 1)
			OpenBrowser();
	}
	else if(view == 1 && view1 == 1)//show photo list
	{
		ShowWindow(KViewIdPhotoList);
		DeleteWindow(KViewIdRegisterScreen);
	}
	else if(view == 1 && view1 == 2)//show share album for photos
	{
		MailNames.Copy(_L(""));
		MailId.Copy(_L(""));
		albumName.Copy(_L(""));
		iMessage.Copy(_L(""));
		iStoragePath = 2;

		ShowWindow(KViewIdContactsViewScreen);
		DeleteWindow(KViewIdRegisterScreen);
	}
	else if(view == 1 && view1 == 3)//show unshare album for photos
	{
		dialog = new (ELeave) CUnShareAlbumDialog(*this);
		dialog->ExecuteLD(R_FOLDER_DIALOG1);
	}
	else if(view == 1 && view1 == 4)//show photo settings
	{
		dialog = new (ELeave) CPhotosSettingDialog(*this);
		err = dialog->ExecuteLD(R_PHOTOS_SETTINGS);	
		//ShowPhotosSettingsDialog();
	}
	else if(view == 2 && view1 == 0)//show video view gallery
	{
		TInt ret=0;
		
		if(iSecureGallery)
			//ret = CheckUserId();
			ret = PasswordDialog();
		else
			ret = 1;

		if(ret == 1)
			OpenBrowser();
	}
	else if(view == 2 && view1 == 1)//show video list
	{
		ShowWindow(KViewIdVideoList);
		DeleteWindow(KViewIdRegisterScreen);
	}
	else if(view == 2 && view1 == 2)//show share album for videos
	{
		MailNames.Copy(_L(""));
		MailId.Copy(_L(""));
		albumName.Copy(_L(""));
		iMessage.Copy(_L(""));
		iStoragePath = 2;

		ShowWindow(KViewIdContactsViewScreen);
		DeleteWindow(KViewIdRegisterScreen);
	}
	else if(view == 2 && view1 == 3)//show unshare album for videos
	{
		dialog = new (ELeave) CUnShareAlbumDialog(*this);
		dialog->ExecuteLD(R_FOLDER_DIALOG1);
	}
	else if(view == 2 && view1 == 4)//show video settings
	{
		dialog = new (ELeave) CPhotosSettingDialog(*this);
		err = dialog->ExecuteLD(R_PHOTOS_SETTINGS);	
		//ShowPhotosSettingsDialog();
	}
	else if(view == 3 && view1 == 0)//show sms view messages
	{
		ShowMessageBoxFromResource(R_TEXT_FEATURE_CONSTRUCTION_VIEW_MESSAGES);
	}
	else if(view == 3 && view1 == 1)//show sms list
	{
		if(PendingSMS())
		{
			ShowMessageBoxFromResource(R_SMS_SENDTOWEB_BUSY);

			ShowWindow(KViewIdStatusScreen);
		}
		else
		{
			ShowWindow(KViewIdSmsList);
			DeleteWindow(KViewIdRegisterScreen);
		}
	}
	else if(view == 3 && view1 == 2)//show sms settings
	{
		dialog = new (ELeave) CSmsSettingDialog(*this);
		err = dialog->ExecuteLD(R_SMS_SETTINGS);	
		
		if(err)
		if(iSmsMode == 0)
		{
			MailNames.Copy(_L(""));
			MailId.Copy(_L(""));
			albumName.Copy(_L(""));
			iMessage.Copy(_L(""));
			iStoragePath = 1;

			ShowWindow(KViewIdContactsViewScreen);
			DeleteWindow(KViewIdRegisterScreen); 
		}
	}

}
*/

void CRippleVaultAppUi::DeleteWindow(TInt val)
{
	if(val == 0)
	{
		if (iUnRegisterView)
		{
			RemoveFromStack(iUnRegisterView);
			delete iUnRegisterView;
			iUnRegisterView = NULL;
		}
	}
/*	else if(val == 1)
	{
		if (iRegisterView)
		{
			RemoveFromStack(iRegisterView);
			delete iRegisterView;
			iRegisterView = NULL;
		}
	}*/	
	else if(val == 2)
	{
		if (iPhotoListView)
		{
			RemoveFromStack(iPhotoListView);
			delete iPhotoListView;
			iPhotoListView = NULL;
		}
	}
	else if(val == 3)
	{
		if (iSmsListView)
		{
			RemoveFromStack(iSmsListView);
			delete iSmsListView;
			iSmsListView = NULL;
		}
	}	
	else if(val == 4)
	{
		if (iStatusScreenView)
		{
			RemoveFromStack(iStatusScreenView);
			delete iStatusScreenView;
			iStatusScreenView = NULL;
		}
		//iCBA->SetCommandSetL(R_REGISTER_CBA);
		//iCBA->DrawNow();
	}	
	else if(val == 5)
	{
		if (iContactsListView)
		{
			RemoveFromStack(iContactsListView);
			delete iContactsListView;
			iContactsListView = NULL;
		}
	}	
	else if(val == 6)
	{
		if (iVideoListView)
		{
			RemoveFromStack(iVideoListView);
			delete iVideoListView;
			iVideoListView = NULL;
		}
	}
}

TInt CRippleVaultAppUi::ReturnScreenValue()
{
	TInt val;
	////
	if (iUnRegisterView)
	{
		val = 0;
	}
/*	else if (iRegisterView)
	{
		val = 1;
	}*/
	else if (iPhotoListView)
	{
		val = 2;
	}
	else if (iSmsListView)
	{
		val = 3;
	}
	else if (iStatusScreenView)
	{
		val = 4;
	}
	else if (iContactsListView)
	{
		val = 5;
	}
	else if (iVideoListView)
	{
		val = 6;
	}

	return val;
}

#else
void CRippleVaultAppUi::ShowWindow(TInt aVal)
{
	if(aVal == 0)//unregister view
	{
		if(iUnRegisterView == NULL)
		{
			iUnRegisterView = CUnRegisterView::NewL( ClientRect(), *this);	
			AddToStackL(iUnRegisterView);

			CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
			appUi->SetToolbarL(R_UNREGISTER_TOOLBAR);

			CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
			menuBar->ChangeMenuBarL(0, R_UNREG_MENUBAR, EFalse);
		}
	}
	/*else if(aVal == 1)//MainScreen view
	{
		if(iMainScreenView == NULL)
		{
			iMainScreenView = CMainScreenView::NewL(ClientRect(), *this);
			AddToStackL(iMainScreenView);	
			AddToStackL(iMainScreenView->ListBox());	
			
			iMainScreenView->UpdateListBoxL(iMainScreenValue);
			
			CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
			
			if(iMainScreenValue == 0)
				appUi->SetToolbarL(R_REGISTER_TOOLBAR);
			else
				appUi->SetToolbarL(R_SCREENLIST_TOOLBAR);

			CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
			menuBar->ChangeMenuBarL(0, R_REGISTER_MENUBAR, EFalse);
		}
	}*/
	else if(aVal == 2)//photolist view
	{
		if(iPhotoListView == NULL)
		{
			iPhotoListView = CPhotoScreenView::NewL(ClientRect(), *this);
			AddToStackL(iPhotoListView);	
			AddToStackL(iPhotoListView->ListBox());	
			
			TInt err;
			if(iImageFlag == 1)
			{
				CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
				appUi->SetToolbarL(R_PHOTOLIST_TOOLBAR_PHONE);

				err = iPhotoListView->Start(KDirPhonePhoto);
			}
			else if(iImageFlag == 2)
			{
				CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
				appUi->SetToolbarL(R_PHOTOLIST_TOOLBAR_MEMORY);

				err = iPhotoListView->Start(KDirMMCPhoto);
			}

			CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
			menuBar->ChangeMenuBarL(0, R_PHOTOLIST_MENUBAR, EFalse);

			if(err == -1)
				ShowMessageBoxFromResource(R_WEBUPLOAD_NOPHOTOIMAGE);
		}
	}
	else if(aVal == 3)//videolist view
	{
		if(iVideoListView == NULL)
		{
			iVideoListView = CVideoScreenView::NewL(ClientRect(), *this);
			AddToStackL(iVideoListView);	
			AddToStackL(iVideoListView->ListBox());	

			TInt err;			
			if(iImageFlag == 3)
			{
				CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
				appUi->SetToolbarL(R_PHOTOLIST_TOOLBAR_PHONE);

				err = iVideoListView->Start(KDirPhoneVideo);
			}
			else if(iImageFlag == 4)
			{
				CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
				appUi->SetToolbarL(R_PHOTOLIST_TOOLBAR_MEMORY);

				err = iVideoListView->Start(KDirMMCVideo);
			}

			CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
			menuBar->ChangeMenuBarL(0, R_PHOTOLIST_MENUBAR, EFalse);
			
			if(err == -1)
				ShowMessageBoxFromResource(R_WEBUPLOAD_NOVIDEOIMAGE);
		}
	}
	else if(aVal == 4)//smslist view
	{	

		if(iSmsListView == NULL)
		{	
			iSmsListView = CSmsScreenView::NewL(ClientRect(), *this);
			AddToStackL(iSmsListView);	
			AddToStackL(iSmsListView->ListBox());	

			TInt err;			
			if(iImageFlag == 5)
			{
				CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
				appUi->SetToolbarL(R_SMSLIST_TOOLBAR_PHONE);

				//iSmsListView->iInboxItemsFlag = ETrue;
				err = iSmsListView->Start();
			}
			else if(iImageFlag == 6)
			{
				CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
				appUi->SetToolbarL(R_SMSLIST_TOOLBAR_MEMORY);

				//iSmsListView->iSentItemsFlag = ETrue;
				err = iSmsListView->Start();
			}

			CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
			menuBar->ChangeMenuBarL(0, R_SMSLIST_MENUBAR, EFalse);
			
			//if(err == -1)
				//ShowMessageBoxFromResource(R_WEBUPLOAD_NOSMS);
		}
	}
	else if(aVal == 5)//status screen view
	{
		if(iStatusScreenView == NULL)
		{
			iStatusScreenView = CStatusScreenView::NewL(ClientRect(), *this);
			AddToStackL(iStatusScreenView);		
			
			CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
			appUi->SetToolbarL(R_SCREENLIST_TOOLBAR);
			
			CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
			menuBar->ChangeMenuBarL(0, R_STATUSSCREEN_MENUBAR, EFalse);
		}
	}
	else if(aVal == 6)//contacts screen view
	{
		if(iContactsScreenView == NULL)
		{
			iContactsScreenView = CContactsScreenView::NewL(ClientRect(), *this);
			AddToStackL(iContactsScreenView);		
			
			CQikAppUi* appUi = STATIC_CAST(CQikAppUi*, iEikonEnv->AppUi());
			if(iStoragePath == 1)
				appUi->SetToolbarL(R_SELECTIVECONTACTSLIST_TOOLBAR);
			else
				appUi->SetToolbarL(R_CONTACTSLIST_TOOLBAR);
		}
	}
}

void CRippleVaultAppUi::DeleteWindow(TInt aVal)
{
	if(aVal == 0)
	{
		if (iUnRegisterView)
		{
			RemoveFromStack(iUnRegisterView);
			delete iUnRegisterView;
			iUnRegisterView = NULL;
		}
	}
	/*else if(aVal == 1)
	{
		if (iMainScreenView)
		{
		    RemoveFromStack(iMainScreenView->ListBox());
			RemoveFromStack(iMainScreenView);
			delete iMainScreenView;
			iMainScreenView = NULL;
		}
	}*/
	else if(aVal == 2)
	{
		if (iPhotoListView)
		{
		    RemoveFromStack(iPhotoListView->ListBox());
			RemoveFromStack(iPhotoListView);
			delete iPhotoListView;
			iPhotoListView = NULL;
		}
	}
	else if(aVal == 3)
	{
		if (iVideoListView)
		{
		    RemoveFromStack(iVideoListView->ListBox());
			RemoveFromStack(iVideoListView);
			delete iVideoListView;
			iVideoListView = NULL;
		}
	}
	else if(aVal == 4)
	{
		if (iSmsListView)
		{
		    RemoveFromStack(iSmsListView->ListBox());
			RemoveFromStack(iSmsListView);
			delete iSmsListView;
			iSmsListView = NULL;
		}
	}
	else if(aVal == 5)
	{
		if (iStatusScreenView)
		{
			RemoveFromStack(iStatusScreenView);
			delete iStatusScreenView;
			iStatusScreenView = NULL;
		}
	}
	else if(aVal == 6)
	{
		if (iContactsScreenView)
		{
			RemoveFromStack(iContactsScreenView);
			delete iContactsScreenView;
			iContactsScreenView = NULL;
		}
	}
}

TInt CRippleVaultAppUi::ReturnScreenValue()
{
	TInt val;

	if (iUnRegisterView)
	{
		val = 0;
	}
	/*else if (iMainScreenView)
	{
		val = 1;
	}*/
	else if (iPhotoListView)
	{
		val = 2;
	}
	else if (iVideoListView)
	{
		val = 3;
	}
	else if (iSmsListView)
	{
		val = 4;
	}
	else if (iStatusScreenView)
	{
		val = 5;
	}
	else if (iContactsScreenView)
	{
		val = 6;
	}

	return val;
}
#endif
#endif

void CRippleVaultAppUi::ShowSecure()
{
	//const TUid starter_uid1= { 0x102032D1 }; 

	RApaLsSession ls;

	TRAPD(errorconnect,ls.Connect());

	CleanupClosePushL(ls);

	_LIT(filen1, "c:\\system\\Secure\\Secure.app"); // dummy

	CApaCommandLine *cmdline = CApaCommandLine::NewLC(filen1);	
	TRAPD(error1, ls.StartApp(*cmdline));

	CleanupStack::PopAndDestroy(2);	///
}

void CRippleVaultAppUi::GetContactsParams(TInt &numRecords)
{
#ifdef __LOGME__
	//LogText.Copy(_L("<GetContactsParams>"));
	//WriteLogFile(LogText);
#endif

	RFs fs;
	RFile file;

	fs.Connect();

	TInt currRecord;

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KRegPath);

	TInt err = file.Open(fs, path, EFileRead);
	if (err == KErrNone) {

		TBuf8<20> state;
		//User::LeaveIfError(file.Read(state));
		file.Read(state);
		
		if(state.Length()!=0)
		{
			TBuf8<10> cr;
			TBuf8<10> nr;

			TInt pos = state.Find(_L8(","));

			cr.Copy(&state[0], pos);
			nr.Copy(&state[pos + 1], state.Length() - pos - 1);


			TLex8 lexc(cr);
			lexc.Val(currRecord);
			
			TLex8 lexn(nr);
			lexn.Val(numRecords);
		}
	}

	file.Close();
	fs.Close();

#ifdef __LOGME__
	//LogText.Copy(_L("</GetContactsParams>"));
	//WriteLogFile(LogText);
#endif
}

void CRippleVaultAppUi::NetworkErrorNotification(TInt aErrorcode)
{
#ifdef __LOGME__
	LogText.Copy(_L("<NetworkErrorNotification>"));
	WriteLogFile(LogText);
#endif

	TInt currentcode;
	iPhotosDB->GetNetworkErrorCode(currentcode);

	if (currentcode == 0 && aErrorcode !=0 && aErrorcode != NETWORK_ERRCODE_TIMEDOUT && aErrorcode != NETWORK_ERRCODE_PARSING && aErrorcode != NETWORK_ERRCODE_FAILED)
	{
		TTime t;
		t.HomeTime();

		TTimeIntervalHours hours(0);

		t.HoursFrom(iSmsNotif, hours);

	if(hours.Int()>=6)
	{/*
		if( iSmsBackupTimer == NULL)
		{
			iSmsBackupTimer = CSmsBackupTimer::NewL(*this);
			iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_NETWORK_ERROR)));
			
			delete iSmsBackupTimer;
			iSmsBackupTimer = NULL;
		}
		else
		{
			iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_NETWORK_ERROR)));
		}*/
		iSmsNotif.HomeTime();
	}
	}

	iPhotosDB->SaveNetworkErrorCode(aErrorcode);

	//UpdateScreen();

#ifdef __LOGME__
	LogText.Copy(_L("</NetworkErrorNotification>"));
	WriteLogFile(LogText);
#endif
}
/*
void CRippleVaultAppUi::UpdateScreen()
{
#ifndef __UIQ__
	if(iPrevViewId == KViewIdStatusScreen)
	{
		//iAppUi.ActivateView(KViewIdEmptyScreen);
		//iAppUi.ActivateView(KViewIdStatusScreen);
		if(iRegView != NULL)
			iRegView->UpdateScreen();
	}
#else
#ifdef __S80__
	if(iStatusScreenView != NULL)
	{
		ShowStatusScreen();
	}
#else
	if(ReturnScreenValue()==5)
	{
		iMainScreenValue = 6;
		ShowStatusScreen();
		//iAppUi.DeleteWindow(5);
	}
#endif
#endif

}
*/
void CRippleVaultAppUi::NetworkStatusNotification(TInt aErrorcode)
{
#ifdef __LOGME__
	LogText.Copy(_L("<NetworkStatusNotification>"));
	WriteLogFile(LogText);
#endif

	iPhotosDB->SaveNetworkStatusCode(aErrorcode);

	//UpdateScreen();

#ifdef __LOGME__
	LogText.Copy(_L("</NetworkStatusNotification>"));
	WriteLogFile(LogText);
#endif
}

TInt CRippleVaultAppUi::GetNetworkErrorCode(TDes& aErrorMsg)
{
#ifdef __LOGME__
	//LogText.Copy(_L("<GetNetworkErrorCode>"));
	//WriteLogFile(LogText);
#endif

	TInt code;
	iPhotosDB->GetNetworkErrorCode(code);

	switch(code)
	{
		case NETWORK_ERRCODE_PARSING:
			aErrorMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_SHORTTEXT_NETWORK_ERRCODE_FAILED)));
			break;
		case NETWORK_ERRCODE_FAILED:
			aErrorMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_SHORTTEXT_NETWORK_ERRCODE_FAILED)));
			break;
		case NETWORK_ERRCODE_TIMEDOUT:
			aErrorMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_SHORTTEXT_NETWORK_ERRCODE_FAILED)));
			break;
		case NETWORK_ERRCODE_MSISDNMISMATCH:
			aErrorMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_SHORTTEXT_NETWORK_ERRCODE_MSISDNMISMATCH)));
			break;
		case NETWORK_ERRCODE_IMEIMISMATCH:
			aErrorMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_SHORTTEXT_NETWORK_ERRCODE_IMEIMISMATCH)));
			break;
		case NETWORK_ERRCODE_ACCSUSPENDED:
			aErrorMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_SHORTTEXT_NETWORK_ERRCODE_ACCSUSPENDED)));
			break;
		default:
			aErrorMsg.Copy(_L(""));
			break;
	}				

#ifdef __LOGME__
	//LogText.Copy(_L("</GetNetworkErrorCode>"));
	//WriteLogFile(LogText);
#endif
	
	return code;
}

TInt CRippleVaultAppUi::GetNetworkStatusCode(TDes& aStatusMsg)
{
#ifdef __LOGME__
	//LogText.Copy(_L("<GetNetworkStatusCode>"));
	//WriteLogFile(LogText);
#endif

	TInt code;

	iPhotosDB->GetNetworkStatusCode(code);

	switch(code)
	{
		case NETWORK_STATUS_CONNECTING:
			aStatusMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_NETWORK_STATUS_CONNECTING)));
			break;
		case NETWORK_STATUS_UPLOADING_MEDIA:
			aStatusMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_NETWORK_STATUS_UPLOADING_MEDIA)));
			break;
		case NETWORK_STATUS_UPLOADING_SMS:
			aStatusMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_NETWORK_STATUS_UPLOADING_SMS)));
			break;
		case NETWORK_STATUS_UPLOADING_CONTACTS:
			aStatusMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_NETWORK_STATUS_UPLOADING_CONTACTS)));
			break;
		case NETWORK_STATUS_FAILED:
			aStatusMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_NETWORK_STATUS_FAILED)));
			break;
		case NETWORK_STATUS_SYNCHING_ALBUMS:
			aStatusMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_NETWORK_STATUS_SYNCHING_ALBUMS)));
			break;
		case NETWORK_STATUS_CHECKING_ACCOUNT:
			aStatusMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_NETWORK_STATUS_CHECKING_ACCOUNT)));
			break;
		case NETWORK_STATUS_RUNNING:
		default:
			{
			TBuf<128> path;
			path = CRippleVaultAppUi::ApplicationDriveAndPath();
			path.Append(KInitUploadFile);

			RFs fs;
			fs.Connect();
			if (BaflUtils::FileExists(fs, path)) 
			{
				code = NETWORK_STATUS_UPLOADING_CONTACTS;
				aStatusMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_NETWORK_STATUS_PROCESS_CONTACTS)));
			}
			else
				aStatusMsg.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_NETWORK_STATUS_RUNNING)));

			fs.Close();
			}
			break;
	}

#ifdef __LOGME__
	//LogText.Copy(_L("</GetNetworkStatusCode>"));
	//WriteLogFile(LogText);
#endif

	return code;
}

void CRippleVaultAppUi::CreateInitUploadFileL()
{
	RFs fs;
	RFile file;

	fs.Connect();

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KInitUploadFile);

	TInt err = file.Replace(fs,path ,EFileWrite);

	if (err == KErrNone) {
		file.Close();
	}

	fs.Close();
}

void CRippleVaultAppUi::CallSetupWizardUploadAll()
{
	TBuf<20> listselection;
	//listselection.Copy(_L("C1"));
	ShowSelectionList(listselection);

	SaveChoice(1);
	CheckAndSaveFolderToDB(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));

	//ShowMessageBoxFromResource(R_VAULT_MESSAGE);
	ShowMessageBoxFromResource(R_TEXT_BACKUP_MESSAGE);

	SendToBackground();

	iSettingsString.Copy(_L(""));
	TBuf<60> tmpstr;
	tmpstr.Copy(_L("settings"));
	LoadFromConfigFile(iSettingsString,tmpstr );

#ifdef __LOGME__
	LogText.Copy(_L("\nCallSetupWizardUploadAll list = "));
	LogText.Append(listselection);
	WriteLogFile(LogText);
#endif

	if(listselection.Find(_L("C1")) >= 0) //contacts upload
	{
#ifdef __LOGME__
	LogText.Copy(_L("\nCallSetupWizardUploadAll Contacts"));
	WriteLogFile(LogText);
#endif

		RFs fs;
		fs.Connect();
		
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KContactSetupWizard);
		
		if (BaflUtils::FileExists(fs, path)) 
		{
			EnableContactsUpload(NOT_REGISTERED);

			BaflUtils::DeleteFile(fs, path);
		}

		fs.Close();

#ifdef __LOGME__
	LogText.Copy(_L("\n//CallSetupWizardUploadAll Contacts"));
	WriteLogFile(LogText);
#endif

	}
	else //contacts not select. set to Manual Upload
	{

#ifdef __LOGME__
	LogText.Copy(_L("\nCallSetupWizardUploadAll DONTUPLOADCONTACTS"));
	WriteLogFile(LogText);
#endif

		SaveContactsSettingsToDB(2);

		RFs fs;
		fs.Connect();
		
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KContactSetupWizard);
		
		if (BaflUtils::FileExists(fs, path)) 
		{
			EnableContactsUpload(NOT_REGISTERED);

			BaflUtils::DeleteFile(fs, path);
		}

		fs.Close();
	}

	if(listselection.Find(_L("P1")) >= 0) //contacts upload
	{
		AddPhotos();
	}

	if(listselection.Find(_L("V1")) >= 0) //contacts upload
	{
		AddVideos();
	}

	if(listselection.Find(_L("S1")) >= 0) //contacts upload
	{
#ifdef __LOGME__
	LogText.Copy(_L("\nCallSetupWizardUploadAll Sms"));
	WriteLogFile(LogText);
#endif

	if(iSmsBackupTimer != NULL)
	{
		TInt val=2;
		iSmsBackupTimer->UploadSms(val);
	}
	else
	{
		iSmsBackupTimer = CSmsBackupTimer::NewL(*this);
		TInt val=2;
		iSmsBackupTimer->UploadSms(val);
	}

#ifdef __LOGME__
	LogText.Copy(_L("\n//CallSetupWizardUploadAll Sms"));
	WriteLogFile(LogText);
#endif

	}

	//TInt flag=0;

#ifdef __LOGME__
	LogText.Copy(_L("\nCallSetupWizardUploadAll\n"));
	WriteLogFile(LogText);
#endif
/*
#ifdef __UIQ__		
#ifdef __S80__
	if(CCknConfirmationDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_2)),*(iEikonEnv->AllocReadResourceL(R_SETUPWIZARD_UPLOADALL))))
#else
	CEikDialog* dialog;		
	dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_SETUPWIZARD_UPLOADALL)), R_SETUP_WIZARD_STEP_2);
	if(dialog->ExecuteLD(R_EIK_DIALOG_YESNO_INFO))
#endif
#else
	CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_SETUPWIZARD_UPLOADALL)));
	abtdlg->PrepareLC(R_GALLERY_INFO);		
	abtdlg->SetHeaderTextL(*(iEikonEnv->AllocReadResourceL(R_SETUP_WIZARD_STEP_2))); 

	if (abtdlg->RunLD())
#endif
	{
		SaveChoice(1);

		SavePhotosSettingsToDB(MODE_AUTO, 0, 0, 1); //set publish mode to auto
		SetDefaultFolder(*(iEikonEnv->AllocReadResourceL(R_DEFAULT_ALBUM)));

		if(iSettingsString.Length() == 0){
		RFs fs;
		fs.Connect();
		
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KContactSetupWizard);
		
		if (BaflUtils::FileExists(fs, path)) 
		{
			EnableContactsUpload(NOT_REGISTERED);

			BaflUtils::DeleteFile(fs, path);
		}

		fs.Close();

			//UploadSms();
			TInt val=2;
			iSmsBackupTimer->UploadSms(val);
			AddPhotos();
		}
		else
		{
			//UploadSms();
			TInt val=2;
			iSmsBackupTimer->UploadSms(val);
			AddPhotos();
			SaveSettingsString();
		}
#ifdef __UIQ__	
		iMainScreenValue = 6;
#ifdef __S80__
		ShowWindow(KViewIdStatusScreen); //status screen
		if(iRegisterView != NULL)
		{
			iRegisterView->ShowStatusScreen();
		}
#else
		ShowWindow(KViewIdStatusScreen); //status screen
		DeleteWindow(KViewIdRegisterScreen);
		ShowStatusScreen();
#endif
#else
		ActivateView(KViewIdStatusScreen);
#endif

	}
	else{
		
		{
			SaveChoice(1);
			SaveSettingsString();

#ifdef __UIQ__	
			iMainScreenValue = 6;
#ifdef __S80__
			ShowWindow(KViewIdStatusScreen); //status screen
			if(iRegisterView != NULL)
			{
				iRegisterView->ShowStatusScreen();
			}
#else
			ShowWindow(KViewIdStatusScreen); //status screen
			DeleteWindow(KViewIdRegisterScreen);
			ShowStatusScreen();
#endif
#else
			ActivateView(KViewIdStatusScreen);
#endif
		}
	}
*/
	
}
/*
void CRippleVaultAppUi::GetConnectionName()
{
	RGenericAgent netAgent1;
	if (netAgent1.Open() == KErrNone)
	{			
		TBuf<50> Name;		
		TUint32 ida=0;
		TUint32 ida1;
		TBuf<20> TableName;
		TableName.Copy(_L("IAP"));

		TBuf<20> ColName1;
		ColName1.Copy(_L("Id"));

		TBuf<20> ColName2;
		ColName2.Copy(_L("Name"));

		CleanupClosePushL(netAgent1);
		CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeIAP);
		CleanupStack::PushL(db);			
		
		CCommsDbConnectionPrefTableView* 
		view1 = db->OpenConnectionPrefTableViewOnRankLC(ECommDbConnectionDirectionOutgoing,1);
		CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref pref;
		if(view1->GotoFirstRecord()!=KErrNotFound)
		{
			view1->ReadConnectionPreferenceL(pref);
			ida=pref.iBearer.iIapId;			
		}
		
		CCommsDbTableView* 
		view = db->OpenTableLC(TableName);

		if(view->GotoFirstRecord()!=KErrNotFound)
		{
			do
			{
				view->ReadUintL(ColName1, ida1);
				if(ida==ida1)
				{
					view->ReadTextL(ColName2, Name);
					iConnectionName.Copy(Name);
				}
			}
			while(view->GotoNextRecord()!=KErrNotFound);
		}
		CleanupStack::PopAndDestroy(4); // view, db, netAgent			
	}

}
*/
void CRippleVaultAppUi::CopyUpgradeFile()
{
ShowMessageBoxFromResource(R_TEXT_UPGRADE_INFO);

TInt ret = 1;//PasswordDialog();//CheckUserId();

if(ret == 1)
{

	delete iPhotosDB;
	iPhotosDB = NULL;

	delete iContactsHandler->iChangesDB;
	iContactsHandler->iChangesDB = NULL;

	RFs file;
	TBuf<128> temppath;
	TBuf<128> path;
	TInt err;

	file.Connect();
		
	err = BaflUtils::FolderExists(file, KUpgradeFilesPath);

	if ( err == 0)
	{
		if(file.MkDir(KUpgradeFilesPath) == KErrNone)
		{		
			temppath.Copy(KUpgradeFilesPath);

			path = CRippleVaultAppUi::ApplicationDriveAndPath();
			path.Append(KConfigFile);
			BaflUtils::CopyFile(file, path, temppath, 0);
			//BaflUtils::DeleteFile(file, path);

			path = CRippleVaultAppUi::ApplicationDriveAndPath();
			path.Append(KContactsDBFile);
			BaflUtils::CopyFile(file, path, temppath, 0);
			BaflUtils::DeleteFile(file, path);

			path = CRippleVaultAppUi::ApplicationDriveAndPath();
			path.Append(KRegPath);
			BaflUtils::CopyFile(file, path, temppath, 0);
			BaflUtils::DeleteFile(file, path);

			path = CRippleVaultAppUi::ApplicationDriveAndPath();
			path.Append(KDatabaseName);									
			BaflUtils::CopyFile(file, path, temppath, 0);
			BaflUtils::DeleteFile(file, path);

			path = CRippleVaultAppUi::ApplicationDriveAndPath();
			path.Append(KTimeStampFile);									
			BaflUtils::CopyFile(file, path, temppath, 0);
			BaflUtils::DeleteFile(file, path);

			path = CRippleVaultAppUi::ApplicationDriveAndPath();
			path.Append(KRegPath1);									
			BaflUtils::CopyFile(file, path, temppath, 0);
			BaflUtils::DeleteFile(file, path);

			path = CRippleVaultAppUi::ApplicationDriveAndPath();
			path.Append(KAutostartSettings);
			if (BaflUtils::FileExists(file, path))
			{
				BaflUtils::CopyFile(file, path, temppath, 0);
				BaflUtils::DeleteFile(file, path);
			}

			path = CRippleVaultAppUi::ApplicationDriveAndPath();
			path.Append(KInitUploadFile);
			if (BaflUtils::FileExists(file, path))
			{
				BaflUtils::CopyFile(file, path, temppath, 0);
				BaflUtils::DeleteFile(file, path);
			}

			path = CRippleVaultAppUi::ApplicationDriveAndPath();
			path.Append(KIMSIFile);									
			BaflUtils::CopyFile(file, path, temppath, 0);
			BaflUtils::DeleteFile(file, path);

			path.Copy(_L("c:\\system\\secure\\"));	
			path.Append(KSecureFile);
			BaflUtils::CopyFile(file, path, temppath, 0);
			BaflUtils::DeleteFile(file, path);

		}	
	}
	file.Close();

	OpenBrowserForUpdate();

	StopSecure();

	User::Exit(EEikCmdExit);
}
}

TBool CRippleVaultAppUi::CheckUpgradeFile()
{	
	RFs file;	
	TBuf<128> temppath;
	TBuf<128> path;

	file.Connect();
	
	TBool flag;
	flag = EFalse;

	if (BaflUtils::FolderExists(file, KUpgradeFilesPath)) 
	{
		path.Copy(_L("c:\\system\\secure\\"));	
		
		temppath.Copy(KUpgradeFilesPath);
		temppath.Append(KSecureFile);
		BaflUtils::CopyFile(file,temppath, path,CFileMan::EOverWrite);

		path = CRippleVaultAppUi::ApplicationDriveAndPath();		

		temppath.Copy(KUpgradeFilesPath);
		temppath.Append(KContactsDBFile);
		BaflUtils::CopyFile(file,temppath, path,CFileMan::EOverWrite);

		temppath.Copy(KUpgradeFilesPath);
		temppath.Append(KRegPath);
		BaflUtils::CopyFile(file,temppath, path,CFileMan::EOverWrite);		

		temppath.Copy(KUpgradeFilesPath);
		temppath.Append(KDatabaseName);
		BaflUtils::CopyFile(file,temppath, path,CFileMan::EOverWrite);				

		temppath.Copy(KUpgradeFilesPath);
		temppath.Append(KTimeStampFile);
		BaflUtils::CopyFile(file,temppath, path,CFileMan::EOverWrite);				

		temppath.Copy(KUpgradeFilesPath);
		temppath.Append(KRegPath1);
		BaflUtils::CopyFile(file,temppath, path,CFileMan::EOverWrite);				

		temppath.Copy(KUpgradeFilesPath);
		temppath.Append(KAutostartSettings);
		if (BaflUtils::FileExists(file, temppath))
		{
		BaflUtils::CopyFile(file,temppath, path,CFileMan::EOverWrite);				
		}

		temppath.Copy(KUpgradeFilesPath);
		temppath.Append(KInitUploadFile);
		if (BaflUtils::FileExists(file, temppath))
		{
		BaflUtils::CopyFile(file,temppath, path,CFileMan::EOverWrite);				
		}

		temppath.Copy(KUpgradeFilesPath);
		temppath.Append(KIMSIFile);
		BaflUtils::CopyFile(file,temppath, path,CFileMan::EOverWrite);				
					
		TBuf<60> tmpstr;
		TBuf<150> upgradeurl;
	
		tmpstr.Copy( _L("version"));	
		LoadFromConfigFile(iVersion,tmpstr);

		tmpstr.Copy(_L("contacts_url"));	
		LoadFromConfigFile(iContactsUrl,tmpstr );

		tmpstr.Copy(_L("registration_url"));	
		LoadFromConfigFile(iRegUrl,tmpstr );	

		tmpstr.Copy(_L("upgrade_url"));	
		LoadFromConfigFile(upgradeurl,tmpstr );

		tmpstr.Copy(_L("media_pkt_upload_url"));	
		LoadFromConfigFile(iPublishUrl,tmpstr );

		tmpstr.Copy(_L("sharing_url"));	
		LoadFromConfigFile(iSharingUrl,tmpstr );
	
		tmpstr.Copy(_L("media_gallery_url"));	
		LoadFromConfigFile(iWapUrl,tmpstr );

		tmpstr.Copy(_L("sms_backup_url"));	
		LoadFromConfigFile(iSmsUrl,tmpstr );	

		tmpstr.Copy(_L("album_synch_url"));	
		LoadFromConfigFile(iAlbumUrl,tmpstr );	
		
		tmpstr.Copy(_L("sub_status_check"));	
		LoadFromConfigFile(iStatusUrl,tmpstr );	

		tmpstr.Copy(_L("apn"));	
		LoadFromConfigFile(iAccessPoints,tmpstr );	

		temppath.Copy(KUpgradeFilesPath);
		temppath.Append(KConfigFile);
		BaflUtils::CopyFile(file,temppath, path,CFileMan::EOverWrite);

		TBuf8<150> aValue;
		TBuf8<150> aParam;
		
		aParam.Copy(_L("version"));
		aValue.Copy(iVersion);	
		EditConfigFile(aValue, aParam);

		aParam.Copy(_L("contacts_url"));
		aValue.Copy(iContactsUrl);	
		EditConfigFile(aValue, aParam);

		aParam.Copy(_L("registration_url"));
		aValue.Copy(iRegUrl);	
		EditConfigFile(aValue, aParam);
		
		aParam.Copy(_L("upgrade_url"));
		aValue.Copy(upgradeurl);	
		EditConfigFile(aValue, aParam);

		aParam.Copy(_L("media_pkt_upload_url"));
		aValue.Copy(iPublishUrl);
		EditConfigFile(aValue, aParam);

		aParam.Copy(_L("sharing_url"));
		aValue.Copy(iSharingUrl);	
		EditConfigFile(aValue, aParam);
		
		aParam.Copy(_L("media_gallery_url"));
		aValue.Copy(iWapUrl);	
		EditConfigFile(aValue, aParam);

		aParam.Copy(_L("sms_backup_url"));
		aValue.Copy(iSmsUrl);	
		EditConfigFile(aValue, aParam);

		aParam.Copy(_L("album_synch_url"));
		aValue.Copy(iAlbumUrl);	
		EditConfigFile(aValue, aParam);
		
		aParam.Copy(_L("sub_status_check"));
		aValue.Copy(iStatusUrl);
		EditConfigFile(aValue, aParam);

		aParam.Copy(_L("apn"));
		aValue.Copy(iAccessPoints);	
		EditConfigFile(aValue, aParam);

		CFileMan* cf=CFileMan::NewL(file);	
		cf->RmDir(KUpgradeFilesPath);

		flag = ETrue;
	}	
	file.Close();

	return flag;
}

TBool CRippleVaultAppUi::GetSubVersion(TDes& aVersion, TInt& subVersion)
{
//#ifdef __LOGME__
//	LogText.Copy(_L("<GetSubVersion>"));
//	WriteLogFile(LogText);
//#endif

	TBuf<5>	num;
	TInt pos;

	pos = aVersion.Find(_L("."));
	if (pos < 0)
		return EFalse;
	num.Copy(&aVersion[0], pos);
	if (pos + 1 < aVersion.Length())
		aVersion.Copy(&aVersion[pos + 1]);
	else
		aVersion.Copy(_L(""));
	
	TLex lex(num);
	lex.Val(subVersion);

//#ifdef __LOGME__
//	LogText.Copy(_L("</GetSubVersion>"));
//	WriteLogFile(LogText);
//#endif

	return ETrue;
}

TBool CRippleVaultAppUi::IsLaterVersion(TDes& aVersion)
{
//#ifdef __LOGME__
//	LogText.Copy(_L("<IsLaterVersion>"));
//	WriteLogFile(LogText);
//#endif

	TInt major1, minor1, build1;
	TInt major2, minor2, build2;
	
	TBuf<40> buf1;
	TBuf<40> buf2;

	buf1.Copy(iVersion);
	buf1.Append(_L("."));
	buf2.Copy(aVersion);
	buf2.Append(_L("."));
	
	// Major 1
	if (!GetSubVersion(buf1, major1))
		return EFalse;
	// Major 2
	if (!GetSubVersion(buf2, major2))
		return EFalse;
	if (major2 > major1)
		return ETrue;
	else if (major2 < major1)
		return EFalse;

	// Minor 1
	if (!GetSubVersion(buf1, minor1))
		return EFalse;
	// Minor 2
	if (!GetSubVersion(buf2, minor2))
		return EFalse;
	if (minor2 > minor1)
		return ETrue;
	else if (minor2 < minor1)
		return EFalse;

	// Build 1
	if (!GetSubVersion(buf1, build1))
		return EFalse;
	// Build 2
	if (!GetSubVersion(buf2, build2))
		return EFalse;
	if (build2 > build1)
		return ETrue;
	else if (build2 < build1)
		return EFalse;

//#ifdef __LOGME__
//	LogText.Copy(_L("</IsLaterVersion>"));
//	WriteLogFile(LogText);
//#endif

	return EFalse;
}

void CRippleVaultAppUi::OpenBrowserForUpdate()
{
	HBufC* param = HBufC::NewLC( 200 ); 

	TBuf<150> upgradeurl;
	TBuf<50> billingtype;
	TBuf<50> encodedhp;
	TBuf<50> par;
	par.Copy(_L("upgrade_url"));
	LoadFromConfigFile(upgradeurl, par);

	par.Copy(_L("billing_reg_type"));
	LoadFromConfigFile(billingtype, par);

	par.Copy(_L("hp"));
	LoadEncodedHpFromConfigFile(encodedhp, par);

	//upgradeurl.Copy(_L("/asurionwap/downloadStart.do"));
#ifndef __UIQ__				
	//param->Des().Copy( _L( "4 http://wap.yahoo.com/" ) ); 	
	param->Des().Copy(_L("4 http://")); 
	param->Des().Append(iServerAddr); 
	param->Des().Append(_L(":"));
	param->Des().AppendNum((TInt)iPort); 
	//param->Des().Append(_L("/"));
	param->Des().Append(upgradeurl);
	param->Des().Append(_L("?uid="));
	param->Des().Append(iUserNames);
	param->Des().Append(_L("?hp="));
	param->Des().Append(encodedhp);	
	param->Des().Append(_L("&dlm=NU&tp="));
	param->Des().Append(billingtype);
	/*param->Des().Copy(_L("4 ")); 
	param->Des().Append(iWapUrl);
	param->Des().Append(_L("?userId="));
	param->Des().Append(iUserNames);*/

	/*TBuf<200> msg;
	msg.Copy(param->Des());
	CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(msg);
	abtdlg->PrepareLC(R_SETACCESSPOINT_ERROR);
	if (abtdlg->RunLD()) 
	{					
	}*/

	// Wap Browser's constants UId	
	const TInt KWmlBrowserUid = 0x10008D39; 

	TUid id( TUid::Uid( KWmlBrowserUid ) ); 
	
	TApaTaskList taskList( CEikonEnv::Static()->WsSession() );
	TApaTask task = taskList.FindApp( id );
	
	if ( task.Exists() )
	{
		HBufC8* param8 = HBufC8::NewLC( param->Length() ); 
		param8->Des().Append( *param ); 
		task.SendMessage( TUid::Uid( 0 ), *param8 ); 		// Uid is not used 
		CleanupStack::PopAndDestroy(); 
	}
	else 
	{ 
		RApaLsSession appArcSession; 
		User::LeaveIfError(appArcSession.Connect()); // connect to AppArc server 
		TThreadId id; 
		appArcSession.StartDocument( *param, TUid::Uid( KWmlBrowserUid ), id ); 
		appArcSession.Close(); 
	} 
#else
	param->Des().Copy(_L("http://")); 
	param->Des().Append(iServerAddr); 
	param->Des().Append(_L(":"));
	param->Des().AppendNum((TInt)iPort); 	
	//param->Des().Append(_L("/"));
	param->Des().Append(upgradeurl);
	param->Des().Append(_L("?uid="));
	param->Des().Append(iUserNames);
	param->Des().Append(_L("?hp="));
	param->Des().Append(encodedhp);	
	param->Des().Append(_L("&dlm=NU&tp="));
	param->Des().Append(billingtype);
	
	/*param->Des().Append(_L("?msisdn="));
	param->Des().Append(iDecodedMobileNo); 

	param->Des().Copy(_L("")); 
	param->Des().Append(iWapUrl);
	param->Des().Append(_L("?userId="));
	param->Des().Append(iUserNames);*/

	TUid KWebAppUid = { 0x100066de }; 

	RApaLsSession appArcSession; 
	appArcSession.Connect(); // connect to AppArc server 
	TApaAppInfo aInfo; 
	CApaCommandLine* aCommandLine = CApaCommandLine::NewL(); 
    
	appArcSession.GetAppInfo(aInfo, KWebAppUid); 
    
	aCommandLine->SetLibraryNameL(aInfo.iFullName); 
	aCommandLine->SetDocumentNameL(param->Des()); 
	aCommandLine->SetCommandL(EApaCommandCreate); 

	appArcSession.StartApp(*aCommandLine);

	SendToBackground();
#endif
	CleanupStack::PopAndDestroy(); // param
}

void CRippleVaultAppUi::StopSecure()
{
	const TUid starter_uid_secure= { 0x102032D1 };

	RWsSession ws;
	User::LeaveIfError( ws.Connect() );
	TApaTaskList taskList( ws );
	
	TApaTask task1 = taskList.FindApp(starter_uid_secure); // Position Last Task
	if(task1.Exists())
	{		
		task1.KillTask();
	}  
	ws.Close(); 

	CreateSecureStopFile();
}

void CRippleVaultAppUi::SaveUpgradeStatus(TInt8 aChoice, TDesC& aVersion)
{
#ifdef __LOGME__
	LogText.Copy(_L("<SaveUpgradeStatus>"));
	WriteLogFile(LogText);
#endif

	TInt8 tem;
	TBuf<20> v;

	GetUpgradeStatus(tem, v);
	iPhotosDB->SaveUpgradeStatus(aChoice, aVersion);

if(tem == 0)
{
	if( iSmsBackupTimer == NULL)
	{
		iSmsBackupTimer = CSmsBackupTimer::NewL(*this);
		iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_UPGRADE_INBOXSMS)));
		
		delete iSmsBackupTimer;
		iSmsBackupTimer = NULL;
	}
	else
	{
		iSmsBackupTimer->InsertSms(*(iEikonEnv->AllocReadResourceL(R_TEXT_UPGRADE_INBOXSMS)));
	}
}

	//UpdateScreen();

#ifdef __LOGME__
	LogText.Copy(_L("</SaveUpgradeStatus>"));
	WriteLogFile(LogText);
#endif

}

void CRippleVaultAppUi::GetUpgradeStatus(TInt8& aChoice, TDes& aVersion)
{
#ifdef __LOGME__
	//LogText.Copy(_L("<GetUpgradeStatus>"));
	//WriteLogFile(LogText);
#endif

	iPhotosDB->GetUpgradeStatus(aChoice, aVersion);

#ifdef __LOGME__
	//LogText.Copy(_L("</GetUpgradeStatus>"));
	//WriteLogFile(LogText);
#endif

}

// ---------------------------------------------------------
// CRippleVaultAppUi::SetAccesspoint()
// Set the active access point, given the APN Name
// ---------------------------------------------------------

TInt CRippleVaultAppUi::AutoAccesspoint()
{
	TInt flag=1;
	//RGenericAgent netAgent;
	//if (netAgent.Open() == KErrNone)
	{
		TBuf<50> Name;
		TBuf<50> Name2;
		TBuf<50> apnname;
		TUint32 ida1=0;
		TBuf<20> TableName;
		TBuf<20> IAPTableName;

		//netAgent.Stop();

		TableName.Copy(_L("OutgoingGPRS"));
		IAPTableName.Copy(_L("IAP"));

		TBuf<20> ColName1;
		ColName1.Copy(_L("Id"));
		
		TBuf<20> ColName2;
		ColName2.Copy(_L("APN"));

		TBuf<20> ColName3;
		ColName3.Copy(_L("Name"));
			
		//CleanupClosePushL(netAgent);	
		CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeIAP);
		CleanupStack::PushL(db);

		CCommsDbTableView* view = db->OpenTableLC(TableName);		
		if(view->GotoFirstRecord()!=KErrNotFound)
		{
			TInt count=0;
			do
			{			
				view->ReadTextL(ColName2, apnname);
				view->ReadTextL(ColName3, Name);

				iApnConnectionName.Copy(Name);

#ifdef __LOGME__
				LogText.Copy(Name);
				LogText.Append(_L("\n"));
				WriteLogFile(LogText);
#endif
				count++;

				if(iApnCount == count)
					break;
			}
			while(view->GotoNextRecord()!=KErrNotFound);			
		
#ifdef __LOGME__
				LogText.Copy(_L("count="));
				LogText.AppendNum(count);
				LogText.Append(_L("\n"));
				WriteLogFile(LogText);

				LogText.Copy(_L("iApnCount="));
				LogText.AppendNum(iApnCount);
				LogText.Append(_L("\n"));
				WriteLogFile(LogText);
#endif

			if(iApnCount>count || view->GotoNextRecord()==KErrNotFound)
				iApnCount = -2;

		}		

		//if(flag==25)
		{	
			CCommsDbTableView* view3 = db->OpenTableLC(IAPTableName);		
			if(view3->GotoFirstRecord()!=KErrNotFound)
			{
				do
				{								
					view3->ReadUintL(ColName1, ida1);
					view3->ReadTextL(ColName3, Name2);

					if(Name.Compare(Name2)==0) //match
					{					
						flag=2;									
						break;
					}
				}
				while(view3->GotoNextRecord()!=KErrNotFound);			
			}		
			CleanupStack::PopAndDestroy();
		}
		iApnID = ida1;
		
		if(flag==2)
		{		
			CCommsDbConnectionPrefTableView* 
			view1 = db->OpenConnectionPrefTableViewOnRankLC(ECommDbConnectionDirectionOutgoing,1);
			CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref pref;
			if(view1->GotoFirstRecord()!=KErrNotFound)
			{																
					view1->ReadConnectionPreferenceL(pref);

					if(pref.iBearer.iIapId==ida1)// default access point same as the one we want so do nothing
					{
						flag = 10;
						view1->UpdateDialogPrefL(ECommDbDialogPrefDoNotPrompt);
					}
					else
					{
						CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref aPref;
						aPref.iBearer.iIapId = ida1;
						aPref.iDirection = ECommDbConnectionDirectionOutgoing; 
						aPref.iDialogPref = ECommDbDialogPrefDoNotPrompt; 
						aPref.iBearer.iBearerSet = ECommDbBearerGPRS; 
						aPref.iRanking = 1; 		
				
						TRAPD(leaveCode,view1->ChangeConnectionPreferenceRankL(0)); // call a function
						TRAPD(leaveCode1,view1->InsertConnectionPreferenceL(aPref, EFalse)); // call a function						
							flag = 10;
				}
			}
			else
			{
				CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref aPref;
				aPref.iBearer.iIapId = ida1;
				aPref.iDirection = ECommDbConnectionDirectionOutgoing; 
				aPref.iDialogPref = ECommDbDialogPrefDoNotPrompt; 
				aPref.iBearer.iBearerSet = ECommDbBearerGPRS; 
				aPref.iRanking = 1; 		
				
				TRAPD(leaveCode1,view1->InsertConnectionPreferenceL(aPref, EFalse)); // call a function						

				flag = 10;				
			}
			CleanupStack::PopAndDestroy();			
		}
		CleanupStack::PopAndDestroy(2); // view, db, netAgent			
	}	
	return flag;
}

void CRippleVaultAppUi::ConnectRegSocket()
{
if(iAccessPoints.Find(_L("NOHARDCODE"))>=0)
{
	iApnCount++;
}
else
{
	iApnCount--;
}

	if(iSocketWriterPublish != NULL)
	{
		if(iAccessPoints.Find(_L("NOHARDCODE"))>=0)
		{
			AutoAccesspoint();
		}
		else
		{
			SetAccesspoint();
		}

		iSocketWriterPublish->Disconnect();
	    iSocketWriterPublish->RegisterPhotos();	
	}
}

void CRippleVaultAppUi::DefaultAccessPoint()
{
	GetApnID(iApnID);

	RGenericAgent netAgent1;
	if (netAgent1.Open() == KErrNone)
	{			
		TBuf<50> Name;		
		TUint32 ida=0;
		TUint32 ida1;
		TBuf<20> TableName;
		TableName.Copy(_L("IAP"));

		TBuf<20> ColName1;
		ColName1.Copy(_L("Id"));

		TBuf<20> ColName2;
		ColName2.Copy(_L("Name"));

		CleanupClosePushL(netAgent1);
		CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeIAP);
		CleanupStack::PushL(db);			
		
		CCommsDbConnectionPrefTableView* 
		view1 = db->OpenConnectionPrefTableViewOnRankLC(ECommDbConnectionDirectionOutgoing,1);
		CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref pref;
		if(view1->GotoFirstRecord()!=KErrNotFound)
		{
			view1->ReadConnectionPreferenceL(pref);
			ida=pref.iBearer.iIapId;

			//if(pref.iBearer.iIapId==iApnID)// default access point same as the one we want so do nothing
			if(iApnID == 0 || pref.iBearer.iIapId==iApnID)// default access point same as the one we want so do nothing
			{
				view1->UpdateDialogPrefL(ECommDbDialogPrefDoNotPrompt);
			}
			else if(iApnID !=0)
			{
				CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref aPref;
				aPref.iBearer.iIapId = iApnID;
				aPref.iDirection = ECommDbConnectionDirectionOutgoing; 
				aPref.iDialogPref = ECommDbDialogPrefDoNotPrompt; 
				aPref.iBearer.iBearerSet = ECommDbBearerGPRS; 
				aPref.iRanking = 1; 		
		
				TRAPD(leaveCode,view1->ChangeConnectionPreferenceRankL(0)); // call a function
				TRAPD(leaveCode1,view1->InsertConnectionPreferenceL(aPref, EFalse)); // call a function						
				ida=iApnID;
			}
		}

		CCommsDbTableView* 
		view = db->OpenTableLC(TableName);

		if(view->GotoFirstRecord()!=KErrNotFound)
		{
			do
			{
				view->ReadUintL(ColName1, ida1);
				if(ida==ida1)
				{
					iApnID = ida;
					view->ReadTextL(ColName2, Name);
					iApnConnectionName.Copy(Name);
				}
			}
			while(view->GotoNextRecord()!=KErrNotFound);
		}
		CleanupStack::PopAndDestroy(4); // view, db, netAgent			
	}
}

void CRippleVaultAppUi::SaveApnID(TUint32 aId)
{
#ifdef __LOGME__
	LogText.Copy(_L("<SaveApnID>"));
	WriteLogFile(LogText);
#endif

	iPhotosDB->SaveApnID(aId);

#ifdef __LOGME__
	LogText.Copy(_L("</SaveApnID>"));
	WriteLogFile(LogText);
#endif

}

void CRippleVaultAppUi::GetApnID(TUint32& aId)
{
#ifdef __LOGME__
	LogText.Copy(_L("<GetApnID>"));
	WriteLogFile(LogText);
#endif

	iPhotosDB->GetApnID(aId);
	
#ifdef __LOGME__
	LogText.Copy(_L("</GetApnID>"));
	WriteLogFile(LogText);
#endif
	
}

#ifdef __S80__
void CRippleVaultAppUi::DisplayProgressBallL(TInt aResourceId)
{
    if (iProgressBall == NULL)// Only allow one instance of the progress ball
    {
		/*HBufC*  message;
		TBuf<300> text;

		message = iEikonEnv->AllocReadResourceL(aResourceId);
		text.Copy(message->Des());

		delete message;
		*/

        // Get the position to display the dialog, offset to allow ProgressBar to show simultaneously
        TPoint point(ClientRect().Center());
        //point.iY -= 30;
        iProgressBall = CCknFloatingProgressBall::NewL(*iEikonEnv->AllocReadResourceL(aResourceId), &point);
        iProgressBall->ActivateL();
    }
}

void CRippleVaultAppUi::DestroyProgressBall()
{
	if(iProgressBall != NULL)
	{
		delete iProgressBall;
		iProgressBall = NULL;
	}
}

#endif

void CRippleVaultAppUi::PromptForSecureExit()
{
#ifdef __UIQ__	
#ifdef __S80__
	if(CCknConfirmationDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),*(iEikonEnv->AllocReadResourceL(R_EXIT_SECUREPROMPT))))
#else
	CEikDialog* dialog;
	dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_EXIT_SECUREPROMPT)), R_EXAMPLE_RIPPLE_PUBLISH);
	if(dialog->ExecuteLD(R_EIK_DIALOG_YESNO_INFO))
#endif
#else
	CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_EXIT_SECUREPROMPT)));
	abtdlg->PrepareLC(R_GALLERY_INFO);		
	abtdlg->SetHeaderTextL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH))); 

	if (abtdlg->RunLD())
#endif
	{
		if(RegFileExists())
		{
			TInt err;
			err = PasswordDialog();				

			if(err == 1)
			{
				StopSecure();
			}
		}
		else
		{
			StopSecure();
		}
	}

	//Exit();
	User::Exit(EEikCmdExit);
}

TInt CRippleVaultAppUi::PasswordDialog()
{
	TInt err=0;
#ifdef __UIQ__	
	CEikDialog* dialog;	
	dialog = new (ELeave) CPassWordDialog(*this);
	if(dialog->ExecuteLD(R_PASSWORD_DIALOG))
	{
		err = 1;
	}
#else
	TBool val=0;
	TBuf<30> password;
		
	do
	{
		CAknTextQueryDialog* phonedialog = CAknTextQueryDialog::NewL(password);
		if(phonedialog->ExecuteLD(R_PASSWORD_DIALOG))
		{					
			if(password.Compare(iPassword) == 0)
			{
				val = 1;				
				err = 1;
			}
			else
			{
				ShowMessageBoxFromResource(R_TEXT_INVALID_PASSWORD);
				//err = 0;
				//CEikonEnv::Static()->AlertWin(_L("Wrong Password"));
			}
		}
		else
			val = 1;

	}
	while(!val);
#endif

	return err;
}

#ifdef __UIQ__
CPassWordDialog::CPassWordDialog(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi)
{
}

CPassWordDialog::~CPassWordDialog()
{
}

TInt CPassWordDialog::OkToExitL(TInt)
{	
	TBuf<30> password;	
	TBuf<30> pword;	

	//get the passord
	CEikRichTextEditor* richTextEditor=STATIC_CAST(CEikRichTextEditor*,Control(EPassWord));
	richTextEditor->GetText(password);
	
	if(password.Compare(iAppUi.iPassword) == 0)
	{	
		return ETrue;	
	}
	else
	{
		iAppUi.ShowMessageBoxFromResource(R_TEXT_INVALID_PASSWORD);
		return EFalse;	
	}
}

TBool CPassWordDialog::ShutL()
{
	return EFalse;
}
#endif

TBool CRippleVaultAppUi::CheckIfPending()
{
	TInt ccounttemp;
	TInt pendingitems=0;
	TInt iPendingCountContacts;
	TInt iPendingCountPhotos;
	TInt iPendingCountVideos;
	TInt iPendingCountSms;

	GetTempContactsUploadSettingsFromDB(ccounttemp); //get temp contacts count
	GetContactsParams(pendingitems); // get total contacts count
	iPendingCountContacts = pendingitems - ccounttemp;

	ccounttemp = 0;
	//pendingitems = iPhotosAppUi->iPhotosDB->RowCount(); // get media pending count
	
	iPendingCountPhotos = iPhotosDB->PhotosRowCount();

	iPendingCountVideos = iPhotosDB->VideosRowCount();
	//pendingitems -= ccounttemp; // subtract sent items

	if(iPendingCountContacts<0) // negative items make no sense:-)
		iPendingCountContacts = 0;

	//iPendingCount = pendingitems;

	iPendingCountSms = iSmsCount + iAutoSmsCount;

	TBuf<50> dum;

	TInt code;

	code = GetNetworkStatusCode(dum);

	if((iPendingCountContacts + iPendingCountPhotos + iPendingCountVideos + iPendingCountSms > 0) || code != NETWORK_STATUS_RUNNING)
		return ETrue;
	else
		return EFalse;
}

TBool CRippleVaultAppUi::PendingSMS()
{
	if(iWriteSmsAO != NULL || iSocketWriterSmsStatus != NULL || iSocketWriterSMS != NULL)
		return ETrue;
	else 
		return EFalse;
}

TInt CRippleVaultAppUi::CheckMediaUploadExists(TDesC& aFilename)
{
	TBuf<128> filename;
	filename.Copy(aFilename);

	TInt ret;
	ret = iPhotosDB->CheckUploadingMediaFileName(aFilename);

	LogText.Copy(_L("CheckMediaUploadExists 1="));
	LogText.Append(aFilename);
	LogText.Append(_L("\n"));
	WriteLogFile(LogText);

	if(ret != 3) 
		ret = iPhotosDB->CheckUploadedMediaFileName(filename);

	LogText.Copy(_L("CheckMediaUploadExists 2="));
	LogText.Append(filename);
	LogText.Append(_L("\n"));
	WriteLogFile(LogText);

	return ret;
}

void CRippleVaultAppUi::ClearMediaQueue()
{
#ifdef __LOGME__	
	LogText.Copy(_L("\nClearMediaQueue\n"));
	WriteLogFile(LogText);
#endif

#ifdef __UIQ__	
#ifdef __S80__
	if(CCknConfirmationDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),*(iEikonEnv->AllocReadResourceL(R_CLEARMEDIAQUEUE_CONFIRM))))
#else
	CEikDialog* dialog;
	dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_CLEARMEDIAQUEUE_CONFIRM)), R_EXAMPLE_RIPPLE_PUBLISH);
	if(dialog->ExecuteLD(R_EIK_DIALOG_YESNO_INFO))
#endif
	{
		if(iSocketWriterPublishStatus !=NULL)
		{
			//iSocketWriterPublishStatus->CancelUpload();
			iSocketWriterPublishStatus->Disconnect();
			delete iSocketWriterPublishStatus;
			iSocketWriterPublishStatus = NULL;

			NetworkStatusNotification(NETWORK_STATUS_RUNNING);
		}

		if(iSocketWriterPublish != NULL)
		{
			//iSocketWriterPublish->CancelUpload();
			iSocketWriterPublish->Disconnect();
			delete iSocketWriterPublish;
			iSocketWriterPublish = NULL;

			NetworkStatusNotification(NETWORK_STATUS_RUNNING);
		}

		while(iPhotosDB->RowCount())
		{
			iPhotosDB->DeleteFirstRecord();
		}
	}
#else
	CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(iEikonEnv->AllocReadResourceL(R_CLEARMEDIAQUEUE_CONFIRM)));
	abtdlg->PrepareLC(R_GALLERY_INFO);		
	abtdlg->SetHeaderTextL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH))); 

	if (abtdlg->RunLD())
	{
		if(iSocketWriterPublishStatus !=NULL)
		{
			//iSocketWriterPublishStatus->CancelUpload();
			iSocketWriterPublishStatus->Disconnect();
			delete iSocketWriterPublishStatus;
			iSocketWriterPublishStatus = NULL;

			NetworkStatusNotification(NETWORK_STATUS_RUNNING);
		}

		if(iSocketWriterPublish != NULL)
		{
			//iSocketWriterPublish->CancelUpload();
			iSocketWriterPublish->Disconnect();
			delete iSocketWriterPublish;
			iSocketWriterPublish = NULL;

			NetworkStatusNotification(NETWORK_STATUS_RUNNING);
		}

		while(iPhotosDB->RowCount())
		{
			iPhotosDB->DeleteFirstRecord();
		}
	}
#endif

	CloseConnection();

#ifdef __LOGME__	
	LogText.Copy(_L("\n/ClearMediaQueue\n"));
	WriteLogFile(LogText);
#endif
}

TBool CRippleVaultAppUi::LoadEncodedHpFromConfigFile(TDes& aValue, TDesC& aParam)
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
		TInt err = file.Open(fs, path, EFileRead|EFileShareAny);
		if (err == KErrNone) 
		{
			TBuf8<2> buf;
			TBuf<2> buf16;
			TBuf<150> line;
			TBuf<150> param;

			param.Copy(aParam);
			param.Append(_L("="));

			TInt len = param.Length();

			line.Copy(_L(""));

			TInt count = 0;
			do 
			{
				/*TInt readErr = */ file.Read(buf, 1);
				if (buf.Length())
				count++;

				buf16.Copy(buf);
				line.Append(buf16);

				if (buf.Length() == 0 || buf.Find(_L8("\n")) != KErrNotFound ||
				buf.Find(_L8("\r")) != KErrNotFound) 
				{
					if (line.Find(param) != KErrNotFound && line.Length() > len) 
					{
						line.Delete(0,len);
						//line.SetLength(count - 7);
						if (buf.Find(_L8("\n")) != KErrNotFound || buf.Find(_L8("\r")) != KErrNotFound)
						{
							line.SetLength(line.Length() - 1);
						}
						
						{
							aValue.Copy(line);
						}
						ret = TRUE;
						break;
					}
					line.Copy(_L(""));
					count = 0;
				}
			}
			while(buf.Length() != 0);
		}
		file.Close();
	}
	fs.Close();
	return ret;
}

TInt CRippleVaultAppUi::CheckSecureApp()
{
	TInt val = 0;

	const TUid starter_uid_secure= { 0x102032D1 };

	RWsSession ws;
	User::LeaveIfError( ws.Connect() );
	TApaTaskList taskList( ws );
	
	TApaTask task1 = taskList.FindApp(starter_uid_secure); // Position Last Task
	
	if(task1.Exists())
		val = 1;
	else
		val = 0;
	
	ws.Close(); 

	return val;
}

void CRippleVaultAppUi::CreateSecureStopFile()
{	
	TBuf<128> path;
	path.Copy(_L("c:\\system\\secure\\"));	
	path.Append(KSecureStopFile);

	RFs fs;
	fs.Connect();

	RFile file;
	TInt err;

	err = file.Open(fs, path, EFileWrite);

	if(err != KErrNone)
	{
		file.Create(fs, path, EFileWrite);
	}	

	file.Close();
	fs.Close();
}

TInt CRippleVaultAppUi::CheckSpecialChars(TDesC& aUserName,TDesC& aPassword)
{
	TInt exists = 1;
	TBuf<50> text;
	text.Copy(aUserName);

	text.LowerCase();

	_LIT(KChar,"abcdefghijklmnopqrstuvwxyz0123456789_-.");
	TBuf<1> temp;
	TBufC16<50> str(KChar);

	for(int i=0; i<text.Length(); i++)
	{
		temp.Copy(&text[i],1);

		if(str.Find(temp) < 0)
		{
			exists = 2;
			break;
		}
	}

	if(exists == 1)
	{
		text.Copy(aPassword);
		text.LowerCase();

		for(int i=0; i<text.Length(); i++)
		{
			temp.Copy(&text[i],1);

			if(str.Find(temp) < 0)
			{
				exists = 3;
				break;
			}
		}
	}

	if(exists == 2)
	{
		exists = 0;
		ShowMessageBoxFromResource(R_TEXT_SPECIAL_CHARACTERS_USERNAME);
	}
	else if(exists == 3)
	{
		exists = 0;
		ShowMessageBoxFromResource(R_TEXT_SPECIAL_CHARACTERS_PASSWORD);
	}

	return exists;
}

void CRippleVaultAppUi::GetConatctsTimeFromDB(TInt& aContactsTime, TInt& aDays, TInt& aSyncState)
{
    TInt contactstime;	
	TInt days;
	TInt syncstate = 0;

    iContactsHandler->iChangesDB->GetConatctsTime(contactstime, days, syncstate);
	    
	aContactsTime = contactstime;	
	aDays = days;	
	aSyncState = syncstate;
}

void CRippleVaultAppUi::SaveConatctsTimeToDB(TInt aContactsTime, TInt aDays, TInt aSyncState)
{
    iContactsHandler->iChangesDB->SaveConatctsTime(aContactsTime, aDays, aSyncState);
}

void CRippleVaultAppUi::GetAccessPointID()
{
	RGenericAgent netAgent1;
	if (netAgent1.Open() == KErrNone)
	{			
		TBuf<50> Name;		
		TUint32 ida=0;
		TUint32 ida1;
		TBuf<20> TableName;
		TableName.Copy(_L("IAP"));

		TBuf<20> ColName1;
		ColName1.Copy(_L("Id"));

		TBuf<20> ColName2;
		ColName2.Copy(_L("Name"));

		CleanupClosePushL(netAgent1);
		CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeIAP);
		CleanupStack::PushL(db);			
		
		CCommsDbConnectionPrefTableView* 
		view1 = db->OpenConnectionPrefTableViewOnRankLC(ECommDbConnectionDirectionOutgoing,1);
		CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref pref;
		if(view1->GotoFirstRecord()!=KErrNotFound)
		{
			view1->ReadConnectionPreferenceL(pref);
			ida=pref.iBearer.iIapId;
			iApnID = ida;
		}
		
		CleanupStack::PopAndDestroy(3); // view1, db, netAgent			
	}
}
/*
TInt CRippleVaultAppUi::GetSimContactCountL()
{
	TBuf<255> iTsyName;

	TInt SimCntCount = 0;

	CCommsDatabase* db = CCommsDatabase::NewL(EDatabaseTypeUnspecified); 
	
	CleanupStack::PushL(db); 
	
	CCommsDbTableView* table = db->OpenTableLC(TPtrC(MODEM)); 
	
	table->GotoFirstRecord(); 
	
	table->ReadTextL(TPtrC(MODEM_TSY_NAME),iTsyName); 
	
	// Cleanup - CommsDB no longer needed 
	CleanupStack::PopAndDestroy(2); // table,db 

	// Connect to the ETel server 
	RTelServer aTelServer; 
	
	User::LeaveIfError(aTelServer.Connect()); 
	
	CleanupClosePushL(aTelServer); 
	
	User::LeaveIfError(aTelServer.LoadPhoneModule(iTsyName)); 
	
	TInt numberOfPhones; 
	
	User::LeaveIfError(aTelServer.EnumeratePhones(numberOfPhones)); 
	
	SimCntCount = 0;

	for (TInt i=numberOfPhones; i>0; i--) { 
		// Get the phone name 
		RTelServer::TPhoneInfo phoneInfo; 
		
		User::LeaveIfError(aTelServer.GetPhoneInfo(i-1,phoneInfo)); 
		
		// Open the phone by name 
		RBasicGsmPhone phone; 
		
		User::LeaveIfError(phone.Open(aTelServer,phoneInfo.iName)); 
		
		TInt phoneBookCount;
		
		phone.EnumeratePhoneBooks(phoneBookCount);
		
		RBasicGsmPhone::TPhoneBookInfo aPbInfo;
		
		for(TInt j=0;j<phoneBookCount;j++){
			phone.GetPhoneBookInfo(j,aPbInfo);
			
			SimCntCount += aPbInfo.iUsed;
		}
		phone.Close();
	} 
	CleanupStack::PopAndDestroy(1); 	
	
	return SimCntCount;
}
*/


void CRippleVaultAppUi::ShowSelectionList(TDes& aListselection)
{
#ifndef __UIQ__
	CArrayFixFlat<TInt>* indexArray = NULL;
	
	indexArray = new(ELeave)CArrayFixFlat<TInt>(6);
    CleanupStack::PushL(indexArray);

    CAknListQueryDialog* dialog = new( ELeave ) CAknListQueryDialog(indexArray);
    dialog->PrepareLC(R_BACKUP_DATA_QUERY);

    CArrayPtr<CGulIcon>* icons = new (ELeave) CAknIconArray(5);
    CleanupStack::PushL(icons);

	_LIT(KAvkonMbmFilePath, "z:\\system\\data\\avkon.mbm");

    icons->AppendL(iEikonEnv->CreateIconL(KAvkonMbmFilePath, 
        EMbmAvkonQgn_indi_marked_add, EMbmAvkonQgn_indi_marked_add_mask));
    icons->AppendL(iEikonEnv->CreateIconL(KAvkonMbmFilePath,
        EMbmAvkonQgn_prop_folder_medium,EMbmAvkonQgn_prop_folder_medium_mask));

    dialog->SetIconArrayL(icons);
    dialog->RunLD();

    CleanupStack::Pop(); //icons

	if(indexArray->Count())
	{
		for (TInt i=0;i<indexArray->Count();i++)
		{
			if(indexArray->At(i) == 0)
				aListselection.Append(_L("C1"));
			else if (indexArray->At(i) == 1)
				aListselection.Append(_L("P1"));
			else if (indexArray->At(i) == 2)
				aListselection.Append(_L("V1"));
			else if (indexArray->At(i) == 3)
				aListselection.Append(_L("S1"));

			//aListselection.AppendNum(_L("1"));
			//TBuf<20> buffer;

			//buffer.Copy(_L("index="));
			//buffer.AppendNum(indexArray->At(i));

			//CEikonEnv::Static ()->AlertWin(buffer);
		}
	}
	
    CleanupStack::PopAndDestroy();
#else
// Construct the demo dialog.
  CEikDialog* dialog = new(ELeave) CCheckBoxDialog(*this, aListselection);

// Run the dialog.
  dialog->ExecuteLD(R_CHECKBOX_DEMO_DIALOG);
#endif
}

void CRippleVaultAppUi::ChangePasswordFromSettings()
{
#ifndef __UIQ__
	TBuf<50> oldpass;
	TBuf<50> newpass;

	TInt flag = 2;
	TInt check;
	do
	{
		CAknMultiLineDataQueryDialog* dialog =	CAknMultiLineDataQueryDialog::NewL(oldpass, newpass);
		check=dialog->ExecuteLD(R_CHANGE_PASSWORD_DIALOG);
		if(check)
		{					
			if(oldpass.Compare(iPassword) == 0 && newpass.Length() >= 6)
			{
				flag = 1;
				iNewPassword.Copy(newpass);
				ChangePassword();
			}
			else
			{
				ShowMessageBoxFromResource(R_TEXT_INVALID_PASSWORD);	
			}
		}
		else
			flag = 1;
	}
	while(flag == 2);
#endif
}

void CRippleVaultAppUi::UploadSuccessfullSms()
{
	TBuf<150> message;
	//TBuf<150> formatBuf;

	message.Copy(_L(""));
	message.Copy(_L(""));
	message.Append(*(iEikonEnv->AllocReadResourceL(R_TEXT_UPLOAD_SUCCESSFULL_SMS)));
	if(iUploadContacts > 0){
		message.AppendNum(iUploadContacts);
		message.Append(_L(" "));
		message.Append(*(iEikonEnv->AllocReadResourceL(R_TEXT_UPLOAD_SUCCESSFULL_SMS1)));
	}

	if(iUploadPhotos > 0){
		if(iUploadContacts > 0)
			message.Append(_L(", "));
		message.AppendNum(iUploadPhotos);
		message.Append(_L(" "));
		message.Append(*(iEikonEnv->AllocReadResourceL(R_TEXT_UPLOAD_SUCCESSFULL_SMS2)));
	}

	if(iUploadVideos > 0){
		if(iUploadContacts > 0 || iUploadPhotos > 0)
			message.Append(_L(", "));
		message.AppendNum(iUploadVideos);
		message.Append(_L(" "));
		message.Append(*(iEikonEnv->AllocReadResourceL(R_TEXT_UPLOAD_SUCCESSFULL_SMS3)));
	}

	if(iUploadVideos > 0){
		if(iUploadContacts > 0 || iUploadPhotos > 0 || iUploadVideos > 0)
			message.Append(_L(", "));
		message.AppendNum(iUploadSms);
		message.Append(_L(" "));
		message.Append(*(iEikonEnv->AllocReadResourceL(R_TEXT_UPLOAD_SUCCESSFULL_SMS4)));
	}

	message.Append(*(iEikonEnv->AllocReadResourceL(R_TEXT_UPLOAD_SUCCESSFULL_SMS5)));

	if( iSmsBackupTimer == NULL)
	{
		iSmsBackupTimer = CSmsBackupTimer::NewL(*this);
		//iSmsBackupTimer->InsertUploadSuccessfullSms(message);
		iSmsBackupTimer->InsertSms(message);

		delete iSmsBackupTimer;
		iSmsBackupTimer = NULL;
	}
	else
	{
		//iSmsBackupTimer->InsertUploadSuccessfullSms(message);
		iSmsBackupTimer->InsertSms(message);
	}

	iUploadContacts = 0;
	iUploadPhotos   = 0;
	iUploadVideos   = 0;
	iUploadSms      = 0;

	/*if( iSmsTimer != NULL)
	{
		delete iSmsTimer;
   		iSmsTimer = NULL;
	}*/
}

void CRippleVaultAppUi::CheckSuccessfullSms()
{
	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KInitUploadFile);

	RFs fs1;

	fs1.Connect();

	if(CheckIfPending() || BaflUtils::FileExists(fs1, path))
	{
		fs1.Close();
		return;
	}
	fs1.Close();

	if(iUploadContacts > 0 || iUploadPhotos > 0 || iUploadVideos > 0 || iUploadSms > 0)
	{
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KInitSmsFile);

		RFs fs1;

		fs1.Connect();
		
		if (BaflUtils::FileExists(fs1, path)) //not suspended
		{
			BaflUtils::DeleteFile(fs1, path);

			UploadSuccessfullSms();
			/*if( iSmsTimer == NULL)
			{
				iSmsTimer = CSmsTimer::NewL(*this);
				iSmsTimer->Start();
			}*/
		}

		fs1.Close();
	}
}

void CRippleVaultAppUi::SaveSmsSettingsToDB(TInt8 aSmsMode)
{
	iPhotosDB->SaveSmsSettingL(aSmsMode);
	iSmsMode = aSmsMode;
}

void CRippleVaultAppUi::SaveSettingsToDB(TInt8 aAuto)
{
	iPhotosDB->SaveGeneralSettingL(aAuto);

    TBuf<128> KAutostartSetting;
    KAutostartSetting = CRippleVaultAppUi::ApplicationDriveAndPath();
	KAutostartSetting.Append(KAutostartSettings);
//	_LIT(KAutostartSetting, "C:\\System\\Apps\\RippleVault\\astart.txt");

	//Create AutostartSetting.cfg or delete it according to iAutostart variable
	// if exists dont autostart else do
	RFs fs;
	RFile file;

	fs.Connect();
	
	if(aAuto==0){

	TInt err = file.Replace(fs, KAutostartSetting, EFileStreamText|EFileWrite|EFileShareAny);

	if (err == KErrNone) 
	{
		file.Close();
	}		
	}
	else{
		//delete file
		fs.Delete(KAutostartSetting);
	}

	fs.Close();		

	iAutostart = aAuto;
}

void CRippleVaultAppUi::SavePhotosSettingsToDB(TInt8 aMode)
{
	iPhotosDB->SavePhotoSettingL(aMode);
	iMode = aMode;
}

#ifdef __UIQ__
CCheckBoxDialog::CCheckBoxDialog(CRippleVaultAppUi& aAppUi, TDes& aListselection):iAppUi(aAppUi),iListSelection(aListselection)
{
}

CCheckBoxDialog::~CCheckBoxDialog()
{
}

void CCheckBoxDialog::PreLayoutDynInitL()
{
	// Get a pointer to the control.
	  //CCoeControl* myControlPtr = this->Control(EMyCheckBoxContacts);

	// Downcast the returned CCoeControl* pointer to the correct type.
	  //CEikCheckBox* myCheckBox = static_cast<CEikCheckBox*>(myControlPtr);

	// Set the state of the Check box control with that specified by the data member.
	  //myCheckBox->SetState(CEikCheckBox::ESet);
	  //myCheckBox->SetBehavior(EEikButtonStaysSet);
}

TInt CCheckBoxDialog::OkToExitL(TInt)
{
	TInt contacts, photos, videos, sms;

	// Get a pointer to the control.
	  CCoeControl* myControlPtr = this->Control(EMyCheckBoxContacts);

	// Downcast the returned CCoeControl* pointer to the correct type.
	  CEikCheckBox* myCheckBox = static_cast<CEikCheckBox*>(myControlPtr);

	// Get the state of the Check box control and assign it to a data member.
	  contacts = myCheckBox->State();	

	  CCoeControl* myPhotosControlPtr = this->Control(EMyCheckBoxPhotos);
	  CEikCheckBox* myPhotosCheckBox = static_cast<CEikCheckBox*>(myPhotosControlPtr);
	  photos = myPhotosCheckBox->State();	

	  CCoeControl* myVideosControlPtr = this->Control(EMyCheckBoxVideos);
	  CEikCheckBox* myVideosCheckBox = static_cast<CEikCheckBox*>(myVideosControlPtr);
	  videos = myVideosCheckBox->State();	

	  CCoeControl* mySmsControlPtr = this->Control(EMyCheckBoxSMS);
	  CEikCheckBox* mySmsCheckBox = static_cast<CEikCheckBox*>(mySmsControlPtr);
	  sms = mySmsCheckBox->State();	

	  if(contacts != CEikCheckBox::EClear)
		iListSelection.Append(_L("C1"));
	  if(photos != CEikCheckBox::EClear)
		  iListSelection.Append(_L("P1"));
	  if(videos != CEikCheckBox::EClear)
		  iListSelection.Append(_L("V1"));
	  if(sms != CEikCheckBox::EClear)
		  iListSelection.Append(_L("S1"));

	  return ETrue;
}

TBool CCheckBoxDialog::ShutL()
{
	return EFalse;
}
#endif

void CRippleVaultAppUi::CaseMinimize()
{
	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KResumeFile);

	RFs fs1;

	fs1.Connect();
	
	TBool isSuspended = EFalse;

	if (BaflUtils::FileExists(fs1, path)) //resume
		isSuspended = EFalse;	
	else
		isSuspended = ETrue;

	fs1.Close();

	if(isSuspended){
#ifndef __UIQ__
		HandleCommandL(EExit);
#else
#ifdef __S80__
		User::Exit(EEikCmdExit);
#else
		PromptForSecureExit();
#endif
#endif
	}
	else{
#ifndef __UIQ__
		HandleCommandL(EHide);
#else
		SendToBackground();
#endif
	}
	//AppUi()->HandleCommandL(EExit);
}

void CRippleVaultAppUi::CaseSuspend()
{
    TBuf<128> KRegStep1;
    KRegStep1 = CRippleVaultAppUi::ApplicationDriveAndPath();
	KRegStep1.Append(_L("autostart.txt"));	

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KResumeFile);

	RFs fs1;

	fs1.Connect();
	
	if (BaflUtils::FileExists(fs1, path)) //not suspended
	{
#ifndef __UIQ__
		CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_SUSPEND_CONFIRM)));
		abtdlg->PrepareLC(R_SMART_CHARGE);
		if(abtdlg->RunLD()) 
#else
#ifdef __S80__
	if(CCknConfirmationDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),*(iEikonEnv->AllocReadResourceL(R_SUSPEND_CONFIRM))))
#else
		CEikDialog* dialog;
		dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_SUSPEND_CONFIRM)), R_EXAMPLE_RIPPLE_PUBLISH);
		if(dialog->ExecuteLD(R_EIK_DIALOG_YESNO_INFO))
#endif
#endif
		{

			//delete file
			//fs1.Delete(KRegStep1);
			BaflUtils::DeleteFile(fs1, path);

			BaflUtils::DeleteFile(fs1, KRegStep1);



			SaveContactsSettingsToDB(2);
			SaveSettingsToDB(DISABLED);
			SavePhotosSettingsToDB(PHOTOSBACKUP_MANUAL);
			SaveSmsSettingsToDB(SMSBACKUP_MANUAL);

			if(iSocketWriterPublishStatus !=NULL)
			{
				//iSocketWriterPublishStatus->CancelUpload();
				iSocketWriterPublishStatus->Disconnect();
				delete iSocketWriterPublishStatus;
				iSocketWriterPublishStatus = NULL;

				NetworkStatusNotification(NETWORK_STATUS_RUNNING);
			}

			if(iSocketWriterPublish != NULL)
			{
				//iSocketWriterPublish->CancelUpload();
				iSocketWriterPublish->Disconnect();
				delete iSocketWriterPublish;
				iSocketWriterPublish = NULL;

				NetworkStatusNotification(NETWORK_STATUS_RUNNING);
			}

			while(iPhotosDB->RowCount())
			{
				iPhotosDB->DeleteFirstRecord();
			}

			CloseConnection();

		}

	}
	else //suspended
	{
#ifndef __UIQ__
		CAknMessageQueryDialog* abtdlg = CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_RESUME_CONFIRM)));
		abtdlg->PrepareLC(R_SMART_CHARGE);
		if(abtdlg->RunLD()) 
#else
#ifdef __S80__
	if(CCknConfirmationDialog::RunDlgLD(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),*(iEikonEnv->AllocReadResourceL(R_RESUME_CONFIRM))))
#else
		CEikDialog* dialog;
		dialog = new (ELeave) CCustomMessageDialog(*(iEikonEnv->AllocReadResourceL(R_RESUME_CONFIRM)), R_EXAMPLE_RIPPLE_PUBLISH);
		if(dialog->ExecuteLD(R_EIK_DIALOG_YESNO_INFO))
#endif
#endif
		{

			RFile file;
			TInt err = file.Replace(fs1,path ,EFileWrite);

			if (err == KErrNone) {
				file.Close();
			}

			SaveContactsSettingsToDB(0);
			SaveSettingsToDB(ENABLED);
			SavePhotosSettingsToDB(PHOTOSBACKUP_PROMPT);
			SaveSmsSettingsToDB(SMSBACKUP_MANUAL);
		}
	}
	fs1.Close();
}

void CRippleVaultAppUi::SaveContactsSettingsToDB(TInt8 aUpload)
{
	iContactsHandler->iContactsUpload = aUpload;
    iContactsHandler->iChangesDB->SaveSettingsL(iContactsHandler->iContactsRoaming, aUpload);

	TTime oldtime;
	TTime newtime;

	iPhotosDB->GetContactsSettingsTimestampFromDB(oldtime, newtime);

	oldtime = newtime;
	newtime.UniversalTime();
	iPhotosDB->UpdateContactsSettingsTimestampToDB(oldtime, newtime);

}
