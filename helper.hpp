#ifndef _HELPER_H_
#define _HELPER_H_

#include <algorithm>
#include <string>
#include <utility>

#include "quiz.hpp"
#include "quizio.hpp"

//************************** Helper Class **********************************
class helper {
public:
    unsigned int n;
    unsigned int key;
    std::string name;

    helper(const char* name, unsigned int n)
        : n(n)
        , name(name)
    {
    }

    virtual ~helper() {}

    void avalidmsg() const
    {
        std::cout << name << " is no longer avaliable\n\n";
    }

    void activatemsg() const
    {
        std::cout << name << " is activted\n\n";
    }

    virtual std::pair<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key) = 0;
};

class randomhelper final : public helper {
public:
    randomhelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::pair<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key)
    {
        if (!n) {
            avalidmsg();
        }

        else {
            std::vector<game::value_type> vec(gm.begin(), gm.end());
            activatemsg();

            std::shuffle(vec.begin() + std::distance(gm.begin(), i), vec.end(), gm.gen);
            gm.assign(vec.begin(), vec.end());
            --n;
        }
        return std::make_pair(p, i);
    }
};

class doublehelper final : public helper {
public:
    doublehelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::pair<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key)
    {
        if (!n) {
            avalidmsg();
        }

        else {
            unsigned int j = 2;
            bool win = false;

            activatemsg();

            do {
                unsigned int choose;
                do {
                    std::cout << "Remain: " << j << "\n\n";

                    showquiz(gm, i);
                    choose = choosequiz(gm);

                    if (choose == i->answer)
                        win = true;

                } while (!(choose >= 1 && choose <= i->size()));

            } while (--j);

            if (win)
                p.score += i->scorepoint;
            --n;
            ++i;
        }
        return std::make_pair(p, i);
    }
};

class passhelper final : public helper {
public:
    passhelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::pair<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key)
    {
        if (!n) {
            avalidmsg();
        } else {
            activatemsg();
            p.score += i->scorepoint;
            --n;
            ++i;
        }

        return std::make_pair(p, i);
    }
};

class hinthelper final : public helper {
public:
    hinthelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::pair<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key)
    {
        if (!n) {
            avalidmsg();
        }

        else {

            activatemsg();

            std::cout << "May be: " << (std::uniform_int_distribution<>(0, 1)(gm.gen) ? i->answer
                                                                                      : std::uniform_int_distribution<>(1, i->size())(gm.gen))
                      << "\n\n";
            --n;
        }

        return std::make_pair(p, i);
    }
};

class pumphelper final : public helper {
public:
    pumphelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::pair<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key)
    {
        if (!n) {
            avalidmsg();
        }

        else {

            std::vector<unsigned int> vec;

            for (const auto& i : gm.help)
                if (key != i.first)
                    vec.push_back(i.first);

            if (vec.empty())
                return std::make_pair(p, i);

            activatemsg();

            unsigned int j = vec[gm.gen() % (vec.size())];

            gm.help[j]->n++;
            std::cout << gm.help[j]->name << " is pubped\n";
            --n;
        }

        return std::make_pair(p, i);
    }
};

class winhelper final : public helper {
public:
    winhelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::pair<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key)
    {
        if (!n) {
            avalidmsg();
        }

        else {

            activatemsg();

            for (; i != gm.end(); ++i)
                p.score += i->scorepoint;

            --n;
        }

        return std::make_pair(p, i);
    }
};

bool isnum(const std::string& str);

std::pair<bool, unsigned int> getkey(const std::map<std::string, unsigned int>& keymap);

#endif
