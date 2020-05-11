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

	//cout << readNumSectorOfVol(vol) << endl;

	fclose(vol);
	return 0;
}