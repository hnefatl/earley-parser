#include <iostream>

#include "earley.h"

Parser makeDemoParser1();
Parser makeDemoParser2();

int main(int argc, char *argv[])
{
    Parser p = makeDemoParser1();
    const std::string sentence = "they can fish in rivers";
    //const std::string sentence = "she eats a quite fresh fish with a silver fork";

    unsigned int interpretations = p.parse(sentence);

    p.printChart();

    std::cout << std::endl;
    std::cout << "Possible interpretations: " << interpretations << std::endl;

    return 0;
}

Parser makeDemoParser1()
{
    Symbol S("S", SymbolType::Nonterminal);
    Symbol NP("NP", SymbolType::Nonterminal);
    Symbol VP("VP", SymbolType::Nonterminal);
    Symbol PP("PP", SymbolType::Nonterminal);
    Symbol N("N", SymbolType::Nonterminal);
    Symbol V("V", SymbolType::Nonterminal);
    Symbol P("P", SymbolType::Nonterminal);

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
    partsOfSpeech.emplace(N, std::set<std::string>{ "they", "can", "fish", "rivers", "December" });
    partsOfSpeech.emplace(P, std::set<std::string>{ "in" });
    partsOfSpeech.emplace(V, std::set<std::string>{ "can", "fish" });

    return Parser(S, rules, partsOfSpeech);
}

Parser makeDemoParser2()
{
    Symbol S("S", SymbolType::Nonterminal);
    Symbol VP("VP", SymbolType::Nonterminal);
    Symbol PP("PP", SymbolType::Nonterminal);
    Symbol NP("NP", SymbolType::Nonterminal);
    Symbol A("A", SymbolType::Nonterminal);
    Symbol N("N", SymbolType::Nonterminal);
    Symbol V("V", SymbolType::Nonterminal);
    Symbol Adv("Adv", SymbolType::Nonterminal);
    Symbol Pn("Pn", SymbolType::Nonterminal);
    Symbol Det("Det", SymbolType::Nonterminal);
    Symbol P("P", SymbolType::Nonterminal);

    std::vector<Rule> rules;
    rules.emplace_back(S, std::vector<Symbol>{ NP, VP });
    rules.emplace_back(VP, std::vector<Symbol>{ VP, PP });
    rules.emplace_back(VP, std::vector<Symbol>{ V, NP });
    rules.emplace_back(VP, std::vector<Symbol>{ V });
    rules.emplace_back(PP, std::vector<Symbol>{ P, NP });
    rules.emplace_back(NP, std::vector<Symbol>{ Det, N });
    rules.emplace_back(NP, std::vector<Symbol>{ N });
    rules.emplace_back(NP, std::vector<Symbol>{ Pn });
    rules.emplace_back(NP, std::vector<Symbol>{ Det, A, N });
    rules.emplace_back(NP, std::vector<Symbol>{ A, NP });
    rules.emplace_back(A, std::vector<Symbol>{ Adv, A });
    rules.emplace_back(A, std::vector<Symbol>{ A, A });

    std::map<Symbol, std::set<std::string>> partsOfSpeech;
    partsOfSpeech.emplace(Adv, std::set<std::string>{ "too", "very", "quite" });
    partsOfSpeech.emplace(Pn, std::set<std::string>{ "she", "he" });
    partsOfSpeech.emplace(A, std::set<std::string>{ "fresh", "tasty", "silver" });
    partsOfSpeech.emplace(N, std::set<std::string>{ "fish", "fork", "apple" });
    partsOfSpeech.emplace(V, std::set<std::string>{ "eats" });
    partsOfSpeech.emplace(Det, std::set<std::string>{ "a", "an", "the" });
    partsOfSpeech.emplace(P, std::set<std::string>{ "with" });

    return Parser(S, rules, partsOfSpeech);
}