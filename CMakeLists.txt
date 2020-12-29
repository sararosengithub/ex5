cmake_minimum_required(VERSION 3.17)
project(ex5)

set(CMAKE_CXX_STANDARD 14)

add_executable(ex5 mainTrain.cpp anomaly_detection_util.cpp SimpleAnomalyDetector.cpp timeseries.cpp minCircle.cpp HybridAnomalyDetector.cpp CLI.cpp anomaly_detection_command.cpp upload_command.cpp)