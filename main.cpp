#include<ncurses.h>
#include <csignal>
#include <chrono>
#include <string>
#include "board.hpp"
#include "game_eg.hpp"
#include "message.hpp"

Board *b;
// install my_handler
void rsz_handler( int sig_number );

bool check()
{
    int row, col;
    getmaxyx(stdscr, row, col);
    mvwprintw(stdscr,2,2,"%i %i", row, col);
    if (row<37 || col<94)
    {
        endwin();
        InfoMessage("Plese make window wider!");
        rsz_handler( 0);
        return true;
    }
    rsz_handler( 0);
    return false;
}

void rsz_handler( int sig_number ) // sig. handler
{
    endwin();
    //init_screen();
    //check();
    wclear(stdscr);
    box(stdscr, ACS_VLINE,ACS_HLINE);
    mvwprintw(stdscr,1,1,"%s","Move through arrows and choose through space. For exit press F1 or ESC");
    doupdate();
    //redraw_board(b);
}

int main()
{
    signal( SIGWINCH, rsz_handler );

    init_screen();

    while (check());
    box(stdscr, ACS_VLINE,ACS_HLINE);
    mvwprintw(stdscr,1,1,"%s","Move through arrows and choose through space. For exit press F1 or ESC");

    //GameFieldStates chps= GameFieldStates(b->dx,b->dy);
    //b->states[0][0]=chps.Choice;
    //b->state[0][1]=6;
    //b->state[0][2]=4;
    //b->state[0][3]=5;
    //redraw_board(b);
    //printw("%i %i \n",b->pics[0].xsz,b->pics[0].ysz);
    //printw("%s",b->pics[0].c);
    //InfoMessage("something !!veeeeeeeeeeeeeeeeeeeeee!!rrrrrrrrrrrrrrrryyyyyyyyyy!! !!looooooonnnnnn!!ggggggggggggggggggggg!!");
    //InfoMessage(((std::string)"You choose "+(std::string)(YNMessage("Do a right choice")? "Yes" : "No")+(std::string)". Press F1 again to exit.").c_str());
    /*int ch;
    while((ch = getch()) != KEY_F(1))
    {	
    	switch(ch)
	    {
	        case '\t': // переход на следующую панель
			    break;
        }
	
	// обновление и перерисовка
    //refresh();
    //doupdate();
    }*/
    while (YNMessage("Do you want to start the game?"))
    {   
        b=init_board(5,4,6,3,8);
        game_state g=game_state(b);
        b->states[3][3]=g.chps.FirstP;
        b->states[3][4]=g.chps.SecondP;
        b->states[4][4]=g.chps.FirstP;
        b->states[4][3]=g.chps.SecondP;
        game(&g);
        delete b;
    }
    endwin();
    return 0;
}