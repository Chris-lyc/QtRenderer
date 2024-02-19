#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtRenderer.h"
#include "QtRendererWidget.h"
#include <QToolButton>

class QtRenderer : public QMainWindow
{
    Q_OBJECT

public:
    QToolBar* pToolBar;
    QAction* Action_SaveImage;
    QAction* Action_LoadModel;
    QAction* Action_ShowTime;
    
    QToolButton* Button_ShadowMenu;
    QMenu* Menu_ShadowMenu;
    QAction* Action_ShadowON;
    QAction* Action_ShadowOFF;

    QToolButton* Button_ZbufferType;
    QMenu* Menu_ZbufferType;
    QAction* Action_Zbuffer;
    QAction* Action_ScanZbuffer;
    QAction* Action_HiZ;
    QAction* Action_HiZOct;

    QToolButton* Button_MultiThread;
    QMenu* Menu_MultiThread;
    QAction* Action_MultiThreadON;
    QAction* Action_MultiThreadOFF;

    QToolButton* Button_Shading;
    QMenu* Menu_Shading;
    QAction* Action_NULLShaing;
    QAction* Action_PhongShading;
    
private:
    Ui::QtRendererClass ui;
    QtRendererWidget* qtrenderer_widget_;
    
public:
    QtRenderer(QWidget *parent = nullptr);
    ~QtRenderer();
    void Create_Action();
    void Creat_ToolBar();

protected:
    void closeEvent(QCloseEvent* e);
    void paintEvent(QPaintEvent* paintevent);

private:
    void on_action_LoadModel_trigger();
    void on_action_SaveImage_trigger();
    void on_action_ShowTime_trigger();
    
    void on_action_ShdowON_trigger();
    void on_action_ShadowOFF_trigger();

    void on_action_Zbuffer_trigger();
    void on_action_ScanZbuffer_trigger();
    void on_action_HiZ_trigger();
    void on_action_HiZOct_trigger();

    void on_action_MultiThreadON_trigger();
    void on_action_MultiThreadOFF_trigger();

    void on_action_NULLShaing_trigger();
    void on_action_PhongShaing_trigger();
};
