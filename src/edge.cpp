#include "edge.h"

#include <assert.h>

std::ostream &operator <<(std::ostream &out, const EdgeString &e)
{
    return out << e.edgeNumber << " " << e.ruleProgress << " " << e.span << " " << e.history;
}

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

    // Copy the history as well
    e->history = history;

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
    // The two edges need to make a chain
    assert(getEnd() == completingEdge->getStart());

    // Remember that this nonterminal was completed by the edge
    history.push_back(completingEdge);
    // Move the end along as this edge now encapsulates the completing edge
    end = completingEdge->getEnd();
    // Consume the nonterminal that was completed
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

EdgeString Edge::print() const
{
    EdgeString out;
    out.edgeNumber = "e" + std::to_string(edgeNumber);

    out.ruleProgress = rule.head.getValue() + " ->";
    // Output the tail symbols with a dot before the current symbol
    for (auto i = rule.tail.begin(); i != rule.tail.end(); ++i)
    {
        if (rulePosition == i)
            out.ruleProgress += " .";
        out.ruleProgress += " " + i->getValue();
    }
    if (rulePosition == rule.tail.end())
        out.ruleProgress += " .";

    out.span = "(" + std::to_string(start) + "," + std::to_string(end) + ")";

    if (history.empty())
        out.history = "";
    else
    {
        auto hi = history.begin(); // History iterator
        auto ti = rule.tail.begin(); // Tail iterator
        out.history = "(";
        for (ti; ti != rule.tail.end(); ++ti)
        {
            if (ti->isNonterminal() && hi != history.end())
            {
                out.history += "e" + std::to_string((**hi).edgeNumber) + ","; // Dereference iterator then edge pointer
                ++hi; // Move history iterator to the next nonterminal we completed
            }
        }
        // Replace the trailing comma with a close bracket
        out.history.back() = ')';
    }

    return out;
}