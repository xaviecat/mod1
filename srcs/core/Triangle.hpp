#ifndef TRIANGLE_HPP
# define TRIANGLE_HPP
# include <QVector>
# include <QVector3D>

class Triangle {
private:
	const QVector3D	_a;
	const QVector3D	_b;
	const QVector3D	_c;
public:
	Triangle(const QVector3D& a, const QVector3D& b, const QVector3D& c);
	const QVector3D& a() const;
	const QVector3D& b() const;
	const QVector3D& c() const;
};

#endif //TRIANGLE_HPP
