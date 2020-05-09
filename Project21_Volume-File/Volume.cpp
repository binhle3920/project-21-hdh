#include "Volume.h"

Volume::Volume() {
	this->volExtension = "bin";
	this->volSize = 512;
}

Volume::~Volume() {}

bool Volume::createNewVol(const char *path, int size) {
	//Init vol name and size
	this->volSize = size;
	this->volName = new char[strlen(path) - 4];
	for (int i=0; i< strlen(path) - 4; i++) {
		volName[i] = path[i];
	}

	FILE* fcreate;
	fcreate = fopen(path, "wb");
	
	if (fcreate == NULL) {
		cout << "Cant create file!" << endl;
		return 0;
	}

	//Create a file with fixed size and zero value
	for (long long i = 0; i < (size * 1000000); i++) {
		fwrite("", 1, 1, fcreate);
	}

	//Write name to the file
	fseek(fcreate, 0, SEEK_SET);
	fwrite(this->volName, 1, strlen(path) - 4, fcreate);

	//Write size to the file

	fclose(fcreate);
	return 1;
}