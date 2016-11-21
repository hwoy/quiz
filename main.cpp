#include "quiz.hpp"
#include <iostream>

class winhelper : public helper {
public:
    winhelper(const char* name, unsigned int n = 1)
        : helper(name, n)
    {
    }

	void action(game& gm, player& p, game::iterator& i, unsigned int key)
	{
		if (!n) {
			avalidmsg();
			return;
		}
		
		activatemsg();
		
		p.score+=std::distance(i,gm.end());
		i=gm.end();
			

		--n;
	}
};

int main()
{
    game g;
    quiz q;

    player voy("Hwoy"), view("View");

    //g.addhelper(new randomhelper("Random", 10));
    //g.addhelper(new doublehelper("Double", 11));
    //g.addhelper(12,new passhelper("Pass"));
    //g.addhelper(new hinthelper("Hint", 13));
    g.addhelper(14,new pumphelper("Pump"));
	g.addhelper(15,new winhelper("Win!"));

    q.quiz = "1+1=?";
    q.clear();
    q.push_back("1");
    q.push_back("2");
    q.push_back("3");
    q.push_back("4");
    q.answer = 2;
    g.push_back(q);

    q.quiz = "What's my name?";
    q.clear();
    q.push_back("Hwoy");
    q.push_back("View");
    q.push_back("Kung");
    q.push_back("Ring");
    q.answer = 1;
    g.push_back(q);

    q.quiz = "2*2=?";
    q.clear();
    q.push_back("4");
    q.push_back("5");
    q.push_back("55");
    q.push_back("6");
    q.answer = 1;
    g.push_back(q);

    q.quiz = "1*2=?";
    q.clear();
    q.push_back("0");
    q.push_back("1");
    q.push_back("2");
    q.push_back("6");
    q.answer = 3;
    g.push_back(q);

    g.shuffle();
    g.play(voy);
    voy.sum();

    return 0;
}