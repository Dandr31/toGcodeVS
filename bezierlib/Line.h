//
//  Line.h
//  ofxCubicBezierToBiarc
//
//  Created by Carsten Høyer on 31/01/2019.
//

#pragma once

#include "../dpoint.h"
class Line {
public:
    Line(DPoint _P, float _m);
    Line(DPoint P1, DPoint P2);
    DPoint P;
    float m;
    static Line CreatePerpendicularAt(DPoint P, DPoint P1);
    DPoint Intersection(Line l);

private:
    static float Slope(DPoint P1, DPoint P2);
    static DPoint VerticalIntersection(Line vl, Line l);
};
