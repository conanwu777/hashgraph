#include "md5.hpp"

unsigned int g_a;
unsigned int g_b;
unsigned int g_c;
unsigned int g_d;

unsigned int g_k[64];

t_bt g_m[16][4];

static unsigned int g_coe[64] = {7, 12, 17, 22, 7, 12, 17, 22,
	7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

static unsigned int	rotate(unsigned int nbr, int n)
{
	int				i;
	unsigned int	bit;

	i = 0;
	while (i < n)
	{
		bit = (nbr & 1) << 31;
		nbr = nbr >> 1;
		nbr += bit;
		i++;
	}
	return (nbr);
}

static void			reverse_bits(unsigned int *c)
{
	unsigned int t;

	t = 0; 
	t += *c & 0xff;
	t = t << 8;
	t += (*c >> 8) & 0xff;
	t = t << 8;
	t += (*c >> 16) & 0xff;
	t = t << 8;
	t += (*c >> 24) & 0xff;
	*c = t;
}

static void	do_step(int const i, t_v *vp)
{
	if (0 <= i && i < 16)
	{
		vp->f = (vp->b & vp->c) | ((~vp->b) & vp->d);
		vp->g = i;
	}
	else if (16 <= i && i < 32)
	{
		vp->f = (vp->d & vp->b) | ((~vp->d) & vp->c);
		vp->g = (5 * i + 1) & 15;
	}
	else if (32 <= i && i < 48)
	{
		vp->f = vp->b ^ vp->c ^ vp->d;
		vp->g = (3 * i + 5) & 15;
	}
	else
	{
		vp->f = vp->c ^ (vp->b | (~vp->d));
		vp->g = 7 * i & 15;
	}
	vp->f += vp->a + g_k[i] + *((unsigned int*)(g_m[vp->g]));
	vp->a = vp->d;
	vp->d = vp->c;
	vp->c = vp->b;
	vp->b += (vp->f << g_coe[i]) | (vp->f >> (32 - g_coe[i]));
}

static void	do_stage(int const count, t_v *vp)
{
	int i;
	int j;

	vp->a = g_a;
	vp->b = g_b;
	vp->c = g_c;
	vp->d = g_d;
	i = -1;
	while (++i < 16)
	{
		j = -1;
		while (++j < 4)
			g_m[i][j] = g_new[count * 64 + i * 4 + j];
	}
	i = -1;
	while (++i < 64)
		do_step(i, vp);
	g_a += vp->a;
	g_b += vp->b;
	g_c += vp->c;
	g_d += vp->d;
}

static void	set_up(std::string const str)
{
	int		i;
	t_bt	bt;

	g_len = str.size() << 3;
	g_cks = (str.size() + 8) / 64 + 1;
	g_new = (t_bt*)malloc(64 * g_cks);
	i = -1;
	while (++i < (int)(64 * g_cks))
		g_new[i] = 0;
	i = -1;
	while (str[++i])
		g_new[i] = str[i];
	bt = 1 << 7;
	g_new[str.size()] = bt;
	i = -1;
	while (++i < 8)
		g_new[64 * g_cks - i - 1] = ((char*)(&g_len))[i];
}

void md5Init()
{
	int i = -1;
	while (++i < 64)
		g_k[i] = floor(pow(2, 32) * fabs(sin(i + 1)));
}

const std::string	md5_hash(std::string const str)
{
	t_v				v;
	unsigned int	count;
	int				i;
	std::ostringstream	s;

	g_a = 0x67452301;
	g_b = 0xefcdab89;
	g_c = 0x98badcfe;
	g_d = 0x10325476;
	set_up(str);
	count = -1;
	while (++count < g_cks)
		do_stage(count, &v);
	reverse_bits(&g_a);
	reverse_bits(&g_b);
	reverse_bits(&g_c);
	reverse_bits(&g_d);
	s << std::hex << std::setfill('0') << std::setw(8) << g_a;
	s << std::hex << std::setfill('0') << std::setw(8) << g_b;
	s << std::hex << std::setfill('0') << std::setw(8) << g_c;
	s << std::hex << std::setfill('0') << std::setw(8) << g_d;
	if (g_new)
		free(g_new);
	return (s.str());
}
