#include <stdlib.h>
#include <stdio.h>
#include "search.h"
#include "stats.h" 
#include "stats.c"

int visited[MAXV] = {0};//用于标记该顶点是否被访问
int g_minnum;//求最短路径中包含的顶点数目 
int g_minlen;//求最短路径长度 
int g_pathmin[MAXV];//存储由u到v的最短路径顶点序号 
int g_dist[MAXV];//记录各顶点到u的距离 
int g_path[MAXV];//记录char类型路径 
int g_temp[MAXV];//记录逆序路径 
Box g_box[MAXV];//记录将边分割为weight-1个顶点后的相对顶点位置 

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
    
    g_path[d] = u;//当前顶点添加到路径中
    g_visited[u] = 1;//置已访问标记
    
    if(u == v)//找到终点 
    {
        for (i = 0; i <= d; i++) 
        {
            g_pathmin[i] = g_path[i];//记录当前路径 
        }
        g_minnum = d + 1;//记录路径中顶点数量 
        return;
    }
    p = G->adjlist[u].firstarc;//p指向顶点u的第一个相邻点
    while(p != NULL)
    {
        w = p->adjvex;//w为u的相邻点编号
        if(g_visited[w] == 0)//若该顶点未标记访问，则递归访问之
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
    int i, e;
    int count = 0;
	ArcNode *t;
    Queue *q;
    q = (Queue*)malloc(sizeof(Queue));
    q->rear = q->front = 0;
    q->isempty = 1;
    for (i = 0; i <= G->maxnum; i++)
    {
    	g_box[i].pre = -1;
    	g_box[i].current = 1;
	}
    g_visited[u] = 1;
    g_box[u].weight = 0;
    enQueue(q, u);
    while (q->isempty == 0)
    {
    	e = deQueue(q);
    	if (e == v && g_box[e].current == g_box[e].weight)
    	{
    		break;
		}
    	if (g_box[e].current >= g_box[e].weight)
    	{
    		t = G->adjlist[e].firstarc;
    		while (t != NULL)
    		{
    			if (g_visited[t->adjvex] == 0 || t->weight <= g_box[t->adjvex].weight - g_box[t->adjvex].current)
    			{
    				g_box[t->adjvex].weight = t->weight;
    				g_box[t->adjvex].current = 1;
    				g_box[t->adjvex].pre = e;
    				if (g_visited[t->adjvex] == 0)
    				{
    					g_visited[t->adjvex] = 1;
    					enQueue(q, t->adjvex);
					}
				}
				t = t->nextarc;
			}
		}
		else
		{
			g_box[e].current++;
			enQueue(q, e);
		}
	}
	if (g_box[v].pre == -1)//未找到路径直接返回 
	{
		return;
	}
	i = v;
	g_minlen = 0;
	while (i != -1) 
	{
		g_minlen += g_box[i].weight; 
		g_temp[count] = i;//逆序记录路径 
		count++;
		i = g_box[i].pre;
	}
	g_minnum = count;
	for (i = 0; i < count; i++)
	{
		g_pathmin[i] = g_temp[count - i - 1];//保存最短路径 
		//printf("%d: %d\n", temp[count - i - 1], b[temp[count - i - 1]].weight);
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
    for (i = 0; i <= G->maxnum; i++)
    {
    	g_dist[i] = INF;
    	g_path[i] = -1;
	}
	g_dist[u] = 0;
    enQueue(q, u);
    while (q->isempty == 0)
    {
    	e = deQueue(q);
    	t = G->adjlist[e].firstarc;
    	while (t != NULL)
    	{
    		//printf("%d %d %d\n", dist[e], t->weight, dist[t->adjvex]);
    		if (g_dist[e] + t->weight < g_dist[t->adjvex])
    		{
    			g_dist[t->adjvex] = g_dist[e] + t->weight;
    			g_path[t->adjvex] = e;
    			enQueue(q, t->adjvex);
			}
			t = t->nextarc;
		}
	}
	if (g_path[v] == -1)
	{
		return;
	}
	i = v;
	while (i != -1)
	{
		g_temp[count] = i;
		count++;
		i = g_path[i];
	}
	g_minnum = count;
	g_minlen = g_dist[v];
	for (i = 0; i < count; i++)
	{
		g_pathmin[i] = g_temp[count - i - 1];
		//printf("%d: %d\n", temp[count - i - 1], dist[temp[count - i - 1]]);
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
        g_visited[i] = 0;
    } 
    
    g_minnum = INF;
    g_minlen = INF;
	
	printf("\nUsing %s:\n", algorithm);
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
	
	if (strcmp(algorithm, "dfs") != 0 && strcmp(algorithm, "DFS") != 0)
	{
		printf("Cost: %d\n", g_minlen);
	}
	
	printf("Path: ");
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
