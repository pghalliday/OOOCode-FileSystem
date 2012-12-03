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

#define OOOIDirectoryRemoveDataClosure(CLOSURE_TYPE) \
	OOOClosureCallbackType(CLOSURE_TYPE, OOOIError * iError) \
	OOOClosureImplementation(OOOIDirectoryRemoveData, CLOSURE_TYPE, char * szPath) \
		char * szPath; \
	OOOClosurePrivateDataEnd \
		OOOClosureMapPrivateData(char *, getPath, szPath) \
		OOOClosureCallbackStart(removed, OOOIError * iError) \
			OOOClosureCallbackEnd(iError) \
	OOOClosureConstructor(OOOIDirectoryRemoveData, CLOSURE_TYPE, char * szPath) \
		OOOClosureMapVirtual(getPath) \
		OOOClosureMapVirtual(removed) \
	OOOClosureMapFields(OOOIDirectoryRemoveData) \
		OOOClosureMapField(szPath) \
	OOOClosureMapFieldsEnd

#endif
