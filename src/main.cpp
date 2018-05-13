#include <iostream>

#include "earley.h"

Parser makeDemoParser();

int main(int argc, char *argv[])
{
    Parser p = makeDemoParser();

    bool success = p.parse("they can fish");

    std::cout << "Parsed Successfully: " << (success ? "true" : "false") << std::endl << std::endl;
    p.printChart();

    return 0;
}

Parser makeDemoParser()
{
    Symbol S("S", SymbolType::Nonterminal);
    Symbol NP("NP", SymbolType::Nonterminal);
    Symbol VP("VP", SymbolType::Nonterminal);
    Symbol PP("PP", SymbolType::Nonterminal);
    Symbol N("N", SymbolType::Nonterminal);
    Symbol V("N", SymbolType::Nonterminal);
    Symbol P("N", SymbolType::Nonterminal);

    std::vector<Rule> rules;
    rules.emplace_back(S,  std::vector<Symbol> { NP, VP });
    rules.emplace_back(NP, std::vector<Symbol> { N, PP });
    rules.emplace_back(NP, std::vector<Symbol> { N });
    rules.emplace_back(PP, std::vector<Symbol> { P, NP });
    rules.emplace_back(VP, std::vector<Symbol> { VP, PP });
    rules.emplace_back(VP, std::vector<Symbol> { V, VP });
    rules.emplace_back(VP, std::vector<Symbol> { V, NP });
    rules.emplace_back(VP, std::vector<Symbol> { V });

    std::map<Symbol, std::set<std::string>> partsOfSpeech;
    partsOfSpeech.emplace(N, std::set<std::string>{ "they", "can", "fish", "rivers" });
    partsOfSpeech.emplace(P, std::set<std::string>{ "in" });
    partsOfSpeech.emplace(V, std::set<std::string>{ "can", "fish" });

    return Parser(S, rules, partsOfSpeech);
}