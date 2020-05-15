#include "Volume.h"

hash<string> pass;

//Some other function
int32_t readValueOfVol(FILE* f, int numByteRead, int posRead) {
	fseek(f, 0, SEEK_SET);
	fseek(f, posRead, SEEK_SET);
	char* buffer = new char[numByteRead];
	fread(buffer, 1, numByteRead, f);

	return charToInt(buffer, numByteRead);
}
int seekToEmptyEntry(FILE* f, const char* path) {
	//Get file name and ex
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
	curPos--; //skip the '.'
	while (curPos >= 0 && path[curPos] != '\\') {
		fileName[fileNamePos++] = path[curPos--];
	}
	_strrev(fileName);
	_strrev(fileExtension);
	//---------------------------------------------------------------------------

	int curByte = -64;
	int countEntry = 1;
	fseek(f, curByte, SEEK_END);
	//Find the entry that is empty
	char checkEntry;
	fread(&checkEntry, 1, 1, f);
	while (checkEntry != '\0') {
		fseek(f, -1, SEEK_CUR); //back to 1 byte
		char* name = new char[27]; //declare file name
		char* ex = new char[4]; //declare file ex
		fread(name, 27, 1, f); //read the entry name
		fread(ex, 4, 1, f); //read the entry ex
		if (strcmp(fileName, name) == 0 && strcmp(fileExtension, ex) == 0) {
			return 0;
		}

		countEntry++;
		fseek(f, curByte * countEntry, SEEK_END);
		fread(&checkEntry, 1, 1, f);
		delete[] name;
		delete[] ex;
	}
	fseek(f, -1, SEEK_CUR); //Move the pointer of file to the first byte of entry
	//The code above found the empty entry

	delete[] fileName;
	delete[] fileExtension;
	return countEntry;
}
long firstFreeCluster(FILE* f) {
	//Num cluster is number cluster need to save a file
	//save the current byte to write "free cluster"
	long curPos = ftell(f);

	int firstFree = 4;

	fseek(f, firstFree * SECTOR_PER_CLUSTER * BYTE_PER_SECTOR, SEEK_SET);
	char checkEntry;
	fread(&checkEntry, 1, 1, f);
	while (checkEntry != '\0') {
		firstFree++;
		fseek(f, firstFree * SECTOR_PER_CLUSTER * BYTE_PER_SECTOR, SEEK_SET);
		fread(&checkEntry, 1, 1, f);
	}

	
	fseek(f, curPos, SEEK_SET);
	return firstFree;

	//Return to the right position

}
unsigned long stringHashing(string s) {
	unsigned long g;
	g = pass(s);
	return g;
}
string hidePass() {
	string initialpass;
	string s;
	char c = 0;
	cout << "Enter pass: ";
	while (c != 13) {
		c = _getch();
		s += c;
		cout << "*";
	}

	for (int i = 0; i < s.length() - 1; i++)
		initialpass += s[i];
	cout << endl;
	return initialpass;
}

//These function used for write volume info
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
//-------------------------------------------------------------------------------------------


//These function used for write file import info
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
	curPos--; //skip the '.'
	while (curPos >= 0 && path[curPos] != '\\') {
		fileName[fileNamePos++] = path[curPos--];
	}
	_strrev(fileName);
	_strrev(fileExtension);
	fwrite(fileName, 1, 27, f);
	fwrite(fileExtension, 1, 4, f);
	fflush(f);
	//the code above file the file name and file extension

	delete[] fileName;
	delete[] fileExtension;
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
void writeClusterStart(FILE* f) {
	long clusterStart = firstFreeCluster(f);
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
void writePassword(FILE* f, unsigned long pass) {
	string* hex = decimalToHex(6, pass);
	char* codeInArray = new char[6];
	for (int i = 0; i < 6; i++) {
		codeInArray[i] = char(hexToDecimal(hex[i]));
	}
	fwrite(codeInArray, 1, 6, f);
	fflush(f);
	delete[] codeInArray;
	delete[] hex;
}
//-------------------------------------------------------------------------------------------

//Create vol or read vol
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
//-------------------------------------------------------------------------------------------


//Vol function
bool importFileToVol(FILE* vol, const char* path, unsigned long pass) {
	FILE* file = fopen(path, "rb");

	if (file == NULL) {
		return 0;
	}

	//Find file size
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	long numClusterNeeded = size / (SECTOR_PER_CLUSTER * BYTE_PER_SECTOR);

	
	if (file == NULL) {
		return 0;
	}

	//Write entry of file
	int indexEntry = seekToEmptyEntry(vol, path);
	if (indexEntry == 0) {
		return 0;
	}

	writeIFNameAndEx(vol, path);
	writeAttr(vol, path);
	writeEntryInDex(vol, indexEntry);
	if (pass == 0) {
		fseek(vol, 6, SEEK_CUR);
	}
	else {
		writePassword(vol, pass);
	}
	writeClusterStart(vol);
	writeSizeOfImportFile(vol, size);

	//Write data of file
	int firstCluster = firstFreeCluster(vol);
	fseek(vol, firstCluster * SECTOR_PER_CLUSTER * BYTE_PER_SECTOR, SEEK_SET);
	char* buffer = new char[1019];
	for (int i = 0; i < 1019; i++) {
		buffer[i] = '\0';
	}
	fseek(file, 0, SEEK_SET);

	int n = fread(buffer, 1, 1019, file);
	fflush(vol);

	do {
		//Find the next free cluster and write it 4 byte to current cluster
		fwrite(".", 1, 1, vol);
		fwrite(buffer, 1, 1019, vol);
		//reset buffer
		for (int i = 0; i < 1019; i++) {
			buffer[i] = '\0';
		}
		int nextCluster = firstFreeCluster(vol);
		string* hexArray = decimalToHex(4, nextCluster);
		char* codeInArray = new char[4];
		for (int i = 0; i < 4; i++) {
			codeInArray[i] = char(hexToDecimal(hexArray[i]));
		}
		fwrite(codeInArray, 1, 4, vol);
		fflush(vol);
		delete[] codeInArray;
		delete[] hexArray;
		//seek to the next cluster
		fseek(vol, nextCluster * BYTE_PER_SECTOR * SECTOR_PER_CLUSTER, SEEK_SET);
		n = fread(buffer, 1, 1019, file);
	} while (n == 1019);
	fwrite(".", 1, 1, vol);
	fwrite(buffer, 1, 1019, vol);
	fclose(file);
	remove(path);
	
	return 1;
}
void printListFile(FILE* vol) {
	int curEntry = 1;
	char* fileName = new char[27];
	char* fileEx = new char[4];

	//move to the 1st file
	fseek(vol, curEntry * (-64), SEEK_END);
	int count = 1;
	char temp;
	fread(&temp, 1, 1, vol);
	while (temp != '\0') {
		fseek(vol, -1, SEEK_CUR);
		fread(fileName, 1, 27, vol);
		fread(fileEx, 1, 4, vol);
		cout << count++ << ". " << fileName << "." << fileEx << endl;
		curEntry++;
		fseek(vol, curEntry * (-64), SEEK_END);
		fread(&temp, 1, 1, vol);
	}


	delete[] fileName;
	delete[] fileEx;
}
bool exportFile(FILE* vol, const char* path, const char* name) {
	FILE* exp;
	exp = fopen(path, "wb");

	if (exp == NULL) {
		return 0;
	}


	//move to the file with that name
	int curEntry = 1;
	char* fileName = new char[27];
	char* fileEx = new char[4];
	char* nameWithEx = new char[31];
	nameWithEx[0] = '\0';

	//move to the 1st file
	fseek(vol, curEntry * (-64), SEEK_END);
	char temp;
	fread(&temp, 1, 1, vol);
	while (temp != '\0') {
		fseek(vol, -1, SEEK_CUR); //back 1 byte
		fread(fileName, 1, 27, vol); //read name
		fread(fileEx, 1, 4, vol); //read extension
		//move back to the first byte of entry
		fseek(vol, -31, SEEK_CUR);
		//------------------
		strcat(nameWithEx, fileName);
		strcat(nameWithEx, ".");
		strcat(nameWithEx, fileEx); 
		if (strcmp(nameWithEx, name) == 0) {
			break;
		}
		nameWithEx[0] = '\0';
		curEntry++;
		fseek(vol, curEntry * (-64), SEEK_END);
		fread(&temp, 1, 1, vol);
	}

	if (temp == '\0') {
		return 0;
	}

	//check password
	char* pass = new char[6];
	fseek(vol, 34, SEEK_CUR);
	fread(pass, 6, 1, vol);
	unsigned long passNumber = charToInt(pass, 6);
	if (passNumber != 0) {
		string s;
		s = hidePass();
		unsigned long checkPassNumber = stringHashing(s);
		if (passNumber != checkPassNumber) {
			return 0;
		}
	}
	fseek(vol, -40, SEEK_CUR);
	

	//currently, vol pointer is at entry of file
	//seek to the byte say where is the first cluster
	fseek(vol, 40, SEEK_CUR);
	//read the first cluster
	char* value = new char[4];
	fread(value, 1, 4, vol);
	int cluster = charToInt(value, 4);
	//seek to that cluster
	fseek(vol, cluster * SECTOR_PER_CLUSTER * BYTE_PER_SECTOR, SEEK_SET);

	//seek to the first byte of file
	fseek(exp, 0, SEEK_SET);
	char* buffer = new char[1019];
	char trash;
	fread(&trash, 1, 1, vol);
	fread(buffer, 1019, 1, vol);
	fread(value, 4, 1, vol);
	fwrite(buffer, 1, 1019, exp);
	cluster = charToInt(value, 4);
	while (cluster != 0) {
		fseek(vol, cluster * SECTOR_PER_CLUSTER * BYTE_PER_SECTOR, SEEK_SET);
		fread(&trash, 1, 1, vol);
		fread(buffer, 1, 1019, vol);
		fread(value, 1, 4, vol);
		fwrite(buffer, 1, 1019, exp);
		cluster = charToInt(value, 4);
	}
	
	delete[] buffer;
	delete[] value;
	delete[] fileName;
	delete[] fileEx;
	delete[] nameWithEx;
	fclose(exp);
	return 1;
}
bool deleteFile(FILE* vol, const char* name) {
	//move to the file with that name
	int curEntry = 1;
	char* fileName = new char[27];
	char* fileEx = new char[4];
	char* nameWithEx = new char[31];
	nameWithEx[0] = '\0';

	//move to the 1st file
	fseek(vol, curEntry * (-64), SEEK_END);
	char temp;
	fread(&temp, 1, 1, vol);
	while (temp != '\0') {
		fseek(vol, -1, SEEK_CUR); //back 1 byte
		fread(fileName, 1, 27, vol); //read name
		fread(fileEx, 1, 4, vol); //read extension
		//move back to the first byte of entry
		fseek(vol, -31, SEEK_CUR);
		//------------------
		strcat(nameWithEx, fileName);
		strcat(nameWithEx, ".");
		strcat(nameWithEx, fileEx);
		if (strcmp(nameWithEx, name) == 0) {
			break;
		}
		nameWithEx[0] = '\0';
		curEntry++;
		fseek(vol, curEntry * (-64), SEEK_END);
		fread(&temp, 1, 1, vol);
	}

	if (temp == '\0') {
		return 0;
	}

	//check password
	char* pass = new char[6];
	fseek(vol, 34, SEEK_CUR);
	fread(pass, 6, 1, vol);
	unsigned long passNumber = charToInt(pass, 6);
	if (passNumber != 0) {
		string s;
		s = hidePass();
		unsigned long checkPassNumber = stringHashing(s);
		if (passNumber != checkPassNumber) {
			return 0;
		}
	}
	fseek(vol, -40, SEEK_CUR);

	//currently, pointer is in first byte of entry 
	fseek(vol, 40, SEEK_CUR);
	char* value = new char[4];
	fread(value, 1, 4, vol);
	int cluster = charToInt(value, 4);

	fseek(vol, cluster * SECTOR_PER_CLUSTER * BYTE_PER_SECTOR, SEEK_SET);

	//seek to the first byte of file
	char* buffer = new char[1020];
	char* tempT = new char[4];
	for (int i = 0; i < 1020; i++) {
		buffer[i] = '\0';
		if (i < 4) {
			tempT[i] = '\0';;
		}
	}

	fwrite(buffer, 1, 1020, vol);
	fread(value, 1, 4, vol);
	fwrite(tempT, 1, 4, vol);
	cluster = charToInt(value, 4);
	while (cluster != 0) {
		fseek(vol, cluster * SECTOR_PER_CLUSTER * BYTE_PER_SECTOR, SEEK_SET);
		fwrite(buffer, 1, 1020, vol);
		fread(value, 1, 4, vol);
		fwrite(tempT, 1, 4, vol);
		cluster = charToInt(value, 4);
	}

	fseek(vol, curEntry * (-64), SEEK_END);
	char* tempB = new char[64];
	for (int i = 0; i < 64; i++) {
		tempB[i] = '\0';
	}
	fwrite(tempB, 1, 64, vol);

	delete[] pass;
	delete[] tempT;
	delete[] buffer;
	delete[] value;
	delete[] fileName;
	delete[] fileEx;
	delete[] nameWithEx;
	return 1;
}
bool renameFile(FILE* vol, const char* name, const char* newName) {
	//move to the file with that name
	int curEntry = 1;
	char* fileName = new char[27];
	char* fileEx = new char[4];
	char* nameWithEx = new char[31];
	nameWithEx[0] = '\0';

	//move to the 1st file
	fseek(vol, curEntry * (-64), SEEK_END);
	char temp;
	fread(&temp, 1, 1, vol);
	while (temp != '\0') {
		fseek(vol, -1, SEEK_CUR); //back 1 byte
		fread(fileName, 1, 27, vol); //read name
		fread(fileEx, 1, 4, vol); //read extension
		//move back to the first byte of entry
		fseek(vol, -31, SEEK_CUR);
		//------------------
		strcat(nameWithEx, fileName);
		strcat(nameWithEx, ".");
		strcat(nameWithEx, fileEx);
		if (strcmp(nameWithEx, name) == 0) {
			break;
		}
		nameWithEx[0] = '\0';
		curEntry++;
		fseek(vol, curEntry * (-64), SEEK_END);
		fread(&temp, 1, 1, vol);
	}

	//not found any entry
	if (temp == '\0') {
		return 0;
	}

	writeIFNameAndEx(vol, newName);

	delete[] fileName;
	delete[] fileEx;
	delete[] nameWithEx;
	return 1;
}