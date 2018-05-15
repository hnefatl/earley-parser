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
bool Symbol::operator !=(const Symbol &rhs) const
{
    return !(operator==(rhs));
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
bool Rule::operator !=(const Rule &rhs) const
{
    return !(operator==(rhs));
}
bool Rule::operator <(const Rule &rhs) const
{
    if (head != rhs.head)
        return head < rhs.head;
    else if (tail.size() != rhs.tail.size())
        return tail.size() < rhs.tail.size();
    else
    {
        for (std::size_t i = 0; i < tail.size(); ++i)
        {
            if (tail[i] != rhs.tail[i])
                return tail[i] < rhs.tail[i];
        }
        return false;
    }
}