/* Copyright (c) 2004, Nokia Mobile Phones. All rights reserved */

#ifndef __PHOTOSCREENLISTBOX_H
#define __PHOTOSCREENLISTBOX_H

#include <eikbctrl.h>
#include <MdaImageConverter.h>

#include <eikclb.h>
#include <eikclbd.h>
#include <eiktxlbm.h>
#include <ckntitle.h>
#include "RippleVaultAppUi.h"

// Forward declarations:
class CFbsFont;
class CCknAppTitle;
class CEikColumnListBox;
class CCustomListBox;
class CRippleVaultAppUi;

/**
 *  This control owns column listbox,
 *  first column of which shows labels for each contact detail data.
 *  The data fields are contained in the second column.
 *
 *  This is non-window owning control.
 *
 *  This represents the right side view in this application.
 */
class CPhotoScreenListBox : public CEikBorderedControl, public MMdaImageUtilObserver
    {
public:

    CPhotoScreenListBox(CRippleVaultAppUi& aAppUi);
    void ConstructL(CCoeControl* aWindowOwningContainer,TRect aRect);
     ~CPhotoScreenListBox();

	CRippleVaultAppUi& iAppUi;


/* 
	CCustomListBox* ListBox()
	{
		return iListbox; 
	}
*/
public:    // From CEikBorderedControl

    void Draw(const TRect& aRect) const;
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

public:
	
	TBuf<300> KDirImageFolder1;

	TInt iPhotoFileIdEnd;

    virtual TSize MinimumSize();
    void SetData(TInt aIsmark); // Don't take ownership
	void SetIconsArray();

	TInt Start(TDesC& aFilename);
	void ScaleBitmap();
	void MarkSelectedPhoto();
	void MarkUnMarkSelectedItem();
	void MarkAllItems();
	void UnMarkAllItems();
	void ResetData();
	void ClearData();
	void SetTitle();
	void GetActualPhotoCount();
	TInt IsImageSelected();
	void SendPhotoToServer();

protected: // From CCoeControl

    virtual TInt CountComponentControls() const;
    virtual CCoeControl* ComponentControl(TInt aIndex) const;
    virtual void SizeChanged();
    virtual void FocusChanged(TDrawNow aDrawNow);

private: // Data

    CCknAppTitle*        iAppTitle;
	//CCustomListBox*		 iListbox;
    CEikColumnListBox*   iListbox;
	CArrayPtr<CGulIcon>* iconList;
	
	RFs iRFsession;	
	CDir *dirList;
	CGulIcon* ico;
	CMdaBitmapScaler *iBitmapScaler;
	TBool iIsScale;
	TBuf<160> comment;
	TInt8 iNewFolder;
	TBool iIsFileOpen;
	TInt iPhotosCount;
	TRect innerRect;
	TBool iIsMark;
	TInt iCursorPos;
	
	CDesCArray *ImageFilearray;
	CDesCArray *ImageFilePatharray;
	CDesCArray *ImageDatearray;
	CArrayFix<TInt>* iSelectedPhotos;
	CArrayFix<TInt>* iPhotoListArray;

	CFbsBitmap *iBitmap;
	CMdaImageFileToBitmapUtility *iConverter;
	void MiuoCreateComplete(TInt aError);
	void MiuoConvertComplete(TInt aError);
	void MiuoOpenComplete(TInt aError);

	CColumnListBoxData* colData;
    };


#endif // __PHOTOSCREENLISTBOX_H
