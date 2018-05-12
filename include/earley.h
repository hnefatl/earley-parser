#ifndef _EARLEY_H
#define _EARLEY_H

#include <vector>
#include <string>

enum SymbolType
{
    Terminal,
    Nonterminal,
};

class Symbol
{
private:
    SymbolType symbolType;
    std::string value;

public:
    Symbol(const std::string value, const SymbolType symbol);

    bool isTerminal() const;
    bool isNonterminal() const;
};

class Rule
{
private:
    std::vector<std::string> symbols;

public:

};

#endif