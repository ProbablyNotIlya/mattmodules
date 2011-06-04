#if !defined(VFS_CRC_H)
#define VFS_CRC_H

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/crc.hpp>

namespace CRC32
{

static unsigned int s_DefaultCRCTable[256];
static unsigned int s_DefaultCRCPoly = 0xEDB88320;

unsigned int CRC32Table(unsigned int nPolynomial, 
	unsigned int CRCTable[256]);

unsigned int CRC32Hash(unsigned int nSeed, 
	const unsigned char* pData, 
	unsigned int cubData, 
	unsigned int CRCTable[256] = s_DefaultCRCTable);

void CRC32Init(void);

}

#endif // VFS_CRC_H