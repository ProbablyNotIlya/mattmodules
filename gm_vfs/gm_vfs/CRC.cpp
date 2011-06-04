#include "CRC.h"

namespace CRC32 {

unsigned int CRC32Table(unsigned int nPolynomial, 
	unsigned int CRCTable[256])
{
	register unsigned int CRC = 0;
	register unsigned int i = 0;
	register unsigned int j = 0;

	for(i = 0; i < 0xFF; i++)
	{
		CRC = i;
		for(j = 8; j; j--)
		{
			CRC = ( CRC & 1 ) ? ((CRC >> 1) ^ nPolynomial) : (CRC >> 1);
		}
		CRCTable[i] = CRC;
	}

	return nPolynomial;
}

unsigned int CRC32Hash(unsigned int nSeed, 
	const unsigned char* pData, 
	unsigned int cubData, 
	unsigned int CRCTable[256])
{
	register unsigned int CRC = ~nSeed;
	register unsigned int i = 0;

	for(i = 0; i < cubData; i++)
	{
		CRC = CRCTable[(CRC ^ pData[i]) & 0xFF] ^ (CRC >> 8);
	}

	return ~CRC;
}

void CRC32Init(void)
{
	CRC32Table(s_DefaultCRCPoly, s_DefaultCRCTable);
}

}