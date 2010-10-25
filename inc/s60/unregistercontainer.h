/*

* ============================================================================

*  Name     : CUnRegisterContainer from UnRegisterContainer.h

*  Part of  : Test

*  Created  : 5/17/2004 by Ezhil

*  Description:

*     Declares container control for application.

*  Version  :

*  Copyright: PurpleACE

* ============================================================================

*/



#ifndef UNREGISTERCONTAINER_H

#define UNREGISTERCONTAINER_H



// INCLUDES

#include <coecntrl.h>

   

// FORWARD DECLARATIONS

class CEikLabel;        // for example labels

class CRippleVaultAppUi;

// CLASS DECLARATION



/**

*  CUnRegisterContainer  container control class.

*  

*/

class CUnRegisterContainer : public CCoeControl, MCoeControlObserver

    {

    public: // Constructors and destructor

        

        /**

        * EPOC default constructor.

        * @param aRect Frame rectangle for container.

        */

        void ConstructL(const TRect& aRect);



        /**

        * Destructor.

        */

        ~CUnRegisterContainer();



    public: // New functions
		CRippleVaultAppUi* iPhotosAppUi;


    public: // Functions from base classes



    private: // Functions from base classes



       /**

        * From CoeControl,SizeChanged.

        */

        void SizeChanged();



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

        

        CEikLabel* iLabel;          // example label      

    };



#endif



// End of File

