#ifndef MD5_HPP
# define MD5_HPP

# include <math.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <sstream>
# include <iostream>
# include <iomanip>

typedef struct			s_v
{
	unsigned int		a;
	unsigned int		b;
	unsigned int		c;
	unsigned int		d;
	unsigned int		e;
	unsigned int		f;
	unsigned int		g;
	unsigned int		h;
}						t_v;

typedef unsigned char	t_bt;

unsigned int			g_cks;
unsigned long long		g_len;
unsigned char			*g_new;

#endif
