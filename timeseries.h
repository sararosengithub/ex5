#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <fstream>
#include <vector>


using namespace std;

class TimeSeries{
private:
    std::vector<std::pair<std::string, std::vector<float>>> timeTable;
    int numOfColumns;
    int numOfRows;
public:

	TimeSeries(const char* CSVfileName);
    const std::vector<string> getColumnNames() const;
    void setColumn();
    void addLine();
	float getValue(int i, string j) const;
	const vector<float> getColumn(string columname) const;
    const vector<float>  getRow(int time) const;
    const int getNumOfRows() const;
    void addLine(const char* CSVfileName);
};



#endif /* TIMESERIES_H_ */
