#ifndef TEST_HPP
# define TEST_HPP
# include "../core/Map.hpp"
# include "../utils/colors.h"
# include <QKeyEvent>
# include <QMatrix4x4>
# include <QMouseEvent>
# include <QOpenGLFunctions>
# include <QOpenGLWidget>
# include <QPainter>
# include <QTime>
# include <QTimer>
# include <QWheelEvent>
# include <iostream>

# define SIZE_MAP 5.0

class Test : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT
public:
	explicit Test(int framesPerSecond, QWidget *parent, char *name);
	~Test();

	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	void drawGizmo();
	void drawTriangles();

	void keyPressEvent(QKeyEvent *keyEvent);
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void rotateBy(int x, int y, int z);

	void convertRawMap();
	float	distance;
	float	xRot;
	float	yRot;
	float	zRot;
	QPoint	lastPos;
public slots:
	virtual void timeOutSlot();
private:
	QTimer	*timer;
	QVector<QVector<QVector3D>>		rawMap ={
		{{0,0,0},{10, 0, 0},{20, 0, 0}, {30, 0, 0}},
		{{0, 0, 10},{10, 20, 10},{20, 15, 10}, {30, 0, 10}},
		{{0, 0, 20},{10, 5, 20},{20, 10, 20}, {30, 0, 20}},
		{{0, 0, 40},{10, 0, 40},{20, 0, 40}, {30, 0, 40}},
	};
	float	maxX = 30;
	float	maxY = 20;
	float	maxZ = 40;
	Map		vertices;
	int		vertices_by_x = 4;
	int		vertices_by_z = 4;
	int		quads_by_x = 3;
	int		quads_by_z = 3;

};

#endif //TEST_HPP
