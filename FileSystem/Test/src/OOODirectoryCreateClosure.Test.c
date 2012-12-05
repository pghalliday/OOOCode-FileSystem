#include "OOOUnitTestDefines.h"
#include "OOOError.h"
#include "OOOIDirectoryCreateData.h"

#define TEST_CLOSURE_DATA	"This is a test"
#define TEST_PATH			"This is a test path"
#define TEST_ERROR			"This is a test error"

#define OOOClass TestTarget
OOODeclarePrivate()
	OOOImplements
	OOOImplementsEnd
	OOOExports
		OOOExport(void, close, OOOIDirectoryCreateData * iDirectoryCreateData)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
OOOPrivateDataEnd

OOODestructor
OOODestructorEnd

OOOMethod(void, close, OOOIDirectoryCreateData * iDirectoryCreateData)
{
	char * szPath = OOOICall(iDirectoryCreateData, getPath);
	OOOError * pError = OOOConstruct(OOOError, TEST_ERROR);
	if (O_strcmp(szPath, TEST_PATH) != 0)
	{
		OOOError("Expected: %s: Received: %s", TEST_PATH, szPath);
	}
	OOOICall(iDirectoryCreateData, created, OOOCast(OOOIError, pError));
	OOODestroy(pError);
}
OOOMethodEnd

OOOConstructorPrivate()
{
	OOOMapMethods
		OOOMapMethod(close)
	OOOMapMethodsEnd
}
OOOConstructorEnd
#undef OOOClass

#define OOOClass Test
OOODeclarePrivate()
	OOOImplements
	OOOImplementsEnd
	OOOExports
		OOOExport(void, start)
	OOOExportsEnd
OOODeclareEnd

// declare the test closure class
#define OOOClosure TestClosure
#define OOOClosureType char *
#include "OOODirectoryCreateClosure.h"
#undef OOOClosureType
#undef OOOClosure

OOOPrivateData
	TestTarget * pTestTarget;
	bool bChecked;
OOOPrivateDataEnd

OOODestructor
{
	OOODestroy(OOOF(pTestTarget));
}
OOODestructorEnd

OOOMethod(void, created, char * szClosureData, OOOIError * iError)
{
	char * szError = OOOICall(iError, toString);
	if (O_strcmp(szClosureData, TEST_CLOSURE_DATA) != 0)
	{
		OOOError("Expected: %s: Received: %s", TEST_CLOSURE_DATA, szClosureData);
	}
	if (O_strcmp(szError, TEST_ERROR) != 0)
	{
		OOOError("Expected: %s: Received: %s", TEST_ERROR, szError);
	}
	OOOF(bChecked) = TRUE;
}
OOOMethodEnd

OOOMethod(void, start)
{
	TestClosure * pClosure = OOOClosureConstruct
	(
		TestClosure,
		created,
		TEST_CLOSURE_DATA,
		TEST_PATH
	);
	OOOCall(OOOF(pTestTarget), close, OOOCast(OOOIDirectoryCreateData, pClosure));
	OOOCheck(OOOF(bChecked));
}
OOOMethodEnd

OOOConstructorPrivate()
{
	OOOMapMethods
		OOOMapMethod(start)
	OOOMapMethodsEnd

	OOOF(pTestTarget) = OOOConstruct(TestTarget);
}
OOOConstructorEnd
#undef OOOClass

OOOTest(OOODirectoryCreateClosure)
{
	Test * pTest = OOOConstruct(Test);
	OOOCall(pTest, start);
	OOODestroy(pTest);
}
