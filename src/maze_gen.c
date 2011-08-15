#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "maze_gen.h"
#include "sort.h"

static void	_b( int n_sections,  t_maze *maze )
{
  int		i;
  int		j;
  int		k;
  int		x;
  int	y;
  int	index;
  int	n_walls;
  int	wall_len;
  int	dir[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  /*printf("n_sections: %d, maze->d: %d, maze->f: %d\n", n_sections, maze->d, maze->f);*/
  for (k = 0; k < n_sections; k++)
    {
      /*printf("- sections %d\n", k);*/
      n_walls = 3 + rand() %  10;
      /*printf("-- n walls: %d\n", n_walls);*/

      x = 2 + rand() % (maze->w - 3);
      y = 2 + rand() % (maze->h - 3);
      maze->cells[y][x] = '.';

      /*printf("-- x: %d, y: %d\n", x, y);*/

      for (i = 0; i < n_walls; i++)
	{
	  index = rand() % 4;
	  wall_len = rand() % (n_walls * 2);
	  /*printf("wall[%d] len: %d\n", i, wall_len);*/

	  for (j = 0; j < wall_len; j++)
	    {
	      maze->cells[y][x] = '.';
	      if (x > 1 && x < maze->w - 2) x += dir[index][0];
	      if (y > 1 && y < maze->h - 2) y += dir[index][1];
	      /*printf("%d;%d ", x, y);*/
	    }
	}
    }
}

static void	move_to_waypoint( t_point *src, t_point *dest, char c, t_maze *maze )
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

static void	build_path( t_point *waypoints, t_point *start, t_point *end, t_maze *maze, char c )
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

int		build_maze( t_maze *maze )
{
  int		i;
  int		j;
  t_point	start;
  t_point	end;
  t_point	*waypoints;

  maze->cells = malloc(maze->h * sizeof(char *));
  if (maze->cells == NULL)
    return 0;
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
  _b(maze->f, maze);
  build_path(waypoints, &start, &end, maze, '-');
  free(waypoints);
  return 1;
}

void	init_maze( t_maze *maze, int w, int h, int d )
{
  maze->w = w;
  maze->h = h;
  maze->d = d;
  maze->f = d * F_DENSITY;
  maze->cells = NULL;
}

void	free_maze( t_maze *maze )
{
  int	i;

  for (i = 0; i < maze->h; i++)
    free(maze->cells[i]);
  free(maze->cells);
}
