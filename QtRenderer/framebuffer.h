#pragma once

#include "dataStructure.hpp"
#include <Qstring>
#include <QImage>
#include <QColor>
#include <vector>

class FrameBuffer
{
public:
    FrameBuffer(int _w, int _h);
    bool JudgeDepth(int x, int y, float z);
    void SetPixel(int x, int y, Color color);
    bool SaveImage(QString filePath);
    void ClearBuffer(Color color);
    QImage& GetImage();
    Buffer* GetDepthBuffer();
    int get_width();
    int get_height();
private:
    int width;
    int height;
    Buffer depthBuffer;
    QImage colorBuffer;
};