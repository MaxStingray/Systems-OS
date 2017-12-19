#ifndef _CONSOLE_H
#define _CONSOLE_H
#include <stdint.h>

struct Vector2;

typedef unsigned char byte;
// Output the specified character the current cursor position.  
// The attribute at that position remains unchanged.

void ConsoleWriteCharacter(unsigned char c); 

// Write the null-terminated string str to the current cursor position on the screen

void ConsoleWriteString(char* str); 

// Write the unsigned integer to the screen, using the specified base, e.g. for
// the number to be displayed as a decimal number, you would specify base 10.  For 
// a number to be displayed as hexadecimal, you would specify base 16.

void ConsoleWriteInt(unsigned int i, unsigned int base); 

// Set the attribute to be used for all subsequent calls to ConsoleWriteXXXX routines

unsigned int ConsoleSetColour(const uint8_t c); 

// Position the cursor at the specified X and Y position. All subsequent calls to 
// ConsoleWriteXXXX routines will start at this position.

void ConsoleGotoXY(unsigned int x, unsigned int y); 

// Get the current X and Y position of the cursor

void ConsoleGetXY(unsigned* x, unsigned* y);

// Get the width of the console window

int ConsoleGetWidth();

int abs(int v);

// Get the height of the console window
void SetPixel(int x, int y, uint8_t color);

byte GetPixel(int x, int y);

int ConsoleGetHeight(); 

// Clear the screen.  Set the attribute byte for all characters on the 
// screen to be c. Set the cursor position to 0,0.

void ConsoleClearScreen(const uint8_t c); 

void ConsoleClearScreenPix(byte colour);

void BresenhamLine(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY, byte colour);

void BresenhamDot(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY, byte colour);

void DrawRect(int width, int height, int startX, int startY, byte colour);

void FillRect(int width, int height, int startX, int startY, byte colour);

void DrawPolygon(int numPoints, int points[], byte colour);

void FloodFill(int x, int y, byte oldColour, byte newColour);

void FillPolygon(int numPoints, int points[], byte colour);

void DrawCircle(int x, int y, int radius, byte colour);

void FillCircle(int x, int y, int radius, byte colour);

#endif