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



int BaseGUI_Run(CB_Ckt *ckt, unsigned nodeCount) { 

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
    const int W = 1200, H = 1000;
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

        //Source Symbol
        for (unsigned i = 0; i < ckt->sourceCount; i++) {
            const CB_Source *s = &ckt->sources[i];
            if (s->type == CB_SRC_VoltageDC) {
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
            }
            else if (s->type == CB_SRC_CurrentDC) {
                cairo_arc(cr, 300.0, 340.0, 40.0, 0.0, 6.283185307179586); // 2*pi
                cairo_stroke(cr);
                //Current sign
                cairo_move_to(cr, 300, 320);
                cairo_line_to(cr, 300, 360);
                cairo_move_to(cr, 300, 320);
                cairo_line_to(cr, 290, 330);
                cairo_move_to(cr, 300, 320);
                cairo_line_to(cr, 310, 330);
                cairo_stroke(cr);
            }
        }

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
        for (unsigned i = 0; i < ckt->sourceCount; i++) {
            const CB_Source *s = &ckt->sources[i];
            if (s->type == CB_SRC_VoltageDC) {
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
            }
            else if (s->type == CB_SRC_CurrentDC) {
                cairo_arc(cr, 300.0, 340.0, 40.0, 0.0, 6.283185307179586); // 2*pi
                cairo_stroke(cr);

                cairo_move_to(cr, 300, 320);
                cairo_line_to(cr, 300, 360);
                cairo_move_to(cr, 300, 320);
                cairo_line_to(cr, 290, 330);
                cairo_move_to(cr, 300, 320);
                cairo_line_to(cr, 310, 330);
                cairo_stroke(cr);
            }
        }
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
        cairo_line_to(cr, 710, 215);
        cairo_line_to(cr, 730, 185);
        cairo_line_to(cr, 750, 215);
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

        //Resistor "outter second mesh"
        cairo_move_to(cr, 860, 300);
        cairo_line_to(cr, 845, 310);
        cairo_line_to(cr, 875, 330);
        cairo_line_to(cr, 845, 350);
        cairo_line_to(cr, 875, 370);
        cairo_line_to(cr, 860, 380);
        cairo_stroke(cr);

        //Bottom wire - Second mesh outter branch
        cairo_move_to(cr, 860, 380);
        cairo_line_to(cr, 860, 480);
        cairo_stroke(cr);

        //GND rail Wire - Second Mesh
        cairo_move_to(cr, 860, 480);
        cairo_line_to(cr, 580, 480);
        cairo_stroke(cr);
        
       
    }
    else if (rateSelect == 3u) {
        // "Wire" - schematic
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
        cairo_set_line_width(cr, 3.0);
        cairo_move_to(cr, 300, 400);
        cairo_line_to(cr, 400, 400);
        cairo_stroke(cr);

        // "Resistor Zig-Zag" - schematic
        cairo_move_to(cr, 400, 400);
        cairo_line_to(cr, 410, 385);
        cairo_line_to(cr, 430, 415);
        cairo_line_to(cr, 450, 385);
        cairo_line_to(cr, 470, 415);
        cairo_line_to(cr, 480, 400);
        cairo_stroke(cr);

        // "Wire" - schematic
        cairo_move_to(cr, 480, 400);
        cairo_line_to(cr, 580, 400);
        cairo_stroke(cr);

        // "Wire" - schematic
        cairo_move_to(cr, 580, 400);
        cairo_line_to(cr, 580, 500);
        cairo_stroke(cr);

        // "Resistor zig zag" 
        cairo_move_to(cr, 580, 500);
        cairo_line_to(cr, 595, 510);
        cairo_line_to(cr, 565, 530);
        cairo_line_to(cr, 595, 550);
        cairo_line_to(cr, 565, 570);
        cairo_line_to(cr, 580, 580);
        cairo_stroke(cr);

        // Wire - below 2nd Resistor
        cairo_move_to(cr, 580, 580);
        cairo_line_to(cr, 580, 680);
        cairo_stroke(cr);

        //GND Point - bottom of Voltage divider
        cairo_move_to(cr, 580, 680);
        cairo_line_to(cr, 300, 680);
        cairo_stroke(cr);

        //WIRE up to voltage Source
        cairo_move_to(cr, 300, 680);
        cairo_line_to(cr, 300, 580);
        cairo_stroke(cr);

        

        //Wire above Voltage source
        cairo_move_to(cr, 300, 500);
        cairo_line_to(cr, 300, 400);
        cairo_stroke(cr);

        //Start of second Mesh loop 
        cairo_move_to(cr, 580, 400);
        cairo_line_to(cr, 680, 400);
        cairo_stroke(cr);

        //Second mesh top resistor 
        cairo_move_to(cr, 680, 400);
        cairo_line_to(cr, 690, 385);
        cairo_line_to(cr, 710, 415);
        cairo_line_to(cr, 730, 385);
        cairo_line_to(cr, 750, 415);
        cairo_line_to(cr, 760, 400);
        cairo_stroke(cr);

        //Second Mesh top right wire
        cairo_move_to(cr, 760, 400);
        cairo_line_to(cr, 860, 400);
        cairo_stroke(cr);

        //Second Mesh outside wall
        cairo_move_to(cr, 860, 400);
        cairo_line_to(cr, 860, 500);
        cairo_stroke(cr);
        for (unsigned i = 0; i < ckt->sourceCount; i++) {
            const CB_Source *s = &ckt->sources[i];
            //Source symbol two - "outter second mesh"
            if (i == 1) {
                if (s->type == CB_SRC_VoltageDC) {
                    //Voltage Source DC #2
                    cairo_arc(cr, 860.0, 540.0, 40.0, 0.0, 6.283185307179586); // 2*pi
                    cairo_stroke(cr);

                    //Plus/Minus signs two
                    cairo_move_to(cr, 870, 520);
                    cairo_line_to(cr, 850, 520);
                    cairo_move_to(cr, 860, 510);
                    cairo_line_to(cr, 860, 530);
                    cairo_move_to(cr, 870, 560);
                    cairo_line_to(cr, 850, 560);
                    cairo_stroke(cr);
                }
                else if (s->type == CB_SRC_CurrentDC) {
                    //Current Source DC
                    cairo_arc(cr, 860.0, 540.0, 40.0, 0.0, 6.283185307179586); // 2*pi
                    cairo_stroke(cr);

                    cairo_move_to(cr, 860, 520);
                    cairo_line_to(cr, 860, 560);
                    cairo_move_to(cr, 860, 520);
                    cairo_line_to(cr, 870, 530);
                    cairo_move_to(cr, 860, 520);
                    cairo_line_to(cr, 850, 530);
                    cairo_stroke(cr);
                }
            }
            //Source Symbol 1
            else if (i == 0) {
                //Voltage source - source #1
                if (s->type == CB_SRC_VoltageDC) {
                    cairo_arc(cr, 300.0, 540.0, 40.0, 0.0, 6.283185307179586); // 2*pi
                    cairo_stroke(cr);

                    //Plus Minus signs
                    cairo_move_to(cr, 310, 520);
                    cairo_line_to(cr, 290, 520);
                    cairo_move_to(cr, 300, 510);
                    cairo_line_to(cr, 300, 530);
                    cairo_move_to(cr, 310, 560);
                    cairo_line_to(cr, 290, 560);
                    cairo_stroke(cr);
                }
                else if (s->type == CB_SRC_CurrentDC) {
                    cairo_arc(cr, 300.0, 540.0, 40.0, 0.0, 6.283185307179586); // 2*pi
                    cairo_stroke(cr);

                    cairo_move_to(cr, 300, 520);
                    cairo_line_to(cr, 300, 560);
                    cairo_move_to(cr, 300, 520);
                    cairo_line_to(cr, 310, 530);
                    cairo_move_to(cr, 300, 520);
                    cairo_line_to(cr, 290, 530);
                    cairo_stroke(cr);
                }
            }
        }   
        //Bottom wire - Second mesh outter branch
        cairo_move_to(cr, 860, 580);
        cairo_line_to(cr, 860, 680);
        cairo_stroke(cr);

        //GND rail Wire - Second Mesh
        cairo_move_to(cr, 860, 680);
        cairo_line_to(cr, 580, 680);
        cairo_stroke(cr);

        //Start of Top mesh
        cairo_move_to(cr, 580, 400);
        cairo_line_to(cr, 580, 300);
        cairo_stroke(cr);

        //First left wall Resistor
        cairo_move_to(cr, 580, 300);
        cairo_line_to(cr, 565, 290);
        cairo_line_to(cr, 595, 270);
        cairo_line_to(cr, 565, 250);
        cairo_line_to(cr, 595, 230);
        cairo_line_to(cr, 580, 220);
        cairo_stroke(cr);
        
        //Left wall top wire
        cairo_move_to(cr, 580, 220);
        cairo_line_to(cr, 580, 120);
        cairo_stroke(cr);

        //Top wall Left wire
        cairo_move_to(cr, 580, 120);
        cairo_line_to(cr, 680, 120);
        cairo_stroke(cr);

        //Top resistor top mesh
        cairo_move_to(cr, 680, 120);
        cairo_line_to(cr, 690, 105);
        cairo_line_to(cr, 710, 135);
        cairo_line_to(cr, 730, 105);
        cairo_line_to(cr, 750, 135);
        cairo_line_to(cr, 760, 120);
        cairo_stroke(cr);

        //Top mesh Top right wire
        cairo_move_to(cr, 760, 120);
        cairo_line_to(cr, 860, 120);
        cairo_stroke(cr);

        //Top Mesh Right wall top wire
        cairo_move_to(cr, 860, 120);
        cairo_line_to(cr, 860, 220);
        cairo_stroke(cr);

        //Top Mesh right wall resistor
        cairo_move_to(cr, 860, 220);
        cairo_line_to(cr, 875, 230);
        cairo_line_to(cr, 845, 250);
        cairo_line_to(cr, 875, 270);
        cairo_line_to(cr, 845, 290);
        cairo_line_to(cr, 860, 300);
        cairo_stroke(cr);

        //Top mesh right wall bottom wire
        cairo_move_to(cr, 860, 300);
        cairo_line_to(cr, 860, 400);
        cairo_stroke(cr);

    }
    else {
        return 0;
    }
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