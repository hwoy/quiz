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

        if (p.choose >= help.front()->key && p.choose <= help.back()->key)
            help[p.choose - help[0]->key]->action(*this, p, i);
        else if (p.choose >= 1 && p.choose <= i->size()) {
            if (p.choose == i->answer)
                ++p.score;
            ++i;
        }
    }
}

void game::showhelper() const
{
    bool zero = true;
    for (const auto& i : help)
        if (i->n) {
            zero = false;
            break;
        }

    if (zero)
        return;

    std::cout << "[ ";

    for (const auto& i : help)
        if (i->n)
            std::cout << i->name << "(" << i->n << ")"
                      << "=" << i->key << " ";
    std::cout << "]\n\n";
}

void game::reset(unsigned int n)
{
    for (auto& i : help)
        i->n = n;
}

void game::shuffle(unsigned int i)
{
    for (unsigned int j = 0; j < i; j++)
        std::shuffle(begin(), end(), gen);
}

void game::choosequiz(player& p)
{
    std::cout << std::endl;
    std::cout << p << ": Please choose -> ";
    std::cin >> p.choose;
    std::cout << std::endl;
}

void game::showquiz(iterator i) const
{
    std::cout << i - begin() + 1 << "/" << size() << " [ " << i->quiz << " ]\n";

    for (auto j = i->begin(); j != i->end(); ++j) {

        std::cout << j - i->begin() + 1 << ") " << *j << std::endl;
    }
}

void game::addhelper(helper* h)
{
    help.push_back(std::unique_ptr<helper>(h));
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

void randomhelper::action(game& gm, player& p, game::iterator& i)
{
    if (!n) {
        avalidmsg();
        return;
    }
    activatemsg();
    std::shuffle(i, gm.end(), gm.gen);
    --n;
}

void doublehelper::action(game& gm, player& p, game::iterator& i)
{
    if (!n) {
        avalidmsg();
        return;
    }
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
        p.score++;
    --n;
    ++i;
}

void passhelper::action(game& gm, player& p, game::iterator& i)
{
    if (!n) {
        avalidmsg();
        return;
    }
    activatemsg();
    p.score++;
    --n;
    ++i;
}

void hinthelper::action(game& gm, player& p, game::iterator& i)
{
    if (!n) {
        avalidmsg();
        return;
    }

    activatemsg();

    std::cout << "May be: " << ((gm.gen() % 2) ? i->answer : (gm.gen() % i->size() + 1)) << "\n\n";
    --n;
}

void pumphelper::action(game& gm, player& p, game::iterator& i)
{
    if (!n) {
        avalidmsg();
        return;
    }

    activatemsg();

    std::vector<unsigned int> vec;

    for (unsigned int i = 0; i < gm.help.size(); i++)
        if (key != gm.help[i]->key)
            vec.push_back(i);

    unsigned int j = vec[gm.gen() % (vec.size())];

    gm.help[j]->n++;
    std::cout << gm.help[j]->name << " is pubped\n";
    --n;
}