#include "caro.h"
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <Windows.h>
#include <string>
#include <locale>
#include <conio.h>
#include <stdlib.h>
#include <fstream>
using namespace std;



//Hằng số 
#define BOARD_SIZE 12		// Kích thức ma trận bàn cờ 
#define LEFT 3				// Tọa độ trái màn hình bàn cờ
#define TOP 1				// Tọa độ trên màn hình bàn cờ 

// Khai báo kiểu dữ liệu 
//struct _POINT { int x, y, c; };				//		x: tọa độ dòng,     y: tọa độ cột,     c: đánh dấu
_POINT _A[BOARD_SIZE][BOARD_SIZE];			//		Ma trận bàn cờ
bool _TURN;									//		TRUE: người 1,		FALSE: người 2
int _COMMAND;								//		Biến nhận giá trị phím người nhập
int _X, _Y;									//		Tọa độ hiện hành trên màn hình bàn cờ

int FLAT[BOARD_SIZE][BOARD_SIZE] = { 0 };			//		kt ô chứa "X" hay "O"
int check_flat;										//		kt luot danh hien tai la "X" hay "O"
int count_full = 0;
int cl;												//		chon mau color

int count_x = 0, count_y = 0;




//FUNCTION

void FixConsoleWindow()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}


//buoc 2:
// Hàm nhóm View 1 
void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


//HÀM NHÓM Model 1
//hàm khởi tạo dữ liệu mặc định ban đầu cho ma trận bàn cờ    (hàm nhóm Model 1)
void ResetData()
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			_A[i][j].x = 4 * j + LEFT + 2;				//		trùng với hoành độ màn hình bàn cờ
			_A[i][j].y = 2 * i + TOP + 1;				//		trùng với tung độ màn hình bàn cờ
			_A[i][j].c = 0;								//		0: chưa ai đánh dấu
														//		Nếu đánh phải theo quy định:   -1 (true đánh), 1 (false đánh)
		}
	}
	_TURN = true;	_COMMAND = -1;						//		Gán lượt và phím mặc định
	_X = _A[0][0].x;									//		thiết lập lại tọa độ hiện hành ban đầu
	_Y = _A[0][0].y;
}

//HÀM NHÓM VIEW
void DrawBoard(int pSize)
{
	for (int i = 0; i <= pSize; i++)
	{
		for (int j = 0; j <= pSize; j++)
		{
			GotoXY(LEFT + 4 * i, TOP + 2 * j);
			cout << ".";
		}
	}
	GotoXY(_A[0][0].x, _A[0][0].y);
}



// Hàm nhóm Control 
void MoveRight()
{
	if (_X < _A[BOARD_SIZE - 1][BOARD_SIZE - 1].x)
	{
		_X += 4;
		GotoXY(_X, _Y);
	}
}

void MoveLeft()
{
	if (_X > _A[0][0].x)
	{
		_X -= 4;
		GotoXY(_X, _Y);
	}
}

void MoveDown()
{
	if (_Y < _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y)
	{
		_Y += 2;
		GotoXY(_X, _Y);
	}
}

void MoveUp()
{
	if (_Y > _A[0][0].y)
	{
		_Y -= 2;
		GotoXY(_X, _Y);
	}
}

																			//////////////MOVE
void move()
{
	bool validEnter = true;
	while (1)
	{
		_COMMAND = toupper(_getch());
		if (_COMMAND == 27)
		{
			Exit();
			
		}
		else
		{
			if (_COMMAND == 65)
			{
				MoveLeft();
				Beep(500, 100);
			}
			else if (_COMMAND == 87)
			{
				MoveUp();
				Beep(500, 100);
			}
			else if (_COMMAND == 83) 
			{
				MoveDown();
				Beep(500, 100);
			}
			else if (_COMMAND == 68) 
			{
				MoveRight();
				Beep(500, 100);
			}
			else if (_COMMAND == 13)
			{												// Người dùng đánh dấu trên màn hình bàn cờ 
				switch (CheckBoard(_X, _Y))
				{
				case -1:
					FLAT[_X][_Y] = -1;
					check_flat = -1;
					count_full++;
					count_x++;
					Beep(600, 100);
					color(9);
					cout << "X";
					
					color(12);
					mini_O();
					countX();
					break;
				case 1:
					FLAT[_X][_Y] = 1;
					check_flat = 1;
					count_full++;
					count_y++;
					Beep(600, 100);
					color(12);
					cout << "O";
					
					color(9);
					mini_X();
					countY();
					break;
				case 0: validEnter = false;				// Khi đánh vào ô đã đánh rồi
				}
				if (validEnter == true)
				{
					switch (ProcessFinish(TestBoard()))							//tạm
					{
					case -1: case 1: case 0:
						if (AskContinue() == 78)
						{
							Exit();
						}
						else if (AskContinue() == 89) StartGame();
					}
				}
				validEnter = true; // Mở khóa  37   
			}
			else if (_COMMAND == 76) {
				GotoXY(70, 11);
				string n,m;
				//STATE tmp;
				int a[12][12];
				file_name(n);
				save_namefile(n);

				for (int i = 0; i < BOARD_SIZE; i++)
					for (int j = 0; j < BOARD_SIZE; j++)
					{		
						a[i][j] = _A[i][j].c;
					}				
				save(a, n,m);
			}
			else if (_COMMAND == 84) loadgame();
			
		}
	}
}

int AskContinue()
{
	GotoXY(58, 17);
	printf("Nhan 'y/n' de tiep tuc/dung: ");
	return toupper(_getch());
}

void GabageCollect()
{
	//dọn dẹp tài nguyên nếu có khai báo con trỏ
}

int Exit()
{
	system("cls");
	GabageCollect();
	system("pause");
	exit(0);
	//có thể lưu game trước khi exit
}
//buoc 10
// Hàm nhóm Model
int CheckBoard(int pX, int pY)
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (_A[i][j].x == pX && _A[i][j].y == pY && _A[i][j].c == 0)
			{
				if (_TURN == true) _A[i][j].c = -1; // Nếu lượt hiện hành là true thì c = -1 
				else _A[i][j].c = 1; // Nếu lượt hiện hành là false thì c = 1 
				return _A[i][j].c;
			}			
		}
	}
	return 0;
}

//HÀM NHÓM VIEW
//hàm xử lý khi người chơi thua
int ProcessFinish(int pWhoWin)
{
	GotoXY(0, _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y + 2);		// Nhảy tới vị trí thích hợp để in chuỗi thắng/thua/hòa 
	switch (pWhoWin)
	{
	case -1:
		//cout << "X thang";
		winX();	
		break;
	case 1:
		//cout << "O thang";
		winO();
		break;
	case 0:
		//cout << "2 thang hoa nhau";
		draw();
		break;
	case 2:
		_TURN = !_TURN; // Đổi lượt nếu không có gì xảy ra
	}
	GotoXY(_X, _Y); // Trả về vị trí hiện hành của con trỏ màn hình bàn cờ 
	return pWhoWin;
}


int TestBoard()
{
	if (check_full() == true)	return 0;
	else
	{
		if ((check_row()) || (check_col() == true) || (check_diagonal_down() == true) || (check_diagonal_up() == true))
			return (_TURN == true ? -1 : 1);
		else return 2;
	}
}




//kt thắng thua

bool check_row()											//		kiem tra hang doc
{	
	int countRow = 1;	
	countRow = countRow +Count_left() + Count_right();
	/*cout << Count_left();
	cout << Count_right();
	cout << countRow;*/
	if (countRow >= 5) return true;
	else return false;
}						

bool check_col()											//			kiem tra hang ngang
{
	int countCol = 1;
	countCol = countCol + Count_up() + Count_down();
	/*cout << Count_left();
	cout << Count_right();
	cout << countRow;*/
	if (countCol >= 5) return true;
	else return false;
}

bool check_diagonal_down()								//			kiem tra duong cheo xuong
{
	int count_dia_down = 1;
	count_dia_down = count_dia_down + count_down_left() + count_down_right();
	/*cout << Count_left();
	cout << Count_right();

	cout << countRow;*/
	if (count_dia_down >= 5) return true;
	else return false;
}

bool check_diagonal_up()							//			kiem tra duong cheo len
{
	int count_dia_up = 1;
	count_dia_up = count_dia_up + count_up_left() + count_up_right();
	/*cout << Count_left();
	cout << Count_right();

	cout << countRow;*/
	if (count_dia_up >= 5) return true;
	else return false;
}



int count_up_left()
{
	int count_ul = 0;
	int tempX = _X - 4;
	int tempY = _Y + 2;
	while (tempX - 4 >= 0 && tempY + 2 <= TOP + 2*BOARD_SIZE && _A[(tempY - TOP - 1)/2][(tempX - LEFT - 2)/4].c == check_flat)
	{															//flat[temx][tempy]
		count_ul++;
		tempX = tempX - 4;
		tempY = tempY + 2;
	}
	return count_ul;
}

int count_up_right()
{
	int count_ur = 0;
	int tempX = _X + 4;
	int tempY = _Y - 2;
	while (tempX + 4 <= LEFT + 4 * BOARD_SIZE  && tempY + 2 >= 0 && _A[(tempY - TOP - 1) / 2][(tempX - LEFT - 2) / 4].c == check_flat)
	{
		count_ur++;
		tempX = tempX + 4;
		tempY = tempY - 2;
	}
	return count_ur;
}

int count_down_left()									//			kiem tra duong cheo xuong - trai
{
	int count_dl = 0;
	int tempX = _X - 4;
	int tempY = _Y - 2;
	while (tempX -4 >=0 && tempY - 2 >= 0 && _A[(tempY - TOP - 1) / 2][(tempX - LEFT - 2) / 4].c == check_flat)
	{
		count_dl++;
		tempX = tempX - 4;
		tempY = tempY - 2;
	}
	return count_dl;
}

int count_down_right()									//			kiem tra duong cheo xuong - trai
{
	int count_dr = 0;
	int tempX = _X + 4;
	int tempY = _Y + 2;
	while (tempX + 4 <= LEFT + 4 * BOARD_SIZE && tempY + 2 <= TOP + 2 * BOARD_SIZE && _A[(tempY - TOP - 1) / 2][(tempX - LEFT - 2) / 4].c == check_flat)
	{
		count_dr++;
		tempX = tempX + 4;
		tempY = tempY + 2;
	}
	return count_dr;
}

int Count_up()
{
	int count_u = 0;
	int tempY = _Y - 2;
	while (tempY - 2 >= 0 && _A[(tempY - TOP - 1)/2][(_X - LEFT - 2)/4].c == check_flat)
	{
		count_u++;
		tempY = tempY - 2;
	}
	return count_u;
}

int Count_down()
{
	int count_d = 0;
	int tempY = _Y + 2;
	while (tempY + 2 <= TOP + 2*BOARD_SIZE && _A[(tempY - TOP - 1)/2][(_X - LEFT - 2)/4].c == check_flat)
	{
		count_d++;
		tempY = tempY + 2;
	}
	return count_d;
}

int Count_left()
{
	int count_l = 0;
	int tempX = _X - 4;
	while (tempX - 4 >= 0 && _A[(_Y - TOP - 1)/2][(tempX - LEFT -2)/4].c == check_flat)
	{
		count_l++;
		tempX = tempX - 4;
	}
	return count_l;
}

int Count_right()
{
	int count_r = 0;
	int tempX = _X + 4;
	while (tempX + 4 <= LEFT + 4*BOARD_SIZE  && _A[(_Y - TOP - 1)/2][(tempX - LEFT -2)/4].c == check_flat)
	{
		count_r++;
		tempX = tempX + 4;
	}
	return count_r;
}



bool check_full()
{
	if (count_full == BOARD_SIZE * BOARD_SIZE)	return true;
	else return false;
}


void color(int cl)
{
	HANDLE hConsoleColor;
	hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleColor, cl);		
}


void screen()													//		man hinh hien thi
{
	int y, x;
	int Xo = LEFT + 4 * (BOARD_SIZE + 1);		
	int X_lim = Xo + 5 * (BOARD_SIZE + 1);
	int Y_lim = TOP + 2 * (BOARD_SIZE + 2) + 1;
	int Yo = 18;
	char kt1 = 176; 
	char kt2 = 240;														
	for (y = 0; y < Y_lim; y++)									//		bien doc
	{
		color(10);
		GotoXY(Xo, y);
		cout << kt1;
	}
	
	for (x = Xo+1; x < X_lim; x++)								//		bien giua
	{
		color(10);
		GotoXY(x, Yo);
		cout << kt2;
	}

	for (y = 0; y<=26; y++)
	{
		color(6);
		GotoXY(Xo-1, y);
		cout << kt1;
	}
	for (x = 0; x<Xo; x++)
	{
		color(6);
		GotoXY(x, 26);
		cout << kt2;
	}

	{
		GotoXY(4, 27);
		color(9);
		cout << "P L A Y E R 1:  X";
	}

	{
		GotoXY(35, 27);
		color(12);
		cout << "P L A Y E R 2:  O";
	}

	GotoXY(57, 19);
	color(11);
	cout << "|[====================== INSTRUCTIONS ======================]|";
	GotoXY(57, 29);
	cout << "|[==========================================================]|";
	for (int y = 20; y < 29; y++)
	{
		GotoXY(57, y);
		cout << "|[";
	}
	for (int y = 20; y < 29; y++)
	{
		GotoXY(117, y);
		cout << "]|";
	}
	{
		GotoXY(78, 22);
		cout << "Press L to save game";
		GotoXY(78, 24);
		cout << "Press T to load game";
		GotoXY(77, 26);
		cout << "Press ESC to exit game";
	}
	
	{																				//		CARO
		color(12);
		GotoXY(73, 1);		cout << "  CCC     AA    RRRRr     OOO";
		GotoXY(73, 2);		cout << " CC      A--A   R rrR    O   O";
		GotoXY(73, 3);		cout << "  CCCC  A    A  RR   RR   OOO";
	}
	{
		color(9);
		GotoXY(77, 12);		cout << "Nguyen Tran Thien Toan";
		GotoXY(77, 13);		cout << "   Nguyen Huu Tuan";
		GotoXY(77, 14);		cout << "   Le Tran Gia Bao";  
		GotoXY(77, 15);		cout << "   Phan Viet THang";
	}
	ruler();
	color(7);
}

void countX()
{	
	GotoXY(12, 28);
	color(11);
	cout << count_x;
}

void countY()
{
	GotoXY(43, 28);
	color(13);
	cout << count_y;
}


void winX()
{
	GotoXY(72, 7);		cout << "           XX       XX                ";
	GotoXY(72, 8);		cout << "             XX   XX	               ";
	GotoXY(72, 9);		cout << "               XXX                    ";
	GotoXY(72, 10);		cout << "             XX   XX	               ";
	GotoXY(72, 11);		cout << "           XX       XX                ";
	GotoXY(72, 12);		cout << "                                      ";
	GotoXY(70, 13);		cout << "XX      XX      XX   XX    XXX   XX   ";
	GotoXY(70, 14);		cout << "  XX  XX  XX  XX     XX    XX XX XX   ";
	GotoXY(70, 15);		cout << "    XX      XX       XX    XX   XXX   ";
	Beep(1200, 500);
	Beep(1500, 500);
	Beep(2000, 500);
}

void winO()
{
	GotoXY(72, 7);		cout << "               OOO                    ";
	GotoXY(72, 8);		cout << "             OO   OO                  ";
	GotoXY(72, 9);		cout << "            OO     OO                 ";
	GotoXY(72, 10);		cout << "             OO   OO                  ";
	GotoXY(72, 11);		cout << "               OOO                    ";
	GotoXY(72, 12);		cout << "                                      ";
	GotoXY(70, 13);		cout << "OO      OO      OO   OO    OOO   OO   ";
	GotoXY(70, 14);		cout << "  OO  OO  OO  OO     OO    OO OO OO   ";
	GotoXY(70, 15);		cout << "    OO      OO       OO    OO   OOO   ";
	Beep(900, 500);
	Beep(1200, 500);
	Beep(1500, 500);
	Beep(2000, 500);
}						

void draw()
{
	GotoXY(63, 7);		cout << "DDDD     RRRRRR         AA    WW    WW   WW    WW";
	GotoXY(63, 8);		cout << "D   D    RR   RR       AAAA    WW    WW WW    WW";
	GotoXY(63, 9);		cout << "D    D   RRRRRR       AA  AA    WW    WWW    WW";
	GotoXY(63, 10);		cout << "D   D    RR   RR     AA AA AA    WW  WW WW  WW";
	GotoXY(63, 11);		cout << "DDDD     RR     RR  AA      AA    WWWW   WWWW";
}

void ruler()
{
	GotoXY(72, 7);		
	color(14);  cout << "YOU WIN       :   ";
	color(3);	cout << "X ";
	color(5);	cout << "O O O O O";
	GotoXY(72, 9);
	color(14);  cout << "YOU still WIN :   ";
	color(3);	cout << "X ";
	color(5);	cout << "O O O O O";
	color(3);	cout << " X (^^)";

}


void menu()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	cout << "\n\n\n";
	cout << "\t\t\t\t  _    _  _  _  __     _  _  __    _" << endl;
	cout << "\t\t\t\t // \\\\  // \\\\ ||\\\\//|| ||       //   // \\\\ || \\\\  // \\\\" << endl;
	cout << "\t\t\t\t((  _ ||=|| || \\/ || ||==    ((    ||=|| ||_// ((   ))" << endl;
	cout << "\t\t\t\t \\\\_ || || || || || || ||___    \\\\__ || || || \\\\  \\\\_//    " << endl;
	
	
	color(9);
	GotoXY(50, 20);		cout << "   NHOM 6 - 19CLC8";
	GotoXY(50, 22);		cout << "Nguyen Tran Thien Toan";
	GotoXY(50, 24);		cout << "   Nguyen Huu Tuan";
	GotoXY(50, 26);		cout << "   Le Tran Gia Bao";
	GotoXY(50, 28);		cout << "   Phan Viet Thang";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	GotoXY(52, 10);
	cout << ">> 1. NEW GAME";
	GotoXY(52, 11);
	cout << ">> 2. LOAD GAME";
	GotoXY(52, 12);
	cout << ">> 3. QUIT ";
	GotoXY(50, 15);
	cout << ">> ENTER YOUR CHOICE: ";
	choice();
}

void choice()
{
	int enter;
	string n,m;
	while (true)
	{
		string ten[255];
		cin >> enter;
		if (enter == 1) StartGame();
		if (enter == 2)	loadgame();
		if (enter == 3) Exit();
	}	
}

void StartGame()
{
	system("cls");
	screen();
	reset();
	FixConsoleWindow();
	DrawBoard(BOARD_SIZE);
	bool validEnter = true;
	move();
}

void reset()
{
	ResetData();
	count_full = 0;
	check_flat = -2;
	count_x = 0, count_y = 0;
}

void mini_X()
{
	GotoXY(25, 27);	cout << "  X X  ";
	GotoXY(25, 28);	cout << "   X   ";
	GotoXY(25, 29);	cout << "  X X  ";
}

void mini_O()
{
	GotoXY(25, 27);	cout << "   OO  ";
	GotoXY(25, 28);	cout << "  O  O ";
	GotoXY(25, 29);	cout << "   OO  ";
}



//-----------------------------------------------------------------------------

// Ham nhom save file

// Hàm nhập của file mà người chơi nhập từ bàn phím
void file_name(string& name_caro)
{
	cout << "Nhap ten file can luu: ";
	cin >> name_caro;
	

	if (AskContinue() == 78)
	{
		Exit();
	}
	else if (AskContinue() == 89) StartGame();
	
}
// Hàm lưu tên của các file mà người chơi nhập
void save_namefile(string name_caro)
{
	ofstream namefile("namefile.txt", ios::app);
	namefile << name_caro << endl;
	namefile.close();
}

// Hàm đọc tên ra thành list để người chơi chọn
void read_namefile(string &ten_game, string name_caro) 
{
	int x = 2;
	ifstream namefile("namefile.txt");
	while (!namefile.eof()) {
		char line[400];
		namefile.getline(line, 400);
		GotoXY(40, x);
		cout << line << endl;
		x = x + 1;
	}
	namefile.close();
	GotoXY(40, x + 1);
	cout << "Nhap ten tro choi muon load: ";
	cin >> ten_game;
}

// Hàm lưu dữ liệu của bàn cờ vào file với tên mà người chơi nhập
void save(int a[][12], string folderpath = "", string name_caro = "")
{
	ofstream f;
	f.open(folderpath + name_caro + ".txt", ios::out);
	for (int i = 0; i < BOARD_SIZE; i++) 
	{
		for (int j = 0; j < BOARD_SIZE; j++)   f << a[i][j] << " ";
		f << endl;
	}
	f << _TURN << endl;
	f << count_full << endl;
	f << check_flat << endl;
	f << count_x << endl;
	f << count_y << endl;

	f.close();
}

// Hàm đọc dữ liệu có trong file mà người chơi nhập
void readdata(int a[12][12], string folderpath = "", string ten_name = "")
{
	ifstream f;
	f.open(folderpath + ten_name + ".txt");
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)  f >> a[i][j];
	}
	f >> _TURN;
	f >> count_full;
	f >> check_flat;
	f >> count_x;
	f >> count_y;
	f.close();
}

void loadgame() {
	system("cls");
	string Ten_game;
	string Name_caro;
	GotoXY(40, 1);
	cout << "Danh sach tro choi: " << endl;
	GotoXY(40, 2);
	read_namefile(Ten_game, Name_caro);
	GotoXY(40, 3);


	//STATE x;
	//int b[12][12];
	string link = "";
	reset();
	readdata(FLAT, link, Ten_game + "");
	system("cls");
	//cout << x.check << " " << x.count << " " << x.turn << endl;
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++) 	_A[i][j].c = FLAT[i][j];
		
	}

	system("cls");
	screen();
	bool validEnter = true;
	FixConsoleWindow();
	DrawBoard(BOARD_SIZE);
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			//GotoXY(LEFT + 4 * i, TOP + 2 * j)
			if (_A[i][j].c == -1)
			{
				GotoXY(4 * j + LEFT + 2, 2 * i + TOP + 1);		
				color(9);	 cout << "X";
				GotoXY(4 * j + LEFT + 2, 2 * i + TOP + 1);
			}
			else if (_A[i][j].c == 1)
			{
				GotoXY(4 * j + LEFT + 2, 2 * i + TOP + 1);		
				color(12);		cout << "O";
				GotoXY(4 * j + LEFT + 2, 2 * i + TOP + 1);
			}
		}
	}
	GotoXY(LEFT + 2, TOP + 1);
	move();
}



