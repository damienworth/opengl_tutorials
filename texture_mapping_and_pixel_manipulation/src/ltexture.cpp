#include "ltexture.hpp"

ltexture::ltexture() = default;

ltexture::~ltexture()
{
    // free texture data if needed
    free_texture();
}

bool
ltexture::load_from_pixels32(GLuint* pixels, std::array<GLuint, 2> dimensions)
{
    // free texture if it exists
    free_texture();

    // get texture dimensions
    _dimensions = std::move(dimensions);

    // generate texture id
    glGenTextures(1, &_texture_id);

    // bind texture
    glBindTexture(GL_TEXTURE_2D, _texture_id);

    // generate texture
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        _dimensions[0],
        _dimensions[1],
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels);

    /* Set texture parameters:
    Here we're setting the GL_TEXTURE_MAG_FILTER and GL_TEXTURE_MIN_FILTER which
    control how the texture is shown when it is magnified and minified
    respectively. We're setting both these attributes to GL_LINEAR which gives
    us nice results.
    */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // check for error
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "error loading texture from "
                  << static_cast<const void*>(pixels) << gluErrorString(error)
                  << '\n';
        return false;
    }

    return true;
}

void
ltexture::free_texture()
{
    // delete texture
    if (_texture_id != 0) {
        glDeleteTextures(1, &_texture_id);
        _texture_id = 0;
    }

    _dimensions = {0, 0};
}

void ltexture::render(std::array<GLfloat, 2> point)
{
    // if the texture exists
    if (!_texture_id) return;

    // remove any previous transformations
    glLoadIdentity();

    // move to rendering point
    glTranslatef(point[0], point[1], 0.f);

    // set texture id
    glBindTexture(GL_TEXTURE_2D, _texture_id);

    // render texture quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.f, 0.f);
    glVertex2f(0.f, 0.f);
    glTexCoord2f(1.f, 0.f);
    glVertex2f(static_cast<GLfloat>(_dimensions[0]), 0.f);
    glTexCoord2f(1.f, 1.f);
    glVertex2f(
        static_cast<GLfloat>(_dimensions[0]),
        static_cast<GLfloat>(_dimensions[1]));
    glTexCoord2f(0.f, 1.f);
    glVertex2f(0.f, static_cast<GLfloat>(_dimensions[1]));
    glEnd();
}

inline GLuint
ltexture::get_texture_id() const
{
    return _texture_id;
}

std::array<GLuint, 2>
ltexture::get_dimensions() const
{
    return _dimensions;
}
