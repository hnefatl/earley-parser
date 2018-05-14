#include "earley.h"

#include <assert.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <queue>

Parser::Parser(const Symbol start, const std::vector<Rule> rules,
               const std::map<Symbol, std::set<std::string>> poS)
    : startSymbol(start), rules(makeRuleMapping(rules)), partsOfSpeech(poS)
{
    // All symbols in the PoS must be nonterminals
    assert(std::all_of(poS.begin(), poS.end(), [](auto p) { return p.first.isNonterminal(); }));
    // There must be exactly one rule with the starting state as head
    assert(std::count_if(rules.begin(), rules.end(), [start](auto r) { return r.head == start; }) == 1);
}
std::map<Symbol, std::vector<Rule>> Parser::makeRuleMapping(const std::vector<Rule> &rules) const
{
    std::map<Symbol, std::vector<Rule>> mapping;
    for (const auto r : rules)
        mapping[r.head].push_back(r);
    return mapping;
}

unsigned int Parser::parse(const std::string &sentence)
{
    std::vector<std::string> tokens;
    std::stringstream ss(sentence);

    std::string tok;
    while (ss >> tok)
        tokens.push_back(tok);

    return parse(tokens);
}
unsigned int Parser::parse(const std::vector<std::string> &words)
{
    // Initialise the new edge chart
    const auto startRule = rules.at(startSymbol).front();

    edgeNumber = 0;
    edges.clear();
    edges.push_back({ std::make_shared<Edge>(nextEdgeNumber(), startRule, 0, 0) });

    // Perform parsing
    auto currentWord = words.begin();
    while (currentWord != words.end())
    {
        predict();

        // Insert new empty set of edges
        edges.push_back({});

        scan(currentWord);
        complete();

        ++currentWord;
    }

    // Check if we parsed successfully
    const auto &lastSet = edges.back();
    std::vector<std::shared_ptr<Edge>> completeParses;
    for (const auto e : lastSet)
    {
        if (e->getHead() == startSymbol && e->completed() && e->getStart() == 0 && e->getEnd() == words.size())
            completeParses.push_back(e);


        // Regenerate parse trees, demonstrate ambiguity etc
    }

    return completeParses.size();
}

void Parser::predict()
{
    auto &lastGen = edges.back();

    // Iterate over the last generation, find the next nonterminals we need to fill
    for (const auto e : lastGen)
    {
        if (e->completed()) // Skip completed edges, we can't predict anything from them
            continue;

        Symbol eNext = e->nextSymbol();
        if (eNext.isTerminal())
            continue;

        // For each rule that would provide a derivation for this nonterminal,
        // add a new edge to our current generation
        const auto ruleIt = rules.find(eNext);
        if (ruleIt == rules.end()) // No such rule
            continue;

        for (const auto &r : ruleIt->second)
        {
            bool alreadyExisted = !lastGen.insert(std::make_shared<Edge>(edgeNumber, r, e->getEnd(), e->getEnd())).second;
            if (!alreadyExisted)
                ++edgeNumber;
        }
    }
}
void Parser::scan(const std::vector<std::string>::const_iterator currentWord)
{
    const auto &lastGen = edges[edges.size() - 2];
    auto &currentGen = edges[edges.size() - 1];

    // Cache the lookaheads we've made this generation to avoid recomputing them
    std::set<Symbol> lookaheads;
    for (const auto e : lastGen)
    {
        if (e->completed()) // Skip completed edges, we don't need to scan them
            continue;

        const Symbol eNext = e->nextSymbol();

        // Next symbol is both a terminal and the next word in the input
        if (eNext.isTerminal() && eNext.getValue() == *currentWord)
        {
            // Copy the edge we're going to advance
            std::shared_ptr<Edge> newEdge = e->copy(nextEdgeNumber());
            newEdge->feedTerminal(); // Advance the edge by the terminal
            currentGen.insert(newEdge);
        }
        // Nonterminal that we've not made a lookahead for already
        else if (eNext.isNonterminal() && lookaheads.find(eNext) == lookaheads.end())
        { 
            // Cache that we've now tried a lookahead for this nonterminal
            lookaheads.insert(eNext);
            
            const auto &partOfSpeech = partsOfSpeech.find(eNext);
            if (partOfSpeech == partsOfSpeech.end())
                continue; // Nonterminal isn't a part of speech

            // Perform lookahead for this nonterminal
            const auto &words = partOfSpeech->second;
            if (words.find(*currentWord) != words.end())
            {
                // We've looked ahead and found a match for this nonterminal in the sentence
                // Make a new "fake rule" matching this nonterminal to the word we found
                const Rule rule(eNext, std::vector<Symbol> { Symbol(*currentWord, SymbolType::Terminal) });
                const auto newEdge = std::make_shared<Edge>(nextEdgeNumber(), rule, edges.size() - 2, edges.size() - 1);
                newEdge->feedTerminal();
                currentGen.insert(newEdge);
            }
        }
    }
}

void Parser::complete()
{
    auto &currentGen = edges.back();

    std::queue<std::shared_ptr<Edge>> currentGenEdges;
    for (const auto e : currentGen)
        currentGenEdges.push(e);

    while (!currentGenEdges.empty())
    {
        const std::shared_ptr<Edge> e = currentGenEdges.front();
        currentGenEdges.pop();

        if (!e->completed()) // Only consider completed edges
            continue;

        const auto &completeableEdges = edges.at(e->getStart());
        for (const auto ce : completeableEdges)
        {
            // The edge we're trying to complete doesn't need this nonterminal yet
            if (ce->completed() || ce->nextSymbol() != e->getHead())
                continue;

            // We can't match these edges together as they don't actually line up in the sentence
            if (ce->getEnd() != e->getStart())
                continue;

            // Make a copy of the edge we're going to complete a nonterminal for
            std::shared_ptr<Edge> newEdge = ce->copy(edgeNumber);
            newEdge->completeNonterminal(e); // Log which edge completed this new edge
            bool alreadyExisted = !currentGen.insert(newEdge).second;
            if (!alreadyExisted)
            {
                ++edgeNumber;
                currentGenEdges.push(newEdge);
            }
        }
    }
}

void Parser::printChart() const
{
    printChart(std::cout);
}
void Parser::printChart(std::ostream &out) const
{
    std::vector<std::vector<EdgeString>> chart;

    for (const auto &es : edges)
    {
        chart.push_back({});

        // Sort the edges by edgeNumber
        std::vector<std::shared_ptr<Edge>> sorted(es.begin(), es.end());
        std::sort(sorted.begin(), sorted.end(),
            [](const auto e1, const auto e2) { return e1->getEdgeNumber() < e2->getEdgeNumber(); });

        for (const auto e : sorted)
            chart.back().push_back(e->print());
    }

    std::size_t edgeNumberWidth = 0, ruleWidth = 0, spanWidth = 0, historyWidth = 0;
    for (const auto &generation : chart)
    {
        for (const auto &edge : generation)
        {
            edgeNumberWidth = std::max(edgeNumberWidth, edge.edgeNumber.size());
            ruleWidth = std::max(ruleWidth, edge.ruleProgress.size());
            spanWidth = std::max(spanWidth, edge.span.size());
            historyWidth = std::max(historyWidth, edge.history.size());
        }
    }

    unsigned int wordCount = 0;
    const std::string spacing = "    ";

    out << std::left; // Left align output
    for (const auto &generation : chart)
    {
        out << "Word " << wordCount++ << std::endl;
        for (const auto &edge : generation)
        {
            out << std::setw(edgeNumberWidth) << edge.edgeNumber << spacing;
            out << std::setw(ruleWidth) << edge.ruleProgress << spacing;
            out << std::setw(spanWidth) << edge.span << spacing;
            out << std::setw(historyWidth) << edge.history << std::endl;
        }
    }
}