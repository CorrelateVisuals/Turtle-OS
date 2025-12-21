// make all functions virtual (override)
#include <GLES2/gl2.h>

class TurtleBaseclass{
    public:
    TurtleBaseclass() = default;
    ~TurtleBaseclass() = default;

    void run(){};

    protected:

    private:

    struct ZoomEffect{
        ZoomEffect(float start, float speed, float final) : _texture_start_scale{start},_zoom_speed(speed),_texture_final_scale(final){};
        GLint _scale_location{0};
        float _texture_start_scale = 15.0f;
        float _zoom_speed = 0.1f;
        float _texture_final_scale = 2.0f + _zoom_speed ;
    };

    ZoomEffect _zoom_effect{15.0f, 0.1f, 2.0f};

};