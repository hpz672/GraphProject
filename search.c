#include <stdlib.h>
#include <stdio.h>
#include "stats.h" 
#include "stats.c"
#include "search.h"

int visited[MAXV] = {0};//���ڱ�Ǹö����Ƿ񱻷���
int g_minnum;//�����·���а����Ķ�����Ŀ 
int g_minlen;//�����·������ 
int g_pathmin[MAXV];//�洢��u��v�����·��������� 
int dist[MAXV];//��¼�����㵽u�ľ��� 
int path[MAXV];//��¼char����·�� 
int temp[MAXV];//��¼����·�� 

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
int BFS(AdjGraph *G,int u,int v)//�󶥵�u������v�����·��
{//���ôӶ���u���������������������������������vʱ���ڶ������ҳ���Ӧ��·��
    struct{
        int vno;//��ǰ������
        int level;//��ǰ����Ĳ��
        int parent;//��ǰ�����˫�׽���ڶ����е��±�
    }qu[MAXV];//����˳���ѭ������
    int qu_front = -1, qu_rear = -1;
    int k, lev, i, j;
    
    g_minlen = INF;
    ArcNode *p;
    visited[u] = 1;
    qu_rear++;//����u�ѷ��ʣ��������
    qu[qu_rear].vno = u;
    qu[qu_rear].level = 0;
    qu[qu_rear].parent = -1;
    while(qu_front<qu_rear)//�ӷǿ�ѭ��
    {
        qu_front++;
        k = qu[qu_front].vno;//���Ӷ���k
        lev = qu[qu_front].level;
        if(k == v)//������kΪ�յ�
        {
            i = 0;
            j = qu_front;
            while(j != -1)
            {
                g_pathmin[lev-i] = qu[j].vno;
                j = qu[j].parent;
                i++;
            }
            g_minlen = lev;//�ҵ�����v����¼����
            return 1;
        }
        p = G->adjlist[k].firstarc;//pָ�򶥵�k�ĵ�һ�����ڵ�
        while(p != NULL)           //��������k�����ڵ�
        {
            if(visited[p->adjvex] == 0)//��δ���ʹ�
            {
                visited[p->adjvex] = 1;
                qu_rear++;
                qu[qu_rear].vno = p->adjvex;//���ʹ������ڵ����
                qu[qu_rear].level = lev + 1;
                qu[qu_rear].parent = qu_front;
            }
            p = p->nextarc; //�ҵ�����k����һ�����ڵ�
        }
    }
    return 0;
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
		printf("\n\tNo path exists!");
		return NULL;
	}
	
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
