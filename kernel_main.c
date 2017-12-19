#include <string.h>
#include <console.h>
#include <hal.h>
#include <exception.h>
#include "physicalmemorymanager.h"
#include "virtualmemorymanager.h"
#include "bootinfo.h"
#include <keyboard.h>
#include <sysapi.h>
#include <user.h>
#include <vgamodes.h>

// This is a dummy __main.  For some reason, gcc puts in a call to 
// __main from main, so we just include a dummy.
 
BootInfo *	_bootInfo;
 
void __main() {}

void InitialisePhysicalMemory()
{
	// Initialise the physical memory manager
	// We place the memory bit map in the next available block of memory after the kernel.
	// Note that all memory addresses here are physical memory addresses since we are dealing
	// with management of physical memory

	uint32_t memoryMapAddress = 0x100000 + _bootInfo->KernelSize;
	if (memoryMapAddress % PMM_GetBlockSize() != 0)
	{
		// Make sure that the memory map is going to be aligned on a block boundary
		memoryMapAddress = (memoryMapAddress / PMM_GetBlockSize() + 1) * PMM_GetBlockSize();
	}
	uint32_t sizeOfMemoryMap = PMM_Initialise(_bootInfo, memoryMapAddress);

	// We now need to mark various regions as unavailable
	
	// Mark first page as unavailable (so that a null pointer is invalid)
	PMM_MarkRegionAsUnavailable(0x00, PMM_GetBlockSize());

	// Mark memory used by kernel as unavailable
	PMM_MarkRegionAsUnavailable(0x100000, _bootInfo->KernelSize);

	// Mark memory used by memory map as unavailable
	PMM_MarkRegionAsUnavailable(PMM_GetMemoryMap(), sizeOfMemoryMap);

	// Reserve two blocks for the stack and make unavailable (the stack is set at 0x90000 in boot loader)
	uint32_t stackSize = PMM_GetBlockSize() * 2;
	PMM_MarkRegionAsUnavailable(_bootInfo->StackTop - stackSize, stackSize);
	
	// Reserve two block for the kernel stack and make unavailable
	PMM_MarkRegionAsUnavailable(0x80000 - stackSize, stackSize);
}
void Initialise()
{
	ConsoleClearScreen(0x1F);
	ConsoleWriteString("UODOS 32-bit Kernel. Kernel size is ");
	ConsoleWriteInt(_bootInfo->KernelSize, 10);
	ConsoleWriteString(" bytes\n");
	HAL_Initialise();
	InitialisePhysicalMemory();
	VMM_Initialise();
	KeyboardInstall(33); 
	InitialiseSysCalls();
	InitialiseVGA();
}

void main(BootInfo * bootInfo) 
{
	_bootInfo = bootInfo;
	Initialise();

	// Uncomment out the following line when you have code that is capable of being running
	// in ring 3
	HAL_EnterUserMode();
	
	// Uncomment out the following line to switch into VGA mode 13h
	// Note that the screen will be full of rubbish until you clear the screen
	// since it will just display what is in memory starting at 0xA0000.
	VGA_SetGraphicsMode(320, 200, 1);
	User_ConsoleClearScreenPix(0);
	User_BresenhamLine(160, 0, 160, 200, 4);
	User_BresenhamDot(0, 100, 320, 100, 4);
	User_DrawRect(75, 50, 10, 140, 7);
	User_FillRect(30, 75, 105, 120, 8);
	int points[] = {180, 120, 250, 170, 180, 170};	
	User_DrawPolygon(3, points, 53);
	int points2[] = {80, 30, 130, 70, 80, 70};
	User_FillPolygon(3, points2, 2);
	User_DrawCircle(250, 70, 20, 42);
	User_FillCircle(290, 50, 20, 4);
	
}
