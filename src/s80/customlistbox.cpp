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
 Module Name				: CustomListBox.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-06-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     subclassing a list box in Symbian (UIQ).
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/

// INCLUDE FILES
#include <coemain.h>
#include <eikenv.h>
#include <gulicon.h>
#include <eiktxlbm.h>   // CTextListBoxModel
//#include <QuartzKeys.h> // keys for jog dial

#include "CustomListBox.h"


// MEMBER FUNCTIONS

// --------------------------------------------------------------------------
// CCustomListItemDrawer
// --------------------------------------------------------------------------

CCustomListItemDrawer::CCustomListItemDrawer(const CEikTextListBox& aListBox)
:CListItemDrawer(),
 iListBox(aListBox)
	{
	// Store a GC for later use
	iGc = &CCoeEnv::Static()->SystemGc();
	SetGc(iGc);
	}

CCustomListItemDrawer::~CCustomListItemDrawer()
	{
	DeleteIconArray();
	}

//
// Draws the item, which might contain an icon and a text.
// The format of the item text shall look like one of the following formats:
//		- "\ttext"				(text only)
//		- "icon\ttext"			(icon and text)
//		- "\theading\ttext"		(heading and text)
//		- "icon\theading\ttext"	(icon, heading and text)
//		
void CCustomListItemDrawer::DrawActualItem(TInt aItemIndex,
		const TRect& aActualItemRect, TBool aItemIsCurrent,
		TBool /*aViewIsEmphasized*/, TBool /*aViewIsDimmed*/,
		TBool aItemIsSelected) const
	{
	const MDesCArray* itemArray = iListBox.Model()->ItemTextArray();
	if ((!itemArray) || (itemArray->MdcaCount() <= aItemIndex))
		return;
    
	// Gets the item text if the conditions above are met.
	TPtrC itemText = itemArray->MdcaPoint(aItemIndex);

	// We have to find the position of tabs to decide the components
	// available in the item text (icon, heading and text).
	TInt tabPosition1 = itemText.Locate('\t');
	TInt tabPosition2 = itemText.Mid(tabPosition1 + 1).Locate('\t');
	if (tabPosition2 >= 0)
		{
		// We need to add tabPosition1 because the return value of
		// Locate() is relative to tabPosition1.
		tabPosition2 += tabPosition1 + 1;
		}
	TInt tabPosition3 = itemText.Mid(tabPosition2 + 1).Locate('\t');
	if (tabPosition3 >= 0)
		{
		// We need to add tabPosition2 because the return value of
		// Locate() is relative to tabPosition2.
		tabPosition3 += tabPosition2 + 1;
		}

	// Sets the attributes to draw the icon.
	iGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
	if ((aItemIsCurrent) || (aItemIsSelected))
		{
		iGc->SetBrushColor(iHighlightedBackColor);
		}
	else
		{
		iGc->SetBrushColor(iBackColor);
		}

	// Gets the icon index, i.e. the number in the text item before
	// the first tab.
	TInt iconIndex = 0;
	if (tabPosition1 > 0)
		{
		TLex(itemText.Mid(0, tabPosition1)).Val(iconIndex);
		}

		TRect aActualItemRect2 = aActualItemRect;

		aActualItemRect2.iBr.iX = aActualItemRect2.iBr.iX + 0;
		aActualItemRect2.iBr.iY = aActualItemRect2.iBr.iY + 0;
		aActualItemRect2.iTl.iX = aActualItemRect2.iTl.iX + 0;
		aActualItemRect2.iTl.iY = aActualItemRect2.iTl.iY + 10;

	if ((iIconArray) && (iIconArray->Count() > iconIndex)
		&& (tabPosition1 > 0))
		{
		// Draws the icon.
		CFbsBitmap* bitmap = (*iIconArray)[iconIndex]->Bitmap();

		iGc->BitBltMasked(aActualItemRect2.iTl,
			bitmap,TRect(TPoint(0,0), bitmap->Header().iSizeInPixels),
			(*iIconArray)[iconIndex]->Mask(),
			ETrue);

		//iGc->BitBltMasked(TPoint(50,50),bitmap,bmpPieceRect,(*iIconArray)[iconIndex]->Mask(),EFalse);

		// Draws the rectangle, just in case there are some icons that are
		// smaller than the height of item width and/or height.
		iGc->SetPenStyle(CGraphicsContext::ENullPen);

		//TRect rect1(161,78,185,92);
		//iGc->DrawRect(rect1);
			
		TInt pos  = aActualItemRect2.iTl.iX;
		TInt pos1 = aActualItemRect2.iTl.iY + bitmap->Header().iSizeInPixels.iHeight;
		TInt pos2 = aActualItemRect2.iTl.iX + iMaxIconSize.iWidth;
		TInt pos3 = aActualItemRect2.iBr.iY;
	
		TInt pos8  = bitmap->Header().iSizeInPixels.iHeight;
		TInt pos9  = iMaxIconSize.iWidth;
		TInt pos10 = bitmap->Header().iSizeInPixels.iWidth;			
		
		TInt pos4 = aActualItemRect2.iTl.iX;
		TInt pos5 = aActualItemRect2.iTl.iY    - 10;// + bitmap->Header().iSizeInPixels.iHeight;
		TInt pos6 = aActualItemRect2.iTl.iX    + pos9;// + iMaxIconSize.iWidth;
		TInt pos7 = aActualItemRect2.iBr.iY	   - pos9	   - 14;

		TRect rect2(pos4,pos5,pos6,pos7);
		iGc->DrawRect(rect2);

		TRect rect(pos,pos1,pos2,pos3);
		iGc->DrawRect(rect);
		}
	else
		{
		iGc->SetPenStyle(CGraphicsContext::ENullPen);
		TRect rect(
			aActualItemRect.iTl.iX,
			aActualItemRect.iTl.iY,
			aActualItemRect.iTl.iX + iMaxIconSize.iWidth,
			aActualItemRect.iBr.iY);
		iGc->DrawRect(rect);
		}

	// Sets the attributes to draw text, except the font.
	const CFont* font = Font(aItemIndex);
	iGc->SetPenStyle(CGraphicsContext::ESolidPen);
	iGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
	if ((aItemIsCurrent) || (aItemIsSelected))
		{
		iGc->SetPenColor(iHighlightedTextColor);
		iGc->SetBrushColor(iHighlightedBackColor);
		}
	else
		{
		iGc->SetPenColor(iTextColor);
		iGc->SetBrushColor(iBackColor);
		}

	// If there is only one tab, it means we have to display single line item
	if (tabPosition2 < 0)
		{		
		if (!font)
			{
			font = CEikonEnv::Static()->TitleFont();
			}
		iGc->UseFont(font);
		TRect textRect(
			TPoint(aActualItemRect.iTl.iX + iMaxIconSize.iWidth,
					aActualItemRect.iTl.iY),
			aActualItemRect.iBr);
		TInt baseline =
			(textRect.iBr.iY - textRect.iTl.iY - font->HeightInPixels()) / 2
				+ font->AscentInPixels();

		if (tabPosition1 < 0)
			{
			iGc->DrawText(itemText, textRect, baseline,
				CGraphicsContext::ELeft, 1);
			}
		else
			{
			iGc->DrawText(itemText.Mid(tabPosition1 + 1), textRect, baseline,
				CGraphicsContext::ELeft, 1);
			}
		}
	else
		{
		// If there are more than one tab, then we have to display double line
		// item text
		
		TRect textRect(
			TPoint(aActualItemRect.iTl.iX + iMaxIconSize.iWidth,
				aActualItemRect.iTl.iY),
			aActualItemRect.iBr);

		textRect.iBr.iY -= aActualItemRect.Height() / 2;

		TBuf<2000> text;
		text.Copy(_L("   "));

		// Draws the first line.
		if (!font)
			{
			font = CEikonEnv::Static()->LegendFont();
			}
		iGc->UseFont(font);
		TInt baseline =
			(textRect.iBr.iY - textRect.iTl.iY - font->HeightInPixels()) / 2
				+ font->AscentInPixels();
		
		text.Append(itemText.Mid(tabPosition1 + 1, tabPosition2 - tabPosition1 - 1));

		iGc->DrawText(
			text,textRect, baseline, CGraphicsContext::ELeft, 1);

		// Draws the second line. For the second line, we use normal font.
		font = CCoeEnv::Static()->NormalFont();
		iGc->UseFont(font);
		textRect.Move(0, aActualItemRect.Height() / 2);
		baseline =
			(textRect.iBr.iY - textRect.iTl.iY - font->HeightInPixels()) / 2
				+ font->AscentInPixels();
		
		text.Copy(_L("   "));

		//text.Append(itemText.Mid(tabPosition2 + 1));		
		text.Append(itemText.Mid(tabPosition2 + 1, tabPosition3 - tabPosition2 - 1));

		iGc->DrawText(text, textRect, baseline,	CGraphicsContext::ELeft, 1);
		
		TRect aActualItemRect1 = aActualItemRect;

		aActualItemRect1.iBr.iX = aActualItemRect1.iBr.iX + 0;
		aActualItemRect1.iBr.iY = aActualItemRect1.iBr.iY + 0;
		aActualItemRect1.iTl.iX = aActualItemRect1.iTl.iX + 270;
		aActualItemRect1.iTl.iY = aActualItemRect1.iTl.iY + 0;

		text.Copy(&itemText[itemText.Length()-1],1);
		TLex(text).Val(iconIndex);

		// Draws the icon.
		CFbsBitmap* bitmap = (*iIconArray)[iconIndex]->Bitmap();

		iGc->BitBltMasked(aActualItemRect1.iTl,
			bitmap,TRect(TPoint(0,0), bitmap->Header().iSizeInPixels),
			(*iIconArray)[iconIndex]->Mask(),
			ETrue);

		// Draws the rectangle, just in case there are some icons that are
		// smaller than the height of item width and/or height.
		iGc->SetPenStyle(CGraphicsContext::ENullPen);
		if (bitmap->Header().iSizeInPixels.iHeight < aActualItemRect1.Height())
			{
			TRect rect(
				aActualItemRect1.iTl.iX,
				aActualItemRect1.iTl.iY + bitmap->Header().iSizeInPixels.iHeight,
				aActualItemRect1.iTl.iX + iMaxIconSize.iWidth,
				aActualItemRect1.iBr.iY);
			iGc->DrawRect(rect);
			}

		if (bitmap->Header().iSizeInPixels.iWidth < iMaxIconSize.iWidth)
			{
			TRect rect = TRect(
				aActualItemRect1.iTl.iX + bitmap->Header().iSizeInPixels.iWidth,
				aActualItemRect1.iTl.iY,
				aActualItemRect1.iTl.iX + iMaxIconSize.iWidth,
				aActualItemRect1.iBr.iY);
			iGc->DrawRect(rect);
			}	
	}
}

void CCustomListItemDrawer::SetIconArray(CArrayPtr<CGulIcon>* aIconArray)
	{
	DeleteIconArray();
	iIconArray = aIconArray;
	CalculateMaxIconSize();
	}

TSize CCustomListItemDrawer::MaxIconSize() const
{
	return iMaxIconSize;
	}

void CCustomListItemDrawer::DeleteIconArray()
	{
	if (iIconArray)
		{
		iIconArray->ResetAndDestroy();
		delete iIconArray;
		iIconArray = 0;
		}
	}

void CCustomListItemDrawer::CalculateMaxIconSize()
	{
	iMaxIconSize = TSize(0, 0);

	// Finds the maximum width and height of the icon.
	if (iIconArray)
		{
		for (TInt i = 0; i < iIconArray->Count(); i++)
			{
			TInt width = (*iIconArray)[i]->Bitmap()->Header().iSizeInPixels.iWidth;
			if (iMaxIconSize.iWidth < width)
				{
				iMaxIconSize.iWidth = width;
				}

			TInt height = (*iIconArray)[i]->Bitmap()->Header().iSizeInPixels.iHeight;
			if (iMaxIconSize.iHeight < height)
				{
				iMaxIconSize.iHeight = height;
				}
			} // for
		} // iIconArray
	}


// --------------------------------------------------------------------------
// CCustomListBox
// --------------------------------------------------------------------------

CCustomListBox::CCustomListBox()
:CEikTextListBox()
	{
	}

void CCustomListBox::ConstructL(const CCoeControl* aParent, TInt aFlags)
	{
	CEikTextListBox::ConstructL(aParent, aFlags);
	}

void CCustomListBox::CreateItemDrawerL()
	{
	iItemDrawer = new (ELeave) CCustomListItemDrawer(*this);
	}

TKeyResponse CCustomListBox::OfferKeyEventL(const TKeyEvent& aKeyEvent,
											TEventCode aType)
	{
	if (EEventKeyDown == aType)
		{
		switch (aKeyEvent.iScanCode)
			{
			case EStdKeyDownArrow:
			case 173:
				{
				if (CurrentItemIndex() < Model()->NumberOfItems() - 1)
					{
					SetCurrentItemIndex(CurrentItemIndex() + 1);
					DrawNow();
					}
				return EKeyWasConsumed;
				}

			case EStdKeyUpArrow:
			case 172:
				{
				if (CurrentItemIndex() > 0)
					{
					SetCurrentItemIndex(CurrentItemIndex() - 1);
					DrawNow();
					}
				return EKeyWasConsumed;
				}
			default:
				return CEikTextListBox::OfferKeyEventL(aKeyEvent, aType);
			}
		}
	return EKeyWasNotConsumed;
	}

// End of File  
 