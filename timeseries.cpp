#include <string>
#include <algorithm>
#include "fstream"
#include "timeseries.h"
#include "sstream"
#include "vector"

TimeSeries::TimeSeries(const char *CSVfileName) {
    string line, word, name;
    int idx = 0;
    float value;
    numOfRows = 0;
    numOfColumns = 0;
    std::ifstream csvFile(CSVfileName);
    if (!csvFile.is_open()) { throw "File is not open!"; }
//read column names from csv file and insert in table:
    if (csvFile.good()) {
        std::getline(csvFile, line);
        std::stringstream ss(line);
        while (std::getline(ss, name, ',')) {
            this->timeTable.push_back({name, std::vector<float>{}});
            numOfColumns++;
        }
    }
    //read data from csv file and insert in table:
    while (std::getline(csvFile, line)) {
        numOfRows++;
        idx = 0;
        std::stringstream ss(line);
        while (ss >> value) {
            this->timeTable.at(idx).second.push_back(value);
            idx++;
            if (ss.peek()== ','){
                ss.ignore();
            }
        }
    }
    csvFile.close();
}

const std::vector<string> TimeSeries::getColumnNames() const {
    std::vector<string> columnNames;
    for (int i = 0; i < this->timeTable.size(); i++) {
        columnNames.push_back(timeTable.at(i).first);
    }
    return columnNames;
}

void setColumn() {
}

void TimeSeries::addLine(const char *CSVfileName) {
    std::ifstream csvFile(CSVfileName);
    if (!csvFile.is_open()) { throw "File is not open!"; }
    int idx = 0;
    float value = 0;
    string line;
    //read data from csv file and insert in table:
    while (std::getline(csvFile, line)) {
        this->numOfRows++;
        std::stringstream ss(line);
        while (ss >> value) {
            this->timeTable.at(idx).second.push_back(value);
            idx++;
        }
    }
}

//returnes the value of element j at time i:
float TimeSeries::getValue(int i, string j) const {
    std::vector<string> columNames = getColumnNames();
    std::vector<string>::iterator itr = std::find(columNames.begin(), columNames.end(), j);
    return this->timeTable[itr - columNames.begin()].second.at(i);
}

const vector<float> TimeSeries::getColumn(string columname) const {
    std::vector<string> columNames = getColumnNames();
    std::vector<string>::iterator itr = std::find(columNames.begin(), columNames.end(), columname);
    return this->timeTable[itr - columNames.begin()].second;
}

const vector<float> TimeSeries::getRow(int time) const {
    vector<float> row;
    for (int i = 0; i < numOfColumns; i++) {
        row.push_back(timeTable.at(i).second[time]);
    }
    return row;
}

const int TimeSeries::getNumOfRows() const {
    return numOfRows;
}