/*
* ============================================================================
*  Name     : CSecureApp from SecureApp.h
*  Part of  : Secure
*  Created  : 8/6/2005 by Ezhil
*  Description:
*     Declares main application class.
*  Version  :
*  Copyright: Purpleace
* ============================================================================
*/

#ifndef SECUREAPP_H
#define SECUREAPP_H

// INCLUDES
#ifdef __UIQ__
#ifdef __S80__
#include <eikapp.h>
#else
#include <qikapplication.h>
#endif
#else
#include <aknapp.h>
#endif

// CONSTANTS
// UID of the application
const TUid KUidSecure = { 0x102032D1 };

// CLASS DECLARATION

/**
* CSecureApp application class.
* Provides factory to create concrete document object.
* 
*/
#ifdef __UIQ__
#ifdef __S80__
class CSecureApp : public CEikApplication
#else
class CSecureApp : public CQikApplication
#endif
#else
class CSecureApp : public CAknApplication
#endif
    {
    
    public: // Functions from base classes
    private:

        /**
        * From CApaApplication, creates CSecureDocument document object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
        
        /**
        * From CApaApplication, returns application's UID (KUidSecure).
        * @return The value of KUidSecure.
        */
        TUid AppDllUid() const;
    };

#endif

// End of File

