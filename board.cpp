#include <panel.h>
#include <stdexcept>
#include "board.hpp"

/*struct Board
{
    WINDOW *mainp;
    WINDOW *wins[8][8];
    PANEL *panels[8][8];
    int state[8][8];
    int xs,ys;
};*/

void init_screen()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    start_color();
    //check();
}


Board* init_board(int xs, int ys, int dx,int dy, int n)
{
    
    
    //init_colorpairs();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    
    /*init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    
    init_pair(5, COLOR_MAGENTA, COLOR_WHITE);
    init_pair(6, COLOR_GREEN, COLOR_WHITE);

    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_WHITE);*/
    
    Board *b = new Board(dx,dy);
    b->xs=xs;
    b->ys=ys;
    b->n=n;
    b->mainp=newwin(dy*(b->n+2), dx*(b->n+2), ys, xs);
    b->mainpl=new_panel(b->mainp);
    for (int x=0;x<(b->n);x++)
        for (int y=0;y<(b->n);y++)
        {
            //b->state[x][y]=2-((x+y)%2==0);
            b->wins[x][y]= subwin(b->mainp,3, 3, 3*x, 3*y);
            b->panels[x][y] = new_panel(b->wins[x][y]);
        }
    //redraw_board(b);
    return b;
}

void redraw_board(Board *b)
{
    del_panel(b->mainpl);
	delwin(b->mainp);
    b->mainp=newwin((b->dy)*(b->n+2), (b->dx)*(b->n+2), b->ys, b->xs);
    b->mainpl=new_panel(b->mainp);
    for (int x=0;x<(b->n)*(b->dx);x++)
    {
        mvwprintw(b->mainp,0,x+(b->dx),"-");
        mvwprintw(b->mainp,(b->dy)-1,x+(b->dx),"-");
        
        mvwprintw(b->mainp,(b->n+2)*(b->dy)-1,x+(b->dx),"-");
        mvwprintw(b->mainp,(b->n+1)*(b->dy),x+(b->dx),"-");
    }
    for (int y=0;y<(b->n)*(b->dy);y++)
    {
        mvwprintw(b->mainp,y+(b->dy),0,"|");
        mvwprintw(b->mainp,y+(b->dy),(b->dx)-1,"|");

        mvwprintw(b->mainp,y+(b->dy),(b->n+2)*(b->dx)-1,"|");
        mvwprintw(b->mainp,y+(b->dy),(b->n+1)*(b->dx),"|");
    }
    for (int x=0;x<(b->n);x++)
    {
        mvwprintw(b->mainp,(b->dy)/2,(x+1)*(b->dx)+(b->dx)/2,"%c",'A'+x);
        
        mvwprintw(b->mainp,(b->n+1)*(b->dy)+(b->dy)/2,(x+1)*(b->dx)+(b->dx)/2,"%c",'A'+x);
    }
    for (int y=(b->n-1);y>-1;y--)
    {
        int l=std::to_string(y+1).size();
        mvwprintw(b->mainp,(b->n-y)*(b->dy)+(b->dy)/2,(b->dx)/2-l/2,"%s",std::to_string(y+1).c_str());

        mvwprintw(b->mainp,(b->n-y)*(b->dy)+(b->dy)/2,(b->n+1)*(b->dx)+(b->dx)/2-l/2,"%s",std::to_string(y+1).c_str());
    }
    for (int xs=0;xs<2;xs++)
        for (int ys=0;ys<2;ys++)
            for (int x=0;x<(b->dx);x++)
                for (int y=0;y<(b->dy);y++)
                    if ((x==0) + (y==0) + (x==(b->dx)-1) + (y==(b->dy)-1)>=2)
                        mvwprintw(b->mainp,y+ys*(b->n+1)*(b->dy),x+(b->n+1)*(b->dx)*xs,"#");
                    else if ((x==0) || (x==(b->dx)-1))
                            mvwprintw(b->mainp,y+ys*(b->n+1)*(b->dy),x+(b->n+1)*(b->dx)*xs,"|");
                        else if ((y==0) || (y==(b->dy)-1))
                            mvwprintw(b->mainp,y+ys*(b->n+1)*(b->dy),x+(b->n+1)*(b->dx)*xs,"-");
                        else
                            mvwprintw(b->mainp,y+ys*(b->n+1)*(b->dy),x+(b->n+1)*(b->dx)*xs,"@");
    
    for (int x=0;x<(b->n);x++)
        for (int y=0;y<(b->n);y++)
        {
            del_panel(b->panels[x][y]);
	        delwin(b->wins[x][y]);
            b->wins[x][y] = subwin(b->mainp,(b->dy), (b->dx), b->ys+(y+1)*(b->dy), b->xs+(x+1)*(b->dx));
	        wbkgdset(b->wins[x][y], COLOR_PAIR((x+y)%2+1));
	        //wclear(b->wins[x][y]);
	        //wrefresh(b->wins[x][y]);
            b->panels[x][y] = new_panel(b->wins[x][y]);
        }
    //update_panels();
	//doupdate();
    fupdate(b);
}

void fupdate(Board *b)
{
    for (int x=0;x<(b->n);x++)
        for (int y=0;y<(b->n);y++)
        {
            werase(b->wins[x][y]);
            if (b->states[x][y].name=='c' && b->states[x][y].bcolor!=0)
            {
                init_pair(40+x+b->n*y, b->states[x][y].color, b->states[x][y].bcolor);
            }
            else 
            {
                init_pair(40+x+b->n*y, b->states[x][y].color, ((x+y)%2==1?COLOR_BLACK:COLOR_WHITE));
            }
            wbkgdset(b->wins[x][y], COLOR_PAIR(40+x+b->n*y));
            mvwprintw(b->wins[x][y],0,0,"%s",b->states[x][y].pic.c);
        }
    update_panels();
	doupdate();
}