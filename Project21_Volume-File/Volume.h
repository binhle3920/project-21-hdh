#pragma once

#include <iostream>
#include <fstream>
#include "String.h"
using namespace std;

#define BYTE_PER_SECTOR 512
#define BIT 8


class Volume
{
public:
	//Constructor and destructor
	Volume();
	~Volume();
	//End of constructor and destructor

	bool createNewVol(const char *path, int size);

private:
	char* volName;
	string volExtension;
	int volSize;
};

