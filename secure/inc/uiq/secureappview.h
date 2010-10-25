// SecureAppView.h
// ------------
//
// Copyright (c) 2002 Symbian Ltd.  All rights reserved.
//

////////////////////////////////////////////////////////////////////
// Secure
// ----------
//
//
// The class definitions for the Secure example application
// containing a single view.
//
// The class definitions are:
//
// CSecureApplication 
// CSecureAppUi
// CSecureAppView
// CSecureDocument
//
//
////////////////////////////////////////////////////////////////////

#ifndef __SECUREAPPVIEW_H
#define __SECUREAPPVIEW_H

#include <Secure_uiq.rsg>
#include "Secure_uiq.hrh"

#include <coecntrl.h>

////////////////////////////////////////////////////////////////////////
//
// CSecureAppView
//
////////////////////////////////////////////////////////////////////////
class CSecureAppView : public CCoeControl
    {
public:
	static CSecureAppView* NewL(const TRect& aRect);
	
	CSecureAppView();
	~CSecureAppView();
    
	void ConstructL(const TRect& aRect);
	TBuf<300> iSecureText;	
	TBuf<300> iTempText;	
	TBuf<300> iExitText;	
	void SecureText(TDes& text, TDes& text1);

private:
	// Inherited from CCoeControl
	void Draw(const TRect& /*aRect*/) const;	

private:
	HBufC*  iExampleText;
    };


#endif

