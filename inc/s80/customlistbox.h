// --------------------------------------------------------------------------
// CustomListBox.h
//
//
// An example of subclassing a list box in Symbian (UIQ).
// --------------------------------------------------------------------------

#ifndef ___CUSTOMLISTBOX_H__
#define ___CUSTOMLISTBOX_H__

// INCLUDES
#include <eiklbi.h>   // CListItemDrawer
#include <eiktxlbx.h> // CEikTextListBox

// FORWARD DECLARATIONS
class CGulIcon;

// CLASS DECLARATIONS

/**
 * CCustomListItemDrawer
 */
class CCustomListItemDrawer: public CListItemDrawer
	{ 
public: // constructor and destructor
	CCustomListItemDrawer(const CEikTextListBox& aListBox);
	~CCustomListItemDrawer();

private: // from CListItemDrawer
	virtual void DrawActualItem(TInt aItemIndex, const TRect& aActualItemRect,
         TBool aItemIsCurrent, TBool aViewIsEmphasized, TBool aViewIsDimmed, 
		 TBool aItemIsSelected) const;

public: // new methods
	void SetIconArray(CArrayPtr<CGulIcon>* aIconArray);
	TSize MaxIconSize() const;

private: // private methods
	void DeleteIconArray();
	void CalculateMaxIconSize();

private:
	const CEikTextListBox& iListBox;
	CArrayPtr<CGulIcon>*   iIconArray;
	TSize                  iMaxIconSize;
	};


/**
 * CCustomListBox
 */
class CCustomListBox: public CEikTextListBox
	{
public: // constructors
	CCustomListBox();
	void ConstructL(const CCoeControl* aParent, TInt aFlags = 0);

public: // from CEikTextListBox
	virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
		TEventCode aType);

private: // from CEikTextListBox
	virtual void CreateItemDrawerL();
	};

#endif ___CUSTOMLISTBOX_H__

// End of File
