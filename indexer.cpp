#include <iostream>
#include <fstream>
#include <string>
#include "CitizenIndex.h"

//I couldn't care less about global namespace pollution
using namespace std;

/*
This file contains the main program loop as well as the command-driven interface.
It also handles loading and unloading of the file.
*/

//you can't switch(strings) so I am using an enum for command interpretation
enum commands 
{
	ADD,
	UPDATE,
	DISPLAY,
	DISPLAYALL,
	HELP,
	QUIT
};
commands hash_commands(string input)
{
	if(input == "add") return ADD;
	if(input == "update") return UPDATE;
	if(input == "display") return DISPLAY;
	if(input == "displayall") return DISPLAYALL;	
	if(input == "help") return HELP;
	if(input == "quit") return QUIT;
}

int main(int argc, char** argv)
{
	//read file and get handle
	cout<<"Reading file..."<<endl;
	fstream database;
	if(!database)
	{
		cout<<"Error reading from file. Aborting."<<endl;
		return 0;
	}
	
	//create the object
	Citizens citizens(database);
	
	//enter command loop
	string command;
	cout<<"What would you like to do? (type help for a list of options)"<<endl;
	do
	{
		cout<<">";
		cin>>command;
		cin.ignore();
		cout<<endl;
		
		char ssn[SSN_SIZE]; //reusable
		
		switch(hash_commands(command)) {
		case HELP:
			cout<<"List of available commands:"<<endl;
			cout<<"add        - add a record to the database"<<endl;
			cout<<"update     - update information about an existing record"<<endl;
			cout<<"display    - display information about an existing record"<<endl;
			cout<<"displayall - display information about all records"<<endl;
			cout<<"help       - show help"<<endl;
			cout<<"quit       - exit the program"<<endl;
			break;
		case DISPLAY:
			cout<<"What is the social security number of the record you would like to view?"<<endl;
			cin>>ssn;
			citizens.display(ssn, database);
			break;
		case DISPLAYALL:
			citizens.displayAll(database);
			break;	
		case ADD:
			char name[NAME_SIZE]; 
			char addr[ADDR_SIZE]; 
			char phone[PHONE_SIZE];
			cout<<"Please enter the following new information: "<<endl;
			cout<<"Please enter the social security number."<<endl;
			cin.getline(ssn, SSN_SIZE);
			cout<<"Please enter the first and last name, separated by a space."<<endl;
			cin.getline(name, NAME_SIZE);
			cout<<"Please enter the phone number."<<endl;
			cin.getline(phone, PHONE_SIZE);
			cout<<"Please enter the address."<<endl;
			cin.getline(addr, ADDR_SIZE);
			citizens.add(ssn, name, addr, phone, database);
			break;
		case UPDATE:
			cout<<"What is the social security number of the record you would like to edit?"<<endl;
			cin>>ssn;
			citizens.update(ssn, database);
			break;
		}
	
	}
	while (command != "quit");
	database.close();
	return 0;
}
