    #ifndef UTILITIES_H
    #define UTILITIES_H

    #include <iostream>
    #include <SDL3/SDL.h>
    #include <SDL3_image/SDL_image.h>
    #include <SDL3/SDL_render.h>
   #define SCREEN_WIDTH 1920
   #define SCREEN_HEIGHT 1080

   struct Vector{
    float x, y;
    float normalize();
   };

   float Vector::normalize(){

   }
    #endif