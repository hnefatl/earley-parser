#include "edge.h"

#include <assert.h>

Edge::Edge(const unsigned int edgeNumber, const Rule r, const unsigned int start, const unsigned int end)
    : edgeNumber(edgeNumber), rule(r), start(start), end(end)
{
    rulePosition = rule.tail.begin();
}
std::shared_ptr<Edge> Edge::copy(const unsigned int edgeNumber) const
{
    // Basic copy - same rule, start, end, but new edgenumber
    std::shared_ptr<Edge> e(new Edge(edgeNumber, this->rule, this->start, this->end));

    // Make sure the new iterator points to the same place as ours
    const auto d = std::distance(rule.tail.begin(), rulePosition);
    std::advance(e->rulePosition, d);

    return e;
}

Symbol Edge::getHead() const
{
    return rule.head;
}

bool Edge::completed() const
{
    return rulePosition == rule.tail.end();
}
Symbol Edge::nextSymbol() const
{
    assert(!completed());

    return *rulePosition;
}

void Edge::feedTerminal()
{
    assert(nextSymbol().isTerminal());

    ++rulePosition;
}
void Edge::completeNonterminal(const std::shared_ptr<Edge> completingEdge)
{
    // Still not sure how to engineer this bit
    ++rulePosition;
}

unsigned int Edge::getStart() const
{
    return start;
}
unsigned int Edge::getEnd() const
{
    return end;
}
unsigned int Edge::getEdgeNumber() const
{
    return edgeNumber;
}

bool Edge::operator <(const Edge &rhs) const
{
    // If two edges have the same number, they should be identical
    if (edgeNumber == rhs.edgeNumber)
        assert(*this == rhs);

    if (rule != rhs.rule)
        return rule < rhs.rule;
    else
    {
        const auto d1 = std::distance(rule.tail.begin(), rulePosition);
        const auto d2 = std::distance(rhs.rule.tail.begin(), rhs.rulePosition);

        if (d1 != d2)
            return d1 < d2;
        else if (start != rhs.start)
            return start < rhs.start;
        else if (end != rhs.end)
            return end < rhs.end;
        else
            return history < rhs.history;
    }
}
bool Edge::operator ==(const Edge &rhs) const
{
    return edgeNumber == rhs.edgeNumber &&
           rule == rhs.rule &&
           std::distance(rule.tail.begin(), rulePosition) == std::distance(rhs.rule.tail.begin(), rhs.rulePosition) &&
           start == rhs.start &&
           end == rhs.end &&
           history == rhs.history;
}

std::ostream &operator <<(std::ostream &out, const Edge &rhs)
{
    out << rhs.edgeNumber << ": " << rhs.rule.head << " ->";

    // Output the tail symbols with a dot before the current symbol
    for (auto i = rhs.rule.tail.begin(); i != rhs.rule.tail.end(); ++i)
    {
        if (rhs.rulePosition == i)
            out << " .";
        out << " " << *i;
    }
    if (rhs.rulePosition == rhs.rule.tail.end())
        out << " .";

    out << "\t(" << rhs.start << "," << rhs.end << ")" << "\tHistory";

    return out;
}