/*
* ============================================================================
*  Name     : PhotoListView.h
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CPhotoListView header
*  Copyright: PurpleAce
* ============================================================================
*/


#ifndef __PHOTOLISTVIEW_H
#define __PHOTOLISTVIEW_H

#include <coecntrl.h>
#include "RippleVaultAppUi.h"
#include <eikbctrl.h>
#include <ckndgtrg.h>

class CCknAppTitle;
class CRippleVaultAppUi;
class CPhotoScreenListBox;

class CPhotoListView : public CEikBorderedControl, public MCoeControlObserver
                            
    {
	public:
		static CPhotoListView* NewL( const TRect& aRect,CRippleVaultAppUi& aAppUi);
		~CPhotoListView();
		void ConstructL( const TRect& aRect );
		void DrawFocusShadow() const;
		virtual void SetFocus(TBool aFocus, TDrawNow aDrawNow=ENoDrawNow);

		CRippleVaultAppUi& iAppUi;

		CPhotoListView(CRippleVaultAppUi& aAppUi);

		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

		void MarkUnMarkSelectedItem();
		void MarkAllItems();
		void UnMarkAllItems();
		void SelectFolder();
		TInt IsImageSelected();
		void SendPhotoToServer();

	private: // From CCoeControl
		void Draw(const TRect& /*aRect*/) const;
		TInt CountComponentControls() const;
		CCoeControl* ComponentControl(TInt aIndex) const;
	    void SizeChanged();
		TInt itemIndex;
	    void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEventType);

	private:
	    CPhotoScreenListBox* iPhotoScreenListBox;
		
		TBuf<300> iFilename;
    };

#endif
