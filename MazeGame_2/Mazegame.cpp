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
#define dY dX//迷宫的长和宽
#define PATH 1
#define BARRICADE 0
#define PACE 3//代表通路、墙和路径
#define LENGTH dX
#define size 25
#define ROW dX
#define COL dY
static const int L = dY;
static const int PATH1 = 0;
static const int BARRICADE1 = 1;//"造墙"法中对应的墙和路
static int rank = 0;//控制迷宫复杂度
int row;
HANDLE hOutput, hOutBuf;
HANDLE* houtpoint;
COORD coord = { 0,0 };
DWORD bytes = 0;//屏幕缓冲区，防止闪屏
unsigned char data[LENGTH + 3][2 * LENGTH];//绘制缓冲区大小
bool showcircle = false;//双缓冲控制
typedef struct
{
    int x;
    int y;
}coordinate;//坐标
int pathspawn_0(void);//随机生成路和墙
int sumloc(int i, int j, int** a);//判断某点四周路径条数，防止生成循环
char trans(int x);//将0、1转化为对应的墙和路
void copy(int** a, int** target);//复制数组
void eleprint(int** puzzle);//打印迷宫
void spawnpuzzle_break(int** puzzle, int x, int y);//"挖路法"生成迷宫
void spawnpuzzle_create(int** maze, int x1, int y1, int x2, int y2);//"造墙法"生成迷宫
void up(int* x, int* y, int** a);
void down(int* x, int* y, int** a);
void left(int* x, int* y, int** a);
void right(int* x, int* y, int** a);//按四个方向移动的函数
void show(void);//双缓冲显示
void draw(int** map, int a);//双缓冲绘制
void map1(int arr[][COL], int row);//绘制地图
void rules(void);//用于反复显示规则
void buttom2(int left, int top, int right, int bottom);//显示退出游戏的按钮
void buttom_1(int left, int top, int right, int bottom);
void buttom_2(int left, int top, int right, int bottom);
void buttom_3(int left, int top, int right, int bottom);
void buttom_4(int left, int top, int right, int bottom);
void buttom_5(int left, int top, int right, int bottom);
void buttom_6(int left, int top, int right, int bottom);
void buttom_7(int left, int top, int right, int bottom);
void buttom_8(int left, int top, int right, int bottom);
void buttom_9(int left, int top, int right, int bottom);
void buttom_10(int left, int top, int right, int bottom);//定义菜单按钮
void imageprint(int** map);
void imageprint2(int** map);//图形化界面绘制迷宫
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
    );//定义两个可写缓冲区
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = 0;
    cci.dwSize = 1;
    SetConsoleCursorInfo(hOutput, &cci);
    SetConsoleCursorInfo(hOutBuf, &cci);//隐藏缓冲区光标
    srand(time(NULL));//随机种子
    SetConsoleOutputCP(437);//英文输出以使用扩展ASCIII码绘制迷宫
    int choice;//迷宫生成方法的选择
    int solution;//走迷宫方法的选择
    int arr[ROW][COL] = { 0 };//建立数组
    //标记起点和终点：
    arr[1][0] = 1;
    arr[ROW - 2][COL - 1] = 1;
    int play;
    int judge = 0;//用于结束某一游戏阶段的变量
    int r = 1, c = 0;//确定前一次点击的数组位置
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
        }//分配内存，定义二维数组以储存迷宫的不同状态
        wchar_t welcome[] = L"Maze Game";
        int dey = 30;//判断字符高度
        outtextxy(dX * size - textwidth(welcome) / 2, 20, welcome);
        settextstyle(25, 0, _T("Bookman Old Style"));
        wchar_t guide1[] = L"Click your choice 1 / 2 / 3 / 4 / 5 to create the maze:";
        outtextxy(dX * size - textwidth(guide1) / 2, 116, guide1);
        buttom_1(dX * size - textwidth(L"1.Create Wall") / 2, 156 + dey, dX * size + textwidth(L"1.Create Wall") / 2, 186 + dey);
        buttom_2(dX * size - textwidth(L"2.Create Path") / 2, 226 + dey, dX * size + textwidth(L"2.Create Path") / 2, 256 + dey);
        buttom_3(dX * size - textwidth(L"3.Type in the coordinates") / 2, 296 + dey, dX * size + textwidth(L"3.Type in the coordinates") / 2, 326 + dey);
        buttom_4(dX * size - textwidth(L"4.Click to create path") / 2, 366 + dey, dX * size + textwidth(L"4.Click to create path") / 2, 396 + dey);
        buttom_5(dX * size - textwidth(L"5.Import from file") / 2, 436 + dey, dX * size + textwidth(L"5.Import from file") / 2, 466 + dey);//绘制图形化界面，生成按钮
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
        } while (tempjudge != 1);//判定按钮点击
       
        int sx = 0, sy = 0;

        FILE* infile;
        cleardevice();
        loadimage(NULL, _T("sunset.jpg"));
        int x1 = 0, y1 = 0;
        switch (choice) {
        case 1://生成墙
            for (int i = 0; i < L; i++)
            {
                for (int j = 0; j < L; j++)
                {
                    if (i == 0 || i == L - 1 || j == 0 || j == L - 1)
                        Maze[i][j] = BARRICADE1;
                }
            }//外围一圈设成墙开始递归
            spawnpuzzle_create(Maze, 1, 1, L - 2, L - 2);
            Maze[1][0] = PATH1;
            Maze[L - 2][L - 1] = PATH1;//起点和终点
            for (int i = 0; i < L; i++)
            {
                for (int j = 0; j < L; j++)
                {
                    if (Maze[i][j] == BARRICADE1)
                        Maze[i][j] = BARRICADE;
                    else
                        Maze[i][j] = PATH;
                }
            }//将标准统一方便操作
            copy(Maze, map);//将迷宫导入代表走迷宫的地图的数组
            for (int i = 0; i < L; i++)
                free(Maze[i]);

            break;
        case 2://生成路
            for (int i = 0; i < (dX + 2); i++)
            {
                for (int j = 0; j < (dY + 2); j++)
                {
                    if (i == 0 || i == (dX + 2) - 1 || j == 0 || j == (dY + 2) - 1)
                        puzzle[i][j] = PATH;
                }
            }//外围一圈设成路防止挖穿
            spawnpuzzle_break(puzzle, 2, 2);
            puzzle[2][1] = PATH;
            for (int i = (dX + 2) - 3; i >= 0; i--)
            {
                if (puzzle[i][(dY + 2) - 3] == PATH)
                {
                    puzzle[i][(dY + 2) - 2] = PATH;
                    break;
                }
            }//应对终点不唯一的情况，确保有路可走

            for (int i = 0; i < dX; i++)
            {
                for (int j = 0; j < dY; j++)
                    map[i][j] = puzzle[i + 1][j + 1];
            }//将迷宫导入代表走迷宫的地图的数组，并舍去其中外面一圈保护数组
            for (int i = 0; i < (dX + 2); i++) free(puzzle[i]);

            break;
        case 3://输坐标生成迷宫

            for (int i = 0; i < dX; i++)
            {
                for (int j = 0; j < dY; j++)
                {
                    Maze[i][j] = BARRICADE;
                }
            }//初始化为墙
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
                }//打印可视化界面坐标轴
                settextstyle(25, 0, _T("Bookman Old Style"));
                EndBatchDraw();
                Maze[1][0] = PATH;//起点
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
        case 4://鼠标点击生成迷宫
            for (int i = 0; i < dX; i++) {
                for (int j = 0; j < dY; j++) {
                    Maze[i][j] = BARRICADE;
                }
            }
            Maze[1][0] = PATH;
            Maze[ROW - 2][COL - 1] = PATH;
            do {
                //显示规则：
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
                        //建立鼠标坐标与数组元素的关系：
                        int row1 = (msg.y - 20) / size;
                        int col1 = (msg.x - 20) / size;
                        cleardevice();
                        loadimage(NULL, _T("sunset.jpg"));
                        //以下四个条件用于确定唯一的起点和终点：
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


                        //绘制迷宫：
                        if (row1 < ROW && col1 < COL) {
                            Maze[row1][col1] = PATH;
                        }
                        //判断是否结束绘制：
                        if (msg.x >= 20 + (COL - 1) * size && msg.x <= 20 + COL * size && msg.y >= 20 + (ROW - 2) * size && msg.y <= 20 + (ROW - 1) * size) {
                            judge = 1;
                            break;
                        }
                        break;
                    }
                    //结束绘制：
                    if (judge == 1) {
                        break;
                    }
                }


            } while (1);


            EndBatchDraw();
            copy(Maze, map);

            imageprint2(map);

            break;

                 //文件导入生成迷宫
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
        imageprint(map);//打印迷宫
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
            //存储代码--只能存在原文档末尾
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
        cleardevice();//清屏
        loadimage(NULL, _T("sunset.jpg"));
        imageprint(map);//打印迷宫
        if (choice != 20)
            choice = 1;//将终点情形统一为两种，方便后续游戏结束的判定

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
        } while (tempjudge2 != 1);//判定是否点击到了按钮
        cleardevice();
        loadimage(NULL, _T("sunset.jpg"));
        imageprint(map);

        int x, y;
        int dx, dy;//坐标改变量
        ExMessage keymsg;
        int judgeju = 0;
        switch (solution) {
        case 1://输坐标走迷宫
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
        case 3://鼠标点击走迷宫
            map[1][0] = PACE;
            judge = 0;
            cleardevice();
            loadimage(NULL, _T("sunset.jpg"));
            imageprint2(map);

            do {

                map[r][c] = 1;//存储上一次操作的地址，用于清空前一次的位置标记，为实时显示迷宫中的位置服务
                //开始进行鼠标操作：
                if (MouseHit()) {
                    MOUSEMSG msg = GetMouseMsg();
                    switch (msg.uMsg) {
                    case WM_LBUTTONDOWN:
                        //建立鼠标坐标与数组元素的关系：
                        int row = (msg.y - 20) / size;
                        int col = (msg.x - 20) / size;
                        if (row > ROW || col > COL) {
                            outtextxy(0, 65 + ROW * size, L"It breaks the rule.");
                            continue;
                        }

                        if (row < ROW && col < COL) {
                            //一格一格走，不可以多走：
                            int dc = (col - c);
                            int dr = (row - r);
                            if (dc > 1 || dr > 1 || dr < -1 || dc < -1 || (dc == 1 && dr == 1) || (dc == -1 && dr == 1) || (dc == -1 && dr == -1) || (dc == 1 && dr == -1)) {

                                outtextxy(0, 65 + ROW * size, L"It breaks the rule.");


                                continue;
                            }

                            //不可以点击墙：
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
                        BeginBatchDraw();//用于清理图形化界面的函数
                        cleardevice();
                        loadimage(NULL, _T("sunset.jpg"));
                        imageprint2(map);
                        FlushBatchDraw();
                        EndBatchDraw();//用于清理图形化界面


                        //判断是否走出迷宫，赢得游戏：
                        if (msg.x >= 20 + (COL - 1) * size && msg.x <= 20 + COL * size && msg.y >= 20 + (ROW - 2) * size && msg.y <= 20 + (ROW - 1) * size) {
                            judge = 2;
                            break;
                        }

                    }
                    //结束走迷宫阶段：
                    if (judge == 2) {
                        break;
                    }
                }
            } while (1);//死循环，用于反复循环
            outtextxy(20, 20 + (dY + 2) * size, L"Congratulations!You've completed the maze!");
            break;
        case 2://"w""s""a""d"或方向键控制走迷宫
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
            }//起点判定
            map[x][y] = PACE;

            imageprint(map);
            map[x][y] = PATH;//初始化地图
            int userkey = 72;//接收键盘操作
            int corrector = 1;//撞墙判定
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
            
            //防止第一步直接移出地图
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
                        break;//按q退出
                    }
                }//键盘控制移动
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

                    corrector = 1;//重置撞墙判定
                }
            } while (y != dY - (choice == 1 ? 1 : 2));//终点判定

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
    } while (ques != 0);//可选择循环游戏
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
}//随机数
void copy(int** a, int** target)
{
    for (int i = 0; i < dY; i++)
    {
        for (int j = 0; j < dY; j++)
        {
            target[i][j] = a[i][j];
        }
    }
}//复制
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
}//打印
int sumloc(int i, int j, int** a)
{
    return a[i - 1][j] + a[i + 1][j] + a[i][j + 1] + a[i][j - 1];
}//通路判定
void spawnpuzzle_create(int** maze, int x1, int y1, int x2, int y2) {//造墙法生成迷宫
    if (x2 - x1 < 2 || y2 - y1 < 2) {
        return;
    }
    int x = x1 + 1 + rand() % (x2 - x1 - 1);
    int y = y1 + 1 + rand() % (y2 - y1 - 1);
    for (int i = x1; i <= x2; i++) maze[i][y] = BARRICADE1;
    for (int i = y1; i <= y2; i++) maze[x][i] = BARRICADE1;//外围一圈设为墙
    spawnpuzzle_create(maze, x1, y1, x - 1, y - 1);
    spawnpuzzle_create(maze, x + 1, y + 1, x2, y2);
    spawnpuzzle_create(maze, x + 1, y1, x2, y - 1);
    spawnpuzzle_create(maze, x1, y + 1, x - 1, y2);//四个方向递归，通过造墙不断分割
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
            }//排除挖穿可能后在墙上随机取点
            maze[rx][ry] = PATH1;//将上一步去到的点挖开形成通路
        }
    }
}
void spawnpuzzle_break(int** puzzle, int x, int y)//挖路法生成迷宫
{
    puzzle[x][y] = PATH;
    int direction[4][2] = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };//创建数组代表挖的方向
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp = direction[0][0];
        direction[0][0] = direction[r][0];
        direction[r][0] = temp;
        temp = direction[0][1];
        direction[0][1] = direction[r][1];
        direction[r][1] = temp;
    }//方向随机
    for (int i = 0; i < 4; i++) {
        int dx = x;
        int dy = y;
        int range = 1;
        if (rank == 0)
            range += 0;
        else
            range += rand() % range;//一次挖的格数，控制迷宫复杂度
        while (range > 0)
        {
            dx += direction[i][0];
            dy += direction[i][1];
            if (puzzle[dx][dy] == PATH) {
                break;
            }//防止重复挖进入死循环
            int count = 0;
            for (int j = dx - 1; j < dx + 2; j++)
            {
                for (int k = dy - 1; k < dy + 2; k++)
                {
                    if (abs(j - dx) + abs(k - dy) == 1 && puzzle[j][k] == PATH)
                        count++;
                }
            }//通过周围通路方向的数量判定该点是否能挖
            if (count > 1)
                break;//不能挖就跳出循环
            --range;
            puzzle[dx][dy] = PATH;//挖路
        }
        if (range <= 0)
            spawnpuzzle_break(puzzle, dx, dy);//递归
    }
}
void up(int* x, int* y, int** a)
{
    a[*x][*y] = PATH;
    a[*x - 1][*y] = PACE;
    (*x)--;
}//向上移动
void down(int* x, int* y, int** a)
{
    a[*x][*y] = PATH;
    a[*x + 1][*y] = PACE;
    (*x)++;
}//向下移动
void left(int* x, int* y, int** a)
{
    a[*x][*y] = PATH;
    a[*x][*y - 1] = PACE;
    (*y)--;
}//向左移动
void right(int* x, int* y, int** a)
{
    a[*x][*y] = PATH;
    a[*x][*y + 1] = PACE;
    (*y)++;
}//向右移动
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
}//将0,1转化为代表迷宫的字符
void show(void)//双缓冲显示
{
    showcircle = !showcircle;//改变使用的缓冲区
    if (showcircle)
    {
        houtpoint = &hOutput;
    }
    else
    {
        houtpoint = &hOutBuf;
    }//指针指向的判定
    for (row = 0; row < LENGTH; row++)
    {
        coord.X = 0;
        coord.Y = row;
        WriteConsoleOutputCharacterA(*houtpoint, (char*)data[row], (2 * LENGTH), coord, &bytes);
    }//将draw绘制的内容显示在缓冲区
    SetConsoleActiveScreenBuffer(*houtpoint);//更换指针指向的缓冲区
    Sleep(200);//防止闪屏
}
void draw(int** map, int a)//双缓冲绘制
{
    memset(data, ' ', (LENGTH + 1) * (2 * LENGTH));//清屏
    for (row = 0; row < LENGTH; row++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            data[row][2 * j] = trans(map[row][j]);
            data[row][2 * j + 1] = trans(map[row][j]);
        }
    }//将地图数组的内容绘制
    data[LENGTH + 2][0] = 'W';
    data[LENGTH + 2][1] = 'a';
    data[LENGTH + 2][2] = 'l';
    data[LENGTH + 2][3] = 'l';
    data[LENGTH + 2][4] = '!';//撞墙信息
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
//按钮定义
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
}//图形化界面打印迷宫