//
//  Arc.h
//  ofxCubicBezierToBiarc
//
//  Created by Carsten Høyer on 31/01/2019.
//

#ifndef ARC_H
#define ARC_H
#include "../dpoint.h"
class Arc {
    
public:
    Arc();
    Arc(DPoint _C, float _r, float _startAngle, float _sweepAngle, DPoint _P1, DPoint _P2, bool _cw);
    DPoint C;//center
    float r;//radius
    float startAngle;
    float sweepAngle;
    bool cw;
    DPoint P1;//start
    DPoint P2;//end
    float Length();
    DPoint PointAt(float t);
    void print();
    // ofPath toPath();
};
#endif