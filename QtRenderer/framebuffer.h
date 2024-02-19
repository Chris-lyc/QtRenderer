#pragma once

#include "dataStructure.hpp"
#include "Hierarchy Zbuffer/zPyramid.h"
#include <Qstring>
#include <QImage>
#include <QColor>

class FrameBuffer
{
public:
    FrameBuffer(int _w, int _h, int zbufferType);
    bool JudgeDepth(int x, int y, float z);
    void SetPixel(int x, int y, Color color);
    bool SaveImage(QString filePath);
    void ClearBuffer(Color color);
    QImage& GetImage();
    Buffer* GetDepthBuffer();
    int get_width();
    int get_height();

    void ClearColorBuffer(Color color);
    
private:
    int width;
    int height;
    Buffer depthBuffer;
    QImage colorBuffer;

    
};