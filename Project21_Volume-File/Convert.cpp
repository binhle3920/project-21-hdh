#include "Convert.h"

int hexToDecimal(string hex) {
	int result = 0;
	for (int i = 0; i < hex.length(); i++) {
		if (hex[i] >= '0' && hex[i] <= '9') {
			//0 in ASCII is 48 ==> 0 is 0 so minus 48
			result += (int(hex[i]) - 48) * pow(16, hex.length() - 1 - i);
		}
		else if (hex[i] >= 'A' && hex[i] <= 'F') {
			//A in ASCII is 65 because A is 10 so we minus 55
			result += (int(hex[i]) - 55) * pow(16, hex.length() - 1 - i);
		}
	}
	return result;
}

string* decimalToHex(int numByte, int32_t decimal) {
	char* byteValue = new char[numByte*2];
	int curByte = numByte*2 - 1;

	int32_t byteNum;
	while (decimal > 0) {
		byteNum = decimal % 16;
		if (byteNum >= 0 && byteNum <= 9) {
			//0 in ASCII is 48 ==> 0 is 0 so plus 48
			byteValue[curByte] = char(byteNum + 48);
		}
		else if (byteNum >= 10 && byteNum <= 15) {
			//A in ASCII is 65 because A is 10 so we plus 55
			byteValue[curByte] = char(byteNum + 55);
		}

		curByte--;
		decimal /= 16;
	}
	
	while (curByte >= 0) {
		byteValue[curByte] = '0';
		curByte--;
	}
	//Example: 4 have 4 bytes, decimal value is 19276 will become 00004B4C
	//Next, we will change it to "00" "00" "4B" "4C"
	string* hex = new string[numByte];
	for (int i = 0; i < numByte; i++) {
		hex[i] = "";
	}
	int curHex = 0;
	for (int i = 0; i < numByte*2; i++) {
		hex[curHex] += byteValue[i];
		if (i % 2 != 0 && i>0)
			curHex++;
	}

	return hex;
}

//This function is used for reading value from volume
//Example: we want to know how many sector 
int charToInt(const char* listChar, int numByte) {
	//numByte here is listChar length, first we convert char to hexvalue 
	//ex: string "AB" will be come "1A5F" and then we convert 1A5F to integer
	string hex = "";
	for (int i = 0; i < numByte; i++) {
		string* temp = decimalToHex(1, int(unsigned char(listChar[i])));
		hex += temp[0];
	}
	return hexToDecimal(hex);
}