int visited[MAXV];
int dist[MAXV];

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
		visited[MAXV] = 0;
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
		if (visited[x] == 0)
		{
			visited[x] = 1;
			n++;
		}
		if (visited[y] == 0)
		{
			visited[y] = 1;
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
		visited[i] = 0;
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
	float max = 0;
	float r;
	G = createAdjGraph(name, G);
	int save[MAXV] = {0};
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
				save[i] += 2;
				save[p->adjvex] += 2;
			}
			else
			{
				save[i] += 1;
				save[p->adjvex] += 1;
			}
			if (save[i] > max)
			{
				max = save[i];
			}
			if (save[p->adjvex] > max)
			{
				max = save[p->adjvex];
			}
			p = p->nextarc;
		}
	}
	for (i = 0; i <= G->maxnum; i++)
	{
		if (save[i] != 0)
		{
			num += (max - save[i]);
		}
	}
	r = (float)num / (G->n - 1) / (G->n - 2) / 2;
	destroyAdjGraph(G);
	return r;
}

float closenessCentrality(char name[], int node)
{
	AdjGraph *G;
	G = createAdjGraph(name, G);
	int i;
	int total = 0;
	float r;
	Dijkstra(G, node, i);
	for (i = 0; i <= G->maxnum; i++)
	{
		if (dist[i] != INF)
		{
			total += dist[i];
		}
	}
	r = 1.0 * (G->n - 1) / total;
	return r;
} 
