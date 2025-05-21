#define _CRT_SECURE_NO_WARNINGS 1
#pragma comment(lib,"Winmm.lib")
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<Windows.h>
#include<conio.h>
#include<cstdio>
#include<cstdlib>
#include<graphics.h>
#define dX 18
#define dY dX//�Թ��ĳ��Ϳ�
#define PATH 1
#define BARRICADE 0
#define PACE 3//����ͨ·��ǽ��·��
#define LENGTH dX
#define size 25
#define ROW dX
#define COL dY
static const int L = dY;
static const int PATH1 = 0;
static const int BARRICADE1 = 1;//"��ǽ"���ж�Ӧ��ǽ��·
static int rank = 0;//�����Թ����Ӷ�
int row;
HANDLE hOutput, hOutBuf;
HANDLE* houtpoint;
COORD coord = { 0,0 };
DWORD bytes = 0;//��Ļ����������ֹ����
unsigned char data[LENGTH + 3][2 * LENGTH];//���ƻ�������С
bool showcircle = false;//˫�������
typedef struct
{
    int x;
    int y;
}coordinate;//����
int pathspawn_0(void);//�������·��ǽ
int sumloc(int i, int j, int** a);//�ж�ĳ������·����������ֹ����ѭ��
char trans(int x);//��0��1ת��Ϊ��Ӧ��ǽ��·
void copy(int** a, int** target);//��������
void eleprint(int** puzzle);//��ӡ�Թ�
void spawnpuzzle_break(int** puzzle, int x, int y);//"��·��"�����Թ�
void spawnpuzzle_create(int** maze, int x1, int y1, int x2, int y2);//"��ǽ��"�����Թ�
void up(int* x, int* y, int** a);
void down(int* x, int* y, int** a);
void left(int* x, int* y, int** a);
void right(int* x, int* y, int** a);//���ĸ������ƶ��ĺ���
void show(void);//˫������ʾ
void draw(int** map, int a);//˫�������
void map1(int arr[][COL], int row);//���Ƶ�ͼ
void rules(void);//���ڷ�����ʾ����
void buttom2(int left, int top, int right, int bottom);//��ʾ�˳���Ϸ�İ�ť
void buttom_1(int left, int top, int right, int bottom);
void buttom_2(int left, int top, int right, int bottom);
void buttom_3(int left, int top, int right, int bottom);
void buttom_4(int left, int top, int right, int bottom);
void buttom_5(int left, int top, int right, int bottom);
void buttom_6(int left, int top, int right, int bottom);
void buttom_7(int left, int top, int right, int bottom);
void buttom_8(int left, int top, int right, int bottom);
void buttom_9(int left, int top, int right, int bottom);
void buttom_10(int left, int top, int right, int bottom);//����˵���ť
void imageprint(int** map);
void imageprint2(int** map);//ͼ�λ���������Թ�
void choice_1(int left, int top, int right, int bottom);
void choice_2(int left, int top, int right, int bottom);
void choice_3(int left, int top, int right, int bottom);
void choice_4(int left, int top, int right, int bottom);
void choice_5(int left, int top, int right, int bottom);
void judge_1(int left, int top, int right, int bottom);
void judge_2(int left, int top, int right, int bottom);
int main(int argc, const char* argv[])
{
    hOutBuf = CreateConsoleScreenBuffer(
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
    hOutput = CreateConsoleScreenBuffer(
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );//����������д������
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = 0;
    cci.dwSize = 1;
    SetConsoleCursorInfo(hOutput, &cci);
    SetConsoleCursorInfo(hOutBuf, &cci);//���ػ��������
    srand(time(NULL));//�������
    SetConsoleOutputCP(437);//Ӣ�������ʹ����չASCIII������Թ�
    int choice;//�Թ����ɷ�����ѡ��
    int solution;//���Թ�������ѡ��
    int arr[ROW][COL] = { 0 };//��������
    //��������յ㣺
    arr[1][0] = 1;
    arr[ROW - 2][COL - 1] = 1;
    int play;
    int judge = 0;//���ڽ���ĳһ��Ϸ�׶εı���
    int r = 1, c = 0;//ȷ��ǰһ�ε��������λ��
    int i = 0;
    int playround = 0;




    initgraph(dX * size * 2, dY*size*2,NULL);
    loadimage(NULL, _T("sunset.jpg"));
    mciSendString(_T("open SnowOnTheBeach.mp3 alias bkmusic"), NULL, 0, NULL);
    mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);
    settextstyle(46, 0, _T("Algerian"));
    int ques = 0;
    do {
        int** puzzle = (int**)malloc((dX + 2) * sizeof(int*));
        for (int i = 0; i < (dX + 2); i++)
        {
            puzzle[i] = (int*)calloc((dY + 2), sizeof(int));
        }
        int** Maze = (int**)malloc(L * sizeof(int*));
        for (int i = 0; i < L; i++)
        {
            Maze[i] = (int*)calloc(L, sizeof(int));
        }
        int** map = (int**)malloc(dX * sizeof(int*));
        for (int i = 0; i < dX; i++)
        {
            map[i] = (int*)calloc(dY, sizeof(int));
        }//�����ڴ棬�����ά�����Դ����Թ��Ĳ�ͬ״̬
        wchar_t welcome[] = L"Maze Game";
        int dey = 30;//�ж��ַ��߶�
        outtextxy(dX * size - textwidth(welcome) / 2, 20, welcome);
        settextstyle(25, 0, _T("Bookman Old Style"));
        wchar_t guide1[] = L"Click your choice 1 / 2 / 3 / 4 / 5 to create the maze:";
        outtextxy(dX * size - textwidth(guide1) / 2, 116, guide1);
        buttom_1(dX * size - textwidth(L"1.Create Wall") / 2, 156 + dey, dX * size + textwidth(L"1.Create Wall") / 2, 186 + dey);
        buttom_2(dX * size - textwidth(L"2.Create Path") / 2, 226 + dey, dX * size + textwidth(L"2.Create Path") / 2, 256 + dey);
        buttom_3(dX * size - textwidth(L"3.Type in the coordinates") / 2, 296 + dey, dX * size + textwidth(L"3.Type in the coordinates") / 2, 326 + dey);
        buttom_4(dX * size - textwidth(L"4.Click to create path") / 2, 366 + dey, dX * size + textwidth(L"4.Click to create path") / 2, 396 + dey);
        buttom_5(dX * size - textwidth(L"5.Import from file") / 2, 436 + dey, dX * size + textwidth(L"5.Import from file") / 2, 466 + dey);//����ͼ�λ����棬���ɰ�ť
        int tempjudge;
        do {
            tempjudge = 0;
            if (MouseHit())
            {
                MOUSEMSG msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN)
                {
                    if (msg.x >= dX * size - textwidth(L"1.Create Wall") / 2 && msg.x < (dX * size + textwidth(L"1.Create Wall") / 2) && msg.y >= 156 + dey && msg.y <= 186 + dey)
                    {
                        choice = 1;
                        tempjudge = 1;
                    }
                    if (msg.x >= dX * size - textwidth(L"2.Create Path") / 2 && msg.x < (dX * size + textwidth(L"2.Create Path") / 2) && msg.y >= 226 + dey && msg.y <= 256 + dey)
                    {
                        choice = 2;
                        tempjudge = 1;
                    }
                    if (msg.x >= dX * size - textwidth(L"3.Type in the coordinates") / 2 && msg.x < (dX * size + textwidth(L"3.Type in the coordinates") / 2) && msg.y >= 296 + dey && msg.y <= 326 + dey)
                    {
                        choice = 3;
                        tempjudge = 1;
                    }
                    if (msg.x >= dX * size - textwidth(L"4.Click to create path") / 2 && msg.x < (dX * size + textwidth(L"4.Click to create path") / 2) && msg.y >= 366 + dey && msg.y <= 396 + dey)
                    {
                        choice = 4;
                        tempjudge = 1;
                    }
                    if (msg.x >= dX * size - textwidth(L"5.Import from file") / 2 && msg.x < (dX * size + textwidth(L"5.Import from file") / 2) && msg.y >= 433 + dey && msg.y <= 466 + dey)
                    {
                        choice = 5;
                        tempjudge = 1;
                    }
                }
            }
        } while (tempjudge != 1);//�ж���ť���
       
        int sx = 0, sy = 0;

        FILE* infile;
        cleardevice();
        loadimage(NULL, _T("sunset.jpg"));
        int x1 = 0, y1 = 0;
        switch (choice) {
        case 1://����ǽ
            for (int i = 0; i < L; i++)
            {
                for (int j = 0; j < L; j++)
                {
                    if (i == 0 || i == L - 1 || j == 0 || j == L - 1)
                        Maze[i][j] = BARRICADE1;
                }
            }//��ΧһȦ���ǽ��ʼ�ݹ�
            spawnpuzzle_create(Maze, 1, 1, L - 2, L - 2);
            Maze[1][0] = PATH1;
            Maze[L - 2][L - 1] = PATH1;//�����յ�
            for (int i = 0; i < L; i++)
            {
                for (int j = 0; j < L; j++)
                {
                    if (Maze[i][j] == BARRICADE1)
                        Maze[i][j] = BARRICADE;
                    else
                        Maze[i][j] = PATH;
                }
            }//����׼ͳһ�������
            copy(Maze, map);//���Թ�����������Թ��ĵ�ͼ������
            for (int i = 0; i < L; i++)
                free(Maze[i]);

            break;
        case 2://����·
            for (int i = 0; i < (dX + 2); i++)
            {
                for (int j = 0; j < (dY + 2); j++)
                {
                    if (i == 0 || i == (dX + 2) - 1 || j == 0 || j == (dY + 2) - 1)
                        puzzle[i][j] = PATH;
                }
            }//��ΧһȦ���·��ֹ�ڴ�
            spawnpuzzle_break(puzzle, 2, 2);
            puzzle[2][1] = PATH;
            for (int i = (dX + 2) - 3; i >= 0; i--)
            {
                if (puzzle[i][(dY + 2) - 3] == PATH)
                {
                    puzzle[i][(dY + 2) - 2] = PATH;
                    break;
                }
            }//Ӧ���յ㲻Ψһ�������ȷ����·����

            for (int i = 0; i < dX; i++)
            {
                for (int j = 0; j < dY; j++)
                    map[i][j] = puzzle[i + 1][j + 1];
            }//���Թ�����������Թ��ĵ�ͼ�����飬����ȥ��������һȦ��������
            for (int i = 0; i < (dX + 2); i++) free(puzzle[i]);

            break;
        case 3://�����������Թ�

            for (int i = 0; i < dX; i++)
            {
                for (int j = 0; j < dY; j++)
                {
                    Maze[i][j] = BARRICADE;
                }
            }//��ʼ��Ϊǽ
            cleardevice();
            loadimage(NULL, _T("sunset.jpg"));
            imageprint2(Maze);

            outtextxy(20, 30 + ROW * size, L"The starting and ending points have been defined.");
            outtextxy(20, 55 + ROW * size, L"No other points can be chosen as the starting and ending points.");
            outtextxy(20, 80 + ROW * size, L"Type in the end point to finish the maze drawing.");
            do {

                BeginBatchDraw();
                settextstyle(20, 0, _T("Bookman Old Style"));
                for (int i = 0; i < dY; i++)
                {
                    TCHAR s[25];
                    if (i < 10)
                        _stprintf_s(s, _T("0%d"), i);
                    if (i >= 10)
                        _stprintf_s(s, _T("%d"), i);
                    outtextxy((i + 1) * size, 0, s);
                    outtextxy(0, (i + 1) * size, s);
                }//��ӡ���ӻ�����������
                settextstyle(25, 0, _T("Bookman Old Style"));
                EndBatchDraw();
                Maze[1][0] = PATH;//���
                wchar_t s[10];
                bool isok1 = InputBox(s, 10, L"Please enter x.\n X and Y can't be more than 19.", 0, 0, 0, 0, false);
                x1 = _wtoi(s);
                if (isok1 == false) {
                    break;
                }
                bool isok2 = InputBox(s, 10, L"Please enter y.\n X and Y can't be more than 19.", 0, 0, 0, 0, false);
                y1 = _wtoi(s);
                if (isok2 == false) {
                    break;
                }
                if (x1 >= COL || y1 >= COL) {
                    outtextxy(0, 65 + ROW * size, L"It breaks the rule.");
                    continue;
                }
                Maze[x1][y1] = PATH;
                imageprint2(Maze);
            } while (x1 != ROW - 2 || y1 != COL - 1);
            EndBatchDraw();
            copy(Maze, map);
            imageprint2(map);


            break;
        case 4://����������Թ�
            for (int i = 0; i < dX; i++) {
                for (int j = 0; j < dY; j++) {
                    Maze[i][j] = BARRICADE;
                }
            }
            Maze[1][0] = PATH;
            Maze[ROW - 2][COL - 1] = PATH;
            do {
                //��ʾ����
                BeginBatchDraw();
                settextcolor(WHITE);
                //settextstyle(15, 10, L"Dotum");
                setbkmode(TRANSPARENT);
                outtextxy(20, 30 + ROW * size, L"The starting and ending points have been defined.");
                outtextxy(20, 55 + ROW * size, L"No other points can be chosen as the starting and ending points.");
                outtextxy(20, 80 + ROW * size, L"Click the end point to finish the maze drawing.");
                imageprint2(Maze);
                FlushBatchDraw();
                if (MouseHit()) {
                    MOUSEMSG msg = GetMouseMsg();
                    switch (msg.uMsg) {
                    case WM_LBUTTONDOWN:
                        //�����������������Ԫ�صĹ�ϵ��
                        int row1 = (msg.y - 20) / size;
                        int col1 = (msg.x - 20) / size;
                        cleardevice();
                        loadimage(NULL, _T("sunset.jpg"));
                        //�����ĸ���������ȷ��Ψһ�������յ㣺
                        if (msg.x >= 20 && msg.x <= size + 20 && msg.y <= 20 + ROW * size && msg.y >= 20) {
                            outtextxy(30, 195 + ROW * size, L"It breaks the rule.");
                            break;
                        }
                        if (msg.x >= 20 && msg.x <= 20 + COL * size && msg.y >= 20 + (ROW - 1) * size && msg.y <= 20 + ROW * size) {
                            outtextxy(30, 195 + ROW * size, L"It breaks the rule.");
                            break;
                        }
                        if ((msg.x >= 20 + (COL - 1) * size && msg.x <= 20 + COL * size) && (msg.y <= 20 + (ROW - 2) * size || msg.y >= 20 + (ROW - 1) * size)) {
                            outtextxy(30, 295 + ROW * size, L"It breaks the rule.");
                            break;
                        }
                        if (msg.x >= 20 && msg.x <= 20 + COL * size && msg.y >= 20 && msg.y <= 20 + size) {
                            outtextxy(30, 295 + ROW * size, L"It breaks the rule.");
                            break;
                        }


                        //�����Թ���
                        if (row1 < ROW && col1 < COL) {
                            Maze[row1][col1] = PATH;
                        }
                        //�ж��Ƿ�������ƣ�
                        if (msg.x >= 20 + (COL - 1) * size && msg.x <= 20 + COL * size && msg.y >= 20 + (ROW - 2) * size && msg.y <= 20 + (ROW - 1) * size) {
                            judge = 1;
                            break;
                        }
                        break;
                    }
                    //�������ƣ�
                    if (judge == 1) {
                        break;
                    }
                }


            } while (1);


            EndBatchDraw();
            copy(Maze, map);

            imageprint2(map);

            break;

                 //�ļ����������Թ�
        default:
            cleardevice();
            loadimage(NULL, _T("sunset.jpg"));
            char filename[10];
            filename[0] = 'm';
            filename[1] = 'a';
            filename[2] = 'z';
            filename[3] = 'e';
            filename[4] = '0';
            filename[5] = '.';
            filename[6] = 't';
            filename[7] = 'x';
            filename[8] = 't';
            filename[9] = '\0';
            outtextxy(20, 20 + 2 * dey, L"Please choose the five options below\n");
            choice_1(20, 20 + 4 * dey, 20 + textwidth(L"1.maze1"), 20 + 5 * dey);
            choice_2(20, 20 + 6 * dey, 20 + textwidth(L"2.maze2"), 20 + 7 * dey);
            choice_3(20, 20 + 8 * dey, 20 + textwidth(L"3.maze3"), 20 + 9 * dey);
            choice_4(20, 20 + 10 * dey, 20 + textwidth(L"4.maze4"), 20 + 11 * dey);
            choice_5(20, 20 + 12 * dey, 20 + textwidth(L"5.maze5"), 20 + 13 * dey);
            do {
                tempjudge = 0;
                if (MouseHit())
                {
                    MOUSEMSG msg = GetMouseMsg();
                    if (msg.uMsg == WM_LBUTTONDOWN)
                    {
                        if (msg.x >= 20 && msg.x < (20 + textwidth(L"1.maze1")) && msg.y >= 20 + 4 * dey && msg.y <= 20 + 5 * dey)
                        {
                            choice = 1;
                            tempjudge = 1;
                        }
                        if (msg.x >= 20 && msg.x < (20 + textwidth(L"2.maze2")) && msg.y >= 20 + 6 * dey && msg.y <= 20 + 7 * dey)
                        {
                            choice = 2;
                            tempjudge = 1;
                        }
                        if (msg.x >= 20 && msg.x < (20 + textwidth(L"3.maze3")) && msg.y >= 20 + 8 * dey && msg.y <= 20 + 9 * dey)
                        {
                            choice = 3;
                            tempjudge = 1;
                        }
                        if (msg.x >= 20 && msg.x < (20 + textwidth(L"4.maze4")) && msg.y >= 20 + 10 * dey && msg.y <= 20 + 11 * dey)
                        {
                            choice = 4;
                            tempjudge = 1;
                        }
                        if (msg.x >= 20 && msg.x < (20 + textwidth(L"5.maze5")) && msg.y >= 20 + 12 * dey && msg.y <= 20 + 13 * dey)
                        {
                            choice = 5;
                            tempjudge = 1;
                        }
                    }
                }
            } while (tempjudge != 1);
            if (choice == 1)
                filename[4] = '1';
            else if (choice == 2)
                filename[4] = '2';
            else if (choice == 3)
                filename[4] = '3';
            else if (choice == 4)
                filename[4] = '4';
            else if (choice == 5)
                filename[4] = '5';
            if ((infile = fopen(filename, "r+")) == NULL)
            {
                fprintf(stderr, "Can't open file %s\n", filename);
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < dX; i++)
            {
                for (int p = 0; p < dY; p++)
                {
                    fscanf(infile, "%d", &Maze[i][p]);

                }
            }
            if (ferror(infile))
            {
                fprintf(stderr, "Error getting data from file %s\n", filename);
                exit(EXIT_FAILURE);
            }
            copy(Maze, map);
            break;
        }
        free(Maze);
        free(puzzle);
        cleardevice();
        loadimage(NULL, _T("sunset.jpg"));
        imageprint(map);//��ӡ�Թ�
        outtextxy(0, 30 + 8 * dey + (dY + 2) * size / 2, L"Save the file?\n");
        judge_1(20, 50 + 10 * dey + (dY + 2) * size / 2, 20 + textwidth(L"No "), 50 + 11 * dey + (dY + 2) * size / 2);
        judge_2(20, 50 + 12 * dey + (dY + 2) * size / 2, 20 + textwidth(L"Yes"), 50 + 13 * dey + (dY + 2) * size / 2);
        do {
            tempjudge = 0;
            if (MouseHit())
            {
                MOUSEMSG msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN)
                {
                    if (msg.x >= 20 && msg.x < (20 + textwidth(L"Yes")) && msg.y >= 50 + 10 * dey + (dY + 2) * size / 2 && msg.y <= 50 + 11 * dey + (dY + 2) * size / 2)
                    {
                        choice = 1;
                        tempjudge = 1;
                    }
                    if (msg.x >= 20 && msg.x < (20 + textwidth(L"No ")) && msg.y >= 50 + 12 * dey + (dY + 2) * size / 2 && msg.y <= 50 + 13 * dey + (dY + 2) * size / 2)
                    {
                        choice = 2;
                        tempjudge = 1;
                    }
                }
            }
        } while (tempjudge != 1);
        if (choice == 1)
        {
            cleardevice();
            loadimage(NULL, _T("sunset.jpg"));
            char filename[10];
            filename[0] = 'm';
            filename[1] = 'a';
            filename[2] = 'z';
            filename[3] = 'e';
            filename[4] = '0';
            filename[5] = '.';
            filename[6] = 't';
            filename[7] = 'x';
            filename[8] = 't';
            filename[9] = '\0';
            outtextxy(0, 20 + 2 * dey, L"Please choose the five options below\n");
            choice_1(20, 20 + 4 * dey, 20 + textwidth(L"1.maze1"), 20 + 5 * dey);
            choice_2(20, 20 + 6 * dey, 20 + textwidth(L"2.maze2"), 20 + 7 * dey);
            choice_3(20, 20 + 8 * dey, 20 + textwidth(L"3.maze3"), 20 + 9 * dey);
            choice_4(20, 20 + 10 * dey, 20 + textwidth(L"4.maze4"), 20 + 11 * dey);
            choice_5(20, 20 + 12 * dey, 20 + textwidth(L"5.maze5"), 20 + 13 * dey);
            do {
                tempjudge = 0;
                if (MouseHit())
                {
                    MOUSEMSG msg = GetMouseMsg();
                    if (msg.uMsg == WM_LBUTTONDOWN)
                    {
                        if (msg.x >= 20 && msg.x < (20 + textwidth(L"1.maze1")) && msg.y >= 20 + 4 * dey && msg.y <= 20 + 5 * dey)
                        {
                            choice = 1;
                            tempjudge = 1;
                        }
                        if (msg.x >= 20 && msg.x < (20 + textwidth(L"2.maze2")) && msg.y >= 20 + 6 * dey && msg.y <= 20 + 7 * dey)
                        {
                            choice = 2;
                            tempjudge = 1;
                        }
                        if (msg.x >= 20 && msg.x < (20 + textwidth(L"3.maze3")) && msg.y >= 20 + 8 * dey && msg.y <= 20 + 9 * dey)
                        {
                            choice = 3;
                            tempjudge = 1;
                        }
                        if (msg.x >= 20 && msg.x < (20 + textwidth(L"4.maze4")) && msg.y >= 20 + 10 * dey && msg.y <= 20 + 11 * dey)
                        {
                            choice = 4;
                            tempjudge = 1;
                        }
                        if (msg.x >= 20 && msg.x < (20 + textwidth(L"5.maze5")) && msg.y >= 20 + 12 * dey && msg.y <= 20 + 13 * dey)
                        {
                            choice = 5;
                            tempjudge = 1;
                        }
                    }
                }
            } while (tempjudge != 1);
            if (choice == 1)
                filename[4] = '1';
            else if (choice == 2)
                filename[4] = '2';
            else if (choice == 3)
                filename[4] = '3';
            else if (choice == 4)
                filename[4] = '4';
            else if (choice == 5)
                filename[4] = '5';
            if ((infile = fopen(filename, "r+")) == NULL)
            {
                outtextxy(0, 20 + 2 * dey, L"Can not open the file\n");
                exit(EXIT_FAILURE);
            }
            //�洢����--ֻ�ܴ���ԭ�ĵ�ĩβ
            fseek(infile, 0L, SEEK_END);
            putc('\n', infile);
            for (int i = 0; i < dX; i++)
            {
                for (int p = 0; p < dX; p++)
                {
                    fprintf(infile, "%d ", map[i][p]);
                }
                putc('\n', infile);
            }
            if (fclose(infile) != 0)
                outtextxy(0, 20 + 2 * dey, L"Can not open the file\n");
        }
        Sleep(100);
        cleardevice();//����
        loadimage(NULL, _T("sunset.jpg"));
        imageprint(map);//��ӡ�Թ�
        if (choice != 20)
            choice = 1;//���յ�����ͳһΪ���֣����������Ϸ�������ж�

        wchar_t guide2[] = L"Click your choice 1/2/3 to solve the maze:";
        outtextxy(dX * size - textwidth(guide2) / 2, 20 + (dY + 2) * size, guide2);
        buttom_6(dX * size - textwidth(L"1.Type in coordinates to control"), 20 + (dY + 2) * size + 4 * dey, dX * size + textwidth(L"1.Type in coordinates to control"), 20 + (dY + 2) * size + 5 * dey);
        buttom_7(dX * size - textwidth(L"2.Use w/s/a/d to control"), 20 + (dY + 2) * size + 6 * dey, dX * size + textwidth(L"2.Use w/s/a/d to control"), 20 + (dY + 2) * size + 7 * dey);
        buttom_8(dX * size - textwidth(L"3.Click to control"), 20 + (dY + 2) * size + 8 * dey, dX * size + textwidth(L"3.Click to control"), 20 + (dY + 2) * size + 9 * dey);
        int tempjudge2;
        do {
            tempjudge2 = 0;
            if (MouseHit())
            {
                MOUSEMSG msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN)
                {
                    if (msg.x >= dX * size - textwidth(L"1.Type in coordinates to control") && msg.x < (dX * size + textwidth(L"1.Type in coordinates to control")) && msg.y >= 20 + (dY + 2) * size + 4 * dey && msg.y <= 20 + (dY + 2) * size + 5 * dey)
                    {
                        solution = 1;
                        tempjudge2 = 1;
                    }
                    if (msg.x >= dX * size - textwidth(L"2.Use w/s/a/d to control") && msg.x < (dX * size + textwidth(L"2.Use w/s/a/d to control")) && msg.y >= 20 + (dY + 2) * size + 6 * dey && msg.y <= 20 + (dY + 2) * size + 7 * dey)
                    {
                        solution = 2;
                        tempjudge2 = 1;
                    }
                    if (msg.x >= dX * size - textwidth(L"3.Click to control") && msg.x < (dX * size + textwidth(L"3.Click to control")) && msg.y >= 20 + (dY + 2) * size + 8 * dey && msg.y <= 20 + (dY + 2) * size + 9 * dey)
                    {
                        solution = 3;
                        tempjudge2 = 1;
                    }
                }
            }
        } while (tempjudge2 != 1);//�ж��Ƿ������˰�ť
        cleardevice();
        loadimage(NULL, _T("sunset.jpg"));
        imageprint(map);

        int x, y;
        int dx, dy;//����ı���
        ExMessage keymsg;
        int judgeju = 0;
        switch (solution) {
        case 1://���������Թ�
            bool isok1, isok2;
            map[1][0] = 3;
            x = 0;
            y = 1;
            imageprint(map);
            do {

                BeginBatchDraw();
                imageprint(map);
                FlushBatchDraw();
                BeginBatchDraw();
                outtextxy(0, 95 + ROW * size, L"Click the Cancel button or X to end the game early.\n");
                FlushBatchDraw();
                wchar_t s[10];
                isok1 = InputBox(s, 10, L"Please enter the change in x.\n X and Y can't become 1 or -1 at the same time.", 0, 0, 0, 0, false);
                dx = _wtoi(s);
                if (isok1 == false) {
                    break;
                }
                isok2 = InputBox(s, 10, L"Please enter the change in y.\n X and Y can't become 1 or -1 at the same time.", 0, 0, 0, 0, false);
                dy = _wtoi(s);
                if (isok2 == false) {
                    break;
                }
                if (x == 0 && y == 1 && dx == -1) {
                    BeginBatchDraw();
                    outtextxy(0, 65 + ROW * size, L"It breaks the rule.");
                    FlushBatchDraw();
                    continue;
                }
                if ((dy != 1 && dy != 0 && dy != -1) || (dx != 1 && dx != 0 && dx != -1) || (dx == 0 && dy == 0) || (dx != 0 && dy != 0)) {
                    BeginBatchDraw();
                    outtextxy(0, 65 + ROW * size, L"It breaks the rule.");
                    FlushBatchDraw();
                    continue;
                }
                cleardevice();
                loadimage(NULL, _T("sunset.jpg"));
                if (map[y + dy][x + dx] == 0) {
                    BeginBatchDraw();
                    outtextxy(0, 80 + ROW * size, L"Wall!!!");
                    FlushBatchDraw();
                    continue;
                }
                cleardevice();
                loadimage(NULL, _T("sunset.jpg"));
                map[y][x] = 1;
                map[y + dy][x + dx] = 3;
                y = y + dy;
                x = x + dx;

                EndBatchDraw();
            } while (x != COL - 1);

            if (isok1 != false || isok2 != false) {
                outtextxy(20, 20 + (dY + 2) * size, L"Congratulations!You've completed the maze!");
            }
            if (isok1 == false || isok2 == false) {
                cleardevice();
                loadimage(NULL, _T("sunset.jpg"));
                EndBatchDraw();
            }
           
            break;
        case 3://��������Թ�
            map[1][0] = PACE;
            judge = 0;
            cleardevice();
            loadimage(NULL, _T("sunset.jpg"));
            imageprint2(map);

            do {

                map[r][c] = 1;//�洢��һ�β����ĵ�ַ���������ǰһ�ε�λ�ñ�ǣ�Ϊʵʱ��ʾ�Թ��е�λ�÷���
                //��ʼ������������
                if (MouseHit()) {
                    MOUSEMSG msg = GetMouseMsg();
                    switch (msg.uMsg) {
                    case WM_LBUTTONDOWN:
                        //�����������������Ԫ�صĹ�ϵ��
                        int row = (msg.y - 20) / size;
                        int col = (msg.x - 20) / size;
                        if (row > ROW || col > COL) {
                            outtextxy(0, 65 + ROW * size, L"It breaks the rule.");
                            continue;
                        }

                        if (row < ROW && col < COL) {
                            //һ��һ���ߣ������Զ��ߣ�
                            int dc = (col - c);
                            int dr = (row - r);
                            if (dc > 1 || dr > 1 || dr < -1 || dc < -1 || (dc == 1 && dr == 1) || (dc == -1 && dr == 1) || (dc == -1 && dr == -1) || (dc == 1 && dr == -1)) {

                                outtextxy(0, 65 + ROW * size, L"It breaks the rule.");


                                continue;
                            }

                            //�����Ե��ǽ��
                            if (map[row][col] != 1) {

                                outtextxy(0, 65 + ROW * size, L"It breaks the rule.");


                                continue;
                            }

                            if (map[row][col] == 1) {
                                map[row][col] = PACE;
                                r = row;
                                c = col;
                            }
                        }
                        BeginBatchDraw();//��������ͼ�λ�����ĺ���
                        cleardevice();
                        loadimage(NULL, _T("sunset.jpg"));
                        imageprint2(map);
                        FlushBatchDraw();
                        EndBatchDraw();//��������ͼ�λ�����


                        //�ж��Ƿ��߳��Թ���Ӯ����Ϸ��
                        if (msg.x >= 20 + (COL - 1) * size && msg.x <= 20 + COL * size && msg.y >= 20 + (ROW - 2) * size && msg.y <= 20 + (ROW - 1) * size) {
                            judge = 2;
                            break;
                        }

                    }
                    //�������Թ��׶Σ�
                    if (judge == 2) {
                        break;
                    }
                }
            } while (1);//��ѭ�������ڷ���ѭ��
            outtextxy(20, 20 + (dY + 2) * size, L"Congratulations!You've completed the maze!");
            break;
        case 2://"w""s""a""d"������������Թ�
            switch (choice) {
            case 1:
                x = 1;
                y = 0;
                break;
            case 2:
                x = 2;
                y = 1;
                break;
            default:
                x = 1;
                y = 0;
            }//����ж�
            map[x][y] = PACE;

            imageprint(map);
            map[x][y] = PATH;//��ʼ����ͼ
            int userkey = 72;//���ռ��̲���
            int corrector = 1;//ײǽ�ж�
            int round = 0;
            outtextxy(20, 20 + (dY + 2) * size, L"Press w/s/a/d to move ; Press q to quit...");
            outtextxy(20, 20 + (dY + 4) * size, L"Now press \"d\" or Right to start:");
            do {
                judgeju = 0;
                while (peekmessage(&keymsg, EM_KEY) && keymsg.message == WM_KEYDOWN)
                {
                    switch (keymsg.vkcode) {
                    case'd':
                    case'D':
                    case 77:
                        judgeju = 1;
                        break;
                    }
                }
            } while (judgeju == 0);

            BeginBatchDraw();
            cleardevice();
            loadimage(NULL, _T("sunset.jpg"));

            right(&x, &y, map);
            imageprint(map);
            EndBatchDraw();
            
            //��ֹ��һ��ֱ���Ƴ���ͼ
            do {
                
                while (peekmessage(&keymsg, EM_KEY) && keymsg.message == WM_KEYDOWN)
                {
                    switch (keymsg.vkcode) {
                    case'w':
                    case'W':
                    case 72:
                        if (map[x - 1][y] == BARRICADE)
                        {
                            corrector = 0;

                            BeginBatchDraw();
                            outtextxy(20, 20 + (dY + 2) * size, L"Wall!");
                            FlushBatchDraw();
                            continue;
                        }
                        up(&x, &y, map);
                        break;
                    case's':
                    case'S':
                    case 80:
                        if (map[x + 1][y] == BARRICADE)
                        {
                            corrector = 0;

                            BeginBatchDraw();
                            outtextxy(20, 20 + (dY + 2) * size, L"Wall!");
                            FlushBatchDraw();
                            continue;
                        }
                        down(&x, &y, map);
                        break;
                    case'a':
                    case'A':
                    case 75:
                        if (map[x][y - 1] == BARRICADE || y == 1)
                        {
                            corrector = 0;
                            BeginBatchDraw();
                            outtextxy(20, 20 + (dY + 2) * size, L"Wall!");
                            FlushBatchDraw();

                            continue;
                        }
                        left(&x, &y, map);
                        break;
                    case'd':
                    case'D':
                    case 77:
                        if (map[x][y + 1] == BARRICADE)
                        {
                            corrector = 0;
                            BeginBatchDraw();
                            outtextxy(20, 20 + (dY + 2) * size, L"Wall!");
                            FlushBatchDraw();

                            continue;
                        }
                        right(&x, &y, map);
                        break;
                    case'q':
                    case'Q':
                        y = dY - (choice == 1 ? 1 : 2);
                        corrector = 20;
                        break;//��q�˳�
                    }
                }//���̿����ƶ�
                if (corrector != 20)
                {
                    BeginBatchDraw();
                    cleardevice();
                    loadimage(NULL, _T("sunset.jpg"));
                    imageprint(map);
                    if (corrector == 0)
                        outtextxy(20, 20 + (dY + 2) * size, L"Wall!");
                    FlushBatchDraw();
                    Sleep(150);
                    EndBatchDraw();

                    corrector = 1;//����ײǽ�ж�
                }
            } while (y != dY - (choice == 1 ? 1 : 2));//�յ��ж�

            if (corrector != 20)
                outtextxy(20, 20 + (dY + 2) * size, L"Congratulations!You've completed the maze!");
            break;
        }
        outtextxy(20, 20 + (dY + 2) * size + dey * 3, L"Try again?");
        buttom_9(20, 20 + (dY + 2) * size + 5 * dey, 20 + textwidth(L"Yes..."), 20 + (dY + 2) * size + 6 * dey);
        buttom_10(20, 20 + (dY + 2) * size + 7 * dey, 20 + textwidth(L"No..."), 20 + (dY + 2) * size + 8 * dey);
        free(map);
        int tempjudge3;
        do {
            tempjudge3 = 0;
            if (MouseHit())
            {
                MOUSEMSG msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN)
                {
                    if (msg.x >= 20 && msg.x < (20 + textwidth(L"Yes...")) && msg.y >= 20 + (dY + 2) * size + 5 * dey && msg.y <= 20 + (dY + 2) * size + 6 * dey)
                    {
                        ques = 1;
                        tempjudge3 = 1;
                    }
                    if (msg.x >= 20 && msg.x < (20 + textwidth(L"No...")) && msg.y >= 20 + (dY + 2) * size + 7 * dey && msg.y <= 20 + (dY + 2) * size + 8 * dey)
                    {
                        ques = 0;
                        tempjudge3 = 1;
                    }
                }
            }
        } while (tempjudge3 != 1);
        cleardevice();
        loadimage(NULL, _T("sunset.jpg"));
    } while (ques != 0);//��ѡ��ѭ����Ϸ
    closegraph();
    mciSendString(_T("close bkmusic"), NULL, 0, NULL);
    system("pause");
    return 0;
}
int pathspawn_0(void)
{
    int spawn;
    srand(time(NULL));
    spawn = rand() % 2;
    return spawn;
}//�����
void copy(int** a, int** target)
{
    for (int i = 0; i < dY; i++)
    {
        for (int j = 0; j < dY; j++)
        {
            target[i][j] = a[i][j];
        }
    }
}//����
void eleprint(int** puzzle)
{
    for (int i = 0; i < dY; i++)
    {
        for (int j = 0; j < dY; j++)
        {
            if (puzzle[i][j] == BARRICADE)
                printf("%c%c", (char)219, (char)219);
            else
            {
                if (puzzle[i][j] == PACE)
                    printf("**");
                else
                    printf("  ");
            }
        }
        printf("\n");
    }
}//��ӡ
int sumloc(int i, int j, int** a)
{
    return a[i - 1][j] + a[i + 1][j] + a[i][j + 1] + a[i][j - 1];
}//ͨ·�ж�
void spawnpuzzle_create(int** maze, int x1, int y1, int x2, int y2) {//��ǽ�������Թ�
    if (x2 - x1 < 2 || y2 - y1 < 2) {
        return;
    }
    int x = x1 + 1 + rand() % (x2 - x1 - 1);
    int y = y1 + 1 + rand() % (y2 - y1 - 1);
    for (int i = x1; i <= x2; i++) maze[i][y] = BARRICADE1;
    for (int i = y1; i <= y2; i++) maze[x][i] = BARRICADE1;//��ΧһȦ��Ϊǽ
    spawnpuzzle_create(maze, x1, y1, x - 1, y - 1);
    spawnpuzzle_create(maze, x + 1, y + 1, x2, y2);
    spawnpuzzle_create(maze, x + 1, y1, x2, y - 1);
    spawnpuzzle_create(maze, x1, y + 1, x - 1, y2);//�ĸ�����ݹ飬ͨ����ǽ���Ϸָ�
    int r[4] = { 0 };
    r[rand() % 4] = 1;
    for (int i = 0; i < 4; i++) {
        if (r[i] == 0) {
            int rx = x;
            int ry = y;
            switch (i) {
            case 0:
                do {
                    rx = x1 + rand() % (x - x1);
                } while (sumloc(rx, ry, maze) > 2 * BARRICADE1);
                break;
            case 1:
                do {
                    ry = y + 1 + rand() % (y2 - y);
                } while (sumloc(rx, ry, maze) > 2 * BARRICADE1);
                break;
            case 2:
                do {
                    rx = x + 1 + rand() % (x2 - x);
                } while (sumloc(rx, ry, maze) > 2 * BARRICADE1);
                break;
            case 3:
                do {
                    ry = y1 + rand() % (y - y1);
                } while (sumloc(rx, ry, maze) > 2 * BARRICADE1);
                break;
            default:
                break;
            }//�ų��ڴ����ܺ���ǽ�����ȡ��
            maze[rx][ry] = PATH1;//����һ��ȥ���ĵ��ڿ��γ�ͨ·
        }
    }
}
void spawnpuzzle_break(int** puzzle, int x, int y)//��·�������Թ�
{
    puzzle[x][y] = PATH;
    int direction[4][2] = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };//������������ڵķ���
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp = direction[0][0];
        direction[0][0] = direction[r][0];
        direction[r][0] = temp;
        temp = direction[0][1];
        direction[0][1] = direction[r][1];
        direction[r][1] = temp;
    }//�������
    for (int i = 0; i < 4; i++) {
        int dx = x;
        int dy = y;
        int range = 1;
        if (rank == 0)
            range += 0;
        else
            range += rand() % range;//һ���ڵĸ����������Թ����Ӷ�
        while (range > 0)
        {
            dx += direction[i][0];
            dy += direction[i][1];
            if (puzzle[dx][dy] == PATH) {
                break;
            }//��ֹ�ظ��ڽ�����ѭ��
            int count = 0;
            for (int j = dx - 1; j < dx + 2; j++)
            {
                for (int k = dy - 1; k < dy + 2; k++)
                {
                    if (abs(j - dx) + abs(k - dy) == 1 && puzzle[j][k] == PATH)
                        count++;
                }
            }//ͨ����Χͨ·����������ж��õ��Ƿ�����
            if (count > 1)
                break;//�����ھ�����ѭ��
            --range;
            puzzle[dx][dy] = PATH;//��·
        }
        if (range <= 0)
            spawnpuzzle_break(puzzle, dx, dy);//�ݹ�
    }
}
void up(int* x, int* y, int** a)
{
    a[*x][*y] = PATH;
    a[*x - 1][*y] = PACE;
    (*x)--;
}//�����ƶ�
void down(int* x, int* y, int** a)
{
    a[*x][*y] = PATH;
    a[*x + 1][*y] = PACE;
    (*x)++;
}//�����ƶ�
void left(int* x, int* y, int** a)
{
    a[*x][*y] = PATH;
    a[*x][*y - 1] = PACE;
    (*y)--;
}//�����ƶ�
void right(int* x, int* y, int** a)
{
    a[*x][*y] = PATH;
    a[*x][*y + 1] = PACE;
    (*y)++;
}//�����ƶ�
char trans(int x)
{
    switch (x) {
    case BARRICADE:
        return (char)219;
        break;
    case PATH:
        return ' ';
        break;
    case PACE:
        return'*';
        break;
    }
}//��0,1ת��Ϊ�����Թ����ַ�
void show(void)//˫������ʾ
{
    showcircle = !showcircle;//�ı�ʹ�õĻ�����
    if (showcircle)
    {
        houtpoint = &hOutput;
    }
    else
    {
        houtpoint = &hOutBuf;
    }//ָ��ָ����ж�
    for (row = 0; row < LENGTH; row++)
    {
        coord.X = 0;
        coord.Y = row;
        WriteConsoleOutputCharacterA(*houtpoint, (char*)data[row], (2 * LENGTH), coord, &bytes);
    }//��draw���Ƶ�������ʾ�ڻ�����
    SetConsoleActiveScreenBuffer(*houtpoint);//����ָ��ָ��Ļ�����
    Sleep(200);//��ֹ����
}
void draw(int** map, int a)//˫�������
{
    memset(data, ' ', (LENGTH + 1) * (2 * LENGTH));//����
    for (row = 0; row < LENGTH; row++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            data[row][2 * j] = trans(map[row][j]);
            data[row][2 * j + 1] = trans(map[row][j]);
        }
    }//����ͼ��������ݻ���
    data[LENGTH + 2][0] = 'W';
    data[LENGTH + 2][1] = 'a';
    data[LENGTH + 2][2] = 'l';
    data[LENGTH + 2][3] = 'l';
    data[LENGTH + 2][4] = '!';//ײǽ��Ϣ
}
void map1(int arr[][COL], int row) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < COL; j++) {
            if (arr[i][j] == 0) {
                setlinecolor(BLUE);
                setfillcolor(GREEN);
                fillrectangle(j * size, i * size, (j + 1) * size, (i + 1) * size);
            }
            if (arr[i][j] == 1) {
                setlinecolor(BLUE);
                setfillcolor(RED);
                fillrectangle(j * size, i * size, (j + 1) * size, (i + 1) * size);
            }
            if (arr[i][j] == 2) {
                setlinecolor(BLUE);
                setfillcolor(BROWN);
                fillcircle(j * size + size / 2, i * size + size / 2, size / 2);
            }
        }
    }
}
void buttom2(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"quit the game" };
    setlinecolor(BLUE);
    setfillcolor(YELLOW);
    fillrectangle(left, top, right, bottom);
    settextcolor(RED);
    settextstyle(size / 2, size / 2, L"Dotum");
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + 0, (bottom - top - height) / 2 + top, text2);
}
void rules(void) {
    printf("The starting and ending points have been defined.\n");
    printf("No other points can be chosen as the starting and ending points.\n");
    printf("Click the end point to finish the maze drawing.\n");
}
void buttom_1(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"1.Create Wall" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void buttom_2(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"2.Create Path" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void buttom_3(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"3.Type in the coordinates" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void buttom_4(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"4.Click to create path" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void buttom_5(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"5.Import from file" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void buttom_6(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"1.Type in coordinates to control" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void buttom_7(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"2.Use w/s/a/d to control" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void buttom_9(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"YES..." };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void buttom_10(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"No..." };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void buttom_8(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"3.Click to control" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void choice_1(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"1.maze1" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void choice_2(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"2.maze2" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void choice_3(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"3.maze3" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void choice_4(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"4.maze4" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void choice_5(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"5.maze5" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void judge_1(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"Yes" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
void judge_2(int left, int top, int right, int bottom) {
    wchar_t text2[] = { L"No" };
    setlinecolor(DARKGRAY);
    setfillcolor(DARKGRAY);
    fillrectangle(left, top, right, bottom);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int wide = textwidth(text2);
    int height = textheight(text2);
    outtextxy((right - left - wide) / 2 + left, (bottom - top - height) / 2 + top, text2);
}
//��ť����
void imageprint2(int** map)
{
    for (int i = 0; i < dX; i++)
    {
        for (int j = 0; j < dY; j++)
        {
            switch (map[i][j])
            {
            case(BARRICADE):
                setfillcolor(WHITE);
                solidrectangle(20 + j * size, 20 + i * size, 20 + (j + 1) * size, 20 + (i + 1) * size);
                break;
            case(PATH):
                setfillcolor(LIGHTGRAY);
                solidrectangle(20 + j * size, 20 + i * size, 20 + (j + 1) * size, 20 + (i + 1) * size);
                break;
            case(PACE):
                setfillcolor(DARKGRAY);
                solidrectangle(20 + j * size, 20 + i * size, 20 + (j + 1) * size, 20 + (i + 1) * size);
                break;
            }
        }
    }
}
void imageprint(int** map)
{
    for (int i = 0; i < dX; i++)
    {
        for (int j = 0; j < dY; j++)
        {
            switch (map[i][j])
            {
            case(BARRICADE):
                setfillcolor(WHITE);
                solidrectangle(dX * size / 2 + j * size, 50 + i * size, dX * size / 2 + (j + 1) * size, 50 + (i + 1) * size);
                break;
            case(PATH):
                setfillcolor(LIGHTGRAY);
                solidrectangle(dX * size / 2 + j * size, 50 + i * size, dX * size / 2 + (j + 1) * size, 50 + (i + 1) * size);
                break;
            case(PACE):
                setfillcolor(DARKGRAY);
                solidrectangle(dX * size / 2 + j * size, 50 + i * size, dX * size / 2 + (j + 1) * size, 50 + (i + 1) * size);
                break;
            }
        }
    }
}//ͼ�λ������ӡ�Թ�