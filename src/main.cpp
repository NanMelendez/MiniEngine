#include "MiniEngine/app.hpp"

int main() {
    std::unique_ptr<MiniEngine::Application> app = std::make_unique<MiniEngine::Application>(800, 600, "MiniEngine [WIP]");

    app->run();

    return 0;
}