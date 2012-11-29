#ifndef OOOPath_H
#define OOOPath_H

#include "OOOCode.h"

#define OOOClass OOOPath
OOODeclare(OOOPath * pParent, char * szPath)
	OOOImplements
	OOOImplementsEnd
	OOOExports
		OOOExport(char *, getAbsolutePath)
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
