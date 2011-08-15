#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "maze_gen.h"
#include "sort.h"

void	_b( int n_sections,  t_maze *maze )
{
  int	i;
  int	j;
  int	k;
  int	x;
  int	y;
  int	index;
  int	n_walls;
  int	wall_len;
  int	dir[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};


  for (k = 0; k < n_sections; k++)
    {
      printf("- sections %d\n", k);
      n_walls = 3 + rand() %  10;
      printf("-- n walls: %d\n", n_walls);

      x = 2 + rand() % (maze->w - 3);
      y = 2 + rand() % (maze->h - 3);
      maze->cells[y][x] = '.';

      printf("-- x: %d, y: %d\n", x, y);

      for (i = 0; i < n_walls; i++)
	{
	  index = rand() % 4;
	  wall_len = rand() % (n_walls * 2);
	  printf("wall[%d] len: %d\n", i, wall_len);

	  for (j = 0; j < wall_len; j++)
	    {
	      maze->cells[y][x] = '.';/*'0' + i;*/
	      if (x > 1 && x < maze->w - 2) x += dir[index][0];
	      if (y > 1 && y < maze->h - 2) y += dir[index][1];
	      printf("%d;%d ", x, y);
	    }

	}

    }
}

void	move_to_waypoint( t_point *src, t_point *dest, char c, t_maze *maze )
{
  int	i;
  int	j;
  int	k;
  i = (src->y < dest->y) ? src->y : dest->y;
  j = (src->y > dest->y) ? src->y + 1 : dest->y;

  for (; i < j; i++)
      maze->cells[i][src->x + 1] = c;
  j = src->x + 1;
  k = dest->x + 1;
  for (; j < k; j++)
      maze->cells[dest->y][j] = c;
}

void	build_path( t_point *waypoints, t_point *start, t_point *end, t_maze *maze, char c )
{
  int	current;
  int	target;

  move_to_waypoint(start, &waypoints[0], c, maze);
  for (current = 0; current < maze->d - 1; current++)
    {
      target = current + 1;
      move_to_waypoint(&waypoints[current], &waypoints[target], c, maze);
    }
  move_to_waypoint(&waypoints[maze->d - 1], end, c, maze);
}

void	print_maze( t_maze *maze )
{
  int	i;
  int	j;
  t_point	*waypoints;
  t_point	*dead_ends;
  t_point	start;
  t_point	end;
  t_point	first;

  maze->cells = malloc(maze->h * sizeof(char *));
  if (maze->cells == NULL)
    return;

  for (i = 0; i < maze->h; i++)
    {
      maze->cells[i] = malloc(maze->w * sizeof(char));
      for (j = 0; j < maze->w; j++)
	maze->cells[i][j] = 'x';
    }


  /* set start and end positions */
  start.x = 0;
  start.y = maze->h / 2;
  end.x = maze->w - 1;
  end.y = (maze->h - 2 < 0) ? 0 : maze->h - 2;
  maze->cells[start.y][start.x] = 'S';
  maze->cells[end.y][end.x] = 'E';


  /* set random waypoints */
  waypoints = malloc(maze->d * sizeof(t_point));
  dead_ends = malloc(maze->d * 4 * sizeof(t_point));

  first.x = maze->w - 1;
  first.y = 0;
  srand((unsigned int)time(NULL));
  for (i = 0; i < maze->d; i++)
    {
      waypoints[i].x = 1 + rand() % (maze->w - 2);
      waypoints[i].y = 1 + rand() % (maze->h - 2);

      if (waypoints[i].x < first.x)
	{
	  first.x = waypoints[i].x;
	  first.y = waypoints[i].y;
	}


      maze->cells[waypoints[i].y][waypoints[i].x] = 'x';
      /* printf("x%d, y:%d\n", waypoints[i].x, waypoints[i].y);*/
    }

  /*maze->cells[first.y][first.x] = 'F';*/


  shell_sort(waypoints, maze->d);
  puts("-------------");
  for (i = 0; i < maze->d; i++)
    {
      /* printf("x%d, y:%d\n", waypoints[i].x, waypoints[i].y);*/

    }

  _b(20, maze);


  /* make dead ends and fake paths */
  /*
    printf("num fake waypoints: %d\n", maze->d * F_DENSITY);
    for (i = 0; i < maze->d * F_DENSITY; i++)
    {
    dead_ends[i].x = 2 + rand() % (maze->w - 3);
    dead_ends[i].y = 1 + rand() % (maze->h - 1);
    }
  */
  /* shell_sort(dead_ends, maze->d * F_DENSITY); */
  /*build_path(dead_ends, &start, &end, maze->d * F_DENSITY, '.', 1);*/



  /* move from one waypoint to another, drawing a path */
  /* move_to_waypoint(start, first); */
  build_path(waypoints, &start, &end, maze->d, '-', 0);




  /* print the maze */
  for (i = 0; i < maze->h; i++)
    {
      for (j = 0; j < maze->w; j++)
	write(1, &maze->cells[i][j], 1);
      write(1, "\n", 1);
    }
  write(1, "\n", 1);
}

void		build_maze( t_maze *maze )
{
  int		i;
  int		j;
  t_point	start;
  t_point	end;
  t_point	*waypoints;

  if (maze->cell == NULL)
    {
      maze->cells = malloc(maze->h * sizeof(char *));
      if (maze->cells == NULL)
	return 0;
    }
  for (i = 0; i < maze->h; i++)
    {
      maze->cells[i] = malloc(maze->w * sizeof(char));
      for (j = 0; j < maze->w; j++)
	maze->cells[i][j] = 'x';
    }

  start.x = 0;
  start.y = maze->h / 2;
  end.x = maze->w - 1;
  end.y = (maze->h - 2 < 0) ? 0 : maze->h - 2;
  maze->cells[start.y][start.x] = 'S';
  maze->cells[end.y][end.x] = 'E';

  waypoints = malloc(maze->d * sizeof(t_point));
  if (waypoints == NULL)
    return 0;

  srand((unsigned int)time(NULL));
  for (i = 0; i < maze->d; i++)
    {
      waypoints[i].x = 1 + rand() % (maze->w - 2);
      waypoints[i].y = 1 + rand() % (maze->h - 2);
    }
  shell_sort(waypoints, maze->d);
  _b(20, maze);
  build_path(waypoints, &start, &end, maze, '-', 0);
}
