#ifndef __LIST_H__
#define __LIST_H__

#include <e32std.h>
#include <e32base.h>

class CList
	{
public:
	// Create an empty list
	CList();
	~CList();

	// Return no of elements in the list
	TInt Count();

	// Add an element to the list
	void Add(TAny *element);

	// Get contents of an element of the list
	TAny *GetContent();

	// Empty the list
	void RemoveAll();
	void RemoveAllAndContent();

	CList *next;

private:

	TAny *content;
	CList *head;
	CList *tail;
	TInt count;
	
	};

#endif

// End of file
