//
// Created by tg on 15/12/21.
//

#ifndef ALPHAGAMEENGINE_ENTITY_H
#define ALPHAGAMEENGINE_ENTITY_H

#include <string>

class Entity {
private:
    std::string m_name;
public:
    Entity(std::string name) {
        m_name = name;
    };

    virtual void init() =0;

    virtual void update(float dt) =0;

    virtual void render() =0;

    virtual void processInput(GLFWwindow *m_window) = 0;
};

#endif //ALPHAGAMEENGINE_ENTITY_H
