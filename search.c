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
int path[MAXV];//��¼·�� 
int temp[MAXV];

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
int Dijkstra(AdjGraph *G, int u, int v)
{
    g_minlen = INF;
    int tempWeight = 0;
    int mindis;
    int i, j, k, l;
    ArcNode *p;//�߽������ָ��
    
    for (i = 0; i < G->n; i++)
    {
        dist[i] = INF;//�����ʼ��
        path[i] = -1;//·����ʼ�� 
        temp[i] = -1;//temp������ʱ�洢u��v����·������ʼ�� 
    }
    visited[u] = 1;	//�Ҳ���ΪʲôҪ��Դ��u�Ž�ȥ�����Ҹо�������һ���㶼����//Դ��u����S��
    for (i = 0; i < G->n; i++)//�������ж��� 
    {
        p = G->adjlist[i].firstarc;//pָ�򶥵�i�ĵ�һ�����ڵ�
        mindis = INF;
        while (p != NULL)//��һ��ѭ��������i��������ĵ� 
        {
            j = p->adjvex;//����ĵ㶥���ż�¼Ϊj 
            if (visited[j] == 0 && dist[j] < mindis)//jû�б����ʹ������뼯��visited�ľ����С 
            {
                k = j;
                mindis = dist[j];//���¶���;�����Ϣ 
            }
            p=p->nextarc;//pָ����һ����� 
        }  
    	visited[k] = 1;	//�Ѿ��ҵ�����k�����Ǿ��뼯��visited������ĵ㣬��k����visited��
    	p = G->adjlist[k].firstarc;//pָ�򶥵�k�ĵ�һ�����ڵ�
    	while (p != NULL)//�޸Ĳ���visited�еĶ���ľ��루����visited�ж���֮��ľ����Ѿ�������̵��ˣ� 
    	{
    	    j = p->adjvex;//p��ָ��Ķ�����Ϊ��Ϊj 
    	    tempWeight = p->weight;//tempWeightΪk��j��Ȩֵ 
    	    if (visited[j] == 0)//���jû���ҵ������·����������� 
    	    {
    	        if (dist[k] + tempWeight < dist[j])//�ڽӱ����治����tempWeightΪINF�����  
                {
                    dist[j] = dist[k] + tempWeight;//�޸�·��Ȩֵ 
                    path[j] = k;//�޸�j��ǰһ������ 
                }
            }
            p=p->nextarc;//pָ����һ�����
        }
    }
    //������·��
    //���׼��д�������������������дһ��β�ݹ麯���������Ͳ�Ҫ���鸳ֵ�� 
    //β�ݹ麯��ʹ�� 
    /*g_min = 0;
    DijPath(path, u, v);*/ 
    i = v;
    j = 0;
    while (i != u)//�����ж����ҵ�����ʱ�ǵ��õ� 
    {
        j++;
        temp[MAXV - j] = path[i];
        i = path[i];
    }
    g_minnum = j;//������� 
    for (i = 0; i < j; i++)//�����ж���������������ȫ�ֱ��������� 
    {
        g_pathmin[i] = temp[MAXV - j + i];
    }
    return 1;//�ɹ����������� 
}

/*void DijPath(int path[], int u, int v)//β�ݹ麯������ֵ 
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
