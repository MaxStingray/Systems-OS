#include <console.h>
#include <keyboard.h>

void User_ConsoleWriteCharacter(unsigned char c)
{
	asm volatile("movl $1, %%eax\n\t"
				 "movzx %0, %%ebx\n\t"
				 "int $0x80\n"
				 : : "b"(c)
				);
}

void User_ConsoleWriteString(char* str)
{
	asm volatile("xor %%eax, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "int $0x80\n"
				 : : "b"(str)
				);
}

void User_ConsoleWriteInt(unsigned int i, unsigned int base)
{
	asm volatile("movl $2, %%eax\n\t"
				 "movl %0, %%ecx\n\t"
				 "movl %1, %%ebx\n\t"
				 "int $0x80\n"
				 : : "c"(base), "b"(i)
				);
}

void User_ConsoleClearScreen(const uint8_t c)
{
	asm volatile("movl $3, %%eax\n\t"
				 "movzx %0, %%ebx\n\t"
				 "int $0x80\n"
				 : : "b"(c)
				);
}

void User_ConsoleGotoXY(unsigned int x, unsigned int y) 
{
	asm volatile("movl $4, %%eax\n\t"
				 "movl %0, %%ecx\n\t"
				 "movl %1, %%ebx\n\t"
				 "int $0x80\n"
				 : : "c"(y), "b"(x)
				);
}

void User_ConsoleClearScreenPix(byte c)
{
	asm volatile("movl $5, %%eax\n\t"
				 "movzx %0, %%ebx\n\t"
				 "int $0x81\n"
				 : : "b"(c)
				);
}

void User_SetPixel(int x, int y, byte colour)
{
	asm volatile("movl $6, %%eax\n\t"
				 "movzx %0, %%edx\n\t"
				 "movl %1, %%ecx\n\t"
				 "movl %2, %%ebx\n\t"
				 "int $0x81\n"
				 : : "d"(colour), "c"(y), "b"(x)
				 );
}

void User_BresenhamLine(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY, byte colour)
{
	asm volatile("movl $7, %%eax\n\t"
				 "movzx %0, %%edi\n\t"
				 "movl %1, %%esi\n\t"
				 "movl %2, %%edx\n\t"
				 "movl %3, %%ecx\n\t"
				 "movl %4, %%ebx\n\t"
				 "int $0x81\n"
				 : : "g"(colour), "X"(endY), "d"(endX), "c"(startY), "b"(startX)
				 );
}

void User_DrawRect(int width, int height, int startX, int startY, byte colour)
{
	asm volatile("movl $8, %%eax\n\t"
				 "movzx %0, %%edi\n\t"
				 "movl %1, %%esi\n\t"
				 "movl %2, %%edx\n\t"
				 "movl %3, %%ecx\n\t"
				 "movl %4, %%ebx\n\t"
				 "int $0x81\n"
				 : : "g"(colour), "X"(startY), "d"(startX), "c"(height), "b"(width)
				 );
}

void User_FillRect(int width, int height, int startX, int startY, byte colour)
{
	asm volatile("movl $9, %%eax\n\t"
				 "movzx %0, %%edi\n\t"
				 "movl %1, %%esi\n\t"
				 "movl %2, %%edx\n\t"
				 "movl %3, %%ecx\n\t"
				 "movl %4, %%ebx\n\t"
				 "int $0x81\n"
				 : : "g"(colour), "X"(startY), "d"(startX), "c"(height), "b"(width)
				 );
}

void User_DrawPolygon(int numPoints, int points[], byte colour)
{
	asm volatile("movl $10, %%eax\n\t"
				 "movzx %0, %%edx\n\t"
				 "movl %1, %%ecx\n\t"
				 "movl %2, %%ebx\n\t"
				 "int $0x81\n"
				 : : "d"(colour), "c"(points), "b"(numPoints)
				 );
}

void User_FillPolygon(int numPoints, int points[], byte colour)
{
	asm volatile("movl $11, %%eax\n\t"
				 "movzx %0, %%edx\n\t"
				 "movl %1, %%ecx\n\t"
				 "movl %2, %%ebx\n\t"
				 "int $0x81\n"
				 : : "d"(colour), "c"(points), "b"(numPoints)
				 );
}

void User_BresenhamDot(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY, byte colour)
{
	asm volatile("movl $12, %%eax\n\t"
				 "movzx %0, %%edi\n\t"
				 "movl %1, %%esi\n\t"
				 "movl %2, %%edx\n\t"
				 "movl %3, %%ecx\n\t"
				 "movl %4, %%ebx\n\t"
				 "int $0x81\n"
				 : : "g"(colour), "X"(endY), "d"(endX), "c"(startY), "b"(startX)
				 );
}

void User_DrawCircle(int x, int y, int radius, byte colour)
{
	asm volatile("movl $13, %%eax\n\t"
				 "movzx %0, %%edi\n\t"
				 "movl %1, %%esi\n\t"
				 "movl %2, %%edx\n\t"
				 "movl %3, %%ecx\n\t"
				 "int $0x81\n"
				 : : "g"(colour), "X"(radius), "d"(y), "c"(x)
				 );
}

void User_FillCircle(int x, int y, int radius, byte colour)
{
	asm volatile("movl $14, %%eax\n\t"
				 "movzx %0, %%edi\n\t"
				 "movl %1, %%esi\n\t"
				 "movl %2, %%edx\n\t"
				 "movl %3, %%ecx\n\t"
				 "int $0x81\n"
				 : : "g"(colour), "X"(radius), "d"(y), "c"(x)
				 );
}
