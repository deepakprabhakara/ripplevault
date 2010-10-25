// --------------------------------------------------------------------------
// MyListBoxAppView.h
//
//
// A view class for MainScreen list box example.
// --------------------------------------------------------------------------

#ifndef __MAINSCREENVIEW_UIQ_H
#define __MAINSCREENVIEW_UIQ_H

// INCLUDES
#include <coecntrl.h>
#include <coeview.h>
#include <eiklbo.h>  // MEikListBoxObserver
#include "ripplevaultappui.h"

class CRippleVaultAppUi;   

// FORWARD DECLARATIONS
class CMainScreenListBox;

// CLASS DECLARATION
/**
 *  CMyListBoxAppView container control class.
 */
class CMainScreenView: public CCoeControl, public MEikListBoxObserver
	{
public: // constructors and destructor
	
	static CMainScreenView* NewL(const TRect& aRect,CRippleVaultAppUi& aAppUi );
	CMainScreenView(CRippleVaultAppUi& aAppUi );		
	~CMainScreenView();
	void ConstructL(const TRect& aRect);

public:
	CMainScreenListBox* ListBox()
	{ return iListBox; }

	TInt iIsDoubleLine;

	void UpdateListBoxL(TInt aIsDoubleLine);

	void SetIconsArray();

	CRippleVaultAppUi& iAppUi;

private: // from CCoeControl
	void SizeChanged();
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl(TInt aIndex) const;
	void Draw(const TRect& aRect) const;
	virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
		TEventCode aType);

private: // from MEikListBoxObserver
	virtual void HandleListBoxEventL(CEikListBox* aListBox,
		TListBoxEvent aEventType);

private: // New functions
	void CreateListBoxL();
        
private: //data
	CMainScreenListBox* iListBox;	
		// ETrue if the list box displays double line item.
		// EFalse if the list box displayes one line item.
	};

#endif // __MAINSCREENVIEW_UIQ_H

// End of File
