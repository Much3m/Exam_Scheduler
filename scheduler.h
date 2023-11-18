#ifndef SCHEDULER_H
# define SCHEDULER_H

#define	MAX_COURSE 30
#define	MAX_COURSE_NAME 30

/*                      including standard headers                              */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

/*                      Functions for input txt file                            */
void	read_File(FILE *ptr);
FILE	*open_File(FILE *ptr, char *filename);

/*                       Functions for creating graph                          */
int 	**create_edge(FILE *fp);
void    create_edge2(int **ret, char list[MAX_COURSE][MAX_COURSE_NAME], int index);
void	sortCourse(void);
void    init_courses(void);

/*                     Functions for coloring graph                            */
bool	is_valid(int i);
void	recursive_coloring(int i);
bool	is_exist(char *name);

#endif
