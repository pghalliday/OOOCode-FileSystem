#include "OOOFileSystem.h"
#include "OOOError.h"
#include "limits.h"

#define OOOClass RecursiveDirectoryCreateData
OOODeclarePrivate(OOOFileSystem * pFileSystem, OOOIDirectoryCreateData * iDirectoryCreateData)
	OOOImplements
		OOOImplement(OOOIDirectoryCreateData)
	OOOImplementsEnd
	OOOExports
		OOOExport(void, create)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	OOOFileSystem * pFileSystem;
	char * szPath;
	char * szParentPath;
	OOOIDirectoryCreateData * iDirectoryCreateData;
OOOPrivateDataEnd

OOODestructor
{
	O_free(OOOF(szParentPath));
}
OOODestructorEnd

OOOMethod(char *, getPath)
{
	return OOOF(szParentPath);
}
OOOMethodEnd

OOOMethod(void, createLast)
{
	if (O_dir_create(OOOF(szPath), OTV_WORLD_READ_WRITE_PERMISSION, UINT_MAX) == SUCCESS)
	{
		OOOICall(OOOF(iDirectoryCreateData), created, NULL);
	}
	else
	{
		OOOError * pError = OOOConstruct(OOOError, "O_dir_create failed: %s", OOOF(szPath));
		OOOICall(OOOF(iDirectoryCreateData), created, OOOCast(OOOIError, pError));
		OOODestroy(pError);
	}
	OOODestroy(OOOThis);
}
OOOMethodEnd

OOOMethod(void, created, OOOIError * iError)
{
	if (iError)
	{
		OOOICall(OOOF(iDirectoryCreateData), created, iError);
		OOODestroy(OOOThis);
	}
	else
	{
		OOOC(createLast);
	}
}
OOOMethodEnd

OOOMethod(void, create)
{
	o_stat tStat;
	if (O_file_get_stat(OOOF(szPath), &tStat) == SUCCESS)
	{
		if (tStat.permissions & OTV_DIRECTORY_FLAG != OTV_DIRECTORY_FLAG)
		{
			OOOError * pError = OOOConstruct(OOOError, "Path exists and is not a directory: %s", OOOF(szPath));
			OOOICall(OOOF(iDirectoryCreateData), created, OOOCast(OOOIError, pError));
			OOODestroy(pError);
		}
		else
		{
			OOOICall(OOOF(iDirectoryCreateData), created, NULL);
		}
		OOODestroy(OOOThis);
	}
	else
	{
		if (O_strlen(OOOF(szParentPath)) > 0)
		{
			OOOICall(OOOCast(OOOIFileSystem, OOOF(pFileSystem)), createDirectory, OOOCast(OOOIDirectoryCreateData, OOOThis));
		}
		else
		{
			OOOC(createLast);
		}
	}
}
OOOMethodEnd

OOOConstructorPrivate(OOOFileSystem * pFileSystem, OOOIDirectoryCreateData * iDirectoryCreateData)
{
	char * szLastPath;

#define OOOInterface OOOIDirectoryCreateData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(created)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
	OOOMapMethodsEnd

	OOOF(pFileSystem) = pFileSystem;
	OOOF(szPath) = OOOICall(iDirectoryCreateData, getPath);
	szLastPath = O_strrchr(OOOF(szPath), '/');
	OOOF(szParentPath) = O_calloc(szLastPath - OOOF(szPath) + 1, sizeof(char));
	O_strncpy(OOOF(szParentPath), OOOF(szPath), szLastPath - OOOF(szPath));
	OOOF(iDirectoryCreateData) = iDirectoryCreateData;
}
OOOConstructorEnd
#undef OOOClass

#define OOOClass RecursiveDirectoryRemoveData
OOODeclarePrivate(OOOFileSystem * pFileSystem, OOOIDirectoryRemoveData * iDirectoryRemoveData)
	OOOImplements
		OOOImplement(OOOIDirectoryRemoveData)
		OOOImplement(OOOIFileRemoveData)
	OOOImplementsEnd
	OOOExports
		OOOExport(void, remove)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	OOOFileSystem * pFileSystem;
	char * szPath;
	OOOIDirectoryRemoveData * iDirectoryRemoveData;
	o_dir * pDir;
	char * szChildPath;
	char * szInsert;
OOOPrivateDataEnd

OOODestructor
{
	O_free(OOOF(szChildPath));
}
OOODestructorEnd

OOOMethod(char *, getPath)
{
	return OOOF(szChildPath);
}
OOOMethodEnd

OOOMethod(void, removeNext)
{
	o_dirent tDirent;
	if (O_dir_read(OOOF(pDir), &tDirent))
	{
		o_stat tStat;
		O_strcpy(OOOF(szInsert), tDirent.d_name);
		if (O_file_get_stat(OOOF(szChildPath), &tStat) == SUCCESS)
		{
			if (tStat.permissions & OTV_DIRECTORY_FLAG == OTV_DIRECTORY_FLAG)
			{
				OOOICall(OOOCast(OOOIFileSystem, OOOF(pFileSystem)), removeDirectory, OOOCast(OOOIDirectoryRemoveData, OOOThis));
			}
			else
			{
				OOOICall(OOOCast(OOOIFileSystem, OOOF(pFileSystem)), removeFile, OOOCast(OOOIFileRemoveData, OOOThis));
			}
		}
		else
		{
			OOOError * pError = OOOConstruct(OOOError, "O_file_get_stat failed: %s", OOOF(szChildPath));
			O_dir_close(OOOF(pDir));
			OOOICall(OOOF(iDirectoryRemoveData), removed, OOOCast(OOOIError, pError));
			OOODestroy(pError);
			OOODestroy(OOOThis);
		}
	}
	else
	{
		O_dir_close(OOOF(pDir));
		if (O_dir_remove(OOOF(szPath)) == SUCCESS)
		{
			OOOICall(OOOF(iDirectoryRemoveData), removed, NULL);
		}
		else
		{
			OOOError * pError = OOOConstruct(OOOError, "O_dir_remove failed: %s", OOOF(szPath));
			OOOICall(OOOF(iDirectoryRemoveData), removed, OOOCast(OOOIError, pError));
			OOODestroy(pError);
		}
		OOODestroy(OOOThis);
	}
}
OOOMethodEnd

OOOMethod(void, removed, OOOIError * iError)
{
	if (iError)
	{
		O_dir_close(OOOF(pDir));
		OOOICall(OOOF(iDirectoryRemoveData), removed, iError);
		OOODestroy(OOOThis);
	}
	else
	{
		OOOC(removeNext);
	}
}
OOOMethodEnd

OOOMethod(void, remove)
{
	OOOF(pDir) = O_dir_open(OOOF(szPath));
	if (OOOF(pDir))
	{
		OOOC(removeNext);
	}
	else
	{
		OOOError * pError = OOOConstruct(OOOError, "O_dir_open failed: %s", OOOF(szPath));
		OOOICall(OOOF(iDirectoryRemoveData), removed, OOOCast(OOOIError, pError));
		OOODestroy(pError);
		OOODestroy(OOOThis);
	}
}
OOOMethodEnd

OOOConstructorPrivate(OOOFileSystem * pFileSystem, OOOIDirectoryRemoveData * iDirectoryRemoveData)
{
#define OOOInterface OOOIDirectoryRemoveData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(removed)
	OOOMapVirtualsEnd
#undef OOOInterface

#define OOOInterface OOOIFileRemoveData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(removed)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
		OOOMapMethod(remove)
	OOOMapMethodsEnd

	OOOF(pFileSystem) = pFileSystem;
	OOOF(szPath) = OOOICall(iDirectoryRemoveData, getPath);
	OOOF(iDirectoryRemoveData) = iDirectoryRemoveData;
	OOOF(szChildPath) = O_calloc(O_strlen(OOOF(szPath)) + 1 + OTV_MAX_FILE_NAME_LENGTH + 1, sizeof(char));
	OOOF(szInsert) = OOOF(szChildPath) + O_strlen(OOOF(szPath)) + 1;
	O_strcpy(OOOF(szChildPath), OOOF(szPath));
	O_strcat(OOOF(szChildPath), "/");
}
OOOConstructorEnd
#undef OOOClass

#define OOOClass OOOFileSystem

OOOPrivateData
OOOPrivateDataEnd

OOODestructor
{
}
OOODestructorEnd

OOOMethod(void, createDirectory, OOOIDirectoryCreateData * iDirectoryCreateData)
{
	RecursiveDirectoryCreateData * pDirectoryData = OOOConstruct(RecursiveDirectoryCreateData, OOOThis, iDirectoryCreateData);
	OOOPCCall(RecursiveDirectoryCreateData, pDirectoryData, create);
}
OOOMethodEnd

OOOMethod(void, removeDirectory, OOOIDirectoryRemoveData * iDirectoryRemoveData)
{
	RecursiveDirectoryRemoveData * pDirectoryData = OOOConstruct(RecursiveDirectoryRemoveData, OOOThis, iDirectoryRemoveData);
	OOOPCCall(RecursiveDirectoryRemoveData, pDirectoryData, remove);
}
OOOMethodEnd

OOOMethod(void, writeFile, OOOIFileWriteData * iFileWriteData)
{
	char * szPath = OOOICall(iFileWriteData, getPath);
	OOOError * pError = OOOConstruct(OOOError, "Write failure: %s", szPath);
	OOOICall(iFileWriteData, written, OOOCast(OOOIError, pError));
	OOODestroy(pError);
}
OOOMethodEnd

OOOMethod(void, readFile, OOOIFileReadData * iFileReadData)
{
	char * szPath = OOOICall(iFileReadData, getPath);
	OOOError * pError = OOOConstruct(OOOError, "No such file: %s", szPath);
	OOOICall(iFileReadData, read, OOOCast(OOOIError, pError), NULL, 0);
	OOODestroy(pError);
}
OOOMethodEnd

OOOMethod(void, removeFile, OOOIFileRemoveData * iFileRemoveData)
{
	char * szPath = OOOICall(iFileRemoveData, getPath);
	OOOError * pError = OOOConstruct(OOOError, "No such file: %s", szPath);
	OOOICall(iFileRemoveData, removed, OOOCast(OOOIError, pError));
	OOODestroy(pError);
}
OOOMethodEnd

OOOConstructor()
{
#define OOOInterface OOOIFileSystem
	OOOMapVirtuals
		OOOMapVirtual(createDirectory)
		OOOMapVirtual(removeDirectory)
		OOOMapVirtual(writeFile)
		OOOMapVirtual(readFile)
		OOOMapVirtual(removeFile)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
	OOOMapMethodsEnd
}
OOOConstructorEnd

#undef OOOClass
