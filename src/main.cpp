#define STB_IMAGE_IMPLEMENTATION
#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec2 TexCoord;

    out vec4 FragColor;

    uniform sampler2D texture1;

    void main()
    {
        FragColor = texture(texture1, TexCoord);
    }
)";

// Texture paths

const char* texturePaths[] = {
    "/home/hazem/opengl/r/r1.png",
    "/home/hazem/opengl/r/r2.png",
    "/home/hazem/opengl/r/r3.png",
    "/home/hazem/opengl/r/r4.png",
    "/home/hazem/opengl/r/r5.png",
    "/home/hazem/opengl/r/r6.png",
    "/home/hazem/opengl/r/r7.png",
    "/home/hazem/opengl/r/r8.png"
};

const char* t_src[] = {
    "/home/hazem/opengl/t/t1.png",
    "/home/hazem/opengl/t/t2.png",
    "/home/hazem/opengl/t/t3.png",
    "/home/hazem/opengl/t/t4.png",
    "/home/hazem/opengl/t/t5.png",
    "/home/hazem/opengl/t/t6.png",
    "/home/hazem/opengl/t/t7.png",
    "/home/hazem/opengl/t/t8.png"
};

const char* g_src[] = {
    "/home/hazem/opengl/g/g1.png",
    "/home/hazem/opengl/g/g2.png",
    "/home/hazem/opengl/g/g3.png",
    "/home/hazem/opengl/g/g4.png",
    "/home/hazem/opengl/g/g5.png",
    "/home/hazem/opengl/g/g6.png",
    "/home/hazem/opengl/g/g7.png",
    "/home/hazem/opengl/g/g8.png"
};

const char* m_src[] = {
    "/home/hazem/opengl/m/m1.png",
    "/home/hazem/opengl/m/m2.png",
    "/home/hazem/opengl/m/m3.png",
    "/home/hazem/opengl/m/m4.png",
    "/home/hazem/opengl/m/m5.png",
    "/home/hazem/opengl/m/m6.png",
    "/home/hazem/opengl/m/m7.png",
    "/home/hazem/opengl/m/m8.png"
};

const char* r_src[] = {
    "/home/hazem/opengl/r/r1.png",
    "/home/hazem/opengl/r/r2.png",
    "/home/hazem/opengl/r/r3.png",
    "/home/hazem/opengl/r/r4.png",
    "/home/hazem/opengl/r/r5.png",
    "/home/hazem/opengl/r/r6.png",
    "/home/hazem/opengl/r/r7.png",
    "/home/hazem/opengl/r/r8.png"
};

int current_anim = 0;


int currentTextureIndex = 0;
double lastTextureChangeTime = 0.0;
double textureChangeInterval = 0.1;
GLuint texture; 

void changeTexture()
{
    int size = 0;
    
    currentTextureIndex = (currentTextureIndex + 1) % (sizeof(texturePaths) / sizeof(texturePaths[0]));
    const char* texturePath = texturePaths[currentTextureIndex];
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data == nullptr)
    {
        std::cout << "Failed to load texture: " << texturePath << std::endl;
        return;
    }

    glBindTexture(GL_TEXTURE_2D, texture);

    if (nrChannels == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (nrChannels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void transfer(const char * first[8],const char * second[8]){
    for(int k =0;k<8;k++){
        first[k] = second[k];
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        switch (current_anim)
        {
        case 0:
            transfer(texturePaths,t_src);
            break;
        case 1:
            transfer(texturePaths,g_src);
            break;
        case 2:
            transfer(texturePaths,m_src);
            break;
        case 3:
            transfer(texturePaths,r_src);
            break;
        
        default:
            current_anim=-1;
            break;
        }
        current_anim++;
    }
	changeTexture();
}
int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1800, 900, "Animations", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
        return -1;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
        return -1;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program linking failed: " << infoLog << std::endl;
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        -0.3f, -0.9f, 0.0f,  0.0f, 1.0f,
        0.3f, -0.9f, 0.0f,  1.0f, 1.0f,
        0.3f,  0.9f, 0.0f,  1.0f, 0.0f,
        -0.3f,  0.9f, 0.0f,  0.0f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    int width, height, nrChannels;
    const char* initialTexturePath = texturePaths[currentTextureIndex];
    unsigned char* data = stbi_load(initialTexturePath, &width, &height, &nrChannels, 0);
    if (data == nullptr)
    {
        std::cout << "Failed to load initial texture: " << initialTexturePath << std::endl;
        return -1;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (nrChannels == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (nrChannels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindTexture(GL_TEXTURE_2D, texture);

        double currentTime = glfwGetTime();
        if (currentTime - lastTextureChangeTime >= textureChangeInterval)
        {
            changeTexture();
            lastTextureChangeTime = currentTime;
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}
