#ifndef SHOWCASER_HPP
#define SHOWCASER_HPP

#include "Hashgraphs.hpp"
#include "Event.hpp"
#include <iomanip>
# include <SDL2/SDL.h>

class Showcaser final{
private:
    SDL_Window *win;
    SDL_Renderer *rend;
    SDL_Texture* p1;
    SDL_Texture* p2;
    SDL_Texture* p3;
    SDL_Texture* p4;
    SDL_Texture* p5;
    SDL_Texture* p6;
    SDL_Texture* number_tex;
    SDL_Texture* throughput;
    SDL_Event event;
    bool stop; // = 0
    int personShown;
    int gossipsCounted;
    clock_t begin;
    double gossipPerSec;
    clock_t pauseSt;
    double pausedTime; // = 0
public:
    Showcaser();
    ~Showcaser();
    void putdigit(int const & x, int const & y, int const & val);
	void putfloat(int const & x, int const & y, std::string const & val);
    void putInt(int const &x, int const &y, std::string const &val);
    void square(Event const & e);
    void connect(Event const & e, Event const & p);
    void ponies();
    void refresh(Person const & p);
    void SDL_Start();
    void get_input();
    int run();
};

#endif
