#pragma once

#include "tgaimage.h"
#include <Qstring>
#include <QImage>
#include <QColor>
#include <vector>

class FrameBuffer
{
public:
    FrameBuffer(int _w, int _h);
    bool JudgeDepth(int x, int y, float z);
    void SetPixel(int x, int y, TGAColor color);
    bool SaveImage(QString filePath);
    void ClearBuffer(TGAColor color);
    QImage& GetImage() { return colorBuffer; }
    int get_width();
    int get_height();
private:
    int width;
    int height;
    std::vector<float> depthBuffer;
    QImage colorBuffer;
};