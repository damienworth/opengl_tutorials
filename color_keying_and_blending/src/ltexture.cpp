#include "ltexture.hpp"

#include <cstring> // for std::memcpy
#include <memory>  // for std::align
#include <numeric> // for std::accumulate

#include <IL/il.h>
#include <IL/ilu.h> // for ILU_PLACEMENT and ILU_UPPER_LEFT etc.

#include <gsl/gsl_util> // for gsl::finally and gsl::narrow

#include "macro_helpers.hpp"

namespace {

template <
    typename Numeric,
    typename = std::enable_if_t<std::is_integral_v<Numeric>>>
Numeric
align(Numeric num)
{
    const auto bits_in_type =
        static_cast<Numeric>(sizeof(Numeric) * 8 /* bits */);
    return num + (bits_in_type - (num + bits_in_type) % bits_in_type);
}

template <
    typename Numeric,
    typename = std::enable_if_t<std::is_integral_v<Numeric>>>
std::array<Numeric, 2>
align(const std::array<Numeric, 2>& unaligned)
{
    return {align<Numeric>(Wv(unaligned)), align<Numeric>(Hv(unaligned))};
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
ltexture::load_from_pixels32()
{
    // there is loaded pixels
    if (_texture_id || !_pixels) {
        std::cerr << "cannot load texture from current pixels\n";

        // texture already exists
        if (_texture_id) {
            std::cerr << "a texture is already loaded\n";
        } /* no pixels loaded */ else if (!_pixels) {
            std::cerr << "no pixels to create a texture from\n";
        }
        return false;
    }

    // generate texture id
    glGenTextures(1, &_texture_id);

    // bind texture id
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
        _pixels.get());

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // check for error
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr
            << "error loading texture from pixels: " << gluErrorString(error)
            << '\n';
        return false;
    } else {
        // release pixels
        _pixels.reset();
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

    // load image
    ILboolean success = ilLoadImage(path.data());
    auto      _       = gsl::finally([success, &img_id]() {
        if (success) {
            // delete file from memory
            ilDeleteImages(1, &img_id);
        }
    });

    bool texture_loaded = false;
    do {
        if (success != IL_TRUE) break;

        // convert image to RGBA
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        if (success != IL_TRUE) break;

        auto img_dims =
            std::array{gsl::narrow_cast<GLuint>(ilGetInteger(IL_IMAGE_WIDTH)),
                       gsl::narrow_cast<GLuint>(ilGetInteger(IL_IMAGE_HEIGHT))};

        auto tex_dims = align(img_dims);

        // texture is the wrong size
        if (tex_dims != img_dims) {
            // place image at upper left
            iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);

            // resize image
            iluEnlargeCanvas(
                gsl::narrow<int>(Wv(img_dims)),
                gsl::narrow<int>(Hv(img_dims)),
                1);
        }

        texture_loaded = load_from_pixels32(
            reinterpret_cast<GLuint*>(ilGetData()), std::move(img_dims));
    } while (false);

    // report error
    if (!texture_loaded) { std::cerr << "unable to load " << path << '\n'; }

    return texture_loaded;
}

bool
ltexture::load_pixels_from_file(std::string_view path)
{
    // deallocate texture data
    free_texture();

    // generate and set current image id
    ILuint img_id = 0;
    ilGenImages(1, &img_id);
    ilBindImage(img_id);

    // load image
    ILboolean success = ilLoadImage(path.data());
    auto      _       = gsl::finally([success, &img_id]() {
        if (success) {
            // delete file from memory
            ilDeleteImages(1, &img_id);
        }
    });

    bool pixels_loaded = false;
    do {
        if (success != IL_TRUE) break;

        // convert image to RGBA
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        if (success != IL_TRUE) break;

        // initialize dimensions
        auto img_dims =
            std::array{gsl::narrow<GLuint>(ilGetInteger(IL_IMAGE_WIDTH)),
                       gsl::narrow<GLuint>(ilGetInteger(IL_IMAGE_HEIGHT))};

        // calculate required texture dimensions
        auto tex_dims = align(img_dims);

        // texture is the wrong size
        if (tex_dims != img_dims) {
            // place image at upper left
            iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);

            // resize image
            iluEnlargeCanvas(
                gsl::narrow<int>(Wv(img_dims)),
                gsl::narrow<int>(Hv(img_dims)),
                1);
        }

        // allocate memory for texture data
        GLuint size = std::accumulate(
            std::begin(img_dims),
            std::end(img_dims),
            1,
            std::multiplies<GLuint>());
        _pixels = std::make_unique<GLuint[]>(size);

        // get image dimensions
        _dimensions = img_dims;

        // copy pixels
        std::memcpy(_pixels.get(), ilGetData(), size * sizeof(GLuint));
        pixels_loaded = true;
    } while (false);

    // delete file from memory
    ilDeleteImages(1, &img_id);

    // report error
    if (!pixels_loaded) { std::cerr << "unable to load " << path << '\n'; }

    return pixels_loaded;
}

bool
ltexture::load_from_file_with_color_key(
    std::string_view path, std::array<GLubyte, 3> rgb, GLubyte a)
{
    // load pixels
    if (!load_pixels_from_file(path)) { return false; }

    // go through pixels
    GLuint size = std::accumulate(
        std::begin(_dimensions),
        std::end(_dimensions),
        1,
        std::multiplies<GLuint>());

    for (size_t i = 0; i < size; ++i) {
        // get pixel colors
        GLubyte* colors = reinterpret_cast<GLubyte*>(&_pixels[i]);

        // color matches
        if (Rc(colors) == Rc(rgb) && Gc(colors) == Gc(rgb) &&
            Bc(colors) == Bc(rgb) && (0 == a || Ac(colors) == a)) {
            // make transparent
            Rc(colors) = 0xff;
            Gc(colors) = 0xff;
            Bc(colors) = 0xff;
            Ac(colors) = 0;
        }
    }

    // create texture
    return load_from_pixels32();
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
