

#ifndef __RIPPLEVAULT_APPLICATION_H__
#define __RIPPLEVAULT_APPLICATION_H__

#ifdef __UIQ__
#include <eikapp.h>
#ifndef __S80__
#include <qikapplication.h>
#endif
#else
#include <aknapp.h>
#endif


/*! 
  @class CRippleVaultApplication
  
  @discussion An instance of CRippleVaultApplication is the application part of the AVKON
  application framework for the RippleVault application
  */
#ifdef __UIQ__
#ifdef __S80__
class CRippleVaultApplication : public CEikApplication
#else
class CRippleVaultApplication : public CQikApplication
#endif
#else
class CRippleVaultApplication : public CAknApplication
#endif
    {
private:

/*! 
  @function CreateDocumentL
  
  @discussion Create a CApaDocument object and return a pointer to it
  @result a pointer to the created document
  */
    CApaDocument* CreateDocumentL();

/*! 
  @function AppDllUid
  
  @discussion Returns the application DLL UID value
  @result the UID of this Application/Dll
  */
    TUid AppDllUid() const;
    };

#endif // __RIPPLEVAULT_APPLICATION_H__
