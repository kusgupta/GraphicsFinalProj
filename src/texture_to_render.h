#ifndef TEXTURE_TO_RENDER_H
#define TEXTURE_TO_RENDER_H

//#include <OpenGL/OpenGL.h>

class TextureToRender {
public:
    TextureToRender();
    ~TextureToRender();
    void create(int width, int height);
    void bind(int framebuffername);
    void unbind();
    int getTexture() const { return renderedTexture; }
private:
    int w_, h_;
    unsigned int fb_ = -1;
    unsigned int tex_ = -1;
    unsigned int dep_ = -1;
    GLuint renderedTexture;
};

#endif
