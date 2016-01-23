#include "Grinliz_Arduino_Util.h"


int hexToDec(char h) {
  if (h >= '0' && h <= '9') return h - '0';
  if (h >= 'a' && h <= 'f') return 10 + h - 'a';
  if (h >= 'A' && h <= 'F') return 10 + h - 'A';
  return 0;
}

char decToHex(int v) {
	if (v >= 0 && v < 10) return '0' + v;
	if (v>= 10 && v < 16) return 'a' + v - 10;
	return '0';
}


void parseNHex(const char* str, uint8_t* c, int n) {
  if (str[n] == 0 || str[n] == ' ' || str[n] == '\t') {
    // short form
    for (int i = 0; i < n; ++i) {
      int h = hexToDec(*str); 
      c[i] =  h * 16 + h;
      ++str;
    }
  }
  else {
    for (int i = 0; i < n; ++i) {
      c[i] = hexToDec(*str) * 16;
      ++str;
      c[i] += hexToDec(*str);
      ++str;
    }
  }
}


void writeNHex(char* str, const uint8_t* c, int n) {
	for(int i=0; i<n; ++i) {
		str[i*2+0] = decToHex(c[i] / 16);
		str[i*2+1] = decToHex(c[i] & 15);
	}
}