#include <panel.h>
#include "game_eg.hpp"
#include "board.hpp"
#include "message.hpp"
#include <chrono>
#include <cstring>

void find_avalible_moves(Board *b, int *avalible_mv, int player)
{
    char fsh[2] = {'f', 's'};
    for (int x = 0; x < b->n; x++)
        for (int y = 0; y < b->n; y++)
        {
            avalible_mv[x + b->n * y] = 0;
            if (b->states[x][y].name == 'f' || b->states[x][y].name == 's')
                // av[x+b->n*y]=5;
                continue;
            for (int dx = -1; dx < 2; dx++)
                for (int dy = -1; dy < 2; dy++)
                {
                    if (dy == 0 && dx == 0)
                        continue;
                    if (avalible_mv[x + y * b->n] == 1)
                        break;
                    for (int l = 1; l * dx + x < b->n && l * dx + x >= 0 && l * dy + y < b->n && l * dy + y >= 0; l++)
                    {
                        if (b->states[l * dx + x][l * dy + y].name == fsh[(player + 1) % 2] && avalible_mv[x + y * b->n] != 1)
                        {
                            avalible_mv[x + y * b->n] = 2;
                            continue;
                        }
                        if (b->states[l * dx + x][l * dy + y].name == fsh[player] && avalible_mv[x + y * b->n] == 2)
                        {
                            avalible_mv[x + y * b->n] = 1;
                            break;
                        }
                        avalible_mv[x + y * b->n] = 0;
                        break;
                    }
                }
        }
}

void izm(int x, int y, state st, game_state *game)
{
    Board *b=game->b;
    state new_sts[20][20];
    for (int i=0;i<b->n;i++)
        for (int j=0;j<b->n;j++)
            new_sts[i][j]=b->states[i][j];
    char fsh[2] = {'f', 's'};
    for (int dx = -1; dx < 2; dx++)
        for (int dy = -1; dy < 2; dy++)
        {
            if (dy == 0 && dx == 0)
                continue;
            int l1=0;
            for (int l = 1; l * dx + x < b->n && l * dx + x >= 0 && l * dy + y < b->n && l * dy + y >= 0; l++)
            {
                if (b->states[l * dx + x][l * dy + y].name == fsh[(game->player + 1) % 2])
                {
                    l1++;
                    continue;
                }
                if (b->states[l * dx + x][l * dy + y].name == fsh[game->player])
                    {
                        for (int i=l1;i>=0;i--)
                            new_sts[(l-i) * dx + x][(l-i) * dy + y]=st;
                        break;
                    }
                break;
            }
        }
    for (int i=0;i<b->n;i++)
        for (int j=0;j<b->n;j++)
            b->states[i][j]=new_sts[i][j];
}

void changeP(game_state *game, int *avalible_mv)
{
    game->player = (game->player + 1) % 2;
    find_avalible_moves(game->b, avalible_mv, game->player);
    int sum = 0;
    for (int x = 0; x < game->b->n; x++)
        for (int y = 0; y < game->b->n; y++)
        {
            sum += avalible_mv[x + y * game->b->n];
        }
    if (sum == 0)
    {
        char *c = new char[29];
        std::memcpy(c, "There no moves for Player _.", 29);
        c[26] = '0' + game->player;
        InfoMessage(c);
        changeP(game, avalible_mv);
    }
    if (game->player==0)
        init_pair(5, COLOR_BLACK, game->chps.FirstP.color);
    else
        init_pair(5, COLOR_BLACK, game->chps.SecondP.color);
}

bool do_move(game_state *game, int *avalible_mv)
{
    //mvwprintw(stdscr, 1, 0, "%s %i", "space pressed player ", game->player);
    if (avalible_mv[game->cur.x + game->cur.y * game->b->n] == 0)
    {
        InfoMessage("You can't do this move!");
        return 0;
    }
    if (game->player == 0)
    {
        game->b->states[game->cur.x][game->cur.y]=game->chps.FirstP;
        izm(game->cur.x,game->cur.y,game->chps.FirstP,game);
    }
    else
    {
        game->b->states[game->cur.x][game->cur.y]=game->chps.SecondP;
        izm(game->cur.x,game->cur.y,game->chps.SecondP,game);
    }
    game->chps.Choice.bcolor=game->b->states[game->cur.x][game->cur.y].color;
    game->stch=game->b->states[game->cur.x][game->cur.y];
    game->scores[0]=0;
    game->scores[1]=0;
    for (int x = 0; x < game->b->n; x++)
        for (int y = 0; y < game->b->n; y++)
        {
            if (game->b->states[x][y].name=='f')
                game->scores[0]++;
            if (game->b->states[x][y].name=='s')
                game->scores[1]++;
        }
    return 1;
}

void game(game_state *game)
{
    int ch;
    auto s1 = std::chrono::high_resolution_clock::now();
    auto s2 = std::chrono::high_resolution_clock::now();
    auto s3 = std::chrono::high_resolution_clock::now();
    bool blink = 1;
    int *avalible_mv = new int[game->b->n * game->b->n];
    changeP(game,avalible_mv);
    changeP(game,avalible_mv);
    for (int x = 0; x < game->b->n; x++)
        for (int y = 0; y < game->b->n; y++)
        {
            if (game->b->states[x][y].name=='f')
                game->scores[0]++;
            if (game->b->states[x][y].name=='s')
                game->scores[1]++;
        }
    redraw_board(game->b);
    while ((ch = getch()) != KEY_F(1))
    {
        s1 = std::chrono::high_resolution_clock::now();
        if ((s1 - s2).count() / 1E9 > 0.4)
        {
            if (blink)
            {
                game->b->states[game->cur.x][game->cur.y] = game->chps.Choice;
            }
            else
            {
                state bl = game->chps.Choice;
                bl.pic = picture();
                game->b->states[game->cur.x][game->cur.y] = bl;
            }
            wattron(stdscr,COLOR_PAIR(5));
            char *c = new char[16];
            std::memcpy(c, "Player's _ turn", 16);
            c[9] = '0' + game->player;
            mvwprintw(stdscr, game->b->ys-1, game->b->xs+(game->b->n+2)*game->b->dx/2 -8, "%s", c); 
            
            wattroff(stdscr,COLOR_PAIR(5));

            mvwprintw(stdscr, game->b->ys-1, game->b->xs+game->b->dx/2 -1, "  ");
            mvwprintw(stdscr, game->b->ys-1, game->b->xs+game->b->dx*(game->b->n+1)+game->b->dx/2 -1, "  ");

            init_pair(6, COLOR_BLACK, game->chps.FirstP.color);
            init_pair(7, COLOR_BLACK, game->chps.SecondP.color);
            
            wattron(stdscr,COLOR_PAIR(6));
            mvwprintw(stdscr, game->b->ys-1, game->b->xs+game->b->dx/2 -1, "%i", game->scores[0]);
            wattroff(stdscr,COLOR_PAIR(6));

            wattron(stdscr,COLOR_PAIR(7));
            mvwprintw(stdscr, game->b->ys-1, game->b->xs+game->b->dx*(game->b->n+1)+game->b->dx/2 -1, "%i", game->scores[1]);
            wattroff(stdscr,COLOR_PAIR(7));

            mvwprintw(stdscr, game->b->ys, game->b->xs+70, "%s %i", "Moves player ", game->player);
            for (int x = 0; x < game->b->n; x++)
                for (int y = 0; y < game->b->n; y++)
                {
                    mvwprintw(stdscr, game->b->ys+y + 1, game->b->xs+x + 70, "%i", avalible_mv[x + y * game->b->n]);
                }
            fupdate(game->b);
            blink = !blink;
            s2 = s1;
        }
        if ((s1 - s3).count() / 1E9 > 0.005)
        {
            fupdate(game->b);
            s3 = s1;
        }
        switch (ch)
        {
        case KEY_UP:
        {
            game->move_cur(0, -1);
            break;
        }
        case KEY_DOWN:
        {
            game->move_cur(0, 1);
            break;
        }
        case KEY_LEFT:
        {
            game->move_cur(-1, 0);
            break;
        }
        case KEY_RIGHT:
        {
            game->move_cur(1, 0);
            break;
        }
        case 27:
        {
            return;
        }
        case ' ':
        {
            if (game->b->states[game->cur.x][game->cur.y].bcolor != 0)
                continue;
            if (do_move(game,avalible_mv))
            {
                if (game->scores[0]+game->scores[1]==game->b->n*game->b->n)
                {
                    game->player=1-(game->scores[0]>game->scores[1]);
                    char *c = new char[40];
                    std::memcpy(c, "Player _ won the game. Congretulations!", 40);
                    c[7] = '0' + game->player;
                    InfoMessage(c);
                    return;
                }
                changeP(game,avalible_mv);
            }
            break;
        }
        }
    }
    delete avalible_mv;
    return;
}