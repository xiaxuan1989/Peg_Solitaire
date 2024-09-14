#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <iomanip>
#include <fstream> // �����ļ�������غ���
#include <random> // �����
using namespace std;

void gotoxy(int x, int y);
void HideCursor();
void color(int c);
void enableAnsiSupport();

void wait_for_enter();
void print_menu();
void print_help();
void background();
void print_interface();

void play_game();
void add_random_tile(int board[4][4]);
bool move_tiles(int board[4][4], char direction, int& score);
void update_board(int board[4][4], int score, int step, int high_score);
void move_with_animation(int x1, int y1, char direction, int num);
void double_with_animation(int x1, int y1, char direction, int num);
void high_precision_sleep(float milliseconds);
void display_tile(int x, int y, int num, int back);

int read_high_score();
void write_high_score(int high_score);

int main() {
    HideCursor();
    enableAnsiSupport();

    char choice = '\0';
    SetConsoleTitle(TEXT("2048"));    // ���ÿ���̨����Ϊ2048
    print_menu();    // ���ò˵���ʾ����

    int select = 0;
    // ��ѭ��
    while (1) {
        color(FOREGROUND_RED | FOREGROUND_INTENSITY);
        gotoxy(16, select + 2); cout << ">";

        // �˵�ѡ��ö��
        enum MenuOptions { CLASSIC_MODE, GAME_RULES, EXIT_GAME, MENU_OPTIONS_COUNT };
        // �����û�ѡ�������Ӧ����
        while (1) {
            choice = _getch();
            if (choice == '\r' or choice == ' ') {       // �س���ո�
                break;
            }
            else if (choice == 72) {    // ��
                gotoxy(16, select + 2); cout << " ";
                select = (select - 1 + MENU_OPTIONS_COUNT) % MENU_OPTIONS_COUNT;
                gotoxy(16, select + 2); cout << ">";
            }
            else if (choice == 80) {    // ��
                gotoxy(16, select + 2); cout << " ";
                select = (select + 1) % MENU_OPTIONS_COUNT;
                gotoxy(16, select + 2); cout << ">";
            }
        }

        switch (select) {
        case CLASSIC_MODE:
            play_game();
            color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            system("CLS");
            print_menu();
            break;
        case GAME_RULES:
            print_help();
            system("CLS");
            print_menu();
            break;
        case EXIT_GAME:
            color(0 | 15);
            system("CLS");
            return 0;
        }
    }
    return 0;
}

//-------��׼����豸������ã����Ǳ�����ͷ�ļ�������Щ����-------
//��ָ����������ַ�
void gotoxy(int x, int y) {
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//���ع��
void HideCursor() {
    CONSOLE_CURSOR_INFO Cursor;
    Cursor.bVisible = FALSE;
    Cursor.dwSize = sizeof(Cursor); //���û��ֵ�Ļ������������Ч
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Cursor);
}
//�����ַ����򵥸��ַ�����ɫ������
void color(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
// ANSI ת������
void enableAnsiSupport() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
//------------------------��Ϸ����------------------------
// �ȴ��س���
void wait_for_enter() {
    cout << "���س����˳�";
    while (_getch() != '\r');
    cout << "\n\n";
}
// �˵�
void print_menu() {
    color(FOREGROUND_BLUE | FOREGROUND_GREEN);    // ǰ��������ɫ
    cout << "              ����������������������������\n";
    for (int i = 0; i < 4; ++i) cout << "              ��            ��\n";
    cout << "              ����������������������������\n";
    color(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    gotoxy(17, 1); cout << "  2048  ";
    gotoxy(16, 2); cout << " ����ģʽ";
    gotoxy(16, 3); cout << " ��Ϸ����";
    gotoxy(16, 4); cout << " �˳���Ϸ";
}
// ����
void print_help() {
    system("CLS");
    color(FOREGROUND_BLUE | FOREGROUND_GREEN);
    cout << "�q�������������������������������������������������������������������������������������������r\n";
    cout << "��                                             ��\n";
    cout << "����������������������������������������������������������������������������������������������\n";
    cout << "��                                             ��\n";
    cout << "�b�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�e\n";
    cout << "��                                             ��\n";
    cout << "����������������������������������������������������������������������������������������������\n";
    for (int i = 0; i < 4; ++i) cout << "��                                             ��\n";
    cout << "�� �q���r�����������������������������������������������������������������������������������ة��r\n";
    cout << "�� �� ��                                           ��\n";
    cout << "�t���s����                                           ��\n";
    cout << "    ��                                           ��\n";
    cout << "    ������������������������������������������������������������������������������������������\n";

    color(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    gotoxy(18, 1); cout << "�� �� ˵ ��";
    gotoxy(1, 3); cout << "�� �������� �� WASD ����             ESC�����˳�";
    gotoxy(18, 5); cout << "�� Ϸ �� ��";
    gotoxy(1, 7); cout << "ÿ��ѡ��һ�������ƶ����ƶ�ʱ������÷���£";    // ��Ϸ����
    gotoxy(1, 8); cout << "��ͬ���ֿɺϲ����ƶ���ո񴦻������������2/4";
    gotoxy(1, 9); cout << "����õ�����2048������Ϸʤ��!";
    gotoxy(1, 10); cout << "������̱������������޷������ƶ�������Ϸ����";

    color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);    // �ָ�����̨������ɫΪĬ����ɫ
    gotoxy(8, 14);
    wait_for_enter();
}
// ������ɫ
void background() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;    // ��ȡ����̨���ڳߴ�
    int cols, rows;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    cout << "\x1b[48;2;250;248;240m";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << ' ';
        }
        cout << "\n";
    }
}
// ��Ϸ����
void print_interface() {
    background();

    gotoxy(12, 0); cout << "\x1b[38;2;185;173;161;48;2;250;248;240m�{�{�{�{�{�{�{�{�{�{�{�{�{";
    gotoxy(26, 0); cout << "�{�{�{�{�{�{�{�{�{�{�{�{�{";
    gotoxy(40, 0); cout << "�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{";
    gotoxy(12, 1); cout << "\x1b[38;2;236;228;219;48;2;185;173;161m ������      ";
    gotoxy(26, 1); cout << " ������      ";
    gotoxy(40, 1); cout << " ��߷֣�      ";
    gotoxy(12, 2); cout << "\x1b[38;2;250;248;240;48;2;185;173;161m�{�{�{�{�{�{�{�{�{�{�{�{�{";
    gotoxy(26, 2); cout << "�{�{�{�{�{�{�{�{�{�{�{�{�{";
    gotoxy(40, 2); cout << "�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{";

    gotoxy(22, 3); cout << "\x1b[38;2;185;173;161;48;2;250;248;240m�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{";
    for (int n = 0; n < 12; ++n) {
        gotoxy(22, 4 + n); cout << "\x1b[48;2;185;173;161m ";
        gotoxy(47, 4 + n); cout << "\x1b[48;2;185;173;161m ";
    }
    gotoxy(22, 16); cout << "\x1b[38;2;250;248;240;48;2;185;173;161m�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{";

    for (int n = 0; n < 4; ++n) {
        for (int m = 0; m < 4; ++m) {
            gotoxy(23 + 6 * m, 4 + 3 * n); cout << "\x1b[38;2;202;193;181;48;2;185;173;161m �{�{�{�{ ";
            gotoxy(23 + 6 * m, 5 + 3 * n); cout << "\x1b[38;2;185;173;161;48;2;202;193;181m��    ��";
            gotoxy(23 + 6 * m, 6 + 3 * n); cout << "���{�{�{�{��";
        }
    }
}
//------------------------�߼�����-----------------------
// ��Ϸ��ѭ��
void play_game() {
    int board[4][4] = {}; // 4x4���̣�����ʼ��
    int score = 0;        // ����
    int step = 0;         // ����
    char choice = '\0';   // �û�ѡ��
    bool game_won = false; // ��־λ�������Ϸ�Ƿ��Ѵ�2048

    int high_score = read_high_score();    // ��߷�

    system("CLS");
    print_interface();    // ��ӡ��Ϸ����
    update_board(board, score, step, high_score);
    Sleep(200);
    add_random_tile(board);     // �������������ʾ
    add_random_tile(board);

    while (1) {
        choice = _getch();        // ��ȡ�û�����

        if (choice == 27) {  // ����û�����ESC��������ѭ��
            break;
        }

        // �����û����������Ӧ����
        if (move_tiles(board, choice, score)) {
            if (score > high_score) {      // ������߷�
                high_score = score;
            }
            update_board(board, score, ++step, high_score);
            Sleep(200);
            add_random_tile(board);
        }

        // ����Ƿ�ﵽ��2048�������û��Ӯ����Ϸ
        if (!game_won) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (board[i][j] == 2048) {
                        game_won = true; // ������Ϸʤ����־
                        gotoxy(29, 18); cout << "\x1b[38;2;117;110;102;48;2;250;248;240m��Ӯ�ˣ�";
                        gotoxy(27, 19); cout << "�Ƿ����(y/n)";
                        while (1) {
                            choice = _getch();
                            if (choice == 'y' or choice == 'Y') {
                                gotoxy(29, 18); cout << "        ";
                                gotoxy(27, 19); cout << "             ";
                                break;
                            }
                            else if (choice == 'n' or choice == 'N') {
                                write_high_score(high_score); //д����߷�
                                return; //ֱ���˳�
                            }
                        }
                        break;
                    }
                }
                if (game_won)
                    break;
            }
        }

        // ����Ƿ����ƶ����飬���ж���Ϸ�Ƿ������������������ѭ��
        bool can_move = false;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (board[i][j] == 0)
                    can_move = true;
                if (i < 3 && board[i][j] == board[i + 1][j])
                    can_move = true;
                if (j < 3 && board[i][j] == board[i][j + 1])
                    can_move = true;
            }
        }
        if (!can_move) {
            gotoxy(29, 18); cout << "\x1b[38;2;117;110;102;48;2;250;248;240m��Ϸ������\n" << endl << endl;
            gotoxy(27, 19); wait_for_enter();
            break;
        }
    }
    write_high_score(high_score); //д����߷�
}
// ����������������������� 2/4 ����ʾ
void add_random_tile(int board[4][4]) {
    random_device rd; // ʹ������豸��Ϊ����
    mt19937 gen(rd());    // �����������
    int empty_tiles[16][2];
    int count = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board[i][j] == 0) {
                empty_tiles[count][0] = i;
                empty_tiles[count][1] = j;
                count++;
            }
        }
    }

    if (count > 0) {
        int random_index = gen() % count;
        int x = empty_tiles[random_index][0];
        int y = empty_tiles[random_index][1];
        board[x][y] = (gen() % 10 + 10) % 10 > 0 ? 2 : 4; // �����ʱ� 9:1 ������� 2 �� 4
        display_tile(24 + y * 6, 5 + x * 3, board[x][y], 14);
    }
}
// �����û��������ƶ�&��������
bool move_tiles(int board[4][4], char direction, int& score) {
    bool moved = false;

    // �� WASD ��ӳ�䵽�������δ��󷽺ϲ������ǿ��ǵ���������
    switch (direction) {
    case 'w':
    case 'W':
        direction = 72; break; // W -> ��
    case 's':
    case 'S':
        direction = 80; break; // S -> ��
    case 'a':
    case 'A':
        direction = 75; break; // A -> ��
    case 'd':
    case 'D':
        direction = 77; break; // D -> ��
    }

    switch (direction) {
    case 72: // ��
        for (int j = 0; j < 4; ++j) {
            for (int i = 1; i < 4; ++i) {
                if (board[i][j] != 0) {
                    int k = i;
                    while (k > 0 && board[k - 1][j] == 0) {        // �ƶ�
                        move_with_animation(j, k, direction, board[k][j]);
                        board[k - 1][j] = board[k][j];
                        board[k][j] = 0;
                        k--;
                        moved = true;
                    }
                    if (k > 0 && board[k - 1][j] == board[k][j]) {        // �ϲ�
                        double_with_animation(j, k, direction, board[k][j]);
                        board[k - 1][j] *= 2;
                        score += board[k - 1][j];
                        board[k][j] = 0;
                        moved = true;
                    }
                }
            }
        }
        break;
    case 80: // ��
        for (int j = 0; j < 4; ++j) {
            for (int i = 2; i >= 0; --i) {
                if (board[i][j] != 0) {
                    int k = i;
                    while (k < 3 && board[k + 1][j] == 0) {
                        move_with_animation(j, k, direction, board[k][j]);
                        board[k + 1][j] = board[k][j];
                        board[k][j] = 0;
                        k++;
                        moved = true;
                    }
                    if (k < 3 && board[k + 1][j] == board[k][j]) {
                        double_with_animation(j, k, direction, board[k][j]);
                        board[k + 1][j] *= 2;
                        score += board[k + 1][j];
                        board[k][j] = 0;
                        moved = true;
                    }
                }
            }
        }
        break;
    case 75: // ��
        for (int i = 0; i < 4; ++i) {
            for (int j = 1; j < 4; ++j) {
                if (board[i][j] != 0) {
                    int k = j;
                    while (k > 0 && board[i][k - 1] == 0) {
                        move_with_animation(k, i, direction, board[i][k]);
                        board[i][k - 1] = board[i][k];
                        board[i][k] = 0;
                        k--;
                        moved = true;
                    }
                    if (k > 0 && board[i][k - 1] == board[i][k]) {
                        double_with_animation(k, i, direction, board[i][k]);
                        board[i][k - 1] *= 2;
                        score += board[i][k - 1];
                        board[i][k] = 0;
                        moved = true;
                    }
                }
            }
        }
        break;
    case 77: // ��
        for (int i = 0; i < 4; ++i) {
            for (int j = 2; j >= 0; --j) {
                if (board[i][j] != 0) {
                    int k = j;
                    while (k < 3 && board[i][k + 1] == 0) {
                        move_with_animation(k, i, direction, board[i][k]);
                        board[i][k + 1] = board[i][k];
                        board[i][k] = 0;
                        k++;
                        moved = true;
                    }
                    if (k < 3 && board[i][k + 1] == board[i][k]) {
                        double_with_animation(k, i, direction, board[i][k]);
                        board[i][k + 1] *= 2;
                        score += board[i][k + 1];
                        board[i][k] = 0;
                        moved = true;
                    }
                }
            }
        }
        break;
    }
    return moved;
}
// ��������
void update_board(int board[4][4], int score, int step, int high_score) {
    gotoxy(18, 1);
    cout << "\x1b[38;2;255;255;255;48;2;185;173;161m" << setw(6) << step;
    gotoxy(32, 1);
    cout << setw(6) << score;
    gotoxy(48, 1);
    cout << setw(6) << high_score;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            display_tile(24 + j * 6, 5 + i * 3, board[i][j], 14);
        }
    }
}
// �ƶ�����
void move_with_animation(int x, int y, char direction, int num) {
    if (num == 0) return;
    const float time = 2; // �ӳ�

    switch (direction) {
    case 72: // ��
        display_tile(24 + x * 6, 5 + y * 3 - 1, num, 0);
        gotoxy(24 + x * 6, 5 + y * 3 + 1); cout << "\x1b[38;2;185;173;161;48;2;202;193;181m�{�{�{�{"; // ���֮ǰ��λ��
        high_precision_sleep(time);

        display_tile(24 + x * 6, 5 + y * 3 - 2, num, 0);
        gotoxy(24 + x * 6, 5 + y * 3); cout << "\x1b[48;2;202;193;181m    ";
        high_precision_sleep(time);

        display_tile(24 + x * 6, 5 + y * 3 - 3, num, 14);
        gotoxy(24 + x * 6, 5 + y * 3 - 1); cout << "\x1b[38;2;202;193;181;48;2;185;173;161m�{�{�{�{";
        high_precision_sleep(time);
        break;
    case 80: // ��
        display_tile(24 + x * 6, 5 + y * 3 + 1, num, 0);
        gotoxy(24 + x * 6, 5 + y * 3 - 1); cout << "\x1b[38;2;202;193;181;48;2;185;173;161m�{�{�{�{";
        high_precision_sleep(time);

        display_tile(24 + x * 6, 5 + y * 3 + 2, num, 0);
        gotoxy(24 + x * 6, 5 + y * 3); cout << "\x1b[48;2;202;193;181m    ";
        high_precision_sleep(time);

        display_tile(24 + x * 6, 5 + y * 3 + 3, num, 14);
        gotoxy(24 + x * 6, 5 + y * 3 + 1); cout << "\x1b[38;2;185;173;161;48;2;202;193;181m�{�{�{�{";
        high_precision_sleep(time);
        break;
    case 75: // ��
        for (int n = 1; n <= 4; ++n) {
            display_tile(24 + x * 6 - n, 5 + y * 3, num, 14);
            gotoxy(24 + x * 6 - n + 4, 5 + y * 3 - 1); cout << "\x1b[38;2;202;193;181;48;2;185;173;161m�{";
            gotoxy(24 + x * 6 - n + 4, 5 + y * 3); cout << "\x1b[48;2;202;193;181m ";
            gotoxy(24 + x * 6 - n + 4, 5 + y * 3 + 1); cout << "\x1b[38;2;185;173;161;48;2;202;193;181m�{";
            high_precision_sleep(time);
        }
        for (int n = 5; n <= 6; ++n) {
            display_tile(24 + x * 6 - n, 5 + y * 3, num, 14);
            gotoxy(24 + x * 6 - n + 4, 5 + y * 3 - 1); cout << "\x1b[48;2;185;173;161m ";
            gotoxy(24 + x * 6 - n + 4, 5 + y * 3); cout << "\x1b[48;2;185;173;161m ";
            gotoxy(24 + x * 6 - n + 4, 5 + y * 3 + 1); cout << "\x1b[48;2;185;173;161m ";
            high_precision_sleep(time);
        }
        break;
    case 77: // ��
        for (int n = 1; n <= 4; ++n) {
            display_tile(24 + x * 6 + n, 5 + y * 3, num, 14);
            gotoxy(24 + x * 6 + n - 1, 5 + y * 3 - 1); cout << "\x1b[38;2;202;193;181;48;2;185;173;161m�{";
            gotoxy(24 + x * 6 + n - 1, 5 + y * 3); cout << "\x1b[48;2;202;193;181m ";
            gotoxy(24 + x * 6 + n - 1, 5 + y * 3 + 1); cout << "\x1b[38;2;185;173;161;48;2;202;193;181m�{";
            high_precision_sleep(time);
        }
        for (int n = 5; n <= 6; ++n) {
            display_tile(24 + x * 6 + n, 5 + y * 3, num, 14);
            gotoxy(24 + x * 6 + n - 1, 5 + y * 3 - 1); cout << "\x1b[48;2;185;173;161m ";
            gotoxy(24 + x * 6 + n - 1, 5 + y * 3); cout << "\x1b[48;2;185;173;161m ";
            gotoxy(24 + x * 6 + n - 1, 5 + y * 3 + 1); cout << "\x1b[48;2;185;173;161m ";
            high_precision_sleep(time);
        }
        break;
    }
}
// �ϲ�&��������
void double_with_animation(int x, int y, char direction, int num) {
    string tile_colors[] = { "202;193;181", "236;228;219", "235;224;203", "232;180;129",
                         "232;154;108", "230;131;103", "228;104;71", "232;208;127",
                         "218;190;107", "241;202;108", "236;200;93", "240;197;80",
                         "235;133;123", "235;78;52", "185;173;161" };
    if (num == 0) return;
    const float time = 2; // �ӳ�
    const float time_d = 50; // �����ӳ�
    int tile_order = log2(num);
    switch (direction) {
    case 72: // ��
        display_tile(24 + x * 6, 5 + y * 3 - 1, num, 0);
        gotoxy(24 + x * 6, 5 + y * 3 + 1); cout << "\x1b[38;2;185;173;161;48;2;202;193;181m�{�{�{�{";
        gotoxy(24 + x * 6, 5 + y * 3 - 2); cout << "\x1b[48;2;" << tile_colors[tile_order] << "m    ";
        high_precision_sleep(time);

        display_tile(24 + x * 6, 5 + y * 3 - 2, num, 0);
        gotoxy(24 + x * 6, 5 + y * 3); cout << "\x1b[48;2;202;193;181m    ";
        gotoxy(24 + x * 6, 5 + y * 3 - 3); cout << "\x1b[48;2;" << tile_colors[tile_order] << "m    ";
        high_precision_sleep(time);

        display_tile(24 + x * 6, 5 + y * 3 - 3, num, 14);
        gotoxy(24 + x * 6, 5 + y * 3 - 1); cout << "\x1b[38;2;202;193;181;48;2;185;173;161m�{�{�{�{";
        high_precision_sleep(time);

        // ����
        gotoxy(24 + x * 6 - 1, 5 + y * 3 - 3); cout << "\x1b[48;2;" << tile_colors[tile_order + 1] << "m      ";
        display_tile(24 + x * 6, 5 + y * 3 - 3, num * 2, 14);
        gotoxy(24 + x * 6 - 1, 5 + y * 3 - 3 - 1); cout << "\x1b[48;2;" << tile_colors[tile_order + 1] << "m      ";
        gotoxy(24 + x * 6 - 1, 5 + y * 3 - 3 + 1); cout << "\x1b[48;2;" << tile_colors[tile_order + 1] << "m      ";
        high_precision_sleep(time_d);

        gotoxy(24 + x * 6 - 1, 5 + y * 3 - 3); cout << "\x1b[48;2;185;173;161m      ";
        gotoxy(24 + x * 6 - 1, 5 + y * 3 - 3 - 1); cout << "\x1b[48;2;185;173;161m      ";
        gotoxy(24 + x * 6 - 1, 5 + y * 3 - 3 + 1); cout << "\x1b[48;2;185;173;161m      ";
        display_tile(24 + x * 6, 5 + y * 3 - 3, num * 2, 14);
        high_precision_sleep(time_d);
        break;
    case 80: // ��
        display_tile(24 + x * 6, 5 + y * 3 + 1, num, 0);
        gotoxy(24 + x * 6, 5 + y * 3 - 1); cout << "\x1b[38;2;202;193;181;48;2;185;173;161m�{�{�{�{";
        gotoxy(24 + x * 6, 5 + y * 3 + 2); cout << "\x1b[48;2;" << tile_colors[tile_order] << "m    ";
        high_precision_sleep(time);

        display_tile(24 + x * 6, 5 + y * 3 + 2, num, 0);
        gotoxy(24 + x * 6, 5 + y * 3); cout << "\x1b[48;2;202;193;181m    ";
        gotoxy(24 + x * 6, 5 + y * 3 + 3); cout << "\x1b[48;2;" << tile_colors[tile_order] << "m    ";
        high_precision_sleep(time);

        display_tile(24 + x * 6, 5 + y * 3 + 3, num, 14);
        gotoxy(24 + x * 6, 5 + y * 3 + 1); cout << "\x1b[38;2;185;173;161;48;2;202;193;181m�{�{�{�{";
        gotoxy(24 + x * 6, 5 + y * 3 + 4); cout << "\x1b[48;2;" << tile_colors[tile_order] << "m    ";
        high_precision_sleep(time);

        // ����
        gotoxy(24 + x * 6 - 1, 5 + y * 3 + 3); cout << "\x1b[48;2;" << tile_colors[tile_order + 1] << "m      ";
        display_tile(24 + x * 6, 5 + y * 3 + 3, num * 2, 14);
        gotoxy(24 + x * 6 - 1, 5 + y * 3 + 3 - 1); cout << "\x1b[48;2;" << tile_colors[tile_order + 1] << "m      ";
        gotoxy(24 + x * 6 - 1, 5 + y * 3 + 3 + 1); cout << "\x1b[48;2;" << tile_colors[tile_order + 1] << "m      ";
        high_precision_sleep(time_d);

        gotoxy(24 + x * 6 - 1, 5 + y * 3 + 3); cout << "\x1b[48;2;185;173;161m      ";
        gotoxy(24 + x * 6 - 1, 5 + y * 3 + 3 - 1); cout << "\x1b[48;2;185;173;161m      ";
        gotoxy(24 + x * 6 - 1, 5 + y * 3 + 3 + 1); cout << "\x1b[48;2;185;173;161m      ";
        display_tile(24 + x * 6, 5 + y * 3 + 3, num * 2, 14);
        high_precision_sleep(time_d);
        break;
    case 75: // ��
        for (int n = 1; n <= 4; ++n) {
            display_tile(24 + x * 6 - n, 5 + y * 3, num, 14);
            gotoxy(24 + x * 6 - n + 4, 5 + y * 3 - 1); cout << "\x1b[38;2;202;193;181;48;2;185;173;161m�{";
            gotoxy(24 + x * 6 - n + 4, 5 + y * 3); cout << "\x1b[48;2;202;193;181m ";
            gotoxy(24 + x * 6 - n + 4, 5 + y * 3 + 1); cout << "\x1b[38;2;185;173;161;48;2;202;193;181m�{";
            high_precision_sleep(time);
        }
        for (int n = 5; n <= 6; ++n) {
            display_tile(24 + x * 6 - n, 5 + y * 3, num, 14);
            gotoxy(24 + x * 6 - n + 4, 5 + y * 3 - 1); cout << "\x1b[48;2;185;173;161m ";
            gotoxy(24 + x * 6 - n + 4, 5 + y * 3); cout << "\x1b[48;2;185;173;161m ";
            gotoxy(24 + x * 6 - n + 4, 5 + y * 3 + 1); cout << "\x1b[48;2;185;173;161m ";
            high_precision_sleep(time);
        }
        // ����
        gotoxy(24 + x * 6 - 6 - 1, 5 + y * 3); cout << "\x1b[48;2;" << tile_colors[tile_order + 1] << "m      ";
        display_tile(24 + x * 6 - 6, 5 + y * 3, num * 2, 14);
        gotoxy(24 + x * 6 - 6 - 1, 5 + y * 3 - 1); cout << "\x1b[48;2;" << tile_colors[tile_order + 1] << "m      ";
        gotoxy(24 + x * 6 - 6 - 1, 5 + y * 3 + 1); cout << "\x1b[48;2;" << tile_colors[tile_order + 1] << "m      ";
        high_precision_sleep(time_d);

        gotoxy(24 + x * 6 - 6 - 1, 5 + y * 3); cout << "\x1b[48;2;185;173;161m      ";
        gotoxy(24 + x * 6 - 6 - 1, 5 + y * 3 - 1); cout << "\x1b[48;2;185;173;161m      ";
        gotoxy(24 + x * 6 - 6 - 1, 5 + y * 3 + 1); cout << "\x1b[48;2;185;173;161m      ";
        display_tile(24 + x * 6 - 6, 5 + y * 3, num * 2, 14);
        high_precision_sleep(time_d);
        break;
    case 77: // ��
        for (int n = 1; n <= 4; ++n) {
            display_tile(24 + x * 6 + n, 5 + y * 3, num, 14);
            gotoxy(24 + x * 6 + n - 1, 5 + y * 3 - 1); cout << "\x1b[38;2;202;193;181;48;2;185;173;161m�{";
            gotoxy(24 + x * 6 + n - 1, 5 + y * 3); cout << "\x1b[48;2;202;193;181m ";
            gotoxy(24 + x * 6 + n - 1, 5 + y * 3 + 1); cout << "\x1b[38;2;185;173;161;48;2;202;193;181m�{";
            high_precision_sleep(time);
        }
        for (int n = 5; n <= 6; ++n) {
            display_tile(24 + x * 6 + n, 5 + y * 3, num, 14);
            gotoxy(24 + x * 6 + n - 1, 5 + y * 3 - 1); cout << "\x1b[48;2;185;173;161m ";
            gotoxy(24 + x * 6 + n - 1, 5 + y * 3); cout << "\x1b[48;2;185;173;161m ";
            gotoxy(24 + x * 6 + n - 1, 5 + y * 3 + 1); cout << "\x1b[48;2;185;173;161m ";
            high_precision_sleep(time);
        }
        // ����
        gotoxy(24 + x * 6 + 6 - 1, 5 + y * 3); cout << "\x1b[48;2;" << tile_colors[tile_order + 1] << "m      ";
        display_tile(24 + x * 6 + 6, 5 + y * 3, num * 2, 14);
        gotoxy(24 + x * 6 + 6 - 1, 5 + y * 3 - 1); cout << "\x1b[48;2;" << tile_colors[tile_order + 1] << "m      ";
        gotoxy(24 + x * 6 + 6 - 1, 5 + y * 3 + 1); cout << "\x1b[48;2;" << tile_colors[tile_order + 1] << "m      ";
        high_precision_sleep(time_d);

        gotoxy(24 + x * 6 + 6 - 1, 5 + y * 3); cout << "\x1b[48;2;185;173;161m      ";
        gotoxy(24 + x * 6 + 6 - 1, 5 + y * 3 - 1); cout << "\x1b[48;2;185;173;161m      ";
        gotoxy(24 + x * 6 + 6 - 1, 5 + y * 3 + 1); cout << "\x1b[48;2;185;173;161m      ";
        display_tile(24 + x * 6 + 6, 5 + y * 3, num * 2, 14);
        high_precision_sleep(time_d);
        break;
    }
}
// �߾����ӳ�
void high_precision_sleep(float milliseconds) {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;

    QueryPerformanceFrequency(&frequency);    // ��ȡ�߾��ȼ�ʱ����Ƶ��
    float wait_time = milliseconds * (frequency.QuadPart / 1000.0);    // ����ȴ�ʱ��
    QueryPerformanceCounter(&start);    // ��ȡ��ǰʱ��

    while (1) {
        QueryPerformanceCounter(&end);        // ��ȡ��ǰʱ��
        float elapsed_time = (float)(end.QuadPart - start.QuadPart);        // �����ѹ�ȥ��ʱ��
        // ����ѹ�ȥ��ʱ����ڵ��ڵȴ�ʱ�䣬�˳�ѭ��
        if (elapsed_time >= wait_time)
            break;
    }
}
// ��ʾ����
void display_tile(int x, int y, int num, int back) {
    if (num == 0) num++;
    string text_colors[] = { "117;110;102", "248;246;242", "255;255;255" };
    string tile_colors[] = { "202;193;181", "236;228;219", "235;224;203", "232;180;129",
                             "232;154;108", "230;131;103", "228;104;71", "232;208;127",
                             "218;190;107", "241;202;108", "236;200;93", "240;197;80",
                             "235;133;123", "235;78;52", "185;173;161" };
    string num_show[] = { "    ", " �� ", " �� ", " �� ",
                          " 16 ", " 32 ", " 64 ", "128 ",
                          "256 ", "512 ", "1024", "2048",
                          "4096", "8192", "XXXX" };

    int text_order;
    if (num < 8200) {
        text_order = num < 5 ? 0 : 1;
    }
    else {
        text_order = 2;
    }
    int tile_order = log2(num);

    gotoxy(x, y - 1); cout << "\x1b[38;2;" << tile_colors[tile_order] << ";48;2;" << tile_colors[back] << "m�{�{�{�{";
    gotoxy(x, y); cout << "\x1b[38;2;" << text_colors[text_order] << ";48;2;" << tile_colors[tile_order] << "m" << num_show[tile_order];
    gotoxy(x, y + 1); cout << "\x1b[38;2;" << tile_colors[back] << ";48;2;" << tile_colors[tile_order] << "m�{�{�{�{";
}
//----------------------��д��߷�---------------------
// ��ȡ��߷�
int read_high_score() {
    ifstream file("2048_highscore.txt");
    int high_score = 0;
    if (file.is_open()) {
        file >> high_score;
        file.close();
    }
    return high_score;
}
// д����߷�
void write_high_score(int high_score) {
    ofstream file("2048_highscore.txt");
    if (file.is_open()) {
        file << high_score;
        file.close();
    }
}