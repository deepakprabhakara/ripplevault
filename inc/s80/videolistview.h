/*
* ============================================================================
*  Name     : VideoListView.h
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CVideoListView header
*  Copyright: PurpleAce
* ============================================================================
*/


#ifndef __VIDEOLISTVIEW_H
#define __VIDEOLISTVIEW_H

#include <coecntrl.h>
#include "RippleVaultAppUi.h"
#include <eikbctrl.h>
#include <ckndgtrg.h>

class CCknAppTitle;
class CRippleVaultAppUi;
class CVideoScreenListBox;

class CVideoListView : public CEikBorderedControl, public MCoeControlObserver
                            
    {
	public:
		static CVideoListView* NewL( const TRect& aRect,CRippleVaultAppUi& aAppUi);
		~CVideoListView();
		void ConstructL( const TRect& aRect );
		void DrawFocusShadow() const;
		virtual void SetFocus(TBool aFocus, TDrawNow aDrawNow=ENoDrawNow);

		CRippleVaultAppUi& iAppUi;

		CVideoListView(CRippleVaultAppUi& aAppUi);

		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

		void MarkUnMarkSelectedItem();
		void MarkAllItems();
		void UnMarkAllItems();
		void SelectFolder();
		TInt IsImageSelected();
		void SendVideoToServer();

	private: // From CCoeControl
		void Draw(const TRect& /*aRect*/) const;
		TInt CountComponentControls() const;
		CCoeControl* ComponentControl(TInt aIndex) const;
	    void SizeChanged();
		TInt itemIndex;
	    void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEventType);

	private:
	    CVideoScreenListBox* iVideoScreenListBox;
		
		TBuf<300> iFilename;
    };

#endif
