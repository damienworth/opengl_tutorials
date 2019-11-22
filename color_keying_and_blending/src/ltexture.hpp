#ifndef LTEXTURE_HPP
#define LTEXTURE_HPP

#include <array>
#include <memory>
#include <optional>

#include "lopengl.hpp"
#include "lrect.hpp"

class ltexture {
    // texture name
    GLuint _texture_id = {0};

    // texture data
    std::unique_ptr<GLuint[]> _pixels;

    // texture dimensions
    std::array<GLuint, 2> _dimensions = {0, 0};

public:
    ltexture();
    ~ltexture();

    /*
    pre-conditions:
        * an existing unlocked texture
    post-conditions:
        * gets member pixels from texture data
        * returns true if texture pixels were retrieved
    side-effects:
        * binds a null-texture
    */
    bool lock();

    /*
    pre-conditions:
        * a locked texture
    post-conditions:
        * updates texture with member pixels
        * returns true if pixels were updated
    side-effects:
        * binds a null-texture
    */
    bool unlock();

    /*
    pre-conditions:
        * available pixels
    post-conditions:
        * returns member pixels
    side-effects: n/a
    */
    GLuint* data();

    /*
    pre-conditions:
        * pixels available
    post-conditions:
        * returns pixel at given position
        * function will segfault if the texture is not locked
    side-effects: n/a
    */
    GLuint pixel(const std::array<GLuint, 2>& point) const;

    /*
    pre-conditions:
        * pixels available
    post-conditions:
        * sets pixel at given position
        * function will segfault if the texture is not locked
    side-effects: n/a
    */
    void set_pixel(const std::array<GLuint, 2>&, GLuint);

    /*
    pre-condition:
        * valid GL context
    post-condition:
        * creates a texture from the given pixels
        * reports error to console if texture could not be created
    side-effects:
        * binds a null-texture
    */
    bool load_from_pixels32(
        GLuint*, /* pixel data */
        std::array<GLuint, 2> /* texture dimensions */);

    /*
    pre-conditions:
        * a valid OpenGL context
        * valid member pixels
    post-conditions:
        * creates a texture from the number of pixels
        * deletes member pixels on success
        * reports error to console if texture could not be created
    side-effects:
        * binds a null-texture
    */
    bool load_from_pixels32();

    bool load_from_file(std::string_view);

    /*
    pre-conditions:
        * initialized DevIL
    post-conditions:
        * loads member pixels from the given file
        * pads image to have aligned dimensions
        * reports error to console if pixels could not be loaded
    side-effects: n/a
    */
    bool load_pixels_from_file(std::string_view);

    /*
    pre-conditions:
        * a valid OpenGL context
        * initialized DevIL
    post-conditions:
        * creates a texture from the given file
        * pads image to have aligned dimensions
        * sets given RGBA value to RFFGFFBFFA00 in pixel data
        * if A = 0, only RGB components are compared
        * reports error to console if texture could not be created
    side-effects:
        * binds a null-texture
    */
    bool load_from_file_with_color_key(
        std::string_view, std::array<GLubyte, 3> rgb, GLubyte a = 0);

    /*
    pre-condition: valid GL context
    post-condition:
        * deletes texture if it exists
        * sets texture id to 0
    side-effects: n/a
    */
    void free_texture();

    /*
    pre-condition:
        * valid GL context
        * active modelview matrix
    post-condition:
        * translates to given position and renders textured quad
        * if given texture clip is null, the full texture is rendered
    side-effects:
        * binds member texture id
    */
    void render(
        std::array<GLfloat, 2>,
        std::optional<lfrect> clip = std::optional<lfrect>());

    /*
    pre-condition: n/a
    post-condition:
        * returns texture name/id
    side-effects: n/a
    */
    GLuint get_texture_id() const;

    /*
    pre-condition: n/a
    post-condition: returns texture dimensions
    side-effects: n/a
    */
    std::array<GLuint, 2> get_dimensions() const;
};

#endif // LTEXTURE_HPP
