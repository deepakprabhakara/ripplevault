/*
* ============================================================================
*  Name     : CMailIdContainer from MailIdContainer.h
*  Part of  : RippleVault
*  Created  : 6/16/2004 by Ezhil
*  Implementation notes:
*     Initial content was generated by Series 60 AppWizard.
*  Version  :
*  Copyright: PurpleAce
* ============================================================================
*/

// INCLUDE FILES
#include "MailIdContainer.h"
#include <ripplevault.rsg>
#include "RippleVaultAppUi.h"
#include <RippleVault.rsg>

// ---------------------------------------------------------
// CMailIdContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CMailIdContainer::ConstructL(const TRect& aRect)
{
    CreateWindowL();	
	//SetRect(aRect);	
	ActivateL();
}

// Destructor
CMailIdContainer::~CMailIdContainer()
    {
    }

// ---------------------------------------------------------
// CMailIdContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CMailIdContainer::SizeChanged()
    {
		// TODO: Add here control resize code etc.		
    }

// ---------------------------------------------------------
// CMailIdContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CMailIdContainer::CountComponentControls() const
    {
    return 0; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CMailIdContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CMailIdContainer::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CMailIdContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CMailIdContainer::Draw(const TRect& aRect) const
    {
    }

// ---------------------------------------------------------
// CMailIdContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CMailIdContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }



// End of File  