#ifndef MATRIX_H
#define MATRIX_H
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286
#define ESP 1e-6


glm::mat3 RotationMatrix(glm::vec3 axis, float theta){
    axis = glm::normalize(axis);
    float a = cos(theta / 2.0f);
    float b, c, d;
    b = -axis.x * sin(theta / 2.0f);
    c = -axis.y * sin(theta / 2.0f);
    d = -axis.z * sin(theta / 2.0f);
    float aa, bb, cc, dd;
    aa = a * a; bb = b * b; cc = c * c; dd = d * d;
    float bc, ad, ac, ab, bd, cd;
    bc = b * c; ad = a * d; ac = a * c; ab = a * b; bd = b * d; cd = c * d;
    glm::mat3 rot_mat;
    rot_mat[0][0] = aa + bb - cc - dd;
    rot_mat[0][1] = 2 * (bc + ad);
    rot_mat[0][2] = 2 * (bd - ac);
    rot_mat[1][0] = 2 * (bc - ad);
    rot_mat[1][1] = aa + cc - bb - dd;
    rot_mat[1][2] = 2 * (cd + ab);
    rot_mat[2][0] = 2 * (bd + ac);
    rot_mat[2][1] = 2 * (cd - ab);
    rot_mat[2][2] = aa + dd - bb - cc;
    return rot_mat;
}

glm::vec3 RotateMatrix(glm::vec3 O, glm::vec3 P, float degree){
    float t = degree * PI / 180;

    // 計算從O到P的向量
    glm::vec3 OP = P - O;
    glm::vec3 Z(0, 0, 1);

    // 定義M平面的向量
    // glm::vec3 OM(O.x - P.x, O.y - P.y, 0);
    glm::vec3 OM(0, 1, 0);

    // 計算旋轉軸
    glm::vec3 axis = glm::cross(OP, OM);

    // 正規化旋轉軸
    axis = glm::normalize(axis);

    // 計算旋轉矩陣
    glm::mat3 R = RotationMatrix(axis, t);

    // 計算旋轉後的點
    glm::vec3 P_prime = R * OP + O;
    return P_prime;
}

glm::vec3 MoveCameraUD(glm::vec3 O, glm::vec3 P, float degree){
    // glm::vec3 O(helicopterX, helicopterY, helicopterZ);
    // glm::vec3 P(lookAtX, lookAtY, lookAtZ);
    glm::vec3 result = RotateMatrix(O, P, degree);
    glm::vec3 tresult = result - O;
    glm::vec3 tv(0, 1, 0);
    float cos_theta = glm::dot(tresult, tv) / (glm::length(tresult) * glm::length(tv));
    float angle = acos(cos_theta) * 180.0 / PI;

    if(angle < ESP || 180.0f - angle < ESP) return P;
    else return result;
}

glm::vec3 MoveCameraLR(glm::vec3 O, glm::vec3 P, float degree){
    //旋轉中心
    float center_x = O.x,
        center_z = O.z;
    //移回中心
    float tox = P.x - center_x,
        toz = P.z - center_z;

    degree = degree * PI / 180.0f;
    //旋轉
    float tx = tox * cos(degree) - toz * sin(degree),
        tz = tox * sin(degree) + toz * cos(degree);
    //移回
    return glm::vec3(tx + center_x, P.y, tz + center_z);
}
#endif