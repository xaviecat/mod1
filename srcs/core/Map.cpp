#include "Map.hpp"
#include <QFile>
#include <QTextStream>

Map::Map(const std::string& filename) {

	QFile infile(QString(filename.data()));

	if (!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		throw std::runtime_error(ERR_OPEN(filename));
	}

	QTextStream in(&infile);
	QString vertex;

	bool first = true;
	while (!in.atEnd()) {
		in >> vertex;
		if (vertex.isEmpty()) continue;

		QVector3D	res = Map::_parseVertex(vertex.toStdString(), first);
		if (!_checkDup(res))
			this->append(res);
		first = false;
	}
	infile.close();
	if (_max.z() >_max.x() && _max.z() > _max.y()) _offset = _max.z() / OFFSET_PROP;
	else _offset =_max.x() / OFFSET_PROP;

	_addCorners();

	int i = 0;
	for (auto &item: *this){
		if (i % 2)
			item.setX(item.x() + 0.2);
		else
			item.setZ(item.z() + 0.2);
		i = !i;
	}
	this->sort();
}

bool Map::_checkDup(const QVector3D& point) const {
	for (auto const &item: *this)
		if (point.z() == item.z() && point.x() == item.x())
			return true;
	return false;
}

QVector3D Map::_parseVertex(const std::string& vertex, bool first) {
	std::stringstream	stream(vertex);
	float				x,y,z;
	char				sep1,sep2;
	char				par1,par2;

	if (vertex.find_first_of(')') != vertex.length() - 1
		|| vertex.find_last_of('(') != 0)
		throw runtime_error(ERR_INPUT_FILE ERR_SYNTAX);
	stream >> par1 >> x >> sep1 >> y >> sep2 >> z >> par2;
	if (!stream || sep1 != ',' || sep2 != ',' || par1 != '(' || par2 != ')')
		throw runtime_error(ERR_INPUT_FILE ERR_SYNTAX);

	std::string rest;
	stream >> rest;
	if (stream)
		throw runtime_error(ERR_INPUT_FILE ERR_SYNTAX);

	if (first){
		_min.setX(x);
		_min.setY(y);
		_min.setZ(z);
		_min.setX(x);
		_min.setY(y);
		_min.setZ(z);
	}
	_setMinMax(x, y, z);
	return {x, y, z};
}

std::ostream &operator<<(ostream &o, const Map &rhs){
	o << "{";

	for (const auto &item: rhs){
		if (item != rhs.first())
			o << ", ";
		o << "{" << item.x() << "," << item.y() << "," << item.z() << "}";
	}
	o << "}";
	return o;
}

void Map::_addCorners() {
	this->append({_min.x() - _offset, 0, _min.z() - _offset});
	this->append({_min.x() - _offset, 0, _max.z() + _offset});
	this->append({_max.x() + _offset, 0, _min.z() - _offset});
	this->append({_max.x() + _offset, 0, _max.z() + _offset});
	for (auto it = this->end() - 4; it != this->end(); ++it)
		_setMinMax(it->x(), it->y(), it->z());

}

void Map::_setMinMax(qreal x, qreal y, qreal z) {
	if (x > _max.x())
		_max.setX(x);
	else if (x < _min.x())
		_min.setX(x);
	if (y > _max.y())
		_max.setY(y);
	else if (y < _min.y())
		_min.setY(y);
	if (z > _max.z())
		_max.setZ(z);
	else if (z < _min.z())
		_min.setZ(z);
}

void Map::normalize() {
	int i = 0;
	for (auto &item:*this){
		if (i % 2)
			item.setX(item.x() - 0.2);
		else
			item.setZ(item.z() - 0.2);
		i = !i;
	}
	if (_min.x() != 0){
		for (auto &item: *this)
			item.setX(item.x() - _min.x());
		_max.setX(_max.x() - _min.x());
		_min.setX(0);
	}
	if (_min.z() != 0){
		for (auto &item: *this)
			item.setZ(item.z() - _min.z());
		_max.setZ(_max.z() - _min.z());
		_min.setZ(0);
	}
	for (auto &item: *this){
		if (item.y())
			item.setY(item.y() / _max.y());
		item.setX(item.x() / _max.x() -0.5);
		item.setZ(item.z() / _max.z() -0.5);
	}
}

void Map::sort() {
	// cout << *this << endl << endl;
	for (int i = 0; i < this->size() - 1;){
		if (this->at(i).x() > this->at(i + 1).x()){
			this->swapItemsAt(i, i + 1);
			i > 1 ? i -= 2 : i = 0;
		}
		else
			++i;
	}
	for (int i = 0; i < this->size() - 1;){
		if (this->at(i).z() > this->at(i + 1).z()){
			this->swapItemsAt(i, i + 1);
			i > 1 ? i -= 2 : i = 0;
		}
		else
			++i;
	}
	// cout << *this << endl;
	// cout << endl;
}
