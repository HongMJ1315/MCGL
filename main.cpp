#include "main.h"

void GlInit(){
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glEnable(GL_LIGHTING);
    glLoadIdentity();

    glEnable(SUN_LIGHT);
    glEnable(HEAD_LIGHT);
}


void Update(){

    if(keyboardState['W']){
        cameraPos.z -= GRAPH_SACLE * MOVE_SPEED * cos(degree * PI / 180.0f);
        cameraPos.x += GRAPH_SACLE * MOVE_SPEED * sin(degree * PI / 180.0f);
        lookPoint.z -= GRAPH_SACLE * MOVE_SPEED * cos(degree * PI / 180.0f);
        lookPoint.x += GRAPH_SACLE * MOVE_SPEED * sin(degree * PI / 180.0f);
        drawRightDownPoint[0] += MOVE_SPEED * sin(degree * PI / 180.0f);
        drawRightDownPoint[2] -= MOVE_SPEED * cos(degree * PI / 180.0f);
    }
    if(keyboardState['S']){
        cameraPos.z += GRAPH_SACLE * MOVE_SPEED * cos(degree * PI / 180.0f);
        cameraPos.x -= GRAPH_SACLE * MOVE_SPEED * sin(degree * PI / 180.0f);
        lookPoint.z += GRAPH_SACLE * MOVE_SPEED * cos(degree * PI / 180.0f);
        lookPoint.x -= GRAPH_SACLE * MOVE_SPEED * sin(degree * PI / 180.0f);
        drawRightDownPoint[0] -= MOVE_SPEED * sin(degree * PI / 180.0f);
        drawRightDownPoint[2] += MOVE_SPEED * cos(degree * PI / 180.0f);
    }
    if(keyboardState['A']){
        cameraPos.z += GRAPH_SACLE * MOVE_SPEED * cos((degree + 90) * PI / 180.0f);
        cameraPos.x -= GRAPH_SACLE * MOVE_SPEED * sin((degree + 90) * PI / 180.0f);
        lookPoint.z += GRAPH_SACLE * MOVE_SPEED * cos((degree + 90) * PI / 180.0f);
        lookPoint.x -= GRAPH_SACLE * MOVE_SPEED * sin((degree + 90) * PI / 180.0f);
        drawRightDownPoint[0] -= MOVE_SPEED * sin((degree + 90) * PI / 180.0f);
        drawRightDownPoint[2] += MOVE_SPEED * cos((degree + 90) * PI / 180.0f);
    }
    if(keyboardState['D']){
        cameraPos.z -= GRAPH_SACLE * MOVE_SPEED * cos((degree + 90) * PI / 180.0f);
        cameraPos.x += GRAPH_SACLE * MOVE_SPEED * sin((degree + 90) * PI / 180.0f);
        lookPoint.z -= GRAPH_SACLE * MOVE_SPEED * cos((degree + 90) * PI / 180.0f);
        lookPoint.x += GRAPH_SACLE * MOVE_SPEED * sin((degree + 90) * PI / 180.0f);
        drawRightDownPoint[0] += MOVE_SPEED * sin((degree + 90) * PI / 180.0f);
        drawRightDownPoint[2] -= MOVE_SPEED * cos((degree + 90) * PI / 180.0f);
    }

    // Rotate
    if(keyboardState['I']){
        lookPoint = MoveCameraUD(cameraPos, lookPoint, -ROTATE_SPEED);
    }
    if(keyboardState['K']){
        lookPoint = MoveCameraUD(cameraPos, lookPoint, ROTATE_SPEED);
    }
    if(keyboardState['J']){
        lookPoint = MoveCameraLR(cameraPos, lookPoint, -ROTATE_SPEED);
        degree -= ROTATE_SPEED;
        if(degree < 0.0f)
            degree += 360.0f;
    }
    if(keyboardState['L']){
        lookPoint = MoveCameraLR(cameraPos, lookPoint, ROTATE_SPEED);
        degree += ROTATE_SPEED;
        if(degree > 360.0f)
            degree -= 360.0f;
    }

    if(directionKey[0]){
        cameraPos.y += MOVE_SPEED * GRAPH_SACLE;
        lookPoint.y += MOVE_SPEED * GRAPH_SACLE;
    }
    if(directionKey[1]){
        cameraPos.y -= MOVE_SPEED * GRAPH_SACLE;
        lookPoint.y -= MOVE_SPEED * GRAPH_SACLE;
    }
    GraphInit();
    glfwPostEmptyEvent();
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
                int color = graph[i][j][k];
                Cube(colorArray[color][0], colorArray[color][1], colorArray[color][2]);
                glPopMatrix();
            }
        }
    }
    glPopMatrix();
}

void Display(GLFWwindow *window){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(60.0f, (float) width / (float) height, 0.1f, 10000.0f);
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, lookPoint.x, lookPoint.y, lookPoint.z, 0.0f, 1.0f, 0.0f);
    // DrawHeadLight(glm::vec3(0.5, 0.5, 0.5), cameraos, lookPoint - cameraPos, 30.0f, 1.0f);
    DrawSunLight({ 0.8f, 0.8f, 0.8f }, 1.0f);
    DrawGraph();
    // DrawFloor();
    glfwSwapBuffers(window);
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        // 在這裡處理錯誤，例如輸出錯誤信息
        std::cerr << "OpenGL error: " << gluErrorString(error) << std::endl;
    }

}


void Reshape(GLFWwindow *window, int w, int h){
    width = w;
    height = h;
    Display(window);
}

void KeyboardDown(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        keyboardState[key] = true;

        switch(key){
            case GLFW_KEY_MINUS:
            perlinFreq -= 0.001f;
            GraphInit();
            break;
            case GLFW_KEY_EQUAL:
            perlinFreq += 0.001f;
            GraphInit();
            break;
            case GLFW_KEY_UP:
            directionKey[0] = true;
            break;
            case GLFW_KEY_DOWN:
            directionKey[1] = true;
            break;
            case GLFW_KEY_LEFT:
            directionKey[2] = true;
            break;
            case GLFW_KEY_RIGHT:
            directionKey[3] = true;
            break;
        }
    }
    else if(action == GLFW_RELEASE){
        keyboardState[key] = false;
        switch(key){
            case GLFW_KEY_UP:
            directionKey[0] = false;
            break;
            case GLFW_KEY_DOWN:
            directionKey[1] = false;
            break;
            case GLFW_KEY_LEFT:
            directionKey[2] = false;
            break;
            case GLFW_KEY_RIGHT:
            directionKey[3] = false;
            break;
        }
    }
}
int main(){
    if(!glfwInit()){
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(width, height, "Perlin Noise 3D", nullptr, nullptr);
    if(!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, KeyboardDown);
    glfwSetWindowSizeCallback(window, Reshape);
    glfwSetCursorPosCallback(window, nullptr);
    glfwSetMouseButtonCallback(window, nullptr);

    GraphInit();
    GlInit();
    FindSpawnPoint();

    while(!glfwWindowShouldClose(window)){

        Update();
        Display(window);

        glfwPollEvents();
    }


    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
