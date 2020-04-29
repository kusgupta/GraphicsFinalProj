#include <GL/glew.h>
#include <iostream>
#include "texture_to_render.h"

TextureToRender::TextureToRender() {

}

TextureToRender::~TextureToRender() {

}

void TextureToRender::create(int width, int height) {
    glGenTextures(1, &renderedTexture);

// "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

// Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 960, 720, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

// Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    tex_ = renderedTexture;
    w_ = width;
    h_ = height;
}


void TextureToRender::bind(int FramebufferName) {
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glViewport(0,0,960,720);
}

void TextureToRender::unbind() {

}
