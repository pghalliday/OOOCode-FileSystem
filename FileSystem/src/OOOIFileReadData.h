#ifndef OOOIFileReadData_H
#define OOOIFileReadData_H

#include "OOOCode.h"
#include "OOOIError.h"
#include "OOOPath.h"

#define OOOInterface OOOIFileReadData
OOOVirtuals
	OOOVirtual(char *, getPath)
	OOOVirtual(void, read, OOOIError * iError, unsigned char * pData, size_t uSize)
OOOVirtualsEnd
#undef OOOInterface

#endif
