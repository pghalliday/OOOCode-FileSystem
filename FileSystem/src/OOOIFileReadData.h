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

#define OOOIFileReadDataClosure(CLOSURE_TYPE) \
	OOOClosureCallbackType(CLOSURE_TYPE, OOOIError * iError, unsigned char * pData, size_t uSize) \
	OOOClosureImplementation(OOOIFileReadData, CLOSURE_TYPE, char * szPath) \
		char * szPath; \
	OOOClosurePrivateDataEnd \
		OOOClosureMapPrivateData(char *, getPath, szPath) \
		OOOClosureCallbackStart(read, OOOIError * iError, unsigned char * pData, size_t uSize) \
			OOOClosureCallbackEnd(iError, pData, uSize) \
	OOOClosureConstructor(OOOIFileReadData, CLOSURE_TYPE, char * szPath) \
		OOOClosureMapVirtual(getPath) \
		OOOClosureMapVirtual(read) \
	OOOClosureMapFields(OOOIFileReadData) \
		OOOClosureMapField(szPath) \
	OOOClosureMapFieldsEnd

#endif
