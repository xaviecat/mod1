#include "Triangulator.hpp"

Triangulator::Triangulator(const Map &vertices) {
	for (auto it = vertices.begin(); it != vertices.end(); ++it){
		if (it + 1 == vertices.end() || it + 2 == vertices.end())
			break;
		for (auto it1 = it + 1; it1 != vertices.end(); ++it1){
			if (it1 + 1 == vertices.end()) break;
			for (auto it2 = it1 + 1; it2 != vertices.end(); ++it2){
				QVector3D	a = *it;
				QVector3D	b = *(it + 1);
				QVector3D	c = *(it + 2);

				cout << "index1: " << it - vertices.begin() << ", index2: " << it1 -vertices.begin() << ", index3: " << it2 - vertices.begin() << endl;

				if (!_checkValid(a, b, c)) continue;

				QPointF circumCenter = _getCircumCenter(a, b, c);
				//√(xA − xB)^2 + (yA − yB)^2
				qreal radius = sqrt(pow(a.x() - circumCenter.x(), 2) + pow(a.z() - circumCenter.y(),2));
				cout << "circumcenter: " << circumCenter.x() << ", " << circumCenter.y() << ", radius: " << radius << endl;

//				if (!_containsVertices(vertices, circumCenter, radius))
//				Triangle curr = Triangle(a, b, c);
				this->append(it - vertices.begin());
				this->append(it1 - vertices.begin());
				this->append(it2 - vertices.begin());
//				for (const auto &item : vertices)
				cout << _isInCircumCenter({0, 0, 3.544}, circumCenter, radius) << endl;
			}
		}

//		break;
	}
}

const QPointF Triangulator::_getIntersectionPoint(const QVector3D &lineA, const QVector3D &lineB) const {
	qreal determinant = lineA.x() * lineB.y() - lineB.x() * lineA.y();

	qreal	x = (lineB.y() * lineA.z() - lineA.y() * lineB.z())/determinant;
	qreal	y = (lineA.x() * lineB.z() - lineB.x() * lineA.z())/determinant;

	return {x, y};
}

const QVector3D
Triangulator::_getPerpendicularBisector(const QVector3D &line, const QVector3D &pa, const QVector3D &pb) const{
	QPointF midPoint = QPointF((pa.x() + pb.x())/2, (pa.z() + pb.z())/2);

	// c = -bx + ay
	float c = (line.y() * -1) * midPoint.x() + line.x() * midPoint.y();
	float a = line.y() * -1;
	float b = line.x();

	return {a, b, c};
}

const QVector3D Triangulator::_getLineFromPoints(const QVector3D &pa, const QVector3D &pb) const {
	// line represented as :  ax + by = c
	float a = pb.z() - pa.z();
	float b = pa.x() - pb.x();
	float c = (a * pa.x()) + (b * pa.z());

	return {a, b, c};
}

const QPointF
Triangulator::_getCircumCenter(const QVector3D &pointA, const QVector3D &pointB, const QVector3D &pointC) const {
	QVector3D	lineAB = _getLineFromPoints(pointA, pointB);
	QVector3D	lineBC = _getLineFromPoints(pointB, pointC);

	QVector3D ABBisector = _getPerpendicularBisector(lineAB, pointA, pointB);
	QVector3D BCBisector = _getPerpendicularBisector(lineBC, pointB, pointC);

	QPointF circumCenter = _getIntersectionPoint(ABBisector, BCBisector);
	return circumCenter;
}

bool Triangulator::_checkValid(const QVector3D &a, const QVector3D &b, const QVector3D &c) {
	if (a.y() == b.y() || a.y() == c.y() || b.y() == c.y())
		return false;
	return true;
}

bool Triangulator::_isInCircumCenter(const QVector3D &point, const QPointF &circumCenter, qreal radius) const {
	qreal pointDistance =  sqrt(pow(point.x() - circumCenter.x(), 2) + pow(point.z() - circumCenter.y(),2));

	return pointDistance < radius;
}

ostream &operator<<(ostream &os, const Triangulator &triangulator) {
	cout << "{" << triangulator.first();
	for (auto it = triangulator.begin() + 1; it != triangulator.end(); ++it)
		cout << ", " << *it;
	cout << "}" << endl;
	return os;
}
