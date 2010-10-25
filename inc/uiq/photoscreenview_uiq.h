// --------------------------------------------------------------------------
// MyListBoxAppView.h
//
//
// A view class for PhotoScreen list box example.
// --------------------------------------------------------------------------

#ifndef __PHOTOSCREENVIEW_UIQ_H
#define __PHOTOSCREENVIEW_UIQ_H

// INCLUDES
#include <coecntrl.h>
#include <coeview.h>
#include <eiklbo.h>  // MEikListBoxObserver
#include <MdaImageConverter.h>
#include "ripplevaultappui.h"

class CRippleVaultAppUi;   

// FORWARD DECLARATIONS
class CMediaScreenListBox;

// CLASS DECLARATION
/**
 *  CMyListBoxAppView container control class.
 */
class CPhotoScreenView: public CCoeControl, public MEikListBoxObserver, public MMdaImageUtilObserver
	{
public: // constructors and destructor
	
	static CPhotoScreenView* NewL(const TRect& aRect,CRippleVaultAppUi& aAppUi );
	CPhotoScreenView(CRippleVaultAppUi& aAppUi );		
	~CPhotoScreenView();
	void ConstructL(const TRect& aRect);

public:
	CMediaScreenListBox* ListBox()
	{ return iListBox; }

	TBuf<50> KDirImageFolder1;
	TBuf<50> KDir1;

	void UpdateListBoxL(TInt aIsDoubleLine);
	void ScaleBitmap();

	//TInt IsImageSelected();
	void MarkUnMarkSelectedPhoto();
	//void UnMarkSelectedPhoto();
	void MarkUnMarkSelectedItem();
	void MarkAllItems();
	void UnMarkAllItems();
	void UpDateIconList();
	void MarkUnMarAllPhoto(TInt aIsMark);
	TInt IsImageSelected();
	void EmptyListBoxL();

	TInt GetCount();
	void MarkSelectedPhoto();
	
	void DrawText();

	void SendPhotoToServer();
	
	//TInt IsDone();
	
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
	TInt iPhotosCount;
	TBool iIsMark;
	
	CDesCArray *ImageFilearray;
	CDesCArray *ImageDatearray;
	CDesCArray *ImageFilePatharray;
	CArrayFix<TInt>* iSelectedPhotos;

	TInt iPhotoFileId;
	TInt iPhotoFileIdEnd;
	CGulIcon* ico;
	TInt iIsDone;
	CMdaBitmapScaler *iBitmapScaler;
	TBool iIsScale;
	TBuf<160> comment;
	TInt8 iNewFolder;
	TBool iIsFileOpen;

//conveter
		CFbsBitmap *iBitmap;
		CMdaImageFileToBitmapUtility *iConverter;
		void MiuoCreateComplete(TInt aError);
		void MiuoConvertComplete(TInt aError);
		void MiuoOpenComplete(TInt aError);
	};

#endif // __PHOTOSCREENVIEW_UIQ_H

// End of File
