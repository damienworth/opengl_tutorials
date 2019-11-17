#include "ltexture.hpp"

#include <IL/il.h>
#include <gsl/gsl_util>

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

bool
ltexture::load_from_file(std::string_view path)
{
    // generate and set current image id
    ILuint img_id = 0;
    ilGenImages(1, &img_id);
    ilBindImage(img_id);

    ILboolean success = ilLoadImage(path.data());
    auto      _       = gsl::finally([success, &img_id]() {
        if (success) {
            // delete file from memory
            ilDeleteImages(1, &img_id);
        }
    });

    bool texture_loaded = false;
    // load image
    do {
        if (success != IL_TRUE) break;

        // convert image to RGBA
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        if (success != IL_TRUE) break;

        texture_loaded = load_from_pixels32(
            reinterpret_cast<GLuint*>(ilGetData()),
            {static_cast<GLuint>(ilGetInteger(IL_IMAGE_WIDTH)),
             static_cast<GLuint>(ilGetInteger(IL_IMAGE_HEIGHT))});
    } while (false);

    // report error
    if (!texture_loaded) { std::cerr << "unable to load " << path << '\n'; }

    return texture_loaded;
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

void ltexture::render(std::array<GLfloat, 2> point, std::optional<lfrect> clip)
{
    // if the texture exists
    if (!_texture_id) return;

    // remove any previous transformations
    glLoadIdentity();

    // texture coordinates
    auto texcoord = lfrect{0.f, 1.f, 0.f, 1.f};

    // vertex coordinates
    auto quad_size = std::array{gsl::narrow<GLfloat>(_dimensions[0]),
                                gsl::narrow<GLfloat>(_dimensions[1])};

    // handle clipping
    if (clip) {
        auto& _clip = *clip;

        // texture coordinates
        texcoord[0] = _clip[0] / gsl::narrow<GLfloat>(_dimensions[0]);
        texcoord[1] =
            (_clip[0] + _clip[2]) / gsl::narrow<GLfloat>(_dimensions[0]);
        texcoord[2] = _clip[1] / gsl::narrow<GLfloat>(_dimensions[1]);
        texcoord[3] =
            (_clip[1] + _clip[3]) / gsl::narrow<GLfloat>(_dimensions[1]);

        quad_size = {_clip[2], _clip[3]};
    }

    // move to rendering point
    glTranslatef(point[0], point[1], 0.f);

    // set texture id
    glBindTexture(GL_TEXTURE_2D, _texture_id);

    // render texture quad
    glBegin(GL_QUADS);
    glTexCoord2f(texcoord[0], texcoord[2]);
    glVertex2f(0.f, 0.f);
    glTexCoord2f(texcoord[1], texcoord[2]);
    glVertex2f(quad_size[0], 0.f);
    glTexCoord2f(texcoord[1], texcoord[3]);
    glVertex2f(quad_size[0], quad_size[1]);
    glTexCoord2f(texcoord[0], texcoord[3]);
    glVertex2f(0.f, quad_size[1]);
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
