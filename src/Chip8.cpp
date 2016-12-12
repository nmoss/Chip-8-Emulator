#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include "Memory.h"
#include "Interpreter.h"

void draw(unsigned char gfx[]);
void graphics_init();

int main(int argc, char** argv){

    graphics_init();

    //std::cout << argv[1] << std::endl;
    Memory mem(argv[1]);
    Memory *m = &mem;
    Interpreter p(m);

    for(;;){

        p.execute();

        if(p.drawFlag){
            draw(m->gfx);
            p.drawFlag = false;
        }
        usleep(1667);
        //sleep(1);


    }

    return 0;
}

void draw(unsigned char gfx[]){
    for(int i = 0; i < 32; i++){
        move(i, 0);
        for(int j = 0; j < 64; j++){
            //unsigned char value = gfx[j+i*64] ? 0xFFFFFFFF : 0;
            //mvwaddch(stdscr, j, i, value);
            //mvwprintw(stdscr, j, i, "%x", value);
            addch(gfx[j + i * 64] ? ' ' | A_REVERSE : ' ');

            //refresh();
        }
    }
    refresh();
}

void graphics_init(){
    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
}
