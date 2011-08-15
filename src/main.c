#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "maze_gen.h"

void	print_usage()
{
  puts("usage: -width -height -density");
}

void	print_maze( t_maze *maze )
{
  int	i;
  int	j;

  for (i = 0; i < maze->h; i++)
    {
      for (j = 0; j < maze->w; j++)
	write(1, &maze->cells[i][j], 1);
      write(1, "\n", 1);
    }
}

int		main( int argc, char **argv )
{
  t_maze	maze;

  if (argc != 4)
    {
      print_usage();
      return 0;
    }
  else
    {
      init_maze(&maze, atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
      printf("generating %d*%d maze...\n", maze.w, maze.h);
      if (build_maze(&maze))
	{
	  print_maze(&maze);
	}
    }
  return 0;
}
