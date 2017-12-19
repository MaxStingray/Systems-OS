#ifndef _USER_H
#define _USER_H
#include <keyboard.h>

void User_ConsoleWriteCharacter(unsigned char c); 
void User_ConsoleWriteString(char* str); 
void User_ConsoleWriteInt(unsigned int i, unsigned int base); 
void User_ConsoleClearScreen(const uint8_t c); 
void User_ConsoleGotoXY(unsigned int x, unsigned int y); 
void User_ConsoleClearScreenPix(byte c);
void User_SetPixel(int x, int y, byte colour);
void User_BresenhamLine(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY, byte colour);
void User_DrawRect(int width, int height, int startX, int startY, byte colour);
void User_FillRect(int width, int height, int startX, int startY, byte colour);
void User_FillPolygon(int numPoints, int points[], byte colour);
void User_BresenhamDot(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY, byte colour);
void User_DrawCircle(int x, int y, int radius, byte colour);
void User_FillCircle(int x, int y, int radius, byte colour);
#endif