int g_visited[MAXV];
int g_dist[MAXV];
int g_save[MAXV] = {0};

AdjGraph* createAdjGraph(char name[], AdjGraph *G)
{
	FILE *fp;
	ArcNode *t;
	char input; 
	int e = 0, n = 0;
	int x, y, weight;
	int prex = -1, prey = -1;
	int i;
	int maxnum = -1;
	G = (AdjGraph*)malloc(sizeof(AdjGraph));
	for (i = 0; i < MAXV; i++)
	{
		g_visited[MAXV] = 0;
		G->adjlist[i].firstarc = NULL;
	}
	fp = fopen(name, "r");
	while (!feof(fp))
	{
		fscanf(fp, "%d %d %d", &x, &y, &weight);
		if (x == prex && y == prey)
		{
			continue;
		}
		prex = x;
		prey = y;
		e++;
		t = (ArcNode*)malloc(sizeof(ArcNode));
		t->adjvex = y;
		t->weight = weight;
		if (g_visited[x] == 0)
		{
			g_visited[x] = 1;
			n++;
		}
		if (g_visited[y] == 0)
		{
			g_visited[y] = 1;
			n++;
		}
		if (maxnum < x)
		{
			maxnum = x;
		}
		if (maxnum < y)
		{
			maxnum = y;
		}
		t->nextarc = G->adjlist[x].firstarc;
		G->adjlist[x].firstarc = t; 
	}
	fclose(fp);
	G->e = e;
	G->n = n;
	G->maxnum = maxnum;
	for (i = 0; i <= maxnum; i++)
	{
		g_visited[i] = 0;
	}
	return G;
}

void destroyAdjGraph(AdjGraph *G)
{
	int i; 
	ArcNode *t, *pre;
	for (i = 0; i <= G->maxnum; i++)
	{
		pre = G->adjlist[i].firstarc;
		if (pre != NULL)
		{
		    t = pre->nextarc;
		    while (t != NULL)
    		{
    			free(pre);
    			pre = t;
    			t = t->nextarc;
    		}
    		free(pre);
        }
	}
	free(G);
}

int numberOfEdges(char name[])
{
	AdjGraph *G;
	int e;
	G = createAdjGraph(name, G);
	e = G->e;
	destroyAdjGraph(G);
	return e;
}

int numberOfVertices(char name[])
{
	AdjGraph *G;
	int n;
	G = createAdjGraph(name, G);
	n = G->n;
	destroyAdjGraph(G);
	return n;
}

float freemanNetworkCentrality(char name[])
{
	AdjGraph *G;
	ArcNode *p, *q;
	int num = 0;
	int i;
	double max = 0;
	double r;
	G = createAdjGraph(name, G);
	for (i = 0; i <= G->maxnum; i++)
	{
		p = G->adjlist[i].firstarc;
		while (p != NULL)
		{
			q = G->adjlist[p->adjvex].firstarc;
			while (q != NULL)
			{
				if (q->adjvex == i)
		        {
		        	break; 
				}
				q = q->nextarc; 
			}
			if (q == NULL)
			{
				g_save[i] += 2;
				g_save[p->adjvex] += 2;
			}
			else
			{
				g_save[i] += 1;
				g_save[p->adjvex] += 1;
			}
			if (g_save[i] > max)
			{
				max = g_save[i];
			}
			if (g_save[p->adjvex] > max)
			{
				max = g_save[p->adjvex];
			}
			p = p->nextarc;
		}
	}
	for (i = 0; i <= G->maxnum; i++)
	{
		if (g_save[i] != 0)
		{
			num += (max - g_save[i]);
		}
	}
	r = (double)num / (G->n - 1) / (G->n - 2) / 2;
	destroyAdjGraph(G);
	return r;
}

float closenessCentrality(char name[], int node)
{
	AdjGraph *G;
	G = createAdjGraph(name, G);
	int i;
	int total = 0;
	int count = 0;
	double r;
	Dijkstra(G, node, node);
	for (i = 0; i <= G->maxnum; i++)
	{
		if (g_dist[i] != INF)
		{
			total += g_dist[i];
			count++;
		}
	}
	r = 1.0 * (count - 1) / total;
	destroyAdjGraph(G); 
	return r;
} 
