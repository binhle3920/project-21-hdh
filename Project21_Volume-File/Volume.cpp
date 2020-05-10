#include "Volume.h"

void writeJumpCode(FILE* f) {
	//Write JUMP CODE to the file (3 bytes, EB 34 90 (hex))
	fseek(f, 0, SEEK_SET);
	string hexArray[3] = { "EB", "34", "90" };
	char* codeInArray = new char[3];
	for (int i = 0; i < 3; i++) {
		codeInArray[i] = char(hexToDecimal(hexArray[i]));
	}
	
	fwrite(codeInArray, 1, 3, f);
	delete[] codeInArray;
	//DONE
}

void writeBytePerSector(FILE* f) {
	//Write BYTE PER SECTOR (2 bytes, 02 00 (hex))
	fseek(f, 11, SEEK_SET);
	string hexArray[2] = { "02", "00" };
	char* codeInArray = new char[2];
	for (int i = 0; i < 2; i++) {
		codeInArray[i] = char(hexToDecimal(hexArray[i]));
	}
	fwrite(codeInArray, 1, 2, f);
	delete[] codeInArray;
	//DONE
}

void writeSectorPerCluster(FILE* f) {
	//Write sector per cluster (1 bytes, 02(hex))
	fseek(f, 13, SEEK_SET);
	int value =  2; 
	fwrite(&value, 1, 1, f);
	//DONE
}

void writeSectorOfBoot(FILE* f) {
	//Number sector of BOOT is 8 (2 bytes ==> 00 08(hex))
	fseek(f, 14, SEEK_SET);
	string hexArray[2] = { "00", "08" };
	char* codeInArray = new char[2];
	for (int i = 0; i < 2; i++) {
		codeInArray[i] = char(hexToDecimal(hexArray[i]));
	}
	fwrite(codeInArray, 1, 2, f);
	delete[] codeInArray;
	//DONE
}

void writeNumSectorOfVol(FILE* f, int size) {
	fseek(f, 19, SEEK_SET);
	int32_t numSector = (size * 1000000) / BYTE_PER_SECTOR;
	string* hexArray = decimalToHex(4, numSector);
	char* codeInArray = new char[4];
	for (int i = 0; i < 4; i++) {
		codeInArray[i] = char(hexToDecimal(hexArray[i]));
	}
	fwrite(codeInArray, 1, 4, f);
	delete[] codeInArray;
	delete[] hexArray;
}

void writeSize(FILE* f, int size) {
	fseek(f, 23, SEEK_SET);
	string* hexArray = decimalToHex(2, size);
	char* codeInArray = new char[2];
	for (int i = 0; i < 2; i++) {
		codeInArray[i] = char(hexToDecimal(hexArray[i]));
	}
	fwrite(codeInArray, 1, 2, f);
	delete[] codeInArray;
	delete[] hexArray;
}
bool createNewVol(const char* path, int size) {
	FILE* fcreate;
	fcreate = fopen(path, "wb");

	if (fcreate == NULL) {
		cout << "Cant create file!" << endl;
		return 0;
	}

	//Create a file with fixed size and zero value
	for (int i = 0; i < size; i++) {
		for (long j = 0; j < 1000000; j++) {
			fwrite("", 1, 1, fcreate);
		}
	}
	
	writeJumpCode(fcreate);
	writeBytePerSector(fcreate);
	writeSectorPerCluster(fcreate);
	writeSectorOfBoot(fcreate);
	writeNumSectorOfVol(fcreate, size);
	writeSize(fcreate, size);


	fclose(fcreate);
	return 1;
}

