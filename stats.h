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
#endif           /*_AdjGraph_H_*/

#ifndef _STATS_H_ 
#define _STATS_H_
AdjGraph* createAdjGraph(char name[], AdjGraph *G);
void destroyAdjGraph(AdjGraph *G);
int numberOfEdges(char name[]);
int numberOfVertices(char name[]);
float freemanNetworkCentrality(char name[]);
float closenessCentrality(char name[], int node);
#endif           /*_Stats_H_*/
