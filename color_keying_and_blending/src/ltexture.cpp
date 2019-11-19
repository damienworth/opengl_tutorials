#include "ltexture.hpp"

#include <memory> // for std::align

#include <IL/il.h>
#include <IL/ilu.h> // for ILU_PLACEMENT and ILU_UPPER_LEFT etc.

#include <gsl/gsl_util> // for gsl::finally and gsl::narrow

#include "macro_helpers.hpp"

namespace {

template <typename Numeric>
Numeric
align_against(Numeric num)
{
    const auto bits_in_type =
        static_cast<Numeric>(sizeof(Numeric) * 8 /* bits */);
    return num + (bits_in_type - (num + bits_in_type) % bits_in_type);
}

} // namespace

ltexture::ltexture() = default; // already implemented with default initializers

ltexture::~ltexture()
{
    // free texture data if needed
    free_texture();
}

bool
ltexture::lock()
{
    // if texture is not locked and a texture exists
    if (_pixels || !_texture_id) return false;

    std::cout << __FUNCTION__ << '\n';

    // allocate memory for texture data
    GLuint size = Wv(_dimensions) * Hv(_dimensions);
    _pixels     = std::make_unique<GLuint[]>(size);

    // set current texture
    glBindTexture(GL_TEXTURE_2D, _texture_id);

    // get pixels
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, _pixels.get());

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

bool
ltexture::unlock()
{
    // if texture is locked and a texture exists
    if (!_pixels || !_texture_id) return false;

    std::cout << __FUNCTION__ << '\n';

    // set current texture
    glBindTexture(GL_TEXTURE_2D, _texture_id);

    // update texture
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        Wv(_dimensions),
        Hv(_dimensions),
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        _pixels.get());

    // delete pixels
    _pixels.reset();

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

// contract: raw pointer is not-owning
// for owning we'll use gsl::owner
GLuint*
ltexture::data()
{
    return _pixels.get();
}

GLuint
ltexture::pixel(const std::array<GLuint, 2>& point) const
{
    return _pixels[Yc(point) * Wv(_dimensions) + Xc(point)];
}

void
ltexture::set_pixel(const std::array<GLuint, 2>& point, GLuint value)
{
    _pixels[Yc(point) * Wv(_dimensions) + Xc(point)] = value;
}

bool
ltexture::load_from_pixels32(GLuint* pixels, std::array<GLuint, 2> tex_dims)
{
    // free texture if it exists
    free_texture();

    // get texture dimensions
    _dimensions = std::move(tex_dims);

    // generate texture id
    glGenTextures(1, &_texture_id);

    // bind texture
    glBindTexture(GL_TEXTURE_2D, _texture_id);

    // generate texture
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        Wv(_dimensions),
        Hv(_dimensions),
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

        auto img_width  = static_cast<GLuint>(ilGetInteger(IL_IMAGE_WIDTH));
        auto img_height = static_cast<GLuint>(ilGetInteger(IL_IMAGE_HEIGHT));

        auto tex_width  = align_against(img_width);
        auto tex_height = align_against(img_height);

        // texture is the wrong size
        if (tex_width != img_width || tex_height != img_height) {
            // place image at upper left
            iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);

            // resize image
            iluEnlargeCanvas(
                gsl::narrow<int>(img_width), gsl::narrow<int>(img_height), 1);
        }

        texture_loaded = load_from_pixels32(
            reinterpret_cast<GLuint*>(ilGetData()), {img_width, img_height});
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

    _pixels.reset();

    _dimensions = {0, 0};
}

void ltexture::render(std::array<GLfloat, 2> point, std::optional<lfrect> clip)
{
    // if the texture exists
    if (!_texture_id) return;

    // remove any previous transformations
    glLoadIdentity();

    // texture coordinates
    auto texcoord = lfrect{
        0.f, // left
        0.f, // top
        1.f, // right
        1.f  // bottom
    };

    // vertex coordinates
    auto quad_size = std::array{gsl::narrow<GLfloat>(Wv(_dimensions)),
                                gsl::narrow<GLfloat>(Hv(_dimensions))};

    // handle clipping
    if (clip) {
        auto& _clip = *clip; // this (clip) thing is std::optional, thus deref

        // texture coordinates
        Lv(texcoord) = Xc(_clip) / gsl::narrow<GLfloat>(Wv(_dimensions));
        Tv(texcoord) = Tv(_clip) / gsl::narrow<GLfloat>(Hv(_dimensions));
        Rv(texcoord) =
            (Xc(_clip) + Rv(_clip)) / gsl::narrow<GLfloat>(Wv(_dimensions));
        Bv(texcoord) =
            (Tv(_clip) + Bv(_clip)) / gsl::narrow<GLfloat>(Hv(_dimensions));

        quad_size = {Rv(_clip), Bv(_clip)};
    }

    // move to rendering point
    glTranslatef(Xc(point), Yc(point), 0.f);

    // set texture id
    glBindTexture(GL_TEXTURE_2D, _texture_id);

    // render texture quad
    glBegin(GL_QUADS);
    glTexCoord2f(Lv(texcoord), Tv(texcoord));
    glVertex2f(0.f, 0.f);
    glTexCoord2f(Rv(texcoord), Tv(texcoord));
    glVertex2f(Wv(quad_size), 0.f);
    glTexCoord2f(Rv(texcoord), Bv(texcoord));
    glVertex2f(Wv(quad_size), Hv(quad_size));
    glTexCoord2f(Lv(texcoord), Bv(texcoord));
    glVertex2f(0.f, Hv(quad_size));
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
