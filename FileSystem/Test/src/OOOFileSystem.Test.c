#include "OOOUnitTestDefines.h"
#include "OOOFileSystem.h"

#define TEST_DIRECTORY1	"/test1"
#define TEST_DIRECTORY2	"test2"
#define TEST_DIRECTORY3	"test3"
#define TEST_DIRECTORY	TEST_DIRECTORY1 "/" TEST_DIRECTORY2 "/" TEST_DIRECTORY3

#define TEST_FILE	TEST_DIRECTORY "/test"

void recursiveDelete(char * szPath)
{
	o_dir * pDir = O_dir_open(szPath);
	if (pDir)
	{
		o_dirent tDirent;
		char * szChildPath = O_calloc(O_strlen(szPath) + 1 + OTV_MAX_FILE_NAME_LENGTH + 1, sizeof(char));
		char * szInsert = szChildPath + O_strlen(szPath) + 1;
		O_strcpy(szChildPath, szPath);
		O_strcat(szChildPath, "/");
		while (O_dir_read(pDir, &tDirent))
		{
			o_stat tStat;
			O_strcpy(szInsert, tDirent.d_name);
			if (O_file_get_stat(szChildPath, &tStat) == SUCCESS)
			{
				if (tStat.permissions & OTV_DIRECTORY_FLAG == OTV_DIRECTORY_FLAG)
				{
					recursiveDelete(szChildPath);
				}
				else
				{
					assert(O_file_remove(szChildPath) == SUCCESS);
				}
			}
		}
		O_free(szChildPath);
		O_dir_close(pDir);
		assert(O_dir_remove(szPath) == SUCCESS);
	}
}

bool isDirectory(char * szPath)
{
	bool bIsDirectory = FALSE;
	o_stat tStat;
	if (O_file_get_stat(szPath, &tStat) == SUCCESS)
	{
		if (tStat.permissions & OTV_DIRECTORY_FLAG == OTV_DIRECTORY_FLAG)
		{
			bIsDirectory = TRUE;
		}
	}
	return bIsDirectory;
}

#define OOOClass TestDirectoryData
OOODeclarePrivate(char * szError, char * szPath)
	OOOImplements
		OOOImplement(OOOIDirectoryCreateData)
		OOOImplement(OOOIDirectoryRemoveData)
	OOOImplementsEnd
	OOOExports
		OOOExport(bool, wasChecked)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	char * szError;
	char * szPath;
	bool bChecked;
OOOPrivateDataEnd

OOODestructor
OOODestructorEnd

OOOMethod(char *, getPath)
{
	return OOOF(szPath);
}
OOOMethodEnd

OOOMethod(void, created, OOOIError * iError)
{
	if (OOOF(szError))
	{
		if (OOOCheck(iError != NULL))
		{
			OOOCheck(O_strcmp(OOOICall(iError, toString), OOOF(szError)) == 0);
		}
	}
	else
	{
		if (!OOOCheck(iError == NULL))
		{
			O_debug("Error: %s\n", OOOICall(iError, toString));
		}
		OOOCheck(isDirectory(OOOF(szPath)));
	}
	OOOF(bChecked) = TRUE;
}
OOOMethodEnd

OOOMethod(void, removed, OOOIError * iError)
{
	if (OOOF(szError))
	{
		if (OOOCheck(iError != NULL))
		{
			OOOCheck(O_strcmp(OOOICall(iError, toString), OOOF(szError)) == 0);
		}
	}
	else
	{
		if (!OOOCheck(iError == NULL))
		{
			O_debug("Error: %s\n", OOOICall(iError, toString));
		}
		OOOCheck(!isDirectory(OOOF(szPath)));
	}
	OOOF(bChecked) = TRUE;
}
OOOMethodEnd

OOOMethod(bool, wasChecked)
{
	return OOOF(bChecked);
}
OOOMethodEnd

OOOConstructorPrivate(char * szError, char * szPath)
{
#define OOOInterface OOOIDirectoryCreateData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(created)
	OOOMapVirtualsEnd
#undef OOOInterface

#define OOOInterface OOOIDirectoryRemoveData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(removed)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
		OOOMapMethod(wasChecked)
	OOOMapMethodsEnd

	OOOF(szError) = szError;
	OOOF(szPath) = szPath;
}
OOOConstructorEnd
#undef OOOClass

#define OOOClass TestFileData
OOODeclarePrivate(char * szError, char * szPath, unsigned char * pData, size_t uSize)
	OOOImplements
		OOOImplement(OOOIFileReadData)
		OOOImplement(OOOIFileWriteData)
		OOOImplement(OOOIFileRemoveData)
	OOOImplementsEnd
	OOOExports
		OOOExport(bool, wasChecked)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	char * szError;
	char * szPath;
	unsigned char * pData;
	size_t uSize;
	bool bChecked;
OOOPrivateDataEnd

OOODestructor
OOODestructorEnd

OOOMethod(char *, getPath)
{
	return OOOF(szPath);
}
OOOMethodEnd

OOOMethod(unsigned char *, getData)
{
	return OOOF(pData);
}
OOOMethodEnd

OOOMethod(size_t, getSize)
{
	return OOOF(uSize);
}
OOOMethodEnd

OOOMethod(void, written, OOOIError * iError)
{
	if (OOOF(szError))
	{
		if (OOOCheck(iError != NULL))
		{
			OOOCheck(O_strcmp(OOOICall(iError, toString), OOOF(szError)) == 0);
		}
	}
	else
	{
		if (!OOOCheck(iError == NULL))
		{
			O_debug("Error: %s\n", OOOICall(iError, toString));
		}

		/* TODO: check the contents of the written file */
	}
	OOOF(bChecked) = TRUE;
}
OOOMethodEnd

OOOMethod(void, read, OOOIError * iError, unsigned char * pData, size_t uSize)
{
	if (OOOF(pData))
	{
		if (OOOCheck((bool) pData))
		{
			OOOCheck(O_strcmp(pData, OOOF(pData)) == 0);
		}
	}
	else
	{
		OOOCheck(pData == NULL);
	}
	OOOCheck(uSize == OOOF(uSize));
	if (OOOF(szError))
	{
		if (OOOCheck(iError != NULL))
		{
			OOOCheck(O_strcmp(OOOICall(iError, toString), OOOF(szError)) == 0);
		}
	}
	else
	{
		if (!OOOCheck(iError == NULL))
		{
			O_debug("Error: %s\n", OOOICall(iError, toString));
		}
	}
	OOOF(bChecked) = TRUE;
}
OOOMethodEnd

OOOMethod(void, removed, OOOIError * iError)
{
	if (OOOF(szError))
	{
		if (OOOCheck(iError != NULL))
		{
			OOOCheck(O_strcmp(OOOICall(iError, toString), OOOF(szError)) == 0);
		}
	}
	else
	{
		if (!OOOCheck(iError == NULL))
		{
			O_debug("Error: %s\n", OOOICall(iError, toString));
		}

		/* TODO: check that the file was removed */
	}
	OOOF(bChecked) = TRUE;
}
OOOMethodEnd

OOOMethod(bool, wasChecked)
{
	return OOOF(bChecked);
}
OOOMethodEnd

OOOConstructorPrivate(char * szError, char * szPath, unsigned char * pData, size_t uSize)
{
#define OOOInterface OOOIFileWriteData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(getData)
		OOOMapVirtual(getSize)
		OOOMapVirtual(written)
	OOOMapVirtualsEnd
#undef OOOInterface

#define OOOInterface OOOIFileReadData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(read)
	OOOMapVirtualsEnd
#undef OOOInterface

#define OOOInterface OOOIFileRemoveData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(removed)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
		OOOMapMethod(wasChecked)
	OOOMapMethodsEnd

	OOOF(szError) = szError;
	OOOF(szPath) = szPath;
	OOOF(pData) = pData;
	OOOF(uSize) = uSize;
}
OOOConstructorEnd
#undef OOOClass

static void createDirectory(OOOFileSystem * pFileSystem, char * szPath, char * szError)
{
	TestDirectoryData * pTestDirectoryData = OOOConstruct(TestDirectoryData, szError, szPath);
	OOOICall(OOOCast(OOOIFileSystem, pFileSystem), createDirectory, OOOCast(OOOIDirectoryCreateData, pTestDirectoryData));
	OOOCheck(OOOCall(pTestDirectoryData, wasChecked));
	OOODestroy(pTestDirectoryData);
}

static void removeDirectory(OOOFileSystem * pFileSystem, char * szPath, char * szError)
{
	TestDirectoryData * pTestDirectoryData = OOOConstruct(TestDirectoryData, szError, szPath);
	OOOICall(OOOCast(OOOIFileSystem, pFileSystem), removeDirectory, OOOCast(OOOIDirectoryRemoveData, pTestDirectoryData));
	OOOCheck(OOOCall(pTestDirectoryData, wasChecked));
	OOODestroy(pTestDirectoryData);
}

static void readFile(OOOFileSystem * pFileSystem, char * szPath, char * szError, unsigned char * pData, size_t uSize)
{
	TestFileData * pTestFileData = OOOConstruct(TestFileData, szError, szPath, pData, uSize);
	OOOICall(OOOCast(OOOIFileSystem, pFileSystem), readFile, OOOCast(OOOIFileReadData, pTestFileData));
	OOOCheck(OOOCall(pTestFileData, wasChecked));
	OOODestroy(pTestFileData);
}

static void writeFile(OOOFileSystem * pFileSystem, char * szPath, char * szError, unsigned char * pData, size_t uSize)
{
	TestFileData * pTestFileData = OOOConstruct(TestFileData, szError, szPath, pData, uSize);
	OOOICall(OOOCast(OOOIFileSystem, pFileSystem), writeFile, OOOCast(OOOIFileWriteData, pTestFileData));
	OOOCheck(OOOCall(pTestFileData, wasChecked));
	OOODestroy(pTestFileData);
}

OOOTest(OOOFileSystem)
{
	OOOFileSystem * pFileSystem = OOOConstruct(OOOFileSystem);

	/* start by removing the test directories and files if they are there */
	recursiveDelete(TEST_DIRECTORY1);

	/* Should create a directory including parent directories */
	createDirectory(pFileSystem, TEST_DIRECTORY, NULL);

	/* Should recursively delete a directory */
	removeDirectory(pFileSystem, TEST_DIRECTORY1, NULL);

	/* Read should error if there is no such file */
	readFile(pFileSystem, TEST_FILE, "No such file: " TEST_FILE, NULL, 0);

	/* Write should error if the data can not be written */
	writeFile(pFileSystem, TEST_FILE, "Write failure: " TEST_FILE, NULL, 0);

	/* TODO: Write should write an empty file to the file system */
	writeFile(pFileSystem, TEST_FILE, NULL, NULL, 0);

	/* TODO: test remove file and lots of other read and write conditions */
	/* TODO: test directory create and remove error conditions */

	OOODestroy(pFileSystem);
}
