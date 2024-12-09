#ifndef message_hpp__
#include <panel.h>
//#include <ncurses.h>
#define message_hpp__

void InfoMessage(char const* str,int max_l=36);

bool YNMessage(char const* str,int max_l=36);

#endif