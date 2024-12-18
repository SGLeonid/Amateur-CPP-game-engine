#include <engine/engine.hpp>

#include <iostream>

int main(int argc, const char *argv[])
{   
    Engine::getInstance().run(argc, argv);
    return 0;
}
