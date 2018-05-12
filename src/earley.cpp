#include "earley.h"

#include <assert.h>
#include <algorithm>

Edge::Edge(const Rule r, const unsigned int start, const unsigned int end)
    : rule(r), rulePosition(rule.tail.begin()), start(start), end(end)
{
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
void Edge::completeNonterminal(std::shared_ptr<Edge> completingEdge)
{
    // Still not sure how to engineer this bit
    assert(false);
}

unsigned int Edge::getStart() const
{
    return start;
}
unsigned int Edge::getEnd() const
{
    return end;
}

Parser::Parser(const Symbol start, const std::vector<Rule> rules,
               const std::map<Symbol, std::set<std::string>> poS, const std::vector<std::string> words)
    : startSymbol(start), rules(makeRuleMapping(rules)), partsOfSpeech(poS), words(words), currentWord(this->words.begin())
{
    assert(std::all_of(poS.begin(), poS.end(), [](auto p) { return p.first.isNonterminal(); }));

    auto startRule = std::find_if(rules.begin(), rules.end(), [start](auto r) { return r.head == start; });

    assert(startRule != rules.end());

    std::shared_ptr<Edge> startEdge = std::make_shared<Edge>(*startRule, 0, 0);

    std::set<std::shared_ptr<Edge>> startSet;
    startSet.insert(startEdge);
    edges.push_back(startSet);
}
std::map<Symbol, std::vector<Rule>> Parser::makeRuleMapping(const std::vector<Rule> &rules) const
{
    std::map<Symbol, std::vector<Rule>> mapping;
    for (auto r : rules)
        mapping[r.head].push_back(r);
    return mapping;
}

void Parser::predict()
{
    auto &lastGen = edges[edges.size() - 2];
    auto &currentGen = edges[edges.size() - 1];

    // Iterate over the last generation, find the next nonterminals we need to fill
    for (auto e : lastGen)
    {
        Symbol eNext = e->nextSymbol();
        if (eNext.isTerminal())
            continue;

        // For each rule that would provide a derivation for this nonterminal,
        // add a new edge to our current generation
        for (auto r : rules.at(eNext))
            currentGen.insert(std::make_shared<Edge>(r, e->getEnd(), e->getEnd()));
    }
}
void Parser::scan()
{
    auto &currentGen = edges.back();

    std::set<std::shared_ptr<Edge>> newEdges;

    // Cache the lookaheads we've made this generation to avoid recomputing them
    std::set<Symbol> lookaheads;
    for (auto e : currentGen)
    {
        Symbol eNext = e->nextSymbol();

        // Next symbol is both a terminal and the next word in the input
        if (eNext.isTerminal() && eNext.getValue() == *currentWord)
        {
            std::shared_ptr<Edge> newEdge = std::make_shared<Edge>(*e);
            newEdge->feedTerminal(); // Advance the edge by the terminal
            newEdges.insert(newEdge);
        }
        else if (eNext.isNonterminal())
        {
            if (lookaheads.find(eNext) == lookaheads.end()) // We've not made a lookahead for this nonterminal before
            {
                auto partOfSpeech = partsOfSpeech.find(eNext);
                if (partOfSpeech != partsOfSpeech.end())
                {
                    auto words = partOfSpeech->second;
                    if (words.find(*currentWord) != words.end())
                    {
                        // We've looked ahead and found a match for this nonterminal in the sentence
                        // Make a new "fake rule" matching this nonterminal to the word we found
                        std::vector<Symbol> tail;
                        tail.emplace_back(*currentWord, SymbolType::Terminal);
                        newEdges.insert(std::make_shared<Edge>(Rule(eNext, tail), e->getStart(), e->getStart() + 1));
                    }
                }
                // Cache that we've already tried a lookahead for this nonterminal
                lookaheads.insert(eNext);
            }
        }
    }
}