#include "earley.h"

Symbol::Symbol(const std::string value, const SymbolType symbol)
{
    this->value = value;
    this->symbolType = symbol;
}
bool Symbol::isTerminal() const
{
    return symbolType == SymbolType::Terminal;
}
bool Symbol::isNonterminal() const
{
    return symbolType == SymbolType::Nonterminal;
}