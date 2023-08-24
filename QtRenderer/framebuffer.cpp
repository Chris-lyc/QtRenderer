#include "framebuffer.h"
#include <iostream>
FrameBuffer::FrameBuffer(int _w, int _h) :width(_w), height(_h),
depthBuffer(width * height),
//depthBuffer(width, height, QImage::Format_Grayscale8),
colorBuffer(width, height, QImage::Format_RGB888) {
    colorBuffer.fill(QColor(0.f, 0.f, 0.f));
    //depthBuffer.fill(QColor(255.f, 255.f, 255.f));
    //depthBuffer.fill(QColor(0.f, 0.f, 0.f));
    std::fill(depthBuffer.begin(), depthBuffer.end(), std::numeric_limits<float>::max());
}

bool FrameBuffer::JudgeDepth(int x, int y, float z) {
    //if (z < depthBuffer.pixelColor(x, height - 1 - y).red()) {
    //    depthBuffer.setPixelColor(x, height - 1 - y, QColor(z, z, z));
    //    return true;
    //}

    //int index = y * width + x;
    //if (index >= width * height || index < 0) {
    //    std::cout << index << std::endl;
    //    std::cout << "no" << std::endl;
    //}

    if (z < depthBuffer[y * width + x]) {
        //std::cout << z << std::endl;
        depthBuffer[y * width + x] = z;
        return true;
    }
    return false;
}

void FrameBuffer::SetPixel(int x, int y, TGAColor color) {
    colorBuffer.setPixelColor(x, height - 1 - y, QColor(color[0], color[1], color[2]));
}

void FrameBuffer::ClearBuffer(TGAColor color) {
    std::fill(depthBuffer.begin(), depthBuffer.end(), std::numeric_limits<float>::max());
    //depthBuffer.fill(QColor(255.f, 255.f, 255.f));
    //depthBuffer.fill(QColor(0.f, 0.f, 0.f));
    //colorBuffer.fill(QColor(color[0] * 255.f, color[1] * 255.f, color[2] * 255.f));
    colorBuffer.fill(QColor(color[0], color[1], color[2]));
}

bool FrameBuffer::SaveImage(QString filePath) {
    //return depthBuffer.save(filePath);
    return colorBuffer.save(filePath);
}

int FrameBuffer::get_width() {
    return width;
}

int FrameBuffer::get_height() {
    return height;
}