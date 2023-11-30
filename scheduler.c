#include "scheduler.h"

////////////////////////////////////////////////////////////////////////////////////
/*                              variables for coloring                            */
int m = 0;                                  // 최대 사용 가능 색상 지정                        
int *vcolor;                                // 각 노드의 색을 저장할 배열
int flag = 0;                               // 목표 달성 여부를 확인하는 플래그
int print = 0;
int total_case = 0;

///////////////////////////////////////////////////////////////////////////////////
/*                              variables for Courses                            */
char	**courses;                         // 과목명을 저장할 이차원 배열 선언
int		coursecount = 0;                   // 과목의 개수를 저장할 변수 선언
int		**adj = NULL;	                   // 노드간의 연결 상태를 파악하기 위한 인접행렬 선언
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
/*                      Functions for input txt file                            */

FILE	*open_File(FILE *ptr, char *filename)
{
	if (access(filename, F_OK) != -1) {   // 존재하는 file인지 확인. -1이면 존재하지 않는 파일
		ptr = fopen(filename, "r");       // 파일을 open
		read_File(ptr);
	}
	else {
		printf("Error : File open error\n"); // 파일이 존재하지 않는다면 Error 출력후 종료.
		exit(1);
	}
	return (ptr);
}

void	read_File(FILE *ptr)
{
	char	buffer[256], single[256];
	char	*token;

	while (!feof(ptr)) { // file의 끝에 도달하면 0 이 아닌 값을 반환
		if (fgets(buffer, 512, ptr) != NULL) {
			strcpy(single, strtok(buffer, "\n")); // 개행을 구분자로 입력을 분리
			strtok(single, ":"); // 학생명을 제거
			token = strtok(NULL, ","); // ,기준으로 과목들을 분리함 (기존위치에서 재탐색 )

			while (token != NULL) {
				if (is_exist(token) == false) { // 기존에 저장된 과목이 아니라면 추가
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
void    init_courses(void) // 과목명을 저장할 이차원 배열의 선언 및 초기화
{
	courses = (char **)malloc(sizeof(char *) * MAX_COURSE);
	for (int i = 0; i < MAX_COURSE; i++)
		courses[i] = (char *)malloc(sizeof(char) * MAX_COURSE);
}

bool	is_exist(char *name) // node 생성 과정에서 이미 목로에 존재하는 과목인지 검사하는 함수
{
	int		i;
	for (i = 0; i < coursecount; i++) {
		if (strcmp(courses[i], name) == 0) // 일치하는 과목이 이미 존재한다면 true리턴
			return (true);
	}
	return (false);
}

void	sortCourse(void) // 저장된 과목들을 정렬함
{
	char	*tmp;
	int i, j;

	for (i = 0; i < coursecount - 1; i++) {
		for (j = 0; j < coursecount - i - 1; j++) {     // bubble sorting
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
	char    buffer[256], single[256], *token;
	char	course_list[MAX_COURSE][MAX_COURSE_NAME];
	int		index = 0;

	while (!feof(fp)) {
		if (fgets(buffer, 512, fp) != NULL) { // 개행을 만날때 까지 읽어서 buffer에 저장
			index = 0;
			strcpy(single, strtok(buffer, "\n")); // 개행문자를 잘라냄.
			strtok(single, ":");          // 이름을 잘라냄
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
				if (check[j] == 1 && i != j) { // i과목, j과목을 모두 수강한다면 연결.
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
	while (j < i && ret) { // 배열의 인덱스 순서대로 칠하기 때문에 현재 인덱스 이전 노드들과 비교
		if (adj[i][j] && vcolor[i] == vcolor[j]) // 연결되어있고 칠해진 색상이 같다면
			ret = false; // false를 리턴
		j++;
	}
	return (ret); // 아니라면 true 리턴
}

void	recursive_coloring(int i)
{
	if (i == coursecount) { // 색을 끝까지 칠했다면
		flag++;
        if (print == 1)
            printf("최단 시험 기간 : %d일\n\n가능한 시험 시간표 배치\n",m);
		if(print != 0 && print <= total_case){ // print가 true라면 경우의 수를 출력
			for(int j = 1; j <=m; j++)
			{
				printf("Exam Day %d -> ", j);
				for (int k = 0; k<coursecount; k++) { // 같은 색상을 가진 과목끼리 출력
					if(vcolor[k] == j)
						printf("%s ",courses[k]);
				}
				printf("\n");
			}
		    printf("\n");
            print++;
		}
	}
	else {
		for (int color = 1; color <= m; color++) {
			vcolor[i] = color; // 1~m까지의 색상을 대입해봄
			if (is_valid(i)) // 위에서 대입한 색상이 유효하다면
				recursive_coloring(i+1); // 함수를 재귀적으로 호출하여 다음칸에 색칠
		}
	}
}

int factorial(int a) // 최소한의 경우의 수를 구하기위한 factorial util function
{
    int ret = 1;
    for (;a >=1; a--)
        ret = ret * a;
    return (ret);
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
/*                              main Function                                   */
int main(void)
{
	init_courses();   // 과목을 저장하기 위한 이차원배열 선언
	FILE *ptr = NULL; // input을 받기 위한 file pointer 선언
	ptr = open_File(ptr, "input.txt");
	sortCourse();

	adj = create_adj(ptr); // 인접행렬 생성

	vcolor = malloc(sizeof(int) * coursecount); // 각 노드의 color를 저장할 배열 선언.
	for (m = 1; m <= coursecount; m++) // 사용가능한 색상의 숫자를 증가시켜가면서 coloring.
	{
		recursive_coloring(0); // 0번 노드부터 색칠 시작
		if(flag) // 최초로 완성한다면 중단 -> 최소m값을 찾음.
			break;
	}
    print++; // 출력을 활성화
    total_case = flag / factorial(m); // 최소한의 경우의수를 구함 (중복케이스 제거)
    recursive_coloring(0);
    return (0);
}

