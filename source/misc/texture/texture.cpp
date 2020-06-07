//
// Created by tomesh on 1/5/19.
//

#include "texture.h"

CTexture::CTexture(const char *file) {  // from BMP file (A8 R8 G8 B8, no palette)
    std::ifstream image;
    image.open(file, std::ios::binary);

    char info[54];
    image.read(info, 54);
    m_Width = *(int *) &info[18];
    m_Height = *(int *) &info[22];

    m_Data = new uint32_t[m_Width * m_Height];

    image.seekg(*(int *) &info[10]); // offset

    image.read((char *) m_Data, m_Width * m_Height * 4);
    image.close();
}

CTexture::~CTexture() {
    delete[] m_Data;
}

uint32_t CTexture::get(float x, float y) {
    x -= (int) x;
    y -= (int) y;
    return m_Data[(int) ((1 - y) * (m_Height - 1)) * m_Width + (int) (x * m_Width)];    // BMP inverted y axis
}

float CTexture::getAspectRatio() {
    return (float)m_Width / m_Height;
}
