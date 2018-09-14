#include "Hashgraphs.hpp"

int runTime = 1;
bool makeForks = 0;
bool writeLog = 1;
SDL_Window *win;
SDL_Renderer *rend;
SDL_Texture* p1;
SDL_Texture* p2;
SDL_Texture* p3;
SDL_Texture* p4;
SDL_Texture* p5;
SDL_Texture* p6;
SDL_Texture* number_tex;
SDL_Event event;
std::array<Person*, N> people;
bool stop = 0;
int personShown;
int gossipsCounted;
clock_t begin;
double gossipPerSec;
clock_t pauseSt;
double pausedTime = 0;

void putdigit(int const & x, int const & y, int const & val)
{
	SDL_Rect rect = {x, y, 10, 15};
	if (val == 10)
		rect.w = 6;
	if (val == 11)
		rect.w = 6;
	SDL_Rect num = {val * 10, 0, (val == 11 ? 10 : rect.w), 15};
	SDL_RenderCopy(rend, number_tex, &num, &rect);
}

void putfloat(int const & x, int const & y, std::string const & val)
{
	int i = 0;
	if (val[0] == '-'){
		putdigit(x, y, 11);
		i = 1;
	}
	for (i = i; val[i] != '.'; i++){
		putdigit(x + i * 10, y, val[i] - '0');
	}
	putdigit(x + i * 10, y, 10);
	i++;
	for (int j = 0; j < 2; j++){
		putdigit(x + (i + j) * 10, y, val[i + j] - '0');
	}
}

void putInt(int const &x, int const &y, std::string const &val)
{
	int i;
	for (i = 0; i < val.size(); i++){
		putdigit(x + i * 10, y, val[i] - '0');
	}
}

void square(Event const & e)
{
	SDL_Rect rect;
	int y = (runTime - e.getData().timestamp) * GAP;
	int x = M + e.getData().owner * (W - 2 * M) / (N - 1);
	if (e.getFamous() == 1)
		SDL_SetRenderDrawColor(rend, 247, 185, 0, 255);
	else if (e.getFamous() == 0)
		SDL_SetRenderDrawColor(rend, 130, 130, 130, 255);
	else if (e.getWitness() == 1)
		SDL_SetRenderDrawColor(rend, 163, 0, 0, 255);
	else if (e.getRoundRecieved() != -1)
		SDL_SetRenderDrawColor(rend, 19, 132, 70, 255);
	else
		SDL_SetRenderDrawColor(rend, 9, 71, 124, 255);
	rect.w = 10;
	rect.h = 10;
	rect.x = x - 5;
	rect.y = y - 5;
	SDL_RenderFillRect(rend, &rect);
}

void connect(Event const & e, Event const & p)
{
	int y = (runTime - e.getData().timestamp) * GAP;
	int x = M + e.getData().owner * (W - 2 * M) / (N - 1);
	int y2 = (runTime - p.getData().timestamp) * GAP;
	int x2 = M + p.getData().owner * (W - 2 * M) / (N - 1);
	SDL_RenderDrawLine(rend, x, y, x2, y2);
	SDL_RenderDrawLine(rend, x, y - 1, x2, y2 - 1);
	SDL_RenderDrawLine(rend, x - 1, y, x2 - 1, y2);
}

double calculateAvg(const std::deque<double> &queue)
{
    double avg = 0;
    for(int i = 0; i != queue.size(); i++)
		avg += queue[i];
    avg /= queue.size();
	return avg;
}

void ponies()
{
	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_Rect rect;
	rect.w = (W - 2 * M) * 2 / (N - 1) / 3;
	rect.h = (W - 2 * M) * 2 / (N - 1) / 3;
	rect.x = 50;
	rect.y = 50;
	switch (personShown)
	{
		case 0 :
			SDL_RenderCopy(rend, p1, NULL, &rect);
			break;
		case 1 :
			SDL_RenderCopy(rend, p2, NULL, &rect);
			break;
		case 2 :
			SDL_RenderCopy(rend, p3, NULL, &rect);
			break;
		case 3 :
			SDL_RenderCopy(rend, p4, NULL, &rect);
			break;
		case 4 :
			SDL_RenderCopy(rend, p5, NULL, &rect);
			break;
		case 5 :
			SDL_RenderCopy(rend, p6, NULL, &rect);
			break;
	}
	rect.w = W;
	rect.h = (W - 4 * M) / (N - 1) + 100;
	rect.x = 0;
	rect.y = H - (W - 4 * M) / (N - 1) - 80;
	SDL_RenderFillRect(rend, &rect);
	rect.w = (W - 2 * M) * 2 / (N - 1) / 3;
	rect.h = (W - 2 * M) * 2 / (N - 1) / 3;
	rect.x = M - rect.w / 2;
	SDL_RenderCopy(rend, p1, NULL, &rect);
	rect.x += (W - 2 * M) / (N - 1);
	SDL_RenderCopy(rend, p2, NULL, &rect);
	rect.x += (W - 2 * M) / (N - 1);
	SDL_RenderCopy(rend, p3, NULL, &rect);
	rect.x += (W - 2 * M) / (N - 1);
	SDL_RenderCopy(rend, p4, NULL, &rect);
	rect.x += (W - 2 * M) / (N - 1);
	SDL_RenderCopy(rend, p5, NULL, &rect);
	rect.x += (W - 2 * M) / (N - 1);
	SDL_RenderCopy(rend, p6, NULL, &rect);
	rect.x = M - rect.w / 2 + N * (W - 2 * M) / (N - 1);
	SDL_RenderFillRect(rend, &rect);
	rect.x = M - rect.w / 2 + 10;
	for (int i = 0; i < N; i++)
	{
		std::ostringstream s;
		s << std::fixed << std::setprecision(2) << people[personShown]->networth[i];
		putfloat(rect.x, H - 50, s.str());
		rect.x += (W - 2 * M) / (N - 1);
	}
}

void refresh(Person const & p)
{
	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_RenderFillRect(rend, NULL);
	SDL_SetRenderDrawColor(rend, 220, 220, 220, 255);
	for (int i = 0; i < N; i++)
		SDL_RenderDrawLine(rend, M + i * (W - 2 * M) / (N - 1), 0,
			M + i * (W - 2 * M) / (N - 1), H);
	for (unsigned int i = 0; i < (p.getHashgraph())->size(); i++)
	{
		if ((runTime - ((*(p.getHashgraph()))[i])->getData().timestamp) * GAP > H)
			continue ;
		SDL_SetRenderDrawColor(rend, 200, 200, 200, 255);
		if ((*(p.getHashgraph()))[i]->getSelfParent())
		{
			connect(*(*(p.getHashgraph()))[i],
				(*((*(p.getHashgraph()))[i])->getSelfParent()));
			connect(*(*(p.getHashgraph()))[i],
				(*((*(p.getHashgraph()))[i])->getGossiperParent()));
		}
	}
	for (std::list<Event*>::const_iterator i = p.getFinishedNodes()->begin();
	i != p.getFinishedNodes()->end(); ++i)
	{
		if ((runTime - (*i)->getData().timestamp) * GAP > H)
			continue ;
		SDL_SetRenderDrawColor(rend, 200, 200, 200, 255);
		if ((*i)->getSelfParent())
		{
			connect(**i,*(*i)->getSelfParent());
			connect(**i,*(*i)->getGossiperParent());
		}
	}
	for (unsigned int i = 0; i < (p.getHashgraph())->size(); i++)
	{
		if ((runTime - ((*(p.getHashgraph()))[i])->getData().timestamp) * GAP > H)
			continue ;
		square(*((*(p.getHashgraph()))[i]));
	}
	for (std::list<Event*>::const_iterator i = p.getFinishedNodes()->begin();
	i != p.getFinishedNodes()->end(); ++i)
	{
		if ((runTime - (*i)->getData().timestamp) * GAP > H)
			continue ;
		square(**(i));
	}
	if (N <= 6)
		ponies();
	putInt(10, 10 ,std::to_string((int)gossipPerSec));
	SDL_RenderPresent(rend);
}

void SDL_Init()
{
	auto t_start = std::chrono::high_resolution_clock::now();
	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow("Hashgraph", 1600, 0, W, H, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(win, -1, 0);
	SDL_Surface *tmpSurf;
	tmpSurf = SDL_LoadBMP("ponies/1.bmp");
	p1 = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("ponies/2.bmp");
	p2 = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("ponies/3.bmp");
	p3 = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("ponies/4.bmp");
	p4 = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("ponies/5.bmp");
	p5 = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("ponies/6.bmp");
	p6 = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("numbers.bmp");
	number_tex = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
}

int main()
{
	SDL_Init();
	for (int i = 0; i < N; i++)
		people[i] = new Person(i);
	personShown = 0;
	runTime++;
	while (1)
	{
		while (SDL_PollEvent(&event))
		    if (event.type == SDL_KEYDOWN || event.type == SDL_QUIT)
		    {
		        if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
		            exit (1);
		        if (event.key.keysym.sym == SDLK_SPACE)
				{
		            stop = !stop;
					if (stop)
						pauseSt = clock();
					else
						pausedTime = double(clock() - pauseSt);
				}
		        if (event.key.keysym.sym == SDLK_f)
		            makeForks = !makeForks;
				if (event.key.keysym.sym == SDLK_l)
		            writeLog = !writeLog;
		        if (N >= 1 && event.key.keysym.sym == SDLK_1)
		        	personShown = 0;
		        if (N >= 2 && event.key.keysym.sym == SDLK_2)
		        	personShown = 1;
		        if (N >= 3 && event.key.keysym.sym == SDLK_3)
		        	personShown = 2;
		        if (N >= 4 && event.key.keysym.sym == SDLK_4)
		        	personShown = 3;
		        if (N >= 5 && event.key.keysym.sym == SDLK_5)
		        	personShown = 4;
		        if (N >= 6 && event.key.keysym.sym == SDLK_6)
		        	personShown = 5;
				refresh(*(people[personShown]));
		    }
		if (stop)
			continue ;
		int i = std::rand() % N;
		int j;
		while ((j = std::rand() % N) == i)
			;
		if (gossipsCounted == 0)
			begin = clock();
		people[i]->gossip(*(people[j]));
		gossipsCounted++;
		if (gossipsCounted >= 100){
			gossipsCounted = 0;
			gossipPerSec = 100 * CLOCKS_PER_SEC / (double(clock() - begin - pausedTime));
			pausedTime = 0;
		}
		refresh(*people[personShown]);
		runTime++;
	}
}
