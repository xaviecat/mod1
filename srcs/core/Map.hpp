#ifndef MAP_HPP
# define MAP_HPP

# include <QVector3D>
# include <QVector>
# include <fstream>
# include <iostream>
# include <exception>
# include <algorithm>
# include <sstream>
# include "errors.h"
# define OFFSET_PROP 8

using namespace std;

class Map : public QVector<QVector3D> {
	using QVector<QVector3D>::QVector;
private:
	QVector3D _parseVertex(const std::string& vertex, bool first);
	void _addCorners();
	void _setMinMax(qreal x, qreal y, qreal z);
	bool _checkDup(const QVector3D& point) const;
	QVector3D _min;
	QVector3D _max;
	float _offset;
public:
	explicit Map(const std::string& filename);

	friend std::ostream& operator<<(std::ostream& o, const Map& rhs);
	void normalize();
};

#endif