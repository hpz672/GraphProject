#ifndef INF
#define INF 2147483647//���������
#endif

#ifndef MAXV 
#define MAXV 1000000//��󶥵����
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
#endif           /*_AdjGraph_H_*/

static void DFS(AdjGraph *G, int u, int v, int d);//���ͼ�дӶ���u��v��������̵�·��
int BFS(AdjGraph *G, int u, int v);//�󶥵�u������v�����·��
int Dijkstra(AdjGraph *G, int u, int v);
int strcmp(const char *strOne, const char *strTwo); 
void DijPath(int path[], int u, int v);//�ݹ�������·������ 
