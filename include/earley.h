#ifndef _EARLEY_H
#define _EARLEY_H

#include "defs.h"

#include <vector>
#include <memory>
#include <set>
#include <map>

class Edge
{
private:
    const Rule rule;
    std::vector<Symbol>::const_iterator rulePosition;

    unsigned int start;
    unsigned int end;

    std::vector<std::vector<const Edge *>> history;

public:
    Edge(const Rule r, const unsigned int start, const unsigned int end);

    Symbol getHead() const;

    bool completed() const;
    Symbol nextSymbol() const;
    void feedTerminal();
    void completeNonterminal(std::shared_ptr<Edge> completingEdge);

    unsigned int getStart() const;
    unsigned int getEnd() const;
};

class Parser
{
private:
    const Symbol startSymbol;
    const std::map<Symbol, std::vector<Rule>> rules;
    std::vector<std::set<std::shared_ptr<Edge>>> edges;

    const std::map<Symbol, std::set<std::string>> partsOfSpeech;

    const std::vector<std::string> words;
    std::vector<std::string>::const_iterator currentWord;

    void predict();
    void scan();
    void complete();

    std::map<Symbol, std::vector<Rule>> makeRuleMapping(const std::vector<Rule> &rules) const;

public:
    Parser(const Symbol startSymbol, const std::vector<Rule> rules,
           const std::map<Symbol, std::set<std::string>> partsOfSpeech, const std::vector<std::string> words);

};

#endif