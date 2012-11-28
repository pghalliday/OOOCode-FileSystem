#ifndef OOOIDirectoryCreateData_H
#define OOOIDirectoryCreateData_H

#include "OOOCode.h"
#include "OOOIError.h"
#include "OOOPath.h"

#define OOOInterface OOOIDirectoryCreateData
OOOVirtuals
	OOOVirtual(char *, getPath)
	OOOVirtual(void, created, OOOIError * iError)
OOOVirtualsEnd
#undef OOOInterface

#endif
