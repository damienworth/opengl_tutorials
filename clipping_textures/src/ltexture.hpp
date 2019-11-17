#ifndef LTEXTURE_HPP
#define LTEXTURE_HPP

#include <array>
#include <optional>

#include "lopengl.hpp"
#include "lrect.hpp"

class ltexture {
    // texture name
    GLuint _texture_id = {0};

    // texture dimensions
    std::array<GLuint, 2> _dimensions = {0, 0};

public:
    ltexture();
    ~ltexture();

    /*
    pre-condition: valid GL context
    post-condition:
        * creates a texture from the given pixels
        * reports error to console if texture could not be created
    side-effects:
        * binds a null-texture
    */
    bool load_from_pixels32(GLuint*, std::array<GLuint, 2>);

    bool load_from_file(std::string_view);

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
