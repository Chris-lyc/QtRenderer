#pragma once

#include "ui_QtRendererWidget.h"
#include <QWidget>
#include <QTimer>
#include <QString>
#include <qevent.h>
#include "renderInstance.h"
#include "model.h"
#include "camera.h"

#define DEFAULT_WIDTH 400
#define DEFAULT_HEIGHT 400
#define DEFAULT_DEPTH 500.f


class QtRendererWidget : public QWidget {
	Q_OBJECT
public:
	QtRendererWidget(QWidget* parent = 0);
	~QtRendererWidget() { delete(ui); delete(model); delete(camera); }
	void initDevice();
	void paintEvent(QPaintEvent* paintevent);
	void loadModel(std::string path);
	void saveImage(QString path);
	void setUseShadow(bool use);
	void setZbufferType(int type);
	void setMultiThread(bool multiThread);
	void setShadingType(int type);
	
	Matrix viewport(int x, int y, int w, int h);

	void wheelEvent(QWheelEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void processInput();

	void displayZbufferType();
	void displayMultiThread();
	void displayTime();
public slots:
	void render();

private:
	int width;
	int height;
	float depth;
	Ui::QtRendererWidget* ui;

	QTimer timer;
	int lastFrameTime;

	Model* model;
	Camera* camera;
	Vec3f lightDir;
	
	Qt::MouseButtons currentButtons;
	QPoint lastPos;
	QPoint currentPos;

	float ratio;

	int deltaTime;

	bool useShadow;
	int zbufferType;
	bool multiThread;
	int shadingType;
};