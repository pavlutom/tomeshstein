//
// Created by tomesh on 1/5/19.
//

#ifndef TOMESHSTEIN_TEXTURE_H
#define TOMESHSTEIN_TEXTURE_H

#include <fstream>


class CTexture {
public:
    explicit CTexture(const char *file);
    ~CTexture();
    uint32_t get(float x, float y);
    float getAspectRatio();

private:
    uint32_t *m_Data;
    int m_Width;
    int m_Height;
};


#endif //TOMESHSTEIN_TEXTURE_H
