//
//  Algorithm.cpp
//  BezierToBiarc
//
//  Created by Carsten Høyer on 31/01/2019.
//

#include "BezierToBiarc.h"



BezierToBiarc::BezierToBiarc() {
    
}

BiarcVector  BezierToBiarc::ApproxCubicBezier(CubicBezier bezier, float samplingStep, float tolerance) {
    vector<BiArc> biarcs;
    stack<CubicBezier> curves;
    
    // ---------------------------------------------------------------------------
    // First, calculate the inflexion points and split the bezier at them (if any)
    
    CubicBezier toSplit = bezier;
    auto inflex = toSplit.InflexionPoints();
    bool i1 = IsRealInflexionPoint(get<0>(inflex));
    bool i2 = IsRealInflexionPoint(get<1>(inflex));
    
    if (i1 && !i2)
    {
        auto splited = toSplit.Split(real(get<0>(inflex)));
        curves.push(get<1>(splited));
        curves.push(get<0>(splited));
    }
    else if (!i1 && i2)
    {
        auto splited = toSplit.Split(real(get<1>(inflex)));
        curves.push(get<1>(splited));
        curves.push(get<0>(splited));
    }
    else if (i1 && i2)
    {
        float t1 = real(get<0>(inflex));
        float t2 = real(get<1>(inflex));
        
        // I'm not sure if I need, but it does not hurt to order them
        if (t1 > t2)
        {
            auto tmp = t1;
            t1 = t2;
            t2 = tmp;
        }
        
        // Make the first split and save the first new curve. The second one has to be splitted again
        // at the recalculated t2 (it is on a new curve)
        
        auto splited1 = toSplit.Split(t1);
        
        t2 = (1 - t1) * t2;
        
        toSplit = get<1>(splited1);
        auto splited2 = toSplit.Split(t2);
        
        curves.push(get<1>(splited2));
        curves.push(get<0>(splited2));
        curves.push(get<0>(splited1));
    }
    else {
        curves.push(toSplit);
    }
    
    // ---------------------------------------------------------------------------
    // Second, approximate the curves until we run out of them
    while (curves.size() > 0)
    {
        CubicBezier bezier = curves.top();
        curves.pop();
        
        // ---------------------------------------------------------------------------
        // Calculate the transition DPoint for the BiArc
        // V: Intersection DPoint of tangent lines
        Line T1 = Line(bezier.P1, bezier.C1);
        Line T2 = Line(bezier.P2, bezier.C2);
        
        // I think this is a hack for an underlying problem.
        // If the lines are parallel on the x axis, we cannot get the incenter, because we cannot create
        // a triangle. So in order to avoid this, we rotate the tangent just a little bit.
        if (isnan(T1.m)) {
            DPoint skewedPoint1 = DPoint(bezier.C1.m_x - 0.001, bezier.C1.m_y);
            T1 = Line(bezier.P1, skewedPoint1);
        }
        if (isnan(T2.m)) {
            DPoint skewedPoint2 = DPoint(bezier.C2.m_x - 0.001, bezier.C2.m_y);
            T2 = Line(bezier.P2, skewedPoint2);
        }
        DPoint V = T1.Intersection(T2);
        
        // G: incenter DPoint of the triangle (P1, V, P2)
        // http://www.mathopenref.com/coordincenter.html
        float dP2V = bezier.P2.distance(V);
        float dP1V = bezier.P1.distance(V);
        float dP1P2 = bezier.P1.distance(bezier.P2);
        DPoint G = (dP2V * bezier.P1 + dP1V * bezier.P2 + dP1P2 * V) / (dP2V + dP1V + dP1P2);
        
        // ---------------------------------------------------------------------------
        // Calculate the BiArc
        
        BiArc biarc = BiArc(bezier.P1, (bezier.P1 - bezier.C1), bezier.P2, (bezier.P2 - bezier.C2), G);
        
        // ---------------------------------------------------------------------------
        // Calculate the maximum error
        
        float maxDistance = 0.0f;
        float maxDistanceAt = 0.0f;
        
        auto nrPointsToCheck = biarc.Length() / samplingStep;
        auto parameterStep = 1.0f / nrPointsToCheck;
        
        for (int i = 0; i <= nrPointsToCheck; i++)
        {
            //cout << "check: " << i << "\n";
            float t = parameterStep * i;
            DPoint u1 = biarc.PointAt(t);
            DPoint u2 = bezier.PointAt(t);
            float distance = (u1 - u2).length();
            
            if (distance > maxDistance)
            {
                maxDistance = distance;
                maxDistanceAt = t;
            }
        }

        // Check if the two curves are close enough
        if (maxDistance > tolerance)
        {
            // If not, split the bezier curve the DPoint where the distance is the maximum
            // and try again with the two halfs
            auto bs = bezier.Split(maxDistanceAt);
            curves.push(get<1>(bs));
            curves.push(get<0>(bs));
        }
        else
        {
            // Otherwise we are done with the current bezier
            biarcs.push_back(biarc);
        }
    }

	return biarcs;
}
                             
bool BezierToBiarc::IsRealInflexionPoint(complex<float> t)
{
    return imag(t) == 0 && real(t) > 0 && real(t) < 1;
}
