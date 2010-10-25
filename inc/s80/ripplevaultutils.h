/*
* ============================================================================
*  Name     : RippleVaultUtils.h
*  Part of  : RippleVault
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     RippleVaultUtils header
*  Copyright: PurpleAce
* ============================================================================
*/

#ifndef __RIPPLEVAULTUTILS_H
#define __RIPPLEVAULTUTILS_H

const TInt KShadowMargin = 5;

// Static util class for application. 

class RippleVaultUtils 
    {
    public: 
		static void DrawShadowL( CWindowGc& aGc, const TRect& aRect );
    private: 
        
    };

#endif
