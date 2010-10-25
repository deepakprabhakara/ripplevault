

#ifndef __RIPPLEVAULTDOCUMENT_H__
#define __RIPPLEVAULTDOCUMENT_H__

#ifdef __UIQ__
#include <eikdoc.h>
	#ifndef __S80__
	#include <qikdocument.h>
	#include <qikapplication.h>
	#endif
#else
#include <eikdoc.h>
#endif

// Forward references
class CRippleVaultAppUi;
#ifdef __UIQ__
#ifdef __S80__
class CEikApplication;
#else
class CQikApplication;
#endif
#else
class CEikApplication;
#endif


/*! 
  @class CRippleVaultDocument
  
  @discussion An instance of class CRippleVaultDocument is the Document part of the AVKON
  application framework for the RippleVault application
  */
#ifdef __UIQ__
#ifdef __S80__
class CRippleVaultDocument : public CEikDocument
#else
class CRippleVaultDocument : public CQikDocument
#endif
#else
class CRippleVaultDocument : public CEikDocument
#endif
    {
public:

#ifdef __UIQ__
#ifdef __S80__
/*!
  @function NewL
  
  @discussion Construct a CRippleVaultDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CRippleVaultDocument
  */
    static CRippleVaultDocument* NewL(CEikApplication& aApp);

/*!
  @function NewLC
  
  @discussion Construct a CRippleVaultDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CRippleVaultDocument
  */
    static CRippleVaultDocument* NewLC(CEikApplication& aApp);
#else
/*!
  @function NewL
  
  @discussion Construct a CRippleVaultDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CRippleVaultDocument
  */
    static CRippleVaultDocument* NewL(CQikApplication& aApp);

/*!
  @function NewLC
  
  @discussion Construct a CRippleVaultDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CRippleVaultDocument
  */
    static CRippleVaultDocument* NewLC(CQikApplication& aApp);
#endif
#else
/*!
  @function NewL
  
  @discussion Construct a CRippleVaultDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CRippleVaultDocument
  */
    static CRippleVaultDocument* NewL(CEikApplication& aApp);

/*!
  @function NewLC
  
  @discussion Construct a CRippleVaultDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CRippleVaultDocument
  */
    static CRippleVaultDocument* NewLC(CEikApplication& aApp);
#endif
/*!
  @function ~CRippleVaultDocument
  
  @discussion Destroy the object and release all memory objects
  */
    ~CRippleVaultDocument();

private:

/*!
  @function ConstructL
  
  @discussion Perform the second phase construction of a CRippleVaultDocument object
  */
    void ConstructL();
#ifdef __UIQ__
#ifdef __S80__
/*!
  @function CRippleVaultDocument
  
  @discussion Perform the first phase of two phase construction 
  @param aApp application creating this document
  */
    CRippleVaultDocument(CEikApplication& aApp);
#else
/*!
  @function CRippleVaultDocument

  @discussion Perform the first phase of two phase construction 
  @param aApp application creating this document
  */
    CRippleVaultDocument(CQikApplication& aApp);
#endif
#else
/*!
  @function CRippleVaultDocument
  
  @discussion Perform the first phase of two phase construction 
  @param aApp application creating this document
  */
    CRippleVaultDocument(CEikApplication& aApp);
#endif
/*!
  @function CreateAppUiL 
  
  @discussion Create a CRippleVaultAppUi object and return a pointer to it
  @result a pointer to the created instance of the AppUi created
  */
    CEikAppUi* CreateAppUiL();
    };


#endif // __RIPPLEVAULTDOCUMENT_H__
