#ifndef OOOIFileWriteData_H
#define OOOIFileWriteData_H

#include "OOOCode.h"
#include "OOOIError.h"
#include "OOOPath.h"

#define OOOInterface OOOIFileWriteData
OOOVirtuals
	OOOVirtual(char *, getPath)
	OOOVirtual(unsigned char *, getData)
	OOOVirtual(size_t, getSize)
	OOOVirtual(void, written, OOOIError * iError)
OOOVirtualsEnd
#undef OOOInterface

#endif
