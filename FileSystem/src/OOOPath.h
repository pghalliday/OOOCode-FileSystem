#ifndef OOOPath_H
#define OOOPath_H

#include "OOOCode.h"

#define OOOClass OOOPath
OOODeclare(OOOPath * pParent, char * szPath)
	OOOImplements
	OOOImplementsEnd
	OOOExports
		OOOExport(char *, toString)
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
