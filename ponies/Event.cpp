#include "Event.hpp"
#include "Hashgraphs.hpp"

Event::Event(Person &p, data const & data) : graph(const_cast<std::vector<Event *>*>(p.getHashgraph())), d(data),
selfParent(NULL), gossiperParent(NULL), consensusTimestamp(-1), roundRecieved(-1),
round(0), witness(d.selfHash == "\0" ? true : false), famous(-1)
{
	hash = makeHash();
}

Event::Event(){}
Event::~Event(){}
Event::Event(const Event &rhs){
	*this = rhs;
}

std::string Event::makeHash()
{
	std::ostringstream s;

	s << *this;
	return md5_hash(s.str());
}

Event&	Event::operator=(const Event &rhs)
{
	graph = rhs.getGraph();
	d = rhs.getData();
	gossiperParent = NULL;
	selfParent = NULL;
	consensusTimestamp = rhs.getConsensusTimestamp();
	roundRecieved = rhs.getRoundRecieved();
	round = rhs.getRound();
	witness = rhs.getWitness();
	famous = rhs.getFamous();
	hash = makeHash();
	return (*this);
}

void	Event::divideRounds()
{
	if (!this->selfParent || !this->gossiperParent)
	{
		round = 0;
		return;
	}
	round = this->selfParent->getRound();
	if (this->gossiperParent->getRound() > round)
		round = this->gossiperParent->getRound();
	int numStrongSee = 0;
	std::vector<Event*> witnesses = people[d.owner]->findWitnesses(round);
	for (unsigned int i = 0; i < witnesses.size(); i++){
		if (numStrongSee > 2 * N / 3) 
			break;
		if (stronglySee(*witnesses[i]))
			numStrongSee++;
	}
	if (numStrongSee > 2 * N / 3)
	{
		round = round + 1;
		if (people[d.owner]->getCurRound() < round)
			people[d.owner]->incCurRound();
	}
	witness = (getSelfParent() == NULL || getSelfParent()->getRound() < round);
}

bool	Event::operator==(Event const & rhs) const
{
	return (hash == rhs.getHash());
}

bool	Event::seeRecursion(Event const & y, std::vector<Event*> & forkCheck,
	bool & done, std::vector<Event*> & visited){
	if (std::find(visited.begin(), visited.end() , this) != visited.end())
		return (*this == y);
	visited.push_back(this);
	if (d.owner == y.getData().owner)
		forkCheck.push_back(this);
	if (done)
		return true;
	if (*this == y)
	{
		done = true;
		return true;
	}
	if (d.timestamp < y.getData().timestamp)
		return false;
	if (!this->getSelfParent())
		return false;
	return this->getSelfParent()->seeRecursion(y, forkCheck, done, visited) ||
	this->getGossiperParent()->seeRecursion(y, forkCheck, done, visited);
}

bool	Event::see(Event const & y)
{
	if (hashesSeen.find(y.getHash()) != hashesSeen.end())
		return true;
	if (hashesNotSeen.find(y.getHash()) != hashesSeen.end())
		return false;
	std::vector<Event*> forkCheck;
	std::vector<Event*> visited;
	bool done = false;
	bool b = seeRecursion(y, forkCheck, done, visited);
	if (b == false)
	{
		hashesNotSeen.insert(y.getHash());
		return false;
	}
	for (unsigned int i = 0; i < forkCheck.size(); i++)
		for (unsigned int j = i + 1; j < forkCheck.size(); j++)
			if (fork(forkCheck[i],forkCheck[j]))
			{
				hashesNotSeen.insert(y.getHash());	
				return false;
			}
	hashesSeen.insert(y.getHash());	
	return true;
}

bool	Event::ancestorRecursion(Event const & y, bool & done,
std::vector<Event*> & visited)
{
	if (std::find(visited.begin(), visited.end() , this) != visited.end())
		return (*this == y);
	visited.push_back(this);
	if (done)
		return true;
	if (*this == y)
	{
		return true;
		done = true;
	}
	if (d.timestamp < y.getData().timestamp)
		return false;
	if (!this->getSelfParent())
		return false;
	return (this->getSelfParent()->see(y)) || (this->getGossiperParent()->see(y));
}

bool	Event::ancestor(Event const & y)
{
	bool b;
	std::vector<Event*> visited;
	bool done = false;

	if (hashesSeen.find(y.getHash()) != hashesSeen.end())
		return true;
	if (ancestorsNotSeen.find(y.getHash()) != ancestorsNotSeen.end())
		return false;
	b = ancestorRecursion(y, done, visited);
	if (!b){
		ancestorsNotSeen.insert(y.getHash());		
	}
	return b;
}

void	Event::decideFame()
{
	std::vector<Event*> s;
	unsigned int count;
	unsigned int countNo;
	bool v;
	int d;

	if (!witness || round < 2)
		return ;
	for (unsigned int x = graph->size() - 1; x < graph->size(); x--)
	{
		if ((*graph)[x]->getWitness() && (*graph)[x]->getFamous() == -1
			&& (*graph)[x]->getRound() <= round - 2)
		{
			s = people[getData().owner]->findWitnesses((*graph)[x]->getRound() + 1);
			count = 0;
			countNo = 0;
			for (unsigned int y = 0; y < s.size(); y++)
			{
				if (!stronglySee(*(s[y]))){
					s.erase(s.begin() + y);
				}
				else {
					if (s[y]->see(*((*graph)[x])))
						count++;
					else
						countNo++;
				}
			}
			d = round - (*graph)[x]->getRound();
			if (count > 2 * N / 3)
				(*graph)[x]->setFamous(1);
			else if (countNo > 2 * N / 3)
				(*graph)[x]->setFamous(0);
			else if (!(d % C))
				(*graph)[x]->setFamous((*graph)[x]->getHash()[16] % 2);
		}
	}
}

bool	Event::stronglySee(Event const & y)
{
	int numSee = 0;
	std::array<bool, N> found = {false};
	for (unsigned int n = 0; n < graph->size(); n++)
	{
		if (found[(*graph)[n]->getData().owner] == true
			|| (*graph)[n]->getRound() < y.getRound())
			continue ;
		if (this->see(*((*graph)[n])) && (*graph)[n]->see(y))
		{
			numSee++;
			found[(*graph)[n]->getData().owner] = true;
			if (numSee > 2 * N / 3){
				return true;
			}
		}
	}
	return false;
}

bool	Event::fork(Event *x, Event *y){
	Event *t;

	if (!(x->getData().owner == y->getData().owner))
		return 0;
	t = x;
	while (t)
	{
		if (t == y)
			return 0;
		t = t->getSelfParent();
	}
	t = y;
	while (t)
	{
		if (t == x)
			return 0;
		t = t->getSelfParent();
	}
	return 1;
}

std::vector<Event*>	*Event::getGraph() const {
	return (graph);
}

Event	*Event::getSelfParent() const {    
	return (selfParent);
}
Event	*Event::getGossiperParent() const {    
	return (gossiperParent);
}
data	Event::getData() const{
	return d;
}
int		Event::getRound() const {    
	return (round);
}
bool	Event::getWitness() const {    
	return (witness);
}
std::string	Event::getHash() const{
	return hash;
}
int		Event::getConsensusTimestamp() const {    
	return (consensusTimestamp);
}   
int		Event::getRoundRecieved() const {    
	return (roundRecieved);
}
char	Event::getFamous() const {    
	return (famous);
}
void	Event::setFamous(char const & fame){
	famous = fame;
}
void	Event::setRoundReceived(int const & r) {
	roundRecieved = r;
}
void	Event::setConsensusTimestamp(int const & t) {
	consensusTimestamp = t;
}
void	Event::setSelfParent(Event* const e) {
	selfParent = e;
}
void	Event::setGossiperParent(Event* const e) {
	gossiperParent = e;
}

std::ostream&	operator<<(std::ostream& os, const Event& e)
{  
	os << e.getData().selfHash << e.getData().gossipHash
	<< e.getData().timestamp << e.getData().owner;
	return os;  
}
