#include "Triangle.hpp"

Triangle::Triangle(const QVector3D &a, const QVector3D &b, const QVector3D &c)
	: _a(a)
	, _b(b)
	, _c(c){
}

Triangle::Triangle(const Triangle &src) : _a(src.a().x(), src.a().y(), src.a().y())
, _b(src.b().x(), src.b().y(), src.b().y())
, _c(src.c().x(), src.c().y(), src.c().y()) {
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

Triangle& Triangle::operator=(const Triangle &src) {
	this->_a = src.a();
	this->_b = src.b();
	this->_c = src.c();
	return *this;
}
