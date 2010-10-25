/*
* ============================================================================
*  Name     : RippleVaultLeftView.h
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CStatusScreenView header
*  Copyright: PurpleAce
* ============================================================================
*/


#ifndef __StatusScreenView_H
#define __StatusScreenView_H

#include <coecntrl.h>
#include "RippleVaultAppUi.h"

class CCknAppTitle;
class CRippleVaultAppUi;

class CStatusScreenView : public CCoeControl
    {
	public:
		static CStatusScreenView* NewL( const TRect& aRect,CRippleVaultAppUi& aAppUi );
		~CStatusScreenView();
		void ConstructL( const TRect& aRect );
		void DrawFocusShadow() const;
		virtual void SetFocus(TBool aFocus, TDrawNow aDrawNow=ENoDrawNow);

		CRippleVaultAppUi& iAppUi;

		CStatusScreenView(CRippleVaultAppUi& aAppUi);

		void ApplicationStatus(TDesC& stat, TDesC& stat1, TDesC& stat2, TDesC& stat3, TDesC& stat4, TDesC& stat5, TDesC& stat6, TBool aIsDone, TBool aQueuedInfo, TDesC& aUploadedItems, TDesC& aUploadedContacts, TDesC& aUploadedPhotos);

		void StartTimer();

		/** periodic timer used to control the animation */
		CPeriodic* iPeriodicTimer;
	
		/*!
		  @function DoPeriodTask
  
		  @discussion Called by period task static function
		  */
			void DoPeriodTask();

		/*!
		  @function Period
  
		  @discussion Call back function for a periodic timer
		  @param aPtr a parameter passed to the timer when the timer is started
		  */
			static TInt Period(TAny* aPtr);

		//TInt iPosX;
		
		//TInt iPosY;
		
		TInt iFlag;

	private: // From CCoeControl
		void Draw(const TRect& /*aRect*/) const;
		TInt CountComponentControls() const;
		CCoeControl* ComponentControl(TInt aIndex) const;

	private:
		CCknAppTitle* iTitle;

		TBuf<50> iLabelTitle;          // example label      
		TBuf<50> iLabelPendingItems;          // example label
		TBuf<50> iLabelContactsCount;          // example label
		TBuf<50> iLabelPhotosCount;          // example label
		TBuf<50> iLabelVersion;          // example label      
		TBuf<50> iLabelAppStatus;          // example label      
		TBuf<50> iLabelErrStatus;          // example label      

		TBuf<50> iLabelUploadedItems;          // example label
		TBuf<50> iLabelUploadedContactsCount;          // example label
		TBuf<50> iLabelUploadedPhotosCount;          // example label

		TBool isDoingSomething;
		TBool isQueuedInfo;

    };

#endif
