/*
* ============================================================================
*  Name     : CSecureContainer from SecureContainer.h
*  Part of  : Secure
*  Created  : 8/6/2005 by Ezhil
*  Implementation notes:
*     Initial content was generated by Series 60 AppWizard.
*  Version  :
*  Copyright: Purpleace
* ============================================================================
*/

// INCLUDE FILES
#include "SecureContainer.h"
#include "Secure.rsg"
#include <fbs.h> 
#include "logo.mbg"
#include <eiklabel.h>  // for example label control
#include "secureappui.h"
#include <eikspane.h>
#include <aknmessagequerydialog.h>
#include <bautils.h> 
#include <akntitle.h>

_LIT(KMBMFileName,"logo.mbm");
_LIT(KLockFile,		"lock.dat");


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CSecureContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CSecureContainer::ConstructL(const TRect& aRect)
    {
    CreateWindowL();	

	RFs fs;
	fs.Connect();

	TBuf<128> path;
	path = CSecureAppUi::ApplicationDriveAndPath();
	path.Append(KLockFile);
	
	if (BaflUtils::FileExists(fs, path))
	{
		CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
		statusPane->SwitchLayoutL(R_AVKON_STATUS_PANE_LAYOUT_EMPTY);
	
		iEikonEnv->AppUiFactory()->StatusPane()->MakeVisible(EFalse);
		
		SetRect(TRect(TPoint(0,0),TSize(176,208)));	
	}
	else
	{
		//CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
		//CAknTitlePane* tp=(CAknTitlePane*)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidTitle)); 
		//tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_SECURE_TEXT))); // Set the text string.

		CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
		statusPane->SwitchLayoutL(R_AVKON_STATUS_PANE_LAYOUT_EMPTY);
	
		iEikonEnv->AppUiFactory()->StatusPane()->MakeVisible(EFalse);

		//SetRect(aRect);
		SetRect(TRect(TPoint(0,0),TSize(176,186)));	
	}

	fs.Close();    
	
    ActivateL();
    }

CSecureContainer::CSecureContainer(CSecureAppUi& aAppUi) 
:iAppUi(aAppUi)
{
}

// Destructor
CSecureContainer::~CSecureContainer()
    {
    }

// ---------------------------------------------------------
// CSecureContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CSecureContainer::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    }

// ---------------------------------------------------------
// CSecureContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CSecureContainer::CountComponentControls() const
    {
    return 0; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CSecureContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CSecureContainer::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CSecureContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CSecureContainer::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();
    // TODO: Add your drawing code here
    // example code...
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushColor(KRgbBlack);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(aRect);
	
	const CFont* fontUsed;    
    fontUsed = iEikonEnv->SymbolFont();
	gc.UseFont(fontUsed);
	gc.SetPenColor(KRgbWhite);

	RFs fs;
	fs.Connect();

	TBuf<128> path1;
	path1 = CSecureAppUi::ApplicationDriveAndPath();
	path1.Append(KLockFile);
	
	if (BaflUtils::FileExists(fs, path1))
	{
		CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();
		CleanupStack::PushL(bitmap);

		TBuf<128> path;
		path = CSecureAppUi::ApplicationDriveAndPath();
		path.Append(KMBMFileName);

		User::LeaveIfError(bitmap->Load(path, EMbmLogoLogo));
		
		//TRect rect(0,0,176,208);
		TRect rect(140,10,166,36);
		gc.DrawBitmap(rect, bitmap);

		gc.DrawText(*(iEikonEnv->AllocReadResourceL(R_FROZEN_TEXT)), TPoint(2,45));

		gc.DrawText(*(iEikonEnv->AllocReadResourceL(R_UNLOCK_TEXT)), TPoint(2,60));
	
		TBuf<30> text;
		TInt pos=75;//drawing postion
		TInt len=28;//length of text to be displayed in 1 line.
		TBool flag=0;
		
		TBuf<50> tmpstr;
		
		tmpstr.Copy( _L("text"));	
		iAppUi.LoadFromConfigFile(iAppUi.iDisplayText,tmpstr,1);

		do
		{
			if(iAppUi.iDisplayText.Length() > len)
			{
				text.Copy(&(iAppUi.iDisplayText[0]),len);
			}
			else
			{
				text.Copy(iAppUi.iDisplayText);		
				flag = 1;
			}
			
			if(flag == 0)
				iAppUi.iDisplayText.Delete(0,len);
			
			gc.DrawText(text, TPoint(2,pos));
			pos = pos + 20;
		}
		while(flag == 0);

		CleanupStack::PopAndDestroy();
	}
	else
	{
		TBuf<30> text;
		TInt pos=70;//drawing postion
		TInt len=28;//length of text to be displayed in 1 line.
		TBool flag=0;
		
		gc.DrawText(*(iEikonEnv->AllocReadResourceL(R_SECURE_STATUS)), TPoint(2,30));

		iAppUi.iDisplayText.Copy(*(iEikonEnv->AllocReadResourceL(R_EXIT_TEXT)));	
		
		do
		{
			if(iAppUi.iDisplayText.Length() > len)
			{
				text.Copy(&(iAppUi.iDisplayText[0]),len);
			}
			else
			{
				text.Copy(iAppUi.iDisplayText);		
				flag = 1;
			}
			
			if(flag == 0)
				iAppUi.iDisplayText.Delete(0,len);
			
			gc.DrawText(text, TPoint(2,pos));
			pos = pos + 20;
		}
		while(flag == 0);
	}

	fs.Close();    		

    }

// ---------------------------------------------------------
// CSecureContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CSecureContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }

// End of File  
