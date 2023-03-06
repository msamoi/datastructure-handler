#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <exception>
#include <string>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"

class DataStructure {
	void freeHeaderA(HEADER_A*);
	void freeHeaderB(HEADER_B*);
	void freeItem(ITEM9*);
	void freeOneItem(ITEM9*);
	HEADER_B* pHeader = 0;
	bool getNameArr(char* input, char* buf);
	void FindHeaderB(HEADER_B** headers, HEADER_B* header, char cBegin);
	void FindHeaderA(HEADER_A** headers, HEADER_A* header, char cBegin);
	void FindItem(ITEM9** items, ITEM9* item, char* pID);
	bool nameCheck(char* input);
	HEADER_A* CreateHeaderA(char cBegin, ITEM9* newItem);
	HEADER_B* CreateHeaderB(char cBeginB, char cBeginA, ITEM9* newItem);
	void Copy(DataStructure* input, const DataStructure& Original);
	friend void LightCLI(DataStructure* ds);
public:
	DataStructure();
	DataStructure(char* pFilename);
	~DataStructure();
	DataStructure(const DataStructure& Original);
	int GetItemsNumber();
	ITEM9* GetItem(char* pID);
	void operator+=(ITEM9*);
	void operator-=(char* pID);
	DataStructure& operator=(const DataStructure& Right);
	int operator==(DataStructure& Other);
	void Write(char* pFilename);
	friend std::ostream& operator<<(std::ostream& ostr, const DataStructure& str);
};

using namespace std;

int main() {
	cout << "Type \"yes\" to enter the light CLI interface or anything else to run default test." << endl << endl;
	string in;
	getline(cin, in);
	DataStructure* pds = new DataStructure;
	if (!in.compare("yes")) LightCLI(pds);
	else {
		for (int i = 0; i < 10; i++)
		{
			ITEM9* item = new ITEM9;
			item = (ITEM9*)GetItem(9);
			*pds += item;
		}
		cout << "New structure:" << endl << * pds << endl << "The number of items in the data structure:" << pds->GetItemsNumber() << endl << endl;
		ITEM9* gottenItem = pds->GetItem((char*)"Light Cyan");
		if (gottenItem) cout << "Found: " << gottenItem->pID << "\nCode:  " << gottenItem->Code << endl << endl;
		gottenItem = pds->GetItem((char*)"X X");
		if (gottenItem) cout << gottenItem->pID;
		DataStructure pdscopy = *pds;
		*pds -= (char*)"Banana Mania";
		*pds -= (char*)"Persian Green";
		*pds -= (char*)"Vegas Gold";
		cout << "Initial structure:" << endl << * pds << endl << endl
			 << "Copied structure:" << endl << pdscopy << endl << endl
			 << "Comparison between initial and copied structure: " << (*pds == pdscopy) << endl << endl;
		pds->Write((char*)"test.dat");
		DataStructure pdsread = DataStructure((char*)"test.dat");
		cout << "Comparison between written and read structure: " << (*pds == pdsread) << endl << endl;
		cout << "Read structure:" << endl << pdsread << endl;
		pdsread = pdscopy;
	}
	return 0;
}

void LightCLI(DataStructure* ds) {	// simple CLI, uses all the class functions created for the project
	cout << endl << "Usage:" << endl	// print out usage message upon running CLI function, self-explanatory
		 << "-help             : Prints out this help message." << endl
		 << "-open <filename>  : Loads data from requested file. Use underscore instead of space" << endl
		 << "-save <filename>  : Saves data to specified file." << endl
		 << "-add  <item name> : Adds item with specified name to current data structure. Format: Firstname Lastname" << endl
		 << "-del  <item name> : Removes item with specified name from current data structure. Format: Firstname Lastname" << endl
		 << "-new  <count>     : Makes a new data structure with specified amount of members" << endl
		 << "-num              : Counts the number of elements in current data structure." << endl
		 << "-print            : Prints out current data structure." << endl
		 << "-exit             : Exits CLI app." << endl << endl;
	while (true) {
		string input;
		getline(cin, input);			// using getline for input because simply cin just gets a single word
		istringstream iss(input);		// istringstream for tokenizing the string
		string token;
		getline(iss, token, ' ');		// actually tokenizing it with getline
		try {
			if (!token.compare("-exit")) {			// if exit argument is given then just return
				break;
			}
			else if (!token.compare("-help")) {		// if help argument is given, reprint the usage message
				cout << endl << "Usage:" << endl
					<< "-help             : Prints out this help message." << endl
					<< "-open <filename>  : Loads data from requested file. Use underscore instead of space" << endl
					<< "-save <filename>  : Saves data to specified file." << endl
					<< "-add  <item name> : Adds item with specified name to current data structure. Format: Firstname Lastname" << endl
					<< "-del  <item name> : Removes item with specified name from current data structure. Format: Firstname Lastname" << endl
					<< "-new  <count>     : Makes a new data structure with specified amount of members" << endl
					<< "-num              : Counts the number of elements in current data structure." << endl
					<< "-print            : Prints out current data structure." << endl
					<< "-exit             : Exits CLI app." << endl << endl;
			}
			else if (!token.compare("-print")) {	// prints out the data structure
				cout << *ds << endl << endl;
			}
			else if (!token.compare("-open")) {		// if open argument is given, open the specified file
				getline(iss, token, ' ');
				if (!token.compare("-open")) {
					throw invalid_argument(string("No filename given!\n"));
				}
				char* in = (char*)token.c_str();
				*ds = DataStructure(in);
			}
			else if (!token.compare( "-save")) {	// if save argument is given, save file with specified name
				getline(iss, token, ' ');
				if (!token.compare("-save")) {
					throw invalid_argument(string("No filename given!\n"));
				}
				char* in = (char*)token.c_str();
				if (ds->pHeader) {
					ds->Write(in);
					cout << "Write complete!" << endl << endl;
				}
				else throw invalid_argument(string("Write failed: data structure empty!\n"));
			}
			else if (!token.compare("-add")) {		// if add argument is given, add item with specified name
				getline(iss, token, ' ');
				ITEM9* item = new ITEM9;
				if (!token.compare("-add")) {		// or generate name, if name is not given
					cout << "No item name given, randomly generated one will be added." << endl;
					item = (ITEM9*)GetItem(9);
				}
				else {
					string itemname;				// since item names are separated by spaces, a new string variable is created
					itemname += token;				// and the first and last name of the item are concatenated into the new string variable
					getline(iss, token, ' ');
					itemname = itemname + ' ' + token;
					item = (ITEM9*)GetItem(9, (char*)itemname.c_str());	// new string variable is then passed to GetItem function with the std::string.c_str() standard function
				}														// because GetItem requires an input of type char*
				*ds += item;
				if (item) cout << "Item added: " << item->pID << endl << endl;
			}
			else if (!token.compare("-del")) {		// if del argument is given, removing specified item
				getline(iss, token, ' ');
				if (!token.compare("-del")) {
					throw invalid_argument(string("No item name given!\n")); // fail if there is no item name
				}
				string itemname;					// same concatenation needed as in the add argument parsing section
				itemname += token;
				getline(iss, token, ' ');
				itemname = itemname + ' ' + token;
				if (ds->pHeader) {
					*ds -= (char*)itemname.c_str();
					cout << "Item deleted: " << itemname << endl << endl;
				}
				else throw invalid_argument(string("Removing item failed: data structure empty!\n"));
			}
			else if (!token.compare("-new")) {		// if new argument is given, generate specified amount of items with generated names.
				getline(iss, token, ' ');			// note: the generated names can and will repeat with larger amounts of generated names.
				if (!token.compare("-new")) {
					throw invalid_argument(string("Invalid count!\n")); // fail if the argument itself is the last item on the line, thus the count is missing
				}
				int count = atoi(token.c_str());
				if (count) {
					for (int i = 0; i < count; i++)
					{
						ITEM9* item = new ITEM9;
						item = (ITEM9*)GetItem(9);
						*ds += item;
					}
				}
				else throw invalid_argument(string("Not an integer!\n"));	// if the argument for -new isnt an integer, fail
			}
			else if (!token.compare("-num")) {
				if (ds->pHeader) {
					int num = ds->GetItemsNumber();
					if (num) cout << "There are " << num << " elements in the data structure." << endl << endl;
				}
				else throw invalid_argument(string("Counting item failed: data structure empty!\n"));
			}
			else {
				throw invalid_argument(string("Argument not recognized!\n"));	// if argument does not match any of the predefined arguments, throw exception
			}
		}
		catch (invalid_argument& e) {
			cout << e.what();
		}
	}
}

DataStructure::DataStructure() {		// simple constructor function, just allocates memory for the first element header b, a and item linked lists
	HEADER_B* pHeader = new HEADER_B;
}

DataStructure::DataStructure(char* pFilename) {			// this function loads data output by Write and creates a DataStructure instance from it.
	ifstream file;
	try {
		file.open(pFilename, ios::in | ios::binary);    // opening the file
		if (!file) throw invalid_argument(string("Could not open file ") + pFilename + "\n");
	}
	catch (invalid_argument& error) {
		cout << error.what() << endl;
		this->pHeader = 0;
		return;
	}
	cout << "Loading file " << pFilename << "... ";
	HEADER_B* pOut = new HEADER_B;						// memory for new pHeader element
	this->pHeader = pOut;
	string buffer;										// buffer for reading the data line by line. each line is one header_b instance.
	getline(file, buffer);
	while (file) {										// loop through file
		pOut->cBegin = buffer[0];						// first character of the line is ALWAYS cBegin if outputted by Write.
		buffer.erase(0, 1);								// remove the cbegin from the beginning of the line to parse the line more easily.
		istringstream buffB(buffer);					// new istringstream variable to tokenize the buffer.
		string headerBtoken;							// new string variable to store the tokens.
		while (!headerBtoken.compare("")) getline(buffB, headerBtoken, '|'); // tokenizing by |, looping it is needed because a line might start with the specified delimiter, 
														// in which case getline will first return an empty string and the next iteration will get the desired line.
		HEADER_A* ppOut = new HEADER_A;								// memory for header_a instance
		pOut->pHeaderA = ppOut;
		while (buffB) {
			ppOut->cBegin = headerBtoken[0];						// the first character in header_a instance is ALWAYS cbegin if outputted by Write.
			headerBtoken.erase(0, 1);								// remove the cbegin from the beginning of the header a entry to ease parsing items.
			istringstream buffItem(headerBtoken);					// new istringstream variable to further tokenize the header a entry.
			string itemtoken;										// new string variable to store these tokens as well.
			while (!itemtoken.compare("")) getline(buffItem, itemtoken, ','); // tokenizing by commas, because the item attributes are separated by commas. looping for the same reason as before.
			ITEM9* item = new ITEM9;								// memory for item
			ppOut->pItems = item;
			while (buffItem) {
				item->pID = new char[sizeof(itemtoken)];			// allocating memory for the pointer attributes of the current item
				strcpy_s(item->pID, sizeof(itemtoken), (char*)itemtoken.c_str()); // copying the current token's contents into the newly allocated memory
				getline(buffItem, itemtoken, ',');					// and moving forward in the tokens.
				char** endptr = nullptr;							// needed for strtoul
				item->Code = strtoul(itemtoken.c_str(), endptr, 10);	// non pointer value can just be assigned
				getline(buffItem, itemtoken, ',');					// since the data outputted by Write should always be consistent, every item will have 5 members.
				item->pDate = new DATE3;							// pID, Code, Day, pMonth, Year. The tokenized string is iterated over based on faith that the items cycle
				item->pDate->Day = atoi(itemtoken.c_str());			// every 5 members as they should. if faulty or self-made data is loaded, behavior is undefined
				getline(buffItem, itemtoken, ',');
				item->pDate->pMonth = new char[sizeof(itemtoken)];
				strcpy_s(item->pDate->pMonth, sizeof(itemtoken), (char*)itemtoken.c_str());
				getline(buffItem, itemtoken, ',');
				item->pDate->Year = atoi(itemtoken.c_str());
				if (getline(buffItem, itemtoken, ',')) {			// if the next token exists, create next element in item linked list and fill it out
					item->pNext = new ITEM9;
					item = item->pNext;
				}
				else item->pNext = nullptr;							// otherwise pointer to next element must be null
			}
			if (getline(buffB, headerBtoken, '|')) {				// same with header a instance
				ppOut->pNext = new HEADER_A;
				ppOut = ppOut->pNext;
			}
			else ppOut->pNext = nullptr;
		}
		if (getline(file, buffer)) {								// if the next line contains something, then we are not out of data
			pOut->pNext = new HEADER_B;								// more memory for the next member in header b linked list
			pOut = pOut->pNext;
		}
		else pOut->pNext = nullptr;									// otherwise we are out of data, pointer to next element must be null
	}
	cout << "File loaded!" << endl << endl;
	file.close();
}

DataStructure::~DataStructure() {		// simple destructor function
	if (pHeader) {						// if data structure contains something, free it
		freeHeaderB(pHeader);
		pHeader = nullptr;				// set pointer to it to null, since the memory is unallocated
	}
}

DataStructure::DataStructure(const DataStructure& Original) {
	Copy(this, Original);				// call the copy function that is shared with the assignment operator overloaded function
}

void DataStructure::Copy(DataStructure* input, const DataStructure& Original) { // function to copy every element in Original data structure to this
	HEADER_B* p;																// variable to loop through original linked list
	HEADER_B* pOut = new HEADER_B;												// variable to store new linked list in
	try {																		// pointing the pHeader pointer of this data structure to the new header b instance
		if (!pOut) throw logic_error(string("Creation of new header failed!\n"));
	}
	catch (logic_error& error) {
		cerr << error.what() << endl;
		return;
	}																				// if pOut creation failed, do not continue
	this->pHeader = pOut;
	for (p = Original.pHeader; p; p = p->pNext)									// loop through original data structure
	{
		pOut->cBegin = p->cBegin;												// copy the cBegin of header b over

		HEADER_A* ppOut = new HEADER_A;											// allocate memory for header a stored in header b
		pOut->pHeaderA = ppOut;													// and point pOut's pHeaderA pointer to the newly allocated memory

		for (HEADER_A* pp = p->pHeaderA; pp; pp = pp->pNext) {					// loop through the original header a linked list and copy over all contents to this
			ppOut->cBegin = pp->cBegin;											// copy the cBegin of header a over
			ITEM9* itemOut = new ITEM9;											// new item9 instance to store the items in current instance of header a
			ppOut->pItems = itemOut;											// point pItems pointer stored in current instance of header a to the new item9 instance

			for (ITEM9* item = (ITEM9*)pp->pItems; item; item = item->pNext) {  // loop through items linked list and copy over each item instance
				itemOut->pID = new char[strlen(item->pID) + 1];					// since pID is a char* pointer, it needs to be allocated memory and copied with strcpy
				strcpy_s(itemOut->pID, strlen(item->pID) + 1, item->pID);		// copying contents of original pID to new pID
				itemOut->Code = item->Code;										// non pointer attributes can simply be assigned
				itemOut->pDate = new DATE3;										// allocating memory for new date3 instance stored in item element
				itemOut->pDate->Day = item->pDate->Day;							// storing the actual date in the new date instance
				itemOut->pDate->pMonth = new char[strlen(item->pDate->pMonth) + 1];	// allocating memory for the pMonth char* pointer
				strcpy_s(itemOut->pDate->pMonth, strlen(item->pDate->pMonth) + 1, item->pDate->pMonth);	// copying over the contents of original pMonth
				itemOut->pDate->Year = item->pDate->Year;						// again, non pointer attribute can be assigned
				if (item->pNext) {												// if there is a next item in original linked list
					itemOut->pNext = new ITEM9;									// allocate memory for it in new linked list
					itemOut = itemOut->pNext;									// and move on to the next element
				}
				else {
					itemOut->pNext = nullptr;									// otherwise initialize the pNext pointer to nullptr so it is clear that it is the last item
				}
			}
			if (pp->pNext) {													// if there is a next header a instance in original linked list
				ppOut->pNext = new HEADER_A;									// allocate memory for it
				ppOut = ppOut->pNext;											// move on to next element
			}
			else {
				ppOut->pNext = nullptr;											// otherwise initialize pNext to nullptr
			}
		}

		if (p->pNext) {
			pOut->pNext = new HEADER_B;											// same idea for header b instance
			pOut = pOut->pNext;
		}
		else {
			pOut->pNext = nullptr;
		}
	}
}




int DataStructure::GetItemsNumber() {											// simple function to count items in the data structure
	int i = 0;
	for (HEADER_B* p = pHeader; p; p = p->pNext) {
		for (HEADER_A* p2 = p->pHeaderA; p2; p2 = p2->pNext) {
			for (ITEM9* p3 = (ITEM9*)p2->pItems; p3; p3 = p3->pNext) i++;		// loop through every item in the structure, and increment i for each element
		}
	}
	return i;																	// return number of elements
}

ITEM9* DataStructure::GetItem(char *pID) {										// function to find the specified item in the linked list
	char buf[3];																// buffer to store first letters of the pID string
	getNameArr(pID, buf);														// function that fills the buffer, makes finding the required headers a lot faster, especially with more items
	try {
		for (HEADER_B* p = pHeader; p; p = p->pNext) {								// loop through header B
			if (p->cBegin == buf[0]) {												// only go into header B if cBegin matches the first letter of the first word
				for (HEADER_A* p2 = p->pHeaderA; p2; p2 = p2->pNext) {				// loop through header A
					if (p2->cBegin == buf[1]) {										// only go into header A if cBegin matches the first letter of the second word
						for (ITEM9* p3 = (ITEM9*)p2->pItems; p3; p3 = p3->pNext)	// loop through item linked list
							if (!strcmp(p3->pID, pID)) return p3;					// if input matches pID of item, return the item instance
					}
				}
			}
		}
		throw invalid_argument(string("Item not found: ") + pID + "\n");
	}																				// if item wasnt found, return 0
	catch (invalid_argument &error) {
		cout << error.what() << endl;
	}
	return 0;
}

void DataStructure::operator+=(ITEM9* toAdd) {
	try {
		if (!toAdd) throw invalid_argument(string("Invalid argument to function operator+=\n"));
		char* itemName = toAdd->pID;
		char buf[3];
		getNameArr(itemName, buf); 	// creating a buffer and extracting cBegin for header a and header b from item name to find the necessary headers more easily

		HEADER_B* headersb[2];			// new variable containing found headersb instance and the one before it
		FindHeaderB(headersb, pHeader, buf[0]);
		HEADER_B* ps = headersb[0];
		if (ps == 0) {					// if right header b instance is not found, a new one is created at the end of header b linked list
			HEADER_B* previous = headersb[1];
			if (previous == 0) {
				this->pHeader = CreateHeaderB(buf[0], buf[1], toAdd);
			}
			else {
				previous->pNext = CreateHeaderB(buf[0], buf[1], toAdd);
				if (previous->pNext == nullptr) { // if creation failed, do not continue
					throw logic_error(string("Creating header failed: nullptr returned from function CreateHeaderB!\n"));
				}
			}
			return;
		}

		HEADER_A* headersa[2];			// same idea as with headersb variable
		FindHeaderA(headersa, ps->pHeaderA, buf[1]);
		HEADER_A* pss = headersa[0];
		if (pss == 0) {
			HEADER_A* previous = headersa[1];
			if (previous == 0) {
				ps->pHeaderA = CreateHeaderA(buf[1], toAdd);
			}
			else {
				previous->pNext = CreateHeaderA(buf[1], toAdd);
				if (previous->pNext == nullptr) {
					throw logic_error(string("Creating header failed : nullptr returned from function CreateHeaderA\n"));
				}
			}
			return;
		}

		ITEM9* items[2];				// and also here, the idea is the same
		FindItem(items, (ITEM9*)pss->pItems, itemName);
		ITEM9* newItem = items[0];
		if (newItem == 0) {				// if item is not found, new item is connected to last item in list
			ITEM9* previous = items[1];
			previous->pNext = toAdd;
			return;
		}
		else {							// if item is found, fail
			throw invalid_argument(string("Error: Item ID already exists!\n"));
		}
		return;
	}
	catch (invalid_argument& error) {
		cout << error.what();
	}
	catch (logic_error& error) {
		cout << error.what();
	}

}

void DataStructure::operator-=(char* pID)
{
	try {
		if (!pHeader) {					// if there is no header to insert to, fail
			throw invalid_argument(string("Invalid input to function InsertItem!\n"));
		}
		nameCheck(pID);					// check the name, will throw invalid_argument' if name is not in the right format
		bool success = false;			// boolean value to signify whether function failed or not, because in case of success
		char buf[3];					// returning instantly is not feasible, as there should also be a check for empty headers
		getNameArr(pID, buf);			// buffer for extracting the first letters of both words in the ID of the item to add,
										   // to find the right headers more easily
		HEADER_B* headersb[2];			   // variable to store found header and the one before it
		FindHeaderB(headersb, pHeader, buf[0]);  // function that finds matching header b for the item
		HEADER_B* ps = headersb[0];
		HEADER_B* previousb = headersb[1];
		if (ps == 0) {						// if header with this letter is not found, the item with the name also does not exist
			throw invalid_argument(string("No item with that name found!\n"));
		}

		HEADER_A* headersa[2];				// same idea as with variable headersb
		FindHeaderA(headersa, ps->pHeaderA, buf[1]); // finding the header a the word is stored in
		HEADER_A* pss = headersa[0];
		HEADER_A* previousa = headersa[1];
		if (pss == 0) {								 // if header is not found, the item also does not exist
			throw invalid_argument(string("No item with that name found!\n"));
		}

		ITEM9* items[2];					// same idea as with the header array variables
		FindItem(items, (ITEM9*)pss->pItems, pID); // finding the item
		ITEM9* item = items[0];
		if (item == 0) {					// if item is not found, fail
			throw invalid_argument(string("No item with that name found!\n"));
		}
		else {
			ITEM9* previous = items[1];		// get the previous item from the item to be deleted
			if (previous != nullptr) previous->pNext = item->pNext; // set the previous item's pNext pointer to the item after the item to be deleted
			else pss->pItems = item->pNext;							// unless there is no previous item, in which case set the header a pItems pointer to the next item
			freeOneItem(item);				// free the required item
			success = true;					// set success to true, because there are things to do before returning
		}

		if (!pss->pItems) {					// check if the header a instance became empty from removing the item, if it did, start removing the header
			if (previousa != nullptr) previousa->pNext = pss->pNext; // if it wasnt the first header a in the linked list, connect the previous and next elements
			else ps->pHeaderA = pss->pNext;							 // if it was, set next element as first element
			delete(pss);											 // delete the empty header
		}

		if (!ps->pHeaderA) {				// check if the header b instance became empty from removing the item, if it did, remove header
			if (previousb != nullptr) previousb->pNext = ps->pNext; // if it wasnt the first header, connect previous and next elements
			else pHeader = pHeader->pNext;							// if it was the first element, make the next element first member
			delete(ps);												// delete empty header
		}
		if (!success) throw logic_error(string("Removing item failed!\n")); // if success isn't true, then the program did not succeed for some reason (this shouldn't be reachable)
	}
	catch (invalid_argument &error) {
		cout << error.what() << endl;
	}
	catch (logic_error& error) {
		cout << error.what() << endl;
	}
}

int DataStructure::operator==(DataStructure& Other) { // function to compare two linked lists, most likely inefficient
	if (this->pHeader == Other.pHeader) return 1;
	for (HEADER_B* p = pHeader; p; p = p->pNext) {
		for (HEADER_A* p2 = p->pHeaderA; p2; p2 = p2->pNext) { 			// loop through every item in first data structure and find the item in the second one
			for (ITEM9* p3 = (ITEM9*)p2->pItems; p3; p3 = p3->pNext)	// if an element is not found, return 0
				if (Other.GetItem(p3->pID) == 0) return 0;
		}
	}

	for (HEADER_B* p = Other.pHeader; p; p = p->pNext) {
		for (HEADER_A* p2 = p->pHeaderA; p2; p2 = p2->pNext) {			// the same has to be repeated for the second data structure, because there might be elements
			for (ITEM9* p3 = (ITEM9*)p2->pItems; p3; p3 = p3->pNext)	// in the second data structure that aren't in the first one
				if (this->GetItem(p3->pID) == 0) return 0;
		}
	}

	return 1; // return 1 if there were no mismatched items
}

DataStructure& DataStructure::operator=(const DataStructure& Right) {
	try {
		if (this == &Right) return *this;				// if this points to the same data structure as Right, then nothing can be done
		if (!Right.pHeader) throw invalid_argument(string("Failed: data structure to be assigned is empty!\n"));
		if (this->pHeader) {							// if the datastructure to be assigned doesn't exist, don't reassign.
			freeHeaderB(this->pHeader);						// if there is something in the data structure, delete it in preparation
		}
	}
	catch (invalid_argument &e) {
		cout << e.what() << endl << endl;
		return *this;
	}
	Copy(this, Right);								// copy everything from Right to this
	return *this;
}

void DataStructure::Write(char* pFilename) {							// function for writing DataStructure contents into a file for later parsing
	ofstream file(pFilename, ios::out|ios::binary|ios::trunc);			// fstream instance
	try {
		HEADER_B* pHeaderB = this->pHeader;
		if (!pHeaderB) {
			throw invalid_argument(string("Nullptr passed to function PrintStruct!\n")); // if there is no contents, fail
		}
		cout << "Writing to file " << pFilename << "... ";
		for (pHeaderB; pHeaderB; pHeaderB = pHeaderB->pNext) {				// loop through header b
			HEADER_A* pHeaderA = pHeaderB->pHeaderA;
			file << pHeaderB->cBegin;										// first letter of every line is the cBegin member of header b
			for (pHeaderA; pHeaderA; pHeaderA = pHeaderA->pNext) {			// loop through header a
				ITEM9* item = (ITEM9*)pHeaderA->pItems;
				file << "|" << pHeaderA->cBegin;							// | is used to signify borders between header as, and the first letter after | is the cbegin of any given header a
				for (item; item; item = item->pNext) {
					file << item->pID << ","								// write item data directly after cbegin member of header a separated by commas for easy tokenization
						<< item->Code << ","
						<< item->pDate->Day << ","
						<< item->pDate->pMonth << ","
						<< item->pDate->Year << ",";
				}
			}
			file << endl;													// new line only after every header b instance
		}
		cout << "Writing done!" << endl << endl;
	}
	catch (invalid_argument &error) {
		cout << error.what() << endl;
	}
	file.close();
}

std::ostream& operator<<(std::ostream& ostr, const DataStructure& str) {	// function for printing out the data structure, formatted a little nicely
	HEADER_B* pHeaderB = str.pHeader;
	try {																		// new variable for the header b in the data structure, to iterate through it freely
		if (!pHeaderB) throw invalid_argument(string("Printing failed: data structure is empty!\n")); // if there is no header b, there is nothing to be print
		for (pHeaderB; pHeaderB; pHeaderB = pHeaderB->pNext) {					// loop through header b
			HEADER_A* pHeaderA = pHeaderB->pHeaderA;							// make a variable for the header a contained in header b
			for (pHeaderA; pHeaderA; pHeaderA = pHeaderA->pNext) {				// loop through header a
				ITEM9* item = (ITEM9*)pHeaderA->pItems;							// make a variable for the item linked list contained in header a
				for (item; item; item = item->pNext) {							// loop through item linked list and print out every element's attributes
					ostr << left << setw(20) << setfill(' ') << item->pID << " "
						<< right << setw(11) << setfill(' ') << item->Code << " "
						<< right << setw(3) << setfill(' ') << item->pDate->Day << " "
						<< left << setw(10) << setfill(' ') << item->pDate->pMonth << " "
						<< right << setw(6) << setfill(' ') << item->pDate->Year
						<< endl;
				}
			}
		}
		return ostr; // return the output stream instance
	}
	catch (invalid_argument &error) {
		cout << error.what() << endl;
		return ostr;
	}
}

bool DataStructure::nameCheck(char* input) { // function for checking the format of the inputted name
	int j = 0;								 // counter for spaces in the name
	for (unsigned int i = 0; i < strlen(input); i++) { // looping through the string
		if (isalpha(input[i]) == 0) {			 // check if the current character is a letter, if it isn't, the format isnt right, fail
			if (input[i] == ' ') {			// if there is a space, increase the space counter
				if (j > 0) throw invalid_argument(string("Name format does not match rules!\n"));
				j++;						// if a space is found and there already has been one, fail, as there are more than 2 words
			}
			else throw invalid_argument(string("Name format does not match rules!\n"));
		}
	}
	if (j == 1) return true; // if name matches format, return 1
	else throw invalid_argument(string("Name format does not match rules!\n"));
}

bool DataStructure::getNameArr(char* input, char* buf) {	// function for getting the first letters of each word in the input for looking up headers more easily
	istringstream buf2str(input);							// example: "Bud Green" will return "BG", as the item will go into HEADER_B with the letter B
	string token;
	getline(buf2str, token, ' ');
	buf[0] = token[0];										// first letter of first word is stored in buf[0]
	getline(buf2str, token, ' ');
	buf[1] = token[0];										// next word's first letter is put into buf[1]
	buf[2] = '\0';											// terminating zero
	return 1;
}

void DataStructure::FindHeaderB(HEADER_B** headers, HEADER_B* header, char cBegin) // function to find header by cBegin stored in it
{
	HEADER_B* p;												// variable to store current header instance
	HEADER_B* previous = nullptr;								// variable to store previous header instance
	for (p = header; p && p->cBegin != cBegin; p = p->pNext)
	{															// loop through header, setting previous to p with every loop
		previous = p;											// by the end of the loop, previous will be the one before p
	}															// because previous will not be incremented with the final loop
	headers[0] = p;												// set the p and previous variables into the array passed into the function
	headers[1] = previous;
}

void DataStructure::FindHeaderA(HEADER_A** headers, HEADER_A* header, char cBegin) // same exact idea as the function above except for type header a
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
void DataStructure::FindItem(ITEM9** items, ITEM9* item, char* pID)					// same function except for type item9
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

HEADER_A* DataStructure::CreateHeaderA(char cBegin, ITEM9* newItem) {				// function that creates new header A instance
	HEADER_A* newHeader = new HEADER_A;												// allocating memory
	if (newHeader != nullptr) {														// if memory allocation succeeded, store the items in the header
		newHeader->cBegin = cBegin;
		newHeader->pItems = newItem;
		newHeader->pNext = nullptr;													// pNext is nullptr because new instance is added to the end of the linked list
		return newHeader;															// return the header instance
	}
	return nullptr;
}

HEADER_B* DataStructure::CreateHeaderB(char cBeginB, char cBeginA, ITEM9* newItem) { // same function except for type header B
	HEADER_B* newHeader = new HEADER_B;
	if (newHeader != nullptr) {
		newHeader->cBegin = cBeginB;
		newHeader->pHeaderA = CreateHeaderA(cBeginA, newItem);						 // calls CreateHeaderA because header b contains header a
		newHeader->pNext = nullptr;
		return newHeader;
	}
	return nullptr;
}

void DataStructure::freeItem(ITEM9* item)		// function to free linked list of items
{
	ITEM9* tmp = item;	// variable to loop through the linked list
	while (tmp) {
		ITEM9* curr = tmp; // new variable to remember current element
		tmp = tmp->pNext;  // reassign tmp to the next element before freeing current element
		freeOneItem(curr); // call function to free only one item
	}
}

void DataStructure::freeOneItem(ITEM9* item)	// function to free only one item
{
	delete(item->pID);				// deletes every pointer attribute the item instance has
	delete(item->pDate->pMonth);
	delete(item->pDate);
	delete(item);					// once every attribute is deleted, deletes instance
}

void DataStructure::freeHeaderA(HEADER_A* header)	// function to free linked list of header as
{
	HEADER_A* tmp = header; // same idea as the freeItem function
	while (tmp) {
		HEADER_A* curr = tmp;
		tmp = tmp->pNext;
		freeItem((ITEM9*)curr->pItems);
		delete(curr);
	}
}

void DataStructure::freeHeaderB(HEADER_B* header) // function to free linked list of header bs, same idea as freeItem and freeHeaderA
{
	HEADER_B* tmp = header;
	while (tmp) {
		HEADER_B* curr = tmp;
		tmp = tmp->pNext;
		freeHeaderA(curr->pHeaderA);
		delete(curr);
	}
}