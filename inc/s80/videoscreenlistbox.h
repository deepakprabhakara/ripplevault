/* Copyright (c) 2004, Nokia Mobile Phones. All rights reserved */

#ifndef __VIDEOSCREENLISTBOX_H
#define __VIDEOSCREENLISTBOX_H

#include <eikbctrl.h>

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
class CVideoScreenListBox : public CEikBorderedControl
    {
public:

    CVideoScreenListBox(CRippleVaultAppUi& aAppUi);
    void ConstructL(CCoeControl* aWindowOwningContainer,TRect aRect);
     ~CVideoScreenListBox();

	CRippleVaultAppUi& iAppUi;

public:    // From CEikBorderedControl

    void Draw(const TRect& aRect) const;
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

public:
	
	TBuf<300> KDirImageFolder1;
	
    virtual TSize MinimumSize();
    void SetData(TInt aIsmark); // Don't take ownership
	void SetIconsArray();

	TInt Start(TDesC& aFilename);
	void MarkSelectedVideo();
	void MarkUnMarkSelectedItem();
	void MarkAllItems();
	void UnMarkAllItems();
	void ResetData();
	void ClearData();
	void SetTitle();
	void GetActualVideoCount();
	TInt IsImageSelected();
	void SendVideoToServer();
	
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
	TBuf<160> comment;
	TInt8 iNewFolder;
	TBool iIsFileOpen;
	TInt iVideosCount;
	TRect innerRect;
	TBool iIsMark;
	TInt iCursorPos;
	
	CDesCArray *ImageFilearray;
	CDesCArray *ImageDatearray;
	CDesCArray *ImageFilePatharray;
	CArrayFix<TInt>* iSelectedVideos;
	CArrayFix<TInt>* iVideoListArray;

	CColumnListBoxData* colData;
    };


#endif // __VIDEOSCREENLISTBOX_H
