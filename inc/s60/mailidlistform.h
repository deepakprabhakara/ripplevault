#ifndef MAILIDLISTFORM_H
#define MAILIDLISTFORM_H

#include <aknform.h>
#include <coecntrl.h>
#include <coemain.h>
#include <aknqueryvaluetext.h>
#include <aknview.h>

#include <e32cons.h>
#include <eikspane.h>

#include <aknnavide.h>
#include <aknnavi.h>

class CRippleVaultAppUi;
class CFindBoxContainer;
class CAknQueryValueTextArray;
class CAknQueryValueText;


class CMailIdListForm : public CAknForm
{

public:
	static CMailIdListForm* NewL();

	virtual ~CMailIdListForm();

	CMailIdListForm();
	CRippleVaultAppUi* iPhotosAppUi;	

	void ConstructL();

	virtual void ProcessCommandL(TInt aResourceId);

	virtual void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane *aMenuPane);

private:
	void PreLayoutDynInitL();

	void PostLayoutDynInitL();

	void ConfigurePop();

	TBool QuerySaveChangesL();
	
	TBool OkToExitL( TInt aButtonId );
		
public:
	TBool SaveFormDataL();

	void DoNotSaveFormDataL();
	
public:	

	CDesCArray* iTextArray[1];

    CAknQueryValueTextArray* iValueTextArray[1];
    
	CAknQueryValueText* iTextValues[1];
};
#endif