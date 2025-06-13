#ifndef TRIANGULATOR_HPP
# define TRIANGULATOR_HPP

# include "Triangle.hpp"
# include <QVector>
# include <ostream>
# include "Map.hpp"

class Triangulator : public QVector<unsigned int>{
private:
	bool _containsVertices(const Map& vertices, const QPointF& circumCenter, qreal radius) const;
	bool _checkValid(const QVector3D& a, const QVector3D& b, const QVector3D& c);
	const QVector3D _getLineFromPoints(const QVector3D& pa, const QVector3D& pb) const;
	const QVector3D _getPerpendicularBisector(const QVector3D& line, const QVector3D& pa, const QVector3D& pb) const;
	const QPointF _getIntersectionPoint(const QVector3D& lineA, const QVector3D& lineB) const;
	const QPointF _getCircumCenter(const QVector3D& pointA, const QVector3D& pointB, const QVector3D& pointC) const;
	bool _isInCircumCenter(const QVector3D& point, const QPointF& circumCenter, qreal radius) const;
public:
	Triangulator(const Map& vertices);
	QVector<QVector3D> normales;
	friend ostream &operator<<(ostream &os, const Triangulator &triangulator);
};


#endif //TRIANGULATOR_HPP
