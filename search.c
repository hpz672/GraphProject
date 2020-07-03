#include <stdlib.h>
#include <stdio.h>
#include "search.h"
#include "stats.h" 
#include "stats.c"

int visited[MAXV] = {0};//���ڱ�Ǹö����Ƿ񱻷���
int g_minnum;//�����·���а����Ķ�����Ŀ 
int g_minlen;//�����·������ 
int g_pathmin[MAXV];//�洢��u��v�����·��������� 
int dist[MAXV];//��¼�����㵽u�ľ��� 
int path[MAXV];//��¼char����·�� 
int temp[MAXV];//��¼����·�� 
Box b[MAXV];//��¼���߷ָ�Ϊweight-1����������Զ���λ�� 

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

//�����ڽӱ��DFS�㷨�� 
static void DFS(AdjGraph *G, int u, int v, int d)//���һ���Ӷ���u��v��·��
{
    if (g_minnum != INF) //�ҵ�·������ 
	{
		return;	
	} 
	int i, w;
    ArcNode *p;//�߽������ָ��
    
    path[d] = u;//��ǰ������ӵ�·����
    visited[u] = 1;//���ѷ��ʱ��
    
    if(u == v)//�ҵ��յ� 
    {
        for (i = 0; i <= d; i++) 
        {
            g_pathmin[i] = path[i];//��¼��ǰ·�� 
        }
        g_minnum = d + 1;//��¼·���ж������� 
        return;
    }
    p = G->adjlist[u].firstarc;//pָ�򶥵�u�ĵ�һ�����ڵ�
    while(p != NULL)
    {
        w = p->adjvex;//wΪu�����ڵ���
        if(visited[w] == 0)//���ö���δ��Ƿ��ʣ���ݹ����֮
        {
        	//printf("%d %d\n", u, w); //���� 
        	DFS(G, w, v, d + 1);
		}
        p = p->nextarc;//��u����һ�����ڵ�
    }
}

//�����ڽӱ��BFS�㷨��
void BFS(AdjGraph *G, int u, int v)//�󶥵�u������v�����·��
{//���ôӶ���u���������������������������������vʱ���ڶ������ҳ���Ӧ��·��
 //��ÿ��ȨֵΪweight�ı߲��Ϊweight-1�����㣬�Ӷ�ת��Ϊ�ȼ��BFS���� 
    int i, e;
    int count = 0;
	ArcNode *t;
    Queue *q;
    q = (Queue*)malloc(sizeof(Queue));
    q->rear = q->front = 0;
    q->isempty = 1;
    for (i = 0; i <= G->maxnum; i++) //����ֵ 
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
	if (b[v].pre == -1)//δ�ҵ�·��ֱ�ӷ��� 
	{
		return;
	}
	i = v;
	while (i != -1) 
	{
		temp[count] = i;//�����¼·�� 
		count++;
		i = b[i].pre;
	}
	g_minnum = count;
	for (i = 0; i < count; i++)
	{
		g_pathmin[i] = temp[count - i - 1];//�������·�� 
	}
}

//�����ڽӱ��Dijkstra�㷨 
void Dijkstra(AdjGraph *G, int u, int v)
{
    int i, e;
    int count = 0;
    ArcNode *t;
    Queue *q;
    q = (Queue*)malloc(sizeof(Queue));
    q->rear = q->front = 0;
    q->isempty = 1;
    for (i = 0; i <= G->maxnum; i++)//����ֵ 
    {
    	dist[i] = INF;
    	path[i] = -1;
	}
	dist[u] = 0;
    enQueue(q, u);//u���� 
    while (q->isempty == 0)
    {
    	e = deQueue(q);
    	visited[e] = 1;//��Ǹó��Ӷ����ѱ������ڶ��� 
    	t = G->adjlist[e].firstarc;
    	while (t != NULL)//����������e�����Ķ���
    	{
    		//printf("%d %d %d\n", dist[e], t->weight, dist[t->adjvex]);//���� 
    		if (dist[e] + t->weight < dist[t->adjvex])//̰�� 
    		{
    			dist[t->adjvex] = dist[e] + t->weight;
    			path[t->adjvex] = e;
    			if (visited[t->adjvex] == 0)//��t��ָ����δ���������ڶ�������� 
    			{
    				enQueue(q, t->adjvex);
				}
			}
			t = t->nextarc;
		}
	}
	if (path[v] == -1)//δ�ҵ�·��ֱ�ӷ��� 
	{
		return;
	}
	i = v;
	while (i != -1) 
	{
		temp[count] = i;//�����¼·�� 
		count++;
		i = path[i];
	}
	g_minnum = count;
	for (i = 0; i < count; i++)
	{
		g_pathmin[i] = temp[count - i - 1];//�������·�� 
	}
}

char* shortestPath(int u, int v, char algorithm[], char name[])
{
    AdjGraph *G;
    int i, j, k;
    //����stats����ĺ�����ͼ���ڽӱ������� 
    
    G = createAdjGraph(name, G);
    
    for(i = 0; i <= G->maxnum; i++)//visit���飬g_pathmin��ֵ
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
	
	if (g_minnum == INF || g_minnum == 1) //δ�ҵ�·������NULL 
	{
		printf("\tNo path exists!");
		return NULL;
	}
	
	printf("Path:");
    char *pathmin; //��·�����ַ�����ʽ���� 
    int length = 0; //��¼���ж�������λ��֮�� 
    int temp;
    for (i = 0; i < g_minnum; i++) //ͳ��ÿһ��������ֵ��λ�� 
    {
    	temp = g_pathmin[i];
    	if (temp == 0)//�ж϶�����ֵ�Ƿ�Ϊ�� 
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
	
    pathmin = (char *)malloc((4 * (g_minnum - 1) + length + 1) * sizeof(char)); //����洢�ռ� 
	int pos = 0; //��¼��ǰ�洢λ�� 
	int temp_pos; //��¼ÿ��������ֵ����Դ洢λ�� 
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
		for (j = pos + temp_pos; j >= pos; j--) //����洢��ֵ�����ɶ�Ӧ���ַ� 
		{
			pathmin[j] = temp - temp /10 * 10 + '0';
			temp /= 10;
		}
		if (i != g_minnum - 1) //�����ͷָ�� 
		{
        	pathmin[pos + temp_pos + 1] = ' ';
        	pathmin[pos + temp_pos + 2] = '-';
        	pathmin[pos + temp_pos + 3] = '>';
        	pathmin[pos + temp_pos + 4] = ' ';
        	pos += (temp_pos + 5);
        }
        else //��Ϊ���һ������������ַ���������־ 
        {
        	pathmin[pos + temp_pos + 1] = '\0';
		}
    }
    return pathmin;
}

//��Ȩ�������������ο���CSDN������js_xj����ԭ�����£���ѭCC 4.0 BY-SA��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
//ԭ�����ӣ�https://blog.csdn.net/js_xj/java/article/details/6253096
int strcmp(const char *strOne, const char *strTwo)//������⣬������û������ 
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
