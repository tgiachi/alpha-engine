#include <iostream>
#include "include/engine.h"
#include "include/entities/cube_entity.h"
AlphaEngine *engine;

int main(int, char **) {
    engine = new AlphaEngine(800, 600, "test", 60);
    auto cube = new CubeEntity("cube");
    if (engine->init()) {
        engine->addEntity(cube);
        engine->loop();
    }
    std::cout << "Hello, world!\n";
}
