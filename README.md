OOOCode-FileSystem
==================

OOOCode FileSystem interfaces and classes for working with files and directories.

## OOOPath

- Should be possible to construct from a fully qualified path string
- Should be possible to construct from an existing OOOPath instance and a relative path string
- Should be possible to construct from a relative path string and generate a path relative to the current directory
- Should expose the fully qualified path as a string

### Roadmap

- Nothing yet

## OOOIFileSystem interface

- Should support creating directories asynchronously
- Should support removing directories asynchronously
- Should support writing files asynchronously
- Should support reading files asynchronously
- Should support removing files asynchronously

### Roadmap

- Nothing yet

## OOOFileSystem

A simple file system implementation exposing the STB filesystem

- Should implement the OOOIFileSystem interface
- Should create parent directories on file creation
- Should create parent directories on directory creation
- Should recursively remove directories
- Should only support absolute paths

### Roadmap

- Nothing yet

## Roadmap

- Should implement an OOOCheckedFileSystem that will append CRCs to files so that they can be checked for corruption
- Should expose directory listings
- Should allow checks for file or dircetory existance

## API

### To use OOOPath

```C
#include "OOOPath.h"

OOOPath * pPath = OOOConstruct(OOOPath, NULL, "/this/is/a/path");
OOOPath * pAnotherPath = OOOConstruct(OOOPath, pPath, "as/well");
OOOPath * pRelativePath = OOOConstruct(OOOPath, NULL, "relative");

/* szPath will contain "/this/is/a/path" */
char * szPath = OOOCall(pPath, getAbsolutePath);

/* szPath will contain "/this/is/a/path/as/well" */
char * szPath = OOOCall(pAnotherPath, getAbsolutePath);

/* szPath will contain "/[CURRENT_DIRCETORY]/relative" */
char * szPath = OOOCall(pRelativePath, getAbsolutePath);

OOODestroy(pPath);
OOODestroy(pAnotherPath);
OOODestroy(pRelativePath);

```

### To implement an OOOIFileSystem

MyFileSystem.h

```C
#ifndef MyFileSystem_H
#define MyFileSystem_H

#include "OOOIFileSystem.h"

#define OOOClass MyFileSystem
OOODeclare()
    OOOImplements
        OOOImplement(OOOIFileSystem)
    OOOImplementsEnd
    OOOExports
    OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
```

MyFileSystem.c

```C
#include "MyFileSystem.h"

#define OOOClass MyFileSystem

OOOPrivateData
OOOPrivateDataEnd

OOODestructor
{
}
OOODestructorEnd

OOOMethod(void, createDirectory, OOOIDirectoryCreateData * iDirectoryCreateData)
{
    char * szPath = OOOICall(iDirectoryCreateData, getPath);

    /* TODO: create the directory at the given path */

    /* Notify the OOOIDirectoryCreateData instance that creation is complete */
    OOOICall(iDirectoryCreateData, created, NULL);
}
OOOMethodEnd

OOOMethod(void, removeDirectory, OOOIDirectoryRemoveData * iDirectoryRemoveData)
{
    char * szPath = OOOICall(iDirectoryRemoveData, getPath);

    /* TODO: create the directory at the given path */

    /* Notify the OOOIDirectoryRemoveData instance that creation is complete */
    OOOICall(iDirectoryRemoveData, removed, NULL);
}
OOOMethodEnd

OOOMethod(void, writeFile, OOOIFileWriteData * iFileWriteData)
{
    char * szPath = OOOICall(iFileWriteData, getPath);
    unsigned char * pData = OOOICall(iFileWriteData, getData);
    size_t uSize = OOOICall(iFileWriteData, getSize);

    /* TODO: write the file to the given path */

    /* Notify the OOOIFileWriteData instance that writing is complete */
    OOOICall(iFileWriteData, written, NULL);
}
OOOMethodEnd

OOOMethod(void, readFile, OOOIFileReadData * iFileReadData)
{
    unsigned char * pData;
    size_t uSize;
    char * szPath = OOOICall(iFileReadData, getPath);

    /* TODO: read the file at the given path */

    /* Notify the OOOIFileReadData instance that reading is complete */
    OOOICall(iFileReadData, read, NULL, pData, uSize);
}
OOOMethodEnd

OOOMethod(void, removeFile, OOOIFileRemoveData * iFileRemoveData)
{
    char * szPath = OOOICall(iFileWriteData, getPath);

    /* TODO: remove the file at the given path */

    /* Notify the OOOIFileRemoveData instance that removal is complete */
    OOOICall(iFileRemoveData, removed, NULL);
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
}
OOOConstructorEnd

#undef OOOClass
```

### To use OOOFileSystem

```C
#include "OOOFileSystem.h"

OOOFileSystem * pFileSystem;

/* Declare private data classes */

#define OOOClass MyDirectory
OOODeclarePrivate(char * szPath)
    OOOImplements
        OOOImplement(OOOIDirectoryCreateData)
        OOOImplement(OOOIDirectoryRemoveData)
    OOOImplementsEnd
    OOOExports
    OOOExportsEnd
OOODeclareEnd
#undef

#define OOOClass MyFile
OOODeclarePrivate(char * szPath, unsigned char * pData, size_t uSize)
    OOOImplements
        OOOImplement(OOOIFileWriteData)
        OOOImplement(OOOIFileReadData)
        OOOImplement(OOOIFileRemoveData)
    OOOImplementsEnd
    OOOExports
    OOOExportsEnd
OOODeclareEnd
#undef

static OOOFileSystem * pFileSystem;
static MyDirectory * pMyDirectory;
static MyFile * pMyFile;

static void start()
{
    pFileSystem = OOOConstruct(OOOFileSystem);
    pDirectoryPath = OOOConstruct(OOOPath, NULL, "/my/directory");
    pMyDirectory = OOOConstruct(MyDirectory, OOOCall(pDirectoryPath, getAbsolutePath));
    OOODestroy(pDirectoryPath);
    OOOICall(OOOCast(OOOIFileSystem, pFileSystem), createDirectory, OOOCast(OOOIDirectoryCreateData, pMyDirectory));
}

static void directoryCreated(OOOIError * iError)
{
    assert(iError == NULL);
    pFilePath = OOOConstruct(OOOPath, pDirectoryPath, "my/file");
    pMyFile = OOOConstruct(MyFile, OOOCall(pFilePath, getAbsolutePath), (unsigned char *) "Hello, World!", O_strlen("Hello, World!") + 1);
    OOODestroy(pFilePath);
    OOOICall(OOOCast(OOOIFileSystem, pFileSystem), writeFile, OOOCast(OOOIFileWriteData, pMyFile));
}

static void fileWritten(OOOIError * iError)
{
    assert(iError == NULL);
    OOOICall(OOOCast(OOOIFileSystem, pFileSystem), readFile, OOOCast(OOOIFileReadData, pMyFile));
}

static void fileRead(OOOIError * iError, unsigned char * pData, size_t uSize)
{
    assert(iError == NULL);
    assert(O_strcmp((char *) pData, "Hello, World!") == 0);    assert(uSize == O_strlen("Hello, World!") + 1);
    OOOICall(OOOCast(OOOIFileSystem, pFileSystem), removeFile, OOOCast(OOOIFileRemoveData, pMyFile));
}

static void fileRemoved(OOOIError * iError)
{
    // file should have been deleted but the directory "/my/directory/my" will remain
    assert(iError == NULL);
    OOODestroy(pMyFile);
    OOOICall(OOOCast(OOOIFileSystem, pFileSystem), removeDirectory, OOOCast(OOOIDirectoryRemoveData, pMyDirectory));
}

static void directoryRemoved(OOOIError * iError)
{
    // directory should have been deleted but the directory "/my" will remain
    assert(iError == NULL);
    OOODestroy(pMyDirectory);
    OOODestroy(pFileSystem);
}

/* Implement the private data classes */

#define OOOClass MyDirectory
OOOPrivateData
    char * szPath;
OOOPrivateDataEnd

OOODestructor
OOODestructorEnd

OOOMethod(char *, getPath)
    return OOOF(szPath);
OOOMethodEnd

OOOMethod(void, created, OOOIError * iError)
    directoryCreated(iError);
OOOMethodEnd

OOOMethod(void, removed, OOOIError * iError)
    directoryRemoved(iError);
OOOMethodEnd

OOOConstructorPrivate(char * szPath)
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
    OOOMapMethodsEnd

    OOOF(szPath) = szPath;
OOOConstructorEnd
#undef OOOClass

#define OOOClass MyFile
OOOPrivateData
    char * szPath;
    unsigned char * pData;
    size_t uSize;
OOOPrivateDataEnd

OOODestructor
OOODestructorEnd

OOOMethod(char *, getPath)
    return OOOF(szPath);
OOOMethodEnd

OOOMethod(unsigned char *, getData)
    return OOOF(pData);
OOOMethodEnd

OOOMethod(size_t, getSize)
    return OOOF(uSize);
OOOMethodEnd

OOOMethod(void, written, OOOIError * iError)
    fileWritten(iError);
OOOMethodEnd

OOOMethod(void, read, OOOIError * iError, unsigned char * pData, size_t uSize)
    fileRemoved(iError, pData, uSize);
OOOMethodEnd

OOOMethod(void, removed, OOOIError * iError)
    fileRemoved(iError);
OOOMethodEnd

OOOConstructorPrivate(char * szPath, unsigned char * pData, size_t uSize)
#define OOOInterface OOOIFileWriteData
    OOOMapVirtuals
        OOOMapVirtual(getPath)
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
    OOOMapMethodsEnd

    OOOF(szPath) = szPath;
    OOOF(pData) = pData;
    OOOF(uSize) = uSize;
OOOConstructorEnd
#undef OOOClass
```

## Contributing

In lieu of a formal styleguide, take care to maintain the existing coding style. Add unit tests for any new or changed functionality.

## Prerequisites

- OpenTV SDK C2.2 from http://community.opentv.com

## Initialising

This project references sub-modules so after cloning the repository...

```
git submodule init
git submodule update
```

Then the easiest way to work with the project in the OpenTV IDE...

1. Create a new workspace in the root of the project
1. In the IDE choose **File/Import...**
1. In the resulting dialog choose **General/Existing Projects Into Workspace** and press **Next**
1. Choose the **Select Root Directory:** Radio button and **Browse...** to the root of the project
1. A number of projects may be listed under **Projects**
1. Select the projects in the root, deselect those under the submodules folder and then click **Finish**
1. You should then be able to build the OpenTV projects by choosing **Project/Build All**

After you have built the projects at least once run the test application in the Virtual Set top box by...

1. Choosing the **FileSystemFlow** project on the left
1. Pressing the green **Run** button at the top
1. In the resulting dialog choose **Local OpenTV Application** and press **OK**

## License
Copyright (c) 2012 Peter Halliday  
Licensed under the MIT license.