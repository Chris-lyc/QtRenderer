//#include "our_gl.h"
//
//Matrix View;
//Matrix Projection;
//Matrix ViewPort;
//const float depth = 255.f;
//
//void viewport(int x, int y, int w, int h) {
//    ViewPort = Matrix::identity();
//    ViewPort[0][3] = x + w / 2.f;
//    ViewPort[1][3] = y + h / 2.f;
//    ViewPort[2][3] = depth / 2.f;
//
//    ViewPort[0][0] = w / 2.f;
//    ViewPort[1][1] = h / 2.f;
//    ViewPort[2][2] = depth / 2.f;
//}
//
//void projection(float coeff) {
//    Projection = Matrix::identity();
//    Projection[3][2] = coeff;
//}
//
//void lookat(Vec3f eye, Vec3f center, Vec3f up) {
//    Vec3f z = (eye - center).normalize();
//    Vec3f x = cross(up, z).normalize();
//    Vec3f y = cross(z, x).normalize();
//    View = Matrix::identity();
//    for (int i = 0; i < 3; i++) {
//        View[0][i] = x[i];
//        View[1][i] = y[i];
//        View[2][i] = z[i];
//        View[i][3] = -center[i];
//    }
//}