#include "OOOPath.h"

#define OOOClass OOOPath

OOOPrivateData
	char * szPath;
OOOPrivateDataEnd

OOODestructor
{
	O_free(OOOF(szPath));
}
OOODestructorEnd

OOOMethod(char *, toString)
{
	return OOOF(szPath);
}
OOOMethodEnd

OOOConstructor(OOOPath * pParent, char * szPath)
{
	OOOMapMethods
		OOOMapMethod(toString)
	OOOMapMethodsEnd

	if (szPath[0] == '/')
	{
		/* szPath is an absolute path */
		OOOF(szPath) = O_strdup(szPath);
	}
	else
	{
		/* szPath is a relative path */
		if (pParent)
		{
			/* szPath is relative to the given parent directory */
			char * szParent = OOOCall(pParent, toString);
			OOOF(szPath) = O_calloc(O_strlen(szParent) + 1 + O_strlen(szPath) + 1, sizeof(char));
			O_strcpy(OOOF(szPath), szParent);
		}
		else
		{
			/* szPath is relative to the current directory */
			char * szCurrentDirectory = O_dir_get_path(NULL, 0);
			OOOF(szPath) = O_calloc(O_strlen(szCurrentDirectory) + 1 + O_strlen(szPath) + 1, sizeof(char));
			O_strcpy(OOOF(szPath), szCurrentDirectory);
			O_free(szCurrentDirectory);
		}
		O_strcat(OOOF(szPath), "/");
		O_strcat(OOOF(szPath), szPath);
	}
}
OOOConstructorEnd

#undef OOOClass
