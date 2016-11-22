#include <functional>
#include <vector>
#include "quiz.hpp"

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

void game::play(player& p)
{
    for (auto i = begin(); i != end();) {
        showhelper();
        showquiz(i);
        choosequiz(p);

        if (!help.empty() && help.find(p.choose) != help.end())
            std::tie(p, i) = help[p.choose]->action(*this, p, i, p.choose);
        else if (p.choose >= 1 && p.choose <= i->size()) {
            if (p.choose == i->answer)
                p.score+=i->scorepoint;
            ++i;
        }
    }
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
    std::cout << "]\n\n";
}

void game::reset(unsigned int n)
{
    for (auto& i : help)
        std::get<1>(i)->n = n;
}

void game::shuffle(unsigned int i)
{
	std::vector<std::reference_wrapper<value_type>> vec(begin(),end());
    for (unsigned int j = 0; j < i; j++)
        std::shuffle(vec.begin(), vec.end(), gen);
}

void game::choosequiz(player& p)
{
    std::cout << std::endl;
    std::cout << p << ": Please choose -> ";
    std::cin >> p.choose;
    std::cout << std::endl;
}

void game::showquiz(iterator i)
{
    std::cout << std::distance(begin(),i) + 1 << "/" << size() << " [ " << i->quizstr << " ]\n";

    for (auto j = i->begin(); j != i->end(); ++j) {

        std::cout << std::distance(i->begin(),j) + 1 << ") " << *j << std::endl;
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
		std::vector<std::reference_wrapper<game::value_type>> vec(i,gm.end());
        activatemsg();
        std::shuffle(vec.begin(), vec.end(), gm.gen);
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

            do {
                std::cout << "Remain: " << j << "\n\n";

                gm.showquiz(i);
                gm.choosequiz(p);

                if (p.choose == i->answer)
                    win = true;

            } while (!(p.choose >= 1 && p.choose <= i->size()));

        } while (--j);

        if (win)
            p.score+=i->scorepoint;
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
        p.score+=i->scorepoint;
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
									: std::uniform_int_distribution<>(1, i->size())(gm.gen)) << "\n\n";
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