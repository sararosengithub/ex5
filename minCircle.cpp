#include <iostream>
#include "math.h"
#include "vector"
#include "random"
#include "algorithm"
#include "minCircle.h"
float dist(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

Circle from_one_point(Point p1) { return Circle( p1,0); }

Circle from_two_points(Point p1, Point p2) {
    Point center = Point(((p1.x + p2.x) / 2), (p1.y + p2.y) / 2);
    float radius = (dist(p1, p2)) / 2;
    return Circle(center,radius);
}

Circle from_three_points(Point p1, Point p2, Point p3) {
    float ax = pow(p1.x, 2);
    float bx = pow(p2.x, 2);
    float cx = pow(p3.x, 2);
    float ay = pow(p1.y, 2);
    float by = pow(p2.y, 2);
    float cy = pow(p3.y, 2);
    float f;
    float g;
    float c;
    float r;

    f = ((ax - cx) * (p1.x - p2.x) + (ay - cy) * (p1.x - p2.x) + (bx - ax) * (p1.x - p3.x) + (by - ay) * (p1.x - p3.x))
        / (2 * ((p3.y - p1.y) * (p1.x - p2.x) - (p2.y - p1.y) * (p1.x - p3.x)));

    g = ((ax - cx) * (p1.y - p2.y) + (ay - cy) * (p1.y - p2.y) + (bx - ax) * (p1.y - p3.y) + (by - ay) * (p1.y - p3.y))
        / (2 * ((p3.x - p1.x) * (p1.y - p2.y) - (p2.x - p1.x) * (p1.y - p3.y)));

    c = -ax - ay - 2 * g * p1.x - 2 * f * p1.y;

    r = sqrt(pow(g, 2) + pow(f, 2) - c);

    return Circle( Point(-g, -f),r);
}

bool point_in_circle(Point p, Circle c) {
    return (dist(p, c.center) <= c.radius);
}

Circle Welzls(const std::vector<Point*> &p, std::vector<Point> &r) {
    std::vector<Point*> p_copy = p;
    int sizeofr = r.size();
    int sizeofp = p.size();
    if (sizeofp == 0 || sizeofr == 4) {
        switch (sizeofr) {
            case (0):
                //this point will be ignored because its not being put into r.
                return from_one_point(Point(0, 0));
                break;
            case (1):
                return from_one_point(r[0]);
                break;
            case (2):
                return from_two_points(r[0], r[1]);
                break;
            case (3):
                if (point_in_circle(r[2], from_two_points(r[0], r[1]))) {
                    r.erase(r.begin() + 2);
                    return from_two_points(r[0], r[1]);
                }
                else if (point_in_circle(r[0], from_two_points(r[2], r[1]))) {
                    r.erase(r.begin());
                    return from_two_points(r[0], r[1]);
                }
                else if (point_in_circle(r[1], from_two_points(r[0], r[2]))) {
                    r.erase(r.begin() + 1);
                    return from_two_points(r[0], r[1]);
                }
                else return from_three_points(r[0], r[1], r[2]);
                break;

            case (4):
                for (int i = 0; i < 3; i++) {
                    Circle c = from_two_points(r[i], r[3]);
                    if ((point_in_circle(r[((i + 1) % 3)], c) )&& (point_in_circle(r[((i + 2) % 3)], c))) {
                        r.erase(r.begin() + ((i + 1) % 3));
                        r.erase(r.begin() + ((i + 1) % 3));//i+2%3 now moved over by one.
                        return c;
                    }
                }
                for (int i = 0; i < 3; i++) {
                    Circle c = from_three_points(r[i], r[(i + 1) % 3], r[3]);
                    if (point_in_circle(r[(i + 2) % 3], c)) {
                        r.erase(r.begin() + ((i + 2) % 3));
                        return c;
                    }

                }
                break;

            default:
                cout<<"error"<<endl;
        }
    }
    Point p_rand = *(*p_copy.begin());
    std::vector<Point> r_copy = r;
    p_copy.erase(p_copy.begin());
    Circle D = Welzls(p_copy, r_copy);
    if (point_in_circle(p_rand, D)) {
        r = r_copy;
        return D; }
    else{r.push_back(p_rand);
        return Welzls(p_copy,  r);}

}
Circle findMinCircle(Point** points,size_t size){
    vector<Point*> p;
    for(int i=0; i<size; i++){
        p.push_back(points[i]);
    }
    random_shuffle(p.begin(), p.end());
    vector<Point> r ={};
    return Welzls(p,r);
}