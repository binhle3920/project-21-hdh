#include <iostream>
#include "Volume.h"
using namespace std;

int main() {
	Volume Vol;

	cout << "Do you want to open or create a volume (o/c): ";
	char user;
	cin >> user;

	if (user == 'c') {
		cout << "Input name of volume (max 10 characters): ";
		char* p = new char[14];
		cin.ignore(); //clear buffer
		cin.getline(p, 14);
		cout << "Input volume size (mb): ";
		int volSize;
		cin >> volSize;
		if (!Vol.createNewVol(p, volSize)) {
			cout << "Error!";
		}	
	}

	return 0;
}