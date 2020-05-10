#pragma once

#include <iostream>
#include <fstream>
#include "String.h"
#include "Convert.h"
using namespace std;

#define BYTE_PER_SECTOR 512
#define SECTOR_PER_CLUSTER 2
#define BIT 8



bool createNewVol(const char* path, int size);