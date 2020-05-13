#include <iostream>
#include "Volume.h"
using namespace std;

int main() {
	FILE* vol{};

	cout << "Do you want to open or create a volume (o/c): ";
	char userChoice;
	cin >> userChoice;
	if (userChoice == 'c') {
		char* p = new char[14];
		while (true) {
			cout << "Input name of volume (max 10 characters): ";
			cin.ignore(); //clear buffer
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
			cin.ignore(); //clear buffer
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

	bool loop = true;
	while (loop) {
		cout << "What you want to do: " << endl;
		cout << "1. Import file" << endl;
		cout << "2. Export file" << endl;
		cout << "3. List of file" << endl;
		cout << "0. Exit" << endl;
		cout << "Type here: ";
		int choice;
		cin >> choice;

		switch (choice) {
		case 0:
			loop = false;
			break;
		case 1:
			cout << "Write file name that you want to import: ";
			char* filePath = new char[100];
			cin.ignore(); //clear buffer
			cin.getline(filePath, 100);

			if (importFileToVol(vol, filePath)) {
				cout << "Import file successful!" << endl;
			}
			else {
				cout << "Cant not open file" << endl;
			}
		}
	}

	fclose(vol);
	return 0;
}