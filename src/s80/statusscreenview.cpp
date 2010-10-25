/*
* ============================================================================
*  Name     : StatusScreenView.cpp
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CStatusScreenView implementation
*  Copyright: PurpleAce
* ============================================================================
*/


#include <cknenv.h>
#include <eikenv.h>
#include <ckntitle.h>

#include "StatusScreenView.h"
#include "RippleVaultUtils.h"
#include <RippleVault_series80.rsg>

#include "logo.mbg"
#include "fbs.h"
#include "constants.h"
///
#include <ckndgtrg.h>

#include "Sync.mbg"

CStatusScreenView* CStatusScreenView::NewL( const TRect& aRect, CRippleVaultAppUi& aAppUi)
{
    CStatusScreenView* self = new (ELeave) CStatusScreenView(aAppUi);
    CleanupStack::PushL(self);
    self->ConstructL( aRect );
    CleanupStack::Pop();
    return self;
}

CStatusScreenView::CStatusScreenView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
}

void CStatusScreenView::ConstructL( const TRect& aRect )
{
    CreateWindowL();

	iFlag = 1;

    iTitle = CCknAppTitle::NewL(CCknAppTitle::EList);
    iTitle->SetContainerWindowL(*this);
    //HBufC* text = iCoeEnv->AllocReadResourceLC(R_LIST_MAINMENU_STATUS);
    iTitle->SetTextL(*iCoeEnv->AllocReadResourceL(R_LIST_MAINMENU_STATUS), CCknAppTitle::EMainTitle);
    //CleanupStack::PopAndDestroy(text);

	TRect titleRect(1, 1, aRect.Width()-KShadowMargin, iTitle->MinimumSize().iHeight);
	iTitle->SetRect(titleRect);

	// TODO : ADD YOUR OTHER CONTROLS HERE
	// COMPLETE VIEW SETUP

	SetRect(aRect);
/*
	_LIT(KPath,"\\");
	
	TBuf<255> path = KPath;
	CCknTargetFolderDialog::RunSelectFolderDlgLD(path);
*/

	// Indicate that the control is blank
    //SetBlank();

    // Set the control's border
    //SetBorder(TGulBorder::EFlatContainer);

    // Set the correct application view (Note: ESkinAppViewWithCbaNoToolband is the default)
    CknEnv::Skin().SetAppViewType(ESkinAppViewWithCbaNoToolband);

	iPeriodicTimer = CPeriodic::NewL(CActive::EPriorityStandard);

	StartTimer();

	ActivateL();
}

CStatusScreenView::~CStatusScreenView()
{
    delete iTitle;
	if (iPeriodicTimer)
		{
		// Stop the periodic timer
		iPeriodicTimer->Cancel();
		}
	delete iPeriodicTimer;
	iPeriodicTimer = NULL;

}

TInt CStatusScreenView::CountComponentControls() const
{
    return 1;
}

void CStatusScreenView::SetFocus(TBool aFocus, TDrawNow aDrawNow)
{
	if (iTitle) 
		{
		iTitle->SetFocus( aFocus, aDrawNow );
		}
	CCoeControl::SetFocus(aFocus, aDrawNow);
}

CCoeControl* CStatusScreenView::ComponentControl(TInt aIndex) const
{
    switch (aIndex)
        {
		case 0:
			return iTitle;
			break;
		default:
			return NULL;
        }
}

void CStatusScreenView::Draw(const TRect& /* aRect */) const
{
	CWindowGc& gc = SystemGc();
	const CFont* fontUsed;
	
	// Start with a clear screen
	gc.Clear();
	
	TRect drawRect = Rect();
    
	drawRect.Shrink(5,5);		   	
	gc.DrawRect(drawRect);
    
	// Use the title font supplied by the UI
	fontUsed = iEikonEnv->LegendFont();

	const CFont* fontbold;
	fontbold = iEikonEnv->TitleFont(); // bold font

	if(isQueuedInfo)
	{
	gc.UseFont(fontbold);//status title
	gc.DrawText(iLabelTitle,TPoint(15,50));	

	gc.UseFont(fontUsed);//version, status text
	gc.DrawText(iLabelVersion,TPoint(400,50));
	gc.DrawText(iLabelAppStatus,TPoint(15,70));

	gc.UseFont(fontbold);//queued:
	gc.DrawText(iLabelPendingItems,TPoint(15,100));

	gc.UseFont(fontUsed);//contacts, SMS, photos, videos count
	gc.DrawText(iLabelContactsCount,TPoint(15,120));
	gc.DrawText(iLabelPhotosCount,TPoint(15,140));

	gc.UseFont(fontbold);
	gc.DrawText(iLabelUploadedItems, TPoint(225,100));

	gc.UseFont(fontUsed);
	gc.DrawText(iLabelUploadedContactsCount, TPoint(225,120));
	gc.DrawText(iLabelUploadedPhotosCount, TPoint(225,140));

	//gc.UseFont(fontbold);//error
	gc.DrawText(iLabelErrStatus,TPoint(15,170));

		CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();
		if(bitmap != NULL)
		{
		CleanupStack::PushL(bitmap);

		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KSyncMbmFileName);

		if(iFlag == 1)
			User::LeaveIfError(bitmap->Load(path, EMbmSyncSync1));
		else if(iFlag == 2)
			User::LeaveIfError(bitmap->Load(path, EMbmSyncSync2));
		else if(iFlag == 3)
			User::LeaveIfError(bitmap->Load(path, EMbmSyncSync3));
		else if(iFlag == 4)
			User::LeaveIfError(bitmap->Load(path, EMbmSyncSync4));
		else
			User::LeaveIfError(bitmap->Load(path, EMbmSyncSync4));


		//TPoint pos(iPosX,iPosY);
		TPoint pos(400,50);
		if(bitmap != NULL)
			gc.BitBlt(pos, bitmap);	
		CleanupStack::PopAndDestroy();
		}	
	}
	else
	{
	gc.UseFont(fontbold);
	gc.DrawText(iLabelUploadedItems, TPoint(15,100));

	gc.UseFont(fontUsed);
	gc.DrawText(iLabelVersion, TPoint(225,50));
	gc.DrawText(iLabelUploadedContactsCount, TPoint(15,120));
	gc.DrawText(iLabelUploadedPhotosCount, TPoint(15,140));

	gc.UseFont(fontbold);
	//gc.DrawText(iLabelTitle, TPoint(15,120));
	gc.DrawText(iLabelTitle, TPoint(15,50));

	gc.UseFont(fontUsed);
	gc.DrawText(iLabelAppStatus, TPoint(15,70));
	//gc.DrawText(iLabelPhotosCount, TPoint(5,110));

	gc.DrawText(iLabelErrStatus, TPoint(15,170));
	}

    // Finished using the font
	gc.DiscardFont();
}

void CStatusScreenView::ApplicationStatus(TDesC& stat, TDesC& stat1, TDesC& stat2, TDesC& stat3, TDesC& stat4, TDesC& stat5, TDesC& stat6, TBool aIsDone, TBool aQueuedInfo, TDesC& aLabelUploadedItems, TDesC& aLabelUploadedContactsCount, TDesC& aLabelUploadedPhotosCount)
{
	isQueuedInfo = aQueuedInfo;
	isDoingSomething = aIsDone;

	iLabelTitle.Copy(stat);
	iLabelVersion.Copy(stat1);
	iLabelAppStatus.Copy(stat2);
	iLabelPendingItems.Copy(stat3);
	iLabelContactsCount.Copy(stat4);
	iLabelPhotosCount.Copy(stat5);
	iLabelErrStatus.Copy(stat6);

	iLabelUploadedItems.Copy(aLabelUploadedItems);
	iLabelUploadedContactsCount.Copy(aLabelUploadedContactsCount);
	iLabelUploadedPhotosCount.Copy(aLabelUploadedPhotosCount);
	//DrawNow();
	DrawDeferred();
}

void CStatusScreenView::StartTimer()
	{
	//If the timer is not already running, start it
	if (!iPeriodicTimer->IsActive())
		{
		iPeriodicTimer->Start(1,500000,TCallBack(CStatusScreenView::Period,this));
		}

	}


void CStatusScreenView::DoPeriodTask()
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

			iAppUi.ShowStatusScreen();
			// Update the screen
			//DrawNow();
		}

	}


// This function is called by the periodic timer
TInt CStatusScreenView::Period(TAny * aPtr)
	{
    ((CStatusScreenView*)aPtr)->DoPeriodTask();
    //returning a value of TRUE indicates the callback should be done again
	return TRUE;
	}
