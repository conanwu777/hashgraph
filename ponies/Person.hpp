#ifndef PERSON_HPP
#define PERSON_HPP

#include "Event.hpp"
#include "Hashgraphs.hpp"

class Person;
class Event;

struct data;

class Person{
private:
	std::vector<Event*> hashgraph;
	std::list<Event*> finishedNodes;
	void	createEvent(int const & time, Person & gossiper);
	Event	*getTopNode(Person const &) const;
	Event	*getForkNode(Person const &) const;
	int		currentRound;
	std::ofstream ofs;
public:
	int		index;
	Person();
	~Person();
	Person(Person const &);
	Person&	operator=(Person const &);
	std::vector<float>	networth;

	Person(int const &);
	bool	operator==(Person const &);
	void	recieveGossip(Person &, std::vector<data> const & gossip);
	const std::vector<Event*>	*getHashgraph() const;
	const std::list<Event*>	*getFinishedNodes() const;
	void	findOrder();
	int		finalizeOrder(int const & n, int const & r,
		std::vector<Event*> const & w);
	void	outputOrder(int const & n);
	void	linkEvents(std::vector<Event*> const & );
	void	insertEvent(Event const &);
	std::vector<Event*>	findWitnesses(int const & round) const;
	void	gossip(Person &);
	int		getCurRound();
	void	incCurRound();
	void	removeOldBalls();
};

#endif
