#ifndef CSTR_INCLUDED
#define CSTR_INCLUDED

#include <string.h>
#include <stdint.h>

/**
  * Returns 'true' if 2 strings are equal.
  * If one or both are null, they are never equal.
  */
inline bool strEqual(const char* a, const char* b) {
  return a && b && strcmp(a, b) == 0;
}


/**
  *	The CStr class is a "c string": a simple array of 
  * char and an int size bound in a class. It allocates
  * no memory, and is very efficient.
  */
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

	bool operator==( const char* str ) const						{ return strEqual(buf, str); }
	bool operator!=( const char* str ) const						{ return !strEqual(buf, str); }
	char operator[]( int i ) const									{ return buf[i]; }
	template < class T > bool operator==( const T& str ) const		{ return strEqual(buf, str.buf); }

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

// --- Hex / Dec Utility --- //
/// Convert a char ('0'-'9', 'a'-'f', 'A'-'F') to the integer value.
int hexToDec(char h);
/// Convert an integer from 0-15 to the hex character. '0' - 'f'
char decToHex(int v);

/** 
 * Convert a string in the form:
 *   aabbcc or
 *   abc
 * To the decimal equivalents.
 */
void parseNHex(const char* str, uint8_t* c, int n);

/** Write a string in the form aabbcc from decimal values.
 *  'str' needs to be long enough for the output and the null terminator.
 */
void writeNHex(char* str, const uint8_t* c, int n);

// --- Range / Min / Max --- //
template<class T>
bool inRange(T a, T b, T c) { return a >= b && a <= c; }

#endif // CSTR_INCLUDED
