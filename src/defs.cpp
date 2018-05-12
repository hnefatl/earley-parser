#include "defs.h"

Symbol::Symbol(const std::string value, const SymbolType symbol)
    :value(value), symbolType(symbol)
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

Rule::Rule(Symbol head, std::vector<Symbol> tail)
    :head(head), tail(tail)
{
}