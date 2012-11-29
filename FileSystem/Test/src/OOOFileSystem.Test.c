#include "OOOUnitTestDefines.h"
#include "OOOFileSystem.h"

#define TEST_DIRECTORY1	"/test1"
#define TEST_DIRECTORY2	"test2"
#define TEST_DIRECTORY3	"test3"
#define TEST_DIRECTORY	TEST_DIRECTORY1 "/" TEST_DIRECTORY2 "/" TEST_DIRECTORY3

#define TEST_FILE		TEST_DIRECTORY "/test"
#define INVALID_FILE	"/"

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
		if (iError != NULL)
		{
			OOOError(OOOICall(iError, toString));
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
		if (iError != NULL)
		{
			OOOError(OOOICall(iError, toString));
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
		if (iError != NULL)
		{
			OOOError(OOOICall(iError, toString));
		}

		/* check the contents of the written file */
		{
			o_file hFile = O_file_open(OOOF(szPath), OTV_O_RDONLY, OTV_O_NO_CREATION_MODE, 0);
			if (OOOCheck(hFile != FAILURE))
			{
				ssize_t nSize = O_fh_get_length(hFile);
				if (OOOCheck(nSize != FAILURE))
				{
					if (OOOCheck((size_t) nSize == OOOF(uSize)))
					{
						if (nSize > 0)
						{
							char * szContents = O_calloc(nSize, sizeof(char));
							nSize = O_fh_read(hFile, szContents, nSize);
							if (OOOCheck(nSize != FAILURE))
							{
								if (OOOCheck((size_t) nSize == OOOF(uSize)))
								{
									if (OOOCheck(O_strcmp(szContents, (char *) OOOF(pData)) == 0))
									{
										nSize = O_fh_read(hFile, szContents, nSize);
										OOOCheck(nSize == 0);
									}
								}
							}
							O_free(szContents);
						}
					}
				}
				O_fh_close(hFile);
			}
		}
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
		if (iError != NULL)
		{
			OOOError(OOOICall(iError, toString));
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
		if (iError != NULL)
		{
			OOOError(OOOICall(iError, toString));
		}

		/* check that the file was removed */
		{
			o_file hFile = O_file_open(OOOF(szPath), OTV_O_RDONLY, OTV_O_NO_CREATION_MODE, 0);
			if (!OOOCheck(hFile == FAILURE))
			{
				O_fh_close(hFile);
			}
		}
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

static void removeFile(OOOFileSystem * pFileSystem, char * szPath, char * szError)
{
	TestFileData * pTestFileData = OOOConstruct(TestFileData, szError, szPath, NULL, 0);
	OOOICall(OOOCast(OOOIFileSystem, pFileSystem), removeFile, OOOCast(OOOIFileRemoveData, pTestFileData));
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

	/* Remove should fail if there is no such file */
	removeFile(pFileSystem, TEST_FILE, "O_file_remove failed: " TEST_FILE);

	/* Read should error if there is no such file */
	readFile(pFileSystem, TEST_FILE, "O_file_open failed: " TEST_FILE, NULL, 0);

	/* Write should error if the data can not be written */
	writeFile(pFileSystem, INVALID_FILE, "Path exists and is a directory: " INVALID_FILE, NULL, 0);

	/* Write should write an empty file to the file system, creating intermediate directories */
	writeFile(pFileSystem, TEST_FILE, NULL, NULL, 0);

	/* Read should read an empty file from the file system */
	readFile(pFileSystem, TEST_FILE, NULL, NULL, 0);

	/* Write should overwrite an existing file */
	writeFile(pFileSystem, TEST_FILE, NULL, "Hello, World!", O_strlen("Hello, World!") + 1);

	/* Read should read a non-empty file from the file system */
	readFile(pFileSystem, TEST_FILE, NULL, "Hello, World!", O_strlen("Hello, World!") + 1);

	/* Remove should remove a file from the file system */
	removeFile(pFileSystem, TEST_FILE, NULL);

	/* TODO: test more error conditions */

	OOODestroy(pFileSystem);
}
