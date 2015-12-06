#ifndef CSTR_INCLUDED
#define CSTR_INCLUDED

#include <string.h>
#include <stdint.h>

template< int ALLOCATE >
class CStr
{
public:
	CStr()							{	clear(); }
	CStr( const char* src )			{	
		clear();
		append(src);
	}

	CStr( const CStr<ALLOCATE>& other ) {										
		memcpy( buf, other.buf, ALLOCATE );
		len = other.len;
	}

	~CStr()	{}

	const char* c_str()	const			{ return buf; }

	int size() const					{ return len; }
	bool empty() const					{ return buf[0] == 0; }

	int length() const 					{ return len; }
	int capacity() const				{ return ALLOCATE-1; }
	void clear()						{ buf[0] = 0; len = 0; }

	bool operator==( const char* str ) const						{ return str && strcmp( buf, str ) == 0; }
	bool operator!=( const char* str ) const						{ return !(*this == str); }
	char operator[]( int i ) const									{ return buf[i]; }
	template < class T > bool operator==( const T& str ) const		{ return !str.empty() && strcmp( buf, str.buf ) == 0; }

	void operator=( const char* src )	{
		clear();
		append(src);
	}
	
	void operator+=( const char* src ) {
		append(src);
	}

	void append(const char* src) {
		for(const char* q = src; q && *q; ++q) {
			append(*q);
		}
	}

	void append(char c) {
		if (len < ALLOCATE-1) {
			buf[len] = c;
			++len;
			buf[len] = 0;
		}
	}

private:
	int len;
	char buf[ALLOCATE];
};

inline bool strEqual(const char* a, const char* b) {
  return a && b && strcmp(a, b) == 0;
}

// --- Hex / Dec Utility --- //
int hexToDec(char h);
char decToHex(int v);
void parseNHex(const char* str, uint8_t* c, int n);
void writeNHex(char* str, const uint8_t* c, int n);

// --- Range / Min / Max --- //
template<class T>
bool inRange(T a, T b, T c) { return a >= b && a <= c; }

#endif // CSTR_INCLUDED
