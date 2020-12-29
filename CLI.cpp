#include "CLI.h"
#include "iostream"
#include "vector"
#include "AnomalyDetector.h"
CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    Data data = Data();
    HybridAnomalyDetector hybrid();
    Upload_command cammand1(dio,&data);
    Algorithem_setting_command cammand2(dio,&data);
    Anomaly_detection_command cammand3(dio,&data);
    Display_results_command cammand4(dio,&data);
    Analyze_results_command cammand5(dio,&data);
    Exit_command cammand6(dio,&data);

    cammands.push_back(*cammand1);
}

void CLI::start(){
    string menu_choice_str;
    float menu_choice =0;
    while (menu_choice!=6){
    dio->write("Welcome to the Anomaly Detection Server.");
    dio->write("Please choose an option:");
    for(int i=0;i<cammands.size(); i++){
        dio->write(cammands[i]->description);
    }
    menu_choice_str = dio->read();
    menu_choice = stof(menu_choice_str);
    cammands.at(menu_choice)->execute();
}
}


CLI::~CLI() {

    }
}

