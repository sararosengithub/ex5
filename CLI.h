
#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"
#include "timeseries.h"
#include "AnomalyDetector.h"
using namespace std;
struct Data{
    TimeSeries* train_ts;
    TimeSeries* test_ts;
    HybridAnomalyDetector detector;
    vector<AnomalyReport> anomalyreports;
    int line_count_test;
    Data(){HybridAnomalyDetector hybrid();}
};
class CLI {
	DefaultIO* dio;
	vector<Command*> cammands;
	// you can add data members
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
