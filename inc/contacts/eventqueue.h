#ifndef __EVENTQUEUE_H__
#define __EVENTQUEUE_H__

#include <e32std.h>
#include <e32base.h>

class LinkedList
{
public:
    LinkedList() { next = NULL; }
    void *iData;
    LinkedList *next;
};

class CEventQueue
{
public:
	CEventQueue();
	~CEventQueue();

	TInt AddToQueue(void *aeventarg);
	TInt RemoveFromQueue(void **arg);
    TInt Size() { return iSize; }

private:
    LinkedList *iFirst;
    LinkedList *iLast;
    TInt iSize;
};

#endif
