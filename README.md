# Hashgraph
Simple implementation of the consensus algorithm Hashgraph with clear visualizer

## Compiling and running
- Run `make`. An executable will compile. Currently only tested on OS X.
- Run with `./hashgraph`.
- Switch to view current graphs of different members using `keys "1-6"` (icon in uper-left corner inicates the current view graph owner)
- Press `"L" key` to turn the file loggingon/off (default being to log all consused nodes in separate file for each player, logged info include the two md5 hash keys, creation timestamp and consensus timespemp, and possible payload)
- Press `"F" key` to turn on forks, in this mode some members will occationally create false nodes, those nodes would not be approved
- Press `Space key` to pause/resume gossiping

![alt text](https://github.com/conanwu777/hashgraph/blob/master/1.png)

## Abstract
This project is done in collaboration with Liam Dehaudt at 42. It is implemented according to the paper by L. Baird, 2016.
https://www.swirlds.com/downloads/SWIRLDS-TR-2016-01.pdf

Hashgraph is considered a strong and more efficient alternative to current block-chain technology. It achieved consensus through frequent communication between all parties(a.k.a. gossips). The algorithm comes with a voting system which 'well known' nodes (i.e. gets 'famous' status by being seen and strongly seen by others) in each round votes to approve newer nodes. With probablity 1, all nodes which are authenatic will be aproved. One of the most interesting thing about this structure is that votes are collected indevidually by each player and yet all will arrive at the exact same result.

In the project, we made an implementation in small scale of the full algorithm, with varying number for population (the speed and visualizing works best for 4-10 people)

## Outline of gossip & approval process
- Each entity(pony) in the population is represented by a vertical line
- When Alice gossips to Bob, she looks through bob's subgraph in her current graph, finds all the new nodes in her graph but not in the subgraph, makes a list and send all of them over
- When Bob receives the gossip, he updates his graph with all verifiable nodes Alice sent, and then he creates a new node and adds it to his own graph with two hashes linking it to the newest nodes in his and Alice's line (self-parent and gossip parent) This transaction documents the event of the gossip
- All new nodes are colored **BLUE** some nodes can carry a **payload** of transaction reflected in the neyworth of each member under their icons
- For computation efficiency, every once in a while there is a layer of special noces, i.e. **whitnesses** for each active player, colored **RED**, they will be where the heavy computation (seeing, voting) is focused on
- When whitnesses are **strongly seen** by many players, they become **famous witnesses** (colored **YELLOW**); Occationally, some whitnesses may be voted **not famous**, those nodes will be colored **GREY** and will never be famous
- When a new node is seen by all the **uniquely famous whitnesses** of a round, a consensus timestamp is put on and the node is considered approved (colored **GREEN**)
- Eventually all authentic nodes (i.e. no forking created by members) would be approved with probablity 1; eventualy there will be no discarpency between members regarding the status of nodes, regardless of the fact that members never communicate the voting

### Picture 1: Breakdown of different types of nodes
![alt text](https://github.com/conanwu777/hashgraph/blob/master/2.png)

### Picture 2: Strongly see
- Node A can **see** node B if A is an ancestor of B and there is no fork between A and B
- Node A can **strongly see** node B if A can see B with paths which collaboratively pass through nodes created by 2/3 of the population
![alt text](https://github.com/conanwu777/hashgraph/blob/master/3.png)

### Picture 3: Vote famous
![alt text](https://github.com/conanwu777/hashgraph/blob/master/4.png)
