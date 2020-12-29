//20702836  31597142
// Created by Sara Rosen id: 207028366 and Miriam Zuckerbrot id: 315971424
//
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include "math.h"
#include "vector"
#include "random"
#include "algorithm"
#include "anomaly_detection_util.h"
using namespace std;

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};
float dist(Point p1, Point p2);
Circle from_one_point(Point p1);
Circle from_two_points(Point p1, Point p2);
Circle from_three_points(Point p1, Point p2, Point p3);
bool point_in_circle(Point p, Circle c) ;
Circle Welzls(const std::vector<Point*> &p, std::vector<Point> &r) ;
Circle findMinCircle(Point** points,size_t size);
#endif /* MINCIRCLE_H_ */
