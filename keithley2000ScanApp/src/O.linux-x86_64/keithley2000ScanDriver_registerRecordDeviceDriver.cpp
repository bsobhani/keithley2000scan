/* THIS IS A GENERATED FILE. DO NOT EDIT! */
/* Generated from ../O.Common/keithley2000ScanDriver.dbd */

#include <string.h>

#include "epicsStdlib.h"
#include "iocsh.h"
#include "iocshRegisterCommon.h"
#include "registryCommon.h"

extern "C" {

epicsShareExtern void (*pvar_func_testRegister)(void);
epicsShareExtern void (*pvar_func_keithley2000ScanDriverRegister)(void);

int keithley2000ScanDriver_registerRecordDeviceDriver(DBBASE *pbase)
{
    const char *bldTop = "/home/bsobhani/iocs/keithley2000scan";
    const char *envTop = getenv("TOP");

    if (envTop && strcmp(envTop, bldTop)) {
        printf("Warning: IOC is booting with TOP = \"%s\"\n"
               "          but was built with TOP = \"%s\"\n",
               envTop, bldTop);
    }

    if (!pbase) {
        printf("pdbbase is NULL; you must load a DBD file first.\n");
        return -1;
    }

    (*pvar_func_testRegister)();
    (*pvar_func_keithley2000ScanDriverRegister)();
    return 0;
}

/* registerRecordDeviceDriver */
static const iocshArg registerRecordDeviceDriverArg0 =
                                            {"pdbbase",iocshArgPdbbase};
static const iocshArg *registerRecordDeviceDriverArgs[1] =
                                            {&registerRecordDeviceDriverArg0};
static const iocshFuncDef registerRecordDeviceDriverFuncDef =
                {"keithley2000ScanDriver_registerRecordDeviceDriver",1,registerRecordDeviceDriverArgs};
static void registerRecordDeviceDriverCallFunc(const iocshArgBuf *)
{
    keithley2000ScanDriver_registerRecordDeviceDriver(*iocshPpdbbase);
}

} // extern "C"
/*
 * Register commands on application startup
 */
static int Registration() {
    iocshRegisterCommon();
    iocshRegister(&registerRecordDeviceDriverFuncDef,
        registerRecordDeviceDriverCallFunc);
    return 0;
}

static int done = Registration();
