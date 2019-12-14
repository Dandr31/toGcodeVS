//
//  CubicBezier.h
//  ofxCubicBezierToBiarc
//
//  Created by Carsten Høyer on 31/01/2019.
//

#pragma once

#include "../dpoint.h"
#include <complex>
#include <tuple>
class CubicBezier {
public:
    CubicBezier();
    //CubicBezier(DPoint path);
    CubicBezier(DPoint p1, DPoint c1, DPoint c2, DPoint p2);
    DPoint P1;
    DPoint P2;
    DPoint C1;
    DPoint C2;
    tuple<complex<float>, complex<float>> InflexionPoints();
    tuple<CubicBezier, CubicBezier> Split(float t);
    DPoint PointAt(float t);
	const string toString() const;
    
private:
    void init(DPoint p1, DPoint c1, DPoint c2, DPoint p2);
};

