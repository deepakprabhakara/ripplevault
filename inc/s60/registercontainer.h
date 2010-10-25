/*

* ============================================================================

*  Name     : CRegisterContainer from RegisterContainer.h

*  Part of  : 

*  Created  : 5/17/2004 by Ezhil

*  Description:

*     Declares container control for application.

*  Version  :

*  Copyright: PurpleACE

* ============================================================================

*/



#ifndef REGISTERCONTAINER_H

#define REGISTERCONTAINER_H



// INCLUDES

#include <coecntrl.h>

 



// CLASS DECLARATION
class CEikLabel;        // for example labels
class CRippleVaultAppUi;

/**

*  CRegisterContainer  container control class.

*  

*/

class CRegisterContainer : public CCoeControl, MCoeControlObserver

    {

    public: // Constructors and destructor

        

        /**

        * EPOC default constructor.

        * @param aRect Frame rectangle for container.

        */

        void ConstructL(const TRect& aRect);

		

		void ApplicationStatus();

		TBuf<20> status;

		CRippleVaultAppUi* iPhotosAppUi;

		TBool isError;
		TBool isUpgrade;

		TInt iErrCode;
		TInt iStatusCode;

		TInt iPendingCountContacts;
		TInt iPendingCountPhotos;
		TInt iPendingCountVideos;
		TInt iPendingCountSms;

		TBool isQueuedInfo;

		TBool isDoingSomething;
        /**

        * Destructor.

        */

        ~CRegisterContainer();



    public: // New functions

		//void UpdateScreen();

    public: // Functions from base classes

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

    private: // Functions from base classes



       /**

        * From CoeControl,SizeChanged.

        */

        //void SizeChanged();



       /**

        * From CoeControl,CountComponentControls.

        */

        TInt CountComponentControls() const;



       /**

        * From CCoeControl,ComponentControl.

        */

        CCoeControl* ComponentControl(TInt aIndex) const;



       /**

        * From CCoeControl,Draw.

        */

        void Draw(const TRect& aRect) const;



       /**

        * From ?base_class ?member_description

        */

        // event handling section

        // e.g Listbox events

        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

        

    private: //data
/*		CEikLabel* iLabelTitle;          // example label      
		CEikLabel* iLabelErrorStatus;          // example label      
		CEikLabel* iLabelPendingItems;          // example label      
		//CEikLabel* iLabel3;          // example label      
		CEikLabel* iLabelErrorTitle;          // example label      
		CEikLabel* iLabelVersion;          // example label      
		CEikLabel* iLabelAppStatus;          // example label      
*/
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
    };



#endif



// End of File

