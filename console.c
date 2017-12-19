//	Basic Console Output.

#include <stdint.h>
#include <string.h>
#include <console.h>

// Video memory
uint16_t *_videoMemory = (uint16_t *)0xB8000;

#define CONSOLE_HEIGHT		25
#define CONSOLE_WIDTH		80

typedef unsigned char byte;


byte *VGA = (byte *)0xA0000;

// Current cursor position
uint8_t _cursorX = 0;
uint8_t _cursorY = 0;

// Current color
uint8_t	_colour = 15;

struct Vector2{
	int x;
	int y;
};

// Write byte to device through port mapped io
void  OutputByteToVideoController(unsigned short portid, unsigned char value) 
{
	asm volatile("movb	%1, %%al \n\t"
				 "movw	%0, %%dx \n\t"
				  "out	%%al, %%dx"
				 : : "m" (portid), "m" (value));				 
}

// Update hardware cursor position

void UpdateCursorPosition(int x, int y) 
{
    uint16_t cursorLocation = y * 80 + x;

	// Send location to VGA controller to set cursor
	// Send the high byte
	OutputByteToVideoController(0x3D4, 14);
	OutputByteToVideoController(0x3D5, cursorLocation >> 8); 
	// Send the low byte
	OutputByteToVideoController(0x3D4, 15);
	OutputByteToVideoController(0x3D5, cursorLocation);      
}

// Displays a character
void ConsoleWriteCharacter(unsigned char c) 
{
    uint16_t attribute = _colour << 8;
	//explicitly handle new line characters
	if(c == '\n')
	{
		_cursorX = 0;
		_cursorY++;
	}
	//assume printable characters
    int16_t* location = _videoMemory + (_cursorY * CONSOLE_WIDTH + _cursorX);
    *location = c | attribute;
	ConsoleGotoXY(_cursorX, _cursorY);
	_cursorX++;
}

// Display specified string

void ConsoleWriteString(char* str) 
{
	if (!str)
	{
		return;
	}
	while (*str)
	{
		ConsoleWriteCharacter(*str++);
	}
}

void ConsoleWriteInt(unsigned int value, unsigned int base){	

	static char buf[32] = {0};
	bool isNegative;
	int i = 30;
	
	if(value < 0 && base == 10)
	{
		isNegative = true;
		value = -value;
	}
		
	for (; value && i ; --i, value /= base)
		buf[i] = "0123456789abcdef"[value % base];
	//handle negatives someplace
	ConsoleWriteString(&buf[i+1]);
}

unsigned int ConsoleSetColour(const uint8_t c){
	
	
}

void ConsoleGotoXY(unsigned int x, unsigned int y){

	UpdateCursorPosition(x, y);
	
}

void ConsoleGetXY(unsigned* x, unsigned* y){
		
	uint16_t cursorLocation = *y * 80 + *x;
	
	return cursorLocation;
}

int ConsoleGetWidth(){
	return CONSOLE_WIDTH;
}

int ConsoleGetHeight(){
	return CONSOLE_HEIGHT;
}

void ConsoleClearScreen(const uint8_t c){
	
	int attribute;
    unsigned clear;
    int i; 
   
    clear = c |(attribute << 8);
   
    for (i = 0; i < CONSOLE_HEIGHT; i++){
        memsetw(_videoMemory + i * CONSOLE_WIDTH, clear, CONSOLE_WIDTH);
    }
       
    _cursorX = 0;
    _cursorY = 0;
    UpdateCursorPosition(0, 0);
	
}

void ConsoleClearScreenPix(byte c){
//loop through each pixel on the screen and set colour
	for (int i = 0; i < 320; i++){
		for (int j = 0; j < 200; j++){
			SetPixel(i, j, c);
	}
	}
}

void SetPixel(int x, int y, byte colour){
	//pass a byte containing colour to this point on the screen
	VGA[320 * y + x] = colour;
}

byte GetPixel(int x, int y){
	//return the colour of the pixel at this location
	return VGA[320 * y + x];
}

int abs(int v){
	const int ret[2] = {v, -v};
	return ret [v<0];
}

int round(float v){
	//handy function to round a number
	int n = (int)(v < 0 ? (v - 0.5) : (v + 0.5));
	return n;
}
//reference this
void BresenhamLine(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY, byte colour){
	
	//define the major and minor axis (major is longer than minor)
	int dx = abs(endX - startX);
	int sx;
	if(startX<endX)
		sx = 1;
	else
		sx = -1;	
	
	int dy = abs(endY - startY);
	int sy;
	if(startY < endY)
		sy = 1;
	else
		sy = -1;
	
	//initialise the error term (decision parameter)
	int error;
	if(dx > dy)
		error = dx / 2;
	else
		error = -dy / 2;
	
	int error2;
	
	for(;;)
	{
		//start plotting pixels
		SetPixel(startX, startY, colour);
		
		//if the start and end match, we have reached the end of the line
		if(startX == endX && startY == endY)
			break;
		
		//otherwise continue checking the error term against the axis to find the next pixel
		error2 = error;
		if(error2 > - dx)
		{
			error -= dy;
			startX += sx;
		}
		if(error2 < dy)
		{
			error += dx; 
			startY += sy;
		}
		
	}
}

/*this is not an ideal solution. If I could pass more parameters we could define dotted or solid through a boolean*/ 
//adapted and simplified from code found at: http://41j.com
void BresenhamDot(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY, byte colour){
	//define the major and minor axis (major is longer than minor)
	int dx = abs(endX - startX);
	int sx;
	if(startX<endX)
		sx = 1;
	else
		sx = -1;	
	
	int dy = abs(endY - startY);
	int sy;
	if(startY < endY)
		sy = 1;
	else
		sy = -1;
	
	//initialise the error term
	int error;
	if(dx > dy)
		error = dx / 2;
	else
		error = -dy / 2;
	
	int error2;
	int pixelIndex = 0;
	for(;;)
	{
		if(pixelIndex%2 == 0)//check pixel index to see if it is even
			SetPixel(startX, startY, colour);//if it is, colour it with the new colour
		else
			SetPixel(startX, startY, 0);//otherwise, colour it black

		//if the start and end match, we have reached the end of the line
		if(startX == endX && startY == endY)
			break;
		
		//otherwise continue checking the error term against the axis to find the next pixel
		error2 = error;
		if(error2 > - dx)
		{
			error -= dy;
			startX += sx;
		}
		if(error2 < dy)
		{
			error += dx; 
			startY += sy;
		}
		pixelIndex++;
	}
}
//TODO: Replace line calls with a for loop to save memory
void DrawRect(int width, int height, int startX, int startY, byte colour){
	//calculate the bottom right corner of the rectangle
	unsigned int endX = startX + width;
	unsigned int endY = startY + height;
	
	//draw lines between each point
	//use loops and SetPixel to save memory
	
	//top line
	for(int x = startX; x < (startX + width) + 1; x++)
	{
		if(x != endX)
			SetPixel(x, startY, colour);
		//right line
		else
			for(int y = (endY - height); y < endY + 1; y++)
			{
				SetPixel(endX, y, colour);
			}
	}
	//bottom line in the other direction
	for(int x2 = (startX + width); x2 > startX - 1; x2--)
	{
		if(x2 != startX)
			SetPixel(x2, endY, colour);
		//left line
		else
			for(int y2 = (startY + height); y2 > startY - 1; y2--)
			{
				SetPixel(startX, y2, colour);
			}
		
	}
}

void FillRect(int width, int height, int startX, int startY, byte colour){
	//draw the rectangle to fill
	DrawRect(width, height, startX, startY, colour);
	
	//loop through each "row" and "column" of the rectangle and draw pixels
	for (int i = startX; i < startX + width; i++){
		for (int j = startY; j < startY + height; j++){
			SetPixel(i, j, colour);
		}
	}
	
}

void DrawPolygon(int numPoints, int points[], byte colour){
	
	//if the given collection of points is even
	if((numPoints * 2)%2 == 0)
	{
		//loop through each x and y co-ordinate
		for(int i = 0; i < numPoints * 2;)
		{
			//check we're not out of array bounds
			if(i + 3 <= numPoints * 2)
			{
				//draw line between this set of points and the next
				BresenhamLine(points[i], points[i+1], points[i+2], points[i+3], colour);
				i += 2;//increment the index to the next pair of points
			}
			else//otherwise we can assume we've reached the end
				break;
		}
		//draw final line to close the polygon
		BresenhamLine(points[0], points[1], points[(numPoints * 2) - 2], points[(numPoints * 2) - 1], colour);
	}
	else//otherwise do nothing
		return;
	
}

/*based on code found at: https://www.thecrazyprogrammer.com/2017/02/flood-fill-algorithm-in-c.html */
void FloodFill(int x, int y, byte oldColour, byte newColour){
	//if the start point has the colour to be replaced
	if(GetPixel(x,y) == oldColour)
	{
		//check each neighboring pixel and call recursively
		SetPixel(x,y,newColour);
		FloodFill(x+1, y, oldColour, newColour);
		FloodFill(x, y+1, oldColour, newColour);
		FloodFill(x-1, y, oldColour, newColour);
		FloodFill(x, y -1, oldColour, newColour);
	}
}
//TODO: define filled or unfilled with a boolean instead of duplicate code
void FillPolygon(int numPoints, int points[], byte colour){
	//loop through each x and y co-ordinate
	for(int i = 0; i < numPoints * 2;)
	{
		//check we're not out of array bounds
		if(i + 3 <= numPoints * 2)
		{
			//draw line between this set of points and the next
			BresenhamLine(points[i], points[i+1], points[i+2], points[i+3], colour);
			i += 2;//increment the index to the next set of points
		}
		else//otherwise we can assume we've reached the end
			break;
	}
	//draw final line to close the polygon
	BresenhamLine(points[0], points[1], points[(numPoints * 2) - 2], points[(numPoints * 2) - 1], colour);
	
	//find the logical centre of all points
	int totalX = 0;
	int totalY = 0;
	for (int j = 0; j < numPoints * 2;)
	{
		totalX += points[j];
		totalY += points[j+1];
		j+=2;
	}
	
	int centreX = (totalX / numPoints);
	int centreY = (totalY / numPoints);
	//flood fill from the middle out
	FloodFill(centreX, centreY, 0, colour);
}


//adapted from code found at http://rosettacode.org/wiki/Bitmap/Midpoint_circle_algorithm
void DrawCircle(int x, int y, int radius, byte colour){
	//set the intial point on the axes
	int x1 = radius -1;
	int y1 = 0;
	
	int dx = 1;
	int dy = 1;
	
	//initialise decision parameter
	int decisionOver2 = -2 * radius;
	
	while (x1 >= y1)
	{
		//draw the next pixel of each octant "mirrored" from the first
		SetPixel(x1 + x, y1 + y, colour);//first octant
		SetPixel(y1 + x, x1 + y, colour);//second octant
		SetPixel(-x1 + x, y1 + y, colour);//third octant
		SetPixel(-y1 + x, x1 + y, colour);//fourth octant
		SetPixel(-x1 + x, -y1 + y, colour);//fifth octant
		SetPixel(-y1 + x, -x1 + y, colour);//sixth octant
		SetPixel(x1 + x, -y1 + y, colour);//seventh octant
		SetPixel(y1 + x, -x1 + y, colour);//eighth octant
		
		if(decisionOver2 <= 0)//the point is on or within the perimeter
		{
			y1++;
			decisionOver2 += dy;
			dy += 2;
		}
		if(decisionOver2 > 0)//the point is outside the circle
		{
			x1--;
			dx += 2;
			decisionOver2 += -2 * radius + dx;
		}
	}
}

//call DrawCircle then fill it
void FillCircle(int x, int y, int radius, byte colour){
	DrawCircle(x, y, radius, colour);
	FloodFill(x,y,0,colour);
}




