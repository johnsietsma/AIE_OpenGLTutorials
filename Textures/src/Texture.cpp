#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Texture::load(const char* pFilename) {
    int imageWidth = 0, imageHeight = 0, imageFormat = 0;

    unsigned char* data = stbi_load(pFilename,
        &imageWidth, &imageHeight, &imageFormat, STBI_default);

    if (data == nullptr) return false;

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight,
        0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return true;
}
