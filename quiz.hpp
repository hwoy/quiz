#ifndef _QUIZ_H_
#define _QUIZ_H_

#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <tuple>
#include <vector>

class helper;
class randomhelper;
class doublehelper;
class passhelper;
class hinthelper;
class pumphelper;

//************************** Player Class **********************************

class player : public std::string {
public:
    unsigned int choose;
    unsigned int score;

    player(const char* str)
        : std::string(str)
        , score(0)
    {
    }

    void sum() const;

    void reset();
};

//************************** Quiz Class **********************************

class quiz : public std::vector<std::string> {
public:
    std::string quiz;
    unsigned int answer;
};

//************************** Game Class **********************************

class game : public std::vector<quiz> {
public:
    typedef std::vector<quiz>::iterator iterator;

    std::mt19937 gen;
    std::map<unsigned int, std::unique_ptr<helper> > help;

    game(std::mt19937::result_type t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
        : gen(t)
    {
    }

    void shuffle(unsigned int i = 1);

    void reset(unsigned int n = 1);

    void play(player& p);

    void choosequiz(player& p);

    void showquiz(iterator i) const;

    void showhelper() const;

    void addhelper(unsigned int key, helper* h);
};

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

    void avalidmsg() const;
    void activatemsg() const;

    virtual std::tuple<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key) = 0;
};

class randomhelper final: public helper {
public:
    randomhelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::tuple<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key);
};

class doublehelper final: public helper {
public:
    doublehelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::tuple<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key);
};

class passhelper final: public helper {
public:
    passhelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::tuple<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key);
};

class hinthelper final: public helper {
public:
    hinthelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::tuple<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key);
};

class pumphelper final: public helper {
public:
    pumphelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::tuple<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key);
};

#endif