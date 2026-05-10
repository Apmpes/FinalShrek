#pragma once
#include "InducedMagField.h"
Vec2D toMathCoords(Vec2D v) { //since windos forms treats 0 ,0 as top left, which messes with angles
    return Vec2D(v.getX(), -v.getY());
}
double computeDivergence(vector<Charge>& Q, vector<inducMag>& mag, float x, float y) {
    float h = 0.5f; // small step

    Vec2D Ex1 = toMathCoords(displayedTotEat(Q, mag, x + h, y));
    Vec2D Ex2 = toMathCoords(displayedTotEat(Q, mag, x - h, y));

    Vec2D Ey1 = toMathCoords(displayedTotEat(Q, mag, x, y + h));
    Vec2D Ey2 = toMathCoords(displayedTotEat(Q, mag, x, y - h));

    double dEx_dx = (Ex1.getX() - Ex2.getX()) / (2 * h);
    double dEy_dy = (Ey1.getY() - Ey2.getY()) / (2 * h); //i inverted it since in windows forms y is upsidown which adds a neg sign. 

    return dEx_dx + dEy_dy;
}

double computeCurl(vector<Charge>& Q, vector<inducMag>& mag, float x, float y) {
    float h = 0.5f;

    Vec2D Ex1 = toMathCoords(displayedTotEat(Q, mag, x + h, y));
    Vec2D Ex2 = toMathCoords(displayedTotEat(Q, mag, x - h, y));

    Vec2D Ey1 = toMathCoords(displayedTotEat(Q, mag, x, y + h));
    Vec2D Ey2 = toMathCoords(displayedTotEat(Q, mag, x, y - h));

    double dEy_dx = (Ex1.getY() - Ex2.getY()) / (2 * h);//also inverted
    double dEx_dy = (Ey1.getX() - Ey2.getX()) / (2 * h);

    return dEy_dx - dEx_dy;
}

Vec2D computeGradMag(vector<Charge>& Q, vector<inducMag>& mag, float x, float y) {
    float h = 1.0f;

    double Em_x1 = Emag(toMathCoords(displayedTotEat(Q, mag, x + h, y)));
    double Em_x2 = Emag(toMathCoords(displayedTotEat(Q, mag, x - h, y)));

    double Em_y1 = Emag(toMathCoords(displayedTotEat(Q, mag, x, y + h)));
    double Em_y2 = Emag(toMathCoords(displayedTotEat(Q, mag, x, y - h)));

    double dE_dx = (Em_x1 - Em_x2) / (2 * h); //also inverted here
    double dE_dy = (Em_y1 - Em_y2) / (2 * h);

    return Vec2D(dE_dx, dE_dy);
}
double toDeg(double theta) {
    double pi = 3.14159265;
    return (180 / pi) * theta;
}
double EdirInDeg(Vec2D E) {
    double theta = atan2(E.getY(), E.getX());
    return toDeg(theta); //the neg is because since y is at top, it was giving me neg angle
}

double testQF(Charge& testQ, vector<Charge>& Q, vector<inducMag>& mag, float x, float y) {
    double fmag = Emag(toMathCoords(displayedTotEat(Q, mag, x, y))) * testQ.getQ();
    return fmag;
}
double gradEmagDir(Vec2D E) {
    double theta = atan2(E.getY(), E.getX());
    return toDeg(theta); //same neg reason as before
}
double switchCoords(double y, int screenHeight) {
    y = screenHeight - y;
    return y;
}
