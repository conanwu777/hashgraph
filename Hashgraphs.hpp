#ifndef HASHGRAPHS_HPP
# define HASHGRAPHS_HPP
# include <iostream>
# include <vector>
# include <unistd.h>
# include <sstream>
# include <fstream>
# include <ctime>

# define W 1000
# define H 1320
# define M 150
# define GAP 50 / N
# define RED "\e[1;38;2;225;20;20m"
# define WHITE "\e[1;38;2;255;251;214m"
# define YELLO "\e[1;38;2;255;200;0m"
# define ORANGE "\e[1;38;2;255;120;10m"

const int	N = 6;
const int	C = 5;

class					Person;
struct	data;
extern bool						makeForks;
extern bool						writeLog;
extern int						runTime;
extern std::array<Person*, N>	people;

const std::string		md5_hash(std::string const str);
void					md5Init();

#endif
