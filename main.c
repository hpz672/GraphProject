#include "search.c" 

int count(const char *str); //计算输入字符串的长度 

int main(int argc, char* argv[])
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
    {
    	printf("\nUsage: ./search-cli [OPTION]... [FILE]...\n");
    	printf("Target: To eliminate the Garlic Devil, and save the Network Safety Mainland!\n");
		printf("The grades are insignificant at all, but what counts most is the Glory of Defeating the Garlic Devil!\n\n"); 
		printf("Mandatory arguments to long options are mandatory for short options too.\n");
		printf("  -h/--help                      to show the helping list as here we are\n\n");
		printf("  -g/--graph <FILE_PATH>         to open the graph based on the given path\n");
	    printf("             -s/--stats          and show the related statistics of the graph\n");
	    printf("                                 such as edges, vertices, freeman and closeness\n\n");
	    printf("  -sp/--shortestpath             to calculate the shortest path in the graph with\n");
	    printf("      <SEARCH_PARAMS>            specific methods (including DFS, BFS and Dijkstra)\n");
	    printf("  -u <STARTING_POINT>            from a specific starting point\n");
	    printf("    -v <TARGET_POINT>            to a specific target point\n\n");
	} 
	else if (argc == 4 && strcmp(argv[0], "./search-cli") == 0 
	&& (strcmp(argv[1], "-g") == 0 || strcmp(argv[1], "--graph") == 0) 
	&& (strcmp(argv[3], "-s") == 0 || strcmp(argv[3], "--stats") == 0))
	{
		FILE *fp;
		fp = fopen(argv[2], "r");
		if (fp == NULL)
		{
			printf("Path not found! Please input again!\n");
			return 0;	
		}
		printf("\n\n");
		printf("    -edges: %d\n", numberOfEdges(argv[2]));
		printf("    -vertices: %d\n", numberOfVertices(argv[2]));
		printf("    -freeman: %.6f\n", freemanNetworkCentrality(argv[2]));
		printf("    -closeness: %.6f\n\n\n", closenessCentrality(argv[2], 1));
		fclose(fp);
	}
	else if (argc == 9 && strcmp(argv[0], "./search-cli") == 0 
	&& (strcmp(argv[1], "-g") == 0 || strcmp(argv[1], "--graph") == 0) 
	&& (strcmp(argv[3], "-sp") == 0 || strcmp(argv[3], "--shortestpath") == 0)
	&& strcmp(argv[5], "-u") == 0 && strcmp(argv[7], "-v") == 0)
	{
		FILE *fp;
		fp = fopen(argv[2], "r");
		if (fp == NULL) 
		{
			printf("Path not found! Please try again!\n");
			return 0;
		}
		if (strcmp(argv[4], "DFS") == 0 || strcmp(argv[4], "dfs") == 0 
		|| strcmp(argv[4], "BFS") == 0 || strcmp(argv[4], "bfs") == 0
		|| strcmp(argv[4], "Dijkstra") == 0)
		{
			int u = 0, v = 0;
			int i = 1;
			int index = 1;
			for (i = count(argv[6]) - 1; i >= 0; i--)
			{
				u += (*(argv[6] + i) - '0') * index;
				index *= 10;
			}
			index = 1;
			for (i = count(argv[8]) - 1; i >= 0; i--)
			{
				v += (*(argv[8] + i) - '0') * index;
				index *= 10;
			}
			printf("\n\t%s\n\n", shortestPath(u, v, argv[4], argv[2]));
		}
		else
		{
			printf("Invalid Method!\n");
			printf("We only accpept the kinds of methods below:\n");
			printf("  DFS(dfs)    BFS(bfs)    Dijkstra\n");
		}
	}
	else
	{
		printf("Command not found!\nPlease input \n \t./search-cli -h/--help\nto check the use of search-cli.\n");
	}
} 

int count(const char *str)
{
	int i = 0;
	while (*(str + i) != '\0')
	{
		i++; 
	}
	return i;	
} 
