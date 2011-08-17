#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze_gen.h"
#include "sort.h"

static const int g_dir[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

static int	get_dir_index()
{
  static int	p = 0;
  int		index;

  index = rand() % 4;
  while (p == index)
    index = rand() % 4;
  p = index;
  return index;
}

static void	_c( t_maze *maze )
{
  int		i;
  int		j;

  for (i = 1; i < maze->h - 1; i++)
    {
      for (j = 1; j < maze->w - 1; j++)
	{
	  if (maze->cells[i - 1][j] == '.' &&
	      maze->cells[i + 1][j] == '.' &&
	      maze->cells[i][j - 1] == '.' &&
	      maze->cells[i][j + 1] == '.')
	    maze->cells[i][j] = 'x';
	  if (maze->cells[i - 1][j] == 'x' &&
	      maze->cells[i + 1][j] == 'x' &&
	      maze->cells[i][j - 1] == 'x' &&
	      maze->cells[i][j + 1] == 'x')
	    maze->cells[i][j] = '.';
	}
    }
}

static void	draw_rand_walls( t_maze *maze )
{
  int		i;
  int		j;
  int		k;
  int		x;
  int		y;
  int		index;
  int		n_walls;
  int		wall_len;

  for (k = 0; k < maze->f; k++)
    {
      n_walls = 3 + rand() %  10;
      x = M_LEFT + rand() % (maze->w - M_RIGHT);
      y = M_LEFT + rand() % (maze->h - M_RIGHT);
      /*maze->cells[y][x] = '.';*/
      for (i = 0; i < n_walls; i++)
	{
	  index = get_dir_index();
	  wall_len = rand() % (n_walls * F_DENSITY);
	  for (j = 0; j < wall_len; j++)
	    {
	      maze->cells[y][x] = '.';
	      if (x > M_LEFT && x < maze->w - M_RIGHT - 1) x += g_dir[index][0];
	      if (y > M_LEFT && y < maze->h - M_RIGHT - 1) y += g_dir[index][1];
	    }
	}
    }
}

static void	move_to_waypoint( t_point *src, t_point *dest,
				  t_maze *maze, char c )
{
  int		i;
  int		j;

  i = (src->y < dest->y) ? src->y : dest->y;
  j = (src->y > dest->y) ? src->y + 1 : dest->y;
  for (; i < j; i++)
      maze->cells[i][src->x + 1] = c;
  i = src->x + 1;
  j = dest->x + 1;
  for (; i < j; i++)
      maze->cells[dest->y][i] = c;
}

static void	build_path( t_point *waypoints, t_point *start,
			    t_point *end, t_maze *maze, char c )
{
  int		current;

  move_to_waypoint(start, &waypoints[0], maze, c);
  for (current = 0; current < maze->d - 1; current++)
    move_to_waypoint(&waypoints[current], &waypoints[current + 1], maze, c);
  move_to_waypoint(&waypoints[maze->d - 1], end, maze, c);
}

int		build_maze( t_maze *maze )
{
  int		i;
  int		j;
  t_point	start;
  t_point	end;
  t_point	*waypoints;

  printf("w:%d-h:%d\n", maze->w, maze->h);
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
  end.y = 1 + rand() % (maze->h - M_LEFT);
  maze->cells[start.y][start.x] = 'S';
  maze->cells[end.y][end.x] = 'E';
  printf("s:%d,%d-e:%d,%d\n", start.x, start.y, end.x, end.y);

  waypoints = malloc(maze->d * sizeof(t_point));
  if (waypoints == NULL)
    return 0;
  for (i = 0; i < maze->d; i++)
    {
      waypoints[i].x = M_LEFT + rand() % (maze->w - M_RIGHT - 1);
      waypoints[i].y = M_LEFT + rand() % (maze->h - M_RIGHT);
    }
  shell_sort(waypoints, maze->d);
  draw_rand_walls(maze);
  if (maze->s == 0)
    _c(maze);
  build_path(waypoints, &start, &end, maze, '.');
  free(waypoints);
  return 1;
}

void	init_maze( t_maze *maze, int w,
		   int h, int d, int solid )
{
  maze->w = w;
  maze->h = h;
  maze->d = d;
  maze->f = d * F_DENSITY;
  maze->s = solid;
  maze->cells = NULL;
  srand((unsigned int)time(NULL));
}

void	free_maze( t_maze *maze )
{
  int	i;

  for (i = 0; i < maze->h; i++)
    free(maze->cells[i]);
  free(maze->cells);
}
