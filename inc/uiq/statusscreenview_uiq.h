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


#ifndef __STATUSSCREENVIEW_UIQ_H
#define __STATUSSCREENVIEW_UIQ_H

#include <coecntrl.h>
#include "RippleVaultAppUi.h"

class CRippleVaultAppUi;

class CStatusScreenView : public CCoeControl
    {
	public:
		static CStatusScreenView* NewL( const TRect& aRect,CRippleVaultAppUi& aAppUi );
		~CStatusScreenView();
		void ConstructL( const TRect& aRect );
		virtual void SetFocus(TBool aFocus, TDrawNow aDrawNow=ENoDrawNow);

		CRippleVaultAppUi& iAppUi;

		CStatusScreenView(CRippleVaultAppUi& aAppUi);
	
		TBuf<20> status;
		TBuf<50> status1;
		TBuf<50> status2;
		TBuf<50> status3;
		TBuf<50> status4;
		TBuf<50> status5;
		TBuf<50> status6;

		TBuf<50> iLabelUploadedItems;          // example label
		TBuf<50> iLabelUploadedContactsCount;          // example label
		TBuf<50> iLabelUploadedPhotosCount;          // example label

		TBool isDoingSomething;
		TBool isQueuedInfo;

	    void ApplicationStatus(TDesC& stat, TDesC& stat1, TDesC& stat2, TDesC& stat3, TDesC& stat4, TDesC& stat5, TDesC& stat6, TBool aIsDoing, TBool aQueuedInfo, TDesC& stat7, TDesC& stat8, TDesC& stat9);

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
		
    };

#endif
