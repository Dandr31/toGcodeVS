//
//  BiArc.hpp
//  ofxCubicBezierToBiarc
//
//  Created by Carsten Høyer on 31/01/2019.
//

#pragma once

#include "../dpoint.h"
#include "Line.h"
#include "Arc.h"
#include <tuple>
class BiArc {
public:
    BiArc(DPoint P1, DPoint T1, DPoint P2, DPoint T2, DPoint T);
    Arc A1;
    Arc A2;
    float Length();
    DPoint PointAt(float t);
    /*static vector<tuple<ofPath, ofPath>> toPaths(vector<BiArc> biarcs);*/
};
