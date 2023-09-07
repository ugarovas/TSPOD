#include <stdlib.h> 
#include <time.h> 
#include <iostream>
#include <conio.h>
#include <unistd.h>

#include <cstdlib>

void clear_screen()
{
#ifdef WINDOWS
    std::clear_screen();
#else
    // Assume POSIX
    std::system ("clear");
#endif
}

using namespace std;
int step_x=2;
int step_y=1;
int Pause=1;
struct TCoord
{
	int x, y;
};
struct node
{
	TCoord coord;
	node *next; 
}; 
class TList
{
public:
	node *head, *tail;
	int nodes_num; 
	TList():head(NULL), tail(NULL), nodes_num(0){}; 
	~TList()
	{
		node *temp = head;
		while (temp != NULL) 
		{
			temp = head->next;
			delete head;  
			head = temp;  
		}
	} 
	void add_node(int x, int y)
	{
	    node *temp = new node;
		temp->coord.x = x;
		temp->coord.y = y;
		temp->next = NULL;
		if (head != NULL)
		{
			tail->next = temp;
			tail = temp;
		}
		else
			head = tail = temp; 
		nodes_num++;
	}
	TCoord get_node(int n)
	{
		node *pnode;
		int i;
		TCoord coord;
		pnode = head;
		i = 0;
	    while((pnode!= NULL)&&(i< n))
		{
			pnode = pnode->next;
			i++;
		}
		if( pnode!= NULL)
		{
			coord.x = pnode->coord.x;
			coord.y = pnode->coord.y;	
		}
		else 
		{
			coord.x = -1;
			coord.y = -1;		
		}
		return coord;
	};
	void del_node(int n)
	{
		node *curr_node, *prev_node;
		int i;
		if(n==0)
		{
			curr_node = head->next;
			if(tail == head) tail = NULL;
			delete head;
			head = curr_node;
			nodes_num--;
			return;
		}
		curr_node = head;
		i = 0;
	    do
		{
			prev_node = curr_node;
			curr_node = curr_node->next;
			i++;
		}
		while((curr_node!= NULL)&&(i< n));
		if( curr_node!= NULL)
		{
			if(curr_node == tail) tail = prev_node;
			prev_node->next = curr_node->next;
			delete curr_node;
			nodes_num--;
		}
		return;
	}
}; 
void output(char **Maz, int max_x, int max_y)
{
	clear_screen();
	for (int x=0; x< max_x-step_x+2; x++)
	{
		for (int y=0; y< max_y-step_y+1; y++)
		{
			if (Maz[x][y]=='*')
			{
				cout << "*";
				cout << "*";
			}
			else
				if (Maz[x][y]=='s')
				{
					cout <<" ";
					cout <<" ";
				}
				else
					cout<<"  ";
		}
		cout<<endl;
	}	
}
void generate(char **Maz, int max_x, int max_y, int flag)	
{
	TList list;
	TCoord coord; 
	int r, i;
	char dir[4]; 
	for(int y=0; y< max_y; y++)
		for(int x=0; x< max_x; x++)
			Maz[x][y] = '*';
	srand((unsigned long)time(NULL));
	coord.x = 1;
	coord.y = 1;
	Maz[coord.x][coord.y]=' ';
	list.add_node(coord.x,coord.y);
	do
	{
		r = rand()%list.nodes_num;
		coord = list.get_node(r);
		i=0;
		if((coord.y>2)&&(Maz[coord.x][coord.y-2]=='*'))
		{
			dir[i]= '1';
			i++;
		}
		if((coord.x>2)&&(Maz[coord.x-2][coord.y]=='*'))
		{
			dir[i]= '2';
			i++;
		}
		if((coord.y<(max_y-3))&&(Maz[coord.x][coord.y+2]=='*'))
		{
			dir[i]= '3';
			i++;
		}
		if((coord.x<(max_x-3))&&(Maz[coord.x+2][coord.y]=='*'))
		{
			dir[i]= '4';
			i++;
		}
		if(i==0)
		{
			list.del_node(r);
		}
		else
		{
			i = rand()%i;
			if(dir[i]=='1')
			{
				Maz[coord.x][coord.y-1]=' ';
				Maz[coord.x][coord.y-2]=' ';
				coord.y = coord.y-2;
			}
			if(dir[i]=='2')
			{
				Maz[coord.x-1][coord.y]=' ';
				Maz[coord.x-2][coord.y]=' ';
				coord.x = coord.x-2;
			}
			if(dir[i]=='3')
			{
				Maz[coord.x][coord.y+1]=' ';
				Maz[coord.x][coord.y+2]=' ';
				coord.y = coord.y+2;
			}
			if(dir[i]=='4')
			{
				Maz[coord.x+1][coord.y]=' ';
				Maz[coord.x+2][coord.y]=' ';
				coord.x = coord.x+2;
			}
			list.add_node(coord.x, coord.y);
			if (flag==1)
			{
				clear_screen();
				output(Maz, max_x, max_y);
				sleep(1);
			}
		}	
	}
	while(list.head != NULL);
	Maz[1][0]='s';
	Maz[max_x-step_x][max_y-step_y]='f';
}
void walk 
		(
			char **Maz, 
			int s1, int s2, 
			int max_x, int max_y 
		  ) 
{
	int flag=0;
	unsigned short keysArrow;
	do
	{
		clear_screen();
		output (Maz, max_x, max_y);
		keysArrow = getch();
		keysArrow = getch();   
		if(keysArrow == 0)
			keysArrow = getch();
		if(keysArrow == 72 || keysArrow == 150 || keysArrow == 230)
		{  
			if (s1!=0) 
			{
				if ((Maz[s1-1][s2]==' ')||(Maz[s1-1][s2]=='f'))
				{
					if (Maz[s1-1][s2]=='f')
						flag=1;
					Maz[s1][s2] = ' ';
					s1--;
					Maz[s1][s2] ='s';
				}
			}
		}
	  if(keysArrow == 80 || keysArrow == 155 || keysArrow == 235)
	  {
		if (s1!=max_x)		
		{
			if ((Maz[s1+1][s2]==' ')||(Maz[s1+1][s2]=='f'))
			{
				if (Maz[s1+1][s2]=='f')
					flag=1;
				Maz[s1][s2] = ' ';
				s1++;
				Maz[s1][s2] ='s';
			}
		}
	  }
	  if(keysArrow == 77 || keysArrow == 130 || keysArrow == 262)
	  {
	   if (s2!=max_y) 
		{
			if ((Maz[s1][s2+1]==' ')||(Maz[s1][s2+1]=='f'))
			{
				if (Maz[s1][s2+1]=='f')
					flag=1;
				Maz[s1][s2] = ' ';
				s2++;
				Maz[s1][s2] ='s';
			}
	   }
	  }
	  if(keysArrow == 75 || keysArrow == 148 || keysArrow == 235)
	  {
		if (s2!=0) 
		{
			if ((Maz[s1][s2-1]==' ')||(Maz[s1][s2-1]=='f'))
			{
				if (Maz[s1][s2-1]=='f')
					flag=1;
				Maz[s1][s2] = ' ';
				s2--;
				Maz[s1][s2] ='s';
			}
		}
	}
	}
	while (flag==0);
	clear_screen();
	output (Maz, max_x, max_y);
	cout<<"\n\n\t\t\t Победа! \n\n";
}
void autoWalk 
			(
				char **Maz, 
				int **path,
				int len,
				int max_x, int max_y 
			)
{
	int x=path[0][0];
	int y=path[0][1];
	Maz[x][y]='!';
	output (Maz, max_x, max_y);
	sleep(Pause);
	for (int i=1; i<=len; i++)
	{
		clear_screen();
		Maz[x][y]=' ';
		x=path[i][0];
		y=path[i][1];
		Maz[x][y]='s';
		output(Maz, max_x,max_y);
		sleep (Pause);
	}
}
int Lee (
			char **Maz, 
			int start_x, int start_y, 
			int end_x, int end_y, 
			int max_x, int max_y 
		)
{
	int **path=new int *[max_x*max_y];
	for (int i=0; i<max_x*max_y; i++)
		path[i]=new int[2];
	int **cMaz= new int *[max_x]; 
	for (int i=0; i<max_x; i++)
		cMaz[i]=new int [max_y];
	for (int i=0; i<max_x; i++)
	{
		for (int j=0; j<max_y; j++)
		{
			if (Maz[i][j]=='*')
				cMaz[i][j]=-1;
			else
				cMaz[i][j]=-2;
		}
	}
	int dx[4]={1, 0, -1, 0};
	int dy[4]={0, 1, 0, -1}; 
	int d, x, y, k, len;
	bool stop;
	if ((cMaz[start_x][start_y]==-1)||(cMaz[end_x][end_y]==-1))
	{
		for (int i=0; i<max_x; i++)
			delete[] cMaz[i];
		delete[] cMaz;
		for (int i=0; i<max_x*max_y; i++)
			delete[] path[i];
		delete[] path;
		return -1; 
	}
	d=0; 
	cMaz[start_x][start_y]=d; 
	do{
		stop=true;
		for (x=0; x<max_x; ++x)
		{
			for (y=0; y<max_y; ++y)
			{
				if (cMaz[x][y]==d)
				{
					for (k=0; k<4; ++k)
					{
						int ix=x+dx[k];
						int iy=y+dy[k];
						if (ix>=0 && ix<max_x&&
							iy>=0&&iy<max_y&&
							cMaz[ix][iy]==-2)
						{
							cMaz[ix][iy]=d+1;
							stop=false;

						}
					}
				}
			}
		}
		d++;
	}
	while (!stop&&cMaz[end_x][end_y]==-2);
	if (cMaz[end_x][end_y]==-2)
	{
		for(int i=0; i<max_x; i++) 
			delete[] cMaz[i];
		delete[] cMaz;
		for (int i=0; i<max_x*max_y; i++)
			delete[] path[i];
		delete[] path;
		return -1; 
	}
	d=cMaz[end_x][end_y];
	len=d;
	x=end_x;
	y=end_y;
	while (d>=0)
	{
		path[d][0]=x;
		path[d][1]=y;
		d--;
		for (k=0; k<4; k++)
		{
			int ix=x+dx[k];
			int iy=y+dy[k];
			if (ix>=0 && ix<max_x && iy>=0 && iy<=max_y && cMaz[ix][iy]==d)
			{
				x=x+dx[k];
				y=y+dy[k];
				break;
			}
		}
	}
	for(int i=0; i< max_x; i++) 
		delete[] cMaz[i];
	delete[] cMaz;
	autoWalk(Maz, path, len, max_x, max_y);
	for (int i=0; i<max_x*max_y; i++)
			delete[] path[i];
		delete[] path;
	return 0;
}
int main(int argc, char *argv[]) 
{ 
		setlocale(LC_ALL, "Russian");
		int choise;
		int max_x = 21;
		int max_y = 21;
		do
		{
			clear_screen();
			cout<<"\t\t\tМЕНЮ\n\n\n"<<" 1. Демонстрация \n\n 2. Игра  \n\n 3. Настройки \n\n 4. Помощь \n\n 0. Выход \n\n Выберите действие: ";
			cin>>choise;
			switch (choise)
			{
			case 0:
			{
				clear_screen();
				cout<<"\n\n\t\tВы уверены? (1 - Да, 0 - Нет) \n\n\n \t Выберите действие: ";
				cin>>choise;
				choise--;
				break;
			}
			case 1: 
				{
					clear_screen();
					int temp;
					char **Maz = new char*[max_x];
					for(int i=0; i< max_x; i++) 
						Maz[i] = new char[max_y];
					// заполняем массив стенами и пустотами:
					generate(Maz,max_x,max_y, 1);
					output (Maz, max_x, max_y);
					cout<<"\n\t Генерация лабиринта завершена. \n\t Желаете увидеть его решение? (1 - Да, 2 - Нет): ";
					cin>>temp;
					if (temp==1)
						Lee (Maz, 1, 0, max_x-step_x, 
							max_y-step_y, max_x, max_y);
					for(int i=0; i< max_x; i++) 
						delete[] Maz[i];
					delete[] Maz;
					sleep(10);
					break;
				}
				case 2: 
				{
					clear_screen();
					char **Maz = new char*[max_x];
					for(int i=0; i< max_x; i++) 
						Maz[i] = new char[max_y];
					generate(Maz,max_x,max_y, 0);
					walk(Maz, 1, 0, max_x, max_y);
					for(int i=0; i< max_x; i++) 
						delete[] Maz[i];
					delete[] Maz;
					sleep(10);
					break;
				}
				case 3:
				{
					int temp;
					do{
					clear_screen();
					cout<<" 1. Задать ширину лабиринта. \n 2. Задать высоту лабиринта. \n 3. Задать продолжительность паузы \n 0. Назад \n\n Выберите действие: ";
					cin>>temp;
					switch (temp)
					{
						case 0: break;
						case 1: 
						{
							do
							{
								clear_screen();
								cout<<"\n\n\t\t Ввод ширины лабиринта. \n\t Внимание! Ширина должна быть не меньше 3! \n\n\t Введите ширину лабиринта: ";
								cin>>max_x;
								if (max_x<3)
								{
									cout<<"\n\n \t Ошибка! Повторите ввод! \n";
									system("Pause");
								}
							}
							while (max_x<3);
							if (max_x%2==0)
								step_x=3;
							else
								step_x=2;
							break;
						}
						case 2:
						{
							do
							{
								clear_screen();
								cout<<"\n\n \t\t Ввод высоты лабиринта. \n\t Внимание! Высота должна быть не меньше 3! \n\n\t Введите высоту лабиринта: ";
								cin>>max_y;
								if (max_y<3)
								{
									cout<<"\n\n \t Ошибка! Повторите ввод! \n";
									system("Pause");
								}
							}
							while (max_y<3);
							if (max_y%2==0)
								step_y=2;
							else
								step_y=1;
							break;
						}
						case 3:
						{
							do 
							{
								clear_screen();
								cout<<"\n\n \t\t Ввод продолжительности паузы. \n\t Данный параметр отвечает за задержку перед \n\t обновлением изображения во время демонстрации \n\n\t Внимание! Данная величина должна быть неотрицательной! \n\n\t Введите продолжительность паузы (мс): ";
								cin>>Pause;
								if (Pause<0)
								{
									cout<<"\n\n \t Ошибка! Повторите ввод! \n";
									sleep(10);
								}
							}
							while (Pause<0);
							break;
						}
						
					}
					}
					while(temp!=0);
					break;
				}
				case 4: 
				{

					cout<<"\t\t\t Помощь \n\n\t Используйте настройки для задания размеров лабиринта и величины паузы.\n\t Для ручного прохождения лабиринта выберите пункт Игра. \n\t Для просмотра процесса генерации и прохождения лабиринта \n\t, используйте пункт Демонстрация"<<endl;											system("Pause");
				}
			}
		}
		while (choise!=0);
		sleep(10);
}
