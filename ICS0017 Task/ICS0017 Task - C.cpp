#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#pragma warning(disable : 4996)
void PrintItem(ITEM9* item);
void PrintStruct(HEADER_B* p);
void PrintStruct(HEADER_A* p);
HEADER_B* InsertItem(HEADER_B *pd, char* pNewItemID = 0);
HEADER_B* RemoveItem(HEADER_B* pd, char* pItemID);
void getNameArr(char* input, char* buf);
void FindHeaderB(HEADER_B** headers, HEADER_B* header, char cBegin);
void FindHeaderA(HEADER_A** headers, HEADER_A* header, char cBegin);
void FindItem(ITEM9** items, ITEM9* item, char* pID);
bool nameCheck(char* input);
HEADER_A* CreateHeaderA(char cBegin, ITEM9* newItem);
HEADER_B* CreateHeaderB(char cBeginB, char cBeginA, ITEM9* newItem);
void freeHeaderA(HEADER_A* header);
void freeHeaderB(HEADER_B* header);
void freeItem(ITEM9* item);
void freeOneItem(ITEM9* item);

int test() {
	const char* ID[] = {
		"Z A",
		"Z Z",
		"Z K",
		"A Z",
		"A A",
		"A K",
		"G Z",
		"G A",
		"G K",
		"M A",
		"M Ba",
		"M Bb",
		"M Z",
		"M Ba",
		"Mba"
	};
	int inpLength = sizeof(ID) / sizeof(ID[0]);
	HEADER_B* p = GetStruct1(9, 30);
	PrintStruct(p);
	printf("\n\n");
	for (int i = 0; i < inpLength; i++) {
		char* tmp = (char*)ID[i];
		HEADER_B* buf = InsertItem(p, tmp);
		if (!buf) printf("Item \"%s\" insertion failed!\n\n", tmp);
		else p = buf;
	}
	PrintStruct(p);
	printf("\n\n");
	for (int i = 0; i < inpLength; i++) {
		char* tmp = (char*)ID[i];
		HEADER_B* buf = RemoveItem(p, tmp);
		if (!buf) printf("Item \"%s\" deletion failed!\n\n", tmp);
		else p = buf;
	}
	PrintStruct(p);
	freeHeaderB(p);
	return 0;
}

void PrintStruct(HEADER_B* p) {
	if (p) {
		HEADER_A* currentItem = (HEADER_A*)p->pHeaderA;
		if (currentItem) {
			PrintStruct(currentItem);
		}
		HEADER_B* ps = p->pNext;
		if (ps) {
			PrintStruct(ps);
		}
	}
	else printf("Nullptr passed to function PrintStruct\n");
}

void PrintStruct(HEADER_A* p) {
	if (p) {
		ITEM9* currentItem = (ITEM9*)p->pItems;
		if (currentItem) {
			PrintItem(currentItem);
		}
		HEADER_A* ps = p->pNext;
		if (ps) {
			PrintStruct(ps);
		}
	}
	else printf("Nullptr passed to function PrintStruct\n");
}

void PrintItem(ITEM9* item) {
	if (item) {
		printf("%s, %lu, %d, %s, %d\n",
			item->pID,
			item->Code,
			item->pDate->Day,
			item->pDate->pMonth,
			item->pDate->Year);
		if (item->pNext) {
			ITEM9* nextitem = item->pNext;
			PrintItem(nextitem);
		}
	}
	else 
	printf("Nullptr passed to function PrintItem\n");
}

HEADER_B* InsertItem(HEADER_B* pd, char* pNewItemID) {
	if (!pd) {
		printf("Invalid input to function InsertItem!\n");
		return 0;
	}
	if (!(*pNewItemID)) *pNewItemID = 0;
	else if (!nameCheck(pNewItemID)) {
		printf("Bad name format!\n");
		return 0;
	}
	ITEM9* newItem = (ITEM9*)GetItem(9, (char*)pNewItemID);
	char* itemName = newItem->pID;
	char buf[3];
	getNameArr(itemName, buf);

	HEADER_B* headersb[2];
	FindHeaderB(headersb, pd, buf[0]);
	HEADER_B* ps = headersb[0];
	if (ps == 0) {
		HEADER_B* previous = headersb[1];
		previous->pNext = CreateHeaderB(buf[0], buf[1], newItem);
		if (previous->pNext == nullptr) {
			printf("Creating header failed: nullptr returned from function CreateHeaderB\n");
			return 0;
		}
		else {
			return pd;
		}
	}

	HEADER_A* headersa[2];
	FindHeaderA(headersa, ps->pHeaderA, buf[1]);
	HEADER_A* pss = headersa[0];
	if (pss == 0) {
		HEADER_A* previous = headersa[1];
		previous->pNext = CreateHeaderA(buf[1], newItem);
		if (previous->pNext == nullptr) {
			printf("Creating header failed: nullptr returned from function CreateHeaderA\n");
			return 0;
		}
		else {
			return pd;
		}
	}

	ITEM9* items[2];
	FindItem(items, (ITEM9*)pss->pItems, itemName);
	ITEM9* item = items[0];
	if (item == 0) {
		ITEM9* previous = items[1];
		previous->pNext = newItem;
		return pd;
	}
	else {
		printf("Error: Item ID already exists!\n");
	}
	return 0;
}

HEADER_B* RemoveItem(HEADER_B* pd, char* pItemID)
{
	if (!pd) {
		printf("Invalid input to function InsertItem!\n");
		return 0;
	}
	if (!nameCheck(pItemID)) {
		printf("Bad name format!\n");
		return 0;
	}
	bool success = false;
	char buf[3];
	getNameArr(pItemID, buf);
	HEADER_B* headersb[2];
	FindHeaderB(headersb, pd, buf[0]);
	HEADER_B* ps = headersb[0];
	HEADER_B* previousb = headersb[1];
	if (ps == 0) {
		printf("No item with that name found!\n");
		return 0;
	}

	HEADER_A* headersa[2];
	FindHeaderA(headersa, ps->pHeaderA, buf[1]);
	HEADER_A* pss = headersa[0];
	HEADER_A* previousa = headersa[1];
	if (pss == 0) {
		printf("No item with that name found!\n");
		return 0;
	}

	ITEM9* items[2];
	FindItem(items, (ITEM9*)pss->pItems, pItemID);
	ITEM9* item = items[0];
	if (item == 0) {
		printf("No item with that name found!\n");
		return 0;
	}
	else {
		ITEM9* previous = items[1];
		if (previous != nullptr) previous->pNext = item->pNext;
		else pss->pItems = item->pNext;
		freeOneItem(item);
		success = true;
	}

	if (!pss->pItems) {
		if (previousa != nullptr) previousa->pNext = pss->pNext;
		else ps->pHeaderA = pss->pNext;
		delete(pss);
	}

	if (!ps->pHeaderA) {
		if (previousb != nullptr) previousb->pNext = ps->pNext;
		else pd = pd->pNext;
		delete(ps);
	}

	if (success) return pd;
	return 0;
}

bool nameCheck(char* input) {
	int j = 0;
	for (unsigned int i = 0; i < strlen(input); i++) {
		if (!isalpha(input[i])) {
			if (input[i] == ' ') {
				if (j > 0) return false;
				j++;
			}
		}
	}
	if (j == 1) return true;
	else return false;
}

void getNameArr(char* input, char* buf) {
	char* buf2 = new char(sizeof(input));
	if (buf2 == nullptr) {
		printf("Error: creating buffer failed!\n");
		return;
	}
	strcpy(buf2, input);
	char* token = strtok(buf2, " ");
	buf[0] = token[0];
	token = strtok(NULL, " ");
	if (!token) {
		printf("Error: ID only one word!\n");
		return;
	}
	buf[1] = token[0];
	buf[2] = '\0';
	if (token = strtok(NULL, " ")) {
		printf("Too many words in item ID!\n");
		return;
	}
}

void FindHeaderB(HEADER_B** headers, HEADER_B* header, char cBegin)
{
	HEADER_B* p;
	HEADER_B* previous = nullptr;
	for (p = header; p && p->cBegin != cBegin; p = p->pNext)
	{
		previous = p;
	}
	headers[0] = p;
	headers[1] = previous;
}

void FindHeaderA(HEADER_A** headers, HEADER_A* header, char cBegin)
{
	HEADER_A* p;
	HEADER_A* previous = nullptr;
	for (p = header; p && p->cBegin != cBegin; p = p->pNext)
	{
		previous = p;
	}
	headers[0] = p;
	headers[1] = previous;
}
void FindItem(ITEM9** items, ITEM9* item, char* pID)
{
	ITEM9* p;
	ITEM9* previous = nullptr;
	for (p = item; p && strcmp(p->pID, pID); p = p->pNext)
	{
		previous = p;
	}
	items[0] = p;
	items[1] = previous;
}

HEADER_A* CreateHeaderA(char cBegin, ITEM9* newItem) {
	HEADER_A* newHeader = new HEADER_A;
	if (newHeader != nullptr) {
		newHeader->cBegin = cBegin;
		newHeader->pItems = newItem;
		newHeader->pNext = nullptr;
		return newHeader;
	}
	return nullptr;
}

HEADER_B* CreateHeaderB(char cBeginB, char cBeginA, ITEM9* newItem) {
	HEADER_B* newHeader = new HEADER_B;
	if (newHeader != nullptr) {
		newHeader->cBegin = cBeginB;
		newHeader->pHeaderA = CreateHeaderA(cBeginA, newItem);
		newHeader->pNext = nullptr;
		return newHeader;
	}
	return nullptr;
}

void freeItem(ITEM9* item)
{
	ITEM9* tmp = item;
	while (tmp) {
		ITEM9* curr = tmp;
		tmp = tmp->pNext;
		freeOneItem(curr);
	}
}

void freeOneItem(ITEM9* item)
{
	delete(item->pID);
	delete(item->pDate->pMonth);
	delete(item->pDate);
	delete(item);
}

void freeHeaderA(HEADER_A* header)
{
	HEADER_A* tmp = header;
	while (tmp) {
		HEADER_A* curr = tmp;
		tmp = tmp->pNext;
		freeItem((ITEM9*)curr->pItems);
		delete(curr);
	}
}

void freeHeaderB(HEADER_B* header)
{
	HEADER_B* tmp = header;
	while (tmp) {
		HEADER_B* curr = tmp;
		tmp = tmp->pNext;
		freeHeaderA(curr->pHeaderA);
		delete(curr);
	}
}