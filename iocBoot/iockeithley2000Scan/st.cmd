#!../../bin/linux-x86_64/keithley2000Scan

## You may have to change keithley2000Scan to something else
## everywhere it appears in this file

#< envPaths

## Register all support components
dbLoadDatabase("../../dbd/keithley2000Scan.dbd",0,0)
dbLoadDatabase("../../dbd/keithley2000ScanDriver.dbd",0,0)
keithley2000Scan_registerRecordDeviceDriver(pdbbase) 
keithley2000ScanDriver_registerRecordDeviceDriver(pdbbase) 

## Load record instances
dbLoadRecords("../../db/keithley2000Scan.db","user=bsobhani,Sys=XF:28IDC-ES,Dev={KDMM6500}")
dbLoadRecords("../../db/keithley2000ScanChannel.db","user=bsobhani,Sys=XF:28IDC-ES,Dev={KDMM6500}")


drvAsynIPPortConfigure("P0","192.168.56.7:5025")

keithley2000ScanDriverConfigure("kscan","P0",1)

asynSetTraceFile("kscan",-1,"")
asynSetTraceMask("kscan",-1,0x09)
asynSetTraceIOMask("kscan",-1,0x2)



iocInit()

## Start any sequence programs
#seq snckeithley2000Scan,"user=bsobhani"

#dbpf TimeTotal .2
#dbpf NumChannels 2
#dbpf ScanInterval 1
#dbpf ScanCount 2
#dbpf ScanFunc 0

