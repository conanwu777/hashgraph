#ifndef EVENT_HPP
#define EVENT_HPP
#include "Person.hpp"
#include "Hashgraphs.hpp" 
#include <unordered_set>

class Person;

 struct data{
	float		payload;
	int			target;
	std::string	selfHash;
	std::string	gossipHash;
	int			timestamp;
	int			owner;
};

class Event{
private:
	std::vector<Event*> *graph;
	std::unordered_set<std::string> ancestorsNotSeen;
	std::unordered_set<std::string> hashesSeen;
	std::unordered_set<std::string> hashesNotSeen;
	std::string hash;
	data	d;
	Event	*selfParent;
	Event	*gossiperParent;
	int		consensusTimestamp;
	int		roundRecieved;
	int		round;
	bool	witness;
	char	famous;
	std::string makeHash();

public:
	Event();
	~Event();
	Event(const Event &);
	Event & operator=(const Event &);
	Event(Person &, data const &) ;
	bool	operator==(Event const &) const;
	bool	operator<(const Event &) const;
	bool	ancestor(Event const &);
	bool	ancestorRecursion(Event const &, bool &, std::vector<Event*> &);
	bool	see(Event const &);
	bool	seeRecursion(Event const &, std::vector<Event*> &, bool &, std::vector<Event*> &);
	bool	stronglySee(Event const &);
	bool	getWitness() const;
	void	divideRounds();
	void	setFamous(char const &);
	void	setRoundReceived(int const &);
	void	setConsensusTimestamp(int const &);
	void	setSelfParent(Event* const e);
	void	setGossiperParent(Event* const e);
	void	decideFame();
	char	getFamous() const;
	int		getRound() const;
	int		getConsensusTimestamp() const;
	int		getRoundRecieved() const;
	Event	*getSelfParent() const;
	Event	*getGossiperParent() const;
	data	getData() const;
	std::string			getHash() const;
	std::vector<Event*>	*getGraph() const;

	static bool fork(Event*, Event*);
};

std::ostream&	operator<<(std::ostream & os, const Event & e); 

#endif
