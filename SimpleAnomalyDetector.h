

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include "timeseries.h"
#include "minCircle.h"
//float CORRLATION = 0.9;
struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	Circle *circle;// = Circle(Point(0,0),0);
	correlatedFeatures(string f1,string f2,float cor){
        feature1 = f1;feature2 = f2;corrlation = cor;
	}
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
	vector<correlatedFeatures> cf;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();

	void learnNormal(const TimeSeries& ts);
	vector<AnomalyReport> detect(const TimeSeries& ts);
    bool is_anomaly(Point *p,correlatedFeatures c);
	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

    static correlatedFeatures createcorrelatedfeature(Point **p, int size1, correlatedFeatures c);
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
