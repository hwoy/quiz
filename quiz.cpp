#include "quiz.hpp"
#include <functional>
#include <vector>

bool isnum(const std::string& str)
{
    for (const auto i : str)
        if (!(i >= '0' && i <= '9'))
            return false;

    return true;
}

std::tuple<bool, unsigned int> getkey(const std::map<std::string, unsigned int>& keymap)
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
            retint = std::get<1>(*i);
        }

    }

    else {
        retbool = true;
        retint = std::stoul(str);
    }

    return std::make_tuple(retbool, retint);
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

std::tuple<game::GAMEID, game::iterator> game::play(player& p, game::iterator i)
{
    GAMEID id;
    unsigned int key;
	
    showhelper();
	showkey();
	std::cout << std::endl;
	
    showquiz(i);
	
    std::cout << std::endl;
    key = choosequiz();
    std::cout << std::endl;

    if (key == GAMEID::ID_QUIT) {
        id = GAMEID::ID_QUIT;
    } else if (key == GAMEID::ID_REDRAW) {
        id = GAMEID::ID_REDRAW;
    } else {
        if (!help.empty() && help.find(key) != help.end())
            std::tie(p, i) = help[key]->action(*this, p, i, key);
        else if (key >= 1 && key <= i->size()) {
            if (key == i->answer)
                p.score += i->scorepoint;
            ++i;
        }
        id = (static_cast<unsigned int>(std::distance(begin(),i))>=n||i == end()) ? GAMEID::ID_OVER : GAMEID::ID_NORMAL;
    }

    return std::make_tuple(id, i);
}

void game::showhelper() const
{
    bool zero = true;
    for (const auto& i : help)
        if (std::get<1>(i)->n) {
            zero = false;
            break;
        }

    if (zero)
        return;

    std::cout << "[ ";

    for (const auto& i : help)
        if (std::get<1>(i)->n)
            std::cout << std::get<1>(i)->name << "(" << std::get<1>(i)->n << ")"
                      << "=" << std::get<0>(i) << " ";
    std::cout << "]\n";
}

void game::showkey()
{
	for(const auto &i:keymap)
	{
		std::string key;
		unsigned int id;
		
		std::tie(key,id)=i;
		
		std::cout << key << "(" << keystr[id] << ") ";
	}
	
	std::cout << std::endl;
}

void game::reset(unsigned int n)
{
    for (auto& i : help)
        std::get<1>(i)->n = n;
}

void game::shuffle(unsigned int i)
{

    std::vector<value_type> vec(begin(), end());
    for (unsigned int j = 0; j < i; j++)
        std::shuffle(vec.begin(), vec.end(), gen);
	
	assign(vec.begin(),vec.end());
}

unsigned int game::choosequiz() const
{
    bool valid;
    unsigned int key;

    do {
        std::cout << " Please choose -> ";
        std::tie(valid, key) = getkey(keymap);
    } while (!valid);

    return key;
}

void game::showquiz(iterator i)
{
    std::cout << std::distance(begin(), i) + 1 << "/" << size() << " [ " << i->quizstr << " ]\n";

    for (auto j = i->begin(); j != i->end(); ++j) {

        std::cout << std::distance(i->begin(), j) + 1 << ") " << *j << std::endl;
    }
}

void game::addhelper(unsigned int key, helper* h)
{
    help[key] = std::unique_ptr<helper>(h);
}

//************************** Helper methodes **********************************
void helper::avalidmsg() const
{
    std::cout << name << " is no longer avaliable\n\n";
}

void helper::activatemsg() const
{
    std::cout << name << " is activted\n\n";
}

std::tuple<player, game::iterator> randomhelper::action(game& gm, player p, game::iterator i, unsigned int key)
{
    if (!n) {
        avalidmsg();
    }

    else {
        std::vector<game::value_type> vec(gm.begin(), gm.end());
        activatemsg();
		
        std::shuffle(vec.begin()+std::distance(gm.begin(),i), vec.end(), gm.gen);
		gm.assign(vec.begin(),vec.end());
        --n;
    }
    return std::make_tuple(p, i);
}

std::tuple<player, game::iterator> doublehelper::action(game& gm, player p, game::iterator i, unsigned int key)
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

                gm.showquiz(i);
                choose = gm.choosequiz();

                if (choose == i->answer)
                    win = true;

            } while (!(choose >= 1 && choose <= i->size()));

        } while (--j);

        if (win)
            p.score += i->scorepoint;
        --n;
        ++i;
    }
    return std::make_tuple(p, i);
}

std::tuple<player, game::iterator> passhelper::action(game& gm, player p, game::iterator i, unsigned int key)
{
    if (!n) {
        avalidmsg();
    } else {
        activatemsg();
        p.score += i->scorepoint;
        --n;
        ++i;
    }

    return std::make_tuple(p, i);
}

std::tuple<player, game::iterator> hinthelper::action(game& gm, player p, game::iterator i, unsigned int key)
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

    return std::make_tuple(p, i);
}

std::tuple<player, game::iterator> pumphelper::action(game& gm, player p, game::iterator i, unsigned int key)
{
    if (!n) {
        avalidmsg();
    }

    else {

        std::vector<unsigned int> vec;

        for (const auto& i : gm.help)
            if (key != std::get<0>(i))
                vec.push_back(std::get<0>(i));

        if (vec.empty())
            return std::make_tuple(p, i);

        activatemsg();

        unsigned int j = vec[gm.gen() % (vec.size())];

        gm.help[j]->n++;
        std::cout << gm.help[j]->name << " is pubped\n";
        --n;
    }

    return std::make_tuple(p, i);
}