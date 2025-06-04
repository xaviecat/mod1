#ifndef TEST_HPP
# define TEST_HPP
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

class Test : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT
public:
	explicit Test(int framesPerSecond, QWidget *parent, char *name);
	~Test();

	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	void keyPressEvent(QKeyEvent *keyEvent);
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void rotateBy(int x, int y, int z);

	float	distance;
	float	xRot;
	float	yRot;
	float	zRot;
	QPoint	lastPos;
public slots:
	virtual void timeOutSlot();
private:
	QTimer	*timer;
};

#endif //TEST_HPP
