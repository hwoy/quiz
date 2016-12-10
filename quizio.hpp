#ifndef _QUIZIO_H_
#define _QUIZIO_H_

#include "quiz.hpp"
#include <iostream>
#include <string>
#include <utility>

template <typename T>
void showhelper(const T& g)
{
    bool zero = true;
    for (const auto& i : g.help)
        if (i.second->n) {
            zero = false;
            break;
        }

    if (zero)
        return;

    std::cout << "[ ";

    for (const auto& i : g.help)
        if (i.second->n)
            std::cout << i.second->name << "(" << i.second->n << ")"
                      << "=" << i.first << " ";
    std::cout << "]\n";
}

template <typename T>
void showkey(const T& g)
{
    for (const auto& i : g.keymap) {
        std::string key;
        unsigned int id;

        std::tie(key, id) = i;

        std::cout << key << "(" << g.keystr.at(id) << ") ";
    }

    std::cout << std::endl;
}

template <typename T>
unsigned int choosequiz(const T& g)
{
    bool valid;
    unsigned int key;

    do {
        std::cout << " Please choose -> ";
        std::tie(valid, key) = getkey(g.keymap);
    } while (!valid);

    return key;
}

template <typename T>
void showquiz(const T& g, typename T::const_iterator i)
{
    std::cout << std::distance(g.begin(), i) + 1 << "/" << (g.n <= g.size() ? g.n : g.size()) << " [ " << i->quizstr << " ]\n";

    for (auto j = i->begin(); j != i->end(); ++j) {

        std::cout << std::distance(i->begin(), j) + 1 << ") " << *j << std::endl;
    }
}

#endif
