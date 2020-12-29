//
// Created by Sara Rosen on 11/3/2020.
//

#include "anomaly_detection_util.h"
#include <cmath>
float average(float *x, int size)
{
    float sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += x[i];
    }
    return (sum / size);
}

float var(float *x, int size)
{
    float var = 0;
    float avr = average(x, size);
    for (int i = 0; i < size; i++)
    {
        var += pow((x[i] - avr), 2);
    }
    return var / size;
}

float cov(float *x, float *y, int size)
{
    float cov = 0;
    float xAvrage = average(x, size);
    float yAvrage = average(y, size);
    for (int i = 0; i < size; i++)
    {
        cov += ((x[i] - xAvrage) * (y[i] - yAvrage));
    }
    cov /= size;
    return cov;
}

float pearson(float *x, float *y, int size)
{
    return cov(x, y, size) / ((sqrt(var(x, size)) * (sqrt(var(y, size)))));
}

Line linear_reg(Point **points, int size)
{
    float *arrX = new float[size];
    float *arrY = new float[size];
    for (int i = 0; i < size; i++)
    {
        arrX[i] = points[i]->x;
        arrY[i] = points[i]->y;
    }
    float a = cov(arrX, arrY, size) / var(arrX, size);
    float b = average(arrY, size) - (a * average(arrX, size));
    return Line(a, b);
}
float dev(Point p, Point **points, int size)
{
    Line line = linear_reg(points, size);
    return dev(p, line);
}

float dev(Point p, Line l)
{
    return std::abs(l.f(p.x)- p.y);
}