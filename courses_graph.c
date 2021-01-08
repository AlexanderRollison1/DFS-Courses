//Alexander Rollison

//NOTES: Everything should work as the rubric states. Correct mapping/printing of adj matrix as well as print in Topological order and cycle detection.
//gcc -o a.out courses_graph.c
//valgrind --leak-check=full ./a.out
//No valgrind errors
//If there are any problems please contact me (alexander.rollison@mavs.uta.edu)as my way of passing 2D arrays was different than how people normally pass 2D arrays (I know how to pass 2D arrays, first param 2nd size, second param 2d array).

//Special case file (special.txt): All courses have the same prereq so it's pointless to have a topo sort

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARRAY_LENGTH 20

int* DFS(int *adj, int count, int *pred,int *prednumarray);
int DFS_visit(int *adj,int u,char *color,int *pred, int count, int *prednum, int* prednumarray);

int main(int argc, char *argv[])
{
	int i;
	int j;
	char filename[30];
	char mode[2] = "r+";
	printf("\nEnter the filename: ");
	scanf("%s",filename);
	
	FILE* fp = NULL;
	fp = fopen(filename,mode);
	if(fp == NULL)
	{
		printf("\nA File you entered does not exist. Exiting...\n");
		exit(0);
	}
	
	char course[MAX_ARRAY_LENGTH][30];
	char index[1000];
	int count = 0;
	char *token = NULL;
	
	while(fgets(index, sizeof(index)-1,fp))
	{
		if(index[strlen(index)-1] == '\n')
		{
			index[strlen(index)-1] = '\0';
		}
		token = strtok(index," ");
		strcpy(course[count], token);
		count++;
	}
	rewind(fp);
	
	int checker = 0;
	while(fgets(index, sizeof(index)-1,fp))
	{
		
		//makes sure the end of the string is null terminator
		if(index[strlen(index)-1] == '\n')
		{
			index[strlen(index)-1] = '\0';
		}
		
		//first word into token
		token = strtok(index," \0");
		while(token)
		{
			checker = 0;
			for(i = 0; i < count-1; i++)
			{
				if(strcmp(course[i], token) != 0)
				{
					checker++;
				}
			}
			if(checker == count-1)
			{
				strcpy(course[count],token);
				count++;
			}
			token = strtok(NULL," ");
		}
	}
	rewind(fp);
	
	printf("Number of vertices in built graph: N = %d",count-1);
	printf("\nVertex - coursename correspondence\n");
	for(i = 0; i < count-1; i++)
	{
		printf("%d - %s\n",i, course[i]);
	}
	printf("\n");
	
	
	
	
	int adj[count-1][count-1];
	int r;
	int c;
	
	for(i = 0; i < count-1; i++)
	{
		for(j = 0; j < count-1; j++)
		{
			adj[i][j] = 0;
		}
	}
	
	
	while(fgets(index,sizeof(index)-1,fp))
	{
		if(index[strlen(index)-1] == '\n')
		{
			index[strlen(index)-1] = '\0';
		}
		token = strtok(index," ");
		
		for(i = 0; i < count-1; i++)
		{
			if(strcmp(course[i], token) == 0)
			{
				c = i;
				i = count;
			}
		}
		
		token = strtok(NULL," ");
		while(token)
		{
			for(i = 0; i < count; i++)
			{
				if(strcmp(course[i], token) == 0)
				{
					r = i;
					i = count;
				}
			}
			adj[r][c] = 1;
			token = strtok(NULL, " ");
		}
	}
	
	
	printf("Adjacency Matrix:\n");
	for(i = 0; i < count-1; i++)
	{
		if(i == 0)
		{
			printf("    |");
			printf("%4d", i);
		}
		else
		{
			printf("%4d", i);
		}
	}
	printf("\n-");
	for(i = 0; i < count*4; i++)
	{
		printf("-");
	}
	printf("\n");
	for(i = 0; i < count-1; i++)
	{
		printf("%4d|", i);
		for(j = 0; j < count-1; j++)
		{
			printf("%4d", adj[i][j]);
		}
		printf("\n");
	}
	
	int pred[count-1];
	int prednumarray[count-1];
	int * arr = DFS(&adj[0][0],count-1, pred, prednumarray);
	if(arr == NULL)
	{
		printf("\nThere was at least one cycle. There is no possible ordering of the courses.\n");
	}
	else
	{
		printf("\n\n");
		printf("Order in which to take courses:\n");
		for(i = 0; i < count-1; i++)
		{
			printf("%d. - %s (corresponds to graph vertex %d)\n", i, course[arr[(count-2)-i]],arr[(count-2)-i]);
		}
	}
	
	fclose(fp);
	return 0;
}

int* DFS(int *adj, int count, int *pred, int *prednumarray)
{
	
	char color[count];
	int u;
	int check = 1;
	int prednum = 0;
	for(u = 0; u < count; u++)
	{
		color[u] = 'w';
		pred[u] = 0;
		prednumarray[u] = 0;
	}
	
	
	for(u = 0; u < count; u++)
	{
		if(color[u] == 'w')
		{
			int check = DFS_visit(adj,u,color,pred,count,&prednum, prednumarray);
			if(check == 1)
			{
				return NULL;
			}
		}
	}
	//return pred;
	return prednumarray;
}

int DFS_visit(int *adj,int u,char *color,int *pred, int count, int *prednum, int *prednumarray)
{
	int check;
	if(color[u] == 'g')
	{
		check = 1;
		return 1;
	}
	else
	{
		check = 0;
	}
	color[u] = 'g';
	//for each v adjacent to u (for any v that has an edge with u (u,v)
	int v;
	//printf("\nU = %d --> LINE AT %d --> ", u, u);
	for(v = 0; v < count; v++)
	{
		//printf("%4d|", adj[v*count + u]);
		if(color[v] == 'g' && adj[u*count + v] == 1)
		{
			check = 1;
			return 1;
		}
		
		if (color[v] == 'w' && adj[u*count + v] == 1)
		{
			//printf("\n--|v=%d and u=%d for %d|--",v,u,adj[v*count + u]);
			pred[v] = u;
			check = DFS_visit(adj,v,color,pred,count,prednum, prednumarray);
		}
	}
	//printf("\nnext-DFS-");
	color[u] = 'b';
	prednumarray[*prednum] = u;
	*prednum += 1;
	
	return check;
}
