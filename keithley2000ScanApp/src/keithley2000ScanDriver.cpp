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

const int MAX_CAPACITY=7000020;
const int REP_LEN=100;
const int BIG_REP_LEN=MAX_CAPACITY*5;
const int BUF_LEN=100;
const int NUM_CHANS=2;

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
	//run();
	sendCmd(rep,"*IDN?");
	//eventId_ = epicsEventCreate(epicsEventEmpty);
	createParam(P_RunString, asynParamInt32, &P_Run);
	createParam(P_TimeTotalString, asynParamFloat64, &P_TimeTotal);
	createParam(P_NumChannelsString, asynParamInt32, &P_NumChannels);
	createParam(P_ScanResultsString, asynParamFloat64Array, &P_ScanResults);
	createParam(P_ScanIntervalString, asynParamFloat64, &P_ScanInterval);
	createParam(P_ScanCountString, asynParamInt32, &P_ScanCount);
	createParam(P_ChanAResultsString, asynParamFloat64Array, &P_ChanAResults);
	createParam(P_ChanBResultsString, asynParamFloat64Array, &P_ChanBResults);
	createParam(P_ScanFuncString, asynParamInt32, &P_ScanFunc);
	createParam(P_ChanAFuncString, asynParamInt32, &P_ChanAFunc);
	createParam(P_ChanBFuncString, asynParamInt32, &P_ChanBFunc);
	createParam(P_ChanATimesString, asynParamInt32, &P_ChanATimes);
	createParam(P_ChanBTimesString, asynParamInt32, &P_ChanBTimes);
	createParam(P_ChanAEnableString, asynParamInt32, &P_ChanAEnable);
	createParam(P_ChanBEnableString, asynParamInt32, &P_ChanBEnable);
	createParam(P_ReadCurrString, asynParamFloat64, &P_ReadCurr);
	createParam(P_ReadVoltString, asynParamFloat64, &P_ReadVolt);
	createParam(P_TimestampString, asynParamFloat64, &P_Timestamp);
	createParam(P_TimestampIntString, asynParamInt32, &P_TimestampInt);
	createParam(P_TimestampFracString, asynParamFloat64, &P_TimestampFrac);
	createParam(P_ReadTypeString, asynParamInt32, &P_ReadType);
	createParam(P_SyncTypeString, asynParamInt32, &P_SyncType);
	
	chan_handles[0] = P_ChanAResults;
	chan_handles[1] = P_ChanBResults;

	chan_times_handles[0] = P_ChanATimes;
	chan_times_handles[1] = P_ChanBTimes;

	chan_enable_handles[0] = P_ChanAEnable;
	chan_enable_handles[1] = P_ChanBEnable;


	chanfunc_handles[0] = P_ChanAFunc;
	chanfunc_handles[1] = P_ChanBFunc;

	//run();
}

void Keithley2000ScanDriver::sendCmd(char* rep, char* buf){
	int len = BIG_REP_LEN;
	int timeout = 10;
	int eomReason;
	size_t nwrite;
	size_t got;
	pasynOctetSyncIO->writeRead( asynUserKscan_p_, buf, strlen(buf), rep, len, timeout, &nwrite, &got, &eomReason);


}

void Keithley2000ScanDriver::sendCmd(char* buf){
	int eomReason;
	size_t nwrite;
	size_t got;
	//char rep[REP_LEN];
	//sendCmd(rep, buf);

	pasynOctetSyncIO->write( asynUserKscan_p_, buf, strlen(buf), 1, &nwrite);
}

asynStatus Keithley2000ScanDriver::set_time_total(){
	char buf[BUF_LEN];
	char rep[REP_LEN];
	double time_total;
	int num_channels;
	getDoubleParam(P_TimeTotal, &time_total);
	getIntegerParam(P_NumChannels, &num_channels);
	sprintf(buf,":SENS:FUNC \"%s\", (@1:%d)",func,num_channels);
	sendCmd(buf);
	sprintf(buf,":SENS:%s:NPLC %lf, (@1:%d)",func,time_total,num_channels);
	sendCmd(buf);
	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::set_chan_time_total(char* chan_func, int chan_num){
	char buf[BUF_LEN];
	char rep[REP_LEN];
	double time_total;
	int num_channels;
	getDoubleParam(P_TimeTotal, &time_total);
	getIntegerParam(P_NumChannels, &num_channels);
	sprintf(buf,":SENS:FUNC \"%s\", (@%d:%d)",chan_func,chan_num,chan_num);
	sendCmd(buf);
	sprintf(buf,":SENS:%s:NPLC %lf, (@%d:%d)",chan_func,time_total,chan_num,chan_num);
	sendCmd(buf);
	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::set_scan_interval(){
	char buf[BUF_LEN];
	char rep[REP_LEN];
	double scan_interval;
	getDoubleParam(P_ScanInterval, &scan_interval);
	sprintf(buf,"ROUT:SCAN:INT %d",(int) scan_interval);
	sendCmd(buf);
	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::set_num_channels(){
	char buf[BUF_LEN];
	char rep[REP_LEN];
	int num_channels;
	getIntegerParam(P_NumChannels, &num_channels);
	sprintf(buf,":ROUT:SCAN:CRE (@1:%d)",num_channels);
	sendCmd(buf);
	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::set_scan_count(){
	char buf[BUF_LEN];
	char rep[REP_LEN];
	int scan_count;
	getIntegerParam(P_ScanCount, &scan_count);
	sprintf(buf,"ROUT:SCAN:COUNT:SCAN %d",scan_count);
	sendCmd(buf);
	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::get_results(double* results, int* num_results){
	char buf[BUF_LEN];
	static char rep[BIG_REP_LEN];
	char* pch;
	int num_channels;
	int scan_count;
	getIntegerParam(P_NumChannels, &num_channels);
	getIntegerParam(P_ScanCount, &scan_count);
	sprintf(buf,"TRAC:DATA? 1, %d, \"defbuffer1\", READ",num_channels*scan_count);
	sendCmd(rep, buf);

	int i = 0;

	pch = strtok(rep,",");
	while(pch != NULL){
		sscanf(pch,"%lf",results+i);
		pch = strtok(NULL,",");
		i++;
	}
	*num_results = i;


	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::get_times(double* results, int* num_results){
	char buf[BUF_LEN];
	static char rep[BIG_REP_LEN];
	char* pch;
	int num_channels;
	int scan_count;
	getIntegerParam(P_NumChannels, &num_channels);
	getIntegerParam(P_ScanCount, &scan_count);
	sprintf(buf,"TRAC:DATA? 1, %d, \"defbuffer1\", REL",num_channels*scan_count);
	sendCmd(rep, buf);

	int i = 0;

	pch = strtok(rep,",");
	while(pch != NULL){
		sscanf(pch,"%lf",results+i);
		pch = strtok(NULL,",");
		i++;
	}
	*num_results = i;


	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::writeInt32(asynUser* pasynUser,epicsInt32 value){
	char buf[BUF_LEN];
	if(pasynUser->reason == P_Run){
		//int time_total;
		//getIntegerParam(P_TimeTotal, &time_total);
		//set_time_total();
		run();
		//get_results();
	}
	if(pasynUser->reason == P_NumChannels){
		//int time_total;
		//getIntegerParam(P_TimeTotal, &time_total);
		setIntegerParam(P_NumChannels, value);

		set_num_channels();
	}
	if(pasynUser->reason == P_ScanCount){
		setIntegerParam(P_ScanCount, value);
		set_scan_count();
	}
	if(pasynUser->reason == P_ScanFunc){
		if(value == 0){
			strcpy(func,"VOLT");
		}
		else if(value == 1){
			strcpy(func,"CURR");
		}
		else if(value == 2){
			strcpy(func,"RES");
		}
		
	}
	
	for(int i = 0; i<NUM_CHANS; ++i){
		if(pasynUser->reason == chanfunc_handles[i]){
			char chan_func[200];
			if(value == 0){
				strcpy(chan_func,"VOLT");
			}
			else if(value == 1){
				strcpy(chan_func,"CURR");
			}
			else if(value == 2){
				strcpy(chan_func,"RES");
			}
			double results[REP_LEN];
			int num_results;
			int num_channels;
			set_chan_time_total(chan_func, i+1);

		}
	}
	if(pasynUser->reason == P_ReadType){
		setIntegerParam(P_ReadType, value);
	}
	if(pasynUser->reason == P_SyncType){
		setIntegerParam(P_SyncType, value);
	}
	return asynSuccess;
}


asynStatus Keithley2000ScanDriver::writeFloat64(asynUser* pasynUser,epicsFloat64 value){
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

asynStatus Keithley2000ScanDriver::update_timestamp(){
	char rep[REP_LEN];
	int timestamp_int;
	double timestamp_frac;
	sendCmd(rep,"FETC? \"defbuffer1\", SEC");
	sscanf(rep,"%d",&timestamp_int);
	sendCmd(rep,"FETC? \"defbuffer1\", FRAC");
	sscanf(rep,"%lf",&timestamp_frac);
	setIntegerParam(P_TimestampInt, timestamp_int);
	setDoubleParam(P_TimestampFrac, timestamp_frac);
}

asynStatus Keithley2000ScanDriver::read_by_type(char* func, epicsFloat64* result){
	char rep[REP_LEN];	
	char buf[BUF_LEN];
	int read_type;
	sprintf(buf,":SENS:FUNC \"%s\"",func);
	sendCmd(buf);
	sendCmd(rep, "READ?");
	sscanf(rep,"%lf",result);
	update_timestamp();
	return asynSuccess;

}


asynStatus Keithley2000ScanDriver::readFloat64(asynUser* pasynUser,epicsFloat64* value){
	if(pasynUser->reason == P_ReadCurr){
		//char rep[REP_LEN];	
		//char buf[BUF_LEN];
		//sendCmd(":SENS:FUNC \"CURR\"");
		//sendCmd(rep, "READ?");
		//sscanf(rep,"%lf",value);
		//update_timestamp();
		int read_type;
		int sync_type;
		getIntegerParam(P_ReadType, &read_type);
		getIntegerParam(P_SyncType, &sync_type);
		if(read_type==1 || sync_type == 1){
			read_by_type("CURR", value);
		}
	}
	if(pasynUser->reason == P_ReadVolt){
		//char rep[REP_LEN];	
		//char buf[BUF_LEN];
		//sendCmd(":SENS:FUNC \"VOLT\"");
		//sendCmd(rep, "READ?");
		//sscanf(rep,"%lf",value);
		//update_timestamp();

		int read_type;
		int sync_type;
		getIntegerParam(P_ReadType, &read_type);
		getIntegerParam(P_SyncType, &sync_type);
		if(read_type==0 || sync_type == 1){
			read_by_type("VOLT", value);
		}
	}
	if(pasynUser->reason == P_TimestampFrac){
		getDoubleParam(P_TimestampFrac, value);
	}
	callParamCallbacks();
	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::readInt32(asynUser* pasynUser,epicsInt32* value){
	if(pasynUser->reason == P_TimestampInt){
		getIntegerParam(P_TimestampInt, value);
	}
	
	callParamCallbacks();
	return asynSuccess;
}

asynStatus Keithley2000ScanDriver::readFloat64Array(asynUser* pasynUser,epicsFloat64* value, size_t nElements, size_t *nIn){

	if(pasynUser->reason == P_ScanResults){
		//value[0] = 8;
		//value[1] = 5;
		//*nIn = 2;
		double results[REP_LEN];
		int num_results;
		int num_channels;
		get_results(results, &num_results);
		getIntegerParam(P_NumChannels, &num_channels);
		for(int i = 0; i<num_results; ++i){
			value[i] = results[i];

		}
		*nIn = num_results;
		
	}
	for(int i = 0; i<NUM_CHANS; ++i){
		if(pasynUser->reason == chan_handles[i]){
			static double results[BIG_REP_LEN];
			int num_results;
			int num_channels;
			get_results(results, &num_results);
			getIntegerParam(P_NumChannels, &num_channels);
			num_results = num_results/num_channels;
			for(int j = 0; j<num_results; ++j){
				value[j] = results[j*num_channels + i];

			}
			*nIn = num_results;
		}
	}
	for(int i = 0; i<NUM_CHANS; ++i){
		if(pasynUser->reason == chan_times_handles[i]){
			double results[REP_LEN];
			int num_results;
			int num_channels;
			get_times(results, &num_results);
			getIntegerParam(P_NumChannels, &num_channels);
			num_results = num_results/num_channels;
			for(int j = 0; j<num_results; ++j){
				value[j] = results[j*num_channels + i];

			}
			*nIn = num_results;
		}
	}
			
	return asynSuccess;
}


void Keithley2000ScanDriver::run(){
	char buf[BUF_LEN];
	sendCmd("INIT");
}

extern "C" {

static const iocshArg * const testinitArgs[] = {};
static const iocshArg initArg0 = {"portName",iocshArgString};
static const iocshArg initArg1 = {"IOPortName",iocshArgString};
static const iocshArg initArg2 = {"max Array Size",iocshArgInt};
static const iocshArg * const initArgs[] = {&initArg0, &initArg1, &initArg2};
static const iocshFuncDef testinitFuncDef = {"test",0, testinitArgs};
static const iocshFuncDef initFuncDef = {"keithley2000ScanDriverConfigure",3, initArgs};


static void initCallFunc(const iocshArgBuf* args){
	new Keithley2000ScanDriver(args[0].sval, args[1].sval, args[2].ival);
}


static void keithley2000ScanDriverRegister(){
	iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(keithley2000ScanDriverRegister);

}
