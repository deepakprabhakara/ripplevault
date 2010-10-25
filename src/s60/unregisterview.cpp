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
 Module Name				: UnRegisterView.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 17-06-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     View seen by user before registration
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/

// INCLUDE FILES
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <ripplevault.rsg>
#include  <aknquerydialog.h>
#include  <akngloballistquery.h> 
#include <aknmessagequerydialog.h>
#include  <akntitle.h>
#include  <es_sock.h> 

#include  "UnRegisterView.h"
#include  "UnRegisterContainer.h" 
#include  "RippleVault.hrh"
#include  "ripplevaultappui.h"
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CUnRegisterView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------

//_LIT(KPlus,"+");
//_LIT(KMinus,"-");


void CUnRegisterView::ConstructL()
    {	
		BaseConstructL( R_UNREG_VIEW);
    }

// ---------------------------------------------------------
// CUnRegisterView::~CUnRegisterView()
// ?implementation_description
// ---------------------------------------------------------
//
CUnRegisterView::~CUnRegisterView()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CUnRegisterView::Id()
// ?implementation_description
// ---------------------------------------------------------
//
TUid CUnRegisterView::Id() const
    {
    return KViewIdUnregisterScreen;
    }

// ---------------------------------------------------------
// CUnRegisterView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------
//
void CUnRegisterView::HandleCommandL(TInt aCommand)
    {   
	TInt Reg;
//	TBuf<100> Temp;
    switch ( aCommand )
        {
/*			case ERegister:				
				if(iPhotosAppUi->iUserNames.Length() > 0 && iPhotosAppUi->iMobileNo.Length() > 0  && iPhotosAppUi->iPassword.Length() > 0)
				{
					iPhotosAppUi->ExistingUser();
				}
				else if(iPhotosAppUi->iUserNames.Length() > 0 && iPhotosAppUi->iMobileNo.Length() > 0)
				{
					iPhotosAppUi->NewUser();
				}
				else
				{				
					Reg = iPhotosAppUi->CheckRegFile();
					if(Reg == -1)
					{
						Reg=0;
						RegisterRipple(Reg);
					}
					else
					{
						//Temp.Copy(_L("User Name:"));
						HBufC*  message;
						message = iEikonEnv->AllocReadResourceL(R_EXAMPLE_USER_NAME);				
						Temp.Copy(message->Des());
						delete message;
						
						Temp.Append(iPhotosAppUi->iUserName);
						CEikonEnv::Static ()->AlertWin(Temp);

						iPhotosAppUi->WritRegisteredFile();
						iPhotosAppUi->UserRegistered();

						iPhotosAppUi->EnableUpload(REGISTERED);
					}
				}
				break;
		case ELogin:
				if(iPhotosAppUi->iUserNames.Length() > 0 && iPhotosAppUi->iMobileNo.Length() > 0  && iPhotosAppUi->iPassword.Length() > 0)
				{
					iPhotosAppUi->ExistingUser();
				}
				else if(iPhotosAppUi->iUserNames.Length() > 0 && iPhotosAppUi->iMobileNo.Length() > 0)
				{
					iPhotosAppUi->NewUser();
				}
				else
				{
					Reg = iPhotosAppUi->CheckRegFile();
					if(Reg == -1)
					{
						Reg=1;
						RegisterRipple(Reg);
					}
					else
					{
						HBufC*  message;
						message = iEikonEnv->AllocReadResourceL(R_EXAMPLE_USER_NAME);				
						Temp.Copy(message->Des());
						delete message;

						Temp.Append(iPhotosAppUi->iUserName);
						CEikonEnv::Static ()->AlertWin(Temp);

						iPhotosAppUi->UserRegistered();

						iPhotosAppUi->EnableUpload(REGISTERED);
					}
				}
			break;*/
			case ERegister:				
			//case ELogin:				
				{
					Reg = iPhotosAppUi->ChooseAccessPoint();
					if(Reg == 10)
						iPhotosAppUi->AutoRegisterRipple();
				}
				break;
        case ESettings:
            {
            AppUi()->HandleCommandL(ESettings);
            break;
            }
        //case EAknSoftkeyBack:
        case EMinimize:
            {
            AppUi()->HandleCommandL(EExit);
            break;
            }
/*		case Esmsbackup:
            {
				AppUi()->HandleCommandL(Esmsbackup);
				break;
            }*/
        default:
            {
            AppUi()->HandleCommandL( aCommand );
            break;
            }
        }
    }

// ---------------------------------------------------------
// CUnRegisterView::HandleClientRectChange()
// ---------------------------------------------------------
//
void CUnRegisterView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CUnRegisterView::DoActivateL(...)
// ?implementation_description
// ---------------------------------------------------------
//
void CUnRegisterView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
	TInt apn;
	apn=0;
    if (!iContainer)
        {
        iContainer = new (ELeave) CUnRegisterContainer;
        iContainer->SetMopParent(this);
		//iContainer->iPhotosAppUi = iPhotosAppUi;
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );

		if(iPhotosAppUi->iRipplePhotos == 0)
			apn = iPhotosAppUi->ChooseAccessPoint();
		iPhotosAppUi->iRipplePhotos=0;
        } 
		
	if(apn == 10)
		iPhotosAppUi->AutoRegisterRipple();
   }

// ---------------------------------------------------------
// CUnRegisterView::HandleCommandL(TInt aCommand)
// ?implementation_description
// ---------------------------------------------------------
//
void CUnRegisterView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }
/*
void CUnRegisterView::RegisterRipple(TInt Reg )
{
	{
		if(Reg==0) //Register Photos
		{
		   TBuf<20> userName;
		   TBuf<20> phone_number;
		   TBool iRegistrationSuccess;
		   TBool Check;
		   TInt pos;
  		   iRegistrationSuccess=EFalse;		   

		   phone_number.Copy(_L("+65-"));
		   
		   if(!iRegistrationSuccess)
			{
				CAknMultiLineDataQueryDialog* dialog =
                CAknMultiLineDataQueryDialog::NewL(userName, phone_number);

		        //iPhotosAppUi->BringToForeground();

				Check=dialog->ExecuteLD(R_CONTACTS_DIALOG_REGISTER);
				if(Check)
                {
					//TBuf8<1024> buffer;
					pos=phone_number.Find(KPlus);
					if(pos!=KErrNotFound)
					phone_number.Delete(pos,1);

					pos=phone_number.Find(KMinus);
					if(pos!=KErrNotFound)
					phone_number.Delete(pos,1);					

					iPhotosAppUi->iUserName.Copy(userName);
					
					iPhotosAppUi->iUserNames.Copy(userName);
					iPhotosAppUi->iMobileNo.Copy(phone_number);

 					iPhotosAppUi->NewUser();								

                }
				else
				{
					iRegistrationSuccess=1;
				}
			}
		}
		else if(Reg==1) //Already Registered
		{
			TBuf<20> userName;
			TBuf<20> password;
			TBuf<20> phone_number;
			TBool iRegistrationSuccess;
			TBool Check;
			TInt pos;
			iRegistrationSuccess=EFalse;
			
			phone_number.Copy(_L("+65-"));

			if(!iRegistrationSuccess)
			{   
				CAknTextQueryDialog* dialog1 = CAknTextQueryDialog::NewL(phone_number);		

				iPhotosAppUi->BringToForeground();
				
                // Display and execute dialog, and act according to return value
				Check=dialog1->ExecuteLD(IMAGE_NAME);
				if (Check)
                {			
					iPhotosAppUi->BringToForeground();

		            CAknMultiLineDataQueryDialog* dialog =
			        CAknMultiLineDataQueryDialog::NewL(userName, password);
                
					Check=dialog->ExecuteLD(R_CONTACTS_DIALOG_ALREADY_REGISTERED);

					if(Check)
					{
						pos=phone_number.Find(KPlus);
						if(pos!=KErrNotFound)
						phone_number.Delete(pos,1);

						pos=phone_number.Find(KMinus);
						if(pos!=KErrNotFound)
						phone_number.Delete(pos,1);

						iPhotosAppUi->iUserName.Copy(userName);
						
						iPhotosAppUi->iUserNames.Copy(userName);
						iPhotosAppUi->iMobileNo.Copy(phone_number);
						iPhotosAppUi->iPassword.Copy(password);

						iPhotosAppUi->ExistingUser();
					}
				}
				else
				{
					iRegistrationSuccess=1;
				}

			}
		}
		else if(Reg==-1) 
		{
			iPhotosAppUi->BringToForeground();
			TBuf<100> msg;
			msg.Copy(_L("Registration Cancelled"));        
			CAknQueryDialog* dlg = new (ELeave) CAknQueryDialog(msg, CAknQueryDialog::ENoTone);
			dlg->ExecuteLD( R_AKNEXQUERY_CONFIRMATION_QUERY2);			
		}
    }
}
*/

// End of File

