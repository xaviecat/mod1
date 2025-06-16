#ifndef TRIANGULATOR_HPP
# define TRIANGULATOR_HPP

# include <QVector>
# include <QtConcurrent>
# include <QFuture>
# include <QMutex>
# include "../utils/colors.h"
# include <ostream>
# include "Map.hpp"

struct Triangle {
	int i1, i2, i3;
	QVector3D normal;

	Triangle(int idx1, int idx2, int idx3, const QVector3D& n)
			: i1(idx1), i2(idx2), i3(idx3), normal(n) {}
};

class Triangulator : public QVector<unsigned int>{
private:
	Triangulator(const Triangulator& src);

	bool _containsVertices(const Map& vertices, const QPointF& circumCenter, qreal radius) const;
	bool _checkValid(const QVector3D& a, const QVector3D& b, const QVector3D& c);
	const QVector3D _getLineFromPoints(const QVector3D& pa, const QVector3D& pb) const;
	const QVector3D _getPerpendicularBisector(const QVector3D& line, const QVector3D& pa, const QVector3D& pb) const;
	const QPointF _getIntersectionPoint(const QVector3D& lineA, const QVector3D& lineB) const;
	const QPointF _getCircumCenter(const QVector3D& pointA, const QVector3D& pointB, const QVector3D& pointC) const;
	bool _isInCircumCenter(const QVector3D& point, const QPointF& circumCenter, qreal radius) const;
	void _triangulate(const Map& vertices, int threadId, int numThreads);

	QMutex	_dataMutex;
public:
	Triangulator(const Map& vertices);
	QVector<QVector3D> normales;
	friend ostream &operator<<(ostream &os, const Triangulator &triangulator);
	const Triangulator tesselated(Map& vertices) const;
};


#endif //TRIANGULATOR_HPP
