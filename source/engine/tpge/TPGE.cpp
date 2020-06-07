//
// Created by tomesh on 1/2/19.
//

#include "TPGE.h"

tpge::CEngine::CEngine() : m_Window(nullptr) {}

tpge::CEngine::~CEngine() {
    if (m_Root) {
        destroy();
    }
}

bool tpge::CEngine::construct(const char *title, unsigned width, unsigned height, unsigned short pixelSize,
                              bool fullscreen) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("could not initialize sdl2: %s", SDL_GetError());
        return false;
    }

    m_Width = width;
    m_Height = height;
    m_PixelSize = pixelSize;

    if (fullscreen) {
        SDL_DisplayMode dm;
        if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
            SDL_Log("could not resolve display mode: %s", SDL_GetError());
            return false;
        }

        if (dm.w < m_Width * m_PixelSize) {
            m_Width = (unsigned) dm.w / m_PixelSize;
        }
        if (dm.h < m_Height * m_PixelSize) {
            m_Height = (unsigned) dm.h / m_PixelSize;
        }

        m_Window = SDL_CreateWindow(
                title,
                0, 0,
                dm.w, dm.h,
                SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN
        );
    } else {
        m_Window = SDL_CreateWindow(
                title,
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                m_Width * m_PixelSize, m_Height * m_PixelSize,
                SDL_WINDOW_SHOWN
        );
    }

    if (m_Window == nullptr) {
        SDL_Log("could not create window: %s", SDL_GetError());
        return false;
    }

    m_Title = title;

//    m_Surface = SDL_GetWindowSurface(m_Window);
    m_KeyboardState = SDL_GetKeyboardState(nullptr);

//    m_Overlay = SDL_CreateRGBSurface(0, m_Width * m_PixelSize, m_Height * m_PixelSize, m_Surface->format->BitsPerPixel,
//                                     0, 0, 0, 0);
//    SDL_SetSurfaceBlendMode(m_Overlay, SDL_BLENDMODE_BLEND);

//    m_FullRect = SDL_Rect();
//    m_FullRect.h = m_Height * m_PixelSize;
//    m_FullRect.w = m_Width * m_PixelSize;
//    m_FullRect.x = 0;
//    m_FullRect.y = 0;

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(m_Renderer, m_Width, m_Height);
    SDL_RenderSetIntegerScale(m_Renderer, SDL_TRUE);
    m_ScreenTexture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_Width, m_Height);
    m_Pixels = (Uint8 *)malloc(m_Width * m_Height * 4);

    m_Root = true;

    return true;
}


bool tpge::CEngine::inWindowOf(const tpge::CEngine &other) {

    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_PixelSize = other.m_PixelSize;

    m_Window = other.m_Window;

    m_Title = other.m_Title;

//    m_Surface = SDL_GetWindowSurface(m_Window);
    m_KeyboardState = SDL_GetKeyboardState(nullptr);

//    m_Overlay = SDL_CreateRGBSurface(0, m_Width * m_PixelSize, m_Height * m_PixelSize, m_Surface->format->BitsPerPixel,
//                                     0, 0, 0, 0);
//    SDL_SetSurfaceBlendMode(m_Overlay, SDL_BLENDMODE_BLEND);

//    m_FullRect = SDL_Rect();
//    m_FullRect.h = m_Height * m_PixelSize;
//    m_FullRect.w = m_Width * m_PixelSize;
//    m_FullRect.x = 0;
//    m_FullRect.y = 0;

    m_Renderer = other.m_Renderer;
//    SDL_RenderSetLogicalSize(m_Renderer, m_Width, m_Height);
    m_ScreenTexture = other.m_ScreenTexture;
    m_Pixels = other.m_Pixels;

//    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
//    m_ScreenTexture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_Width * m_PixelSize, m_Height * m_PixelSize);
//    m_Pixels = (Uint8 *)malloc(m_Width * m_Height * m_PixelSize * m_PixelSize * 4);

    m_Root = false;

    return true;
}


void tpge::CEngine::destroy() {
//    SDL_FreeSurface(m_Surface);
//    SDL_FreeSurface(m_Overlay);

    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

void tpge::CEngine::printFrame() {
    SDL_UpdateTexture(m_ScreenTexture, NULL, m_Pixels, m_Width * 4);
    SDL_RenderCopy(m_Renderer, m_ScreenTexture, NULL, NULL);
    SDL_RenderPresent(m_Renderer);
//    SDL_UpdateWindowSurface(m_Window);
}

int tpge::CEngine::run() {
    onUserCreate();
    std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();
    bool running = true;
    int signal = 0;
    float elapsed;
    SDL_Event event;
    while (running) {
        elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count();
        t1 = t2;
        t2 = std::chrono::system_clock::now();

        while (SDL_PollEvent(&event)) {
            running &= event.type != SDL_QUIT;
        }
        running &= onUserUpdate(elapsed, signal);
    }

    return signal;
}

bool tpge::CEngine::isKeyPressed(SDL_Scancode key) {
    return m_KeyboardState[key];
}

void tpge::CEngine::drawPixel(int x, int y, Uint32 color) {
    *((Uint32 *)&m_Pixels[m_Width * y * 4 + x * 4]) = color;
//    SDL_Rect pixel{x * m_PixelSize, y * m_PixelSize, m_PixelSize, m_PixelSize};
//    SDL_FillRect(m_Surface, &pixel, color);
}

void tpge::CEngine::blendPixel(int x, int y, float alpha, Uint32 color) {
    return;
    Uint32 oldColor = *((Uint32 *) ((Uint8 *) m_Surface->pixels + (y * m_PixelSize) * m_Surface->pitch + (x * m_PixelSize) * 4));
    Uint32 newColor = oldColor;
    float inverseAlpha = 1 - alpha;

    *((Uint8 *) &newColor + 2) = (float)*((Uint8 *) &oldColor + 2) * inverseAlpha + (float)*((Uint8 *) &color + 2) * alpha;
    *((Uint8 *) &newColor + 1) = (float)*((Uint8 *) &oldColor + 1) * inverseAlpha + (float)*((Uint8 *) &color + 1) * alpha;
    *((Uint8 *) &newColor) = (float)*((Uint8 *) &oldColor) * inverseAlpha + (float)*((Uint8 *) &color) * alpha;

    drawPixel(x, y, newColor);
}

void tpge::CEngine::blendScreen(float alpha, Uint32 color) {
    return;
    SDL_FillRect(m_Overlay, nullptr, color);
    SDL_SetSurfaceAlphaMod(m_Overlay, (Uint8) (255 * alpha));

    SDL_BlitSurface(m_Overlay, nullptr, m_Surface, &m_FullRect);
}

void tpge::CEngine::drawRectangle(int x, int y, int width, int height, Uint32 color) {
    return;
    SDL_FillRect(m_Overlay, nullptr, color);
    SDL_Rect r;
    r.x = x * m_PixelSize;
    r.y = y * m_PixelSize;
    r.w = width * m_PixelSize;
    r.h = height * m_PixelSize;

    SDL_BlitSurface(m_Overlay, &r, m_Surface, &r);
}

bool tpge::CEngine::readText(char *buffer, int x, int y, Uint32 color, short scale) {
    SDL_StartTextInput();
    bool running = true;
    SDL_Event event;
    buffer[0] = '\0';
    short len = 0;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                // enter pressed - confirm
                running = false;
                SDL_StopTextInput();
                break;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                // escape pressed - cancel
                buffer[0] = '\0';
                running = false;
                SDL_StopTextInput();
                break;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                // backspace
                if (len > 0) {  // not empty
                    len--;
                    buffer[len] = '\0';
                    // draw
                    drawRectangle(x, y, (len + 1) * scale * 12, scale * 16, TPGEColor::BLACK);
                    drawString(x, y, color, scale, buffer);
                    printFrame();
                }
            } else if (event.type == SDL_TEXTINPUT) {
                // check length
                char *ptr = event.text.text;
                short suffix = 0;
                while (*ptr != '\0') {
                    suffix++;
                    ptr++;
                }
                if (len + suffix <= 16) {
                    len += suffix;
                    // save text
                    strcat(buffer, event.text.text);
                    // draw
                    drawRectangle(x, y, len * scale * 12, scale * 16, TPGEColor::BLACK);
                    drawString(x, y, color, scale, buffer);
                    printFrame();
                }
            }
        }
    }
    return true;
}

unsigned tpge::CEngine::getScreenWidth() {
    return m_Width;
}

unsigned tpge::CEngine::getScreenHeight() {
    return m_Height;
}

unsigned short tpge::CEngine::getPixelSize() {
    return m_PixelSize;
}

const char *tpge::CEngine::getTitle() {
    return m_Title;
}

const uint16_t tpge::CEngine::font[96][12] = {
        {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},     //
        {0x0600, 0x600F, 0x00F0, 0x0F00, 0xF00F, 0x0060, 0x0600, 0x6000, 0x0000, 0x0600, 0x6000, 0x0000},     // !
        {0x0000, 0x0019, 0x8198, 0x1981, 0x9800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},     // "
        {0x0000, 0x6606, 0x6066, 0x3FF0, 0xCC0C, 0xC198, 0x1987, 0xFC33, 0x0330, 0x3300, 0x0000, 0x0000},     // #
        {0x0600, 0x601F, 0x83FC, 0x3603, 0x603F, 0x81FC, 0x06C0, 0x6C3F, 0xC1F8, 0x0600, 0x6000, 0x0000},     // $
        {0x0000, 0x0000, 0x1383, 0x3873, 0x8E01, 0xC038, 0x0700, 0xE01C, 0x038E, 0x70E6, 0x0E00, 0x0000},     // %
        {0x0000, 0x700D, 0x8198, 0x1981, 0xB00E, 0x01E0, 0x3E03, 0x3633, 0xC318, 0x3BC1, 0xE600, 0x0000},     // &
        {0x0E00, 0xE00E, 0x0060, 0x0600, 0xC000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},     // '
        {0x0380, 0x600E, 0x00C0, 0x1C01, 0xC01C, 0x01C0, 0x1C01, 0xC00C, 0x00E0, 0x0600, 0x3800, 0x0000},     // (
        {0x1C00, 0x6007, 0x0030, 0x0380, 0x3803, 0x8038, 0x0380, 0x3803, 0x0070, 0x0601, 0xC000, 0x0000},     // )
        {0x0000, 0x0000, 0x036C, 0x36C1, 0xF80F, 0x03FC, 0x0F01, 0xF836, 0xC36C, 0x0000, 0x0000, 0x0000},     // *
        {0x0000, 0x0000, 0x0000, 0x0600, 0x6006, 0x03FC, 0x3FC0, 0x6006, 0x0060, 0x0000, 0x0000, 0x0000},     // +
        {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00E0, 0x0E00, 0xE006, 0x00C0},     // ,
        {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x03FC, 0x3FC0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},     // -
        {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00E0, 0x0E00, 0xE000, 0x0000},     // .
        {0x0000, 0x0100, 0x3007, 0x00E0, 0x1C03, 0x8070, 0x0E01, 0xC038, 0x0700, 0x6000, 0x0000, 0x0000},     // /
        {0x0F83, 0xFE30, 0x6607, 0x60F6, 0x1B63, 0x3663, 0x6C37, 0x8370, 0x3306, 0x3FE0, 0xF800, 0x0000},     // 0
        {0x0300, 0x701F, 0x01F0, 0x0300, 0x3003, 0x0030, 0x0300, 0x3003, 0x0030, 0x1FE1, 0xFE00, 0x0000},     // 1
        {0x1FC3, 0xFE70, 0x7603, 0x6070, 0x0E01, 0xC038, 0x0700, 0xE01C, 0x0380, 0x7FF7, 0xFF00, 0x0000},     // 2
        {0x1FC3, 0xFE70, 0x7603, 0x0030, 0x070F, 0xE0FC, 0x0060, 0x0360, 0x3707, 0x3FE1, 0xFC00, 0x0000},     // 3
        {0x01C0, 0x3C07, 0xC0EC, 0x1CC3, 0x8C70, 0xC60C, 0x7FF7, 0xFF00, 0xC00C, 0x00C0, 0x0C00, 0x0000},     // 4
        {0x7FF7, 0xFF60, 0x0600, 0x6007, 0xFC3F, 0xE007, 0x0030, 0x0360, 0x3707, 0x3FE1, 0xFC00, 0x0000},     // 5
        {0x03C0, 0x7C0E, 0x01C0, 0x3803, 0x007F, 0xC7FE, 0x7076, 0x0360, 0x3707, 0x3FE1, 0xFC00, 0x0000},     // 6
        {0x7FF7, 0xFF00, 0x6006, 0x00C0, 0x0C01, 0x8018, 0x0300, 0x3006, 0x0060, 0x0C00, 0xC000, 0x0000},     // 7
        {0x0F81, 0xFC38, 0xE306, 0x3063, 0x8E1F, 0xC3FE, 0x7076, 0x0360, 0x3707, 0x3FE1, 0xFC00, 0x0000},     // 8
        {0x1FC3, 0xFE70, 0x7603, 0x6037, 0x073F, 0xF1FF, 0x0060, 0x0E01, 0xC038, 0x1F01, 0xE000, 0x0000},     // 9
        {0x0000, 0x0000, 0x0000, 0x0E00, 0xE00E, 0x0000, 0x0000, 0x000E, 0x00E0, 0x0E00, 0x0000, 0x0000},     // :
        {0x0000, 0x0000, 0x0000, 0x0E00, 0xE00E, 0x0000, 0x0000, 0x000E, 0x00E0, 0x0E00, 0x6006, 0x00C0},     // ;
        {0x00C0, 0x1C03, 0x8070, 0x0E01, 0xC038, 0x0380, 0x1C00, 0xE007, 0x0038, 0x01C0, 0x0C00, 0x0000},     // <
        {0x0000, 0x0000, 0x0000, 0x0003, 0xFE3F, 0xE000, 0x0003, 0xFE3F, 0xE000, 0x0000, 0x0000, 0x0000},     // =
        {0x3003, 0x801C, 0x00E0, 0x0700, 0x3801, 0xC01C, 0x0380, 0x700E, 0x01C0, 0x3803, 0x0000, 0x0000},     // >
        {0x1F83, 0xFC70, 0xE606, 0x60E0, 0x1C03, 0x8070, 0x0600, 0x6006, 0x0000, 0x0600, 0x6000, 0x0000},     // ?
        {0x1FC3, 0xFE30, 0x667B, 0x6FB6, 0xDB6D, 0xB6DB, 0x6DB6, 0xFE67, 0xC700, 0x3FC0, 0xFC00, 0x0000},     // @
        {0x0600, 0x600F, 0x00F0, 0x0F01, 0x9819, 0x8198, 0x30C3, 0xFC3F, 0xC606, 0x6066, 0x0600, 0x0000},     // A
        {0x7F07, 0xF861, 0xC60C, 0x60C6, 0x1C7F, 0x87FC, 0x60E6, 0x0660, 0x660E, 0x7FC7, 0xF800, 0x0000},     // B
        {0x0F81, 0xFC38, 0xE306, 0x6006, 0x0060, 0x0600, 0x6006, 0x0030, 0x638E, 0x1FC0, 0xF800, 0x0000},     // C
        {0x7F07, 0xF861, 0xC60C, 0x6066, 0x0660, 0x6606, 0x6066, 0x0660, 0xC61C, 0x7F87, 0xF000, 0x0000},     // D
        {0x7FE7, 0xFE60, 0x0600, 0x6006, 0x007F, 0x87F8, 0x6006, 0x0060, 0x0600, 0x7FE7, 0xFE00, 0x0000},     // E
        {0x7FE7, 0xFE60, 0x0600, 0x6006, 0x007F, 0x87F8, 0x6006, 0x0060, 0x0600, 0x6006, 0x0000, 0x0000},     // F
        {0x0FC1, 0xFE38, 0x6300, 0x6006, 0x0063, 0xE63E, 0x6066, 0x0630, 0x6386, 0x1FE0, 0xFE00, 0x0000},     // G
        {0x6066, 0x0660, 0x6606, 0x6066, 0x067F, 0xE7FE, 0x6066, 0x0660, 0x6606, 0x6066, 0x0600, 0x0000},     // H
        {0x1F81, 0xF806, 0x0060, 0x0600, 0x6006, 0x0060, 0x0600, 0x6006, 0x0060, 0x1F81, 0xF800, 0x0000},     // I
        {0x0060, 0x0600, 0x6006, 0x0060, 0x0600, 0x6006, 0x0066, 0x0660, 0x670C, 0x3FC1, 0xF800, 0x0000},     // J
        {0x6066, 0x0E61, 0xC638, 0x6706, 0xE07C, 0x07C0, 0x6E06, 0x7063, 0x861C, 0x60E6, 0x0600, 0x0000},     // K
        {0x6006, 0x0060, 0x0600, 0x6006, 0x0060, 0x0600, 0x6006, 0x0060, 0x0600, 0x7FE7, 0xFE00, 0x0000},     // L
        {0x6067, 0x0E70, 0xE79E, 0x79E6, 0xF66F, 0x6666, 0x6666, 0x0660, 0x6606, 0x6066, 0x0600, 0x0000},     // M
        {0x6067, 0x0670, 0x6786, 0x6C66, 0xC666, 0x6666, 0x6366, 0x3661, 0xE60E, 0x60E6, 0x0600, 0x0000},     // N
        {0x0F01, 0xF839, 0xC30C, 0x6066, 0x0660, 0x6606, 0x6066, 0x0630, 0xC39C, 0x1F80, 0xF000, 0x0000},     // O
        {0x7F87, 0xFC60, 0xE606, 0x6066, 0x0660, 0xE7FC, 0x7F86, 0x0060, 0x0600, 0x6006, 0x0000, 0x0000},     // P
        {0x0F01, 0xF839, 0xC30C, 0x6066, 0x0660, 0x6606, 0x6066, 0x3633, 0xC39C, 0x1FE0, 0xF600, 0x0000},     // Q
        {0x7F87, 0xFC60, 0xE606, 0x6066, 0x0660, 0xE7FC, 0x7F86, 0x7063, 0x861C, 0x60E6, 0x0600, 0x0000},     // R
        {0x1F83, 0xFC70, 0xE606, 0x6007, 0x003F, 0x81FC, 0x00E0, 0x0660, 0x670E, 0x3FC1, 0xF800, 0x0000},     // S
        {0x3FC3, 0xFC06, 0x0060, 0x0600, 0x6006, 0x0060, 0x0600, 0x6006, 0x0060, 0x0600, 0x6000, 0x0000},     // T
        {0x6066, 0x0660, 0x6606, 0x6066, 0x0660, 0x6606, 0x6066, 0x0660, 0x630C, 0x3FC1, 0xF800, 0x0000},     // U
        {0x6066, 0x0660, 0x630C, 0x30C3, 0x0C19, 0x8198, 0x1980, 0xF00F, 0x00F0, 0x0600, 0x6000, 0x0000},     // V
        {0x6066, 0x0660, 0x6606, 0x6066, 0x0660, 0x6666, 0x6666, 0xF679, 0xE70E, 0x70E6, 0x0600, 0x0000},     // W
        {0x6066, 0x0630, 0xC30C, 0x1980, 0xF006, 0x0060, 0x0F01, 0x9830, 0xC30C, 0x6066, 0x0600, 0x0000},     // X
        {0x6066, 0x0630, 0xC30C, 0x1981, 0x980F, 0x00F0, 0x0600, 0x6006, 0x0060, 0x0600, 0x6000, 0x0000},     // Y
        {0x7FE7, 0xFE00, 0xC00C, 0x0180, 0x3006, 0x0060, 0x0C01, 0x8030, 0x0300, 0x7FE7, 0xFE00, 0x0000},     // Z
        {0x1F81, 0xF818, 0x0180, 0x1801, 0x8018, 0x0180, 0x1801, 0x8018, 0x0180, 0x1F81, 0xF800, 0x0000},     // [
        {0x0004, 0x0060, 0x0700, 0x3801, 0xC00E, 0x0070, 0x0380, 0x1C00, 0xE007, 0x0030, 0x0000, 0x0000},     // \ (backslash)
        {0x1F81, 0xF801, 0x8018, 0x0180, 0x1801, 0x8018, 0x0180, 0x1801, 0x8018, 0x1F81, 0xF800, 0x0000},     // ]
        {0x0200, 0x700F, 0x81DC, 0x38E7, 0x0760, 0x3000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},     // ^
        {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x007F, 0xF7FF},     // _
        {0x0000, 0x7007, 0x0070, 0x0600, 0x6003, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},     // `
        {0x0000, 0x0000, 0x0000, 0x0001, 0xFC3F, 0xE006, 0x1FE3, 0xFE60, 0x6606, 0x7FE3, 0xFE00, 0x0000},     // a
        {0x6006, 0x0060, 0x0600, 0x6006, 0xF87F, 0xC70E, 0x6066, 0x0660, 0x660E, 0x7FC7, 0xF800, 0x0000},     // b
        {0x0000, 0x0000, 0x0000, 0x0001, 0xF83F, 0xC706, 0x6006, 0x0060, 0x0706, 0x3FC1, 0xF800, 0x0000},     // c
        {0x0060, 0x0600, 0x6006, 0x0061, 0xF63F, 0xE71E, 0x6066, 0x0660, 0x6706, 0x3FE1, 0xFE00, 0x0000},     // d
        {0x0000, 0x0000, 0x0000, 0x0001, 0xF83F, 0xC706, 0x7FE7, 0xFC60, 0x0700, 0x3FC1, 0xF800, 0x0000},     // e
        {0x0780, 0xF81C, 0x0180, 0x1801, 0x807F, 0x07F0, 0x1801, 0x8018, 0x0180, 0x1801, 0x8000, 0x0000},     // f
        {0x0000, 0x0000, 0x0000, 0x0001, 0xFE3F, 0xE706, 0x6067, 0x0E3F, 0xE1F6, 0x0060, 0x0E3F, 0xC3F8},     // g
        {0x6006, 0x0060, 0x0600, 0x6006, 0xF07F, 0x871C, 0x60C6, 0x0C60, 0xC60C, 0x60C6, 0x0C00, 0x0000},     // h
        {0x0000, 0x0006, 0x0060, 0x0000, 0xE00E, 0x0060, 0x0600, 0x6006, 0x0060, 0x1F81, 0xF800, 0x0000},     // i
        {0x0000, 0x0001, 0x8018, 0x0000, 0x3803, 0x8018, 0x0180, 0x1801, 0x8018, 0x0181, 0x981F, 0x80F0},     // j
        {0x3003, 0x0030, 0x0300, 0x3003, 0x1833, 0x8370, 0x3E03, 0xE037, 0x0338, 0x31C3, 0x0C00, 0x0000},     // k
        {0x0E00, 0xE006, 0x0060, 0x0600, 0x6006, 0x0060, 0x0600, 0x6006, 0x0060, 0x1F81, 0xF800, 0x0000},     // l
        {0x0000, 0x0000, 0x0000, 0x0005, 0x987F, 0xC7FE, 0x6666, 0x6666, 0x6666, 0x6666, 0x6600, 0x0000},     // m
        {0x0000, 0x0000, 0x0000, 0x0003, 0xF83F, 0xC30E, 0x3063, 0x0630, 0x6306, 0x3063, 0x0600, 0x0000},     // n
        {0x0000, 0x0000, 0x0000, 0x0001, 0xF83F, 0xC70E, 0x6066, 0x0660, 0x670E, 0x3FC1, 0xF800, 0x0000},     // o
        {0x0000, 0x0000, 0x0000, 0x0007, 0xF87F, 0xC60E, 0x6066, 0x0670, 0xE7FC, 0x6F86, 0x0060, 0x0600},     // p
        {0x0000, 0x0000, 0x0000, 0x0001, 0xFE3F, 0xE706, 0x6066, 0x0670, 0xE3FE, 0x1F60, 0x0600, 0x6006},     // q
        {0x0000, 0x0000, 0x0000, 0x0003, 0x7C3F, 0xE386, 0x3003, 0x0030, 0x0300, 0x3003, 0x0000, 0x0000},     // r
        {0x0000, 0x0000, 0x0000, 0x0003, 0xF07F, 0x8600, 0x7F03, 0xF801, 0x8018, 0x7F83, 0xF000, 0x0000},     // s
        {0x0001, 0x8018, 0x0180, 0x1807, 0xF07F, 0x0180, 0x1801, 0x8018, 0x0180, 0x1F80, 0xF800, 0x0000},     // t
        {0x0000, 0x0000, 0x0000, 0x0006, 0x0660, 0x6606, 0x6066, 0x0660, 0x670E, 0x3FE1, 0xF600, 0x0000},     // u
        {0x0000, 0x0000, 0x0000, 0x0006, 0x0660, 0x630C, 0x30C1, 0x9819, 0x80F0, 0x0F00, 0x6000, 0x0000},     // v
        {0x0000, 0x0000, 0x0000, 0x0006, 0x6666, 0x6666, 0x6666, 0x666F, 0x63FC, 0x39C1, 0x0800, 0x0000},     // w
        {0x0000, 0x0000, 0x0000, 0x0006, 0x0C71, 0xC3B8, 0x1F00, 0xE01F, 0x03B8, 0x71C6, 0x0C00, 0x0000},     // x
        {0x0000, 0x0000, 0x0000, 0x0003, 0x0C30, 0xC198, 0x1980, 0xF00F, 0x0060, 0x0600, 0xC00C, 0x0180},     // y
        {0x0000, 0x0000, 0x0000, 0x0007, 0xFC7F, 0x8030, 0x0600, 0xC018, 0x0300, 0x7FC7, 0xFC00, 0x0000},     // z
        {0x03C0, 0x7C0E, 0x00C0, 0x0C00, 0xC01C, 0x0380, 0x1C00, 0xC00C, 0x00C0, 0x0E00, 0x7C03, 0xC000},     // {
        {0x0600, 0x6006, 0x0060, 0x0600, 0x6006, 0x0060, 0x0600, 0x6006, 0x0060, 0x0600, 0x6000, 0x0000},     // |
        {0x3C03, 0xE007, 0x0030, 0x0300, 0x3003, 0x801C, 0x0380, 0x3003, 0x0030, 0x0703, 0xE03C, 0x0000},     // }
        {0x0000, 0x001C, 0x636C, 0x6380, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},     // ~
        {0x0000, 0x0000, 0x0000, 0x0600, 0xF019, 0x830C, 0x6066, 0x067F, 0xE7FE, 0x0000, 0x0000, 0x0000}      // 
};
