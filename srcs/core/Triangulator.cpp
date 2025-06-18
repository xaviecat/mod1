#include "Triangulator.hpp"

Triangulator::Triangulator(const Map &vertices) {
	normales.resize(vertices.size());

	int numThreads = QThread::idealThreadCount();
	cout << "Ideal thread count: " << numThreads << endl;
	QVector<QFuture<void>> futures;

	for (int thread = 0; thread < numThreads; ++thread) {
		QFuture<void> future = QtConcurrent::run([this, &vertices, thread, numThreads]() {
			_triangulate(vertices, thread, numThreads);
		});
		futures.append(future);
	}

	for (auto& future : futures) {
		future.waitForFinished();
	}
}

void Triangulator::_triangulate(const Map &vertices, int threadId, int numThreads){
	QVector<Triangle> localTriangles;

	for (int i = threadId; i < vertices.size() - 2; i += numThreads){
		for (int j = i + 1; j < vertices.size() - 1; ++j){
			for (int k = j + 1; k < vertices.size(); ++k){
				QVector3D	a = vertices[i];
				QVector3D	b = vertices[j];
				QVector3D	c = vertices[k];

				if (!_checkValid(a, b, c)) continue;

				QPointF circumCenter = _getCircumCenter(a, b, c);
				if (circumCenter.x() == -std::numeric_limits<double>::infinity()
					|| circumCenter.x() == std::numeric_limits<double>::infinity())
					continue;
				//√(xA − xB)^2 + (yA − yB)^2
				qreal radius = sqrt(pow(a.x() - circumCenter.x(), 2) + pow(a.z() - circumCenter.y(),2));
				if (!_containsVertices(vertices, circumCenter, radius)){
					QVector3D v1 = b - a;
					QVector3D v2 = c - a;
					QVector3D normal = QVector3D::crossProduct(v2, v1);
					if (_isCCW(a, b, c))
						normal = QVector3D::crossProduct(v1, v2);
					localTriangles.append(Triangle(i, j, k, normal));
				}
			}
		}
	}

	QMutexLocker locker(&_dataMutex);
	for (const auto& triangle : localTriangles) {
		if (_isCCW(vertices.at(triangle.i1), vertices.at(triangle.i2), vertices.at(triangle.i3))){
			this->append(triangle.i1);
			this->append(triangle.i2);
			this->append(triangle.i3);
		}
		else{
			this->append(triangle.i1);
			this->append(triangle.i3);
			this->append(triangle.i2);
		}

		normales[triangle.i3] += triangle.normal;
		normales[triangle.i2] += triangle.normal;
		normales[triangle.i1] += triangle.normal;
	}
}

bool Triangulator::_isCCW(const QVector3D& a, const QVector3D& b, const QVector3D& c) const{
	QVector3D deltaBA = QVector3D(b.x(), b.z(), 0) - QVector3D(a.x(), a.z(), 0);
	QVector3D deltaCA = QVector3D(c.x(), c.z(), 0) - QVector3D(a.x(), a.z(), 0);
	QVector3D crossProduct = QVector3D::crossProduct(deltaBA, deltaCA);

	if (crossProduct.z() < 0.0)
		return true;
	else
		return false;

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
	if ((a.z() == b.z() && a.z() == c.z()) || (a.x() == b.x() && a.x() == c.x()))
		return false;
	return true;
}

bool Triangulator::_isInCircumCenter(const QVector3D &point, const QPointF &circumCenter, qreal radius) const {
	qreal pointDistance =  sqrt(pow(point.x() - circumCenter.x(), 2) + pow(point.z() - circumCenter.y(),2));

	return pointDistance < radius;
}

bool Triangulator::_containsVertices(const Map &vertices, const QPointF &circumCenter, qreal radius) const {
	for (const auto &item: vertices){
		qreal distanceToCircumCenter = sqrt(pow(item.x() - circumCenter.x(), 2) + pow(item.z() - circumCenter.y(), 2));
		if (distanceToCircumCenter < radius - 0.01)
			return true;
	}
	return false;
}

ostream &operator<<(ostream &os, const Triangulator &triangulator) {
	cout << "{" << triangulator.first();
	for (auto it = triangulator.begin() + 1; it != triangulator.end(); ++it)
		cout << ", " << *it;
	cout << "}" << endl;
	return os;
}

const Triangulator Triangulator::tesselated(Map &vertices) const {
	if (this->size() % 3) return *this;
	if (vertices.size() > 200)
		cout << END_MSG("The provided map contains more than 200 vertices, the triangulation might take long...") << endl;
	for (auto it = this->begin(); it != this->end();){
		QVector3D pointA = vertices.at(*it);
		QVector3D pointB = vertices.at(*(it + 1));
		QVector3D pointC = vertices.at(*(it + 2));

		QVector3D centroid(
			(pointA.x() + pointB.x() + pointC.x()) / 3,
			(pointA.y() + pointB.y() + pointC.y()) / 3,
			(pointA.z() + pointB.z() + pointC.z()) / 3);
		vertices.append(centroid);
		it = it + 3;
	}
	if (vertices.size() < 200)
		return (Triangulator(vertices).tesselated(vertices));
	return Triangulator(vertices);
}

Triangulator::Triangulator(const Triangulator& src) : QVector<unsigned int>(){
	for (auto const & item: src){
		this->append(item);
	}
}
