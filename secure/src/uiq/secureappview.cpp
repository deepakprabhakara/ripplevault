// CSecureAppView.cpp
// -------------------
//

// Copyright (c) 2002 Symbian Ltd.  All rights reserved.
//

////////////////////////////////////////////////////////////////////////
//
// Source file for the implementation of the 
// application view class - CSecureAppView
//
////////////////////////////////////////////////////////////////////////

#include "SecureAppView.h"
#include <bautils.h> 
#include "logo.mbg"
#include "secureappui.h"

#include <Secure_uiq.rsg>	
#include "Secure_uiq.hrh"

_LIT(KMBMFileName,	"logo.mbm");
_LIT(KLockFile,		"lock.dat");

//
//Constructor for the view.
//
CSecureAppView::CSecureAppView()
	{
	}


//             Static NewL() function to start the standard two
//             phase construction.
//
CSecureAppView* CSecureAppView::NewL(const TRect& aRect)
	{
	CSecureAppView* self = new(ELeave) CSecureAppView();
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop();
	return self;
	}


//
//             Destructor for the view.
//
CSecureAppView::~CSecureAppView()
	{
	}


//             Second phase construction.
//
void CSecureAppView::ConstructL(const TRect& aRect)
    {
	           // Control is a window owning control
	CreateWindowL();
	           // Extent of the control. This is
	           // the whole rectangle available to application.
	           // The rectangle is passed to us from the application UI.
	//SetRect(aRect);
	RFs fs;
	fs.Connect();

	TBuf<128> path;
	path = CSecureAppUi::ApplicationDriveAndPath();
	path.Append(KLockFile);
	
	if (BaflUtils::FileExists(fs, path))
	{
		SetRect(TRect(TPoint(0,0),TSize(240,320)));
	}
	else
	{
		SetRect(aRect);		
	}

	fs.Close(); 	
	// At this stage, the control is ready to draw so
	// we tell the UI framework by activating it.
	ActivateL();
	}


//             Drawing the view - 
//
void CSecureAppView::Draw(const TRect& aRect) const
	{
    // Window graphics context
	CWindowGc& gc = SystemGc();

	// Start with a clear screen
	gc.Clear();
	gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushColor(KRgbBlack);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(aRect);

	// Font used for drawing text
    const CFont* fontUsed;

	fontUsed = iEikonEnv->TitleFont();
	gc.UseFont(fontUsed);
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

		TRect rect(180,10,206,36);
		gc.DrawBitmap(rect, bitmap);
	
		CleanupStack::PopAndDestroy();     	

		TPoint pos1(10,30);
		gc.DrawText(*(iEikonEnv->AllocReadResourceL(R_FROZEN_TEXT)),pos1);	

		TPoint pos2(10,50);
		gc.DrawText(*(iEikonEnv->AllocReadResourceL(R_UNLOCK_TEXT_UIQ)),pos2);	

		//TPoint pos3(10,90);
		//gc.DrawText(*(iEikonEnv->AllocReadResourceL(R_UNLOCK_TEXT1)),pos3);	

		iSecureText.Copy(iTempText);	
	}
	else
	{
		TPoint pos2(10,30);
		gc.DrawText(*(iEikonEnv->AllocReadResourceL(R_SECURE_STATUS)),pos2);

		iSecureText.Copy(iExitText);		
	}
		
	TBuf<30> text;
	TInt pos=70;//drawing postion
	TInt len=25;//length of text to be displayed in 1 line.
	TBool flag=0;

	do
	{
		if(iSecureText.Length() > len)
		{
			text.Copy(&(iSecureText[0]),len);
		}
		else
		{
			text.Copy(iSecureText);		
			flag = 1;
		}
		
		if(flag == 0)
			iSecureText.Delete(0,len);
		
		gc.DrawText(text, TPoint(2,pos));
		pos = pos + 20;
	}
	while(flag == 0);
	
	fs.Close();
		
    //Finished using the font
	gc.DiscardFont();	           

	}

void CSecureAppView::SecureText(TDes& text, TDes& text1)
{	
	iTempText.Copy(text);		
	iExitText.Copy(text1);		
	//iSecureText.Copy(iTempText);	
}

