#include "asynPortDriver.h"
#include <epicsEvent.h>

#define P_RunString "SCAN_RUN" /* asynInt32,    r/w */
#define P_TimeTotalString "SCAN_TIME_TOTAL" /* asynFloat64,    r/w */
#define P_NumChannelsString "SCAN_NUM_CHANNELS" /* asynInt32,    r/w */
#define P_ScanResultsString "SCAN_RESULTS" /* asynFloat64ArrayIn,    r/w */
#define P_ScanIntervalString "SCAN_INTERVAL" /* asynFloat64,    r/w */
#define P_ScanCountString "SCAN_COUNT" /* asynInt32,    r/w */
#define P_ScanFuncString "SCAN_FUNC" /* asynInt32,    r/w */
#define P_ChanAResultsString "CHANA_RESULTS" /* asynInt32,    r/w */
#define P_ChanBResultsString "CHANB_RESULTS" /* asynInt32,    r/w */
#define P_ChanAFuncString "CHANA_FUNC" /* asynInt32,    r/w */
#define P_ChanBFuncString "CHANB_FUNC" /* asynInt32,    r/w */

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
	int P_ScanInterval;
	int P_NumChannels;
	int P_ScanResults;
	int P_ScanCount;
	int P_ScanFunc;
	int P_ChanAFunc;
	int P_ChanBFunc;
	int P_ChanAResults;
	int P_ChanBResults;
	//double time_total;
	asynStatus writeInt32(asynUser* pasynUser,epicsInt32 value);
	asynStatus writeFloat64(asynUser* pasynUser,epicsFloat64 value);
	asynStatus readFloat64Array(asynUser* pasynUser,epicsFloat64* value, size_t nElements, size_t *nIn);
	epicsEventId eventId_;
	asynStatus set_time_total();
	asynStatus set_chan_time_total(char*,int);
	asynStatus set_scan_interval();
	asynStatus set_scan_count();
	asynStatus set_num_channels();
	asynStatus get_results(double*, int*);
	int chan_handles[200];
	int chanfunc_handles[200];
	char func[200];
};
