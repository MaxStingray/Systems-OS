// Implementation of string and memory functions

#include <string.h>
//compare two strings and 
int strcmp (const char* str1, const char* str2) {
	const unsigned char *p1 = (const unsigned char *)str1;
	const unsigned char *p2 = (const unsigned char *)str2;
	
	while (*p1 != '\0') {
		if (*p2 == '\0') return 1;
		if (*p2 > *p1) 	 return -1;
		if (*p1 > *p2)	 return 1;
		
		p1++;
		p2++;
	}
	
	if (*p2 != '\0') return -1;
	
	return 0;
}

char * strcpy(char * destination, const char * source) {
	char *orig = destination;
	while (*destination++ = *source++)
		;
	return orig;
}

errno_t strcpy_s(char *destination, size_t numberOfElements, const char *source){
	
}

size_t strlen(const char* source){
	for (size_t len = 0;;++len) if (source[len]==0) return len;
}

void * memcpy(void * destination, const void * source, size_t count){
	 char *sourceChar = (char *)source;
	 char *destChar = (char *)source; //typecast source and dest to char
	 
	 for (int i = 0; i < count; i++)//if i is less than the size_t (integer value) of count (amount of characters to copy)
		 destChar[i] = sourceChar[i];//copy char by char
}
//stores c into the first n characters of the object pointed to by s.
void * memset(void *destination, char val, size_t count){//initialise a memory block
	unsigned char* p = (unsigned char*)destination; //typecast destination as an unsigned char
	int valInt = (int)val;//typecast val as an integer
	unsigned char x = valInt & 0xff;
	
	while (count--)//pretty sure this just decrements count until it hits zero while(n--). Pretty cool.
		*p++ = x;
	return destination;
}

unsigned short * memsetw(unsigned short * destination, unsigned short val, size_t count){//just a wordwise memset instead of bytewise
	unsigned short* p = (unsigned short*)destination; //typecast destination as an unsigned char
	int valInt = (int)val;//typecast val as an integer
	unsigned char x = valInt & 0xff;
	
	while (count--)//pretty sure this just decrements count until it hits zero while(n--). Pretty cool.
		*p++ = x;
	return (unsigned short)destination;
}