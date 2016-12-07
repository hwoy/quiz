#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "Cgrap.h"
#include "Copt.h"
#include "quiz.hpp"

#define BSIZE 256
#define Q "Q"
#define A "A"
#define PLAYER "Hwoy"
#define NQUIZ 10

static std::map<int, std::string> err = { { 1, "File can not be access!" },
    { 2,
        "An Question has not an answer." },
    { 3, "Number of Question record doesn not match" }, { 4, "Question doesn't with Q" },
    { 5,
        "Number of Answer record doesn not match" },
    { 6, "Answer doesn't start with A" }, { 7, "-n: option error not a number" }, { 8, "Invalid Option" } };

static const std::vector<std::string> option = { "-f:", "-p:", "-n:", "-s", "-h" };
enum optid : unsigned int { opt_f,
    opt_p,
    opt_n,
    opt_s,
    opt_h };

static const std::vector<std::string> optionstr = { "Quiz File", "Player Name", "Number of Quiz", "Don't Shuffle", "Help" };

class winhelper final : public helper {
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

            for (; i != gm.end(); ++i)
                p.score += i->scorepoint;

            --n;
        }

        return std::make_tuple(p, i);
    }
};

static void showHelp(const char *argv[],const std::vector<std::string>& option, const std::vector<std::string>& optionstr)
{
    for (unsigned int i = 0; i < option.size() && i < optionstr.size(); ++i)
        std::cerr << option[i] << " = " << optionstr[i] << std::endl;
	
	std::cerr << "\nExample\n";
	std::cerr << grappath(argv[0]) << " " << option[optid::opt_f] << "quiz.txt\n";
}

static std::pair<int, unsigned int> init(game& g, std::ifstream& ifs)
{
    unsigned int line = 0;

    std::unique_ptr<char[]> buff(new char[BSIZE]);
    Cgrap grap;
    quiz q;

    while (!ifs.eof()) {
        ifs.getline(buff.get(), BSIZE);
        line++;
        grap.clear();
        grap.action(buff.get(), ":");

        if (grap.empty())
            continue;

        if (grap.size() != 2)
            return std::make_pair(3, line);
        else if (grap[0].compare(Q))
            std::make_pair(4, line);

        q.clear();
        q.quizstr = grap[1];

        do {
            if (ifs.eof())
                return std::make_pair(2, line);

            ifs.getline(buff.get(), 256);
            line++;
            grap.clear();
            grap.action(buff.get(), ":");

        } while (grap.empty());

        if (grap.size() < 3)
            return std::make_pair(5, line);
        else if (grap[0].compare(A))
            return std::make_pair(6, line);

        for (auto i = grap.begin(); i != grap.end(); ++i) {
            auto j = std::distance(grap.begin(), i);

            switch (j) {
            case 0:
                break;
            case 1:
                q.answer = std::stoull(grap[j]);
                break;
            default:
                q.push_back(grap[j]);
            }
        }

        g.push_back(q);
    }

    return std::make_pair(0, line);
}

int main(int argc, const char* argv[])
{
    bool shuffle = true;
    unsigned int id;
	
    game g;
    player voy(PLAYER);
    std::string file;
    Copt opt(argc, argv, option);
    std::string str;

    g.n = NQUIZ;

    if (opt.argc == 1) {
        showHelp(argv,option, optionstr);
        return 0;
    }

    while (std::tie(id, str) = opt.action(), id != Copt::ID::END) {
        switch (id) {
        case optid::opt_f:
            file = str;
            break;

        case optid::opt_p:
            voy.assign(str);
            break;

        case optid::opt_n:
            if (!isnum(str)) {
                std::cerr << " Error code:" << 7 << " = " << err[7] << std::endl;
                return 7;
            }
            g.n = std::stoul(str);
            break;

        case optid::opt_s:
            shuffle = false;
            break;

        case optid::opt_h:
            showHelp(argv,option, optionstr);
            return 0;
            break;

        default:
            std::cerr << "Option: " << str << " is invalid\n";
            std::cerr << " Error code:" << 8 << " = " << err[8] << std::endl;
            std::cerr << std::endl;
            showHelp(argv,option, optionstr);
            return 1;
        }
    }

    g.addhelper(10, new randomhelper("Random"));
    g.addhelper(11, new doublehelper("Double"));
    g.addhelper(12, new passhelper("Pass"));
    g.addhelper(13, new hinthelper("Hint"));
    g.addhelper(14, new pumphelper("Pump"));
    //g.addhelper(15, new winhelper("Win!"));

    {
        int retcode;
        unsigned int line;
        std::ifstream ifs(file);

        if (!ifs) {
            std::cerr << " Error code:" << 1 << " = " << err[1] << std::endl;
            return 1;
        }

        std::tie(retcode, line)
            = init(g, ifs);

        if (retcode) {
            std::cerr << "line: " << line << " Error code:" << retcode << " = " << err[retcode] << std::endl;
            return retcode;
        }
    }

    if (shuffle)
        g.shuffle();

    for (game::iterator i = g.begin(); i != g.end();) {
        game::GAMEID id;
        std::tie(id, i) = g.play(voy, i);

        if (id == game::GAMEID::ID_QUIT) {
            break;
        } else if (id == game::GAMEID::ID_OVER) {
            break;
        }
    }

    voy.sum();

    return 0;
}