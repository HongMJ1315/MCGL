#ifndef MAIN_H
#define MAIN_H

#include "PerlinNoise.hpp"
#include "matrix.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <algorithm>

// GLFW
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLU
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <GL/glu.h>

#define DEFAULT_HEIGHT  32 // x
#define DEFAULT_LENGTH  32 // y
#define DEFAULT_WIDTH   32 // z

#define GRAPH_SACLE 100.0f

#define ROTATE_SPEED 5.0f
#define MOVE_SPEED 1.0f

#define PERLIN_FREQ 0.1f
#define PERLIN_OCTAVE 5

#define SUN_LIGHT GL_LIGHT0
#define HEAD_LIGHT GL_LIGHT1

float perlinFreq = PERLIN_FREQ;
// Window size
int width = (2048 + 1024) / 2;
int height = width / 16 * 9;

// Camera position
glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 0.0f);
glm::vec3 lookPoint = glm::vec3(0.0f, 10.0f, 10.0f);
float degree = 180.0f;

float drawRightDownPoint[3] = { 0 - DEFAULT_HEIGHT / 2.0, 0, 0 - DEFAULT_WIDTH / 2.0 };

float points[][3] = { {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1} };
int face[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };

int graph[DEFAULT_HEIGHT][DEFAULT_LENGTH][DEFAULT_WIDTH];
int tmpGraph[DEFAULT_HEIGHT][DEFAULT_WIDTH];

bool keyboardState[1000];
bool directionKey[4];

bool viewMode = false;

int HEIGHT = DEFAULT_HEIGHT;
int LENGTH = DEFAULT_LENGTH;
int WIDTH = DEFAULT_WIDTH;

GLuint programID; // Shader program ID
GLuint vertexArrayID; // Vertex Array Object ID
GLuint vertexBufferID; // Vertex Buffer Object ID

enum material{
    FLOOR, EARTH
} MATERIAL;

enum blockType{
    AIR = 0, BEDROCK = 1, STONE, DIRT, GRASS
} BLOCK_TYPE;
float colorArray[][3] = {
    // 0
    {0, 0, 0},
    // bedrock
    {45 / 255.0f, 45 / 255.0f, 45 / 255.0f},
    // stone
    {97 / 255.0f, 106 / 255.0f, 107 / 255.0f},
    // dirt
    {142 / 255.0f, 86 / 255.0f, 26 / 255.0f},
    // grass
    {35 / 255.0f, 155 / 255.0f, 86 / 255.0f}
};

void Cube(float r, float g, float b);

void SetMaterial(material materialType, float r = 1, float g = 1, float b = 1){
    GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_shininess[] = { 0.0 };
    GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 0.0 };

    switch(materialType){
        case EARTH:
        mat_ambient[0] = r; mat_ambient[1] = g; mat_ambient[2] = b;
        mat_diffuse[0] = r; mat_diffuse[1] = g; mat_diffuse[2] = b;
        mat_specular[0] = 0.7; mat_specular[1] = 0.7; mat_specular[2] = 0.7;
        mat_shininess[0] = 30;
        break;

        case FLOOR:
        mat_ambient[0] = r; mat_ambient[1] = g; mat_ambient[2] = b;
        mat_diffuse[0] = r; mat_diffuse[1] = g; mat_diffuse[2] = b;
        mat_specular[0] = 0.0; mat_specular[1] = 0.0; mat_specular[2] = 0.0;
        mat_shininess[0] = 0.0;

        default:
        break;
    }

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
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
            glVertex3fv(points[face[i][j]]);
        }
        glEnd();

        // 繪製黑色邊緣
        SetMaterial(EARTH, 0, 0, 0);
        glBegin(GL_LINE_LOOP);
        for(int j = 0; j < 4; j++){
            glVertex3fv(points[face[i][j]]);
        }
        glEnd();
    }
    glPopMatrix();
}

void DrawAxisString(){
  
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

void SetLook(glm::vec3 pos){
    glViewport(0, 0, width, height);
    gluPerspective(60.0f, (float) width / (float) height, 0.1f, 10000.0f);
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

void DrawView(){
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, lookPoint.x, lookPoint.y, lookPoint.z, 0.0f, 1.0f, 0.0f);
    // DrawHeadLight(glm::vec3(0.5, 0.5, 0.5), cameraos, lookPoint - cameraPos, 30.0f, 1.0f);
    DrawAxisString();
    DrawSunLight({ 0.8f, 0.8f, 0.8f }, 1.0f);
    DrawGraph();
    // DrawFloor();
}

void GraphInit(){
    unsigned int seed = 0;
    std::mt19937 mt{ seed };
    siv::PerlinNoise perlin{ mt() };

    for(int h = 0; h < HEIGHT; h++){
        for(int i = 0; i < LENGTH; i++){
            for(int j = 0; j < WIDTH; j++){
                float n = perlin.normalizedOctave3D_01(
                    (h + floor(drawRightDownPoint[0])) * perlinFreq,
                    (i + floor(drawRightDownPoint[1])) * perlinFreq,
                    (j + floor(drawRightDownPoint[2])) * perlinFreq,
                    PERLIN_OCTAVE);
                graph[h][i][j] = (n > 0.5 ? blockType::STONE : blockType::AIR);
            }
        }
    }

    perlin.reseed(mt());
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            float n = perlin.normalizedOctave2D_01(
                (i + floor(drawRightDownPoint[0])) * perlinFreq * 2,
                (j + floor(drawRightDownPoint[2])) * perlinFreq * 2,
                PERLIN_OCTAVE);
            int nn = std::min(n * 20 + LENGTH * 1 / 4, float(LENGTH - 1));
            tmpGraph[i][j] = nn;
        }
    }
    perlin.reseed(mt());
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            float n = perlin.normalizedOctave2D_01(
                (i + floor(drawRightDownPoint[0])) * perlinFreq * 2,
                (j + floor(drawRightDownPoint[2])) * perlinFreq * 2,
                PERLIN_OCTAVE);
            int nn = std::min(n * 20 + LENGTH * 2 / 4, float(LENGTH - 1));
            for(int k = tmpGraph[i][j]; k < nn; k++){
                if(graph[i][k][j] == 0) continue;
                graph[i][k][j] = blockType::DIRT;
            }
            for(int k = nn + 1; k < LENGTH; k++)
                graph[i][k][j] = blockType::AIR;
            if(graph[i][nn][j] == 0) continue;
            graph[i][nn][j] = blockType::GRASS;
        }
    }
    perlin.reseed(mt());
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            for(int k = 1; k < 3; k++){
                graph[i][k][j] = (mt() % 2 ? blockType::BEDROCK : graph[i][k][j]);
            }
            graph[i][0][j] = blockType::BEDROCK;
        }
    }

}

void FindSpawnPoint(){
    for(int i = LENGTH - 1; i >= 0; i--){
        if(graph[HEIGHT / 2][i][WIDTH / 2] == 0){
            cameraPos = glm::vec3(0, i + 1, 0) * GRAPH_SACLE;
            lookPoint = glm::vec3(0, i + 1, 0 + 1) * GRAPH_SACLE;
            break;
        }
    }
}



#endif // MAIN_H