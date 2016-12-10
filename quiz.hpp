#ifndef _QUIZ_H_
#define _QUIZ_H_

#include <chrono>
#include <list>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <utility>

class helper;

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
    typedef std::list<quiz>::const_iterator const_iterator;
    typedef std::list<quiz>::value_type value_type;
    enum GAMEID : unsigned int { ID_NORMAL = 0,
        ID_OVER = -1U,
        ID_QUIT = -2U,
        ID_REDRAW = -3U };

    std::mt19937 gen;
    std::map<unsigned int, std::shared_ptr<helper> > help;
    std::map<std::string, unsigned int> keymap;
    std::map<unsigned int, std::string> keystr;

    unsigned int n;

    game(std::mt19937::result_type t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
        : gen(t)
        , n(10)
    {
    }

    void shuffle(unsigned int i = 1);

    void reset(unsigned int n = 1);

    std::pair<game::GAMEID, game::iterator> play(player& p, game::iterator i, unsigned int key);

    void addhelper(unsigned int key, helper* h);

    bool isover(const_iterator i) const;

    void addkey(unsigned int gid, const char* kpress, const char* kstr);
};

bool isnum(const std::string& str);

std::pair<bool, unsigned int> getkey(const std::map<std::string, unsigned int>& keymap);

#endif
