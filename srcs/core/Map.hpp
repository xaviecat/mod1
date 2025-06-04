#ifndef MAP_HPP
# define MAP_HPP

# include <QVector3D>
# include <QVector>
# include <fstream>
# include <iostream>
# include <exception>
# include <algorithm>
# include <sstream>
# include "../utils/errors.h"

using namespace std;

class Map : public QVector<QVector3D> {
	using QVector<QVector3D>::QVector;
private:
	static QVector3D	_parseVertex(const std::string& vertex);
public:
	explicit Map(const std::string& filename);

	friend std::ostream& operator<<(std::ostream& o, const Map& rhs);
};

#endif