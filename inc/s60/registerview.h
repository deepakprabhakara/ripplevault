/*

* ============================================================================

*  Name     : CRegisterView from RegisterView.h

*  Part of  : 

*  Created  : 5/17/2004 by Ezhil

*  Description:

*     Declares view for application.

*  Version  :

*  Copyright: PurpleACE

* ============================================================================

*/



#ifndef REGISTERVIEW_H

#define REGISTERVIEW_H



// INCLUDES

#include <aknview.h>





// CONSTANTS

// UID of view

//const TUid KView4Id = {4};



// FORWARD DECLARATIONS

class CRegisterContainer;

class CRippleVaultAppUi;



// CLASS DECLARATION



/**

*  CRegisterView view class.

* 

*/

class CRegisterView : public CAknView

    {

    public: // Constructors and destructor



        /**

        * EPOC default constructor.

        */

        void ConstructL();



        /**

        * Destructor.

        */

        ~CRegisterView();



    public: // Functions from base classes

        

        /**

        * From ?base_class ?member_description

        */

        TUid Id() const;

	

		CRippleVaultAppUi* iPhotosAppUi;	

        /**

        * From ?base_class ?member_description

        */

        void HandleCommandL(TInt aCommand);



        /**

        * From ?base_class ?member_description

        */

        void HandleClientRectChange();


		void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
		//void UpdateScreen();

    private:



        /**

        * From AknView, ?member_description

        */

        void DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,

            const TDesC8& aCustomMessage);



        /**

        * From AknView, ?member_description

        */

        void DoDeactivate();

		void DisplayFormL();

    private: // Data

        CRegisterContainer* iContainer;

    };



#endif



// End of File

