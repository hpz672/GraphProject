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

#ifndef _Queue_
#define _Queue_
//以下定义队列相关类型 
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
#endif          /*_Queue_*/

void enQueue(Queue *q, int e);//进队
int deQueue(Queue *q);//出队 
static void DFS(AdjGraph *G, int u, int v, int d);//输出顶点u到v的一条路径
void BFS(AdjGraph *G, int u, int v);//求顶点u到顶点v的最短路径
void Dijkstra(AdjGraph *G, int u, int v);//求顶点u到顶点v的最短路径 
int strcmp(const char *strOne, const char *strTwo);//比较字符串 
