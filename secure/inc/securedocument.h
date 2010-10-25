/*
* ============================================================================
*  Name     : CSecureDocument from SecureDocument.h
*  Part of  : Secure
*  Created  : 8/6/2005 by Ezhil
*  Description:
*     Declares document for application.
*  Version  :
*  Copyright: Purpleace
* ============================================================================
*/

#ifndef SECUREDOCUMENT_H
#define SECUREDOCUMENT_H

// INCLUDES
#ifdef __UIQ__
#ifdef __S80__
#include <eikdoc.h>
#else
#include <qikdocument.h>
#endif
#else
#include <akndoc.h>
#endif
   
// CONSTANTS

// FORWARD DECLARATIONS
class  CEikAppUi;

// CLASS DECLARATION

/**
*  CSecureDocument application class.
*/
#ifdef __UIQ__
#ifdef __S80__
class CSecureDocument : public CEikDocument
#else
class CSecureDocument : public CQikDocument
#endif
#else
class CSecureDocument : public CAknDocument
#endif
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CSecureDocument* NewL(CEikApplication& aApp);

        /**
        * Destructor.
        */
        virtual ~CSecureDocument();

    public: // New functions

    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * EPOC default constructor.
        */
        CSecureDocument(CEikApplication& aApp);
        void ConstructL();

    private:

        /**
        * From CEikDocument, create CSecureAppUi "App UI" object.
        */
        CEikAppUi* CreateAppUiL();
    };

#endif

// End of File

