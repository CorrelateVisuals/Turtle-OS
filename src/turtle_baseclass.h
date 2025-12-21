// make all functions virtual (override)
#include <GLES2/gl2.h>

#include <iostream>

class TurtleBaseclass{
    public:
    TurtleBaseclass() = default;
    ~TurtleBaseclass() = default;

    void run(){};

    

    struct ZoomEffect{
        ZoomEffect(float start, float speed, float final) : _texture_start_scale{start},_zoom_speed(speed),_texture_final_scale(final - speed){
            std::cout << "Initializing ZoomEffect." << std::endl;
            getSettings();
        };
        GLint _scale_location{-1};
        float _texture_start_scale{-1.0f};
        float _zoom_speed{-1.0f};
        float _texture_final_scale{-1.0f};

        private:

        void getSettings(){
            std::cout << "ZoomEffect _scale_location: " <<_scale_location << ",  _texture_start_scale: " << _texture_start_scale << ", _zoom_speed: " << _zoom_speed << ", _texture_final_scale:" << _texture_final_scale << std::endl << std::endl;
        };

    };

    protected:

    private:

};