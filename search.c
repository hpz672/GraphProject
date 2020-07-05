#include <stdlib.h>
#include <stdio.h>
#include "search.h"
#include "stats.h" 
#include "stats.c"

int visited[MAXV] = {0};//���ڱ�Ǹö����Ƿ񱻷���
int g_minnum;//�����·���а����Ķ�����Ŀ 
int g_minlen;//�����·������ 
int g_pathmin[MAXV];//�洢��u��v�����·��������� 
int g_dist[MAXV];//��¼�����㵽u�ľ��� 
int g_path[MAXV];//��¼char����·�� 
int g_temp[MAXV];//��¼����·�� 
Box g_box[MAXV];//��¼���߷ָ�Ϊweight-1����������Զ���λ�� 

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
    
    g_path[d] = u;//��ǰ������ӵ�·����
    g_visited[u] = 1;//���ѷ��ʱ��
    
    if(u == v)//�ҵ��յ� 
    {
        for (i = 0; i <= d; i++) 
        {
            g_pathmin[i] = g_path[i];//��¼��ǰ·�� 
        }
        g_minnum = d + 1;//��¼·���ж������� 
        return;
    }
    p = G->adjlist[u].firstarc;//pָ�򶥵�u�ĵ�һ�����ڵ�
    while(p != NULL)
    {
        w = p->adjvex;//wΪu�����ڵ���
        if(g_visited[w] == 0)//���ö���δ��Ƿ��ʣ���ݹ����֮
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
	if (g_box[v].pre == -1)//δ�ҵ�·��ֱ�ӷ��� 
	{
		return;
	}
	i = v;
	g_minlen = 0;
	while (i != -1) 
	{
		g_minlen += g_box[i].weight; 
		g_temp[count] = i;//�����¼·�� 
		count++;
		i = g_box[i].pre;
	}
	g_minnum = count;
	for (i = 0; i < count; i++)
	{
		g_pathmin[i] = g_temp[count - i - 1];//�������·�� 
		//printf("%d: %d\n", temp[count - i - 1], b[temp[count - i - 1]].weight);
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
    //����stats����ĺ�����ͼ���ڽӱ������� 
    
    G = createAdjGraph(name, G);
    
    for(i = 0; i <= G->maxnum; i++)//visit���飬g_pathmin��ֵ
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
	
	if (g_minnum == INF || g_minnum == 1) //δ�ҵ�·������NULL 
	{
		printf("\tNo path exists!");
		return NULL;
	}
	
	if (strcmp(algorithm, "dfs") != 0 && strcmp(algorithm, "DFS") != 0)
	{
		printf("Cost: %d\n", g_minlen);
	}
	
	printf("Path: ");
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
