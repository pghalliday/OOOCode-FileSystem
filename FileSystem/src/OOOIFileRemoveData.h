#ifndef OOOIFileRemoveData_H
#define OOOIFileRemoveData_H

#include "OOOCode.h"
#include "OOOIError.h"
#include "OOOPath.h"

#define OOOInterface OOOIFileRemoveData
OOOVirtuals
	OOOVirtual(char *, getPath)
	OOOVirtual(void, removed, OOOIError * iError)
OOOVirtualsEnd
#undef OOOInterface

#endif
