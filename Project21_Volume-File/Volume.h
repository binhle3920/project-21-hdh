#pragma once

#include <iostream>
#include <windows.h>
#include <fstream>
#include "String.h"
#include "Convert.h"
#include <functional>
#include <conio.h>

using namespace std;



#define BYTE_PER_SECTOR 512
#define SECTOR_PER_CLUSTER 2

#define NUM_SECTOR_POS 19
#define NUM_SECTOR_BYTE 4

#define NUM_SECTOR_OF_BOOT_POS 14
#define SECTOR_OF_BOOT_BYTE 2

#define NUM_SECTOR_PER_CLUSTER_POS 13
#define SECTOR_PER_CLUSTER_BYTE 1

#define NUM_BYTE_PER_SECTOR_POS 11
#define BYTE_PER_SECTOR_BYTE 2

#define SIZE_OF_VOL_POS 23
#define SIZE_OF_VOL_BYTE 2

#define NAME_OF_VOL_POS 36
#define NAME_OF_VOL_BYTE 8

#define EX_OF_VOL_POS 44
#define EX_OF_VOL_BYTE 4

#define BYTE_PER_ENTRY 64

FILE* createNewVol(const char* path, int size);
FILE* readVol(const char* path);

bool importFileToVol(FILE* vol, const char* path, unsigned long pass);
void printListFile(FILE* vol);
bool exportFile(FILE* vol, const char* path, const char* name);
bool deleteFile(FILE* vol, const char* name);

int32_t readValueOfVol(FILE* f, int numByteRead, int posRead);

unsigned long stringHashing(string s);
string hidePass();