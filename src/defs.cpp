#include "defs.h"

Symbol::Symbol(const std::string value, const SymbolType symbol)
    : value(value), symbolType(symbol)
{
}
bool Symbol::isTerminal() const
{
    return symbolType == SymbolType::Terminal;
}
bool Symbol::isNonterminal() const
{
    return symbolType == SymbolType::Nonterminal;
}
std::string Symbol::getValue() const
{
    return value;
}

bool Symbol::operator ==(const Symbol &rhs) const
{
    return symbolType == rhs.symbolType && value == rhs.value;
}
bool Symbol::operator <(const Symbol &rhs) const
{
    if (symbolType != rhs.symbolType)
        return symbolType < rhs.symbolType;
    else
        return value < rhs.value;
}


Rule::Rule(Symbol head, std::vector<Symbol> tail)
    : head(head), tail(tail)
{
}

bool Rule::operator ==(const Rule &rhs) const
{
    return head == rhs.head && tail == rhs.tail;
}