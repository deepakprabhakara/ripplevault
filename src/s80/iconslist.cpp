/*
* ============================================================================
*  Name     : IconsList.cpp
*  Part of  : Grid
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CIconsList implementation
*  Copyright: Purpleace
* ============================================================================
*/


#include <barsread.h>
#include <badesca.h>
#include <cknenv.h>
#include <eikenv.h>
#include <eikimage.h>
#include <gulicon.h>
#include <RippleVault.rsg>
 
#include "IconsList.h"
#include "icons.mbg"
#include "constants.h"

// Empty constructor
CIconsList::CIconsList() 
	{ 
	}

// Empty destructor. List box owns the item array and handles deleting. . 
CIconsList::~CIconsList()
{
  //delete iItemArray;	
}

CIconsList* CIconsList::NewL(TInt aScreenValue)
{
    CIconsList* self=new(ELeave) CIconsList;
    CleanupStack::PushL(self);
    self->ConstructL(aScreenValue);
    CleanupStack::Pop(self);
	
    return self;
}

// Populates given icon array. Icons are defined in R_GRID_ICONS resource. 
void CIconsList::SetIconArray( CArrayPtr<CGulIcon>* aIconArray ) 
{
	TResourceReader reader;

	if(iScreenValue == 0)
	{		
		CEikonEnv::Static()->CreateResourceReaderLC(reader, R_MAINSCREEN_ICONS);
	}	
	else if(iScreenValue == 1)
	{
		CEikonEnv::Static()->CreateResourceReaderLC(reader, R_MAINSUBSCREEN_CONTACTS_ICONS);
	}
	else if(iScreenValue == 2)
	{
		CEikonEnv::Static()->CreateResourceReaderLC(reader, R_MAINSUBSCREEN_PHOTOS_ICONS);
	}
	else if(iScreenValue == 3)
	{
		CEikonEnv::Static()->CreateResourceReaderLC(reader, R_MAINSUBSCREEN_VIDEOS_ICONS);
	}
	else if(iScreenValue == 4)
	{
		CEikonEnv::Static()->CreateResourceReaderLC(reader, R_MAINSUBSCREEN_SMS_ICONS);
	}
	
	TInt numIcons = reader.ReadInt16();

	for (TInt count = 0; count < numIcons; ++count)
	{
		CEikImage* image = new (ELeave) CEikImage;
		CleanupStack::PushL(image);

		//image->ConstructFromResourceL(reader);
		TBuf<128> imagename = CRippleVaultAppUi::ApplicationDriveAndPath();
		imagename.Append(KSmsMbmFileName);
		image->CreatePictureFromFileL(imagename,0,0);
		image->SetPictureOwnedExternally(ETrue);


		CGulIcon* icon = CGulIcon::NewL(const_cast<CFbsBitmap*>(image->Bitmap()), const_cast<CFbsBitmap*>(image->Mask()));
		CleanupStack::PushL(icon);

		aIconArray->AppendL(icon);

		CleanupStack::Pop(icon);
		CleanupStack::PopAndDestroy(image);
	}

	CleanupStack::PopAndDestroy(); // reader
	
}

// Model has item array as data member so we are populating it when constructing the model. 
// Populating icon array has own function. 
void CIconsList::ConstructL(TInt aScreenValue)
{
	iScreenValue = aScreenValue;
	
	if(iScreenValue == 0)
	{
		iItemArray = new (ELeave) CDesCArrayFlat(5);

		iItemCount = 5;

		iItemArray->AppendL(_L("Contacts"));
		iItemArray->AppendL(_L("Photos"));
		iItemArray->AppendL(_L("Videos"));
		iItemArray->AppendL(_L("Sms"));
		iItemArray->AppendL(_L("Status"));

	}
	else if(iScreenValue == 1)
	{
		iItemArray = new (ELeave) CDesCArrayFlat(2);

		iItemCount = 2;

		iItemArray->AppendL(_L("View Contacts"));
		iItemArray->AppendL(_L("Settings"));
	}
	else if(iScreenValue == 2 || iScreenValue == 3)
	{
		iItemArray = new (ELeave) CDesCArrayFlat(5);

		iItemCount = 5;

		iItemArray->AppendL(_L("View Gallery"));
		iItemArray->AppendL(_L("Send to Vault"));
		iItemArray->AppendL(_L("Share Album"));
		iItemArray->AppendL(_L("UnShare Album"));
		iItemArray->AppendL(_L("Settings"));
	}
	else if(iScreenValue == 4)
	{
		iItemArray = new (ELeave) CDesCArrayFlat(3);

		iItemCount = 3;

		iItemArray->AppendL(_L("View Messages"));
		iItemArray->AppendL(_L("Send to Vault"));
		iItemArray->AppendL(_L("Settings"));
	}
}


// Returns nth index name from item array. 
TPtrC CIconsList::ItemName( TInt aIndex ) 
	{
	return (*iItemArray)[aIndex];
	}

// Returns amount of items in item array. 
TInt CIconsList::ItemCount()
	{
	return iItemCount;
	}

// Returns whole item array. 
CDesCArray* CIconsList::GetItemArray()
    {
    return iItemArray;
    }
