#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
	struct DPoint {
	public:
		DPoint() :m_x(0.f), m_y(0.f) {}
		DPoint(float x, float y) :m_x(x), m_y(y) {}
		float m_x, m_y;
		void print() {
			std::cout << "[" << m_x << "," << m_y << "]" << " ";
		}
		float distance(const DPoint& pnt) const {
			float vx = m_x - pnt.m_x;
			float vy = m_y - pnt.m_y;
			return (float)sqrt(vx * vx + vy * vy);
		}
		//def mag(self): return math.hypot(self.x, self.y)
		float mag() {
			return hypot(m_x, m_y);
		}
		float angle() {
			return atan2(m_y, m_x);
		}
		inline DPoint operator*(const float f) const {
			return DPoint(m_x * f, m_y * f);
		}

		inline DPoint& operator*=(const float f) {
			m_x *= f;
			m_y *= f;
			return *this;
		}

		inline DPoint operator-(const DPoint& vec) const {
			return DPoint(m_x - vec.m_x, m_y - vec.m_y);
		}
		inline DPoint operator+(const DPoint& vec) const {
			return DPoint(m_x + vec.m_x, m_y + vec.m_y);
		}
		inline DPoint operator/(const DPoint& vec) const {
			return DPoint(vec.m_x != 0 ? m_x / vec.m_x : m_x, vec.m_y != 0 ? m_y / vec.m_y : m_y);
		}

		inline DPoint& operator/=(const DPoint& vec) {
			vec.m_x != 0 ? m_x /= vec.m_x : m_x;
			vec.m_y != 0 ? m_y /= vec.m_y : m_y;

			return *this;
		}
		inline DPoint operator/(const float f) const {
			if (f == 0) return DPoint(m_x, m_y);

			return DPoint(m_x / f, m_y / f);
		}

		inline DPoint& operator/=(const float f) {
			if (f == 0) return *this;

			m_x /= f;
			m_y /= f;
			return *this;
		}
		inline float length() const {
			return (float)sqrt(m_x * m_x + m_y * m_y);
		}
	};
	inline DPoint operator*(float f, const DPoint& vec) {
		return DPoint(f * vec.m_x, f * vec.m_y);
	}
	inline DPoint operator/(float f, const DPoint& vec) {
		return DPoint(f / vec.m_x, f / vec.m_y);
	}
	typedef std::vector<DPoint> FourPoints;
	typedef std::vector<DPoint> ThreePoints;//save three points[p0,p1,p2]
	typedef std::vector<ThreePoints> TPList;// [ [p0,p1,p2],[p0,p1,p2] ]  [ThreePoints0,ThreePoints1,...]

#endif //POINT_H
/*
    DPoint p(0.5f,1.7f);
    PointList pl;
    pl.push_back(p);
    pl.push_back(DPoint(13.7f,20.9f));
    std::cout<<p.m_x<<"\t"<<p.m_y<<endl;
    for(std::vector<DPoint>::const_iterator plit =pl.begin();plit!=pl.end();plit++){
        std::cout<<(*plit).m_x<<"\t"<<(*plit).m_y<<endl;
    }
*/