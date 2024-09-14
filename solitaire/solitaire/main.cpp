#include <graphics.h>
#include <conio.h>
#pragma comment(lib,"MSIMG32.LIB")

ExMessage mouse_msg;

//透明显示
inline void putimage_alpha(int x, int y, IMAGE* img) {
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

void menu() {
	cleardevice();
	IMAGE menu;
	loadimage(&menu, _T("image/background.png"));
	putimage(0, 0, &menu);
}

void help() {
	while (1) {
		IMAGE help;
		loadimage(&help, _T("image/help.png"));
		putimage(0, 0, &help);
		getmessage(&mouse_msg);
		if (mouse_msg.message == WM_LBUTTONDOWN && mouse_msg.x >= 466 && mouse_msg.x <= 811 && mouse_msg.y >= 545 && mouse_msg.y <= 661) break;
		FlushBatchDraw();
	}
}

struct other_standard{
	int board_1[7][7] = {
		{-1,-1,0,0,0,-1,-1},
		{-1,-1,0,0,0,-1,-1},
		{0,0,1,0,1,0,0},
		{0,1,1,1,1,1,0},
		{0,0,1,1,1,0,0},
		{-1,-1,0,1,0,-1,-1},
		{-1,-1,0,0,0,-1,-1}
	};
	int board_2[7][7] = {
						{-1,-1,0,0,0,-1,-1},
						{-1,-1,0,0,0,-1,-1},
						{0,0,0,1,1,1,0},
						{0,0,0,1,1,1,0},
						{0,0,0,1,0,0,0},
						{-1,-1,0,1,0,-1,-1},
						{-1,-1,0,0,0,-1,-1}
	};
	int board_3[7][7] = {
						{-1,-1,0,0,0,-1,-1},
						{-1,-1,0,0,0,-1,-1},
						{0,0,1,1,1,0,0},
						{0,1,1,1,1,1,0},
						{0,0,0,0,0,0,0},
						{-1,-1,0,0,0,-1,-1},
						{-1,-1,0,0,0,-1,-1}
	};
	int board_4[7][7] = {
						{-1,-1,0,0,0,-1,-1},
						{-1,-1,0,1,0,-1,-1},
						{0,0,0,1,0,0,0},
						{0,0,1,0,1,0,0},
						{0,0,0,1,0,0,0},
						{-1,-1,0,1,0,-1,-1},
						{-1,-1,0,0,0,-1,-1}
	};
} canju;

class standard {
private:
	int board[7][7] = {
		{-1,-1,1,1,1,-1,-1},
		{-1,-1,1,1,1,-1,-1},
		{1,1,1,1,1,1,1},
		{1,1,1,0,1,1,1},
		{1,1,1,1,1,1,1},
		{-1,-1,1,1,1,-1,-1},
		{-1,-1,1,1,1,-1,-1}
	};
	int memory[7][7][32] = {};
	int memory_count = 0;
	int remaining = 32;
	int X = -1, Y = -1, to_X = -1, to_Y = -1;
	bool is_end;
public:
	void show_board() {
		cleardevice();
		IMAGE standard;
		loadimage(&standard, _T("image/standard.png"));
		putimage(0, 0, &standard);
	}
	void show_pieces() {
		IMAGE piece;
		loadimage(&piece, _T("image/piece.png"));
		for (int i = 0; i < 7; ++i) {
			for (int j = 0; j < 7; ++j) {
				if (board[i][j] == 1) {
					putimage_alpha(288 + j * 103, 125 + i * 103, &piece);
				}
			}
		}
	}
	void show_selected() {
		if (X > -1 && Y > -1 && board[Y][X] == 1) {
			IMAGE piece_resized;
			loadimage(&piece_resized, _T("image/piece_resized.png"));
			putimage_alpha(288 + X * 103 - 13, 125 + Y * 103 - 13, &piece_resized);
			
			IMAGE piece_moveTo;
			loadimage(&piece_moveTo, _T("image/piece_moveTo.png"));
			if (Y - 2 >= 0 && board[Y - 1][X] == 1 && board[Y - 2][X] == 0) {
				putimage_alpha(288 + X * 103 - 13, 125 + (Y - 2) * 103 - 13, &piece_moveTo);
			}
			if (Y + 2 <= 6 && board[Y + 1][X] == 1 && board[Y + 2][X] == 0) {
				putimage_alpha(288 + X * 103 - 13, 125 + (Y + 2) * 103 - 13, &piece_moveTo);
			}
			if (X - 2 >= 0 && board[Y][X - 1] == 1 && board[Y][X - 2] == 0) {
				putimage_alpha(288 + (X - 2) * 103 - 13, 125 + Y * 103 - 13, &piece_moveTo);
			}
			if (X + 2 <= 6 && board[Y][X + 1] == 1 && board[Y][X + 2] == 0) {
				putimage_alpha(288 + (X + 2) * 103 - 13, 125 + Y * 103 - 13, &piece_moveTo);
			}
		}
	}
	bool can_move() {
		if (to_X > -1 && to_Y > -1 && board[to_Y][to_X] == 0) {
			if (X - 2 == to_X && Y == to_Y && board[Y][X - 1] == 1) {
				board[to_Y][to_X] = 0;
				board[Y][X] = 0;
				board[Y][X - 1] = 0;
				return true;
			}
			if (X + 2 == to_X && Y == to_Y && board[Y][X + 1] == 1) {
				board[to_Y][to_X] = 0;
				board[Y][X] = 0;
				board[Y][X + 1] = 0;
				return true;
			}
			if (Y - 2 == to_Y && X == to_X && board[Y - 1][X] == 1) {
				board[to_Y][to_X] = 0;
				board[Y][X] = 0;
				board[Y - 1][X] = 0;
				return true;
			}
			if (Y + 2 == to_Y && X == to_X && board[Y + 1][X] == 1) {
				board[to_Y][to_X] = 0;
				board[Y][X] = 0;
				board[Y + 1][X] = 0;
				return true;
			}
		}
		return false;
	}

	void move_animation(int x, int y, int to_x, int to_y, int step) {
		for (int n = 0; n < step; ++n) {
			show_board();
			show_pieces();

			float fading_size = 0.1 * n * (step - n) + 92 - (92 * n / step);
			IMAGE fading_piece;
			loadimage(&fading_piece, _T("image/piece.png"), fading_size, fading_size);
			putimage_alpha(x + (to_x - x) / 2 - (fading_size - 92) / 2, y + (to_y - y) / 2 - (fading_size - 92) / 2, &fading_piece);

			float flying_size = 0.4 * n * (step - n) + 92;
			IMAGE flying_piece;
			loadimage(&flying_piece, _T("image/piece.png"), flying_size, flying_size);
			putimage_alpha(x + n * (to_x - x) / step - (flying_size - 92) / 2, y + n * (to_y - y) / step - (flying_size - 92) / 2, &flying_piece);
			FlushBatchDraw();
		}
	}

	void save_memory() {
		for (int i = 0; i < 7; ++i) {
			for (int j = 0; j < 7; ++j) {
				memory[i][j][memory_count] = board[i][j];
			}
		}
	}

	void game() {
		save_memory();
		while (1) {
			show_board();
			show_pieces();

			getmessage(&mouse_msg);

			//通过隐藏按键进入残局
			if (mouse_msg.message == WM_KEYDOWN) {
				if (mouse_msg.vkcode == '1') {
					remaining = 11;
					for (int i = 0; i < 7; ++i) {
						for (int j = 0; j < 7; ++j) {
							board[i][j] = canju.board_1[i][j];
						}
					}
				}
				if (mouse_msg.vkcode == '2') {
					remaining = 8;
					for (int i = 0; i < 7; ++i) {
						for (int j = 0; j < 7; ++j) {
							board[i][j] = canju.board_2[i][j];
						}
					}
				}
				if (mouse_msg.vkcode == '3') {
					remaining = 8;
					for (int i = 0; i < 7; ++i) {
						for (int j = 0; j < 7; ++j) {
							board[i][j] = canju.board_3[i][j];
						}
					}
				}
				if (mouse_msg.vkcode == '4') {
					remaining = 6;
					for (int i = 0; i < 7; ++i) {
						for (int j = 0; j < 7; ++j) {
							board[i][j] = canju.board_4[i][j];
						}
					}
				}
			}

			if (mouse_msg.message == WM_LBUTTONDOWN) {
				//悔棋
				if (mouse_msg.x >= 1143 && mouse_msg.x <= 1280 && mouse_msg.y >= 25 && mouse_msg.y <= 156) {
					if (memory_count > 0) {
						memory_count--;
						for (int i = 0; i < 7; ++i) {
							for (int j = 0; j < 7; ++j) {
								board[i][j] = memory[i][j][memory_count];
							}
						}
					}
					X = Y = to_X = to_Y = -1; //不显示
				}
				//退出
				else if (mouse_msg.x >= 0 && mouse_msg.x <= 137 && mouse_msg.y >= 25 && mouse_msg.y <= 156) break;

				//棋盘内
				if (mouse_msg.x >= 288 && mouse_msg.x <= 288 + 7 * 103 && mouse_msg.y >= 125 && mouse_msg.y <= 125 + 7 * 103) {
					int state = board[(mouse_msg.y - 125) / 103][(mouse_msg.x - 288) / 103]; //此处状态
					//棋子
					if (state == 1) {
						X = (mouse_msg.x - 288) / 103;
						Y = (mouse_msg.y - 125) / 103;
					}
					//空位
					else if (state == 0) {
						to_X = (mouse_msg.x - 288) / 103;
						to_Y = (mouse_msg.y - 125) / 103;
					}
				}
				//帮助
				else if (mouse_msg.x >= 0 && mouse_msg.x <= 131 && mouse_msg.y >= 824 && mouse_msg.y <= 939) {
					help();
				}
			}

			show_selected(); //显示选中棋子&可走空位

			if (can_move()) {
				remaining--;
				move_animation(288 + X * 103, 125 + Y * 103, 288 + to_X * 103, 125 + to_Y * 103, 15); //移动动画
				board[to_Y][to_X] = 1;
				memory_count++;
				save_memory();
			}

			//是否通关
			is_end = true;
			for (int i = 0; i < 7; ++i) {
				for (int j = 0; j < 7; ++j) {
					if (board[i][j] == 1) {
						if ((i + 2 < 7 && board[i + 1][j] == 1 && board[i + 2][j] == 0)
							|| (i - 2 >= 0 && board[i - 1][j] == 1 && board[i - 2][j] == 0)
							|| (j + 2 < 7 && board[i][j + 1] == 1 && board[i][j + 2] == 0)
							|| (j - 2 >= 0 && board[i][j - 1] == 1 && board[i][j - 2] == 0))
							is_end = false;
					}
				}
			}

			if (is_end) {
				if (remaining == 1) {
					IMAGE solved;
					loadimage(&solved, _T("image/solved.png"));
					putimage_alpha(0, 0, &solved);

				}
				else if (remaining != 1) {
					IMAGE end;
					loadimage(&end, _T("image/end.png"));
					putimage_alpha(0, 0, &end);

					settextstyle(16, 0, _T("Consolas")); 
					LOGFONT f;
					gettextstyle(&f);			
					f.lfHeight = 36;					
					_tcscpy_s(f.lfFaceName, _T("华文行楷"));	
					f.lfQuality = ANTIALIASED_QUALITY;		
					settextstyle(&f);					
					settextcolor(WHITE);
					TCHAR str[100] = {};
					_stprintf_s(str, _T("%d"), remaining);
					outtextxy(792, 461, str);
				}
				if (mouse_msg.message == WM_LBUTTONDOWN) {
					if (mouse_msg.x >= 562 && mouse_msg.x <= 714 && mouse_msg.y >= 525 && mouse_msg.y <= 641) {
						break;
					}
				}
			}

			FlushBatchDraw();
		}
	}
};

int main() {
	initgraph(1280, 960);
	BeginBatchDraw();
	//菜单循环
	while (1) {
		menu();
		getmessage(&mouse_msg);
		
		//点击
		if (mouse_msg.message == WM_LBUTTONDOWN) {
			//开始游戏
			if (mouse_msg.x >= 466 && mouse_msg.x <= 811 && mouse_msg.y >= 824 && mouse_msg.y <= 939) {
				standard Game;
				Game.game();
			}
			//帮助
			else if (mouse_msg.x >= 0 && mouse_msg.x <= 137 && mouse_msg.y >= 824 && mouse_msg.y <= 939) {
				help();
			}
		}

		FlushBatchDraw();
	}
	EndBatchDraw();
	return 0;
}
