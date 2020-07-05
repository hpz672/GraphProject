#ifndef INF
#define INF 2147483647//���������
#endif

#ifndef MAXV 
#define MAXV 5000000//��󶥵����
#endif

#ifndef _AdjGraph_H_
#define _AdjGraph_H_
//���¶����ڽӱ������ 
typedef struct ArcNode {
    int adjvex;//�ñߵ��ڽӵ���
    struct ArcNode *nextarc;//ָ����һ���ߵ�ָ��
    int weight;//�ñߵ������Ϣ,��Ȩֵ�������ͱ�ʾ��
}ArcNode;//�߽������

typedef struct VNode {
    char info;//������Ϣ
    ArcNode *firstarc;//ָ���һ����
}VNode;//�ڽӱ�������

typedef struct Graph {
    VNode adjlist[MAXV];//�ڽӱ�ͷ�������
    int n;//ͼ�ж�����
    int e;//ͼ�б���
    int maxnum;//ͼ����󶥵�ֵ 
}AdjGraph;//������ͼ�ڽӱ�����

AdjGraph* createAdjGraph(char name[], AdjGraph *G);
void destroyAdjGraph(AdjGraph *G);
#endif           /*_AdjGraph_H_*/

#ifndef _Queue_H_ 
#define _Queue_H_
//���¶������������� 
typedef struct{
	int data[MAXV];
	int front;
	int rear;
	int isempty;
}Queue;

typedef struct{
	int weight;
	int current;
	int pre; 
}Box;

void enQueue(Queue *q, int e);//����
int deQueue(Queue *q);//���� 
#endif          /*_Queue_H_*/

#ifndef _SEARCH_H_
#define _SEARCH_H_
static void DFS(AdjGraph *G, int u, int v, int d);//�������u��v��һ��·��
void BFS(AdjGraph *G, int u, int v);//�󶥵�u������v�����·��
void Dijkstra(AdjGraph *G, int u, int v);//�󶥵�u������v�����·�� 
char* shortestPath(int u, int v, char algorithm[], char name[]);//���ַ�����ʽ�������·�� 
#endif         /*_Search_H_*/

int strcmp(const char *strOne, const char *strTwo);//�Ƚ��ַ��� 
