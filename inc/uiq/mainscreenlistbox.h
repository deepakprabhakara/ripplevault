// --------------------------------------------------------------------------
// CustomListBox.h
//
//
// An example of subclassing a list box in Symbian (UIQ).
// --------------------------------------------------------------------------

#ifndef ___MAINSCREENLISTBOX_H__
#define ___MAINSCREENLISTBOX_H__

// INCLUDES
#include <eiklbi.h>   // CListItemDrawer
#include <eiktxlbx.h> // CEikTextListBox

// FORWARD DECLARATIONS
class CGulIcon;

// CLASS DECLARATIONS

/**
 * CMainScreenListItemDrawer
 */
class CMainScreenListItemDrawer: public CListItemDrawer
	{ 
public: // constructor and destructor
	CMainScreenListItemDrawer(const CEikTextListBox& aListBox);
	~CMainScreenListItemDrawer();

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
 * CMainScreenListBox
 */
class CMainScreenListBox: public CEikTextListBox
	{
public: // constructors
	CMainScreenListBox();
	void ConstructL(const CCoeControl* aParent, TInt aFlags = 0);

public: // from CEikTextListBox
	virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
		TEventCode aType);

private: // from CEikTextListBox
	virtual void CreateItemDrawerL();
	};

#endif ___MAINSCREENLISTBOX_H__

// End of File
