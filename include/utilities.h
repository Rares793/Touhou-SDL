    #ifndef UTILITIES_H
    #define UTILITIES_H

    #include <iostream>
    #include <cmath>
    #include <stddef.h>
    #include <SDL3/SDL.h>
    #include <SDL3_image/SDL_image.h>
    #include <SDL3/SDL_render.h>

    #define SCREEN_WIDTH 1920
     #define SCREEN_HEIGHT 1080

    namespace usefulFunctions{
        float inline random_float(){
            // Returns a random real number in the [0, 1) range
            return std::rand() / (RAND_MAX + 1.0);
        }

        float inline random_float(float min, float max){
            // Returns a random real number in the [min, max) range
            return min + (max - min) * random_float();
        }
        
    }
#endif