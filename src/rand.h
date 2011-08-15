#ifndef __RANDOM_H__
# define __RANDOM_H__

#include <time.h>

inline void	srandom( unsigned int seed )
{
  srand( seed  * time(NULL));
}

inline int	random( int max )
{
  return rand() % max;
}

#endif /* __RANDOM_H__ */
