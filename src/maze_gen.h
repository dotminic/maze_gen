#ifndef __MAZEGEN_H__
# define __MAZEGEN_H__

#define	F_DENSITY	4
#define	M_LEFT		1
#define M_RIGHT		2

typedef struct	s_point
{
  int		x;
  int		y;
}		t_point;


typedef struct	s_maze
{
  int		w;		/* width */
  int		h;		/* height */
  int		d;		/* density */
  int		f;		/* num fake points */
  char		s;		/* is the maze 'solid' */
  char		**cells;
}		t_maze;

void	init_maze( t_maze *maze, int w,
		   int h, int d, int solid );
void	free_maze( t_maze *maze );
int	build_maze( t_maze *maze );

#endif /* !__MAZEGEN_H__ */
