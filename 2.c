#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define	MAX_COURSE 30
#define	MAX_COURSE_NAME 30

typedef struct t_Vertex {
	char	name[MAX_COURSE_NAME];
	int		color;
	bool	visited;
}Vertex;

/* variables for coloring */
int m = 0;
int size = 0;
int *vcolor;
bool print = false;
////////////////////////////

char	**courses;
int		coursecount = 0;
int		**adj = NULL;	   // 노드간의 연결 상태를 파악하기 위한 인접행렬 선언

bool	is_exist(char *name);
void create_edge2(int **ret, char list[30][30], int index);

void	read_File(FILE *ptr)
{
	char	input[256], word[256];
	char	*token;

	while (!feof(ptr)) { // file의 끝에 도달하면 0 이 아닌 값을 반환
		if (fgets(input, 512, ptr) != NULL) {
			strcpy(word, strtok(input, "\n")); // 개행을 구분자로 입력을 분리
			strtok(word, ":"); // 학생명을 제거
			token = strtok(NULL, ","); // ,기준으로 과목들을 분리함 (기존위치에서 재탐색 )

			while (token != NULL) {
				if (is_exist(token) == false) {
					strcpy(courses[coursecount], token);
					coursecount++;
				}
				token = strtok(NULL, ",");
			}
		}
	}
}

bool	is_exist(char *name) // vertex 생성 과정에서 이미 목로에 존재하는 과목인지 검사하는 함수
{
	int		i;
	for (i = 0; i < coursecount; i++) {
		if (strcmp(courses[i], name) == 0)
			return (true);
	}
	return (false);
}

FILE	*open_File(FILE *ptr, char *filename)
{
	if (access(filename, F_OK) != -1) {
		ptr = fopen(filename, "r");
		read_File(ptr);
	}
	else {
		printf("Error : File open error\n");
		exit(1);
	}
	return (ptr);
}

void	createVertexlist(Vertex **list)
{
	for(int i = 0; i < coursecount; i++) {
		Vertex	*new = malloc(sizeof(Vertex));
		strcpy(new->name, courses[i]);
		new->color = -1;
		new->visited = false;
		list[i] = new;
	}
}

void	sortVertex(Vertex **list) // 저장된 과목들을 정렬함
{
	Vertex	*tmp;
	char	*tmp2;
	int i, j;

	for (i = 0; i < coursecount - 1; i++) {
		for (j = 0; j < coursecount - i - 1; j++) { // bubble sorting
			if (strcmp(list[j]->name, list[j+1]->name) > 0) {
				tmp = list[j];
				list[j] = list[j+1];
				list[j+1] = tmp;
				tmp2 = courses[j];
				courses[j] = courses[j + 1];
				courses[j+1] = tmp2;
			}
		}
	}
}

void	**init_courses(void)
{
	courses = (char **)malloc(sizeof(char *) * 30);
	for (int i = 0; i < 30; i++)
		courses[i] = (char *)malloc(sizeof(char) * 30);
}

int	**create_edge(FILE *fp) {
	rewind(fp);
	int **ret;
	int i, j;

	ret = (int **)malloc(sizeof(int *) * coursecount); // 인접행렬의 선언
	for (i = 0; i < coursecount; i++)
		ret[i] = (int *)malloc(sizeof(int) * coursecount);

	for (i = 0; i < coursecount; i++) { // 인접행렬의 초기화
		for (j = 0; j < coursecount; j++)
			ret[i][j] = 0;
	}
	char input[256], word[256], *token;
	char	course_list[MAX_COURSE][MAX_COURSE_NAME];
	int		index = 0;

	while (!feof(fp)) {
		if (fgets(input, 512, fp) != NULL) {
			index = 0;
			strcpy(word, strtok(input, "\n"));
			strtok(word, ":");
			token = strtok(NULL, ",");

			while (token != NULL) {
				strcpy(course_list[index], token);
				index++;
				token = strtok(NULL, ",");
			}
			create_edge2(ret, course_list, index);
		}
	}
	return (ret);
}

void create_edge2(int **ret, char list[30][30], int index)
{
	int	i, j, check[coursecount];
	for (int i = 0; i < coursecount; i++) {
		check[i] = 0;
	}

	for (i = 0; i < coursecount; i++) {
		for (j = 0; j < index; j++) {
			if (strcmp(courses[i], list[j]) == 0)
				check[i]++;
		}
	}

	for (i = 0; i < coursecount; i++) {
		if (check[i] == 1) {
			for (j = 0; j < coursecount; j++) {
				if (check[j] == 1 && i != j) {
					ret[i][j] = 1;
					ret[j][i] = 1;
				}
			}
		}
	}
}

bool	promising(int i)
{
	int j = 0;
	bool	ret = true;
	while (j < i && ret)
	{
		if (adj[i][j] && vcolor[i] == vcolor[j])
			ret = false;
		j++;
	}
	return (ret);
}


void	m_coloring(int i)
{
	int		color;
	if(promising(i - 1))
	{
		if (i == coursecount)
		{
			if(print)
			{
				for(int j = 1; j <=m; j++)
				{
					printf("Exam Day %d -> ", j);
					for (int k = 0; k<coursecount; k++)
					{
						if(vcolor[k] == j)
							printf("%s ",courses[k]);
					}
					printf("\n");
				}
				printf("\n");
			}
		}
		else {
			for (color = 1; color <=m; color++)
			{
				vcolor[i] = color;
				m_coloring(i + 1);
			}
		}
	}
}

int main(void)
{
	int	i, j, k;

	init_courses();
	FILE *ptr = NULL; // data file을 받기 위한 file pointer 선언
	ptr = open_File(ptr, "input.txt");

	Vertex *list[coursecount]; // 과목 숫자만큼 포인터배열 선언.

	createVertexlist(list);
	sortVertex(list);

	adj = create_edge(ptr);
	printf("\n\t");

	for(i = 0; i < coursecount + 1; i++){

		for(j = 0; j < coursecount + 1; j++){
			if(i == 0 && j != coursecount)
				printf("%s\t", list[j] -> name);
			else if(j == 0)
				printf("%s\t", list[i - 1] -> name);
			if(i != 0 && j != 0)
				printf("   %d\t", adj[i - 1][j - 1]);
		}
		printf("\n");
	}

	vcolor = malloc(sizeof(int) * coursecount);
	for (int i = 0; i < coursecount; i++)
		vcolor[i] = -1;
	for (m = 1; m <= coursecount; m++)
	{
		m_coloring(0);
		if(vcolor[coursecount - 1] != -1)
			break;
	}

	printf("최단 시험 기간 : %d일\n\n",m);
	printf("가능한 시험 시간표 배치\n");
	print = true;
	m_coloring(0);
}

