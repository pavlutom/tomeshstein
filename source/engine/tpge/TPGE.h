//
// Created by tomesh on 1/2/19.
//

#ifndef TOMESHPIXELGAMEENGINE_TPGE_H
#define TOMESHPIXELGAMEENGINE_TPGE_H

#include <SDL2/SDL.h>
#include <chrono>
#include <iostream>
#include <vector>

#include "../utils/threading/renderer_thread.h"


namespace tpge {

    class CEngine {
    public:
        CEngine();
        ~CEngine();

        bool construct(const char *title, unsigned width, unsigned height, unsigned short pixelSize, bool fullscreen, unsigned short renderingThreadCount);
        bool inWindowOf(const CEngine &other);
        void destroy();
        int run();

    protected:
        virtual void onUserCreate()                                 = 0;
        virtual bool onUserUpdate(float elapsedTime, int & signal)  = 0;

        enum TPGEColor{
            AMARANTH            = 0xFFE52B50,
            AMBER               = 0xFFFFBF00,
            AMETHYST            = 0xFF9966CC,
            APRICOT             = 0xFFFBCEB1,
            AQUAMARINE          = 0xFF7FFFD4,
            AZURE               = 0xFF007FFF,
            BABY_BLUE           = 0xFF89CFF0,
            BEIGE               = 0xFFF5F5DC,
            BLACK               = 0xFF000000,
            BLUE                = 0xFF0000FF,
            BLUE_GREEN          = 0xFF0095B6,
            BLUE_VIOLET         = 0xFF8A2BE2,
            BLUSH               = 0xFFDE5D83,
            BRONZE              = 0xFFCD7F32,
            BROWN               = 0xFF964B00,
            BURGUNDY            = 0xFF800020,
            BYZANTIUM           = 0xFF702963,
            CARMINE             = 0xFF960018,
            CERISE              = 0xFFDE3163,
            CERULEAN            = 0xFF007BA7,
            CHAMPAGNE           = 0xFFF7E7CE,
            CHARTREUSE_GREEN    = 0xFF7FFF00,
            CHOCOLATE           = 0xFF7B3F00,
            COBALT_BLUE         = 0xFF0047AB,
            COFFEE              = 0xFF6F4E37,
            COPPER              = 0xFFB87333,
            CORAL               = 0xFFF88379,
            CRIMSON             = 0xFFDC143C,
            CYAN                = 0xFF00FFFF,
            DESERT_SAND         = 0xFFEDC9Af,
            ELECTRIC_BLUE       = 0xFF7DF9FF,
            EMERALD             = 0xFF50C878,
            ERIN                = 0xFF00FF3F,
            GOLD                = 0xFFFFD700,
            GRAY                = 0xFF808080,
            GREEN               = 0xFF00FF00,
            HARLEQUIN           = 0xFF3FFF00,
            INDIGO              = 0xFF4B0082,
            IVORY               = 0xFFFFFFF0,
            JADE                = 0xFF00A86B,
            JUNGLE_GREEN        = 0xFF29AB87,
            LAVENDER            = 0xFFB57EDC,
            LEMON               = 0xFFFFF700,
            LILAC               = 0xFFC8A2C8,
            LIME                = 0xFFBFFF00,
            MAGENTA             = 0xFFFF00FF,
            MAGENTA_ROSE        = 0xFFFF00AF,
            MAROON              = 0xFF800000,
            MAUVE               = 0xFFE0B0FF,
            NAVY_BLUE           = 0xFF000080,
            OCHER               = 0xFFCC7722,
            OLIVE               = 0xFF808000,
            ORANGE              = 0xFFFF6600,
            ORANGE_RED          = 0xFFFF4500,
            ORCHID              = 0xFFDA70D6,
            PEACH               = 0xFFFFE5B4,
            PEAR                = 0xFFD1E231,
            PERIWINKLE          = 0xFFCCCCFF,
            PERSIAN_BLUE        = 0xFF1C39BB,
            PINK                = 0xFFFD6C9E,
            PLUM                = 0xFF8E4585,
            PRUSSIAN_BLUE       = 0xFF003153,
            PUCE                = 0xFFCC8899,
            PURPLE              = 0xFF800080,
            RASPBERRY           = 0xFFE30B5C,
            RED                 = 0xFFFF0000,
            RED_VIOLET          = 0xFFC71585,
            ROSE                = 0xFFFF007F,
            RUBY                = 0xFFE0115F,
            SALMON              = 0xFFFA8072,
            SANGRIA             = 0xFF92000A,
            SAPPHIRE            = 0xFF0F52BA,
            SCARLET             = 0xFFFF2400,
            SILVER              = 0xFFC0C0C0,
            SLATE_GRAY          = 0xFF708090,
            SPRING_BUD          = 0xFFA7FC00,
            SPRING_GREEN        = 0xFF00FF7F,
            TAN                 = 0xFFD2B48C,
            TAUPE               = 0xFF483C32,
            TEAL                = 0xFF008080,
            TURQUOISE           = 0xFF40E0D0,
            ULTRAMARINE         = 0xFF3F00FF,
            VIOLET              = 0xFF8F00FF,
            VIRIDIAN            = 0xFF40826D,
            WHITE               = 0xFFFFFFFF,
            YELLOW              = 0xFFFFFF00,
            GROUND_BROWN        = 0xFF463C2D,
            DARK_GREEN          = 0xFF008000
        };

        void printFrame();
        bool isKeyPressed(SDL_Scancode key);
        void drawPixel(int x, int y, Uint32 color);
        void blendPixel(int x, int y, float alpha, Uint32 color);
        void blendScreen(float alpha, Uint32 color);

        template<typename ...Args>
        void drawString(int x, int y, Uint32 color, short scale, const char * format, Args && ...args) {
            int tableOffset = -32;
            int cix = 0;
            char str[0xFF];
            sprintf(str, format, args...);

            while (str[cix] != '\0') {
                int bit = 0;
                for (int i = 0; i < 12; i++) {
                    for (int j = 0; j < 16; j++) {
                        if ((font[str[cix] + tableOffset][i] << j) & 0x8000) {  // highest bit only
                            for (int si = 0; si < scale; si++) {
                                for (int sj = 0; sj < scale; sj++) {
                                    drawPixel(x + (bit % 12 + cix * 12) * scale + si, y + (bit / 12) * scale + sj, color);
                                }
                            }
                        }
                        bit++;
                    }
                }
                cix++;
            }
        }
        void drawRectangle(int x, int y, int width, int height, Uint32 color);

        bool readText(char * buffer, int x, int y, Uint32 color, short scale);

        unsigned getScreenWidth();
        unsigned getScreenHeight();
        unsigned short getPixelSize();
        const char * getTitle();

        template<typename ...Args>
        void setTitle(const char * format, Args && ...args) {
            char buffer[0xFF];
            std::sprintf(buffer, format, args...);
            SDL_SetWindowTitle(m_Window, buffer);
        }

        std::vector<CRendererThread> &getRendererThreads();

    private:
        bool m_Root;

        unsigned m_Width;
        unsigned m_Height;
        unsigned short m_PixelSize;

        const char *m_Title;

        SDL_Window *m_Window;
        SDL_Renderer *m_Renderer;
        SDL_Texture *m_ScreenTexture;
        SDL_Texture *m_OverlayTexture;
        Uint32 *m_Pixels;

        std::vector<CRendererThread> m_RendererThreads;

        bool m_IsOverlayOn;

        const Uint8 *m_KeyboardState;

        static const uint16_t font[96][12];
    };
}


#endif //TOMESHPIXELGAMEENGINE_TPGE_H
