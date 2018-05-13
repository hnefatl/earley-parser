#ifndef _EARLEY_H
#define _EARLEY_H

#include "defs.h"
#include "edge.h"

#include <vector>
#include <memory>
#include <set>
#include <map>
#include <ostream>

class Parser
{
private:
    const Symbol startSymbol;
    const std::map<Symbol, std::vector<Rule>> rules;
    // Need to use pointers as we modify this set frequently, and we don't want to invalidate pointers to edges
    std::vector<std::set<std::shared_ptr<Edge>, EdgePointerComparator>> edges;

    unsigned int edgeNumber;
    unsigned int nextEdgeNumber() { return edgeNumber++; }

    const std::map<Symbol, std::set<std::string>> partsOfSpeech;

    void predict();
    void scan(const std::vector<std::string>::const_iterator currentWord);
    void complete();

    std::map<Symbol, std::vector<Rule>> makeRuleMapping(const std::vector<Rule> &rules) const;

public:
    Parser(const Symbol startSymbol, const std::vector<Rule> rules,
           const std::map<Symbol, std::set<std::string>> partsOfSpeech);

    bool parse(const std::string &sentence);
    bool parse(const std::vector<std::string> &words);

    void printChart() const;
    void printChart(std::ostream &out) const;
};

#endif