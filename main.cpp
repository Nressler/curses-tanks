#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <cmath>

#if defined(_WIN32)
#include <Windows.h>
#include "curses.h"
#else
#include <curses.h>
#include <cstdlib>
#include <unistd.h>
#endif

#include "ground.hpp"
#include "player.hpp"

using namespace std;

extern int base_height_divisor;
extern int max_height_divisor;

const double PI = 3.141592653589793238463;

void MySleep(int milliseconds)
{
#if defined(_WIN32)
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif
}
void DrawScreen(Ground & g, Player * players, int turn)
{
	erase();
	box(stdscr, 0, 0);
	g.Draw();
	players[0].Draw(g);
	players[1].Draw(g);
	players[0].DrawSettings(turn);
	players[1].DrawSettings(turn);
	refresh();
}

//http://www.iforce2d.net/b2dtut/projected-trajectory

void Shoot(Ground & g, Player * players, int turn)
{
	bool hit = false;
	if (hit == true)
	{
		mvaddstr(LINES / 2, COLS / 2, "Hit");
	}
	double angle = players[turn].angle / 180.0 * PI;
	//double power = players[turn].power / 5 * 10; Need to figure out "rules" for set values of increasing/decreasing power
	//Try modifying the speed/time lapse of the shot
	double y_component = sin(angle) * players[turn].power * 0.2;
	double x_component = cos(angle) * players[turn].power * 0.2;
	
	double pNx;
	double pNy;
	double time_divisor = 15.0;
	
	if (players[turn].s == RIGHT)
		x_component = -x_component;

	double p0x = players[turn].col;
	double p0y = g.ground.at(players[turn].col);
	// higher ground numbers are lower altitudes (0 is first line, etc).
	p0y = LINES - p0y;
	for (int i = 1; i < 5000; i++)
	{
		double di = i / time_divisor;

		pNx = (int)(p0x + di * x_component);
		pNy = p0y + di * y_component + (di * di + di) * -9.8 / time_divisor / 1.5;
		pNy = (int)(LINES - pNy);

		//The idea is to test if the area is good or not
		//All the statements will define the surrounding good areas
		//Leaving the inner hit zone for the player
		if (pNy > g.ground.at((int)pNy) - 1 || pNy < g.ground.at((int)pNy) + 1)
		{
			hit = false;
			
		}
		else if (pNx < g.ground.at((int)pNx) + 1 || pNx > g.ground.at((int)pNx) - 1)
		{
			hit = false;
		}
		else
		{
			hit = true;
			mvaddstr(LINES / 2, COLS / 2, "Hit");
		}
			
		//Testing x and y boundries, terminates when it hits the border
		if (pNx < 1 || pNx >= COLS - 2)
			break;
		if (pNy < 1) 
		{
			MySleep(50);
			continue;
		}
		
	//	if (pNy >= LINES - 2)
	//		break;
		if (pNy > g.ground.at((int)pNx))
			break;

		move((int)pNy - 1, (int)pNx + 1);
		addch('*');
		refresh();
		MySleep(50);
	}
	
	class Vec2D
	{
		int px, py;
		Vec2D();
	};
}

int main(int argc, char * argv[])
{
	srand((unsigned int)time(nullptr));

	int turn = 0;
	bool keep_going = true;
	Ground g;
	Player players[2];

	initscr();
	noecho();
	keypad(stdscr, 1);

	g.InitializeGround();
	players[0].Initialize(rand() % (COLS / 4), LEFT);
	players[1].Initialize(rand() % (COLS / 4) + 3 * COLS / 4 - 2, RIGHT);
	
	DrawScreen(g, players, turn);
	
	while (keep_going)
	{
		bool show_char = false;
		int c = getch();
		switch (c)
		{
		case 27:
			keep_going = false;
			break;

		case 'm': //Not connected right
			players[turn].PowerDown();
			break;

		case 'n': //Not connected right
			players[turn].PowerUp();
			break;

		case 'u':
			players[turn].AngleUp();
			break;

		case 'd':
			players[turn].AngleDown();
			break;

		case 10:
		case KEY_ENTER:
#if defined(WIN32)
		case PADENTER:
#endif
			Shoot(g, players, turn);
			turn = 1 - turn;
			break;

		default:
			show_char = true;
			break;
		}
		//Keypad Trouble Shooting//
		DrawScreen(g, players, turn);
		if (show_char) {
			move(0, 1);
			stringstream ss;
			ss << setw(4) << c << " ";
			addstr(ss.str().c_str());
			refresh();
		}
	}
	erase();
	addstr("Hit any key to exit");
	refresh();
	getch();
	echo();
	endwin();
	return 0;
}
