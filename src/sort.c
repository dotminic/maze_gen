#include "sort.h"

void	shell_sort( t_point *waypoints, int n )
{
  t_point	temp;
  int		i;
  int		k;
  int		seq;

  i = 0;
  k = 0;
  seq = 1;
  while (seq <= n / 3)
    seq = seq * 3 + 1;
  while (seq > 0)
    {
      for (k = seq; k < n; k++)
	{
	  temp = waypoints[k];
	  i = k;
	  while (i > seq - 1 && waypoints[i - seq].x >= temp.x)
	    {
	      waypoints[i] = waypoints[i - seq];
	      i -= seq;
	    }
	  waypoints[i] = temp;
	}
      seq = (seq - 1) / 3;
    }
}
