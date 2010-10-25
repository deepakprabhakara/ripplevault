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
 Module Name				: RegisterContainer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 17-06-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     Status Screen for S60
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/

// INCLUDE FILES
#include "RegisterContainer.h"
#include "RippleVaultAppUi.h"
#include <RippleVault.rsg>
#include <avkon.hrh>
#include <eiklabel.h>  // for example label control
#include <akncontext.h> 
#include <akntitle.h>
#include <eikspane.h>
#include <fbs.h> 
#include <in_sock.h> 

#include <aknnavi.h>

#include "rippleimage.mbg"
#include "logo.mbg"

#include "Sync.mbg"

#include "RippleVault.hrh"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCRegisterContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------

void CRegisterContainer::ConstructL(const TRect& aRect)
    {
    CreateWindowL();

	iFlag = 1;
	//iPosX = 0;
	//iPosY = 0;	

	ApplicationStatus();

	CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
	CAknContextPane* cp=(CAknContextPane *)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidContext));

	CAknNavigationControlContainer* aNaviPane = (CAknNavigationControlContainer *)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidNavi));

	CAknNavigationDecorator* aNaviDecorator = aNaviPane->CreateMessageLabelL(_L(""));

    aNaviPane->PushL(*aNaviDecorator);

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KRippleImageMbmFileName);

	CFbsBitmap* bitmap = iEikonEnv->CreateBitmapL(path, EMbmRippleimageRippleimage);
	CleanupStack::PushL(bitmap);
	CFbsBitmap* bitmapmask = iEikonEnv->CreateBitmapL(path, EMbmRippleimageRippleimagem);
	CleanupStack::PushL(bitmapmask);
	cp->SetPicture(bitmap, bitmapmask);
	CleanupStack::Pop(); // bitmapmask
	CleanupStack::Pop(); // bitmap
	//DrawNow();
	DrawDeferred();

	CAknTitlePane* tp=(CAknTitlePane*)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidTitle)); 
    tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH))); // Set the text string.
		
	TBuf<128> addr;
	TInt Port;
	TInt Mode;
	TInt desc;
	TInt roam;
	TInt autostart;
	TInt securegallery;

	iPhotosAppUi->GetSettingsFromDB(addr, Port, autostart);

	iPhotosAppUi->GetPhotosSettingsFromDB(Mode, desc, roam, securegallery);

	//TBuf<20> buf;
/*	
	if(Mode==2)
	{
		//buf.Copy(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_STOPPED)));
		ApplicationStatus(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_STOPPED)));
		//iPhotosAppUi->EnableUpload(Mode);
	}
	else
	{
		//buf.Copy(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RUNNING)));
		ApplicationStatus(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RUNNING)));
		//iPhotosAppUi->EnableUpload(Mode);
	}
*/
	TRect Please(TPoint(0,44),TPoint(176,188));
    SetRect(Please);
    //SetRect(aRect);

	iPeriodicTimer = CPeriodic::NewL(CActive::EPriorityStandard);

	StartTimer();

    ActivateL();

	if( iPhotosAppUi->iSmsValue == 0)	
		iPhotosAppUi->ImsiCheck();

    }

// Destructor
CRegisterContainer::~CRegisterContainer()
    {
	if (iPeriodicTimer)
		{
		// Stop the periodic timer
		iPeriodicTimer->Cancel();
		}
	delete iPeriodicTimer;
	iPeriodicTimer = NULL;

    }

// ---------------------------------------------------------
// CCRegisterContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CRegisterContainer::CountComponentControls() const
    {
    return 0; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CCRegisterContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CRegisterContainer::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CCRegisterContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CRegisterContainer::Draw(const TRect& aRect) const
    {	

    CWindowGc& gc = SystemGc();
    // TODO: Add your drawing code here
    // example code...
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushColor(KRgbWhite);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(aRect);

	const CFont* font;
	font = iEikonEnv->DenseFont(); //light font

	const CFont* fontbold;
	fontbold = iEikonEnv->LegendFont(); // bold font

	if(isQueuedInfo)
	{

	gc.UseFont(fontbold);
	gc.DrawText(iLabelTitle, TPoint(5,20));

	gc.UseFont(font);
	gc.DrawText(iLabelVersion, TPoint(50,20));
	gc.DrawText(iLabelAppStatus, TPoint(5,40));

	gc.UseFont(fontbold);
	gc.DrawText(iLabelPendingItems, TPoint(5,60));

	gc.UseFont(font);
	gc.DrawText(iLabelContactsCount, TPoint(60,60));
	gc.DrawText(iLabelPhotosCount, TPoint(5,80));

	gc.UseFont(fontbold);
	gc.DrawText(iLabelUploadedItems, TPoint(5,100));

	gc.UseFont(font);
	gc.DrawText(iLabelUploadedContactsCount, TPoint(60,100));
	gc.DrawText(iLabelUploadedPhotosCount, TPoint(5,120));

	gc.DrawText(iLabelErrStatus, TPoint(5,140));

		CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();

		if(bitmap != NULL)
		{
		CleanupStack::PushL(bitmap);
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KSyncMbmFileName);

		TInt bmaperr;

		if(iFlag == 1){
			//User::LeaveIfError(bitmap->Load(path, EMbmSyncSync1));
			TRAP(bmaperr, bitmap->Load(path, EMbmSyncSync1));
		}
		else if(iFlag == 2){
			//User::LeaveIfError(bitmap->Load(path, EMbmSyncSync2));
			TRAP(bmaperr, bitmap->Load(path, EMbmSyncSync2));
		}
		else if(iFlag == 3){
			//User::LeaveIfError(bitmap->Load(path, EMbmSyncSync3));
			TRAP(bmaperr, bitmap->Load(path, EMbmSyncSync3));
		}
		else if(iFlag == 4){
			//User::LeaveIfError(bitmap->Load(path, EMbmSyncSync4));
			TRAP(bmaperr, bitmap->Load(path, EMbmSyncSync4));
		}
		else
			TRAP(bmaperr, bitmap->Load(path, EMbmSyncSync4));

		//TPoint pos(iPosX,iPosY);
		TPoint pos(135,5);
		if(bitmap!=NULL)
			gc.BitBlt(pos, bitmap);	
			//gc.DrawBitmap(pos, bitmap);

		CleanupStack::PopAndDestroy();
		}
		
	}
	else
	{

	gc.UseFont(fontbold);
	gc.DrawText(iLabelTitle, TPoint(5,20));

	gc.UseFont(font);
	gc.DrawText(iLabelAppStatus, TPoint(5,40));
	//gc.DrawText(iLabelPhotosCount, TPoint(5,110));

	gc.UseFont(fontbold);
	gc.DrawText(iLabelUploadedItems, TPoint(5,70));

	gc.UseFont(font);
	gc.DrawText(iLabelVersion, TPoint(125,20));
	gc.DrawText(iLabelUploadedContactsCount, TPoint(60,70));
	gc.DrawText(iLabelUploadedPhotosCount, TPoint(5,90));

	gc.DrawText(iLabelErrStatus, TPoint(5,140));

	}

	if( iPhotosAppUi->iSmsValue == 1)
	{
		if(iPhotosAppUi->iWaitDialog == NULL)
		{
			iPhotosAppUi->ExitApplication();
		}
	}
}

// ---------------------------------------------------------
// CCRegisterContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CRegisterContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }

void CRegisterContainer::ApplicationStatus()
{

	isDoingSomething = EFalse;

	isQueuedInfo = EFalse;

	TBuf<50> message;
	TBuf<50> formatBuf;
	TInt ccounttemp;
	TInt pendingitems=0;
	
	iPhotosAppUi->GetTempContactsUploadSettingsFromDB(ccounttemp); //get temp contacts count
	iPhotosAppUi->GetContactsParams(pendingitems); // get total contacts count
	iPendingCountContacts = pendingitems - ccounttemp;

	ccounttemp = 0;
	//pendingitems = iPhotosAppUi->iPhotosDB->RowCount(); // get media pending count
	
	iPendingCountPhotos = iPhotosAppUi->iPhotosDB->PhotosRowCount();

	iPendingCountVideos = iPhotosAppUi->iPhotosDB->VideosRowCount();
	//pendingitems -= ccounttemp; // subtract sent items

	if(iPendingCountContacts<0) // negative items make no sense:-)
		iPendingCountContacts = 0;

	//iPendingCount = pendingitems;

	iPendingCountSms = iPhotosAppUi->iSmsCount + iPhotosAppUi->iAutoSmsCount;

    iLabelTitle.Copy(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RUNNING)));

	TBuf<50> dum;

	TInt code;

	code = iPhotosAppUi->GetNetworkStatusCode(dum);

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
		if(iPendingCountSms>0  || (iPendingCountSms == 0 && !isQueuedInfo)){
		ccounttemp++;
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_SMS_UPLOADED)));
		message.Format(formatBuf, iPendingCountSms);

		if(ccounttemp == 2)
			iLabelContactsCount.Append(_L(", "));

		iLabelContactsCount.Append(message);
		}

		message.Copy(_L(""));
		iLabelPhotosCount.Copy(message);

		if(iPendingCountPhotos>0 || (iPendingCountPhotos == 0 && !isQueuedInfo)){
		ccounttemp++;
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_PHOTOS_UPLOADED)));
		message.Format(formatBuf, iPendingCountPhotos);

		if(ccounttemp == 2)
			iLabelContactsCount.Append(_L(", "));

		if(ccounttemp > 2)
			iLabelPhotosCount.Append(message);
		else
			iLabelContactsCount.Append(message);

		}

		message.Copy(_L(""));
		if(iPendingCountVideos>0 || (iPendingCountVideos == 0 && !isQueuedInfo)){
		ccounttemp++;
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_VIDEOS_UPLOADED)));
		message.Format(formatBuf, iPendingCountVideos);

		if(ccounttemp == 2)
			iLabelContactsCount.Append(_L(", "));
		else if (ccounttemp == 4)
			iLabelPhotosCount.Append(_L(", "));
			
		if(ccounttemp > 2)
			iLabelPhotosCount.Append(message);
		else
			iLabelContactsCount.Append(message);

		}

		//}
		if((iPendingCountContacts + iPendingCountPhotos + iPendingCountVideos + iPendingCountSms == 0) && isQueuedInfo)	
		{
			iLabelContactsCount.Copy(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_ZEROITEMS)));
		}


		TInt album;
		TInt albumshares;
		
		iPhotosAppUi->GetUploadSettingsFromDB(album, iPendingCountPhotos, iPendingCountVideos, albumshares);
		iPhotosAppUi->GetContactsUploadSettingsFromDB(iPendingCountContacts);
		iPhotosAppUi->GetSmsUploadSettingsFromDB(iPendingCountSms);

//-------------------------
	}
	else
	{
		isQueuedInfo = EFalse;

		TInt album;
		TInt albumshares;
		
		iPhotosAppUi->GetUploadSettingsFromDB(album, iPendingCountPhotos, iPendingCountVideos, albumshares);
		iPhotosAppUi->GetContactsUploadSettingsFromDB(iPendingCountContacts);
		iPhotosAppUi->GetSmsUploadSettingsFromDB(iPendingCountSms);

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
	if(iPendingCountSms>0  || (iPendingCountSms == 0 && !isQueuedInfo)){
	ccounttemp++;
	formatBuf.Copy(_L(""));
	formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_SMS_UPLOADED)));
	message.Format(formatBuf, iPendingCountSms);

	if(ccounttemp == 2)
		iLabelUploadedContactsCount.Append(_L(", "));

	iLabelUploadedContactsCount.Append(message);
	}

	message.Copy(_L(""));
	iLabelUploadedPhotosCount.Copy(message);

	if(iPendingCountPhotos>0 || (iPendingCountPhotos == 0 && !isQueuedInfo)){
	ccounttemp++;
	formatBuf.Copy(_L(""));
	formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_PHOTOS_UPLOADED)));
	message.Format(formatBuf, iPendingCountPhotos);

	if(ccounttemp == 2)
		iLabelUploadedContactsCount.Append(_L(", "));

	if(ccounttemp > 2)
		iLabelUploadedPhotosCount.Append(message);
	else
		iLabelUploadedContactsCount.Append(message);

	}

	message.Copy(_L(""));
	if(iPendingCountVideos>0 || (iPendingCountVideos == 0 && !isQueuedInfo)){
	ccounttemp++;
	formatBuf.Copy(_L(""));
	formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_VIDEOS_UPLOADED)));
	message.Format(formatBuf, iPendingCountVideos);

	if(ccounttemp == 2)
		iLabelUploadedContactsCount.Append(_L(", "));
	else if (ccounttemp == 4)
		iLabelUploadedPhotosCount.Append(_L(", "));
		
	if(ccounttemp > 2)
		iLabelUploadedPhotosCount.Append(message);
	else
		iLabelUploadedContactsCount.Append(message);

	}

	//}
	if((iPendingCountContacts + iPendingCountPhotos + iPendingCountVideos + iPendingCountSms == 0) && isQueuedInfo)	
	{
		iLabelUploadedContactsCount.Copy(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_ZEROITEMS)));
	}

	message.Copy(_L("v"));
	message.Append(iPhotosAppUi->iVersion);
	
    iLabelVersion.Copy(message);

	message.Copy(_L("")); // Application status
	iStatusCode = iPhotosAppUi->GetNetworkStatusCode(message);
	if(iStatusCode != NETWORK_STATUS_FAILED && iStatusCode != NETWORK_STATUS_RUNNING)
	{
		isDoingSomething = ETrue;
	}

	if(message.Compare(_L("")) == 0){
		message.Copy(*(iEikonEnv->AllocReadResourceL(R_STATUSTEXT_RUNNING)));
	}

	if(!iPhotosAppUi->ReadImsiNumber()){
		isDoingSomething = EFalse;
		message.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_NETWORK_STATUS_SUSPENDED_SIMCHANGE)));
	}
	
    iLabelAppStatus.Copy(message);

	if(!isQueuedInfo)
	{
		isDoingSomething = EFalse;

		TBuf<160> message;

		TTime btime;
		iPhotosAppUi->iPhotosDB->GetBackupTimestampFromDB(btime);

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
		iPhotosAppUi->LogText.Copy(_L("\nBackup Time="));
		iPhotosAppUi->LogText.Append(message);
		iPhotosAppUi->LogText.Append(_L("\n"));
		iPhotosAppUi->WriteLogFile(iPhotosAppUi->LogText);
#endif

		iLabelAppStatus.Copy(message);
	}

	isError = EFalse;

	message.Copy(_L(""));
	iErrCode = iPhotosAppUi->GetNetworkErrorCode(message);

	if(iErrCode!=0)
		isError = ETrue;

	isUpgrade = EFalse;

	TInt8 choice;
	TBuf<50> vers;
	
	iPhotosAppUi->GetUpgradeStatus(choice, vers);
	if(choice == 1)
	{
		isUpgrade = ETrue;
		formatBuf.Copy(_L(""));
		formatBuf.Append(*(iEikonEnv->AllocReadResourceL(R_TEXT_UPGRADE_AVAILABLE)));
		message.Copy(_L(""));
		message.Format(formatBuf, &vers);
	}

	iLabelErrStatus.Copy(message);

}
// End of File  
/*
void CRegisterContainer::UpdateScreen()
{
	ApplicationStatus();

	//DrawNow();
	DrawDeferred();
}
*/
void CRegisterContainer::StartTimer()
	{
	//If the timer is not already running, start it
	if (!iPeriodicTimer->IsActive())
		{
		iPeriodicTimer->Start(1,500000,TCallBack(CRegisterContainer::Period,this));
		}

	}


void CRegisterContainer::DoPeriodTask()
	{
	/*if(iPosX > 120)
		iFlag = 1;
	else if(iPosX < 10)
		iFlag = 0;

	
	if(iFlag == 0)
		iPosX = iPosX + 10;
	else
		iPosX = iPosX - 10;*/

	if(iFlag == 4)
		iFlag = 0;

		if(isDoingSomething)
		{
			iFlag++;

		}

		ApplicationStatus();

		// Update the screen
		//DrawNow();
		DrawDeferred();

	}


// This function is called by the periodic timer
TInt CRegisterContainer::Period(TAny * aPtr)
	{
    ((CRegisterContainer*)aPtr)->DoPeriodTask();
    //returning a value of TRUE indicates the callback should be done again
	return TRUE;
	}
