/*
* ============================================================================
*  Name     : CMailIdView from MailIdView.h
*  Part of  : 
*  Created  : 6/16/2004 by Ezhil
*  Description:
*     Declares view for application.
*  Version  :
*  Copyright: PurpleACE
* ============================================================================
*/

#ifndef MAILIDVIEW_H
#define MAILIDVIEW_H

// INCLUDES
#include <aknview.h>


// CONSTANTS
// UID of view
//const TUid KView8Id = {8};

// FORWARD DECLARATIONS
class CMailIdContainer;
class CRippleVaultAppUi;

// CLASS DECLARATION

/**
*  CMailIdView view class.
* 
*/
class CMailIdView : public CAknView
    {
    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */
        void ConstructL();
		CRippleVaultAppUi* iPhotosAppUi;	
        /**
        * Destructor.
        */
        ~CMailIdView();

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
        CMailIdContainer* iContainer;
    };

#endif

// End of File
