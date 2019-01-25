#include "asynPortDriver.h"
#include <epicsEvent.h>

#define P_RunString "SCAN_RUN" /* asynInt32,    r/w */
#define P_TimeTotalString "SCAN_TIME_TOTAL" /* asynFloat64,    r/w */
#define P_NumChannelsString "SCAN_NUM_CHANNELS" /* asynInt32,    r/w */
#define P_ScanResultsString "SCAN_RESULTS" /* asynFloat64ArrayIn,    r/w */

class Keithley2000ScanDriver : public asynPortDriver{
public:
	int x;
	Keithley2000ScanDriver(const char* portName, const char* IOPortName, int maxArraySize);
	void sendCmd(char*,char*);
	void sendCmd(char*);
	void run();
	asynUser *asynUserKscan_p_;
	int P_Run;
	int P_TimeTotal;
	int P_NumChannels;
	int P_ScanResults;
	//double time_total;
	asynStatus writeInt32(asynUser* pasynUser,epicsInt32 value);
	asynStatus writeFloat64(asynUser* pasynUser,epicsFloat64 value);
	asynStatus readFloat64Array(asynUser* pasynUser,epicsFloat64* value, size_t nElements, size_t *nIn);
	epicsEventId eventId_;
	asynStatus set_time_total();
	asynStatus set_num_channels();
	asynStatus get_results(double*);
};
