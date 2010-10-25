/*
* ============================================================================
*  Name     : CSecureDocument from SecureDocument.h
*  Part of  : Secure
*  Created  : 8/6/2005 by Ezhil
*  Implementation notes:
*     Initial content was generated by Series 60 AppWizard.
*  Version  :
*  Copyright: Purpleace
* ============================================================================
*/

// INCLUDE FILES
#include "SecureDocument.h"
#include "SecureAppUi.h"

// ================= MEMBER FUNCTIONS =======================

// constructor
#ifdef __UIQ__
#ifdef __S80__
CSecureDocument::CSecureDocument(CEikApplication& aApp)
: CEikDocument(aApp) 
#else
CSecureDocument::CSecureDocument(CEikApplication& aApp)
: CQikDocument(aApp) 
#endif
#else
CSecureDocument::CSecureDocument(CEikApplication& aApp)
: CAknDocument(aApp) 
#endif   
    {
    }

// destructor
CSecureDocument::~CSecureDocument()
    {
    }

// EPOC default constructor can leave.
void CSecureDocument::ConstructL()
    {
    }

// Two-phased constructor.
CSecureDocument* CSecureDocument::NewL(
        CEikApplication& aApp)     // CSecureApp reference
    {
    CSecureDocument* self = new (ELeave) CSecureDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }
    
// ----------------------------------------------------
// CSecureDocument::CreateAppUiL()
// constructs CSecureAppUi
// ----------------------------------------------------
//
CEikAppUi* CSecureDocument::CreateAppUiL()
    {
    return new (ELeave) CSecureAppUi;
    }

// End of File  
