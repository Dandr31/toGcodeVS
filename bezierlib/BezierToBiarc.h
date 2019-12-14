//
//  Algorithm.h
//  BezierToBiarc
//
//  Created by Carsten Høyer on 31/01/2019.
//

#pragma once

#include "../dpoint.h"
#include "CubicBezier.h"
#include <complex>
#include <tuple>
#include <stack> 
#include "BiArc.h"
typedef std::vector<BiArc> BiarcVector;
class BezierToBiarc {
public:
    BezierToBiarc();
	BiarcVector ApproxCubicBezier(CubicBezier bezier, float samplingStep, float tolerance);
    
private:
    static bool IsRealInflexionPoint(complex<float> t);
};
