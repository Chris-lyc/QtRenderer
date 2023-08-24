#include "QtRenderer.h"
#include <QFileDialog>

QtRenderer::QtRenderer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    qtrenderer_widget_ = new QtRendererWidget();
    setCentralWidget(qtrenderer_widget_);
    Create_Action();
    Creat_ToolBar();
}

QtRenderer::~QtRenderer()
{}

void QtRenderer::closeEvent(QCloseEvent* e)
{}

void QtRenderer::paintEvent(QPaintEvent* paintevent)
{}

void QtRenderer::Creat_ToolBar() {
    pToolBar = addToolBar(tr("&Main"));
    pToolBar->addAction(Action_LoadModel);
    pToolBar->addAction(Action_SaveImage);
    
}

void QtRenderer::on_action_LoadModel_trigger() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Model File", "", "OBJ(*.obj)");
    if (!filePath.isEmpty())
        qtrenderer_widget_->loadModel(filePath);
}

void QtRenderer::on_action_SaveImage_trigger() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG(*.png)");
    if (!fileName.isEmpty())
        qtrenderer_widget_->saveImage(fileName);
}

void QtRenderer::Create_Action() {
    Action_LoadModel = new QAction(tr("&Model"), this);
    connect(Action_LoadModel, &QAction::triggered, this, &QtRenderer::on_action_LoadModel_trigger);
    Action_SaveImage = new QAction(tr("&Save"), this);
    connect(Action_SaveImage, &QAction::triggered, this, &QtRenderer::on_action_SaveImage_trigger);
    
}
