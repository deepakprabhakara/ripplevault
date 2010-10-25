/*
* ============================================================================
*  Name     : ContactsView.cpp
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CContactsView implementation
*  Copyright: PurpleAce
* ============================================================================
*/


#include <cknenv.h>
#include <eikenv.h>
#include <ckntitle.h>

#include "ContactsView.h"
#include "RippleVaultUtils.h"
#include <RippleVault_series80.rsg>
#include "RippleVault_series80.hrh"
#include "ContactsSubScreenView.h"
#include "ContactsScreenView.h"
#include "Ripplevaultappui.h"

#include "logo.mbg"
#include "fbs.h"
#include "constants.h"

CContactsView* CContactsView::NewL( const TRect& aRect,CRippleVaultAppUi& aAppUi)
{
    CContactsView* self = new (ELeave) CContactsView(aAppUi);
    CleanupStack::PushL(self);
    self->ConstructL( aRect );
    CleanupStack::Pop();
    return self;
}

CContactsView::CContactsView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
}

void CContactsView::ConstructL( const TRect& aRect )
{
    CreateWindowL();

	itemIndex = 0;
	
	iContactsScreenView = new(ELeave)CContactsScreenView(iAppUi);
    iContactsScreenView->ConstructL(this);
    iContactsScreenView->SetFocus(ETrue, ENoDrawNow);
    iContactsScreenView->SetFocus(ETrue);
    iContactsScreenView->SetObserver(this);

	// Construct rights side "popup window" looking area
    iContactsSubScreenView = new(ELeave)CContactsSubScreenView(iAppUi);
    iContactsSubScreenView->ConstructL(this);
    iContactsSubScreenView->SetObserver(this);
	// Set logical border type to EWindow to get the shadow look (See also
    
	// CknEnv::SetDropShadowToLogicalBorder
    iContactsScreenView->SetBorder(TGulBorder::EWindow);
    iContactsSubScreenView->SetBorder(TGulBorder::EWindow);

    // Set the windows size
    SetRect(aRect);
    // Activate the window, which makes it ready to be drawn
    ActivateL();
}

CContactsView::~CContactsView()
{
	delete iContactsScreenView;
    delete iContactsSubScreenView;
}

TInt CContactsView::CountComponentControls() const
{
    return 2;
}

void CContactsView::SetFocus(TBool aFocus, TDrawNow aDrawNow)
{
	/*
	if (iTitle) 
		{
		iTitle->SetFocus( aFocus, aDrawNow );
		}
	CCoeControl::SetFocus(aFocus, aDrawNow);
	*/
}

CCoeControl* CContactsView::ComponentControl(TInt aIndex) const
{
    switch (aIndex)
        {
		case 0:
            return iContactsScreenView;
			break;
		case 1:
			return iContactsSubScreenView;
			break;
		default:
			return NULL;
        }
}

void CContactsView::Draw(const TRect& /* aRect */) const
{
	CWindowGc& gc = SystemGc();
	gc.Clear();
}

// Handle key events. Called by framework
TKeyResponse CContactsView::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,TEventCode aType)
 {
	if(aType == EEventKey && aKeyEvent.iCode == 13 && iContactsScreenView->IsFocused())
	{
		AddContacts();
		return EKeyWasConsumed;
	}

	// First handle right/left arrow keys. Switch focused control, if needed
    if(aType == EEventKey && iContactsScreenView->IsFocused() &&
       aKeyEvent.iCode == EKeyRightArrow && itemIndex != 4)
    {
        iContactsScreenView->SetFocus(EFalse, EDrawNow);
        iContactsSubScreenView->SetFocus(ETrue, EDrawNow);

		iAppUi.iCBA->DimCommand(EContactsDel, EFalse);
		iAppUi.iCBA->DimCommand(EContactsAdd, ETrue);
		iAppUi.iCBA->DrawNow();

        return EKeyWasConsumed;
    }
    else if(aType == EEventKey && iContactsSubScreenView->IsFocused() &&
       aKeyEvent.iCode == EKeyLeftArrow)
    {
        iContactsScreenView->SetFocus(ETrue, EDrawNow);
        iContactsSubScreenView->SetFocus(EFalse, EDrawNow);

		iAppUi.iCBA->DimCommand(EContactsDel, ETrue);
		iAppUi.iCBA->DimCommand(EContactsAdd, EFalse);
		iAppUi.iCBA->DrawNow();

        return EKeyWasConsumed;
    }
	// Let the focused control handle any other key events
	if(iContactsScreenView->IsFocused())
		return iContactsScreenView->OfferKeyEventL(aKeyEvent, aType);
	else if(iContactsSubScreenView->IsFocused())
		return iContactsSubScreenView->OfferKeyEventL(aKeyEvent, aType);
	else
		return EKeyWasNotConsumed;
}

// Handle search results listbox data or user selection changes. Refresh
// right side view details. This is called by framework, when one of contained
// controls calls their ReportEventL functions.
void CContactsView::HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType)
{
	if(aEventType == EEventStateChanged && aControl == iContactsScreenView)
    {
       // User selected an item from right side listbox. Load the detail
       // data to the right side control
       //TInt itemIndex = iContactsScreenView->CurrentItemIndex();
       if(itemIndex >= 0)
       {
          //iContactsSubScreenView->SetData();
       }
       else
       {
          //iContactsSubScreenView->SetData();
       }
    }
}

// Set the left and right view controls positions. Called by framework
void CContactsView::SizeChanged()
{
    TRect innerRect = Border().InnerRect(Rect()); // This should be same as
                                                  // Rect(), because this
                                                  // window has no border.
    if(iContactsScreenView)
    {
        TRect ContactsScreenView(innerRect);
        ContactsScreenView.SetWidth(innerRect.Width()/3);
        ContactsScreenView.Shrink(5,5); // Allow space for border shadows
        iContactsScreenView->SetRect(ContactsScreenView);
    }
    
	if(iContactsSubScreenView)
    {
        TRect ContactsSubScreenView(innerRect);
        ContactsSubScreenView.iTl.iX = innerRect.iTl.iX + innerRect.Width()/3;
        ContactsSubScreenView.SetWidth(innerRect.Width()/1.5);
        ContactsSubScreenView.Shrink(5,5); // Allow space for border shadows
        iContactsSubScreenView->SetRect(ContactsSubScreenView);
    }
}

TInt CContactsView::GetContactsScreenCurrentItemIndex()
{
	TInt index;
    index = iContactsScreenView->CurrentItemIndex();
	return index;
}

TInt CContactsView::GetContactsSubScreenCurrentItemIndex()
{
    TInt index;
    index = iContactsSubScreenView->CurrentItemIndex();
	return index;
}

void CContactsView::AddContacts()
{
	if(iContactsScreenView->IsFocused())
	{
		if(iAppUi.iStoragePath == 1)
		{
			TBuf<150> contactDetails;	
			TBuf<150> mobileDetails;

			TInt ret;
			TInt id = iContactsScreenView->iContactsIdArray->At(GetContactsScreenCurrentItemIndex());

			ret = iContactsScreenView->GetContacts(id, contactDetails, mobileDetails);
			
			if(ret == 1)
			{
				iContactsSubScreenView->iContactsArray->AppendL(contactDetails);
				iContactsSubScreenView->iMobileArray->AppendL(mobileDetails);
			
				iContactsSubScreenView->SetData();

				iAppUi.SaveSelectiveSmsMailsForS80(contactDetails,mobileDetails);
			}
		}
		else
		{
			if(iContactsSubScreenView->iContactsArray->MdcaCount() < 10)
			{			
				TBuf<150> contactDetails;	
				TBuf<150> mobileDetails;

				TInt ret;
				TInt id = iContactsScreenView->iContactsIdArray->At(GetContactsScreenCurrentItemIndex());

				ret = iContactsScreenView->GetContacts(id, contactDetails, mobileDetails);
				
				if(ret == 1)
				{
					iContactsSubScreenView->iContactsArray->AppendL(contactDetails);
					iContactsSubScreenView->iMobileArray->AppendL(mobileDetails);
				
					iContactsSubScreenView->SetData();
				}
			}
			else
			{
				//CEikonEnv::Static ()->AlertWin(_L("10 Contacts allowed"));
				iAppUi.ShowMessageBoxFromResource(R_SELECT_RECEPIENT);
			}
		}
	}
}
		
void CContactsView::DeleteContacts()
{
	if(iContactsSubScreenView->IsFocused())
	{
		if(iContactsSubScreenView->iContactsArray->MdcaCount() > 0)
		{			
			iContactsSubScreenView->iContactsArray->Delete(GetContactsSubScreenCurrentItemIndex());
			iContactsSubScreenView->iMobileArray->Delete(GetContactsSubScreenCurrentItemIndex());
			
			if(iAppUi.iStoragePath == 1)
				iAppUi.DeleteContactsForSms(GetContactsSubScreenCurrentItemIndex());
			
			iContactsSubScreenView->SetData();	
		}
		else
		{
			//CEikonEnv::Static ()->AlertWin(_L("No Contacts"));
			iAppUi.ShowMessageBoxFromResource(R_SELECT_RECEPIENT);
		}
	}
}

void CContactsView::ShareContacts()
{
	TInt sharecount;

	//if(iContactsSubScreenView->IsFocused())
	{
		sharecount = 0;
		if(iContactsSubScreenView->iContactsArray->MdcaCount() > 0)
		{	
			for(TInt i = 0; i < iContactsSubScreenView->iContactsArray->MdcaCount() ; i++)
			{
				iAppUi.MailId.Append((*iContactsSubScreenView->iContactsArray)[i]);
				iAppUi.MailId.Append(_L(";"));
				
				iAppUi.MailNames.Append((*iContactsSubScreenView->iMobileArray)[i]);
				iAppUi.MailNames.Append(_L(";"));

				sharecount++;
			}

			if(iAppUi.iStoragePath == 2)
				iAppUi.SharingListBoxDialog(sharecount);
			else if(iAppUi.iStoragePath == 3)
			{
				iAppUi.SaveMails(iAppUi.albumName,iAppUi.MailNames,iAppUi.MailId,iAppUi.iMessage,sharecount);
				iAppUi.SendSmsForClientSharing();

				iAppUi.ShowWindow(KViewIdStatusScreen);
				iAppUi.DeleteWindow(KViewIdContactsViewScreen);
				iAppUi.ShowMessageBoxFromResource(R_TEXT_SHARING_APPLICATION_SUCCESS);
				//iAppUi.GetClientSharingAlbumXML();
			}

		}
		else
		{
			//CEikonEnv::Static ()->AlertWin(_L("No Contacts"));
			iAppUi.ShowMessageBoxFromResource(R_SELECT_RECEPIENT);
		}
	}	
}
