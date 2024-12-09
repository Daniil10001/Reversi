#ifndef board_hpp__
#include <panel.h>
//#include <ncurses.h>
#define board_hpp__

struct picture
{
    int xsz,ysz;
    char c[256];
    picture()
    {
        this->c[0]='\0';
        this->xsz=0;
        this->ysz=0;
    }
    void chip(int xsz,int ysz)
    {
        this->xsz=xsz;
        this->ysz=ysz;
        this->c[(xsz)*ysz]='\0';
        for (int x=0;x<xsz;x++)
            for (int y=0;y<ysz;y++)
            {
                //if (x==xsz)
                //    this->c[x+y*xsz]='\n';
                if ((x==xsz/2 || x==(xsz-1)/2) && (y==ysz/2 || y==(ysz-1)/2))
                    this->c[x+y*xsz]='%';
                else if (x==0 || y==0 || x==xsz-1 || y==ysz-1)
                    this->c[x+y*xsz]='*';
                else
                    this->c[x+y*xsz]=' ';
            }
    }
    void fill_ch(int xsz,int ysz,char cz)
    {
        this->xsz=xsz;
        this->ysz=ysz;
        this->c[(xsz+1)*ysz]='\0';
        for (int x=0;x<xsz;x++)
            for (int y=0;y<ysz;y++)
            {
                if (x==xsz-1)
                    this->c[x+y*xsz]='\n';
                else 
                    this->c[x+y*xsz]=cz;
            }
    }
    void cborder(int xsz,int ysz,char ch)
    {
        this->xsz=xsz;
        this->ysz=ysz;
        for (int x=0;x<this->xsz;x++)
            for (int y=0;y<this->ysz;y++)
            {
                if  (x==0 || y==0 || x==xsz-1 || y==ysz-1)
                    this->c[x+y*xsz]=ch;
                else
                    this->c[x+y*xsz]=' ';
            }
    }
    /*~picture()
    {
        delete[] this->c;
    }*/
};

struct state
{
    int color;
    int bcolor;
    picture pic;
    char name;
    state()
    {
        name='e';
        this->bcolor=0;
        this->color=COLOR_WHITE;
        this->pic=picture();
    }
};

struct Board
{
    WINDOW *wins[20][20];
    PANEL *panels[20][20];
    WINDOW *mainp;
    PANEL *mainpl;
    state states[20][20];
    int n;
    int xs,ys, dx,dy;
    Board(int dx,int dy)
    {
        this->dx=dx;
        this->dy=dy;
    }
};

struct GameFieldStates
{
    state Empty;
    state FirstP,SecondP;
    state Choice;
    GameFieldStates(){}
    GameFieldStates(int dx, int dy)
    {
        this->FirstP.pic.chip(dx,dy);
        this->SecondP.pic.chip(dx,dy);
        this->Choice.pic.cborder(dx,dy,'~');
        this->FirstP.color=COLOR_GREEN;
        this->SecondP.color=COLOR_MAGENTA;
        this->Choice.color=COLOR_CYAN;
        this->Choice.name='c';
        this->FirstP.name='f';
        this->SecondP.name='s';
    }
};
// ?'"~`"

void redraw_board(Board *b);
Board* init_board(int xs, int ys, int dx, int dy, int n);
void init_screen();
void fupdate(Board *b);

#endif