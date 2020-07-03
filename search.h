#ifndef INF
#define INF 2147483647//定义无穷大
#endif

#ifndef MAXV 
#define MAXV 1000000//最大顶点个数
#endif

#ifndef _AdjGraph_H_
#define _AdjGraph_H_
//以下定义邻接表的类型 
typedef struct ArcNode {
    int adjvex;//该边的邻接点编号
    struct ArcNode *nextarc;//指向下一条边的指针
    int weight;//该边的相关信息,如权值（用整型表示）
}ArcNode;//边结点类型

typedef struct VNode {
    char info;//顶点信息
    ArcNode *firstarc;//指向第一条边
}VNode;//邻接表结点类型

typedef struct Graph {
    VNode adjlist[MAXV];//邻接表头结点数组
    int n;//图中顶点数
    int e;//图中边数
    int maxnum;//图中最大顶点值 
}AdjGraph;//完整的图邻接表类型
#endif           /*_AdjGraph_H_*/

static void DFS(AdjGraph *G, int u, int v, int d);//输出图中从顶点u到v的所有最短的路径
int BFS(AdjGraph *G, int u, int v);//求顶点u到顶点v的最短路径
int Dijkstra(AdjGraph *G, int u, int v);
int strcmp(const char *strOne, const char *strTwo); 
void DijPath(int path[], int u, int v);//递归输出最短路径函数 
