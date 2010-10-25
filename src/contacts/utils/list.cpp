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
 Module Name				: List.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     Linked List to implement Contacts event queue
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/


#include "list.h"

//---------------------------------------------------------
// Create an empty list
//---------------------------------------------------------
CList::CList()
{
	count = 0;
	content = NULL;
	head = NULL;
	next = NULL;
	tail = NULL;
}

//---------------------------------------------------------
// Destructor
//---------------------------------------------------------
CList::~CList()
{
	
}

//---------------------------------------------------------
// Return the number of elements in the list
//---------------------------------------------------------
TInt CList::Count()
{
	return count;
}

//---------------------------------------------------------
// Add an element to the list
//---------------------------------------------------------
void CList::Add(TAny *element)
{
	if (count == 0) {
		//head = tail = new CList;
		head = tail = this;
		head->content = element;
		//tail = head;
	}
	else {
		tail->next = new CList;
		tail = tail->next;
		tail->content = element;
	}
	count++;
}
	
//---------------------------------------------------------
// Get the contents of an element in the list
//---------------------------------------------------------
TAny* CList::GetContent()
{
	return content;
	//if (next == NULL)
	//	return NULL;
	//else
	//	return next->content;
}

//---------------------------------------------------------
// Empty the list
//---------------------------------------------------------
void CList::RemoveAll()
{
	CList *ptr = head;
	for (int i = 0; i < count; i++)
	{
		CList *tmp = ptr;
		ptr = ptr->next;
		delete tmp;
	}
	//count = 0;
}

void CList::RemoveAllAndContent()
{
	CList *ptr = head;
	for (int i = 0; i < count; i++)
	{
		CList *tmp = ptr;
		ptr = ptr->next;
		TAny* _content = tmp->GetContent();
		if (_content)
		{
			delete _content;
		}
		delete tmp;
	}
}
