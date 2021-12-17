//
// Created by tg on 15/12/21.
//

#ifndef ALPHAGAMEENGINE_CUBE_ENTITY_H
#define ALPHAGAMEENGINE_CUBE_ENTITY_H


#include "../entity.h"

class CubeEntity : public Entity {
public:
    explicit CubeEntity(std::string name) : Entity(name) {};

    void update(float dt);

    void init();

    void render();

    void processInput(GLFWwindow *m_window);

private:
    unsigned int m_vertexShader;

};

void CubeEntity::update(float dt) {


}

void CubeEntity::init() {
    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";

    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(m_vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(m_vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

}

void CubeEntity::render() {



}

void CubeEntity::processInput(GLFWwindow *m_window) {

    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
        spdlog::info("W Key");
    }
}

#endif //ALPHAGAMEENGINE_CUBE_ENTITY_H
