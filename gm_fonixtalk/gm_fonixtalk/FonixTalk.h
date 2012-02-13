#pragma once

struct FonixHandle_t
{
	unsigned int Index[30];
};

struct FonixParams_t
{
	unsigned short Dummy0;		// 1, unknown
	unsigned short Echo;		// 6, seems like echo
	unsigned short Dummy1;		// 100, unknown
	unsigned short Pitch;		// 50 - 350
	unsigned short Dummy3;		// 10, unknown
};