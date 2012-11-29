#include "OOOFileSystem.h"
#include "OOOError.h"
#include "limits.h"

#define OOOClass DirectoryCreateData
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
	OOOIDirectoryCreateData * iDirectoryCreateData;
	char * szPath;
	char * szParentPath;
OOOPrivateDataEnd

OOODestructor
{
	if (OOOF(szParentPath))
	{
		O_free(OOOF(szParentPath));
	}
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
	OOOF(szPath) = OOOICall(OOOF(iDirectoryCreateData), getPath);
	if (OOOF(szPath) && OOOF(szPath)[0] == '/')
	{
		o_stat tStat;
		if (O_file_get_stat(OOOF(szPath), &tStat) == SUCCESS)
		{
			if ((tStat.permissions & OTV_DIRECTORY_FLAG) != OTV_DIRECTORY_FLAG)
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
			char * szLastPath = O_strrchr(OOOF(szPath), '/');
			OOOF(szParentPath) = O_calloc(szLastPath - OOOF(szPath) + 1, sizeof(char));
			O_strncpy(OOOF(szParentPath), OOOF(szPath), szLastPath - OOOF(szPath));

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
	else
	{
		OOOError * pError = OOOConstruct(OOOError, "Path is not an absolute path: %s", OOOF(szPath));
		OOOICall(OOOF(iDirectoryCreateData), created, OOOCast(OOOIError, pError));
		OOODestroy(pError);
		OOODestroy(OOOThis);
	}
}
OOOMethodEnd

OOOConstructorPrivate(OOOFileSystem * pFileSystem, OOOIDirectoryCreateData * iDirectoryCreateData)
{
#define OOOInterface OOOIDirectoryCreateData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(created)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
	OOOMapMethodsEnd

	OOOF(pFileSystem) = pFileSystem;
	OOOF(iDirectoryCreateData) = iDirectoryCreateData;
}
OOOConstructorEnd
#undef OOOClass

#define OOOClass DirectoryRemoveData
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
	OOOIDirectoryRemoveData * iDirectoryRemoveData;
	char * szPath;
	o_dir * pDir;
	char * szChildPath;
	char * szInsert;
OOOPrivateDataEnd

OOODestructor
{
	if (OOOF(szChildPath))
	{
		O_free(OOOF(szChildPath));
	}
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
			if ((tStat.permissions & OTV_DIRECTORY_FLAG) == OTV_DIRECTORY_FLAG)
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
	OOOF(szPath) = OOOICall(OOOF(iDirectoryRemoveData), getPath);
	if (OOOF(szPath) && OOOF(szPath)[0] == '/')
	{
		OOOF(pDir) = O_dir_open(OOOF(szPath));
		if (OOOF(pDir))
		{
			OOOF(szChildPath) = O_calloc(O_strlen(OOOF(szPath)) + 1 + OTV_MAX_FILE_NAME_LENGTH + 1, sizeof(char));
			OOOF(szInsert) = OOOF(szChildPath) + O_strlen(OOOF(szPath)) + 1;
			O_strcpy(OOOF(szChildPath), OOOF(szPath));
			O_strcat(OOOF(szChildPath), "/");

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
	else
	{
		OOOError * pError = OOOConstruct(OOOError, "Path is not an absolute path: %s", OOOF(szPath));
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
	OOOF(iDirectoryRemoveData) = iDirectoryRemoveData;
}
OOOConstructorEnd
#undef OOOClass

#define OOOClass FileWriteData
OOODeclarePrivate(OOOFileSystem * pFileSystem, OOOIFileWriteData * iFileWriteData)
	OOOImplements
		OOOImplement(OOOIDirectoryCreateData)
	OOOImplementsEnd
	OOOExports
		OOOExport(void, create)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	OOOFileSystem * pFileSystem;
	OOOIFileWriteData * iFileWriteData;
	char * szPath;
	char * szParentPath;
OOOPrivateDataEnd

OOODestructor
{
	if (OOOF(szParentPath))
	{
		O_free(OOOF(szParentPath));
	}
}
OOODestructorEnd

OOOMethod(char *, getPath)
{
	return OOOF(szParentPath);
}
OOOMethodEnd

OOOMethod(void, writeFile)
{
	o_file hFile = O_file_open(OOOF(szPath), OTV_O_WRONLY | OTV_O_CREAT | OTV_O_TRUNC, OTV_WORLD_READ_WRITE_PERMISSION, UINT_MAX);
	if (hFile == FAILURE)
	{
		OOOError * pError = OOOConstruct(OOOError, "O_file_open failed: %s", OOOF(szPath));
		OOOICall(OOOF(iFileWriteData), written, OOOCast(OOOIError, pError));
		OOODestroy(pError);
		OOODestroy(OOOThis);
	}
	else
	{
		unsigned char * pData = OOOICall(OOOF(iFileWriteData), getData);
		size_t uSize = OOOICall(OOOF(iFileWriteData), getSize);
		if (uSize)
		{
			if (pData)
			{
				ssize_t nSize = O_fh_write(hFile, pData, uSize);
				if (nSize == FAILURE)
				{
					OOOError * pError = OOOConstruct(OOOError, "O_fh_write failed: %s", OOOF(szPath));
					O_fh_close(hFile);
					OOOICall(OOOF(iFileWriteData), written, OOOCast(OOOIError, pError));
					OOODestroy(pError);
					OOODestroy(OOOThis);
				}
				else
				{
					if ((size_t) nSize == uSize)
					{
						if (O_fh_close(hFile) == FAILURE)
						{
							OOOError * pError = OOOConstruct(OOOError, "O_fh_close failed: %s", OOOF(szPath));
							OOOICall(OOOF(iFileWriteData), written, OOOCast(OOOIError, pError));
							OOODestroy(pError);
							OOODestroy(OOOThis);
						}
						else
						{
							OOOICall(OOOF(iFileWriteData), written, NULL);
							OOODestroy(OOOThis);
						}
					}
					else
					{
						OOOError * pError = OOOConstruct(OOOError, "should have written %u bytes, actually wrote %d bytes: %s", uSize, nSize, OOOF(szPath));
						O_fh_close(hFile);
						OOOICall(OOOF(iFileWriteData), written, OOOCast(OOOIError, pError));
						OOODestroy(pError);
						OOODestroy(OOOThis);
					}
				}
			}
			else
			{
				OOOError * pError = OOOConstruct(OOOError, "Trying to write NULL data with size %u: %s", uSize, OOOF(szPath));
				O_fh_close(hFile);
				OOOICall(OOOF(iFileWriteData), written, OOOCast(OOOIError, pError));
				OOODestroy(pError);
				OOODestroy(OOOThis);
			}
		}
		else
		{
			if (O_fh_close(hFile) == FAILURE)
			{
				OOOError * pError = OOOConstruct(OOOError, "O_fh_close failed: %s", OOOF(szPath));
				OOOICall(OOOF(iFileWriteData), written, OOOCast(OOOIError, pError));
				OOODestroy(pError);
				OOODestroy(OOOThis);
			}
			else
			{
				OOOICall(OOOF(iFileWriteData), written, NULL);
				OOODestroy(OOOThis);
			}
		}
	}
}
OOOMethodEnd

OOOMethod(void, created, OOOIError * iError)
{
	if (iError)
	{
		OOOICall(OOOF(iFileWriteData), written, iError);
		OOODestroy(OOOThis);
	}
	else
	{
		OOOC(writeFile);
	}
}
OOOMethodEnd

OOOMethod(void, write)
{
	OOOF(szPath) = OOOICall(OOOF(iFileWriteData), getPath);
	if (OOOF(szPath) && OOOF(szPath)[0] == '/')
	{
		o_stat tStat;
		char * szLastPath = O_strrchr(OOOF(szPath), '/');
		OOOF(szParentPath) = O_calloc(szLastPath - OOOF(szPath) + 1, sizeof(char));
		O_strncpy(OOOF(szParentPath), OOOF(szPath), szLastPath - OOOF(szPath));

		if (O_file_get_stat(OOOF(szPath), &tStat) == SUCCESS)
		{
			if ((tStat.permissions & OTV_DIRECTORY_FLAG) == OTV_DIRECTORY_FLAG)
			{
				OOOError * pError = OOOConstruct(OOOError, "Path exists and is a directory: %s", OOOF(szPath));
				OOOICall(OOOF(iFileWriteData), written, OOOCast(OOOIError, pError));
				OOODestroy(pError);
				OOODestroy(OOOThis);
			}
			else
			{
				OOOICall(OOOCast(OOOIFileSystem, OOOF(pFileSystem)), createDirectory, OOOCast(OOOIDirectoryCreateData, OOOThis));
			}
		}
		else
		{
			OOOICall(OOOCast(OOOIFileSystem, OOOF(pFileSystem)), createDirectory, OOOCast(OOOIDirectoryCreateData, OOOThis));
		}
	}
	else
	{
		OOOError * pError = OOOConstruct(OOOError, "Path is not an absolute path: %s", OOOF(szPath));
		OOOICall(OOOF(iFileWriteData), written, OOOCast(OOOIError, pError));
		OOODestroy(pError);
		OOODestroy(OOOThis);
	}
}
OOOMethodEnd

OOOConstructorPrivate(OOOFileSystem * pFileSystem, OOOIFileWriteData * iFileWriteData)
{
#define OOOInterface OOOIDirectoryCreateData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(created)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
		OOOMapMethod(write)
	OOOMapMethodsEnd

	OOOF(pFileSystem) = pFileSystem;
	OOOF(iFileWriteData) = iFileWriteData;
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
	DirectoryCreateData * pDirectoryData = OOOConstruct(DirectoryCreateData, OOOThis, iDirectoryCreateData);
	OOOPCCall(DirectoryCreateData, pDirectoryData, create);
}
OOOMethodEnd

OOOMethod(void, removeDirectory, OOOIDirectoryRemoveData * iDirectoryRemoveData)
{
	DirectoryRemoveData * pDirectoryData = OOOConstruct(DirectoryRemoveData, OOOThis, iDirectoryRemoveData);
	OOOPCCall(DirectoryRemoveData, pDirectoryData, remove);
}
OOOMethodEnd

OOOMethod(void, writeFile, OOOIFileWriteData * iFileWriteData)
{
	FileWriteData * pFileData = OOOConstruct(FileWriteData, OOOThis, iFileWriteData);
	OOOPCCall(FileWriteData, pFileData, write);
}
OOOMethodEnd

OOOMethod(void, readFile, OOOIFileReadData * iFileReadData)
{
	char * szPath = OOOICall(iFileReadData, getPath);
	o_file hFile = O_file_open(szPath, OTV_O_RDONLY, OTV_O_NO_CREATION_MODE, 0);
	if (hFile == FAILURE)
	{
		OOOError * pError = OOOConstruct(OOOError, "O_file_open failed: %s", szPath);
		OOOICall(iFileReadData, read, OOOCast(OOOIError, pError), NULL, 0);
		OOODestroy(pError);
	}
	else
	{
		ssize_t nSize = O_fh_get_length(hFile);
		if (nSize == FAILURE)
		{
			OOOError * pError = OOOConstruct(OOOError, "O_fh_get_length failed: %s", szPath);
			O_fh_close(hFile);
			OOOICall(iFileReadData, read, OOOCast(OOOIError, pError), NULL, 0);
			OOODestroy(pError);
		}
		else
		{
			if (nSize > 0)
			{
				unsigned char * pData = O_calloc(nSize, sizeof(unsigned char));
				nSize = O_fh_read(hFile, pData, nSize);
				if (nSize == FAILURE)
				{
					OOOError * pError = OOOConstruct(OOOError, "O_fh_read failed: %s", szPath);
					O_fh_close(hFile);
					OOOICall(iFileReadData, read, OOOCast(OOOIError, pError), NULL, 0);
					OOODestroy(pError);
				}
				else
				{
					if (O_fh_close(hFile) == FAILURE)
					{
						OOOError * pError = OOOConstruct(OOOError, "O_fh_close failed: %s", szPath);
						OOOICall(iFileReadData, read, OOOCast(OOOIError, pError), pData, nSize);
						OOODestroy(pError);
					}
					else
					{
						OOOICall(iFileReadData, read, NULL, pData, nSize);
					}
				}
				O_free(pData);
			}
			else
			{
				if (O_fh_close(hFile) == FAILURE)
				{
					OOOError * pError = OOOConstruct(OOOError, "O_fh_close failed: %s", szPath);
					OOOICall(iFileReadData, read, OOOCast(OOOIError, pError), NULL, 0);
					OOODestroy(pError);
				}
				else
				{
					OOOICall(iFileReadData, read, NULL, NULL, 0);
				}
			}
		}
	}
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
