#include <iostream>
#include "keithley2000ScanDriver.h"
#include <iocsh.h>
#include <epicsExport.h>
#include <epicsString.h>
#include <asynOctetSyncIO.h>
#include <errlog.h>
//#include <cstdlib>
//#include <cstdio>
#include <cstring>

using namespace std;

const int REP_LEN=200;

Keithley2000ScanDriver::Keithley2000ScanDriver(const char* portName, const char* IOPortName, int maxArraySize)
  : asynPortDriver(portName, maxArraySize, 100, 
		  asynInt32Mask  | asynFloat64Mask | asynFloat64ArrayMask | asynDrvUserMask,
		  asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask ,
		  0,
		  1,
		  0,
		  0)
{
	char rep[REP_LEN];
	cout << portName << endl;
	cout << maxArraySize << endl;
	pasynOctetSyncIO->connect(IOPortName, 0, &asynUserKscan_p_, NULL);

	char junk[100];
	size_t got_junk;
	int eomReason;
	pasynOctetSyncIO->read(asynUserKscan_p_, junk, sizeof(junk), 2.0, &got_junk, &eomReason);
	if ( got_junk ) {
		epicsPrintf("Keithley2000Scan(%s): WARNING - detected unexpected characters on link (%s); make sure you have a RAW (not TELNET) connection\n", junk, IOPortName);
	}

	pasynOctetSyncIO->setInputEos ( asynUserKscan_p_, "\n", 1 );
	pasynOctetSyncIO->setOutputEos( asynUserKscan_p_, "\n", 1 );
	char* buf = "READ?";
	int len = sizeof(rep);
	int timeout = 10;
	size_t nwrite;
	size_t got;
	pasynOctetSyncIO->writeRead( asynUserKscan_p_, buf, strlen(buf), rep, len, timeout, &nwrite, &got, &eomReason);
	cout << "result:" <<rep << endl;
	//run();
	sendCmd(rep,"*IDN?");
	//eventId_ = epicsEventCreate(epicsEventEmpty);
	createParam(P_RunString, asynParamInt32, &P_Run);
	createParam(P_TimeTotalString, asynParamFloat64, &P_TimeTotal);
	createParam(P_NumChannelsString, asynParamInt32, &P_NumChannels);
	createParam(P_ScanResultsString, asynParamInt32, &P_ScanResults);
	createParam(P_ScanIntervalString, asynParamFloat64, &P_ScanInterval);
	createParam(P_ScanCountString, asynParamInt32, &P_ScanCount);
	//run();
}

void Keithley2000ScanDriver::sendCmd(char* rep, char* buf){
	//char* buf = "READ?";
	//char rep[200];
	int len = REP_LEN;
	int timeout = 1;
	int eomReason;
	size_t nwrite;
	size_t got;
	pasynOctetSyncIO->writeRead( asynUserKscan_p_, buf, strlen(buf), rep, len, timeout, &nwrite, &got, &eomReason);
	cout << "result:" <<len <<rep << endl;


}

void Keithley2000ScanDriver::sendCmd(char* buf){
	char rep[200];
	sendCmd(rep, buf);
}

asynStatus Keithley2000ScanDriver::set_time_total(){
	char buf[200];
	char rep[200];
	double time_total;
	int num_channels;
	getDoubleParam(P_TimeTotal, &time_total);
	getIntegerParam(P_NumChannels, &num_channels);
	sprintf(buf,":SENS:FUNC \"VOLT\", (@1:%d)",num_channels);
	sendCmd(rep, buf);
	sprintf(buf,":SENS:VOLT:NPLC %lf, (@1:%d)",time_total,num_channels);
	cout << buf;
	sendCmd(rep, buf);
	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::set_scan_interval(){
	char buf[200];
	char rep[200];
	double scan_interval;
	getDoubleParam(P_ScanInterval, &scan_interval);
	//sprintf(buf,":SENS:FUNC \"VOLT\", (@1:%d)",num_channels);
	//sendCmd(rep, buf);
	sprintf(buf,"ROUT:SCAN:MEAS:INT %lf",scan_interval);
	cout << buf;
	sendCmd(rep, buf);
	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::set_num_channels(){
	char buf[200];
	char rep[200];
	int num_channels;
	getIntegerParam(P_NumChannels, &num_channels);
	sprintf(buf,":ROUT:SCAN:CRE (@1:%d)",num_channels);
	cout << buf;
	sendCmd(rep, buf);
	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::set_scan_count(){
	char buf[200];
	char rep[200];
	int scan_count;
	getIntegerParam(P_ScanCount, &scan_count);
	sprintf(buf,"ROUT:SCAN:COUNT:SCAN %d",scan_count);
	cout << buf;
	sendCmd(rep, buf);
	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::get_results(double* results){
	char buf[200];
	char rep[200];
	char* pch;
	int num_channels;
	getIntegerParam(P_NumChannels, &num_channels);
	sprintf(buf,"TRAC:DATA? 1, %d, \"defbuffer1\", READ",num_channels);
	sendCmd(rep, buf);
	cout << rep << endl;

	int i = 0;

	pch = strtok(NULL,",");
	while(pch != NULL){
		sscanf(pch,"%lf",results+i);
		pch = strtok(NULL,",");
	}


	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::writeInt32(asynUser* pasynUser,epicsInt32 value){
	cout << "asdfasdfas";
	char buf[200];
	if(pasynUser->reason == P_Run){
		//int time_total;
		//getIntegerParam(P_TimeTotal, &time_total);
		//cout << time_total;
		set_time_total();
		set_num_channels();
		//run();
		//get_results();
	}
	if(pasynUser->reason == P_NumChannels){
		//int time_total;
		//getIntegerParam(P_TimeTotal, &time_total);
		//cout << time_total;
		setIntegerParam(P_NumChannels, value);
	}
	if(pasynUser->reason == P_ScanCount){
		setDoubleParam(P_ScanCount, value);
		set_scan_count();
	}
	return asynSuccess;
}


asynStatus Keithley2000ScanDriver::writeFloat64(asynUser* pasynUser,epicsFloat64 value){
	cout << "Write Float";
	if(pasynUser->reason == P_TimeTotal){
		setDoubleParam(P_TimeTotal, value);
	}
	if(pasynUser->reason == P_ScanInterval){
		setDoubleParam(P_ScanInterval, value);
		set_scan_interval();
	}
	callParamCallbacks();
	return asynSuccess;
}


asynStatus Keithley2000ScanDriver::readFloat64Array(asynUser* pasynUser,epicsFloat64* value, size_t nElements, size_t *nIn){
	//cout << "Array";

	if(pasynUser->reason == P_ScanResults){
		//cout << value[0] << " " <<value[1] << endl;
		value[0] = 8;
		value[1] = 5;
		*nIn = 2;
		
	}
	return asynSuccess;
}


void Keithley2000ScanDriver::run(){
	char buf[200];
	sendCmd(buf,"INIT");
}

extern "C" {

void test(){
	cout << "asdfg\n";
}


static const iocshArg * const testinitArgs[] = {};
static const iocshArg initArg0 = {"portName",iocshArgString};
static const iocshArg initArg1 = {"IOPortName",iocshArgString};
static const iocshArg initArg2 = {"max Array Size",iocshArgInt};
static const iocshArg * const initArgs[] = {&initArg0, &initArg1, &initArg2};
static const iocshFuncDef testinitFuncDef = {"test",0, testinitArgs};
static const iocshFuncDef initFuncDef = {"keithley2000ScanDriverConfigure",3, initArgs};

static void testinitCallFunc(const iocshArgBuf* args){
	test();
}

static void initCallFunc(const iocshArgBuf* args){
	new Keithley2000ScanDriver(args[0].sval, args[1].sval, args[2].ival);
}

static void testRegister(){
	iocshRegister(&testinitFuncDef,testinitCallFunc);
}

static void keithley2000ScanDriverRegister(){
	iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(testRegister);
epicsExportRegistrar(keithley2000ScanDriverRegister);

}
