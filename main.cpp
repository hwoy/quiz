#include "quiz.hpp"
#include <iostream>

class winhelper final: public helper {
public:
    winhelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

    std::tuple<player, game::iterator> action(game& gm, player p, game::iterator i, unsigned int key)
    {
        if (!n) {
            avalidmsg();
        }

        else {

            activatemsg();
			
			for(;i!=gm.end();++i)
				p.score+=i->scorepoint;
			
            --n;
        }

        return std::make_tuple(p, i);
    }
};

int main()
{
    game g;

    player voy("Hwoy"), view("View");

    g.addhelper(10,new randomhelper("Random"));
    g.addhelper(11,new doublehelper("Double"));
    g.addhelper(12,new passhelper("Pass"));
    g.addhelper(13,new hinthelper("Hint"));
    g.addhelper(14,new pumphelper("Pump"));
    g.addhelper(15,new winhelper("Win!"));

	{
		quiz q("1+1=?",2);
		q={"1","2","3","4"};
		g.push_back(q);
	}

	{
		quiz q("What's my name?",1);
		q={"Hwoy","View","Kung","Ring"};
		g.push_back(q);
	}

	{
		quiz q("2*2=?",1);
		q={"4","8","2","16"};
		g.push_back(q);
	}

	{
		quiz q("1*2=?",3);
		q={"0","1","2","4"};
		g.push_back(q);
	}

    g.shuffle();
    g.play(voy);
    voy.sum();

    return 0;
}