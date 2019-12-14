//
//  CubicBezier.cpp
//  ofxCubicBezierToBiarc
//
//  Created by Carsten Høyer on 31/01/2019.
//

#include "CubicBezier.h"
#include <math.h>

CubicBezier::CubicBezier()
{
    
}

//CubicBezier::CubicBezier(ofPath path)
//{
//    auto commands = path.getCommands();
//    DPoint P1 = (DPoint) commands[0].to;
//    DPoint C1 = (DPoint) commands[1].cp1;
//    DPoint C2 = (DPoint) commands[1].cp2;
//    DPoint P2 = (DPoint) commands[1].to;
//    
//    init(P1, C1, C2, P2);
//}

CubicBezier::CubicBezier(DPoint p1, DPoint c1, DPoint c2, DPoint p2)
{
    init(p1, c1, c2, p2);
}

void CubicBezier::init(DPoint p1, DPoint c1, DPoint c2, DPoint p2)
{
    P1 = p1;
    C1 = c1;
    C2 = c2;
    P2 = p2;
}

tuple<complex<float>, complex<float>> CubicBezier::InflexionPoints() {
    DPoint A = C1 - P1;
    DPoint B = C2 - C1 - A;
    DPoint C = P2 - C2 - A - 2 * B;
    
    complex<float> a(B.m_x * C.m_y - B.m_y * C.m_x, 0);
    complex<float> b(A.m_x * C.m_y - A.m_y * C.m_x, 0);
    complex<float> c(A.m_x * B.m_y - A.m_y * B.m_x, 0);
    
    auto t1 = (-b + sqrt(b * b - 4.0f * a * c)) / (2.0f * a);
    auto t2 = (-b - sqrt(b * b - 4.0f * a * c)) / (2.0f * a);
    
    return tuple<complex<float>, complex<float>>(t1, t2);
}

tuple<CubicBezier, CubicBezier> CubicBezier::Split(float t)
{
    auto p0 = P1 + t * (C1 - P1);
    auto p1 = C1 + t * (C2 - C1);
    auto p2 = C2 + t * (P2 - C2);
    
    auto p01 = p0 + t * (p1 - p0);
    auto p12 = p1 + t * (p2 - p1);
    
    auto dp = p01 + t * (p12 - p01);
    
    return tuple<CubicBezier, CubicBezier>(CubicBezier(P1, p0, p01, dp), CubicBezier(dp, p12, p2, P2));
}

DPoint CubicBezier::PointAt(float t)
{
    return (float) pow(1 - t, 3) * P1 +
        (float)(3 * pow(1 - t, 2) * t) * C1 +
        (float)(3 * (1 - t) * pow(t, 2)) * C2 +
        (float) pow(t, 3) * P2;
}
const string CubicBezier::toString() const
{
	stringstream s;
	s << "cubic" << " " << P1.m_x << " " << P1.m_y << "  "
		<< " " << C1.m_x << " " << C1.m_y << "  "
		<< " " << C2.m_x << " " << C2.m_y << "  "
		<< " " << P2.m_x << " " << P2.m_y << "  ";
		
	return s.str();
}