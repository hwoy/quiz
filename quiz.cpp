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

game::game()
    : gen(std::time(nullptr))
{
    help.push_back(std::unique_ptr<helper>(new randomhelper("Random", 10)));
    help.push_back(std::unique_ptr<helper>(new doublehelper("Double", 11)));
    help.push_back(std::unique_ptr<helper>(new passhelper("Pass", 12)));
    help.push_back(std::unique_ptr<helper>(new hinthelper("Hint", 13)));
    help.push_back(std::unique_ptr<helper>(new pumphelper("Pump", 14)));
}

void game::play(player& p)
{
    for (auto i = begin(); i != end(); ++i) {
        showhelper();
        showquiz(i);
        choosequiz(p);

        if (p.choose >= help.front()->key && p.choose <= help.back()->key)
            help[p.choose - help[0]->key]->action(*this, p, i);
        else if (p.choose == i->answer)
            p.score++;
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

    unsigned int n = 1;
    for (const auto& j : *i) {

        std::cout << n++ << ") " << j << std::endl;
    }
}

//************************** Helper methodes **********************************

void randomhelper::action(game& gm, player& p, game::iterator& i)
{
    if (!n) {
        std::cout << name << " is no longer avaliable\n\n";
    } else {
        std::cout << name << " is activted\n\n";
        std::shuffle(i, gm.end(), gm.gen);
        --n;
    }
    --i;
}

void doublehelper::action(game& gm, player& p, game::iterator& i)
{
    if (!n) {
        std::cout << name << " is no longer avaliable\n\n";
        return;
    }
    unsigned int j = 2;
    bool win = false;

    std::cout << name << " is activted\n\n";

    do {
        std::cout << "Remain: " << j << "\n\n";

        gm.showquiz(i);
        gm.choosequiz(p);
        if (p.choose == i->answer)
            win = true;

    } while (--j);

    if (win)
        p.score++;
    --n;
}

void passhelper::action(game& gm, player& p, game::iterator& i)
{
    if (!n) {
        std::cout << name << " is no longer avaliable\n\n";
        return;
    }
    std::cout << name << " is activted\n\n";
    p.score++;
    --n;
}

void hinthelper::action(game& gm, player& p, game::iterator& i)
{
    if (!n) {
        std::cout << name << " is no longer avaliable\n\n";
        return;
    }

    std::cout << name << " is activted\n\n";
    unsigned int j;

    j = (gm.gen() % 2) ? i->answer : (gm.gen() % i->size() + 1);
    std::cout << "May be: " << j << "\n\n";
    --n;
    --i;
}

void pumphelper::action(game& gm, player& p, game::iterator& i)
{
    if (!n) {
        std::cout << name << " is no longer avaliable\n\n";
        return;
    }

    std::cout << name << " is activted\n\n";

    std::vector<unsigned int> vec;

    for (unsigned int i = 0; i < gm.help.size(); i++)
        if (key != gm.help[i]->key)
            vec.push_back(i);

    unsigned int j = vec[gm.gen() % (vec.size())];

    gm.help[j]->n++;
    std::cout << gm.help[j]->name << " is pubped\n";
    --n;
    --i;
}