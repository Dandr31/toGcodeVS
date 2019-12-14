//
//  Line.cpp
//  ofxCubicBezierToBiarc
//
//  Created by Carsten Høyer on 31/01/2019.
//

#include "Line.h"

Line::Line(DPoint _P1, DPoint _P2) : Line(_P1, Slope(_P1, _P2))
{
    
}

Line::Line(DPoint _P, float _m)
{
    P = _P;
    m = _m;
}

Line Line::CreatePerpendicularAt(DPoint P, DPoint P1)
{
    float m = Slope(P, P1);
    
    if (m == 0)
    {
        return Line(P, nanf("0"));
    }
    else if(isnan(m))
    {
        return Line(P, 0);
    }
    else
    {
        return Line(P, -1.0f / m);
    }
}

DPoint Line::Intersection(Line l)
{
    if(isnan(m) || m == 0)
    {
        return Line::VerticalIntersection((*this), l);
    }
    else if(isnan(l.m) || l.m == 0)
    {
        return Line::VerticalIntersection(l, (*this));
    }
    else
    {
        float x = (m * P.m_x- l.m * l.P.m_x- P.m_y + l.P.m_y) / (m - l.m);
        float y = m * x - m * P.m_x+ P.m_y;
        return DPoint(x, y);
    }
}

DPoint Line::VerticalIntersection(Line vl, Line l)
{
    float x = vl.P.m_x;
    float y = l.m * (x - l.P.m_x) + l.P.m_y;
    return DPoint(x, y);
}

float Line::Slope(DPoint P1, DPoint P2)
{
    if(P2.m_x== P1.m_x)
    {
        return nanf("0");
    }
    else
    {
        return (P2.m_y - P1.m_y) / (P2.m_x- P1.m_x);
    }
}
