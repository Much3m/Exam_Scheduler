#include "scheduler.h"

////////////////////////////////////////////////////////////////////////////////////
/*                              variables for coloring                            */
int m = 0;
int size = 0;
int *vcolor;
bool print = false;
bool flag = false;

///////////////////////////////////////////////////////////////////////////////////
/*                              variables for Courses                            */
char	**courses;
int		coursecount = 0;
int		**adj = NULL;	                   // 노드간의 연결 상태를 파악하기 위한 인접행렬 선언
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
/*                      Functions for input txt file                            */

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
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
/*                     Functions for creating graph                            */
void    init_courses(void)
{
	courses = (char **)malloc(sizeof(char *) * MAX_COURSE);
	for (int i = 0; i < MAX_COURSE; i++)
		courses[i] = (char *)malloc(sizeof(char) * MAX_COURSE);
}

bool	is_exist(char *name) // node 생성 과정에서 이미 목로에 존재하는 과목인지 검사하는 함수
{
	int		i;
	for (i = 0; i < coursecount; i++) {
		if (strcmp(courses[i], name) == 0)
			return (true);
	}
	return (false);
}

void	sortCourse(void) // 저장된 과목들을 정렬함
{
	char	*tmp;
	int i, j;

	for (i = 0; i < coursecount - 1; i++) {
		for (j = 0; j < coursecount - i - 1; j++) { // bubble sorting
			if (strcmp(courses[j], courses[j+1]) > 0) {
				tmp = courses[j];
				courses[j] = courses[j + 1];
				courses[j+1] = tmp;
			}
		}
	}
}

int	**create_adj(FILE *fp) {
	rewind(fp); // filepointer를 초기 위치로 변경.
	int **ret;
	int i, j;

	ret = (int **)malloc(sizeof(int *) * coursecount); // 인접행렬의 선언
	for (i = 0; i < coursecount; i++)
		ret[i] = (int *)malloc(sizeof(int) * coursecount);

	for (i = 0; i < coursecount; i++) { // 인접행렬의 초기화
		for (j = 0; j < coursecount; j++)
			ret[i][j] = 0;
	}
	char    buffer[256], word[256], *token;
	char	course_list[MAX_COURSE][MAX_COURSE_NAME];
	int		index = 0;

	while (!feof(fp)) {
		if (fgets(buffer, 512, fp) != NULL) { // 개행을 만날때 까지 읽어서 buffer에 저장
			index = 0;
			strcpy(word, strtok(buffer, "\n")); // 개행문자를 잘라냄.
			strtok(word, ":");          // 이름을 잘라냄
			token = strtok(NULL, ","); // ,을 기준으로 과목을 구분.

			while (token != NULL) { // 학생 한명이 수강하는 과목을 모두 토큰화
				strcpy(course_list[index], token);
				index++;
				token = strtok(NULL, ",");
			}
			create_adj2(ret, course_list, index); // 학생 한명이 수강하는 과목명, 과목수를 넘겨줌
		}
	}
	return (ret);
}

void create_adj2(int **ret, char list[MAX_COURSE][MAX_COURSE_NAME], int index)
{
	int	i, j, check[coursecount];
	for (int i = 0; i < coursecount; i++) {
		check[i] = 0; // check 배열을 초기화
	}

	for (i = 0; i < coursecount; i++) {
		for (j = 0; j < index; j++) {
			if (strcmp(courses[i], list[j]) == 0) // 전체과목명, 학생이 수강하는 과목명을 비교
				check[i]++; // 일치하는 과목이 있다면 check배열을 증가.
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

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
/*                     Functions for coloring graph                            */
bool	is_valid(int i)
{
	int j = 0;
	bool	ret = true;
	while (j < i && ret) {
		if (adj[i][j] && vcolor[i] == vcolor[j])
			ret = false;
		j++;
	}
	return (ret);
}

void	recursive_coloring(int i)
{
	if (i == coursecount) {
		flag = true;
		if(print){
			for(int j = 1; j <=m; j++)
			{
				printf("Exam Day %d -> ", j);
				for (int k = 0; k<coursecount; k++) {
					if(vcolor[k] == j)
						printf("%s ",courses[k]);
				}
				printf("\n");
			}
		printf("\n");
		}
	}
	else {
		for (int color = 1; color <= m; color++) {
			vcolor[i] = color;
			if (is_valid(i))
				recursive_coloring(i+1);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
/*                              main Function                                   */
int main(void)
{
	int	i, j;

	init_courses();
	FILE *ptr = NULL; // data file을 받기 위한 file pointer 선언
	ptr = open_File(ptr, "input.txt");
	sortCourse();

	adj = create_adj(ptr);
    printf("\n");
    for(int i = 0; i < coursecount*4 - 7; i++)
        printf(" ");
    printf("Adjacent Matirx\n\t");

	for(i = 0; i < coursecount + 1; i++){

        for(j = 0; j < coursecount + 1; j++){
            if(i == 0 && j != coursecount)
                printf("   %s\t", courses[j]);
            else if(j == 0)
                printf("%s\t", courses[i - 1]);
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
		recursive_coloring(0);
		if(flag == true)
			break;
	}

	printf("\n\n최단 시험 기간 : %d일\n\n",m);
	printf("가능한 시험 시간표 배치\n");
	print = true;
	recursive_coloring(0);
}

