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

#define OOOIFileWriteDataClosure(CLOSURE_TYPE) \
	OOOClosureCallbackType(CLOSURE_TYPE, OOOIError * iError) \
	OOOClosureImplementation(OOOIFileWriteData, CLOSURE_TYPE, char * szPath, unsigned char * pData, size_t uSize) \
		char * szPath; \
		unsigned char * pData; \
		size_t uSize; \
	OOOClosurePrivateDataEnd \
		OOOClosureMapPrivateData(char *, getPath, szPath) \
		OOOClosureMapPrivateData(unsigned char *, getData, pData) \
		OOOClosureMapPrivateData(size_t, getSize, uSize) \
		OOOClosureCallbackStart(written, OOOIError * iError) \
			OOOClosureCallbackEnd(iError) \
	OOOClosureConstructor(OOOIFileWriteData, CLOSURE_TYPE, char * szPath, unsigned char * pData, size_t uSize) \
		OOOClosureMapVirtual(getPath) \
		OOOClosureMapVirtual(getData) \
		OOOClosureMapVirtual(getSize) \
		OOOClosureMapVirtual(written) \
	OOOClosureMapFields(OOOIFileWriteData) \
		OOOClosureMapField(szPath) \
		OOOClosureMapField(pData) \
		OOOClosureMapField(uSize) \
	OOOClosureMapFieldsEnd

#endif
