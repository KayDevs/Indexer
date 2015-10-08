#include "CitizenIndex.h"
#include <cmath>

/*
This file contains the implementations for the Citizens functions defined in CitizenIndex.h
*/

/*
for reference:

struct CitizenIndex
{
	char ssn[SSN_SIZE];
	unsigned int fileIndex;
};

*/

int Citizens::search(char ssn[SSN_SIZE])
{
	cout<<"Searching for a match:"<<endl;
	for(int i = 0; i < ARRAY_SIZE; ++i)
	{
		string str_index_ssn = string(indices[i].ssn);
		string str_ssn = string(ssn);
		//cout<<"Comparing "<<str_index_ssn<<" to "<<str_ssn<<" at position "<<i<<endl;
		if(str_index_ssn == str_ssn)
		{
			cout<<"Match found at "<<i<<endl<<endl;
			return indices[i].fileIndex;
		}
	}
	//nothing was found at this point
	cout<<"No match found."<<endl<<endl;
	return -1; //error code
}

Citizens::Citizens(fstream& database)
{
	database.open("citizens.dat", ios::in | ios::app);
	ARRAY_SIZE = 0;
	database.seekg(0, database.end);
	cout<<database.tellg()<<" bytes in file."<<endl;
	ARRAY_SIZE = (int)database.tellg() / (int)sizeof(Citizen);
	cout<<"The array is "<<ARRAY_SIZE<<" elements long."<<endl;
	database.seekg(database.beg);
	
	//TODO: read into the indexing array.
	indices = new CitizenIndex[ARRAY_SIZE];
	for(int i = 0; i < ARRAY_SIZE; ++i)
	{
		database.seekg(i*sizeof(Citizen));
		Citizen citizen;
		database.read(reinterpret_cast<char *>(&citizen), sizeof(citizen));
		for(int j = 0; j < SSN_SIZE; ++j) {
			indices[i].ssn[j] = citizen.ssn[j];
		}
		indices[i].fileIndex = i;
	}
	database.close();
}

void Citizens::add(char ssn[SSN_SIZE], char name[NAME_SIZE], char addr[ADDR_SIZE], char phone[PHONE_SIZE], fstream& database)
{
	database.open("citizens.dat", ios::out | ios::app);
	Citizen citizen;
	//I hated every second of this.
	for(int i = 0; i < SSN_SIZE; ++i)
		citizen.ssn[i] = ssn[i];
	for(int i = 0; i < NAME_SIZE; ++i)
		citizen.name[i] = name[i];
	for(int i = 0; i < ADDR_SIZE; ++i)
		citizen.addr[i] = addr[i];
	for(int i = 0; i < PHONE_SIZE; ++i)
		citizen.phone[i] = phone[i];
		
	if(database.write(reinterpret_cast<char *>(&citizen), sizeof(citizen)))
	{
		cout<<"Write successful."<<endl;
		
		int key = ARRAY_SIZE;
		
		//expand the ARRAY and add a new entry to it.
		cout<<"location in file is: "<<key<<endl;
		ARRAY_SIZE++;
		
		CitizenIndex *tarray = new CitizenIndex[ARRAY_SIZE];
		cout<<"Repopulating array."<<endl;
		for(int i = 0; i < ARRAY_SIZE - 1; ++i) {
			cout<<i<<endl;
			for(int s = 0; s < SSN_SIZE; ++s) {
				tarray[i].ssn[s] = indices[i].ssn[s];
			}
			cout<<tarray[i].ssn<<endl;
			tarray[i].fileIndex = indices[i].fileIndex;
		}
		
		CitizenIndex citizen_index;
		for(int i = 0; i < SSN_SIZE; ++i) {
			citizen_index.ssn[i] = ssn[i];
		}
		citizen_index.fileIndex = key;
		
		tarray[ARRAY_SIZE-1] = citizen_index;
		
		delete [] indices;
		indices = tarray;
		
		cout<<"Added SSN: "<<indices[ARRAY_SIZE-1].ssn<<endl;
		cout<<"Array is "<<ARRAY_SIZE<<" elements long."<<endl;
	}
	else
	{
		cout<<"Write unsuccessful!"<<endl;	
	}
	
	database.close();
}

void Citizens::update(char ssn[SSN_SIZE], fstream& database)
{
	database.open("citizens.dat", ios::in | ios::out);
	Citizen citizen;
	int key = search(ssn);
	if(key != -1)
	{
		database.seekg(key * sizeof(citizen));
		database.read(reinterpret_cast<char *>(&citizen), sizeof(citizen));
		
		//UI Code
		string command;
		cout<<"Which property would you like to edit? (options are name, phone, and addr)"<<endl;
		cin>>command;
		cin.ignore();
		if(command == "name")
		{
			cout<<"Please enter the first and last name, separated by a space."<<endl;
			cin.getline(citizen.name, NAME_SIZE);
		}
		if(command == "phone")
		{
			cout<<"Please enter the phone number."<<endl;
			cin.getline(citizen.phone, PHONE_SIZE);
		}
		if(command == "addr")
		{
			cout<<"Please enter the address."<<endl;
			cin.getline(citizen.addr, ADDR_SIZE);
		}
		
		database.seekp(key * sizeof(Citizen));
		database.write(reinterpret_cast<char *>(&citizen), sizeof(Citizen));
	}
	database.close();
}

void Citizens::display(char ssn[SSN_SIZE], fstream& database)
{
	database.open("citizens.dat", ios::in);
	Citizen citizen;
	int key = search(ssn);
	if(key != -1)
	{
		database.seekg(key * sizeof(citizen));
		database.read(reinterpret_cast<char *>(&citizen), sizeof(citizen));
		cout<<"Name: "<<citizen.name<<endl;
		cout<<"SSN: "<<citizen.ssn<<endl;
		cout<<"Phone Number: "<<citizen.phone<<endl;
		cout<<"Lives at "<<citizen.addr<<endl;
	}
	database.close();
}

void Citizens::displayAll(fstream& database)
{
	database.open("citizens.dat", ios::in);
	Citizen citizen;
	cout<<"Reading..."<<endl;
	database.read(reinterpret_cast<char *>(&citizen), sizeof(citizen));
	cout<<"Read."<<endl;
	while(database.good())
	{
		cout<<"Name: "<<citizen.name<<endl;
		cout<<"SSN: "<<citizen.ssn<<endl;
		cout<<"Phone Number: "<<citizen.phone<<endl;
		cout<<"Lives at "<<citizen.addr<<endl;
		cout<<endl;
		database.read(reinterpret_cast<char *>(&citizen), sizeof(citizen));
	}
	database.close();
	
	/*cout<<"Summary of Index Array: "<<endl;
	for(int i = 0; i < ARRAY_SIZE; ++i) {
		cout<<"SSN at "<<i<<": "<<indices[i].ssn<<endl;
		cout<<"Key at "<<i<<": "<<indices[i].fileIndex<<endl;
	}*/
}