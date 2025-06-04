#include "Triangle.hpp"

Triangle::Triangle(const QVector3D &a, const QVector3D &b, const QVector3D &c)
	: _a(a)
	, _b(b)
	, _c(c){
}

const QVector3D &Triangle::a() const {
	return this->_a;
}

const QVector3D &Triangle::b() const {
	return this->_b;
}

const QVector3D &Triangle::c() const {
	return this->_c;
}
