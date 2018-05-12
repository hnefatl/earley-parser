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
    const SymbolType symbolType;
    const std::string value;

public:
    Symbol(const std::string value, const SymbolType symbol);

    bool isTerminal() const;
    bool isNonterminal() const;

    std::string getValue() const;

    bool operator ==(const Symbol &rhs) const;
    bool operator  <(const Symbol &rhs) const;
};

class Rule
{
public:
    const Symbol head;
    const std::vector<Symbol> tail;

    Rule(const Symbol head, const std::vector<Symbol> tail);

    bool operator ==(const Rule &rhs) const;
};

#endif