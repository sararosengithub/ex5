
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub
    correlation_threshold = 0.9;

}
bool HybridAnomalyDetector::is_anomaly(Point *p, correlatedFeatures c) {
    if (c.corrlation < correlation_threshold && c.corrlation > 0.5) {
        if (!point_in_circle(*p, *(c.circle))) { return true; } else return false;
    } else return SimpleAnomalyDetector::is_anomaly(p, c);
}
correlatedFeatures HybridAnomalyDetector::createcorrelatedfeature(Point **p, int size1, correlatedFeatures c){
    if (c.corrlation < correlation_threshold && c.corrlation > 0.5){
        Circle minCircle =findMinCircle(p,size1);
        Circle minCircle2 = Circle(minCircle.center,minCircle.radius*1.1);
        c.circle = &minCircle2;
        return c;
    }else return SimpleAnomalyDetector::createcorrelatedfeature(p,size1,c);
}
float HybridAnomalyDetector::get_correlation_threshold(){return correlation_threshold;}
void HybridAnomalyDetector::set_correlation_threshold(float f){
    correlation_threshold = f;
}
HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

