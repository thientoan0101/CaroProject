#ifndef _caro_h_
#define _caro_h_
#include <iostream>
#include <fstream>
using namespace std;


//ham nhom VIEW:
void menu();
void choice();
void StartGame();
void FixConsoleWindow();
void GotoXY(int, int);
void DrawBoard(int);
int ProcessFinish(int);


//ham nnhom Model
void ResetData();
void reset();
int CheckBoard(int, int);
int AskContinue();
int TestBoard();
int Exit();
void GabageCollect();


//ham nhom control
void MoveRight();
void MoveLeft();
void MoveDown();
void MoveUp();
void move();

//ham test
bool check_full();

bool check_row();
int Count_left();
int Count_right();

bool check_col();
int Count_up();
int Count_down();

bool check_diagonal_down();
int count_down_left();
int count_down_right();

bool check_diagonal_up();
int count_up_right();
int count_up_left();


//ham color
void color(int);


void screen();
void countX();
void countY();

void mini_X();
void mini_O();


void winX();
void winO();
void draw();
void ruler();



/////
void file_name(string&);
void loadgame();
void save_namefile(string);
void save(int [][12], string, string);
void read_namefile(string&, string);
void readdata(int [][12], string, string);

struct _POINT { int x, y, c; };				//		x: tọa độ dòng,     y: tọa độ cột,     c: đánh dấu
#endif // !_caro_h_
