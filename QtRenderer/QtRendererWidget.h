#pragma once

#include "ui_QtRendererWidget.h"
#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QString>
#include <qevent.h>
#include "scene.h"
#include "model.h"
#include "camera.h"

#define DEFAULT_WIDTH 600
#define DEFAULT_HEIGHT 600
#define DEFAULT_DEPTH 500.f


class QtRendererWidget : public QWidget {
	Q_OBJECT
public:
	QtRendererWidget(QWidget* parent = 0);
	~QtRendererWidget() {};
	void initDevice();
	void paintEvent(QPaintEvent* paintevent);
	void loadModel(QString path);
	void saveImage(QString path);
	Matrix viewport(int x, int y, int w, int h);

	void wheelEvent(QWheelEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void processInput();
	
public slots:
	void render();
private:
	int width;
	int height;
	Ui::QtRendererWidget* ui;

	QTimer timer;
	int lastFrameTime;

	Model* model;
	Camera camera;
	Vec3f lightDir;
	
	Qt::MouseButtons currentButtons;
	QPoint lastPos;
	QPoint currentPos;

	float ratio;
};