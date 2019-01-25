TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = keithley2000Scan
# keithley2000Scan.dbd will be created and installed
DBD += keithley2000Scan.dbd
DBD += keithley2000ScanDriver.dbd
#DBD += testRegister.dbd

# keithley2000Scan.dbd will be made up from these files:
keithley2000Scan_DBD += base.dbd
keithley2000Scan_DBD += asyn.dbd
keithley2000Scan_DBD += drvAsynIPPort.dbd

# Include dbd files from all support applications:
#keithley2000Scan_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#keithley2000Scan_LIBS += xxx

# keithley2000Scan_registerRecordDeviceDriver.cpp derives from keithley2000Scan.dbd
keithley2000Scan_SRCS += keithley2000Scan_registerRecordDeviceDriver.cpp
keithley2000Scan_SRCS += keithley2000ScanDriver_registerRecordDeviceDriver.cpp
keithley2000Scan_SRCS += keithley2000ScanDriver.cpp

# Build the main IOC entry point on workstation OSs.
keithley2000Scan_SRCS_DEFAULT += keithley2000ScanMain.cpp
keithley2000Scan_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#keithley2000Scan_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
keithley2000Scan_LIBS += asyn
keithley2000Scan_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

