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
    }//�����ڴ棬����������ά�����Դ����Թ��Ĳ�ͬ״̬
    do {
        printf("Maze Game\n");
        printf("Type in your choice 1/2/3/4/5 to create the maze:\n1.Create wall\n2.Create path\n3.Type in the coordinates\n4.Click to create path\n5.Import from file\n");
        int sx = 0, sy = 0;
        do {
            scanf_s("%d", &choice);
        } while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5);//�Թ����ɷ�����ѡ��
        system("cls");
        FILE* infile;
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
            free(Maze);//�ͷ��ڴ�
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
            //eleprint(puzzle);
            //printf("\n");
            for (int i = 0; i < dX; i++)
            {
                for (int j = 0; j < dY; j++)
                    map[i][j] = puzzle[i + 1][j + 1];
            }//���Թ�����������Թ��ĵ�ͼ�����飬����ȥ��������һȦ��������
            for (int i = 0; i < (dX + 2); i++) free(puzzle[i]);
            free(puzzle);//�ͷ��ڴ�
            break;
        case 3://�����������Թ�
            for (int i = 0; i < dX; i++)
            {
                for (int j = 0; j < dY; j++)
                {
                    temp[i][j] = BARRICADE;
                }
            }//��ʼ��Ϊǽ
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
            }//���������ӡ
            do
            {
                printf("Enter 1 0 to start...\n");
                scanf_s("%d %d", &sx, &sy);
            } while (sx != 1 || sy != 0);
            temp[1][0] = PATH;//���
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
                    printf("\n");//ʵʱ״̬��ʾ
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
            } while (sx != dX - 2 || sy != dY - 1);//����·������ֱ���յ�
            copy(temp, map);//���Թ�����������Թ��ĵ�ͼ������
            for (int i = 0; i < dX; i++) free(temp[i]);
            free(temp);//�ͷ��ڴ�
            system("cls");
            break;
        case 4://����������Թ�
            //����ͼ�λ����棺
            initgraph((COL + 4) * (size + 20), (ROW + 4) * (size + 20), SHOWCONSOLE);
            setbkcolor(WHITE);
            cleardevice();

            BeginBatchDraw();//��������ͼ�λ�����ĺ���
            //ע������˵����
            printf("The starting and ending points have been defined.\n");
            printf("No other points can be chosen as the starting and ending points.\n");
            printf("Click the end point to finish the maze drawing.\n");
            //���ڻ����Թ���������ѭ����
            do {
                map1(arr, ROW);//���Ƶ�ͼ����ѭ���пɷ������ƣ�
                FlushBatchDraw();//��������ͼ�λ�����ĺ���
                //��ʼ������������
                if (MouseHit()) {
                    MOUSEMSG msg = GetMouseMsg();
                    switch (msg.uMsg) {
                    case WM_LBUTTONDOWN:
                        //�����������������Ԫ�صĹ�ϵ��
                        int row1 = msg.y / size;
                        int col1 = msg.x / size;
                        //�����ĸ���������ȷ��Ψһ�������յ㣺
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
                        //�����Թ���
                        if (row1 < ROW && col1 < COL) {
                            arr[row1][col1] = 1;
                        }
                        //�ж��Ƿ�������ƣ�
                        if (msg.x >= (COL - 1) * size && msg.x <= COL * size && msg.y >= (ROW - 2) * size && msg.y <= (ROW + 1) * size) {
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
            } while (1);//��ѭ�������ڷ���ѭ��
            EndBatchDraw();//��������ͼ�λ�����ĺ���
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
            choice = 1;//���յ�����ͳһΪ���֣����������Ϸ�������ж�
        eleprint(map);//��ӡ�Թ�
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
            //�洢����--ֻ�ܴ���ԭ�ĵ�ĩβ
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
        } while (solution != 1 && solution != 2 && solution != 3);//���Թ�������ѡ��
        int x, y;
        int dx, dy;//����ı���
        switch (solution) {
        case 1://���������Թ�
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
            }//����ж�
            map[y][x] = PACE;
            system("cls");
            eleprint(map);//���뵱ǰ���λ�õ���ʾ
            map[y][x] = PACE;//��ʼ�����
            do {
                printf("Please enter the change of x and y coordinates to go to the next position.(You can only type in 1,0,-1):\nexample: 1 0\n");
                scanf_s("%d %d", &dx, &dy);
                if (((int)dy != 1 && (int)dy != 0 && (int)dy != -1) || ((int)dx != 1 && (int)dx != 0 && (int)dx != -1) || ((int)dx != 0 && (int)dy != 0)) {
                    printf("It's against the rule!!!\n");
                    continue;
                }//�������������ų�
                map[y][x] = PATH;
                if (map[y + dy][x + dx] == BARRICADE) {
                    printf("You've crashed into the wall!!!\n");
                    continue;
                }//ײǽ��ʾ
                system("cls");
                map[y + dy][x + dx] = PACE;
                y = y + dy;
                x = x + dx;
                eleprint(map);//��ӡ��ǰ״̬
            } while (x != (choice == 1 ? dX - 1 : dX - 2));//�յ��ж�
            printf("Congratulations!You've completed the maze!\n");
            break;
        case 3://��������Թ�
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
            BeginBatchDraw();//��������ͼ�λ�����ĺ���
            do {
                arr[r][c] = 1;//�洢��һ�β����ĵ�ַ���������ǰһ�ε�λ�ñ�ǣ�Ϊʵʱ��ʾ�Թ��е�λ�÷���
                //��ʼ������������
                if (MouseHit()) {
                    MOUSEMSG msg = GetMouseMsg();
                    switch (msg.uMsg) {
                    case WM_LBUTTONDOWN:
                        //�����������������Ԫ�صĹ�ϵ��
                        int row = msg.y / size;
                        int col = msg.x / size;
                        //������ǰ������Ϸ��
                        if (msg.x >= 0 && msg.x <= COL * size && msg.y >= (ROW + 1) * size && msg.y <= (ROW + 2) * size) {
                            judge = 1;
                            break;
                        }
                        if (row < ROW && col < COL) {
                            //һ��һ���ߣ������Զ��ߣ�
                            int dc = (col - c);
                            int dr = (row - r);
                            if (dc > 1 || dr > 1 || dr < -1 || dc < -1 || (dc == 1 && dr == 1) || (dc == -1 && dr == 1) || (dc == -1 && dr == -1) || (dc == 1 && dr == -1)) {
                                i++;
                                system("cls");
                                rules();
                                printf("It breaks the rule %d times.\n", i);
                                continue;
                            }
                            //�����Ե����ɫ��ǽ��
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
                        //�ж��Ƿ��߳��Թ���Ӯ����Ϸ��
                        if (msg.x >= (COL - 1) * size && msg.x <= COL * size && msg.y <= (ROW - 1) * size && msg.y >= (ROW - 2) * size) {
                            judge = 2;
                            break;
                        }
                        map1(arr, ROW);//���Ƶ�ͼ
                        buttom2(0, (ROW + 1) * size, COL * size, (ROW + 2) * size);//���ư�ť
                        FlushBatchDraw();//��������ͼ�λ�����ĺ���
                    }
                    //�������Թ��׶Σ�
                    if (judge == 1 || judge == 2) {
                        break;
                    }
                }
            } while (1);//��ѭ�������ڷ���ѭ��

            EndBatchDraw();//��������ͼ�λ�����ĺ���
            for (int j = 0; j < ROW; j++) {
                for (int k = 0; k < COL; k++) {
                    maze2[j][k] = arr[j][k];
                }
            }
            copy(maze2, map);
            //ף�أ�
            if (judge == 2) {
                printf("You win!!!\n");
            }
            if (judge == 1) {
                printf("You quit the game early.\n");
            }
            getchar();
            closegraph();
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
            system("cls");
            eleprint(map);
            map[x][y] = PATH;//��ʼ����ͼ
            int userkey = 72;//���ռ��̲���
            int corrector = 1;//ײǽ�ж�
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
            show();//��ֹ��һ��ֱ���Ƴ���ͼ
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
                }//���̿����ƶ�
                draw(map, corrector);
                show();//˫������Ƶ�ͼ��ǰ״̬
                corrector = 1;//����ײǽ�ж�
            } while (y != dY - (choice == 1 ? 1 : 2));//�յ��ж�
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