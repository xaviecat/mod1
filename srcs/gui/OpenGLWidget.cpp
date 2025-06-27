#include "OpenGLWidget.hpp"

#include "../core/Triangulator.hpp"

OpenGLWidget::OpenGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
	, delay(10)
	, frameCount(0)
	, lastCount(0)
	, distance(-2.5)
	, xRot(45)
	, yRot(-45)
	, zRot(0)
	, xTrans(0)
	, yTrans(0)
	, zTrans(0)
	, animation(false)
	, light_alpha(0)
	, mode(SHADERS)
#ifndef __EMSCRIPTEN__
	, fillMode(GL_FILL)
#endif
	, vertices(Map(":/maps/demo1.mod1"))
	, indexArray(Triangulator(vertices).tesselated(vertices))
	, vertexBuffer(QOpenGLBuffer::VertexBuffer)
	, indexBuffer(QOpenGLBuffer::IndexBuffer)
	, textureBuffer(QOpenGLBuffer::VertexBuffer)
	, normalBuffer(QOpenGLBuffer::VertexBuffer) {
	std::cout << C_MSG("Test parametric constructor called") << std::endl;

	setWindowTitle("Draw Coordinate");

	vertices.normalize();
	initializeUVMap();

	// setup display refresh
	timer.start(delay);
	connect(&timer, &QTimer::timeout, this, &OpenGLWidget::timeOutSlot);
	timeLastFrame = QTime::currentTime();
}

void OpenGLWidget::initializeGL() {
	initializeOpenGLFunctions();

	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	texture = new QOpenGLTexture(QImage(":/textures/rock.jpg").flipped());
	texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);

	normales = indexArray.normales;
	initializeShaders();
	initializeVAO();
}

void OpenGLWidget::initializeVAO() {
	// VAO
	terrainVAO.create();
	terrainVAO.bind();

	// Vertex VBO
	vertexBuffer.create();
	vertexBuffer.bind();
	vertexBuffer.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));
	glEnableVertexAttribArray(vertexAttribute);
	glVertexAttribPointer(vertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Normal VBO
	normalBuffer.create();
	normalBuffer.bind();
	normalBuffer.allocate(normales.constData(), normales.size() * sizeof(QVector3D));
	glEnableVertexAttribArray(normalAttribute);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Texture VBO
	textureBuffer.create();
	textureBuffer.bind();
	textureBuffer.allocate(UVMap.constData(), UVMap.size() * sizeof(QVector3D));
	glEnableVertexAttribArray(textureAttribute);
	glVertexAttribPointer(textureAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Barycentric VBO
#ifdef __EMSCRIPTEN__
	generateBarycentricCoordinates();
	barycentricBuffer.create();
	barycentricBuffer.bind();
	barycentricBuffer.allocate(barycentricCoords.constData(), barycentricCoords.size() * sizeof(QVector3D));
	glEnableVertexAttribArray(barycentricAttribute);
	glVertexAttribPointer(barycentricAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
#endif

	// EBO
	indexBuffer.create();
	indexBuffer.bind();
	indexBuffer.allocate(indexArray.constData(), indexArray.size() * sizeof(GLuint));

	terrainVAO.release();
	// std::cout << "Terrain VAO initialized with " << vertices.size() << " vertices, " << indexArray.size() << " indices" << std::endl;
}

#ifdef __EMSCRIPTEN__
void OpenGLWidget::generateBarycentricCoordinates() {
	barycentricCoords.clear();
	barycentricCoords.reserve(vertices.size());

	// For each triangle, assign barycentric coordinates (1,0,0), (0,1,0), (0,0,1)
	for (int i = 0; i < indexArray.size(); i += 3) {
		// This is a simplified approach - you might need to adjust based on your indexing
		for (int j = 0; j < vertices.size(); ++j) {
			if (j % 3 == 0) barycentricCoords.append(QVector3D(1.0f, 0.0f, 0.0f));
			else if (j % 3 == 1) barycentricCoords.append(QVector3D(0.0f, 1.0f, 0.0f));
			else barycentricCoords.append(QVector3D(0.0f, 0.0f, 1.0f));
		}
	}
}
#endif

void OpenGLWidget::paintGL() {
	refreshMVPMatrix();

	switch (mode) {
	case SHADERS:
		drawShaders();
		break;
	case BUFFERS:
#ifndef __EMSCRIPTEN__
		drawBuffers();
#endif
		break;
	}

#ifndef __EMSCRIPTEN__
	drawGizmo();
#endif

	++frameCount;
	QTime	actualTime = QTime::currentTime();
	if (timeLastFrame.msecsTo(actualTime) >= 1000)
	{
		lastCount = frameCount;
		frameCount = 0;
		timeLastFrame = QTime::currentTime();
	}
}

void OpenGLWidget::paintEvent(QPaintEvent *event) {
	QOpenGLWidget::paintEvent(event);
	QPainter painter(this);
	painter.setPen(Qt::white);
	painter.drawText(10, 15, QString("FPS: %1").arg(lastCount));
	painter.drawText(10, 30, "Spacebar to change transfer mode");
	// painter.drawText(10, 45, "Press T to draw on/off texture");
	// painter.drawText(10, 60, "Press F to fill on/off");
	painter.drawText(10, height()-15, QString("%1").arg(renderingMode[mode]));
}

void OpenGLWidget::initializeShaders() {
#ifndef __EMSCRIPTEN__
	if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/desktop/terrain.vert"))
		std::cerr << program.log().toStdString() << std::endl;
	if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/desktop/terrain.frag"))
		std::cerr << program.log().toStdString() << std::endl;
#else
	if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/web/terrain.vert"))
		std::cerr << program.log().toStdString() << std::endl;
	if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/web/terrain.frag"))
		std::cerr << program.log().toStdString() << std::endl;
#endif
	program.bindAttributeLocation("aVertex", vertexAttribute);
	program.bindAttributeLocation("aNormal", normalAttribute);
	program.bindAttributeLocation("aTexCoords", textureAttribute);
#ifdef __EMSCRIPTEN__
	program.bindAttributeLocation("aBarycentric", barycentricAttribute);
#endif

	if (!program.link())
		std::cerr << program.log().toStdString() << std::endl;

	matrixLocation = program.uniformLocation("mvpmatrix");
	lightDirectionLocation = program.uniformLocation("light_direction");
	ambientColorLocation = program.uniformLocation("ambient_color");
	// textureLocation = program.uniformLocation("texture2d");
#ifdef __EMSCRIPTEN__
	wireframeModeLocation = program.uniformLocation("wireframe_mode");
	wireframeColorLocation = program.uniformLocation("wireframe_color");
	wireframeWidthLocation = program.uniformLocation("wireframe_width");
#endif

	bool error = matrixLocation < 0 || lightDirectionLocation < 0 || ambientColorLocation < 0;// || textureLocation < 0;
	if (error)
		std::cerr << "Error while getting uniforms locations: " << matrixLocation << lightDirectionLocation << ambientColorLocation << std::endl;

	program.bind();
	program.setUniformValue(ambientColorLocation, QVector4D(0.4, 0.4, 0.4, 1.0));
	program.setUniformValue(lightDirectionLocation, QVector4D(cos(light_alpha), 1.0, sin(light_alpha), 1.0));
#ifdef __EMSCRIPTEN__
	program.setUniformValue(wireframeModeLocation, wireframeMode);
	program.setUniformValue(wireframeColorLocation, QVector3D(1.0, 1.0, 1.0)); // White wireframe
	program.setUniformValue(wireframeWidthLocation, 1.0f);
#endif
	program.release();
}

void OpenGLWidget::drawShaders() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifndef __EMSCRIPTEN__
	glPolygonMode(GL_FRONT, fillMode);
	glPolygonMode(GL_BACK, GL_LINES);
#endif
	program.bind();
	program.setUniformValue(matrixLocation, mvpMatrix);

	texture->bind();
	terrainVAO.bind();

	glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, nullptr);

	terrainVAO.release();
	texture->release();

	if (animation) {
		light_alpha += 0.02;
		if (light_alpha > 2 * M_PI) light_alpha -= 2 * M_PI;
		program.setUniformValue("light_direction", QVector4D(cos(light_alpha), 1.0, sin(light_alpha), 1.0));
	}

	program.release();
#ifndef __EMSCRIPTEN__
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
}

void OpenGLWidget::refreshMVPMatrix() {
	QMatrix4x4	modelMatrix;
	QMatrix4x4	viewMatrix;
	QMatrix4x4	projectionMatrix;

	viewMatrix.translate(0.0, 0.0, distance);

	modelMatrix.rotate(xRot, 1.0f, 0.0f, 0.0f);
	modelMatrix.rotate(yRot, 0.0f, 1.0f, 0.0f);
	modelMatrix.rotate(zRot, 0.0f, 0.0f, 1.0f);

	viewMatrix.translate(xTrans, yTrans, zTrans);

	projectionMatrix.perspective(60.0f, width() / height(), 0.1f, 500.0f);

	mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
}

#ifndef __EMSCRIPTEN__
void OpenGLWidget::drawBuffers() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT, fillMode);
	glPolygonMode(GL_BACK, GL_LINE);

	glColor3f(0.0, 0.5, 1.0);
	glEnableClientState(GL_VERTEX_ARRAY);

	vertexBuffer.bind();
	glVertexPointer(3, GL_FLOAT, 0, nullptr);
	vertexBuffer.release();

	indexBuffer.bind();
	glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, nullptr);
	indexBuffer.release();

	glDisableClientState(GL_VERTEX_ARRAY);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
#endif

void OpenGLWidget::initializeUVMap() {
	for (auto vertex: vertices) {
		QVector2D	texCoords;

		texCoords.setX(vertex.x());
		texCoords.setY(vertex.z());
		UVMap.push_back(texCoords);
	}
}

#ifndef __EMSCRIPTEN__
void OpenGLWidget::drawGizmo() {
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mvpMatrix.constData());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glBegin(GL_LINES); // x line (red)
	{
		glColor3d(1,0,0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
	}
	glEnd();
	glBegin(GL_LINES); // y line (blue)
	{
		glColor3d(0,0,1);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
	}
	glEnd();
	glBegin(GL_LINES); // z line (green)
	{
		glColor3d(0,1,0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
	}
	glEnd();
}
#endif

void OpenGLWidget::resizeGL(int width, int height) {
	glViewport(0,0, width, height);
}

void OpenGLWidget::keyPressEvent(QKeyEvent *keyEvent) {
	if (keyEvent->key() == Qt::Key_Escape)
		close();
	else if (keyEvent->key() == Qt::Key_Space)
		mode  = static_cast<eRenderingMode>((mode + 1) % 2);
#ifndef __EMSCRIPTEN__
	else if (keyEvent->key() == Qt::Key_F)
		fillMode = fillMode == GL_LINE ? GL_FILL : GL_LINE;
#endif
	else if (keyEvent->key() == Qt::Key_L)
		animation = !animation;
	else if (keyEvent->key() == Qt::Key_W)
		yTrans -= 0.1;
	else if (keyEvent->key() == Qt::Key_A)
		xTrans += 0.1;
	else if (keyEvent->key() == Qt::Key_S)
		yTrans += 0.1;
	else if (keyEvent->key() == Qt::Key_D)
		xTrans -= 0.1;
	else if (keyEvent->key() == Qt::Key_Up)
		xRot += 15;
	else if (keyEvent->key() == Qt::Key_Down)
		xRot -= 15;
	else if (keyEvent->key() == Qt::Key_Left)
		yRot += 15;
	else if (keyEvent->key() == Qt::Key_Right)
		yRot -= 15;
	else {
		std::cout << std::hex << keyEvent->key() << std::endl;
	}
}

void OpenGLWidget::wheelEvent(QWheelEvent *event) {
	distance *= 1.0 + (1.0 * event->angleDelta().y() / 1200.0);
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
	lastPos = event->pos();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
	int dx = event->position().x() - lastPos.x();
	int dy = event->position().y() - lastPos.y();

	if (event->buttons() & Qt::RightButton)
	{
		rotateBy(dy*8, 0, 0);
		rotateBy(0, dx*8, 0);
	}
	lastPos = event->pos();
}

void OpenGLWidget::rotateBy(int x, int y, int z) {
	xRot += x / 15;
	yRot += y / 15;
	zRot += z / 15;
}

void OpenGLWidget::timeOutSlot() {
	update();
}

OpenGLWidget::~OpenGLWidget() {
	std::cout << C_MSG("Test destructor called") << std::endl;
	delete texture;
}
