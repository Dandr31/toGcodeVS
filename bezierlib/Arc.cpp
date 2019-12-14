//
//  Arc.cpp
//  ofxCubicBezierToBiarc
//
//  Created by Carsten Høyer on 31/01/2019.
//

#include "Arc.h"
#include <cmath>
#include <iostream>

Arc::Arc()
{
    
}

Arc::Arc(DPoint _C, float _r, float _startAngle, float _sweepAngle, DPoint _P1, DPoint _P2, bool _cw)
{
    C = _C;
    r = _r;
    startAngle = _startAngle;
    sweepAngle = _sweepAngle;
    cw = _cw;
    P1 = _P1;
    P2 = _P2;
}

float Arc::Length()
{
    return r * abs(sweepAngle);
}

DPoint Arc::PointAt(float t)
{
    float x = C.m_x + r * cos(startAngle + t * sweepAngle);
    float y = C.m_y + r * sin(startAngle + t * sweepAngle);
    return DPoint(x, y);
}
void Arc::print(){
    std::cout<<P1.m_x<<"\t"
			<<P1.m_y<< "\t"
			<<P2.m_x<<"\t"
		    <<P2.m_y<<"\n";
}

// ofPath Arc::toPath()
// {
//     ofPath path;
//     auto a1 = startAngle * 180.0f / M_PI;
//     auto a2 = (sweepAngle * 180.0f / M_PI);
//     path.arc(C, r, r, a1, a1 + a2, cw);
//     return path;
// }

