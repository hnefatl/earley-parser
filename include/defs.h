#ifndef _DEFS_H
#define _DEFS_H

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

    std::string getValue() const;
};

class Rule
{
private:
    Symbol head;
    std::vector<Symbol> tail;

    std::vector<Symbol>::iterator current;

public:
    Rule(Symbol head, std::vector<Symbol> tail);
};

#endif