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

#define OOOIFileReadDataClosure(CLOSURE_NAME, CLASS_NAME, CLOSURE_TYPE) \
	typedef void (* CLOSURE_NAME##_CALLBACK)(CLASS_NAME * pInstance, CLOSURE_TYPE * pClosure, OOOIError * iError, unsigned char * pData, size_t uSize); \
	__OOODeclare(OOOPrivate, CLOSURE_NAME, CLASS_NAME * pInstance, CLOSURE_NAME##_CALLBACK cbCallback, CLOSURE_TYPE * pClosure, char * szPath) \
		OOOImplements \
			OOOImplement(OOOIFileReadData) \
		_OOOImplementsEnd(CLOSURE_NAME) \
		_OOOExports(CLOSURE_NAME) \
		_OOOExportsEnd(CLOSURE_NAME) \
	_OOODeclareEnd(CLOSURE_NAME) \
	_OOOPrivateData(CLOSURE_NAME) \
		CLASS_NAME * pInstance; \
		CLOSURE_NAME##_CALLBACK cbCallback; \
		CLOSURE_TYPE * pClosure; \
		char * szPath; \
	_OOOPrivateDataEnd(CLOSURE_NAME) \
	_OOODestructor(CLOSURE_NAME) \
	_OOODestructorEnd(CLOSURE_NAME) \
	_OOOMethod(CLOSURE_NAME, char *, getPath) \
	{ \
		return _OOOField(CLOSURE_NAME, OOOThis, szPath); \
	} \
	OOOMethodEnd \
	_OOOMethod(CLOSURE_NAME, void, read, OOOIError * iError, unsigned char * pData, size_t uSize) \
	{ \
		_OOOField(CLOSURE_NAME, OOOThis, cbCallback)(_OOOField(CLOSURE_NAME, OOOThis, pInstance), _OOOField(CLOSURE_NAME, OOOThis, pClosure), iError, pData, uSize); \
		OOODestroy(OOOThis); \
	} \
	OOOMethodEnd \
	__OOOConstructor(OOOPrivate, CLOSURE_NAME, CLASS_NAME * pInstance, CLOSURE_NAME##_CALLBACK cbCallback, CLOSURE_TYPE * pClosure, char * szPath) \
	{ \
		__OOOMapVirtuals(CLOSURE_NAME, OOOIFileReadData) \
			_OOOMapVirtual(CLOSURE_NAME, getPath) \
			_OOOMapVirtual(CLOSURE_NAME, read) \
		_OOOMapVirtualsEnd(OOOIFileReadData) \
		_OOOMapMethods(CLOSURE_NAME) \
		OOOMapMethodsEnd \
		_OOOField(CLOSURE_NAME, OOOThis, pInstance) = pInstance; \
		_OOOField(CLOSURE_NAME, OOOThis, cbCallback) = cbCallback; \
		_OOOField(CLOSURE_NAME, OOOThis, pClosure) = pClosure; \
		_OOOField(CLOSURE_NAME, OOOThis, szPath) = szPath; \
	} \
	OOOConstructorEnd

#define __OOOClosureConstruct(CLASS_NAME, CONTAINER_CLASS, CALLBACK, ARGS...) \
	OOOConstruct(CLASS_NAME, OOOThis, CONTAINER_CLASS##_##CALLBACK, ARGS)
#define _OOOClosureConstruct(CLASS_NAME, CONTAINER_CLASS, CALLBACK, ARGS...) __OOOClosureConstruct(CLASS_NAME, CONTAINER_CLASS, CALLBACK, ARGS)
#define OOOClosureConstruct(CLASS_NAME, CALLBACK, ARGS...) _OOOClosureConstruct(CLASS_NAME, OOOClass, CALLBACK, ARGS)


#endif
