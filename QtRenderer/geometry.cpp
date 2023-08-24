#include "geometry.h"

template <> template <> vec<4, int>  ::vec(const vec<4, float>& v) : x(int(v.x + .5f)), y(int(v.y + .5f)), z(int(v.z + .5f)), w(int(v.w+.5f)) {}
template <> template <> vec<4, float>::vec(const vec<4, int>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
template <> template <> vec<3, int>  ::vec(const vec<3, float>& v) : x(int(v.x + .5f)), y(int(v.y + .5f)), z(int(v.z + .5f)) {}
template <> template <> vec<3, float>::vec(const vec<3, int>& v) : x(v.x), y(v.y), z(v.z) {}
template <> template <> vec<2, int>  ::vec(const vec<2, float>& v) : x(int(v.x + .5f)), y(int(v.y + .5f)) {}
template <> template <> vec<2, float>::vec(const vec<2, int>& v) : x(v.x), y(v.y) {}

float radians(float angle) {
    return angle / 180.f * M_PI;
}

Matrix lookat(Vec3f position, Vec3f target, Vec3f up) {
    Vec3f z = (position - target).normalize();
    Vec3f x = cross(up, z).normalize();
    Vec3f y = cross(z, x).normalize();
    Matrix View = Matrix::identity();
    Matrix pan = Matrix::identity();
    for (int i = 0; i < 3; ++i) {
        View[0][i] = x[i];
        View[1][i] = y[i];
        View[2][i] = z[i];
        pan[i][3] = -position[i];
    }
    View = View * pan;
    return View;
}

Matrix perspective(float fovy, float aspect, float near, float far) {
    float z_range = far - near;
    Matrix m = Matrix::identity();
    //std::cout << fovy << " " << aspect << std::endl;
    assert(fovy > 0 && aspect > 0);
    assert(near > 0 && far > 0 && z_range > 0);
    m[1][1] = 1 / (float)tan(fovy / 2);
    m[0][0] = m[1][1] / aspect;
    m[2][2] = -(near + far) / z_range;
    m[2][3] = -2 * near * far / z_range;
    m[3][2] = -1;
    m[3][3] = 0;
    return m;
}

Matrix projection(float coeff) {
    Matrix Projection = Matrix::identity();
    Projection[3][2] = coeff;
    return Projection;
}
