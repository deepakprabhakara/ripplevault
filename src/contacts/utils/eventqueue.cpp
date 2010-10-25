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
 Module Name				: EventQueue.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Queue for buffering contacts DB events, before contacthandler can handle the logic
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/


#include "EventQueue.h"

CEventQueue::CEventQueue()
{
    iFirst = iLast = NULL;
    iSize = 0;
}

CEventQueue::~CEventQueue()
{
	LinkedList *node;
	if (iFirst != NULL)
	{
		while(iFirst)
		{
			node = iFirst;
			iFirst = iFirst->next;
			delete node;
		}
	}
}

TInt CEventQueue::AddToQueue(void *aeventarg)
{
    if (iFirst == NULL)
    {
        iFirst = new LinkedList();
        iLast = iFirst;
    }
    else
    {
        LinkedList *last = new LinkedList();
        iLast->next = last;
        iLast = last;
    }
    iLast->iData = aeventarg;
    iSize++;
    return iSize;
}

TInt CEventQueue::RemoveFromQueue(void **arg)
{
    *arg = NULL;
    if (iSize <= 0)
    {
        return -1;
    }
    LinkedList *node = iFirst;
    *arg = iLast->iData;
    iSize--;

    if (iFirst == iLast) // single entry
    {
        delete iLast;
        iFirst = iLast = NULL;
        return iSize;
    }
    while (node->next != iLast)
    {
        node = node->next;
    }
    iLast = node;
    delete node->next;

    return iSize;
}
