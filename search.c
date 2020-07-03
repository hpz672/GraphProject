#include <stdlib.h>
#include <stdio.h>
#include "search.h"
#include "stats.h" 
#include "stats.c"

int visited[MAXV] = {0};//用于标记该顶点是否被访问
int g_minnum;//求最短路径中包含的顶点数目 
int g_minlen;//求最短路径长度 
int g_pathmin[MAXV];//存储由u到v的最短路径顶点序号 
int dist[MAXV];//记录各顶点到u的距离 
int path[MAXV];//记录char类型路径 
int temp[MAXV];//记录逆序路径 
Box b[MAXV];//记录将边分割为weight-1个顶点后的相对顶点位置 

void enQueue(Queue *q, int e)
{
	if (q->isempty == 1)
	{
		q->isempty = 0;
	}
	else
	{
		q->front = (q->front + 1) % MAXV;
	}
	q->data[q->front] = e; 
	//printf("enQueue: %d\n", e);
}

int deQueue(Queue *q)
{
	int e = q->data[q->rear];
	if (q->front != q->rear)
	{
		q->rear = (q->rear + 1) % MAXV;
	}
	else
	{
		q->isempty = 1;
	}
	//printf("deQueue: %d\n", e);
	return e;
}

//采用邻接表的DFS算法： 
static void DFS(AdjGraph *G, int u, int v, int d)//输出一条从顶点u到v的路径
{
    if (g_minnum != INF) //找到路径返回 
	{
		return;	
	} 
	int i, w;
    ArcNode *p;//边结点类型指针
    
    path[d] = u;//当前顶点添加到路径中
    visited[u] = 1;//置已访问标记
    
    if(u == v)//找到终点 
    {
        for (i = 0; i <= d; i++) 
        {
            g_pathmin[i] = path[i];//记录当前路径 
        }
        g_minnum = d + 1;//记录路径中顶点数量 
        return;
    }
    p = G->adjlist[u].firstarc;//p指向顶点u的第一个相邻点
    while(p != NULL)
    {
        w = p->adjvex;//w为u的相邻点编号
        if(visited[w] == 0)//若该顶点未标记访问，则递归访问之
        {
        	//printf("%d %d\n", u, w); //测试 
        	DFS(G, w, v, d + 1);
		}
        p = p->nextarc;//找u的下一个相邻点
    }
}

//采用邻接表的BFS算法：
void BFS(AdjGraph *G, int u, int v)//求顶点u到顶点v的最短路径
{//采用从顶点u出发广度优先搜索方法，当搜索到顶点v时，在队列中找出对应的路径
 //将每条权值为weight的边拆分为weight-1个顶点，从而转化为等间距BFS搜索 
    int i, e;
    int count = 0;
	ArcNode *t;
    Queue *q;
    q = (Queue*)malloc(sizeof(Queue));
    q->rear = q->front = 0;
    q->isempty = 1;
    for (i = 0; i <= G->maxnum; i++) //赋初值 
    {
    	b[i].pre = -1;
    	b[i].current = 1;
	}
    visited[u] = 1;
    b[u].weight = 1;
    enQueue(q, u);
    while (q->isempty == 0)
    {
    	e = deQueue(q);
    	if (e == v && b[e].current == b[e].weight)
    	{
    		break;
		}
    	if (b[e].current == b[e].weight)
    	{
    		t = G->adjlist[e].firstarc;
    		while (t != NULL)
    		{
    			if (visited[t->adjvex] == 0 || t->weight < b[t->adjvex].weight - b[t->adjvex].current)
    			{
    				visited[t->adjvex] = 1;
    				b[t->adjvex].weight = t->weight;
    				b[t->adjvex].current = 1;
    				b[t->adjvex].pre = e;
    				enQueue(q, t->adjvex);
				}
				t = t->nextarc;
			}
		}
		else
		{
			b[e].current++;
			enQueue(q, e);
		}
	}
	if (b[v].pre == -1)//未找到路径直接返回 
	{
		return;
	}
	i = v;
	while (i != -1) 
	{
		temp[count] = i;//逆序记录路径 
		count++;
		i = b[i].pre;
	}
	g_minnum = count;
	for (i = 0; i < count; i++)
	{
		g_pathmin[i] = temp[count - i - 1];//保存最短路径 
	}
}

//采用邻接表的Dijkstra算法 
void Dijkstra(AdjGraph *G, int u, int v)
{
    int i, e;
    int count = 0;
    ArcNode *t;
    Queue *q;
    q = (Queue*)malloc(sizeof(Queue));
    q->rear = q->front = 0;
    q->isempty = 1;
    for (i = 0; i <= G->maxnum; i++)//赋初值 
    {
    	dist[i] = INF;
    	path[i] = -1;
	}
	dist[u] = 0;
    enQueue(q, u);//u进队 
    while (q->isempty == 0)
    {
    	e = deQueue(q);
    	visited[e] = 1;//标记该出队顶点已遍历相邻顶点 
    	t = G->adjlist[e].firstarc;
    	while (t != NULL)//访问所有与e相连的顶点
    	{
    		//printf("%d %d %d\n", dist[e], t->weight, dist[t->adjvex]);//测试 
    		if (dist[e] + t->weight < dist[t->adjvex])//贪心 
    		{
    			dist[t->adjvex] = dist[e] + t->weight;
    			path[t->adjvex] = e;
    			if (visited[t->adjvex] == 0)//若t所指顶点未遍历其相邻顶点则进队 
    			{
    				enQueue(q, t->adjvex);
				}
			}
			t = t->nextarc;
		}
	}
	if (path[v] == -1)//未找到路径直接返回 
	{
		return;
	}
	i = v;
	while (i != -1) 
	{
		temp[count] = i;//逆序记录路径 
		count++;
		i = path[i];
	}
	g_minnum = count;
	for (i = 0; i < count; i++)
	{
		g_pathmin[i] = temp[count - i - 1];//保存最短路径 
	}
}

char* shortestPath(int u, int v, char algorithm[], char name[])
{
    AdjGraph *G;
    int i, j, k;
    //调用stats里面的函数把图的邻接表做出来 
    
    G = createAdjGraph(name, G);
    
    for(i = 0; i <= G->maxnum; i++)//visit数组，g_pathmin赋值
    {
        visited[i] = 0;
    } 
    
    g_minnum = INF;
    g_minlen = INF;
	
	printf("Using %s:\n", algorithm);
	if (strcmp(algorithm, "DFS") == 0 || strcmp(algorithm, "dfs") == 0)	
	{
		DFS(G, u, v, 0);
	}
	else if (strcmp(algorithm, "BFS") == 0 || strcmp(algorithm, "bfs") == 0)
	{
		BFS(G, u, v);
	}
	else if (strcmp(algorithm, "Dijkstra") == 0)
	{
		Dijkstra(G, u, v); 
	}
    
    destroyAdjGraph(G);
    
    //for (i = 0; i < g_minnum; i++)
    //{
    //	printf("%d ", g_pathmin[i]); 
	//}
	
	if (g_minnum == INF || g_minnum == 1) //未找到路径返回NULL 
	{
		printf("\tNo path exists!");
		return NULL;
	}
	
	printf("Path:");
    char *pathmin; //将路径以字符串形式储存 
    int length = 0; //记录所有顶点数字位数之和 
    int temp;
    for (i = 0; i < g_minnum; i++) //统计每一个顶点数值的位数 
    {
    	temp = g_pathmin[i];
    	if (temp == 0)//判断顶点数值是否为零 
		{
			length++;
			continue;	
		} 
    	while (temp != 0)
		{
			temp /= 10;
			length++;	
		} 
	}
	
    pathmin = (char *)malloc((4 * (g_minnum - 1) + length + 1) * sizeof(char)); //分配存储空间 
	int pos = 0; //记录当前存储位置 
	int temp_pos; //记录每个顶点数值的相对存储位置 
    for (i = 0; i < g_minnum; i++)
    {
        temp = g_pathmin[i];
        if (temp != 0)
        {
        	temp_pos = -1; 
		}
		else
		{
			temp_pos = 0;
		}
    	while (temp != 0)
		{
			temp /= 10;
			temp_pos++;
		}
		temp = g_pathmin[i];
		for (j = pos + temp_pos; j >= pos; j--) //逆序存储数值并生成对应的字符 
		{
			pathmin[j] = temp - temp /10 * 10 + '0';
			temp /= 10;
		}
		if (i != g_minnum - 1) //存入箭头指向 
		{
        	pathmin[pos + temp_pos + 1] = ' ';
        	pathmin[pos + temp_pos + 2] = '-';
        	pathmin[pos + temp_pos + 3] = '>';
        	pathmin[pos + temp_pos + 4] = ' ';
        	pos += (temp_pos + 5);
        }
        else //若为最后一个顶点则存入字符串结束标志 
        {
        	pathmin[pos + temp_pos + 1] = '\0';
		}
    }
    return pathmin;
}

//版权声明：本函数参考了CSDN博主「js_xj」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/js_xj/java/article/details/6253096
int strcmp(const char *strOne, const char *strTwo)//经过检测，本函数没有问题 
{
    /*if ((NULL == strOne) || (NULL == strTwo))
        throw"Invalid Arguments!";*/
    while((*strOne != '\0') && (*strTwo != '\0') && (*strOne == *strTwo))
    {
        strOne++;
        strTwo++;
    }
    return (*strOne - *strTwo);
}
