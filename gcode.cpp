#include "gcode.h"
#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
using namespace std;

namespace Gcode{	
	//Base class
	GcodeCommand::GcodeCommand(float _x, float _y,float _z) : x(_x), y(_y),z(_z) {}

	

	//Move 
	GcodeMoveTo::GcodeMoveTo(float x, float y,float z) : GcodeCommand(x, y,z) {}

	const string GcodeMoveTo::toString() const {
		stringstream s;
		s <<"move "<< x << "," << y<<","<<z;
		return s.str();;
	}
	const string GcodeMoveTo::toGcode() const {
		stringstream s;
		s << "G00" << " " << "X" << x << " " << "Y" << y << " " << "Z" << z;
		return s.str();
	}
	GcodeCommandType GcodeMoveTo::getType(){
		return GCODE_MOVE_TO;
	}	

	//Line
	GcodeLineTo::GcodeLineTo(float x, float y,float z) : GcodeCommand(x, y, z) {}
	const string GcodeLineTo::toString() const {
		stringstream s;
		s <<"line "<< x << "," << y << "," << z;
		return s.str();;
	}
	const string GcodeLineTo::toGcode() const {
		stringstream s;
		s << "G01" << " " << "X" << x << " " << "Y" << y << " " << "Z" << z;
		return s.str();
	}
	GcodeCommandType GcodeLineTo::getType() {
		return GCODE_LINE_TO;
	}

	//Arc
	GcodeArcTo::GcodeArcTo(float rx, float ry,float x0,float y0, bool cw, float x, float y,float z) : GcodeCommand(x, y,z) {
		this->rx = rx;
		this->ry = ry;
		this->cw = cw;
		this->x0 = x0;
		this->y0 = y0;
	}
	const string GcodeArcTo::toString() const {
		stringstream s;
		s<<"arc "<< "center :  "<< rx << "," << ry <<
			"  start  "<<x0 <<","<<y0<< 
			"  isCw  "<<cw  << 
			"  end Point  " << x << "," <<y<<","<<z;
		return s.str();
	}
	const string GcodeArcTo::toGcode() const {
		stringstream s;
		/*G03 X181.1383 Y88.2407 I1.0938 J0.3359
	    G02 X182.7720 Y86.1443 I - 3.3832 J - 4.3212*/
		float i = rx - x0;
		float j = ry - y0;

		//float i = x0 - rx;
		//float j = y0 - rx;
		if (!cw) {
			s << "G02" <<" "<<"X"<< x <<" "<<"Y"<<y<<" "<<"I"<<i<<" "<<"J"<<j;
		}else {
			s << "G03" <<" "<<"X"<< x <<" "<<"Y"<<y<<" "<<"I"<<i<<" "<< "J" <<j;
		}
		return s.str();
	}

	
	GcodeCommandType GcodeArcTo::getType(){
		return GCODE_ARC_TO;
	}
	GcodeComment::GcodeComment(string s):GcodeCommand(0, 0, 0){
		this->str = s;
	}
	const string GcodeComment::toString() const
	{
		stringstream s;
		s << "(" << str << ")";
		return s.str();
	}
	const string GcodeComment::toGcode() const
	{
		stringstream s;
		s << "(" << str << ")";
		return s.str();
	}
	GcodeCommandType GcodeComment::getType() {
		return GCODE_COMMENT;
	}
	GcodePath arcToLines(GcodeArcTo gArc)
	{
		GcodePath gPath;
		float arc_epsilon = 5E-7;
		float arc_tolerance = 0.1;
		float s_x = gArc.x0;
		float s_y = gArc.y0;
		float rel_c_x = gArc.rx - gArc.x0;
		float rel_c_y = gArc.ry - gArc.y0;
		float e_x = gArc.x;
		float e_y = gArc.y;
		bool isCW = !gArc.cw;

		float r = sqrt(rel_c_x * rel_c_x + rel_c_y * rel_c_y);

		float	abs_c_x = s_x + rel_c_x;
		float	abs_c_y = s_y + rel_c_y;

		//vector center->start position
		// x1 = abs_c_x - s_x
		// y1 = abs_c_y - s_y
		float	x1 = -rel_c_x;
		float	y1 = -rel_c_y;

	    //vector center->end position
		float	x2 = e_x - abs_c_x;
		float	y2 = e_y - abs_c_y;

		float angular_travel = atan2(x1 * y2 - y1 * x2, x1 * x2 + y1 * y2);
		float pi = 3.1415926;
		if (isCW) {
			if (angular_travel >= -arc_epsilon)
				angular_travel -= 2 *pi;
		}
		else {
			if (angular_travel <= arc_epsilon)
				angular_travel += 2 * pi;
		}
				
		int segments = floor(fabs(0.5 - angular_travel * r) / sqrt(arc_tolerance * (2 * r - arc_tolerance)));
		float theta_per_segment= angular_travel / segments;
		if (segments == 0) {
			segments = 1;
		    theta_per_segment = angular_travel / segments;
		}
						

		float cos_T = 2.0 - theta_per_segment * theta_per_segment;
		float sin_T = theta_per_segment * 0.16666667 * (cos_T + 4.0);

		cos_T = cos_T*0.5;

		float cos_Ti = 0;
		float sin_Ti = 0;

		float rel_new_vector_y = 0;
	    int count = 0;
	    int i;
		float new_e_x;
		float new_e_y;
		float rel_vector_x, rel_vector_y;
	    for (i = 1; i < segments; i++) {
		    if (false) {
			  rel_new_vector_y = rel_vector_x * sin_T + rel_vector_x * cos_T;
			  rel_vector_x = rel_vector_x * cos_T - rel_vector_y * sin_T;
			  rel_vector_y = rel_new_vector_y;
			  count += 1;
		   }else {
			  cos_Ti = cos(i * theta_per_segment);
			  sin_Ti = sin(i * theta_per_segment);
		      rel_vector_x = -(rel_c_x)*cos_Ti + (rel_c_y)*sin_Ti;
		      rel_vector_y = -(rel_c_x)*sin_Ti - (rel_c_y)*cos_Ti;
		      count = 0;
		  }
			new_e_x = abs_c_x + rel_vector_x;
			new_e_y = abs_c_y + rel_vector_y;
			gPath.push_back(new GcodeLineTo(new_e_x, new_e_y,gArc.z));

	  }
		return gPath;
	}
    std::ostream& operator<< (std::ostream& out, const Gcode::GcodePath& path){
        for(Gcode::GcodePath::const_iterator it = path.begin(); it != path.end(); it++){
            out << (*it)->toString();
        }
        return out;
    }
    std::ostream& operator<< (std::ostream& out, const Gcode::GcodeCommand* cmd){
        out << cmd->toString();
        return out;
    }

};

