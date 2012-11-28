#ifndef OOOIFileSystem_H
#define OOOIFileSystem_H

#include "OOOCode.h"
#include "OOOIFileReadData.h"
#include "OOOIFileWriteData.h"
#include "OOOIFileRemoveData.h"
#include "OOOIDirectoryCreateData.h"
#include "OOOIDirectoryRemoveData.h"

#define OOOInterface OOOIFileSystem
OOOVirtuals
	OOOVirtual(void, createDirectory, OOOIDirectoryCreateData * iDirectoryCreateData)
	OOOVirtual(void, removeDirectory, OOOIDirectoryRemoveData * iDirectoryRemoveData)
	OOOVirtual(void, writeFile, OOOIFileWriteData * iFileWriteData)
	OOOVirtual(void, readFile, OOOIFileReadData * iFileReadData)
	OOOVirtual(void, removeFile, OOOIFileRemoveData * iFileRemoveData)
OOOVirtualsEnd
#undef OOOInterface

#endif
