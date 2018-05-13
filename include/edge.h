#ifndef _EDGE_H
#define _EDGE_H

#include "defs.h"

#include <vector>
#include <memory>

class Edge
{
private:
    const unsigned int edgeNumber;
    const Rule rule;
    std::vector<Symbol>::const_iterator rulePosition;

    unsigned int start;
    unsigned int end;

    std::vector<std::vector<const Edge *>> history;

public:
    Edge(const unsigned int edgeNumber, const Rule r, const unsigned int start, const unsigned int end);
    Edge(const Edge &copy) = delete; // Don't allow implicit copies, force them to be explicit

    std::shared_ptr<Edge> copy(const unsigned int edgeNumber) const;

    Symbol getHead() const;

    bool completed() const;
    Symbol nextSymbol() const;
    void feedTerminal();
    void completeNonterminal(const std::shared_ptr<Edge> completingEdge);

    unsigned int getStart() const;
    unsigned int getEnd() const;

    unsigned int getEdgeNumber() const;

    bool operator  <(const Edge &rhs) const;
    bool operator ==(const Edge &rhs) const;
    void operator  =(const Edge &rhs) = delete;

    friend std::ostream &operator <<(std::ostream &out, const Edge &rhs);
};

// Lets us store sets of pointers to edges, with order checked using the edges not the pointers
class EdgePointerComparator
{
public:
    bool operator() (const std::shared_ptr<Edge> e1, const std::shared_ptr<Edge> e2) const
    {
        return *e1 < *e2;
    }
};

#endif