#ifndef Gcode_H
#define Gcode_H
#include <vector>
#include <string>
using namespace std;
namespace Gcode{
	enum GcodeCommandType {GCODE_INVALID = -1,
		GCODE_LINE_TO,
		GCODE_ARC_TO,
		GCODE_MOVE_TO,
		GCODE_COMMENT
		};
	
	class GcodeCommand {
		public:
			GcodeCommand(float, float, float);	
			float x, y,z;
			virtual const string toString() const = 0;
			virtual const string toGcode()const = 0;
			virtual GcodeCommandType getType() = 0;
	};
	class GcodeMoveTo : public GcodeCommand {
		public:
			static const int length = 2;
			GcodeMoveTo(float, float,float);
			virtual const string toString() const;
			virtual const string toGcode() const;
			virtual GcodeCommandType getType();
	};
	
	class GcodeLineTo : public GcodeCommand {
		public:
			static const int length = 2;
			GcodeLineTo(float, float,float);
			virtual const string toString() const;
			virtual const string toGcode() const;
			virtual GcodeCommandType getType();
			
	};
	
	class GcodeArcTo : public GcodeCommand {
		public:
			float rx,ry,x0,y0;
            bool cw;
			GcodeArcTo(float, float,float,float,bool, float, float,float);
			virtual const string toString() const;
			virtual const string toGcode() const;
			virtual GcodeCommandType getType();	
	};
	class GcodeComment :public GcodeCommand {
		  public:
			  string str;
			  GcodeComment(string s);
			  virtual const string toString() const;
			  virtual const string toGcode() const;
			  virtual GcodeCommandType getType();
	};
	
	typedef vector<GcodeCommand*> GcodePath;
	GcodePath arcToLines(GcodeArcTo gArc);
    std::ostream& operator<< (std::ostream&, const Gcode::GcodePath&);
    std::ostream& operator<< (std::ostream&, const Gcode::GcodeCommand*);
};
#endif//Gcode_H
