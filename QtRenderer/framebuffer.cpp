#include "framebuffer.h"

FrameBuffer::FrameBuffer(int _w, int _h,int zbufferType) : width(_w), height(_h),
                                           depthBuffer(width, height),
                                           colorBuffer(width, height, QImage::Format_RGB888)
{
    colorBuffer.fill(QColor(0.f, 0.f, 0.f));
    std::fill(depthBuffer.data, depthBuffer.data + (width * height), std::numeric_limits<float>::max());
    //
    // if(zbufferType==1)
    // {
    //     zpyramid=new zPyramid(_w,_h);
    // }
}

bool FrameBuffer::JudgeDepth(int x, int y, float z)
{
    // if(zbufferType==1)
    // {
    //     zpyramid->zTest(x,y,z);
    //     return true;
    // }
    
    float zBufferDepth = depthBuffer.getPixel(x, y);
    if (z < zBufferDepth)
    {
        depthBuffer.setPixel(x, y, z);
        return true;
    }
    return false;

    // if (z < depthBuffer.data[y * width + x]) {
    //     depthBuffer.data[y * width + x] = z;
    //     return true;
    // }
    // return false;
}

void FrameBuffer::SetPixel(int x, int y, Color color)
{
    colorBuffer.setPixelColor(x, height - 1 - y, QColor(color[0], color[1], color[2]));
}

void FrameBuffer::ClearBuffer(Color color)
{
    std::fill(depthBuffer.data, depthBuffer.data + (width * height), std::numeric_limits<float>::max());
    colorBuffer.fill(QColor(color[0], color[1], color[2]));
}

void FrameBuffer::ClearColorBuffer(Color color)
{
    colorBuffer.fill(QColor(color[0], color[1], color[2]));
}


QImage& FrameBuffer::GetImage()
{
    return colorBuffer;
}

Buffer* FrameBuffer::GetDepthBuffer()
{
    return &depthBuffer;
}

bool FrameBuffer::SaveImage(QString filePath)
{
    //return depthBuffer.save(filePath);
    return colorBuffer.save(filePath);
}

int FrameBuffer::get_width()
{
    return width;
}

int FrameBuffer::get_height()
{
    return height;
}

