#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// 깊이 우선 순회
void depth_first_Traversal(int *graph, int num_vertex)
{
    int *visited = (int *)calloc(num_vertex + 1, sizeof(int));
    int *stack = (int *)malloc((num_vertex + 1) * sizeof(int));
    int top = -1;

    for (int start = 1; start <= num_vertex; start++) {
        if (!visited[start]) {
            stack[++top] = start;
            visited[start] = 1; 

            while (top != -1) {
                int vertex = stack[top--];
                // printf("Pop from stack: %d\n", vertex);

                // printf("Visit: %d\n", vertex);
				printf("%d ", vertex);

                for (int i = 1; i <= num_vertex; i++) {
                    if (*(graph + (vertex ) * num_vertex + (i )) == 1 && !visited[i]) {
                        // printf("Visited: %d\n", visited[i]);
                        stack[++top] = i;
                        visited[i] = 1; 
                        // printf("Push to stack: %d\n", i);
                    }
                }
                
                // printf("Current stack: ");
                // for (int i = 0; i <= top; i++) {
                //     printf("%d ", stack[i]);
                // }
                // printf("\n");
                // printf("Visited nodes: ");
                // for (int i = 1; i <= num_vertex; i++) {
                //     printf("%d ", visited[i]);
                // }
                // printf("\n");
            }
        }
    }

    free(visited);
    free(stack);
    printf("\n");
}


// 너비 우선 순회
void breadth_first_Traversal(int *graph, int num_vertex)
{
    int *visited = (int *)calloc(num_vertex + 1, sizeof(int));
    int *queue = (int *)malloc((num_vertex + 1) * sizeof(int));
    int front = 0, rear = 0;

    for (int start = 1; start <= num_vertex; start++) {
        if (!visited[start]) {
            queue[rear++] = start;
            visited[start] = 1;

            while (front != rear) {
                int vertex = queue[front++];
                // printf("Dequeue: %d\n", vertex);
                // printf("Visit: %d\n", vertex);

				printf("%d ", vertex);

                for (int i = 1; i <= num_vertex; i++) {
                    if (*(graph + (vertex) * num_vertex + (i)) == 1 && !visited[i]) {
                        queue[rear++] = i;
                        visited[i] = 1;
                        // printf("Enqueue: %d\n", i);
                    }
                }
                // printf("Current queue: ");
                // for (int i = front; i < rear; i++) {
                //     printf("%d ", queue[i]);
                // }
                // printf("\n");
                // printf("Visited nodes: ");
                // for (int i = 1; i <= num_vertex; i++) {
                //     printf("%d ", visited[i]);
                // }
                // printf("\n");
            }
        }
    }

    free(visited);
    free(queue);
    printf("\n");
}




////////////////////////////////////////////////////////////////////////////////
int *load_graph( char *filename, int *num_vertex)
{
	char str[100];
	int num;
	int from, to;
	
	FILE *fp = fopen( filename, "r");
	if (fp == NULL)
	{
		printf( "Error: cannot open file [%s]\n", filename);
		return NULL;
	}
	
	fscanf( fp, "%s%d", str, &num);
	assert( num > 0);
	assert( strcmp( str, "*Vertices") == 0);
	
	int *graph = (int *)calloc( (num+1) * (num+1), sizeof(int));
	
	if (graph == NULL) return NULL;
	
	fscanf( fp, "%s", str);
	assert( strcmp( str, "*Edges") == 0);
	
	int ret;
	
	while( 2 == fscanf( fp, "%d%d", &from, &to))
	{
		*(graph + from * num + to) = 1; // graph[from][to]
		*(graph + to * num + from) = 1;
	}
	
	fclose( fp);

	*num_vertex = num;
	
	return graph;
}

////////////////////////////////////////////////////////////////////////////////
void print_graph( int *graph, int num)
{
	int i, j;
	
	for (i = 1; i <= num; i++)
	{
		for (j = 1; j <= num; j++)
			printf( "%d\t", *(graph + i * num + j));
		
		printf( "\n");
	}
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int *graph;
	int num_vertex = 0;

	if (argc != 2)
	{
		printf( "Usage: %s FILE(.net)\n", argv[0]);
		return 2;
	}

	// .net 파일 읽어서 adjacent matrix로 저장
	// (num_vertex+1) * (num_vertex+1)의 2차원 배열
	graph = load_graph( argv[1], &num_vertex);
	if (graph == NULL) return 0;
	
	// 그래프 출력
	print_graph( graph, num_vertex);

	// 각각 스택과 큐를 사용
	// 배열을 사용하나, 스택이나 큐에 포함될 원소의 수는 정점의 수와 동일하므로 overflow 상태가 될 위험이 없음
	printf( "DFS : ");
	depth_first_Traversal( graph, num_vertex);

	printf( "BFS : ");
	breadth_first_Traversal( graph, num_vertex);
	
	free( graph);
	
	return 0;
}

