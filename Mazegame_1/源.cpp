#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<Windows.h>
#include<conio.h>
#include<cstdio>
#include<cstdlib>
#include<graphics.h>
#define dX 8
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
    int** temp = (int**)malloc(dX * sizeof(int*));
    for (int i = 0; i < dX; i++)
    {
        temp[i] = (int*)calloc(dY, sizeof(int));
    }
    int** maze1 = (int**)malloc(ROW * sizeof(int*));
    for (int j = 0; j < COL; j++) {
        maze1[j] = (int*)calloc(COL, sizeof(int));
    }
    int** maze2 = (int**)malloc(ROW * sizeof(int*));
    for (int j = 0; j < COL; j++) {
        maze2[j] = (int*)calloc(COL, sizeof(int));
    }
    int** map = (int**)malloc(dX * sizeof(int*));
    for (int i = 0; i < dX; i++)
    {
        map[i] = (int*)calloc(dY, sizeof(int));
    }
    int** figure = (int**)malloc(dX * sizeof(int*));
    for (int i = 0; i < dX; i++)
    {
        figure[i] = (int*)calloc(dY, sizeof(int));
    }
    int** mapback = (int**)malloc(dX * sizeof(int*));
    for (int i = 0; i < dX; i++)
    {
        mapback[i] = (int*)calloc(dY, sizeof(int));
    }//分配内存，定义六个二维数组以储存迷宫的不同状态
    do {
        printf("Maze Game\n");
        printf("Type in your choice 1/2/3/4/5 to create the maze:\n1.Create wall\n2.Create path\n3.Type in the coordinates\n4.Click to create path\n5.Import from file\n");
        int sx = 0, sy = 0;
        do {
            scanf_s("%d", &choice);
        } while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5);//迷宫生成方法的选择
        system("cls");
        FILE* infile;
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
            free(Maze);//释放内存
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
            //eleprint(puzzle);
            //printf("\n");
            for (int i = 0; i < dX; i++)
            {
                for (int j = 0; j < dY; j++)
                    map[i][j] = puzzle[i + 1][j + 1];
            }//将迷宫导入代表走迷宫的地图的数组，并舍去其中外面一圈保护数组
            for (int i = 0; i < (dX + 2); i++) free(puzzle[i]);
            free(puzzle);//释放内存
            break;
        case 3://输坐标生成迷宫
            for (int i = 0; i < dX; i++)
            {
                for (int j = 0; j < dY; j++)
                {
                    temp[i][j] = BARRICADE;
                }
            }//初始化为墙
            system("cls");
            printf("Please type in the coordinates of the path. Enter 1 0 to start...\n");
            printf("Example: 1 0 ...\n");
            printf("x%cy", (char)92);
            for (int i = 0; i < dY; i++)
                printf("%02d  ", i);
            printf("\n");
            for (int i = 0; i < 2 * dX; i++)
            {
                if (i % 2 == 0)
                {
                    printf("%02d ", i / 2);
                    for (int j = 0; j < dY; j++)
                    {
                        switch (temp[i / 2][j]) {
                        case PATH:
                            printf("    ");
                            break;
                        case BARRICADE:
                            printf("%c%c  ", (char)219, (char)219);
                            break;
                        }
                    }
                }
                else
                    printf("  ");
                if (i == 2 * dX - 4)
                    printf("<<EXIT(Enter this coordinate to end)");
                printf("\n");
            }//坐标网格打印
            do
            {
                printf("Enter 1 0 to start...\n");
                scanf_s("%d %d", &sx, &sy);
            } while (sx != 1 || sy != 0);
            temp[1][0] = PATH;//起点
            do
            {
                system("cls");
                printf("Please type in the coordinates of the path. Enter two coordinates and the front and back coordinates must be close to each other.\n");
                printf("Example: 1 1 ...\n");
                printf("x%cy", (char)92);
                for (int i = 0; i < dY; i++)
                    printf("%02d  ", i);
                printf("\n");
                for (int i = 0; i < 2 * dX; i++)
                {
                    if (i % 2 == 0)
                    {
                        printf("%02d ", i / 2);
                        for (int j = 0; j < dY; j++)
                        {
                            switch (temp[i / 2][j]) {
                            case PATH:
                                printf("    ");
                                break;
                            case BARRICADE:
                                printf("%c%c  ", (char)219, (char)219);
                                break;
                            }
                        }
                    }
                    else
                        printf("  ");
                    if (i == 2 * dX - 4)
                        printf("<<EXIT(Enter this coordinate to end)");
                    printf("\n");//实时状态显示
                }
                int time = 0;
                do {
                    if (time != 0)
                        printf("Illiegal coordinates!\n");
                    scanf_s("%d %d", &sx, &sy);
                    time++;
                } while (sx <= 0 || sx > dX - 2 || sy <= 0 || sy > dY - 1);
                time = 0;
                temp[sx][sy] = PATH;
            } while (sx != dX - 2 || sy != dY - 1);//输入路的坐标直到终点
            copy(temp, map);//将迷宫导入代表走迷宫的地图的数组
            for (int i = 0; i < dX; i++) free(temp[i]);
            free(temp);//释放内存
            system("cls");
            break;
        case 4://鼠标点击生成迷宫
            //绘制图形化界面：
            initgraph((COL + 4) * (size + 20), (ROW + 4) * (size + 20), SHOWCONSOLE);
            setbkcolor(WHITE);
            cleardevice();

            BeginBatchDraw();//用于清理图形化界面的函数
            //注意事项说明：
            printf("The starting and ending points have been defined.\n");
            printf("No other points can be chosen as the starting and ending points.\n");
            printf("Click the end point to finish the maze drawing.\n");
            //用于绘制迷宫的鼠标操作循环：
            do {
                map1(arr, ROW);//绘制地图（在循环中可反复绘制）
                FlushBatchDraw();//用于清理图形化界面的函数
                //开始进行鼠标操作：
                if (MouseHit()) {
                    MOUSEMSG msg = GetMouseMsg();
                    switch (msg.uMsg) {
                    case WM_LBUTTONDOWN:
                        //建立鼠标坐标与数组元素的关系：
                        int row1 = msg.y / size;
                        int col1 = msg.x / size;
                        //以下四个条件用于确定唯一的起点和终点：
                        if (msg.x >= 0 && msg.x <= size && msg.y <= ROW * size && msg.y >= 0) {
                            i++;
                            system("cls");
                            rules();
                            printf("It breaks the rule %d times.\n", i);
                            break;
                        }
                        if (msg.x >= 0 && msg.x <= COL * size && msg.y >= (ROW - 1) * size && msg.y <= ROW * size) {
                            i++;
                            system("cls");
                            rules();
                            printf("It breaks the rule %d times.\n", i);
                            break;
                        }
                        if ((msg.x >= (COL - 1) * size && msg.x <= COL * size) && (msg.y <= (ROW - 2) * size || msg.y >= (ROW + 1) * size)) {
                            i++;
                            system("cls");
                            rules();
                            printf("It breaks the rule %d times.\n", i);
                            break;
                        }
                        if (msg.x >= 0 && msg.x <= COL * size && msg.y >= 0 && msg.y <= size) {
                            i++;
                            system("cls");
                            rules();
                            printf("It breaks the rule %d times.\n", i);
                            break;
                        }
                        //绘制迷宫：
                        if (row1 < ROW && col1 < COL) {
                            arr[row1][col1] = 1;
                        }
                        //判断是否结束绘制：
                        if (msg.x >= (COL - 1) * size && msg.x <= COL * size && msg.y >= (ROW - 2) * size && msg.y <= (ROW + 1) * size) {
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
            } while (1);//死循环，用于反复循环
            EndBatchDraw();//用于清理图形化界面的函数
            closegraph();
            printf("End map drawing\n");
            for (int j = 0; j < ROW; j++) {
                for (int k = 0; k < COL; k++) {
                    maze1[j][k] = arr[j][k];
                }
            }
            copy(maze1, map);
            break;
        case 5:
            char filename[dX];
            printf("Please enter a filename: ");
            scanf("%49s", filename);

            if ((infile = fopen(filename, "r+")) == NULL)
            {
                fprintf(stderr, "Can't open file %s\n", filename);
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < dX; i++)
            {
                for (int p = 0; p < dY; p++)
                {
                    fscanf(infile, "%d", &figure[i][p]);
                }
            }
            if (ferror(infile))
            {
                fprintf(stderr, "Error getting data from file %s\n", filename);
                exit(EXIT_FAILURE);
            }
            //for (int i = 0; i < dX; i++)
            //{
            //    for (int p = 0; p < dY; p++)
            //    {
            //        printf("%d", figure[i][p]);
            //    }
            //    putchar('\n');
            //}


            copy(figure, map);
            break;
        }
        if (choice != 20)
            choice = 1;//将终点情形统一为两种，方便后续游戏结束的判定
        eleprint(map);//打印迷宫
        printf("Save the maze?\n1.Yes...\t2.No...\t");
        do {
            scanf_s("%d", &solution);
        } while (solution != 1 && solution != 2);
        if (solution == 1)
        {
            char filename[dX];
            printf("Please enter a filename: ");
            scanf("%49s", filename);

            if ((infile = fopen(filename, "r+")) == NULL)
            {
                fprintf(stderr, "Can't open file %s\n", filename);
                exit(EXIT_FAILURE);
            }
            //存储代码--只能存在原文档末尾
            fseek(infile, 0L, SEEK_END);
            putc('\n', infile);
            for (int i = 0; i < dX; i++)
            {
                for (int p = 0; p < dX; p++)
                {
                    fprintf(infile, "%d", map[i][p]);
                }
                putc('\n', infile);
            }
            if (fclose(infile) != 0)
            {
                fprintf(stderr, "Can't close file %s\n", filename);
            }
        }
        system("cls");
        eleprint(map);
        playround = 0;
        copy(map, mapback);
        printf("Type in your choice 1/2/3 to solve the maze:\n1.Type in coordinates to control\n2.Use w/s/a/d to control\n3.Click to control\n");
        do {
            scanf_s("%d", &solution);
        } while (solution != 1 && solution != 2 && solution != 3);//走迷宫方法的选择
        int x, y;
        int dx, dy;//坐标改变量
        switch (solution) {
        case 1://输坐标走迷宫
            switch (choice) {
            case 1:
                x = 0;
                y = 1;
                break;
            case 2:
                x = 1;
                y = 2;
                break;
            default:
                x = 0;
                y = 1;
            }//起点判定
            map[y][x] = PACE;
            system("cls");
            eleprint(map);//加入当前玩家位置的显示
            map[y][x] = PACE;//初始化起点
            do {
                printf("Please enter the change of x and y coordinates to go to the next position.(You can only type in 1,0,-1):\nexample: 1 0\n");
                scanf_s("%d %d", &dx, &dy);
                if (((int)dy != 1 && (int)dy != 0 && (int)dy != -1) || ((int)dx != 1 && (int)dx != 0 && (int)dx != -1) || ((int)dx != 0 && (int)dy != 0)) {
                    printf("It's against the rule!!!\n");
                    continue;
                }//玩家意外情况的排除
                map[y][x] = PATH;
                if (map[y + dy][x + dx] == BARRICADE) {
                    printf("You've crashed into the wall!!!\n");
                    continue;
                }//撞墙提示
                system("cls");
                map[y + dy][x + dx] = PACE;
                y = y + dy;
                x = x + dx;
                eleprint(map);//打印当前状态
            } while (x != (choice == 1 ? dX - 1 : dX - 2));//终点判定
            printf("Congratulations!You've completed the maze!\n");
            break;
        case 3://鼠标点击走迷宫
            initgraph((COL + 4) * (size + 20), (ROW + 4) * (size + 20), SHOWCONSOLE);
            setbkcolor(WHITE);
            cleardevice();
            printf("Click on the right-hand side of screen to continue.\n");

            judge = 0;
            for (int j = 0; j < ROW; j++) {
                for (int k = 0; k < COL; k++) {
                    arr[j][k] = map[j][k];
                }
            }
            BeginBatchDraw();//用于清理图形化界面的函数
            do {
                arr[r][c] = 1;//存储上一次操作的地址，用于清空前一次的位置标记，为实时显示迷宫中的位置服务
                //开始进行鼠标操作：
                if (MouseHit()) {
                    MOUSEMSG msg = GetMouseMsg();
                    switch (msg.uMsg) {
                    case WM_LBUTTONDOWN:
                        //建立鼠标坐标与数组元素的关系：
                        int row = msg.y / size;
                        int col = msg.x / size;
                        //用于提前结束游戏：
                        if (msg.x >= 0 && msg.x <= COL * size && msg.y >= (ROW + 1) * size && msg.y <= (ROW + 2) * size) {
                            judge = 1;
                            break;
                        }
                        if (row < ROW && col < COL) {
                            //一格一格走，不可以多走：
                            int dc = (col - c);
                            int dr = (row - r);
                            if (dc > 1 || dr > 1 || dr < -1 || dc < -1 || (dc == 1 && dr == 1) || (dc == -1 && dr == 1) || (dc == -1 && dr == -1) || (dc == 1 && dr == -1)) {
                                i++;
                                system("cls");
                                rules();
                                printf("It breaks the rule %d times.\n", i);
                                continue;
                            }
                            //不可以点击绿色的墙：
                            if (arr[row][col] != 1) {
                                i++;
                                system("cls");
                                rules();
                                printf("It breaks the rule %d times.\n", i);
                                continue;
                            }

                            else {
                                arr[row][col] = 2;
                                r = row;
                                c = col;
                            }
                        }
                        //判断是否走出迷宫，赢得游戏：
                        if (msg.x >= (COL - 1) * size && msg.x <= COL * size && msg.y <= (ROW - 1) * size && msg.y >= (ROW - 2) * size) {
                            judge = 2;
                            break;
                        }
                        map1(arr, ROW);//绘制地图
                        buttom2(0, (ROW + 1) * size, COL * size, (ROW + 2) * size);//绘制按钮
                        FlushBatchDraw();//用于清理图形化界面的函数
                    }
                    //结束走迷宫阶段：
                    if (judge == 1 || judge == 2) {
                        break;
                    }
                }
            } while (1);//死循环，用于反复循环

            EndBatchDraw();//用于清理图形化界面的函数
            for (int j = 0; j < ROW; j++) {
                for (int k = 0; k < COL; k++) {
                    maze2[j][k] = arr[j][k];
                }
            }
            copy(maze2, map);
            //祝贺：
            if (judge == 2) {
                printf("You win!!!\n");
            }
            if (judge == 1) {
                printf("You quit the game early.\n");
            }
            getchar();
            closegraph();
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
            system("cls");
            eleprint(map);
            map[x][y] = PATH;//初始化地图
            int userkey = 72;//接收键盘操作
            int corrector = 1;//撞墙判定
            int round = 0;
            printf("Press \"d\" or Right to start:\n");
            do {
                userkey = _getch();
                if (userkey == 'd' || userkey == 'D' || userkey == 77)
                    break;
            } while (userkey != 'd' && userkey != 'D' && userkey != 77);
            system("cls");
            right(&x, &y, map);
            draw(map, corrector);
            show();//防止第一步直接移出地图
            do {
                switch (userkey = _getch())
                {
                case'w':
                case'W':
                case 72:
                    if (map[x - 1][y] == BARRICADE)
                    {
                        corrector = 0;
                        WriteConsoleOutputCharacterA(*houtpoint, (char*)data[LENGTH + 2], (2 * LENGTH), coord, &bytes);
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
                        WriteConsoleOutputCharacterA(*houtpoint, (char*)data[LENGTH + 2], (2 * LENGTH), coord, &bytes);
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
                        WriteConsoleOutputCharacterA(*houtpoint, (char*)data[LENGTH + 2], (2 * LENGTH), coord, &bytes);
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
                        WriteConsoleOutputCharacterA(*houtpoint, (char*)data[LENGTH + 2], (2 * LENGTH), coord, &bytes);
                        continue;
                    }
                    right(&x, &y, map);
                    break;
                }//键盘控制移动
                draw(map, corrector);
                show();//双缓冲绘制地图当前状态
                corrector = 1;//重置撞墙判定
            } while (y != dY - (choice == 1 ? 1 : 2));//终点判定
            eleprint(map);
            printf("Congratulations!You've completed the maze!\n");
            break;
        }
        closegraph();
    } while (0);
    //system("pause");
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
    char text2[] = { "quit the game" };
    setlinecolor(BLUE);
    setfillcolor(YELLOW);
    fillrectangle(left, top, right, bottom);
    settextcolor(RED);
    settextstyle(size / 2, size / 2, "Dotum");
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