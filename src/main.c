#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
# include <io.h>
#define write	_write
#else
# include <unistd.h>
#endif
#include "maze_gen.h"

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

  if (argc != 5)
    puts("usage: -width -height -density -solid");
  else
    {
      init_maze(&maze, atoi(argv[1]), atoi(argv[2]),
		atoi(argv[3]), atoi(argv[4]));
      if (build_maze(&maze))
	print_maze(&maze);
    }
  return 0;
}
