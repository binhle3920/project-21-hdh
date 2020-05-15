#include <iostream>

#include "Volume.h"
#include <ios>
#include <limits>
using namespace std;


int main() {
	FILE* vol{};

	cout << "Do you want to open or create a volume (o/c): ";
	char userChoice;
	cin >> userChoice;
	cin.ignore();
	if (userChoice == 'c') {
		char* p = new char[14];
		while (true) {
			cout << "Input name of volume (max 10 characters): ";
			cin.getline(p, 14);
			cout << "Input volume size (mb): ";
			int volSize;
			cin >> volSize;
			vol = createNewVol(p, volSize);
			if (vol == NULL) {
				cout << "Error!" << endl;
			}
			else {
				cout << "Create success!!!" << endl;
				break;
			}
		}
		delete[] p;
	}
	else if (userChoice == 'o') {
		//let the user input the path again if open get error
		char* p = new char[14];
		while (true) { 
			cout << "Input name of volume (max 10 characters): ";
			cin.getline(p, 14);
			vol = readVol(p);
			if (vol == NULL) {
				cout << "Error reading volume" << endl;
			} else {
				cout << "Open volume success" << endl;
				break;
			}
		}
		delete[] p;
	}

	char* filePath = new char[100];
	char* pass = new char[100];

	bool loop = true;
	while (loop) {
		
		int choice;

		cout << "What you want to do: " << endl;
		cout << "1. Import file" << endl;
		cout << "2. Export file" << endl;
		cout << "3. List of file" << endl;
		cout << "4. Delete file" << endl;
		cout << "5. Rename file" << endl;
		cout << "0. Exit" << endl;
		cout << "Type here: ";
		cin >> choice;
		cin.ignore();
		switch (choice) {
		case 0: {
			loop = false;
			break;
		}
		case 1: {
			cout << "Write file name that you want to import: ";
			cin.getline(filePath, 100);
			cout << "Set password for your file (y/n)?: ";
			char c;
			cin >> c;
			cin.ignore();
			unsigned long password = 0;
			if (c == 'n') {
				password = 0;
			}
			else {
				string s;
				s = hidePass();
				password = stringHashing(s);
			}

			if (importFileToVol(vol, filePath, password)) {
				cout << "Import file successful!" << endl;
			}
			else {
				cout << "Error in importing file!" << endl;
			}
			break;
		}
		case 2: {
			cout << "File you want to export: ";
			char* fileName = new char[31];
			cin.getline(fileName, 31);
			cout << "Output path: ";
			char* path = new char[100];
			cin.getline(path, 100);
			if (!exportFile(vol, path, fileName)) {
				cout << "Error in exporting file" << endl;
			}
			else {
				cout << "Export file success." << endl;
			}

			delete[] fileName;
			delete[] path;
			break;
		}
		case 3: {
			printListFile(vol);
			break;
		}
		case 4: {
			cout << "Write file name you want to delete: ";
			char* fileName = new char[31];
			cin.getline(fileName, 31);
			if (!deleteFile(vol, fileName)) {
				cout << "Cant find file with that name" << endl;
			}
			else {
				cout << "Delete file success!" << endl;
			}
			delete[] fileName;
			break;
		}
		case 5: {
			cout << "Write file name you want to rename: ";
			char* fileName = new char[31];
			cin.getline(fileName, 31);
			char* newName = new char[31];
			cout << "Write new name: ";
			cin.getline(newName, 31);
			if (!renameFile(vol, fileName, newName)) {
				cout << "Cant find file with that name" << endl;
			}
			else {
				cout << "Rename file success!" << endl;
			}
			delete[] fileName;
			break;
		}
		default: {
			cout << "Error command!" << endl;
			break;
		}
		}
	}

	delete[] pass;
	delete[] filePath;
	fclose(vol);
	return 0;
}