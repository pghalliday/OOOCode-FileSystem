#ifndef OOOIDirectoryRemoveData_H
#define OOOIDirectoryRemoveData_H

#include "OOOCode.h"
#include "OOOIError.h"
#include "OOOPath.h"

#define OOOInterface OOOIDirectoryRemoveData
OOOVirtuals
	OOOVirtual(char *, getPath)
	OOOVirtual(void, removed, OOOIError * iError)
OOOVirtualsEnd
#undef OOOInterface

#endif
