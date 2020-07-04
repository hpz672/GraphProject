#include "search.c" 

int count(const char *str);//计算输入字符串的长度 
int charToNumber(const char *str);//将字符串转换为数字 

int main(int argc, char* argv[])
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
    {
    	printf("\nUsage: ./search-cli [OPTION]... [FILE]...\n");
    	printf("Target: To eliminate the Garlic Devil, and save the Network Safety Mainland!\n");
		printf("The grades are insignificant at all, but what counts most is the Glory of Defeating the Garlic Devil!\n\n"); 
		printf("Mandatory arguments to long options are mandatory for short options too.\n");
		printf("        -h/--help                    to show the helping list as here we are\n\n");
		printf("  -g/--graph <FILE_PATH>             to open the graph based on the given path\n");
	    printf("        -s/--stats                   and show the related statistics of the graph\n");
	    printf("      <STATS_PARAMS>                 remember to input the parameters in the last position\n");
	    printf("(-egdes/-vertices/-freeman/)         such as edges, vertices, freemanNetworkCentrality,\n");
	    printf("   (-closeness <node>)               and closenessCentrality of a certain node\n");
	    printf("                                     remember only one parameter for each time\n\n");
	    printf("    -sp/--shortestpath               to calculate the shortest path in the graph with\n");
	    printf("     <SEARCH_PARAMS>                 specific methods (including DFS, BFS and Dijkstra)\n");
	    printf("    -u <STARTING_POINT>              from a specific starting point\n");
	    printf("    -v <TARGET_POINT>                to a specific target point\n\n");
	} 
	else if (argc == 5 && strcmp(argv[0], "./search-cli") == 0 
	&& (strcmp(argv[1], "-g") == 0 || strcmp(argv[1], "--graph") == 0) 
	&& (strcmp(argv[3], "-s") == 0 || strcmp(argv[3], "--stats") == 0)
	&& (strcmp(argv[4], "-edges") == 0 || strcmp(argv[4], "-vertices") == 0 
	|| strcmp(argv[4], "-freeman") == 0)
	|| argc == 6 && strcmp(argv[0], "./search-cli") == 0 
	&& (strcmp(argv[1], "-g") == 0 || strcmp(argv[1], "--graph") == 0) 
	&& (strcmp(argv[3], "-s") == 0 || strcmp(argv[3], "--stats") == 0)
	&& strcmp(argv[4], "-closeness") == 0)
	{
		FILE *fp;
		fp = fopen(argv[2], "r");
		if (fp == NULL)
		{
			printf("Path not found! Please input again!\n");
			return 0;	
		}
		printf("\n");
		if (strcmp(argv[4], "-edges") == 0)
		{
			printf("    Edges: %d\n\n", numberOfEdges(argv[2]));
		}
		else if (strcmp(argv[4], "-vertices") == 0)
        {
        	printf("    Vertices: %d\n\n", numberOfVertices(argv[2]));
		}
        else if (strcmp(argv[4], "-freeman") == 0)
        {
        	printf("    Freeman Network Centrality: %.6f\n\n", freemanNetworkCentrality(argv[2]));
		}
		else
		{
			int n = charToNumber(argv[5]);
			printf("    Closeness Centrality of Node %s: %.6f\n\n", argv[5], closenessCentrality(argv[2], n));
		}
		
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
			int u = charToNumber(argv[6]);
			int v = charToNumber(argv[8]); 
			printf("%s\n\n", shortestPath(u, v, argv[4], argv[2]));
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

int charToNumber(const char *s)
{
	int n = 0;
	int i = 1;
	int index = 1;
	for (i = count(s) - 1; i >= 0; i--)
	{
		n += (*(s + i) - '0') * index;
		index *= 10;
	}
	return n;
}
