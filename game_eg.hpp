#ifndef game_eg_hpp__
#include "board.hpp"
#include <panel.h>
#include <utility>
//#include <ncurses.h>
#define game_eg_hpp__


struct pose
{
    int x,y;
    pose()
    {
        x=0;
        y=0;
    }
    int min(int a,int b)
    {
        return (a<b?a:b);
    }
    int max(int a,int b)
    {
        return (a>b?a:b);
    }
    void normalise(int n)
    {
        this->x=max(0,min(this->x,n-1));
        this->y=max(0,min(this->y,n-1));
    }
};


struct game_state
{
    pose cur;
    Board *b;
    GameFieldStates chps;
    state stch;
    int player;
    int scores[2];
    game_state(Board *b)
    {
        this->chps=GameFieldStates(b->dx,b->dy);
        this->b=b;
        this->player=0;
        this->scores[0]=0;
        this->scores[1]=0;
    }

    template <class T> void swap (T& a, T& b) //just swap from std
    {
    T c(std::move(a)); a=std::move(b); b=std::move(c);
    }

    void move_cur(int dx, int dy)
    {
        swap((this->stch),(this->b->states[this->cur.x][this->cur.y]));
        this->cur.x+=dx;
        this->cur.y+=dy;
        this->cur.normalise(this->b->n);
        if (this->b->states[this->cur.x][this->cur.y].name!='e')
            this->chps.Choice.bcolor=this->b->states[this->cur.x][this->cur.y].color;
        else
            this->chps.Choice.bcolor=0;
        this->stch=this->chps.Choice;
        swap((this->stch),(this->b->states[this->cur.x][this->cur.y]));
    }
    
};


void game(game_state *game);

#endif