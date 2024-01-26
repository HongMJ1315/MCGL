#include "draw.h"

void GlInit(){
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(SUN_LIGHT);
    glEnable(HEAD_LIGHT);
}

int FTInit(){
    // 初始化FreeType
    if(FT_Init_FreeType(&ft)){
        return -1;
    }

    // 載入字型文件
    if(FT_New_Face(ft, "path/to/your/font.ttf", 0, &ftFace)){
        return -1;
    }

    // 設置字型大小
    FT_Set_Pixel_Sizes(ftFace, 0, 24);

}

void Move(float dx, float dy, float dz){
    glMatrixMode(GL_MODELVIEW);
    int hfront = (int) floor(HEIGHT / 2 + (dx / MOVE_SPEED));
    int ffront = (int) floor(WIDTH / 2 + (dz / MOVE_SPEED));
    std::cout << hfront << " " << ffront << std::endl;
    if(graph[hfront][(int) floor(cameraPos.y - 1)][ffront] != 0 ||
        graph[hfront][(int) floor(cameraPos.y)][ffront] != 0){
        return;
    }
    cameraPos.x += GRAPH_SACLE * dx;
    cameraPos.y += GRAPH_SACLE * dy;
    cameraPos.z += GRAPH_SACLE * dz;
    lookPoint.x += GRAPH_SACLE * dx;
    lookPoint.y += GRAPH_SACLE * dy;
    lookPoint.z += GRAPH_SACLE * dz;
    drawRightDownPoint[0] += dx;
    // drawRightDownPoint[1] += dy;
    drawRightDownPoint[2] += dz;
}

void Update(){
    if(keyboardState['W']){
        float dx = MOVE_SPEED * sin(degree * PI / 180.0f);
        float dz = MOVE_SPEED * cos(degree * PI / 180.0f);
        Move(dx, 0.0f, -dz);
    }
    if(keyboardState['S']){
        float dx = MOVE_SPEED * sin(degree * PI / 180.0f);
        float dz = MOVE_SPEED * cos(degree * PI / 180.0f);
        Move(-dx, 0.0f, dz);
    }
    if(keyboardState['A']){
        float dx = MOVE_SPEED * sin((degree + 90) * PI / 180.0f);
        float dz = MOVE_SPEED * cos((degree + 90) * PI / 180.0f);
        Move(-dx, 0.0f, dz);
    }
    if(keyboardState['D']){
        float dx = MOVE_SPEED * sin((degree + 90) * PI / 180.0f);
        float dz = MOVE_SPEED * cos((degree + 90) * PI / 180.0f);
        Move(dx, 0.0f, -dz);
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



void SingleView(GLFWwindow *window){
    glMatrixMode(GL_MODELVIEW);

    SetLook(cameraPos);
    DrawView();
}

void Display(GLFWwindow *window){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    SingleView(window);
    DrawCoordinateString();
    glfwSwapBuffers(window);
    // DropPositon();
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

void KeyboardEvent(GLFWwindow *window, int key, int scancode, int action, int mods){
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
            // 0
            case GLFW_KEY_0:
            viewMode = !viewMode;
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
int main(int argc, char **argv){
    glutInit(&argc, argv);
    if(!glfwInit()){
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(width, height, "Perlin Noise 3D", nullptr, nullptr);
    if(!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, KeyboardEvent);
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
