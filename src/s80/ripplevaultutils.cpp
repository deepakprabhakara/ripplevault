/*
* ============================================================================
*  Name     : RippleVaultUtils.cpp
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     RippleVaultUtils implementation
*  Copyright: PurpleAce
* ============================================================================
*/

#include <gulcolor.h>
#include <laffonts.h>
#include <lafckncl.hrh>
#include <eikenv.h>

#include "RippleVaultUtils.h"

void RippleVaultUtils::DrawShadowL( CWindowGc& aGc, const TRect& aRect )
	{

    CEikonEnv* eikEnv = CEikonEnv::Static();
    CColorArray& colorArray = eikEnv->CustomColorArray();

    // Let's draw black line around the actual window.
    TRect windowRect = aRect;
    windowRect.iBr.iX -= KShadowMargin-1;
    windowRect.iBr.iY -= KShadowMargin-1;
    aGc.DrawRect(windowRect);
    
    // This is the area of window's right shadow. 
    TRect rightShadowArea = aRect;
    rightShadowArea.iTl.iX = rightShadowArea.iBr.iX - KShadowMargin + 1;
    rightShadowArea.iTl.iY += KShadowMargin;
        
    // This is the area of window's bottom shadow. 
    TRect bottomShadowArea = aRect;
    bottomShadowArea.iTl.iX += KShadowMargin + 1;
    bottomShadowArea.iTl.iY += bottomShadowArea.iBr.iY - KShadowMargin + 1;
    
    // draw gradient shadow
    // Note: When using ECknColorShadow color of the shadow changes when user changes the color of display.
    for (TInt i=0; i<4; i++)
        {
        switch (i)
            {
            case 0:
                aGc.SetPenColor(colorArray.Color(ECknColorShadow4));
                break;
            case 1:
                aGc.SetPenColor(colorArray.Color(ECknColorShadow3));
                break;
            case 2:
                aGc.SetPenColor(colorArray.Color(ECknColorShadow2));
                break;
            case 3:
                aGc.SetPenColor(colorArray.Color(ECknColorShadow1));
                break;
            default:
                break;
            }

        aGc.DrawLine(TPoint(rightShadowArea.iTl.iX+i, rightShadowArea.iTl.iY+i),
            TPoint(rightShadowArea.iTl.iX+i, rightShadowArea.iBr.iY+i));

        aGc.DrawLine(TPoint(bottomShadowArea.iTl.iX+i, bottomShadowArea.iTl.iY+i),
            TPoint(bottomShadowArea.iBr.iX+i, bottomShadowArea.iTl.iY+i));
        }
    }
