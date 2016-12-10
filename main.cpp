#include <exception>
#include <fstream>
#include <iostream>
#include <string>

#include "Cgrap.h"
#include "Copt.h"
#include "helper.hpp"
#include "quiz.hpp"
#include "quizio.hpp"

#define BSIZE 256
#define Q "Q"
#define A "A"
#define PLAYER "Hwoy"
#define NQUIZ 10
#define DELIM ":"

class initexception final : public std::exception {
private:
    std::string msg;

public:
    initexception(unsigned int line, unsigned int eid, const std::map<unsigned int, std::string>& err)
    {
        msg = std::string(" Line:") + std::to_string(line) + "\n" + " Error id:" + std::to_string(eid) + " = " + err.at(eid);
    }

    const char* what() const noexcept
    {
        return msg.c_str();
    }
};

static const std::map<unsigned int, std::string> err = { { 1, "File IO failed!" },
    { 2, "An Question has not an answer." },
    { 3, "Number of Question record doesn not match" },
    { 4, "Question ID doesn't match" },
    { 5, "Number of Answer record doesn not match" },
    { 6, "Answer ID doesn't match" }, { 7, "Can not covert to a number" }, { 8, "Invalid Option" } };

enum errid : unsigned int {
    file_io = 1,
    question_answer = 2,
    question_n = 3,
    question_id = 4,
    answer_n = 5,
    answer_id = 6,
    NaN = 7,
    invalid_opt = 8,
};

static const std::vector<std::string> option = { "-f:", "-p:", "-n:", "-s", "-h" };
enum optid : unsigned int { opt_f,
    opt_p,
    opt_n,
    opt_s,
    opt_h };

static const std::vector<std::string> optionstr = { "Quiz File", "Player Name", "Number of Quiz", "Don't Shuffle", "Help" };

static void showHelp(const char* argv[], const std::vector<std::string>& option, const std::vector<std::string>& optionstr)
{
    for (unsigned int i = 0; i < option.size() && i < optionstr.size(); ++i)
        std::cerr << option[i] << " = " << optionstr[i] << std::endl;

    std::cerr << "\nExample\n";
    std::cerr << grappath(argv[0]) << " " << option[optid::opt_f] << "quiz.txt\n";
}

static void init(game& g, std::ifstream& ifs, const std::map<unsigned int, std::string>& err)
{
    unsigned int line = 0;

    std::unique_ptr<char[]> buff(new char[BSIZE]);
    Cgrap grap;
    quiz q;

    while (!ifs.eof()) {

        line++;

        ifs.getline(buff.get(), BSIZE);

        grap.clear();
        grap.action(buff.get(), DELIM);

        if (grap.empty())
            continue;

        if (grap.size() != 2)
            throw(initexception(line, errid::question_n, err));

        else if (grap[0].compare(Q))
            throw(initexception(line, errid::question_id, err));

        q.clear();
        q.quizstr = grap[1];

        do {
            if (ifs.eof())
                throw(initexception(line, errid::question_answer, err));

            line++;

            ifs.getline(buff.get(), BSIZE);

            grap.clear();
            grap.action(buff.get(), DELIM);

        } while (grap.empty());

        if (grap.size() < 3)
            throw(initexception(line, errid::answer_n, err));

        else if (grap[0].compare(A))
            throw(initexception(line, errid::answer_id, err));

        for (auto i = grap.begin(); i != grap.end(); ++i) {
            auto j = std::distance(grap.begin(), i);

            switch (j) {
            case 0:
                break;
            case 1:
                try {
                    q.answer = std::stoul(grap[j]);
                } catch (const std::exception& e) {
                    throw(initexception(line, errid::NaN, err));
                }

                break;
            default:
                q.push_back(grap[j]);
            }
        }

        g.push_back(q);
    }
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

    //********************* Option ************************

    if (opt.argc == 1) {
        showHelp(argv, option, optionstr);
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
            try {
                g.n = std::stoul(str);
            } catch (const std::exception& e) {
                std::cerr << " Exception what():\n"
                          << " " << e.what() << std::endl;
                std::cerr << " Error id:" << errid::NaN << " = " << err.at(errid::NaN) << std::endl;
                return errid::NaN;
            }
            break;

        case optid::opt_s:
            shuffle = false;
            break;

        case optid::opt_h:
            showHelp(argv, option, optionstr);
            return 0;
            break;

        default:
            std::cerr << " Option: " << str << " is invalid\n";
            std::cerr << " Error id:" << errid::invalid_opt << " = " << err.at(errid::invalid_opt) << std::endl;
            std::cerr << std::endl;
            showHelp(argv, option, optionstr);
            return errid::invalid_opt;
        }
    }

    //********************* Add Questions ************************

    {
        std::ifstream ifs;
        try {
            try {
                ifs.exceptions(std::ifstream::failbit);
                ifs.open(file);
                init(g, ifs, err);
            } catch (const initexception& e) {
                std::cerr << " File:" << file << std::endl;
                std::cerr << " Exception what():\n"
                          << e.what() << std::endl;
                return 1;
            }
        } catch (const std::exception& e) {
            if (!ifs.eof()) {
                std::cerr << " File:" << file << std::endl;
                std::cerr << " Exception what():\n"
                          << " " << e.what() << std::endl;
                std::cerr << " Error id:" << errid::file_io << " = " << err.at(errid::file_io) << std::endl;
                ;
                return 2;
            }
        }
    }

    //********************* Add Helper ************************

    g.addhelper(10, new randomhelper("Random"));
    g.addhelper(11, new doublehelper("Double"));
    g.addhelper(12, new passhelper("Pass"));
    g.addhelper(13, new hinthelper("Hint"));
    g.addhelper(14, new pumphelper("Pump"));

    //********************* Add Key ************************

    g.addkey(game::GAMEID::ID_QUIT, "-", "Quit");
    g.addkey(game::GAMEID::ID_REDRAW, "+", "Redraw");

    //********************* Play game ************************

    if (shuffle)
        g.shuffle();

    for (auto i = g.begin(); !g.isover(i);) {
        unsigned int key;
        game::GAMEID id;

        showhelper(g);
        showkey(g);
        std::cout << std::endl;
        showquiz(g, i);

        std::cout << std::endl;
        key = choosequiz(g);
        std::cout << std::endl;

        std::tie(id, i) = g.play(voy, i, key);

        if (id == game::GAMEID::ID_QUIT) {
            break;
        } else if (id == game::GAMEID::ID_OVER) {
            break;
        }
    }

    voy.sum();

    return 0;
}