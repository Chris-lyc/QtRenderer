#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtRenderer.h"
#include "QtRendererWidget.h"

class QtRenderer : public QMainWindow
{
    Q_OBJECT

public:
    QtRenderer(QWidget *parent = nullptr);
    ~QtRenderer();

    QToolBar* pToolBar;
    QAction* Action_SaveImage;
    QAction* Action_LoadModel;
    
    void Create_Action();
    void Creat_ToolBar();
protected:
    void closeEvent(QCloseEvent* e);
    void paintEvent(QPaintEvent* paintevent);
private:
    Ui::QtRendererClass ui;
    QtRendererWidget* qtrenderer_widget_;

    void on_action_LoadModel_trigger();
    void on_action_SaveImage_trigger();
};
