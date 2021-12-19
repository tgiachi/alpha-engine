#include <string>
#include <utility>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
#include "shaderClass.h"
#include "entity.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"


void error_callback(int error, const char *msg) {
    spdlog::error("OpenGL error: [{}] -> {}", error, msg);
}

// Vertices coordinates
GLfloat vertices[] =
        { //     COORDINATES     /        COLORS      /   TexCoord  //
                -0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
                -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
                0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
                0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
                0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f
        };

// Indices for vertices order
GLuint indices[] =
        {
                0, 1, 2,
                0, 2, 3,
                0, 1, 4,
                1, 2, 4,
                2, 3, 4,
                3, 0, 4
        };

class AlphaEngine {
private:
    int m_width;
    int m_height;
    int m_frames = 0;
    float m_limitFps;
    int m_timer = 0;
    std::string m_title;
    GLFWwindow *m_window;
    Shader *m_shaders;
    Camera *m_camera;
    std::vector<Entity *> m_entities;
    float m_lastDeltaTime = 0;
    float m_deltaTime = 0;
    float m_currentDeltaTime = 0;
    glm::mat4 m_model = glm::mat4(1.0f);
    glm::mat4 m_view = glm::mat4(1.0f);
    glm::mat4 m_proj = glm::mat4(1.0f);
    float m_rotation = 0.0f;


    void loadShaders();


public:
    AlphaEngine(int width, int height, std::string title, int limitFps) {
        m_width = width;
        m_height = height;
        m_title = std::move(title);
        m_limitFps = 1.0f / limitFps;
    };

    bool init();

    void loop();

    void addEntity(Entity *entity) {
        m_entities.push_back(entity);
        entity->init();
    }
};

bool AlphaEngine::init() {

    glfwSetErrorCallback(error_callback);
    // Initialize GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);

    // Error check if the window fails to create
    if (m_window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwSetKeyCallback(m_window, [](GLFWwindow *, int, int, int, int) {});

    // Introduce the window into the current context
    glfwMakeContextCurrent(m_window);

    gladLoadGL();

    //Load GLAD so it configures OpenGL
    // gladLoadGL();

    glViewport(0, 0, m_width, m_height);
    loadShaders();



    return true;

}

void AlphaEngine::loop() {

    m_lastDeltaTime = glfwGetTime();
    m_timer = m_lastDeltaTime;
    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));

    // Links VBO attributes such as coordinates and colors to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *) 0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
    GLuint uniID = glGetUniformLocation(m_shaders->ID, "scale");
    Texture brickTex("assets/textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    brickTex.texUnit(reinterpret_cast<Shader &>(m_shaders), "tex0", 0);
    m_camera = new Camera(m_width, m_height, glm::vec3(0.0f, 0.0f, 2.0f));
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(m_window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);


        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shaders->Activate();
        m_currentDeltaTime = (float) glfwGetTime();
        m_deltaTime += (m_currentDeltaTime - m_lastDeltaTime) / m_limitFps;


        for (auto entity: m_entities) {
            entity->processInput(m_window);
            entity->update(m_deltaTime);
            entity->render();
        }
        m_frames++;


        if (glfwGetTime() - m_timer > 1.0) {

            m_rotation += 0.5f;
            char buff[100];
            snprintf(buff, sizeof(buff), "%s [%i FPS]", m_title.c_str(), m_frames);
            m_timer++;
            spdlog::info("Frame per second: {}", m_frames);
            glfwSetWindowTitle(m_window, buff);
            m_frames = 0;
        }

        // Handles camera inputs
        m_camera->Inputs(m_window);
        // Updates and exports the camera matrix to the Vertex Shader
        m_camera->Matrix(45.0f, 0.1f, 100.0f, reinterpret_cast<Shader &>(m_shaders), "camMatrix");

        brickTex.Bind();
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(m_window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    brickTex.Delete();
    m_shaders->Delete();
    // Delete window before ending the program
    glfwDestroyWindow(m_window);
    // Terminate GLFW before ending the program
    glfwTerminate();
}

void AlphaEngine::loadShaders() {
    m_shaders = new Shader("assets/shaders/default.vert", "assets/shaders/default.frag");
}
