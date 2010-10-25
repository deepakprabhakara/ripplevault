// --------------------------------------------------------------------------
// MyListBoxAppView.h
//
//
// A view class for VideoScreen list box example.
// --------------------------------------------------------------------------

#ifndef __VIDEOSCREENVIEW_UIQ_H
#define __VIDEOSCREENVIEW_UIQ_H

// INCLUDES
#include <coecntrl.h>
#include <coeview.h>
#include <eiklbo.h>  // MEikListBoxObserver
#include "ripplevaultappui.h"

class CRippleVaultAppUi;   

// FORWARD DECLARATIONS
class CMediaScreenListBox;

// CLASS DECLARATION
/**
 *  CMyListBoxAppView container control class.
 */
class CVideoScreenView: public CCoeControl, public MEikListBoxObserver
	{
public: // constructors and destructor
	
	static CVideoScreenView* NewL(const TRect& aRect,CRippleVaultAppUi& aAppUi );
	CVideoScreenView(CRippleVaultAppUi& aAppUi );		
	~CVideoScreenView();
	void ConstructL(const TRect& aRect);

public:
	CMediaScreenListBox* ListBox()
	{ return iListBox; }

	TBuf<50> KDirImageFolder1;
	TBuf<50> KDir1;

	void UpdateListBoxL(TInt aIsDoubleLine);

	void MarkUnMarkSelectedVideo();
	void MarkUnMarkSelectedItem();
	void MarkAllItems();
	void UnMarkAllItems();
	void MarkUnMarAllVideo(TInt aIsMark);
	TInt IsImageSelected();
	void EmptyListBoxL();
	
	void DrawText();

	TInt GetCount();
	void MarkSelectedVideo();

	void SendVideoToServer();
	
	TInt Start(TDesC& aFilename);
	
	void ResetData();

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
	CMediaScreenListBox* iListBox;

	RFs iRFsession;	
	CDir *dirList;
		
	CArrayPtr<CGulIcon>* iconList;
	TInt iCursorPos;
	TInt iVideosCount;
	TBool iIsMark;
	
	CDesCArray *ImageFilearray;
	CDesCArray *ImageDatearray;
	CDesCArray *ImageFilePatharray;
	CArrayFix<TInt>* iSelectedVideos;

	TBuf<160> comment;
	TInt8 iNewFolder;

	};

#endif // __VIDEOSCREENVIEW_UIQ_H

// End of File
