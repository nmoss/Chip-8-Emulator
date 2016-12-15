#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include "Memory.h"
#include "Interpreter.h"

void keyboard_handler(unsigned char keys[]);
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

        for(int i = 0; i < 16; i++){
            //m->key[i] = 0;
        }
        keyboard_handler(m->key);
        usleep(2000);
        //sleep(1);


    }

    return 0;
}

void keyboard_handler(unsigned char keys[]){
    int ch = 99;
    if((ch = getch()) != ERR){
        switch(ch){
            case '1':
                keys[0] = !keys[0];
            break;
            case '2':
                keys[1] = !keys[1];
            break;
            case '4':
                keys[2] = !keys[2];
            break;
            case 'q':
                keys[3] = !keys[3];
            break;
            case 'w':
                keys[4] = !keys[4];
            break;
            case 'e':
                keys[5] = !keys[5];
            break;
            case 'r':
                keys[6] = !keys[6];
            break;
            case 'a':
                keys[7] = !keys[7];
            break;
            case 's':
                keys[8] = !keys[8];
            break;
            case 'd':
                keys[9] = !keys[9];
            break;
            case 'f':
                keys[10] = !keys[10];
            break;
            case 'z':
                keys[11] = !keys[11];
            break;
            case 'x':
                keys[12] = !keys[12];
            break;
            case 'c':
                keys[13] = !keys[13];
            break;
            case 'v':
                keys[14] = !keys[14];
            break;
            case '3':
                keys[15] = !keys[15];
            break;
        }
    }
}

void draw(unsigned char gfx[]){
    //clear();
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
