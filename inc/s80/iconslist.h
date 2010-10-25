/*
* ============================================================================
*  Name     : IconsList.h
*  Part of  : Grid
*  Created  : 15.10.2005 by Ezhil
*  Description:
*     CIconsList header
*  Copyright: Purpleace
* ============================================================================
*/

#ifndef __ICONSLIST_H
#define __ICONSLIST_H

#include <badesca.h>

class CIconsList : public CCoeControl
	{
	public: 
		CIconsList();
		~CIconsList();
		static CIconsList* NewL(TInt aScreenValue);
		void SetIconArray( CArrayPtr<CGulIcon>* aIconArray);
		TPtrC ItemName( TInt aIndex );
		TInt ItemCount();
		CDesCArray* GetItemArray();

	private: 
		void ConstructL(TInt aScreenValue);

	private: 
		CDesCArray *iItemArray;
		TInt iItemCount;
		TInt iScreenValue;	
	};

#endif 
