

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include "CLI.h"
#include "sstream"

using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}

    // you may add additional methods here
};
class StandardIO: public DefaultIO{
    string StandardIO::read(){
        string string1;
        cin>>string1;
        return string1;
    }

    void StandardIO::write(string text){
        cout<<text<<endl;
    }

    void StandardIO::write(float f){
        cout<<f<<endl;
    }

    void StandardIO::read(float *f){

    }
};
// you may add here helper classes


// you may edit this class
class Command {
protected:
    DefaultIO *dio;
public:
    string description;
    Data *data;

    Command(DefaultIO *dio, Data *data) : dio(dio), data(data) {}

    virtual void execute() = 0;

    virtual ~Command() {}
};

// implement here your command classes
class Upload_command : public Command {
public:
    Upload_command(DefaultIO *dio, Data *data)
            : Command(dio, data) {
        description = "1. upload a time series csv file";
    }

    void execute() {
        //upload train csv:
        ofstream trainFile("traindata.csv");
        dio->write("Please upload your local train CSV file.");
        string line = "";
        do {
            line = dio->read();
            trainFile << line << endl;
        } while (line != "done");
        trainFile.close();
        dio->write("Upload complete.");
        TimeSeries traintimeSeries("traindata.csv");
        data->train_ts = &traintimeSeries;

        //upload test csv:
        ofstream testFile("testdata.csv");
        dio->write("Please upload your local test CSV file.");
        data->line_count_test = 0;
        do {
            line = dio->read();
            testFile << line << endl;
            data->line_count_test++;
        } while (line != "done");
        testFile.close();
        dio->write("Upload complete.");
        TimeSeries testtimeSeries("testdata.csv");
        data->test_ts = &testtimeSeries;
    }
};

class Algorithem_setting_command : public Command {
public:
    Algorithem_setting_command(DefaultIO *dio, Data *data)
            : Command(dio, data) {
        description = "2. algorithm settings";
    }

    void execute() {
        float correlation_threshold = data->detector.get_correlation_threshold();
        dio->write("The current correlation threshold is " + to_string(correlation_threshold) + '\n' +
                   "Type a new threshold");
        correlation_threshold = stof(dio->read());
        data->detector.set_correlation_threshold(correlation_threshold);
    }
};

class Anomaly_detection_command : public Command {
public:
    Anomaly_detection_command(DefaultIO *dio, Data *data)
            : Command(dio, data) {
        description = "3. detect anomalies";
    }

    void execute() {
        data->detector.learnNormal(*data->train_ts);
        data->anomalyreports = data->detector.detect(*data->test_ts);
        dio->write("anomaly detection complete.");
    }
};

class Display_results_command : public Command {
public:
    Display_results_command(DefaultIO *dio, Data *data)
            : Command(dio, data) {
        description = "4. display results";
    }

    void execute() {
        for (int i = 0; i < data->anomalyreports.size(); i++) {
            dio->write(to_string(data->anomalyreports[i].timeStep) +
                       '\t' + data->anomalyreports[i].description);
        }
        dio->write("Done.");
    }
};

class Analyze_results_command : public Command {
public:
    Analyze_results_command(DefaultIO *dio, Data *data)
            : Command(dio, data) {
        description = "5. upload anomalies and analyze results";
    }

    void execute() {
        //create vector that holds start and end time of each anamoly given by user
        vector<pair<long, long>> anomaly_instances_reported;
        int positive_steps = 0;
        string line;
        long time;
        do {
            line = dio->read();
            stringstream ss(line);
            vector<string> times;
            string intermediate;
            while (getline(ss, intermediate, ',')) {
                times.push_back(intermediate);
            }
            anomaly_instances_reported.push_back(make_pair(stol(times.at(0)),
                                                           stol(times.at(1))));
            positive_steps += stol(times.at(1)) - stol(times.at(0)) + 1;
            times.clear();
        } while (line != "done");
        /*
        //get anomalys from user:
        string filename = dio->read();
        ofstream anomaliesFile(filename);
        string line;
        do{
            line =dio->read();
            anomaliesFile<<line;
        }while (line!="done");
        anomaliesFile.close();
        */
        //devide anomaly reports into chuncks of time:
        vector<pair<long, long>> anomaly_instances;
        string anamoly_description = data->anomalyreports[0].description;
        long timestart = data->anomalyreports[0].timeStep;
        long timestep = data->anomalyreports[0].timeStep;
        int i = 1;
        // for (int i = 1;i<data->anomalyreports.size();i++){
        do {
            while ((data->anomalyreports[i].description == anamoly_description) &&
                   (data->anomalyreports[i].timeStep == timestep + 1)) {
                timestep++;
            }
            anomaly_instances.push_back(make_pair(timestart, timestep));
            timestart = data->anomalyreports[i].timeStep;
            timestep = timestart;
            i++;
        } while (i < data->anomalyreports.size());
        //calculate p and n:
        float P = anomaly_instances.size();
        float N = data->line_count_test - positive_steps;
        float TP = 0, FP = 0;
        bool flag = 0;
        //iterate over instances in anomaly_instance and check if they overlap with anomaly_instance_reported.
        for (int i = 0; i < anomaly_instances.size(); i++) {
            flag = 0;
            for (int j = 0; j < anomaly_instances_reported.size(); j++) {
                //if left side of a is bigger then right side of b
                if (anomaly_instances.at(i).first > anomaly_instances_reported.at(j).second) {
                    break;
                }
                    //if right side of a is smaller then left side of b
                else if (anomaly_instances[i].second < anomaly_instances_reported[j].first) {
                    break;
                }
                    //option1 - left side of a is in range b
                else if (anomaly_instances[i].first > anomaly_instances_reported[j].first &&
                         anomaly_instances[i].first < anomaly_instances_reported[j].second) {
                    TP++;
                    break;
                }//option2 - right side of a is in range b
                        else if (anomaly_instances[i].second < anomaly_instances_reported[j].second &&
                                 anomaly_instances[i].second > anomaly_instances_reported[j].first) {
                    flag = 1;
                    break;
                }//option3 - b is in range a
                        else if (anomaly_instances[i].first < anomaly_instances_reported[j].first &&
                                 anomaly_instances[i].second > anomaly_instances_reported[j].second) {
                    flag = 1;
                    break;
                }
            }
            if (flag) TP++;
            else FP++;
        }float true_positive_rate = TP/P;
        float false_alarm_rate = FP/N;
        dio->write("True Positive Rate: "+to_string(true_positive_rate));
        dio->write("False Positive Rate: "+to_string(false_alarm_rate));

    }
};

class Exit_command : public Command {
public:
    Exit_command(DefaultIO *dio, Data *data)
            : Command(dio, data) {
        description = "6. exit";
    }
};

#endif /* COMMANDS_H_ */
