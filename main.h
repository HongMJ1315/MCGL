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
#include <GL/glut.h>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#define DEFAULT_HEIGHT  32 // x
#define DEFAULT_LENGTH  32 // y
#define DEFAULT_WIDTH   32 // z

#define GRAPH_SACLE 1.0f

#define ROTATE_SPEED 5.0f
#define MOVE_SPEED 0.1f

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
int faces[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };

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

FT_Library ft;
FT_Face ftFace;

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

void SetLook(glm::vec3 pos){
    glViewport(0, 0, width, height);
    gluPerspective(60.0f, (float) width / (float) height, 0.1f, 10000.0f);
}


void GraphInit(){
    unsigned int seed = 1;
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
        if(graph[HEIGHT / 2][i - 2][WIDTH / 2] != 0){
            cameraPos = glm::vec3(0.5, i + 1, 0.5) * GRAPH_SACLE;
            lookPoint = glm::vec3(0.5, i + 1, 0 + 1.5) * GRAPH_SACLE;
            break;
        }
    }
}

#endif // MAIN_H