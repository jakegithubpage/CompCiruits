/* intent to write a base popup window that builds a diagram with listed values 
pulled from randomized seeding and selected options in the circuit build section*/
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cairo/cairo.h>
#include <stdbool.h>
#include <stdint.h> 
#include "CktEngine.h"
#include "BaseGUI.h"

volatile int rateSelect;



int BaseGUI_Run(unsigned nodeCount) { 
    CB_Ckt ckt;
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;

    //Build window - Check for invalid build
    SDL_Window* win = SDL_CreateWindow(
        "Circuit Pop-up GUI",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1000, 750, SDL_WINDOW_SHOWN
    );
    if (!win) return 1; 

    rateSelect = (int)nodeCount;
    //Render - check if rendered properly
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!ren) return 1;

    //Dimensions for window initialization and creating process
    const int W = 1000, H = 750;
    cairo_surface_t* surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, W, H);
    cairo_t* cr = cairo_create(surf);

    //BackGround - white
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    if (rateSelect == 1u) {
        // "Wire" - schematic
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
        cairo_set_line_width(cr, 3.0);
        cairo_move_to(cr, 300, 200);
        cairo_line_to(cr, 400, 200);
        cairo_stroke(cr);

        // "Resistor Zig-Zag" - schematic
        cairo_move_to(cr, 400, 200);
        cairo_line_to(cr, 410, 185);
        cairo_line_to(cr, 430, 215);
        cairo_line_to(cr, 450, 185);
        cairo_line_to(cr, 470, 215);
        cairo_line_to(cr, 480, 200);
        cairo_stroke(cr);

        // "Wire" - schematic
        cairo_move_to(cr, 480, 200);
        cairo_line_to(cr, 580, 200);
        cairo_stroke(cr);

        // "Wire" - schematic
        cairo_move_to(cr, 580, 200);
        cairo_line_to(cr, 580, 300);
        cairo_stroke(cr);

        // "Resistor zig zag" 
        cairo_move_to(cr, 580, 300);
        cairo_line_to(cr, 595, 310);
        cairo_line_to(cr, 565, 330);
        cairo_line_to(cr, 595, 350);
        cairo_line_to(cr, 565, 370);
        cairo_line_to(cr, 580, 380);
        cairo_stroke(cr);

        // Wire - below 2nd Resistor
        cairo_move_to(cr, 580, 380);
        cairo_line_to(cr, 580, 480);
        cairo_stroke(cr);

        //GND Point - bottom of Voltage divider
        cairo_move_to(cr, 580, 480);
        cairo_line_to(cr, 300, 480);
        cairo_stroke(cr);

        //WIRE up to voltage Source
        cairo_move_to(cr, 300, 480);
        cairo_line_to(cr, 300, 380);
        cairo_stroke(cr);

        //Source symbol
        cairo_arc(cr, 300.0, 340.0, 40.0, 0.0, 6.283185307179586); // 2*pi
        cairo_stroke(cr);

        //Plus Minus signs
        cairo_move_to(cr, 310, 320);
        cairo_line_to(cr, 290, 320);
        cairo_move_to(cr, 300, 310);
        cairo_line_to(cr, 300, 330);
        cairo_move_to(cr, 310, 360);
        cairo_line_to(cr, 290, 360);
        cairo_stroke(cr);

        //Wire above Voltage source
        cairo_move_to(cr, 300, 300);
        cairo_line_to(cr, 300, 200);
        cairo_stroke(cr);

        
        cairo_surface_flush(surf);
    }
    else if (rateSelect == 2u) {
        // "Wire" - schematic
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
        cairo_set_line_width(cr, 3.0);
        cairo_move_to(cr, 300, 200);
        cairo_line_to(cr, 400, 200);
        cairo_stroke(cr);

        // "Resistor Zig-Zag" - schematic
        cairo_move_to(cr, 400, 200);
        cairo_line_to(cr, 410, 185);
        cairo_line_to(cr, 430, 215);
        cairo_line_to(cr, 450, 185);
        cairo_line_to(cr, 470, 215);
        cairo_line_to(cr, 480, 200);
        cairo_stroke(cr);

        // "Wire" - schematic
        cairo_move_to(cr, 480, 200);
        cairo_line_to(cr, 580, 200);
        cairo_stroke(cr);

        // "Wire" - schematic
        cairo_move_to(cr, 580, 200);
        cairo_line_to(cr, 580, 300);
        cairo_stroke(cr);

        // "Resistor zig zag" 
        cairo_move_to(cr, 580, 300);
        cairo_line_to(cr, 595, 310);
        cairo_line_to(cr, 565, 330);
        cairo_line_to(cr, 595, 350);
        cairo_line_to(cr, 565, 370);
        cairo_line_to(cr, 580, 380);
        cairo_stroke(cr);

        // Wire - below 2nd Resistor
        cairo_move_to(cr, 580, 380);
        cairo_line_to(cr, 580, 480);
        cairo_stroke(cr);

        //GND Point - bottom of Voltage divider
        cairo_move_to(cr, 580, 480);
        cairo_line_to(cr, 300, 480);
        cairo_stroke(cr);

        //WIRE up to voltage Source
        cairo_move_to(cr, 300, 480);
        cairo_line_to(cr, 300, 380);
        cairo_stroke(cr);

        //Source symbol
        cairo_arc(cr, 300.0, 340.0, 40.0, 0.0, 6.283185307179586); // 2*pi
        cairo_stroke(cr);

        //Plus Minus signs
        cairo_move_to(cr, 310, 320);
        cairo_line_to(cr, 290, 320);
        cairo_move_to(cr, 300, 310);
        cairo_line_to(cr, 300, 330);
        cairo_move_to(cr, 310, 360);
        cairo_line_to(cr, 290, 360);
        cairo_stroke(cr);

        //Wire above Voltage source
        cairo_move_to(cr, 300, 300);
        cairo_line_to(cr, 300, 200);
        cairo_stroke(cr);

        //Start of second Mesh loop 
        cairo_move_to(cr, 580, 200);
        cairo_line_to(cr, 680, 200);
        cairo_stroke(cr);

        //Second mesh top resistor 
        cairo_move_to(cr, 680, 200);
        cairo_line_to(cr, 690, 185);
        cairo_move_to(cr, 710, 215);
        cairo_line_to(cr, 730, 185);
        cairo_move_to(cr, 750, 215);
        cairo_line_to(cr, 760, 200);
        cairo_stroke(cr);

        //Second Mesh top right wire
        cairo_move_to(cr, 760, 200);
        cairo_line_to(cr, 860, 200);
        cairo_stroke(cr);

        //Second Mesh outside wall
        cairo_move_to(cr, 860, 200);
        cairo_line_to(cr, 860, 300);
        cairo_stroke(cr);
        
       
    }
    /*else if (rateSelect == 3u) {

    }*/
    SDL_Texture* tex = SDL_CreateTexture(
        ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, W, H
    );
    SDL_UpdateTexture(tex, NULL, cairo_image_surface_get_data(surf), cairo_image_surface_get_stride(surf));

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) { 
            if (e.type == SDL_QUIT) running = false;
        }
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, NULL);
        SDL_RenderPresent(ren);
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    cairo_destroy(cr);
    cairo_surface_destroy(surf);
    SDL_Quit();
    return 0;
}