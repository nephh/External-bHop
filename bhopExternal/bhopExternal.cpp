#include <Windows.h>
#include "MemMan.h"
#include <iostream>

MemMan MemClass;

struct offsets
{
	DWORD m_fFlags = 0x100;
	DWORD dwForceJump = 0x50E13BC;
	DWORD dwLocalPlayer = 0xC618AC;
	DWORD m_vecVelocity = 0x110;
}	offset;

struct variables
{
	DWORD localPlayer;
	BYTE flag;
}	val;

struct vector
{
	float x, y, z;
};

bool isPlayerMoving()
{
	vector playerVel = MemClass.readMem<vector>(val.localPlayer + offset.m_vecVelocity);
	int vel = playerVel.x + playerVel.y + playerVel.z;
	if (vel != 0)
		return true;
	else
		return false;
}

int main()
{
	DWORD proc = MemClass.getProcess("csgo.exe");
	int gameModule = MemClass.getModule(proc, "client_panorama.dll");
	
	val.localPlayer = MemClass.readMem<DWORD>(gameModule + offset.dwLocalPlayer);

	if (val.localPlayer == NULL)
		while (val.localPlayer == NULL)
			val.localPlayer = MemClass.readMem<DWORD>(gameModule + offset.dwLocalPlayer);

	while (true)
	{
		val.flag = MemClass.readMem<BYTE>(val.localPlayer + offset.m_fFlags);
		
		if (isPlayerMoving())
			if (GetAsyncKeyState(VK_SPACE) && val.flag & (1 << 0))
				MemClass.writeMem<DWORD>(gameModule + offset.dwForceJump, 6);

		Sleep(1);
	}
	
	return 0;
}

