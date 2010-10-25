/*******************************************************************

 Copyright(c) 2000-2005 

 PurpleACE Wireless Solutions Pvt. Ltd.
 www.purpleace.com
 #708, A Wing, Carlton Towers, Airport Road, Bangalore - 560017,
 India
 All rights reserved.

 This software is the confidential and proprietary information of
 PurpleACE Wireless Solutions Pvt. Ltd. ("Confidential Information").
 You shall not disclose such Confidential Information and
 shall use it only in accordance with the terms of the license
 Agreement you entered into with PurpleACE Wireless Solutions Pvt. Ltd..

 Project Name               : Ripple Vault Symbian Client
 Module Name				: HelpContainer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Displays help for Series 60
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/


// INCLUDE FILES
#include <eiklabel.h>
#include "RippleVaultAppUi.h"
#include "HelpContainer.h"
#include <ripplevault.rsg>
#include <txtfmlyr.h> 
#include <aknmessagequerydialog.h>

#include <aknnavi.h>

#include "logo.mbg"

// ---------------------------------------------------------
// CHelpContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------

void CHelpContainer::ConstructL()
{
    CreateWindowL();
//	iCursorPos=0;	
	//TSize Size(176,144);
	
	/*	
	TResourceReader reader;
	iCoeEnv->CreateResourceReaderLC(reader, R_HELP_BOX);
    iEdwin = new (ELeave) CEikEdwin;
    iEdwin->SetContainerWindowL(*this);
    iEdwin->ConstructFromResourceL(reader);
    CleanupStack::PopAndDestroy();  // Resource reader
    iEdwin->SetExtent(TPoint(0,0), Size);
	
	CCharFormatLayer* FormatLayer=CEikonEnv::NewDefaultCharFormatLayerL();

	TCharFormat charFormat;
	TCharFormatMask charFormatMask;
	FormatLayer->Sense(charFormat, charFormatMask);

	charFormat.iFontSpec.iHeight = 3;
	charFormat.iFontPresentation.iTextColor=KRgbBlack;

	charFormatMask.SetAttrib(EAttColor);

	FormatLayer->SetL(charFormat, charFormatMask);

	iEdwin->SetCharFormatLayer(FormatLayer);		

	iEdwin->SetTextL(&text);

	iEdwin->SetFocus(ETrue);*/

	TRect fullScreen(TPoint(0,44),TPoint(176,188));
    SetRect(fullScreen);	
	ActivateL();
}

// Destructor
CHelpContainer::~CHelpContainer()
{
    //delete iEdwin;
}

// ---------------------------------------------------------
// CHelpContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CHelpContainer::SizeChanged()
{ 
}

// ---------------------------------------------------------
// CHelpContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CHelpContainer::CountComponentControls() const
{
    return 0; // return nbr of controls inside this container
}

// ---------------------------------------------------------
// CHelpContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CHelpContainer::ComponentControl(TInt aIndex) const
{
    switch ( aIndex )
	{
	//case 0:
	//	return iEdwin;
	default:
		return NULL;
	}
}

// ---------------------------------------------------------
// CHelpContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CHelpContainer::Draw(const TRect& aRect) const
{
    CWindowGc& gc = SystemGc();
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushColor(KRgbWhite);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(aRect);	

	const CFont* fontUsed;    
    fontUsed = iEikonEnv->LegendFont();
	gc.UseFont(fontUsed);

	CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KLogoMbmFileName);

	User::LeaveIfError(bitmap->Load(path, EMbmLogoLogo));
	TPoint pos(40,30);
	gc.BitBlt(pos, bitmap);	
	CleanupStack::PopAndDestroy();

	//TBuf<50> msg;
	
	//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)));
	//gc.DrawText(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),TPoint(40,30));

	//msg.Copy(*(iEikonEnv->AllocReadResourceL(R_PURPLEACE_WEB)));
	gc.DrawText(*(iEikonEnv->AllocReadResourceL(R_PURPLEACE_WEB)),TPoint(30,125));
}

// ---------------------------------------------------------
// CHelpContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CHelpContainer::HandleControlEventL(
	CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
{
    // TODO: Add your control event handler code here
}
/*
TKeyResponse CHelpContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent, TEventCode aType)
 {
    if (iEdwin)
        {
        if (iEdwin->IsFocused())
            {
				switch ( aKeyEvent.iCode ) // The code of key event is...
				{
					case EKeyUpArrow:            						
						return iEdwin->OfferKeyEventL(aKeyEvent, aType);
						break; 
					case EKeyDownArrow:						
						return iEdwin->OfferKeyEventL(aKeyEvent, aType);
						break;
				}				
            }
        }    
    return EKeyWasNotConsumed;
}
*/
void CHelpContainer::AboutRippleVault()
{
	CAknMessageQueryDialog* abtdlg;

	if(iHelpFlag == 1)
	{
/*		text.Copy(_L("Ripple Vault is a phonebook, photos / videos backup application."));
		text.Append(_L("The contacts in your phonebook and the photos / videos on your phone are transferred "));
		text.Append(_L("to your secure Vault on our server. Deleting photos / videos from phone to free up space will NOT delete them."));
*/
	
		abtdlg= CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_HELP_RIPPLE_VAULT)));
		abtdlg->PrepareLC(R_UPGRADE_QUERY);
		abtdlg->SetHeaderTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)));
	}
	else if(iHelpFlag == 2)
	{
/*		text.Copy(_L("An exact replica of the contacts stored in your phone memory is maintained in your Vault. "));
		text.Append(_L("Changes to contacts on your phone will automatically reflect in your Vault phonebook & vice versa. "));
		text.Append(_L("You can access your Vault phone book through a user friendly web interface by logging on to www.myRipple.com.ph. "));
*/
		abtdlg= CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_HELP_CONTACTSBACKUP)));
		abtdlg->PrepareLC(R_UPGRADE_QUERY);
		abtdlg->SetHeaderTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TITLE_HELP_CONTACTSBACKUP)));
	}
	else if(iHelpFlag == 3)
	{
/*		text.Copy(_L("In case you change your phone, you can simply download & install the Ripple Vault application to your new phone by "));
		text.Append(_L("clicking on the Change Handset link after logging into your Vault account on the web. Once the application gets into "));
		text.Append(_L("the 'Running' mode, your contacts will automatically be restored to your new phone."));
*/
		abtdlg= CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_HELP_RESTORECONTACTS)));
		abtdlg->PrepareLC(R_UPGRADE_QUERY);
		abtdlg->SetHeaderTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TITLE_HELP_RESTORECONTACTS)));
	}
	else if(iHelpFlag == 4)
	{
/*		text.Copy(_L("Every time you take a photo or a video it is automatically backed up to your Vault. "));
		text.Append(_L("You are prompted to choose the album you wish to publish to & the description for the photo you captured & the "));
		text.Append(_L("photo is seamlessly uploaded to the server. You can additionally go to options -> settings to choose various levels of "));
		text.Append(_L("intrusiveness, to make backup as easy or as detailed as you wish to. You may also backup any of your existing photos / videos "));
		text.Append(_L("to your Vault. Simply go to options -> publish & choose Phone Memory or MMC, depending on where the photos you wish to upload are stored."));
*/

		text.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_HELP_PUBLISHBACKUP)));
		text.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_HELP_PUBLISHBACKUP1)));

		abtdlg= CAknMessageQueryDialog::NewL(text);
		abtdlg->PrepareLC(R_UPGRADE_QUERY);
		abtdlg->SetHeaderTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TITLE_HELP_PUBLISHBACKUP)));
	}
	else if(iHelpFlag == 5)
	{
/*		text.Copy(_L("You can view all the albums / photos / videos on your vault over WAP or web. Simply go "));
		text.Append(_L("to Options -> go to WAP gallery on you Ripple Vault application and you will be taken to your Vault on the WAP browser on your phone. "));
		text.Append(_L("You may also view your albums / photos / videos over the web. Visit http://ripplevault.purpleace.com "));		
*/

		abtdlg= CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_HELP_VIEWPHOTOSVIDEOS)));
		abtdlg->PrepareLC(R_UPGRADE_QUERY);
		abtdlg->SetHeaderTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TITLE_HELP_VIEWPHOTOSVIDEOS)));

	}	
	else if(iHelpFlag == 6)
	{
/*		text.Copy(_L("You can view all the albums / photos / videos on your vault over WAP or web. Simply go "));
		text.Append(_L("to Options -> go to WAP gallery on you Ripple Vault application and you will be taken to your Vault on the WAP browser on your phone. "));
		text.Append(_L("You may also view your albums / photos / videos over the web. Visit http://ripplevault.purpleace.com "));		
*/

		abtdlg= CAknMessageQueryDialog::NewL(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_HELP_SMSBACKUP)));
		abtdlg->PrepareLC(R_UPGRADE_QUERY);
		abtdlg->SetHeaderTextL(*(CEikonEnv::Static()->AllocReadResourceL(R_TITLE_HELP_SMSBACKUP)));
	}	
	if (abtdlg->RunLD()) 
	{					
	}					
}

/*TKeyResponse CHelpContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent, TEventCode aType)
 {
    if (iEdwin)
        {
        if (iEdwin->IsFocused())
            {
				if(iCursorPos == -1)
				{
					iEdwin->SetCursorPosL(130,EFalse );
					iCursorPos++;
				}
				switch ( aKeyEvent.iCode ) // The code of key event is...
				{
					case EKeyUpArrow:            						
						if(iEdwin->CursorPos() == 179)
						{
							iEdwin->SetCursorPosL(0,EFalse );
						}
						else if(iEdwin->CursorPos() == 155)
						{
							iEdwin->SetCursorPosL(0,EFalse );
						}
						else if(iEdwin->CursorPos() == 352 || iEdwin->CursorPos() == 327)
						{
							iEdwin->SetCursorPosL(179,EFalse );
						}
						else if(iEdwin->CursorPos() == 487 || iEdwin->CursorPos() == 467)
						{
							iEdwin->SetCursorPosL(352,EFalse );
						}
						else if(iEdwin->CursorPos() == 327)
						{
							iEdwin->SetCursorPosL(278,EFalse );
						}
						else if(iEdwin->CursorPos() == 665 || iEdwin->CursorPos() == 642)
						{
							iEdwin->SetCursorPosL(487,EFalse );
						}
						else if(iEdwin->CursorPos() == 842 || iEdwin->CursorPos() == 814)
						{
							iEdwin->SetCursorPosL(665,EFalse );
						}
						else if(iEdwin->CursorPos() == 998 || iEdwin->CursorPos() == 984)
						{
							iEdwin->SetCursorPosL(842,EFalse );
						}
						else if(iEdwin->CursorPos() == 1181 || iEdwin->CursorPos() == 1158)
						{
							iEdwin->SetCursorPosL(997,EFalse );
						}
						else if(iEdwin->CursorPos() == 1351 || iEdwin->CursorPos() == 1328)
						{
							iEdwin->SetCursorPosL(1181,EFalse );
						}
						else if(iEdwin->CursorPos() == 1469 || iEdwin->CursorPos() == 1441)
						{
							iEdwin->SetCursorPosL(1351,EFalse );
						}
						else if(iEdwin->CursorPos() == 1647 || iEdwin->CursorPos() == 1620)
						{
							iEdwin->SetCursorPosL(1469,EFalse );
						}
						else if(iEdwin->CursorPos() == 1815 || iEdwin->CursorPos() == 1800)
						{
							iEdwin->SetCursorPosL(1647,EFalse );
						}
						else if(iEdwin->CursorPos() == 1991 || iEdwin->CursorPos() == 1968)
						{
							iEdwin->SetCursorPosL(1814,EFalse );
						}
						else if(iEdwin->CursorPos() == 2167 || iEdwin->CursorPos() == 2142)
						{
							iEdwin->SetCursorPosL(1991,EFalse );
						}
						else if(iEdwin->CursorPos() == 2316 || iEdwin->CursorPos() == 2289)
						{
							iEdwin->SetCursorPosL(2167,EFalse );
						}
						else if(iEdwin->CursorPos() == 2483 || iEdwin->CursorPos() == 2460)
						{
							iEdwin->SetCursorPosL(2316,EFalse );
						}
						else if(iEdwin->CursorPos() == 2635 || iEdwin->CursorPos() == 2611)
						{
							iEdwin->SetCursorPosL(2483,EFalse );
						}
						else if(iEdwin->CursorPos() == 2813 || iEdwin->CursorPos() == 2783)
						{
							iEdwin->SetCursorPosL(2635,EFalse );
						}
						else if(iEdwin->CursorPos() == 2938 || iEdwin->CursorPos() == 2922)
						{
							iEdwin->SetCursorPosL(2813,EFalse );
						}
						else if(iEdwin->CursorPos() == 3111 || iEdwin->CursorPos() == 3087)
						{
							iEdwin->SetCursorPosL(2937,EFalse );
						}
						else if(iEdwin->CursorPos() == 3274 || iEdwin->CursorPos() == 3261)
						{
							iEdwin->SetCursorPosL(3111,EFalse );
						}
						else if(iEdwin->CursorPos() == 3533)
						{
							iEdwin->SetCursorPosL(3274,EFalse );
						}
						return iEdwin->OfferKeyEventL(aKeyEvent, aType);
						break; 
					case EKeyDownArrow:						
						if(iEdwin->CursorPos() == 0)
						{
							iEdwin->SetCursorPosL(155,EFalse );
						}
						else if(iEdwin->CursorPos() == 130)
						{
							iEdwin->SetCursorPosL(155,EFalse );
						}
						else if(iEdwin->CursorPos() == 179 || iEdwin->CursorPos() == 155)
						{
							iEdwin->SetCursorPosL(327,EFalse );
						}
						else if(iEdwin->CursorPos() == 352 || iEdwin->CursorPos() == 327)
						{
							iEdwin->SetCursorPosL(467,EFalse );
						}
						else if(iEdwin->CursorPos() == 487 || iEdwin->CursorPos() == 467)
						{
							iEdwin->SetCursorPosL(642,EFalse );
						}
						else if(iEdwin->CursorPos() == 665 || iEdwin->CursorPos() == 642)
						{
							iEdwin->SetCursorPosL(814,EFalse );
						}
						else if(iEdwin->CursorPos() == 842 || iEdwin->CursorPos() == 814)
						{
							iEdwin->SetCursorPosL(984,EFalse );
						}
						else if(iEdwin->CursorPos() == 998 || iEdwin->CursorPos() == 984)
						{
							iEdwin->SetCursorPosL(1158,EFalse );
						}
						else if(iEdwin->CursorPos() == 1181 || iEdwin->CursorPos() == 1158)
						{
							iEdwin->SetCursorPosL(1328,EFalse );
						}
						else if(iEdwin->CursorPos() == 1351 || iEdwin->CursorPos() == 1328)
						{
							iEdwin->SetCursorPosL(1441,EFalse );
						}
						else if(iEdwin->CursorPos() == 1469 || iEdwin->CursorPos() == 1441)
						{
							iEdwin->SetCursorPosL(1620,EFalse );
						}
						else if(iEdwin->CursorPos() == 1647 || iEdwin->CursorPos() == 1620)
						{
							iEdwin->SetCursorPosL(1800,EFalse );
						}
						else if(iEdwin->CursorPos() == 1815 || iEdwin->CursorPos() == 1800)
						{
							iEdwin->SetCursorPosL(1968,EFalse );
						}
						else if(iEdwin->CursorPos() == 1991 || iEdwin->CursorPos() == 1968)
						{
							iEdwin->SetCursorPosL(2142,EFalse );
						}
						else if(iEdwin->CursorPos() == 2167 || iEdwin->CursorPos() == 2142)
						{
							iEdwin->SetCursorPosL(2289,EFalse );
						}
						else if(iEdwin->CursorPos() == 2316 || iEdwin->CursorPos() == 2289)
						{
							iEdwin->SetCursorPosL(2460,EFalse );
						}
						else if(iEdwin->CursorPos() == 2483 || iEdwin->CursorPos() == 2460)
						{
							iEdwin->SetCursorPosL(2611,EFalse );
						}
						else if(iEdwin->CursorPos() == 2635 || iEdwin->CursorPos() == 2611)
						{
							iEdwin->SetCursorPosL(2783,EFalse );
						}
						else if(iEdwin->CursorPos() == 2813 || iEdwin->CursorPos() == 2783)
						{
							iEdwin->SetCursorPosL(2922,EFalse );
						}
						else if(iEdwin->CursorPos() == 2938 || iEdwin->CursorPos() == 2922)
						{
							iEdwin->SetCursorPosL(3087,EFalse );
						}
						else if(iEdwin->CursorPos() == 3111 || iEdwin->CursorPos() == 3087)
						{
							iEdwin->SetCursorPosL(3261,EFalse );
						}
						else if(iEdwin->CursorPos() == 3274 || iEdwin->CursorPos() == 3261)
						{
							iEdwin->SetCursorPosL(3533,EFalse );
						}
						return iEdwin->OfferKeyEventL(aKeyEvent, aType);						
					break; 
				}				
            }
        }    
    return EKeyWasNotConsumed;
}*/
// End of File  
