#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <ctime>
#define SIZE	20
#define bSIZE	SIZE+2
#define MIN_VALUE -2
#define TOP		1
#define BOT		TOP+2*SIZE
#define LEFT	3
#define RIGHT	LEFT+4*SIZE

void create_mine();
int check(int x, int y);		//kiem tra so mines xung quanh o (x,y)
void taoBaiMin();			//tao ra bai mines
void game();
void draw();				//ve ban co
void create_Borders();			//tao bien cho ma tran
void init();				//khoi tao ban co, chon level, tao bai mines
void level();				//chon so luong mines
void loang(int ai, int aj);
void enter(int ai, int aj);		//xu ly sau khi nhan ENTER
void control();				//dieu khien cua player
void xuat();				//xuat tat ca cac gia tri bai mines ra man hinh, replay
void gotoXY(int x, int y);
void textColor (int color);
void resizeConsole(int width, int height);
int A[bSIZE][bSIZE], value;
unsigned int x, y, avail;

int main(){
	resizeConsole(1000,780);
	game();
	return 0;
}

void game(){
	system("cls");	srand(time(NULL));
	init();
	control();
	xuat();
}
void init(){
	x=5;y=2;
	/*Khoi tao mang trong*/
	level();			//chon level
	draw();				//ve ban co
	create_Borders();		//tao bien cho bai mines
	avail=SIZE*SIZE+1;		//khoi tao avail cho biet chua open o nao
	control();			//xac dinh vi tri o 1st duoc open
	create_mine();			//tao ra vi tri mines
	taoBaiMin();			//tao ma tran bai mines
	avail=SIZE*SIZE;		//khoi tao lai avail
	int xx=(y-TOP-1)/2+1,	yy=(x-LEFT-2)/4+1;
	if (A[xx][yy]==0)
		loang(xx,yy);	//loang sau khi open o 1st
	else{
		textColor(A[xx][yy]);	printf("%d",A[xx][yy]);
		A[xx][yy]=-2;	;
	}
	avail=SIZE*SIZE-1;
}
void level(){
	int x=3*SIZE-10,	y=SIZE;
	textColor(3);
	gotoXY(x,3*SIZE/4);	printf("GAME  MINESWEEPERS");
	gotoXY(x,y++);	printf("LEVEL\t\tMINES");
	gotoXY(x,y++);	printf("LEVEL 1:	 30\n");
	gotoXY(x,y++);	printf("LEVEL 2:	 50\n");
	gotoXY(x,y++);	printf("LEVEL 3:	 70\n");
	gotoXY(x,y++);	printf("LEVEL 4:	 85\n");
	gotoXY(x,y++);	printf("LEVEL 5:	100\n");
	gotoXY(x,y);	printf("Chon Level:\t  ");
	do{
		scanf("%d",&value);
		if (value<1 || value>5){
			gotoXY(x,y);	printf("Nhap lai, chon Level: ");
		}
	} while (value<1 || value>5);
	switch (value)	{
		case 1: value=30;	break;
		case 2: value=50;	break;
		case 3: value=70;	break;
		case 4:	value=85;	break;
		case 5: value=100;	break;
	}
	fflush(stdin);	system("cls");
}
void enter(int ai, int aj){
	if (avail<SIZE*SIZE){			//da open it nhat 1 o
		if (A[ai][aj]==-1) xuat();	//di vao o chua mine
		else if (A[ai][aj]==0)		//di vao vung trong
			loang(ai,aj);
		else if (A[ai][aj]!=-2){	//di vao cac o mang gia tri >0
			textColor(A[ai][aj]);	printf("%d",A[ai][aj]);	avail--;
		}
	}else				//chua open o nao, mac dinh o dau tien khong co mine
	{	avail=0;	}	//dat avail=0 de ket thuc control
}
void control(){
	while (avail){			//khi van con o chua open
		gotoXY(x,y);	
		int ai=(y-TOP-1)/2+1,	aj=(x-LEFT-2)/4+1;
		if (kbhit()){
			char key=getch();
			switch (key){
				case 13:{
					enter(ai,aj);		break;
				}
				case 32:{
					textColor(4);	printf("%c",254);avail--;
					A[ai][aj]=-2;	//flag: o da duyet
					break;
				}
				case 72:{
					if (y-2<TOP+1)	y=BOT-1;	else	y-=2;	break;
				}
				case 75:{
					if (x-4<LEFT+2)	x=RIGHT-2;	else	x-=4;	break;
				}
				case 77:{
					if (x+4>RIGHT)	x=LEFT+2;	else	x+=4;	break;
				}
				case 80:{
					if (y+2>BOT)	y=TOP+1;	else	y+=2;	break;
				}
			}
		}
	}
}
void loang(int ai, int aj){
	int X[]={-1,0,1}, row, col;
	for (int i=0;i<3;i++)
		for (int j=0;j<3;j++){
			row=ai+X[i],	col=aj+X[j];
			gotoXY(4*col+LEFT-2,2*row+TOP-1);
			if (A[row][col]==0){
				textColor(0);	printf("%c",219);
				A[row][col]=-2;		//flag: danh dau o da duyet
				avail--;		//opened them 1 o
				loang(row,col);
			}
			else if (A[row][col]>0){
				textColor(A[row][col]);	printf("%d",A[row][col]);
				A[row][col]=-2;		//flag: danh dau o da duyet
				avail--;		//opened them 1 o
			}
		}
}
void create_Borders(){
	int i, j;
	for (i=0;i<bSIZE;i++)
		for (j=0;j<bSIZE;j++)
			A[i][j]=0;
	for (j=0;j<bSIZE;j++){
		A[0][j]=MIN_VALUE;	A[bSIZE-1][j]=MIN_VALUE;
	}
	for (i=1;i<bSIZE-1;i++){
		A[i][0]=MIN_VALUE;	A[i][bSIZE-1]=MIN_VALUE;
	}
}
void create_mine(){
	int count=0, ai=(y-TOP-1)/2+1,	aj=(x-LEFT-2)/4+1;
	while (count<value){
		int i=rand()%(SIZE)+1, j=rand()%(SIZE)+1;
		if (A[i][j]>=0)
			if (!(i==ai && j==aj)){
				A[i][j]=-1;	//Gia tri -1 cho biet vi tri do co Mi`n
				count++;
			}
	} 
}
int check(int x, int y){	//dem so luong Mi`n xung quanh
	int count=0;
	for (int i=x-1;i<=x+1;i++)
		for (int j=y-1;j<=y+1;j++)
			if (A[i][j]==-1)	count++;
	return count;
}
void taoBaiMin(){	//tao bai Mi`n
	for (int i=0;i<SIZE+2;i++)
		for (int j=0;j<SIZE+2;j++)
			if (A[i][j]!=-1&&A[i][j]!=MIN_VALUE)	//tranh vi tri dat Mi`n
				A[i][j]=check(i,j);
}
void xuat(){
	for (int i=1;i<bSIZE;i++)
		for (int j=1;j<bSIZE;j++){
			gotoXY(4*j+LEFT-2,2*i+TOP-1);
			if (A[i][j]>=0){
				textColor(A[i][j]);	printf("%d",A[i][j]);
			}
			else if (A[i][j]==-1){
				textColor(4);	printf("%c",248);
			}
		}
	int yy=SIZE;	gotoXY(98,yy++);	textColor(4);
	if (avail)	printf("YOU LOST");
	else printf("YOU WON");
	textColor(3);
	gotoXY(94,yy++);	printf("Press ESC to escape.");
	gotoXY(94,yy++);	printf("Press ENTER to replay.");
	gotoXY(x,y);	//tro ve vi tri trong lan nhan ENTER cuoi cung
	char ch;
	do{
		if (kbhit()){
			ch=getch();
			if ((int)ch==27)	break;
			else if (ch==13)	game();
		}
	} while (ch!=27 || ch!=13);
	system("cls");	exit(0);
}
void draw(){
	int i,j;	textColor(7);
	gotoXY(LEFT+1,TOP);					//TOP
	for (i=0;i<(2*SIZE);i++)
		if (i%2)	printf("%c",194);	else	printf("%c%c%c",196,196,196);
	gotoXY(LEFT+1,BOT);					//BOT
	for (i=0;i<2*SIZE;i++)
		if (i%2)	printf("%c",193);	else	printf("%c%c%c",196,196,196);
	for (i=0;i<2*SIZE;i++){
		gotoXY(LEFT,TOP+i);		if (i%2)	printf("%c",179);	else	printf("%c",195);
		gotoXY(RIGHT,TOP+i);	if (i%2)	printf("%c",179);	else	printf("%c",180);
	}
	char S0[200], S1[200];
	for (int j=0;j<4*SIZE-1;j++)
		if (j%4==0)	S0[j]=179;	
		else if (j%4==2)	S0[j]=254;	
			else S0[j]=' ';
	for (int j=0;j<4*SIZE-1;j++)
		if ((j+1)%4==0)	S1[j]=197;
		else S1[j]=196;
	S1[4*SIZE-1]='\0';	S0[4*SIZE-1]='\0';
	i=1;
	while (i<2*SIZE-1){
		gotoXY(LEFT,TOP+i++);	printf("%s",S0);
		gotoXY(LEFT+1,TOP+i++);	printf("%s",S1);
	}
	gotoXY(LEFT,TOP+i);	printf("%s",S0);
	gotoXY(LEFT,TOP);	printf("%c",218);
	gotoXY(RIGHT,TOP);	printf("%c",191);
	gotoXY(LEFT,BOT);	printf("%c",192);
	gotoXY(RIGHT,BOT);	printf("%c",217);
	int y=SIZE/2;
	gotoXY(98,y);	printf("HOW TO PLAY");
	y+=3;
	gotoXY(95,y++);	printf("ENTER: Open o duoc chon");
	gotoXY(95,y++);	printf("SPACE: Danh dau Flag");
	gotoXY(95,y++);	printf("Mui ten: Di chuyen");
}
void resizeConsole(int width, int height){
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}
void textColor (int color){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE) , color);
}
void gotoXY(int x, int y){
	static HANDLE h=NULL;
	if (!h)	h=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c={x,y};
	SetConsoleCursorPosition(h,c);
}
