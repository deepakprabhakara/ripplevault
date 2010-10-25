/*
* ============================================================================
*  Name     : CUnRegisterView from UnRegisterView.h
*  Part of  : 
*  Created  : 5/17/2004 by Ezhil
*  Description:
*     Declares view for application.
*  Version  :
*  Copyright: PurpleACE
* ============================================================================
*/

#ifndef UNREGISTERVIEW_H
#define UNREGISTERVIEW_H

// INCLUDES
#include <aknview.h>
#include <aknwaitdialog.h> 

// CONSTANTS
// UID of view
//const TUid KView3Id = {3};

// FORWARD DECLARATIONS
class CUnRegisterContainer;
class CRippleVaultAppUi;

// CLASS DECLARATION
/**
*  CUnRegisterView view class.
* 
*/

class CUnRegisterView : public CAknView
    {
    public: // Constructors and destructor
        /**
        * EPOC default constructor.
        */
        void ConstructL();
        /**
        * Destructor.
        */
        ~CUnRegisterView();
    public: // Functions from base classes
		//Registration				
		TBuf8<1024> buffer;			
		//void RegisterRipple(TInt Reg);
        CRippleVaultAppUi* iPhotosAppUi;
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
        CUnRegisterContainer* iContainer;		
    };
#endif

// End of File
