/*
* ============================================================================
*  Name     : StatusScreenView_Uiq.cpp
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CStatusScreenView implementation
*  Copyright: PurpleAce
* ============================================================================
*/


#include "StatusScreenView_Uiq.h"
#include <RippleVault_Uiq.rsg>

#include "logo.mbg"
#include "fbs.h"

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

	SetRect(aRect);

	iPeriodicTimer = CPeriodic::NewL(CActive::EPriorityStandard);

	StartTimer();

	ActivateL();
}

CStatusScreenView::~CStatusScreenView()
{
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
    return 0;
}

void CStatusScreenView::SetFocus(TBool aFocus, TDrawNow aDrawNow)
{
	CCoeControl::SetFocus(aFocus, aDrawNow);
}

CCoeControl* CStatusScreenView::ComponentControl(TInt aIndex) const
{
    switch (aIndex)
        {
		default:
			return NULL;
        }
}

void CStatusScreenView::Draw(const TRect& /* aRect */) const
{
	CWindowGc& gc = SystemGc();
	gc.Clear();

	const CFont* fontUsed;
	
	// Start with a clear screen
	gc.Clear();
	
	TRect drawRect = Rect();
    
	drawRect.Shrink(5,5);		   	
	gc.DrawRect(drawRect);
    
	// Use the title font supplied by the UI
	//fontUsed = iEikonEnv->LegendFont();
	fontUsed = iEikonEnv->AnnotationFont();

	const CFont* fontbold;
	//fontbold = iEikonEnv->TitleFont(); // bold font
	fontbold = iEikonEnv->LegendFont(); // bold font

	if(isQueuedInfo)
	{
		gc.UseFont(fontbold);//status title
		gc.DrawText(status,TPoint(15,40));	

		gc.UseFont(fontUsed);//version, status text
		gc.DrawText(status1,TPoint(145,60));
		gc.DrawText(status2,TPoint(15,60));

		gc.UseFont(fontbold);//queued:
		gc.DrawText(status3,TPoint(15,90));

		gc.UseFont(fontUsed);//contacts, SMS, photos, videos count
		gc.DrawText(status4,TPoint(15,105));
		gc.DrawText(status5,TPoint(15,120));

		gc.UseFont(fontbold);//status title
		gc.DrawText(iLabelUploadedItems,TPoint(15,140));	

		gc.UseFont(fontUsed);//version, status text
		gc.DrawText(iLabelUploadedContactsCount,TPoint(15,155));
		gc.DrawText(iLabelUploadedPhotosCount,TPoint(15,170));

		gc.UseFont(fontUsed);//error
		gc.DrawText(status6,TPoint(15,200));

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

			TPoint pos(145,6);
			if(bitmap!=NULL)
				gc.BitBlt(pos, bitmap);	
			CleanupStack::PopAndDestroy();
		}	
	}
	else
	{
	gc.UseFont(fontbold);//queued:
	gc.DrawText(status,TPoint(15,65));

	gc.UseFont(fontUsed);//contacts, SMS, photos, videos count
	gc.DrawText(status2,TPoint(15,80));
	
	gc.UseFont(fontbold);//status title
	gc.DrawText(iLabelUploadedItems,TPoint(15,120));	

	gc.UseFont(fontUsed);//version, status text
	gc.DrawText(status1,TPoint(145,40));
	gc.DrawText(iLabelUploadedContactsCount,TPoint(15,135));
	gc.DrawText(iLabelUploadedPhotosCount,TPoint(15,150));

	gc.UseFont(fontUsed);//error
	gc.DrawText(status6,TPoint(15,200));

	}

    // Finished using the font
	gc.DiscardFont();
}

void CStatusScreenView::ApplicationStatus(TDesC& stat, TDesC& stat1, TDesC& stat2, TDesC& stat3, TDesC& stat4, TDesC& stat5, TDesC& stat6, TBool aIsDoing, TBool aQueuedInfo, TDesC& stat7, TDesC& stat8, TDesC& stat9)
{
	isQueuedInfo = aQueuedInfo;
	isDoingSomething = aIsDoing;
	status.Copy(stat);
	status1.Copy(stat1);
	status2.Copy(stat2);
	status3.Copy(stat3);
	status4.Copy(stat4);
	status5.Copy(stat5);
	status6.Copy(stat6);

	iLabelUploadedItems.Copy(stat7);
	iLabelUploadedContactsCount.Copy(stat8);
	iLabelUploadedPhotosCount.Copy(stat9);

	//DrawNow();
	DrawDeferred();

}

void CStatusScreenView::StartTimer()
	{
	//If the timer is not already running, start it
	if (!iPeriodicTimer->IsActive())
		{
		iPeriodicTimer->Start(1, 1500000,TCallBack(CStatusScreenView::Period,this));
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
		}
		// Update the screen
		//DrawNow();
		iAppUi.ShowStatusScreen();

	}


// This function is called by the periodic timer
TInt CStatusScreenView::Period(TAny * aPtr)
	{
    ((CStatusScreenView*)aPtr)->DoPeriodTask();
    //returning a value of TRUE indicates the callback should be done again
	return TRUE;
	}
