# hashgraph
Simple implementation of the consensus algorithm Hashgraph with clear visualizer

## Compiling and running
Run `make`. An executable will compile. Currently only tested on OS X.
Run with `./hashgraph`.

![alt text](https://github.com/conanwu777/hashgraph/blob/master/1.png)

## Abstract
This project is done in collaboration with Liam Dehaudt at 42. It is implemented according to the paper by L. Baird, 2016.
https://www.swirlds.com/downloads/SWIRLDS-TR-2016-01.pdf

Hashgraph is considered a strong and more efficient alternative to current block-chain technology. It achieved consensus through frequent communication between all parties(a.k.a. gossips). The algorithm comes with a voting system which 'well known' nodes (i.e. gets 'famous' status by being seen and strongly seen by others) in each round votes to approve newer nodes. With probablity 1, all nodes which are authenatic will be aproved. One of the most interesting thing about this structure is that votes are collected indevidually by each player and yet all will arrive at the exact same result.

In the project, we made an implementation in small scale of the full algorithm, with varying number for population (the speed and visualizing works best for 4-10 people)

## Simple outline of the gossip & approval process

