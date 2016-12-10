#include <algorithm>
#include <vector>

#include "quiz.hpp"
#include "helper.hpp"
#include "quizio.hpp"

bool isnum(const std::string& str)
{
    for (const auto i : str)
        if (!(i >= '0' && i <= '9'))
            return false;

    return str.empty() ? false : true;
}

std::pair<bool, unsigned int> getkey(const std::map<std::string, unsigned int>& keymap)
{
    std::string str;
    std::cin >> str;

    bool retbool;
    unsigned int retint;

    if (!isnum(str)) {
        auto i = keymap.find(str);
        if (i == keymap.end()) {
            retbool = false;
            retint = 0;
        }

        else {
            retbool = true;
            retint = i->second;
        }

    }

    else {
        retbool = true;
        retint = std::stoul(str);
    }

    return std::make_pair(retbool, retint);
}

//************************** Player methodes **********************************

void player::sum() const
{
    std::cout << *this << ": Your score = " << score << std::endl;
}

void player::reset()
{
    score = 0;
}

//************************** Game methodes **********************************

std::pair<game::GAMEID, game::iterator> game::play(player& p, game::iterator i, unsigned int key)
{
    GAMEID id;

    if (!keystr.empty() && keystr.find(key) != keystr.end()) {
        id = reinterpret_cast<game::GAMEID&>(key);
    } else {
        if (!help.empty() && help.find(key) != help.end())
            std::tie(p, i) = help[key]->action(*this, p, i, key);
        else if (key >= 1 && key <= i->size()) {
            if (key == i->answer)
                p.score += i->scorepoint;
            ++i;
        }
        id = (static_cast<unsigned int>(std::distance(begin(), i)) >= n || i == end()) ? GAMEID::ID_OVER : GAMEID::ID_NORMAL;
    }

    return std::make_pair(id, i);
}

void game::reset(unsigned int n)
{
    for (auto& i : help)
        i.second->n = n;
}

void game::shuffle(unsigned int i)
{

    std::vector<value_type> vec(begin(), end());
    for (unsigned int j = 0; j < i; j++)
        std::shuffle(vec.begin(), vec.end(), gen);

    assign(vec.begin(), vec.end());
}

void game::addhelper(unsigned int key, helper* h)
{
    help[key].reset(h);
}

bool game::isover(const_iterator i) const
{
    return (static_cast<unsigned int>(std::distance(begin(), i)) >= n) || (i == end());
}

void game::addkey(unsigned int gid, const char* kpress, const char* kstr)
{
    keymap[kpress] = gid;
    keystr[gid] = kstr;
}
