#include "Test.hpp"

Test::Test(int framesPerSecond, QWidget *parent, char *name) : QOpenGLWidget(parent) {
	setWindowTitle(QString::fromUtf8(name));
	std::cout << C_MSG("Test parametric constructor called") << std::endl;

	// setup display refresh
	if(framesPerSecond == 0)
		timer = NULL;
	else
	{
		int seconde = 1000;
		int timerInterval = seconde / framesPerSecond;
		timer = new QTimer(this);
		connect(timer, &QTimer::timeout, this, &Test::timeOutSlot);
		timer->start(timerInterval);
	}
}

void Test::initializeGL() {
	initializeOpenGLFunctions();

	distance = -5.0;
	xRot = 0;
	yRot = 0;
	zRot = 0;
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
}

void Test::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	QMatrix4x4	modelMatrix;
	QMatrix4x4	viewMatrix;
	QMatrix4x4	projectionMatrix;

	QVector3D	eye(0.0, 0.0, -distance);
	QVector3D	center(0.0, 0.0, 0.0);
	QVector3D	up(0.0, 1.0, 0.0);

	viewMatrix.lookAt(eye, center, up);

	modelMatrix.rotate(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
	modelMatrix.rotate(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
	modelMatrix.rotate(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

	float aspect = static_cast<float>(width()) / static_cast<float>(height());

	projectionMatrix.perspective(60.0f, aspect, 0.1f, 100.0f);

	QMatrix4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mvpMatrix.constData());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glBegin(GL_LINES); // x line (red)
		glColor3d(1,0,0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES); // y line (blue)
		glColor3d(0,0,1);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES); // z line (green)
		glColor3d(0,1,0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();

}

void Test::resizeGL(int width, int height)
{
	glViewport(0,0, width, height);
}

void Test::keyPressEvent(QKeyEvent *keyEvent)
{
	if (keyEvent->key() == Qt::Key_Escape)
		close();
}

void Test::wheelEvent(QWheelEvent *event)
{
	distance *= 1.0 + (1.0 * event->angleDelta().y() / 1200.0);
}

void Test::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
}

void Test::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->position().x() - lastPos.x();
	int dy = event->position().y() - lastPos.y();

	if (event->buttons() & Qt::RightButton)
	{
		rotateBy(dy*8, 0, 0);
		rotateBy(0, dx*8, 0);
	}
	lastPos = event->pos();
}

void Test::rotateBy(int x, int y, int z)
{
	xRot += x;
	yRot += y;
	zRot += z;
}
void Test::timeOutSlot()
{
	update();
}

Test::~Test() {
	std::cout << C_MSG("Test destructor called") << std::endl;
}
