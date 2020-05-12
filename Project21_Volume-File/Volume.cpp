#include "Volume.h"

int32_t readValueOfVol(FILE* f, int numByteRead, int posRead) {
	fseek(f, 0, SEEK_SET);
	fseek(f, posRead, SEEK_SET);
	cout << ftell(f) << endl;
	char* buffer = new char[numByteRead];
	fread(buffer, 1, numByteRead, f);

	return charToInt(buffer, numByteRead);
}
int seekToEmptyEntry(FILE* f) {
	int curByte = -64;
	int countEntry = 1;
	fseek(f, curByte, SEEK_END);
	//Find the entry that is empty
	char checkEntry;
	fread(&checkEntry, 1, 1, f);
	while (checkEntry != '\0') {
		countEntry++;
		fseek(f, curByte * countEntry, SEEK_END);
		fread(&checkEntry, 1, 1, f);
	}
	fseek(f, -1, SEEK_CUR); //Move the pointer of file to the first byte of entry
	//The code above found the empty entry

	return countEntry;
}
long firstFreeCluster(FILE* f, int first, long numCluster) {
	//Num cluster is number cluster need to save a file
	//save the current byte to write "free cluster"
	long curPos = ftell(f);

	int firstFree = first;

	fseek(f, firstFree * SECTOR_PER_CLUSTER * BYTE_PER_SECTOR, SEEK_SET);
	char checkEntry;
	fread(&checkEntry, 1, 1, f);
	while (checkEntry != '\0') {
		firstFree++;
		fseek(f, firstFree * SECTOR_PER_CLUSTER * BYTE_PER_SECTOR, SEEK_SET);
		fread(&checkEntry, 1, 1, f);
	}
	fseek(f, -1, SEEK_CUR);

	//count if the number of cluster fit the size of file
	int countCluster = 0;
	int temp = firstFree;
	while (checkEntry == '\0' && countCluster < numCluster) {
		fseek(f, temp * SECTOR_PER_CLUSTER * BYTE_PER_SECTOR, SEEK_SET);
		fread(&checkEntry, 1, 1, f);
		temp++;
		countCluster++;
	}
	if (countCluster == numCluster) {
		fseek(f, curPos, SEEK_SET);
		return firstFree;
	}
	else {
		fseek(f, curPos, SEEK_SET);
		return firstFreeCluster(f, temp, numCluster);
	}
	//Return to the right position

}



void writeJumpCode(FILE* f) {
	//Write JUMP CODE to the file (3 bytes, EB 34 90 (hex))
	fseek(f, 0, SEEK_SET);
	string hexArray[3] = { "EB", "34", "90" };
	char* codeInArray = new char[3];
	for (int i = 0; i < 3; i++) {
		codeInArray[i] = char(hexToDecimal(hexArray[i]));
	}
	
	fwrite(codeInArray, 1, 3, f);
	fflush(f);
	delete[] codeInArray;
	//DONE
}
void writeBytePerSector(FILE* f) {
	//Write BYTE PER SECTOR (2 bytes, 02 00 (hex))
	fseek(f, NUM_BYTE_PER_SECTOR_POS, SEEK_SET);
	string hexArray[BYTE_PER_SECTOR_BYTE] = { "02", "00" };
	char* codeInArray = new char[BYTE_PER_SECTOR_BYTE];
	for (int i = 0; i < BYTE_PER_SECTOR_BYTE; i++) {
		codeInArray[i] = char(hexToDecimal(hexArray[i]));
	}
	fwrite(codeInArray, 1, BYTE_PER_SECTOR_BYTE, f);
	fflush(f);
	delete[] codeInArray;
	//DONE
}
void writeSectorPerCluster(FILE* f) {
	//Write sector per cluster (1 bytes, 02(hex))
	fseek(f, NUM_SECTOR_PER_CLUSTER_POS, SEEK_SET);
	int value =  2; 
	fwrite(&value, 1, SECTOR_PER_CLUSTER_BYTE, f);
	fflush(f);
	//DONE
}
void writeSectorOfBoot(FILE* f) {
	//Number sector of BOOT is 8 (2 bytes ==> 00 08(hex))
	fseek(f, NUM_SECTOR_OF_BOOT_POS, SEEK_SET);
	string hexArray[SECTOR_OF_BOOT_BYTE] = { "00", "08" };
	char* codeInArray = new char[SECTOR_OF_BOOT_BYTE];
	for (int i = 0; i < SECTOR_OF_BOOT_BYTE; i++) {
		codeInArray[i] = char(hexToDecimal(hexArray[i]));
	}
	fwrite(codeInArray, 1, SECTOR_OF_BOOT_BYTE, f);
	fflush(f);
	delete[] codeInArray;
	//DONE
}
void writeNumSectorOfVol(FILE* f, int size) {
	fseek(f, NUM_SECTOR_POS, SEEK_SET);
	int32_t numSector = (size * 1000000) / BYTE_PER_SECTOR;
	string* hexArray = decimalToHex(NUM_SECTOR_BYTE, numSector);
	char* codeInArray = new char[NUM_SECTOR_BYTE];
	for (int i = 0; i < NUM_SECTOR_BYTE; i++) {
		codeInArray[i] = char(hexToDecimal(hexArray[i]));
	}
	fwrite(codeInArray, 1, NUM_SECTOR_BYTE, f);
	fflush(f);
	delete[] codeInArray;
	delete[] hexArray;
}
void writeSize(FILE* f, int size) {
	fseek(f, SIZE_OF_VOL_POS, SEEK_SET);
	string* hexArray = decimalToHex(SIZE_OF_VOL_BYTE, size);
	char* codeInArray = new char[SIZE_OF_VOL_BYTE];
	for (int i = 0; i < SIZE_OF_VOL_BYTE; i++) {
		codeInArray[i] = char(hexToDecimal(hexArray[i]));
	}
	fwrite(codeInArray, 1, SIZE_OF_VOL_BYTE, f);
	fflush(f);
	delete[] codeInArray;
	delete[] hexArray;
}
void writeFirstFreeCluster(FILE* f, int first) {
	fseek(f, 25, SEEK_SET);
	string* hexArray = decimalToHex(4, first);
	char* codeInArray = new char[4];
	for (int i = 0; i < 4; i++) {
		codeInArray[i] = char(hexToDecimal(hexArray[i]));
	}
	fwrite(codeInArray, 1, 4, f);
	fflush(f);
	delete[] codeInArray;
	delete[] hexArray;
}
void writeFileName(FILE* f, const char* name, int length) {
	fseek(f, NAME_OF_VOL_POS, SEEK_SET);
	fwrite(name, 1, length, f);
	fflush(f);
}
void writeFileExtension(FILE* f, const char* extension, int length) {
	fseek(f, EX_OF_VOL_POS, SEEK_SET);
	fwrite(extension, 1, length, f);
	fflush(f);
}



void writeIFNameAndEx(FILE* f, const char* path) {
	//Write information of file to the entry
	//Find the find name and file extension to here
	char* fileName = new char[27];
	char* fileExtension = new char[4];
	for (int i = 0; i < 27; i++) {
		fileName[i] = '\0';
		if (i < 4) {
			fileExtension[i] = '\0';
		}
	}
	int curPos = strlen(path) - 1;
	int fileExtensionPos = 0;
	while (path[curPos] != '.') {
		fileExtension[fileExtensionPos++] = path[curPos--];
	}
	int fileNamePos = 0;
	while (curPos >= 0 && path[curPos] != '\\') {
		fileName[fileNamePos++] = path[curPos--];
	}
	_strrev(fileName);
	_strrev(fileExtension);
	fwrite(fileName, 1, 27, f);
	fwrite(fileExtension, 1, 4, f);
	fflush(f);
	//the code above file the file name and file extension
}
void writeEntryInDex(FILE* f, int index) {
	string* hex = decimalToHex(2, index);
	char* codeInArray = new char[2];
	for (int i = 0; i < 2; i++) {
		codeInArray[i] = char(hexToDecimal(hex[i]));
	}
	fwrite(codeInArray, 1, 2, f);
	fflush(f);
	delete[] codeInArray;
	delete[] hex;
}
void writeAttr(FILE* f, const char* path) {
	char attr = 0;

	DWORD subattr = GetFileAttributesA(path);
	if (subattr & FILE_ATTRIBUTE_READONLY)
		attr |= (1 << 0);
	if (subattr & FILE_ATTRIBUTE_HIDDEN)
		attr |= (1 << 1);
	if (subattr & FILE_ATTRIBUTE_SYSTEM)
		attr |= (1 << 2);
	if (subattr & FILE_ATTRIBUTE_DIRECTORY)
		attr |= (1 << 4);
	if (subattr & FILE_ATTRIBUTE_ARCHIVE)
		attr |= (1 << 5);
	fwrite(&attr, 1, 1, f);
}
void writeClusterStart(FILE* f, int size) {
	long clusterStart = firstFreeCluster(f, 4, size);
	string* hex = decimalToHex(4, clusterStart);
	char* codeInArray = new char[4];
	for (int i = 0; i < 4; i++) {
		codeInArray[i] = char(hexToDecimal(hex[i]));
	}
	fwrite(codeInArray, 1, NUM_SECTOR_BYTE, f);
	fflush(f);
	delete[] codeInArray;
	delete[] hex;
}
void writeSizeOfImportFile(FILE* f, int size) {
	string* hex = decimalToHex(4, size);
	char* codeInArray = new char[4];
	for (int i = 0; i < 4; i++) {
		codeInArray[i] = char(hexToDecimal(hex[i]));
	}
	fwrite(codeInArray, 1, NUM_SECTOR_BYTE, f);
	fflush(f);
	delete[] codeInArray;
	delete[] hex;
}


FILE* createNewVol(const char* path, int size) {
	FILE* fcreate;
	fcreate = fopen(path, "wb+");

	if (fcreate == NULL) {
		return fcreate;
	}

	//Create a file with fixed size and zero value
	for (int i = 0; i < size; i++) {
		for (long j = 0; j < 1000000; j++) {
			fwrite("", 1, 1, fcreate);
		}
	}
	fflush(fcreate);

	writeJumpCode(fcreate);
	writeBytePerSector(fcreate);
	writeSectorPerCluster(fcreate);
	writeSectorOfBoot(fcreate);
	writeNumSectorOfVol(fcreate, size);
	writeSize(fcreate, size);
	writeFirstFreeCluster(fcreate, 4);

	//The code below to create the vol name base on path
	char* name = new char[8]; //name have maximum 8 byte
	int length = 0; //count the lenght of name (bytes)
	for (int i = 0; i < strlen(path); i++) {
		if (path[i] == '.') {
			break;
		}
		length++; //increase length per character
		name[i] = path[i];
	}
	//The code above to create the vol name base on path

	//The code below to create the vol extension through path
	//extension have maximum 4 byte
	char* ex = new char[4];
	//path[length] currently is "."
	int curLength = length+1;
	int i;
	for (i = 0; i < 4; i++) {
		ex[i] = path[curLength];
		curLength++;
		//end of path string
		if (curLength == strlen(path)) {
			break;
		}
	}
	//The code above to create the vol extension through path


	writeFileName(fcreate, name, length);
	delete[] name;
	writeFileExtension(fcreate, ex, i + 1);
	delete[] ex;

	return fcreate;
}
FILE* readVol(const char* path) {
	FILE* file = fopen(path, "rb+");
	return file;
}

bool importFileToVol(FILE* vol, const char* path) {
	FILE* file = fopen(path, "rb");

	//Find file size
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	long numClusterNeeded = size / (SECTOR_PER_CLUSTER * BYTE_PER_SECTOR);

	
	if (file == NULL) {
		return 0;
	}


	int indexEntry = seekToEmptyEntry(vol);
	writeIFNameAndEx(vol, path);
	writeAttr(vol, path);
	writeEntryInDex(vol, indexEntry);
	fseek(vol, 6, SEEK_CUR); //write pass
	writeClusterStart(vol, numClusterNeeded);
	writeSizeOfImportFile(vol, size);

	return 1;
}