#include <panel.h>
#include "message.hpp"


int str_size(char const* str)
{
    int sz=0;
    for (;str[sz]!='\0';sz++){}
    return sz;
}

int min(int a,int b)
{
    return (a<b?a:b);
}

int max(int a,int b)
{
    return (a>b?a:b);
}

struct STR
{
    char s[30][30] ;
    int ns[30] ;
    int n;
};

struct lines
{
    char lns[30][34];
    int sz[20];
    int n;
};


lines get_l(char const* str, int max_l)
{
    STR s;
    lines l;
    s.n=1;
    s.ns[0]=0;
    for (int i=0;str[i]!='\0';i++)
    {
        s.s[s.n-1][s.ns[s.n-1]]=str[i];
        s.ns[s.n-1]++;
        if (str[i]==' ' || s.ns[s.n-1]==30)
        {
            s.ns[s.n]=0;
            s.n++;
        }
    }
    l.n=1;
    for (int i=0;i<20;i++)
        l.sz[i]=0;
    for (int i=0;i<s.n;i++)
    {
        if (l.sz[l.n-1]==0 && s.ns[i]>max_l)
        {
            for (int j=0;j<max_l;j++)
                l.lns[l.n-1][l.sz[l.n-1]+j]=s.s[i][j];
            l.sz[l.n-1]+=max_l;
            l.lns[l.n-1][l.sz[l.n-1]]='\0';
            l.n++;
            for (int j=max_l;j<s.ns[i];j++)
                s.s[i][j-max_l]=s.s[i][j];
            s.ns[i]-=max_l;
            i--;
        }
        else if (s.ns[i]+l.sz[l.n-1]<=max_l)
        {
            for (int j=0;j<s.ns[i];j++)
                l.lns[l.n-1][l.sz[l.n-1]+j]=s.s[i][j];
            l.sz[l.n-1]+=s.ns[i];

        }
        else
        {
            l.lns[l.n-1][l.sz[l.n-1]]='\0';
            l.n++;
            i--;
        }
    }
    l.lns[l.n-1][l.sz[l.n-1]]='\0';
    return l;
}

void print_centred(WINDOW *win,char const* str,int ys,int max_l,int max_lw)
{
    
    int sz=str_size(str);
    for (int i=0;i<(sz+max_l-1)/(max_l);i++)
        for (int j=0;j+i*max_l<sz && j<max_l;j++)
        {
            mvwprintw(win,ys+i,(max_lw-min(sz-max_l*i,max_l))/2+j,"%c",str[j+i*max_l]);
            wmove(win, ys+i,(max_lw-min(sz-max_l*i,max_l))/2+j);
        }
}

void print_centred(WINDOW *win,lines l,int ys,int max_l,int max_lw)
{
    for (int i=0;i<l.n;i++)
       // for (int j=0;j+i*max_l<sz && j<max_l;j++)
        {
            mvwprintw(win,ys+i,(max_lw-l.sz[i])/2,"%s",l.lns[i]);
            wmove(win, ys+i,(max_lw-l.sz[i])/2-1);
        }
}

void InfoMessage(char const* str, int max_l)
{
    int row,col;
    lines l=get_l(str, max_l-4);
    int ym=l.n+8;//(sz+max_l-5)/(max_l-4)+8;
    getmaxyx(stdscr,row,col);
    WINDOW *win=newwin(ym, max_l, (row-ym)/2, (col-max_l)/2);
    mvwhline(win, 0, 0, ACS_HLINE, max_l);
    mvwhline(win, 2, 0, ACS_HLINE, max_l);
    mvwhline(win, ym-1, 0, ACS_HLINE, max_l);
    mvwvline(win, 0, 0, ACS_VLINE, ym);
    mvwvline(win, 0, max_l-1, ACS_VLINE, ym);
    mvwaddch(win,0,0,ACS_ULCORNER);
    mvwaddch(win,0,max_l-1,ACS_URCORNER);
    mvwaddch(win,ym-1,0,ACS_LLCORNER);
    mvwaddch(win,ym-1,max_l-1,ACS_LRCORNER);
    mvwaddch(win,2,0,ACS_LTEE);
    mvwaddch(win,2,max_l,ACS_RTEE);

    print_centred(win, "Info Message", 1,max_l-4,max_l);

    print_centred(win, l, 4,max_l-4,max_l);

    init_pair(4, COLOR_BLACK, 7);
    wattron(win,COLOR_PAIR(4));
    print_centred(win, "< Ok >", ym-3,max_l-4,max_l);
    wattroff(win,COLOR_PAIR(4));
    PANEL* p=new_panel(win);
    update_panels();
	doupdate();
    int ch, cycle=0;
    while((cycle) != -1)
    {	
        ch = getch();
    	switch(ch)
	    {
            case KEY_ENTER:
            case '\n':
	        case ' ':
                cycle=-1;
			    break;
            
        }
    }
    del_panel(p);
    delwin(win);
}


void print_qw(WINDOW *win,char const* str1,char const* str2,int ch,int ys,int max_lw)
{
    int sz1=str_size(str1),sz2=str_size(str2);
    init_pair(4, COLOR_BLACK, 7);
    if (ch==0)
        wattron(win,COLOR_PAIR(4));
    mvwprintw(win,ys,max_lw*1/3-sz1/2,"%s",str1);
    if (ch==0)
        wattroff(win,COLOR_PAIR(4));
    if (ch==1)
        wattron(win,COLOR_PAIR(4));
    mvwprintw(win,ys,max_lw*2/3-sz2/2,"%s",str2);
    if (ch==1)
        {
            wattroff(win,COLOR_PAIR(4));
            wmove(win, ys,max_lw*2/3-sz2/2+sz2-1);
        }
    else
        wmove(win, ys,max_lw*1/3-sz1/2+sz1-1);
}
    

bool YNMessage(char const* str, int max_l)
{
    int row,col;
    lines l=get_l(str, max_l-4);
    int ym=l.n+8;//(sz+max_l-5)/(max_l-4)+8;
    getmaxyx(stdscr,row,col);
    WINDOW *win=newwin(ym, max_l, (row-ym)/2, (col-max_l)/2);
    mvwhline(win, 0, 0, ACS_HLINE, max_l);
    mvwhline(win, 2, 0, ACS_HLINE, max_l);
    mvwhline(win, ym-1, 0, ACS_HLINE, max_l);
    mvwvline(win, 0, 0, ACS_VLINE, ym);
    mvwvline(win, 0, max_l-1, ACS_VLINE, ym);
    mvwaddch(win,0,0,ACS_ULCORNER);
    mvwaddch(win,0,max_l-1,ACS_URCORNER);
    mvwaddch(win,ym-1,0,ACS_LLCORNER);
    mvwaddch(win,ym-1,max_l-1,ACS_LRCORNER);
    mvwaddch(win,2,0,ACS_LTEE);
    mvwaddch(win,2,max_l,ACS_RTEE);

    print_centred(win, "Question", 1,max_l-4,max_l);

    print_centred(win, l, 4,max_l-4,max_l);

    print_qw(win, "< Yes >", "< No >", 0, ym-3, max_l);
    PANEL* p=new_panel(win);
    update_panels();
	doupdate();
    int ch, cycle=0, chs=0;
    while((cycle) != -1)
    {	
        ch = getch();
    	switch(ch)
	    {
            case KEY_LEFT:
                chs=max(chs-1,0);
                print_qw(win, "< Yes >", "< No >", chs, ym-3, max_l);
                update_panels();
	            doupdate();
                break;
            case KEY_RIGHT:
                chs=min(chs+1,1);
                print_qw(win, "< Yes >", "< No >", chs, ym-3, max_l);
                update_panels();
	            doupdate();
                break;
            case KEY_ENTER:
            case '\n':
	        case ' ':
                cycle=-1;
			    break;
            
        }
    }
    del_panel(p);
    delwin(win);
    return 1-chs;
}