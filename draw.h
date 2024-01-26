#ifndef DRAW_H
#define DRAW_H

#include "main.h"


void DrawCoordinateString(){
    std::string coordinateStr = "x: " + std::to_string(cameraPos.x).substr(0, 5) + " y: " + std::to_string(cameraPos.y).substr(0, 5) + " z: " + std::to_string(cameraPos.z).substr(0, 5);
    std::string rightDownPointStr = "x: " + std::to_string(floor(drawRightDownPoint[0])).substr(0, 5) + " z: " + std::to_string(floor(drawRightDownPoint[2])).substr(0, 5);
    std::string standOn = "Floor: x: " + std::to_string(HEIGHT / 2) + " y: " + std::to_string((int) floor(std::min(cameraPos.y - 2, (float) LENGTH - 1))) + " z: " + std::to_string(WIDTH / 2) + std::to_string(graph[HEIGHT / 2][(int) floor(std::min(cameraPos.y - 2, (float) LENGTH - 1))][WIDTH / 2]);
    std::string seeBlock = "Front: x: " + std::to_string((int) floor(HEIGHT / 2 + sin(degree * PI / 180.0f))) + " y: " + std::to_string((int) floor(cameraPos.y - 1)) + " z: " + std::to_string((int) floor(WIDTH / 2 - cos(degree * PI / 180.0f))) + " Block : " + std::to_string(graph[(int) floor(HEIGHT / 2 + sin(degree * PI / 180.0f))][(int) floor(cameraPos.y - 1)][(int) floor(WIDTH / 2 - cos(degree * PI / 180.0f))]);


    glMatrixMode(GL_PROJECTION);
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2f(10, height - 30);
    for(int i = 0; i < coordinateStr.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, coordinateStr[i]);
    }
    glRasterPos2f(10, height - 60);
    for(int i = 0; i < rightDownPointStr.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, rightDownPointStr[i]);
    }
    glRasterPos2f(10, height - 90);
    for(int i = 0; i < standOn.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, standOn[i]);
    }
    glRasterPos2f(10, height - 120);
    for(int i = 0; i < seeBlock.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, seeBlock[i]);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}



void DrawGraph(){
    glMatrixMode(GL_MODELVIEW);
    glScalef(GRAPH_SACLE, GRAPH_SACLE, GRAPH_SACLE);
    glPushMatrix();
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < LENGTH; j++){
            for(int k = 0; k < WIDTH; k++){
                if(!graph[i][j][k])continue;
                glPushMatrix();
                glTranslatef(
                    (floor(drawRightDownPoint[0]) + i),
                    (floor(drawRightDownPoint[1]) + j),
                    (floor(drawRightDownPoint[2]) + k));
                int type = graph[i][j][k];
                // if(type == 1)
                Cube(colorArray[type][0], colorArray[type][1], colorArray[type][2]);
                glPopMatrix();
            }
        }
    }
    glPopMatrix();
}

void DrawHeadLight(glm::vec3 color, glm::vec3 loc, glm::vec3 dir, float cutoff, float intensity, bool isOn = true){
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    loc = loc + dir * 0.1f;
    glTranslatef(loc.x, loc.y, loc.z);

    float r = color.x * intensity;
    float g = color.y * intensity;
    float b = color.z * intensity;
    r = std::min(r, 1.0f);
    g = std::min(g, 1.0f);
    b = std::min(b, 1.0f);
    r = std::max(r, 0.0f);
    g = std::max(g, 0.0f);
    b = std::max(b, 0.0f);
    GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 1.0 };  // Light position (x, y, z, w)
    GLfloat lightAmbient[] = { 0.3 * intensity * r, 0.3 * intensity * g, 0.3 * intensity * b, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { r, g, b, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { r, g, b, 1.0 };    // Specular light color (RGBA)

    glLightfv(HEAD_LIGHT, GL_POSITION, lightPosition);
    glLightfv(HEAD_LIGHT, GL_AMBIENT, lightAmbient);
    glLightfv(HEAD_LIGHT, GL_DIFFUSE, lightDiffuse);
    glLightfv(HEAD_LIGHT, GL_SPECULAR, lightSpecular);

    GLfloat spot_direction[] = { dir.x, dir.y, dir.z };
    glLightfv(HEAD_LIGHT, GL_SPOT_DIRECTION, spot_direction);
    glLightf(HEAD_LIGHT, GL_SPOT_CUTOFF, cutoff);
    glPopMatrix();
}

void DrawSunLight(glm::vec3 color, float instance){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(HEIGHT / 2, LENGTH + 10, WIDTH / 2);
    float r = color.x, g = color.y, b = color.z;
    GLfloat lightPosition[] = { 0, 0, 0, 0.0 };  // Light position (x, y, z, w), w=0 for directional light
    GLfloat lightAmbient[] = { r * 0.7 * instance, g * 0.7 * instance, b * 0.7 * instance, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { r, g, b, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { r, g, b, 1.0 };    // Specular light color (RGBA)
    GLfloat lightDirection[] = { 0, -1, 0 };  // Light direction for directional light
    glLightfv(SUN_LIGHT, GL_POSITION, lightPosition);
    glLightfv(SUN_LIGHT, GL_AMBIENT, lightAmbient);
    glLightfv(SUN_LIGHT, GL_DIFFUSE, lightDiffuse);
    glLightfv(SUN_LIGHT, GL_SPECULAR, lightSpecular);
    glLightfv(SUN_LIGHT, GL_SPOT_DIRECTION, lightDirection);
    glPopMatrix();
}

void Cube(float r, float g, float b){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    for(int i = 0; i < 6; i++){
        SetMaterial(EARTH, r, g, b);
        glBegin(GL_POLYGON);
        for(int j = 0; j < 4; j++){
            glVertex3fv(points[faces[i][j]]);
        }
        glEnd();

        // 繪製黑色邊緣
        SetMaterial(EARTH, 0, 0, 0);
        glBegin(GL_LINE_LOOP);
        for(int j = 0; j < 4; j++){
            glVertex3fv(points[faces[i][j]]);
        }
        glEnd();
    }
    glPopMatrix();
}


void DrawFloor(){
    int i, j;
    glMatrixMode(GL_MODELVIEW);
    for(i = 0; i < 25; i++)
        for(j = 0; j < 25; j++){
            if((i + j) % 2 == 0){
                SetMaterial(FLOOR, 1.0, 0.8, 0.8);
            }
            else{
                SetMaterial(FLOOR, 0.1, 0.1, 0.7);
            }
            glBegin(GL_POLYGON);
            glVertex3f((i - 5.0) * 10.0, -2.5, (j - 5.0) * 10.0);
            glVertex3f((i - 5.0) * 10.0, -2.5, (j - 4.0) * 10.0);
            glVertex3f((i - 4.0) * 10.0, -2.5, (j - 4.0) * 10.0);
            glVertex3f((i - 4.0) * 10.0, -2.5, (j - 5.0) * 10.0);
            glEnd();
        }
}

void DrawView(){
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glEnable(GL_LIGHTING);
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, lookPoint.x, lookPoint.y, lookPoint.z, 0.0f, 1.0f, 0.0f);
    // DrawHeadLight(glm::vec3(0.5, 0.5, 0.5), cameraos, lookPoint - cameraPos, 30.0f, 1.0f);
    DrawSunLight({ 0.8f, 0.8f, 0.8f }, 1.0f);
    DrawGraph();

    // DrawFloor();
}


#endif // DRAW_H