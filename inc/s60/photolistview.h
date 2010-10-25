/*

* ============================================================================

*  Name     : CPhotoListView from PhotoListView.h

*  Part of  : To Show Thumbnails Photo List

*  Created  : 17/06/2004 by Ezhil

*  Description:

*     Declares view for application.

*  Version  :

*  Copyright: PurpleACE

* ============================================================================

*/



#ifndef PHOTOLISTVIEW_H

#define PHOTOLISTVIEW_H



// INCLUDES

#include "RippleVaultappui.h"
#include "Photosengine.h"
#include <aknview.h>





// CONSTANTS

// UID of view

//const TUid KView5Id = {5};



// FORWARD DECLARATIONS

class CPhotoListContainer;



// CLASS DECLARATION



/**

*  CPhotoListView view class.

* 

*/

class CPhotoListView : public CAknView

    {

    public: // Constructors and destructor



        /**

        * EPOC default constructor.

        */

        void ConstructL();



        /**

        * Destructor.

        */

        ~CPhotoListView();



    public: // Functions from base classes

        

        /**

        * From ?base_class ?member_description

        */

        TUid Id() const;



        /**

        * From ?base_class ?member_description

        */

        void HandleCommandL(TInt aCommand);



        /**

        * From ?base_class ?member_description

        */

        void HandleClientRectChange();

		void GetFolder();
		void SendPhotoToServer();
		void StoreFolder();
		void ReadandSendData();
		void DataWritten();
		TInt GetPhotoCount();



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



    private: // Data

        CPhotoListContainer* iContainer;
	public:
			CRippleVaultAppUi* iPhotosAppUi;
			TInt iFolderStatus;
			TInt iNewFolder;
			TInt Count;
			TBuf<160> comment;
			RFs iRFsession;
			TInt iPhotoSendMode;
			TBool iPublic_Photo;

    };



#endif



// End of File

