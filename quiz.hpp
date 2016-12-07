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
#include <utility>

class helper;
class randomhelper;
class doublehelper;
class passhelper;
class hinthelper;
class pumphelper;

bool isnum(const std::string& str);

std::pair<bool, unsigned int> getkey(const std::map<std::string, unsigned int>& keymap);

//************************** Player Class **********************************

class player : public std::string {
public:
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

class quiz : public std::list<std::string> {
public:
    std::string quizstr;
    unsigned int answer;
    unsigned int scorepoint;

    quiz(unsigned int scorepoint = dscorepoint)
        : scorepoint(scorepoint)
    {
    }
    quiz(std::string quizstr, unsigned int answer, unsigned int scorepoint = dscorepoint)
        : quizstr(quizstr)
        , answer(answer)
        , scorepoint(scorepoint)
    {
    }

    quiz& operator=(std::initializer_list<std::string> ilist)
    {
        std::list<std::string>::operator=(ilist);
        return *this;
    }

    static const unsigned int dscorepoint = 1;
};

//************************** Game Class **********************************

class game : public std::list<quiz> {
public:
    typedef std::list<quiz>::iterator iterator;
    typedef std::list<quiz>::value_type value_type;
    enum GAMEID : unsigned int { ID_NORMAL = 0L,
        ID_OVER = -1UL,
        ID_QUIT = -2UL,
        ID_REDRAW = -3UL };

    std::mt19937 gen;
    std::map<unsigned int, std::unique_ptr<helper> > help;
    std::map<std::string, unsigned int> keymap;
    std::map<unsigned int, std::string> keystr;

    unsigned int n;

    game(std::mt19937::result_type t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
        : gen(t)
        , n(10)
    {
        keymap["-"] = GAMEID::ID_QUIT;
        keymap["+"] = GAMEID::ID_REDRAW;

        keystr[GAMEID::ID_QUIT] = "Quit";
        keystr[GAMEID::ID_REDRAW] = "Redraw";
    }

    void shuffle(unsigned int i = 1);

    void reset(unsigned int n = 1);

    std::pair<game::GAMEID, game::iterator> play(player& p, game::iterator i);

    unsigned int choosequiz() const;

    void showquiz(iterator i);

    void showhelper() const;

    void showkey();

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

    virtual std::pair<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key) = 0;
};

class randomhelper final : public helper {
public:
    randomhelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::pair<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key);
};

class doublehelper final : public helper {
public:
    doublehelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::pair<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key);
};

class passhelper final : public helper {
public:
    passhelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::pair<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key);
};

class hinthelper final : public helper {
public:
    hinthelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::pair<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key);
};

class pumphelper final : public helper {
public:
    pumphelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::pair<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key);
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

#endif