#include <iostream>
#include "Volume.h"
using namespace std;

int main() {
	cout << "Do you want to open or create a volume (o/c): ";
	char userChoice;
	cin >> userChoice;

	if (userChoice == 'c') {
		cout << "Input name of volume (max 10 characters): ";
		char* p = new char[14];
		cin.ignore(); //clear buffer
		cin.getline(p, 14);
		cout << "Input volume size (mb): ";
		int volSize;
		cin >> volSize;
		if (!createNewVol(p, volSize)) {
			cout << "Error!";
		}
		else {
			cout << "Create success!!!" << endl;
		}
	}

	return 0;
}