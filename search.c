#include <stdlib.h>
#include <stdio.h>
#include "stats.h" 
#include "stats.c"
#include "search.h"

int visited[MAXV] = {0};//用于标记该顶点是否被访问
int g_minnum;//求最短路径中包含的顶点数目 
int g_minlen;//求最短路径长度 
int g_pathmin[MAXV];//存储由u到v的最短路径顶点序号 
int dist[MAXV];//记录各顶点到u的距离 
int path[MAXV];//记录路径 
int temp[MAXV];

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
int BFS(AdjGraph *G,int u,int v)//求顶点u到顶点v的最短路径
{//采用从顶点u出发广度优先搜索方法，当搜索到顶点v时，在队列中找出对应的路径
    struct{
        int vno;//当前顶点编号
        int level;//当前顶点的层次
        int parent;//当前顶点的双亲结点在队列中的下标
    }qu[MAXV];//定义顺序非循环队列
    int qu_front = -1, qu_rear = -1;
    int k, lev, i, j;
    
    g_minlen = INF;
    ArcNode *p;
    visited[u] = 1;
    qu_rear++;//顶点u已访问，将其入队
    qu[qu_rear].vno = u;
    qu[qu_rear].level = 0;
    qu[qu_rear].parent = -1;
    while(qu_front<qu_rear)//队非空循环
    {
        qu_front++;
        k = qu[qu_front].vno;//出队顶点k
        lev = qu[qu_front].level;
        if(k == v)//若顶点k为终点
        {
            i = 0;
            j = qu_front;
            while(j != -1)
            {
                g_pathmin[lev-i] = qu[j].vno;
                j = qu[j].parent;
                i++;
            }
            g_minlen = lev;//找到顶点v，记录其层次
            return 1;
        }
        p = G->adjlist[k].firstarc;//p指向顶点k的第一个相邻点
        while(p != NULL)           //依次搜索k的相邻点
        {
            if(visited[p->adjvex] == 0)//若未访问过
            {
                visited[p->adjvex] = 1;
                qu_rear++;
                qu[qu_rear].vno = p->adjvex;//访问过的相邻点进队
                qu[qu_rear].level = lev + 1;
                qu[qu_rear].parent = qu_front;
            }
            p = p->nextarc; //找到顶点k的下一个相邻点
        }
    }
    return 0;
}

//采用邻接表的Dijkstra算法 
int Dijkstra(AdjGraph *G, int u, int v)
{
    g_minlen = INF;
    int tempWeight = 0;
    int mindis;
    int i, j, k, l;
    ArcNode *p;//边结点类型指针
    
    for (i = 0; i < G->n; i++)
    {
        dist[i] = INF;//距离初始化
        path[i] = -1;//路径初始化 
        temp[i] = -1;//temp用于临时存储u到v的逆路径，初始化 
    }
    visited[u] = 1;	//我不懂为什么要把源点u放进去……我感觉随便放入一个点都可以//源点u放入S中
    for (i = 0; i < G->n; i++)//遍历所有顶点 
    {
        p = G->adjlist[i].firstarc;//p指向顶点i的第一个相邻点
        mindis = INF;
        while (p != NULL)//第一个循环找与结点i距离最近的点 
        {
            j = p->adjvex;//最近的点顶点编号记录为j 
            if (visited[j] == 0 && dist[j] < mindis)//j没有被访问过并且与集合visited的距离更小 
            {
                k = j;
                mindis = dist[j];//更新顶点和距离信息 
            }
            p=p->nextarc;//p指向下一个结点 
        }  
    	visited[k] = 1;	//已经找到顶点k现在是距离集合visited中最近的点，将k加入visited中
    	p = G->adjlist[k].firstarc;//p指向顶点k的第一个相邻点
    	while (p != NULL)//修改不在visited中的顶点的距离（集合visited中顶点之间的距离已经都是最短的了） 
    	{
    	    j = p->adjvex;//p所指向的顶点编号为记为j 
    	    tempWeight = p->weight;//tempWeight为k到j的权值 
    	    if (visited[j] == 0)//如果j没有找到过最短路径，考察距离 
    	    {
    	        if (dist[k] + tempWeight < dist[j])//邻接表里面不存在tempWeight为INF的情况  
                {
                    dist[j] = dist[k] + tempWeight;//修改路径权值 
                    path[j] = k;//修改j的前一个顶点 
                }
            }
            p=p->nextarc;//p指向下一个结点
        }
    }
    //输出最短路径
    //如果准许写其他函数，这里可以另写一个尾递归函数，这样就不要两遍赋值了 
    //尾递归函数使用 
    /*g_min = 0;
    DijPath(path, u, v);*/ 
    i = v;
    j = 0;
    while (i != u)//将所有顶点找到，此时是倒置的 
    {
        j++;
        temp[MAXV - j] = path[i];
        i = path[i];
    }
    g_minnum = j;//顶点个数 
    for (i = 0; i < j; i++)//将所有顶点正过来，放入全局变量数组内 
    {
        g_pathmin[i] = temp[MAXV - j + i];
    }
    return 1;//成功返回主函数 
}

/*void DijPath(int path[], int u, int v)//尾递归函数将赋值 
{
    if(path[u] != v)
    {
        dijPath(path, path[u] ,v);
    }
    g_pathmin[g_min] = u;
}*/

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
		printf("\n\tNo path exists!");
		return NULL;
	}
	
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
