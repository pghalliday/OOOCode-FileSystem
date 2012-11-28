#include "opentv.h"
#include "assert.h"
#include "OOODebugLog.h"
#include "OOOLogReporter.h"
#include "OOOUnitTestsRun.h"


void main(void)
{
	/* run the tests first */
	OOODebugLog * pLog = OOOConstruct(OOODebugLog);
	OOOLogReporter * pReporter = OOOConstruct(OOOLogReporter, OOOCast(OOOILog, pLog));
	OOOUnitTestsRun(OOOCast(OOOIReporter, pReporter));
	OOODestroy(pReporter);
	OOODestroy(pLog);

	while (TRUE) {
		// Do nothing
	}
}
