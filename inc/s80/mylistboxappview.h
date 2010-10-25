// --------------------------------------------------------------------------
// MyListBoxAppView.h
//
//
// A view class for custom list box example.
// --------------------------------------------------------------------------

#ifndef MYLISTBOXAPPVIEW_H
#define MYLISTBOXAPPVIEW_H

// INCLUDES
#include <coecntrl.h>
#include <coeview.h>
#include <eiklbo.h>  // MEikListBoxObserver
#include "ripplevaultappui.h"
#include <ckntitle.h>

class CCknAppTitle;
class CRippleVaultAppUi;   
// FORWARD DECLARATIONS
class CCustomListBox;

// CLASS DECLARATION

/**
 *  CMyListBoxAppView container control class.
 */
class CMyListBoxAppView: public CCoeControl,
							public MEikListBoxObserver,public MCoeView
	{
public: // constructors and destructor
	
	static CMyListBoxAppView* NewL(const TRect& aRect);
	CMyListBoxAppView();		
	~CMyListBoxAppView();
	void ConstructL(const TRect& aRect);

public:
	CCustomListBox* ListBox()
	{ return iListBox; }

	//TInt IsDoubleLine()
	//{ return iIsDoubleLine; }

	TInt iIsDoubleLine;

	void UpdateListBoxL(TInt aIsDoubleLine);

	void SetIconsArray();

	TVwsViewId ViewId() const;

	void SetAppIns(CRippleVaultAppUi *aAppUi);

	virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
		TEventCode aType);

	virtual void SetFocus(TBool aFocus, TDrawNow aDrawNow=ENoDrawNow);

private: // from CCoeControl
	void SizeChanged();
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl(TInt aIndex) const;
	void Draw(const TRect& aRect) const;
	

private: // from MEikListBoxObserver
	virtual void HandleListBoxEventL(CEikListBox* aListBox,
		TListBoxEvent aEventType);

    void ViewActivatedL(const TVwsViewId& aPrevViewId,\
                         TUid aCustomMessageId,const TDesC8& aCustomMessage);
    void ViewDeactivated();

private: // New functions
	void CreateListBoxL();
        
private: //data
	CCustomListBox* iListBox;	
	CRippleVaultAppUi *iAppUi;
	CCknAppTitle* iTitle;
		// ETrue if the list box displays double line item.
		// EFalse if the list box displayes one line item.
	};

#endif // MYLISTBOXAPPVIEW_H

// End of File
