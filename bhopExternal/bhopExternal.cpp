#include <Windows.h>
#include "MemMan.h"
#include <iostream>

//Accessing MemMan class from MemMan.h.
MemMan MemClass;

//Offsets.
struct offsets
{
	DWORD m_fFlags = 0x100;
	DWORD dwForceJump = 0x50E13BC;
	DWORD dwLocalPlayer = 0xC618AC;
}	offset;

//A few values we need to find.
struct variables
{
	DWORD localPlayer;
	BYTE flag;
}	val;

int main()
{
	//Getting the csgo process and module.
	DWORD proc = MemClass.getProcess("csgo.exe");
	int gameModule = MemClass.getModule(proc, "client_panorama.dll");
	
	//Getting localPlayer value
	val.localPlayer = MemClass.readMem<DWORD>(gameModule + offset.dwLocalPlayer);

	//Will continue to loop until the value is found if localPlayer comes back null.
	if (val.localPlayer == NULL)
		while (val.localPlayer == NULL)
			val.localPlayer = MemClass.readMem<DWORD>(gameModule + offset.dwLocalPlayer);

	//Main loop
	while (true)
	{
		//Getting player flags, so we can determine if we are on the ground.
		val.flag = MemClass.readMem<BYTE>(val.localPlayer + offset.m_fFlags);
		
		//If space is pressed and we are on the ground, write to memory to force jump. Value 6 is jump.
		if (GetAsyncKeyState(VK_SPACE) && val.flag & (1 << 0))
			MemClass.writeMem<DWORD>(gameModule + offset.dwForceJump, 6);

		//So the process does not use too much memory. Always use for external.
		Sleep(1);
	}
	
	return 0;
}

