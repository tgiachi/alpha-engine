#include <string>
#include <utility>


#include "glad/glad.h"

#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
#include "shaderClass.h"
#include "entity.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



void error_callback(int error, const char *msg) {
    spdlog::error("OpenGL error: [{}] -> {}", error, msg);
}

class AlphaEngine {
private:
    int m_width;
    int m_height;
    int m_frames = 0;
    float m_limitFps;
    int m_timer = 0;
    std::string m_title;
    GLFWwindow *m_window;
    std::vector<Entity *> m_entities;
    float m_lastDeltaTime = 0;
    float m_deltaTime = 0;
    float m_currentDeltaTime = 0;

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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

    return true;

}

void AlphaEngine::loop() {

    m_lastDeltaTime = glfwGetTime();
    m_timer = m_lastDeltaTime;
    while (!glfwWindowShouldClose(m_window)) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);


        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_currentDeltaTime = (float) glfwGetTime();
        m_deltaTime += (m_currentDeltaTime - m_lastDeltaTime) / m_limitFps;


        for (auto entity: m_entities) {
            entity->processInput(m_window);
            entity->update(m_deltaTime);
            entity->render();
        }
        m_frames++;

        if (glfwGetTime() - m_timer > 1.0) {

            char buff[100];
            snprintf(buff, sizeof(buff), "%s [%i FPS]", m_title.c_str(), m_frames);
            m_timer++;
            spdlog::info("Frame per second: {}", m_frames);
            glfwSetWindowTitle(m_window, buff);
            m_frames = 0;
        }


        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}

void AlphaEngine::loadShaders() {
    Shader shaderProgram("default.vert", "default.frag");

}
