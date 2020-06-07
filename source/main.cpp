#include "engine/menu/menu.h"

int main() {
    const char *   TITLE      = "TomeshStein";

    unsigned rendering_thread_count;
    unsigned pixel_size;
    unsigned width;
    unsigned height;
    bool     fullscreen;

    std::ifstream configFile;
    configFile.open("../video.cfg");
    configFile >> width >> height >> fullscreen >> pixel_size >> rendering_thread_count;
    configFile.close();

    width /= pixel_size;
    height /= pixel_size;

    std::shared_ptr<CMenu> m =
            std::make_shared<CMenu>(TITLE, width, height, pixel_size, fullscreen, rendering_thread_count);
    m->run();

    return 0;
}