#include "OOOUnitTestDefines.h"
#include "OOOPath.h"

#define TEST_DIRECTORY1	"/test1"
#define TEST_DIRECTORY2	"test2"
#define TEST_DIRECTORY3	"test3"
#define TEST_DIRECTORY	TEST_DIRECTORY1 "/" TEST_DIRECTORY2 "/" TEST_DIRECTORY3

char * resolvePath(char * szPath)
{
	char * szAbsolutePath;
	if (szPath[0] == '/')
	{
		szAbsolutePath = O_strdup(szPath);
	}
	else
	{
		char * szCurrentDirectory = O_dir_get_path(NULL, 0);
		szAbsolutePath = O_calloc(O_strlen(szCurrentDirectory) + 1 + O_strlen(szPath) + 1, sizeof(char));
		O_strcpy(szAbsolutePath, szCurrentDirectory);
		O_free(szCurrentDirectory);
		O_strcat(szAbsolutePath, "/");
		O_strcat(szAbsolutePath, szPath);
	}
	return szAbsolutePath;
}

OOOTest(OOOPath)
{
	char * szPath;
	OOOPath * pPath1 = OOOConstruct(OOOPath, NULL, TEST_DIRECTORY1);
	OOOPath * pPath2 = OOOConstruct(OOOPath, pPath1, TEST_DIRECTORY2);
	OOOPath * pPath3 = OOOConstruct(OOOPath, pPath2, TEST_DIRECTORY3);
	OOOPath * pRelativePath = OOOConstruct(OOOPath, NULL, TEST_DIRECTORY3);
	OOOPath * pAbsolutePath = OOOConstruct(OOOPath, pPath3, TEST_DIRECTORY1);

	/* Should be able to get the absolute path */
	OOOCheck(O_strcmp(TEST_DIRECTORY1, OOOCall(pPath1, getAbsolutePath)) == 0);
	OOOCheck(O_strcmp(TEST_DIRECTORY, OOOCall(pPath3, getAbsolutePath)) == 0);

	/* Absolute paths with a parent specified should should ignore the parent path */
	OOOCheck(O_strcmp(TEST_DIRECTORY1, OOOCall(pAbsolutePath, getAbsolutePath)) == 0);

	/* Relative paths with no parent specified should resolve relative to the current directory */
	szPath = resolvePath(TEST_DIRECTORY3);
	OOOCheck(O_strcmp(szPath, OOOCall(pRelativePath, getAbsolutePath)) == 0);
	O_free(szPath);

	OOODestroy(pPath1);
	OOODestroy(pPath2);
	OOODestroy(pPath3);
	OOODestroy(pRelativePath);
	OOODestroy(pAbsolutePath);
}
