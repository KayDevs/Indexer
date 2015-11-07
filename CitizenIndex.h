#include <iostream>
#include <fstream>

using namespace std;

#ifndef CITIZENINDEX_H
#define CITIZENINDEX_H

/*
This file contains the index structure as well as the Citizens class definitions.
*/

//These are the lengths of each datum in the Citizen structure.
const int SSN_SIZE = 10, NAME_SIZE = 51, ADDR_SIZE = 51, PHONE_SIZE = 14;
//(and dob is an 8-digit unsigned int)

//used for reading and writing to the file.
struct Citizen
{
	char ssn[SSN_SIZE]; 
	char name[NAME_SIZE]; 
	char addr[ADDR_SIZE]; 
	char phone[PHONE_SIZE]; 
}; 

//used for indexing the file.
struct CitizenIndex
{
	char ssn[SSN_SIZE];
	unsigned int fileIndex;
};

//Container class for CitizenIndex structs
class Citizens
{
	private:
		//This is the size of the array to start with.
		int ARRAY_SIZE;
		CitizenIndex *indices; //THE MAIN THING (it's a dynamically-allocated array)
		int search(char[SSN_SIZE]);
	public:
		Citizens(fstream& database);
		void add(char[SSN_SIZE], char[NAME_SIZE], char[ADDR_SIZE], char[PHONE_SIZE], fstream& database);
		void remove(char[SSN_SIZE], fstream& database);
		void update(char[SSN_SIZE], fstream& database);
		void display(char[SSN_SIZE], fstream& database);
		void displayAll(fstream& database);
	
};

#endif