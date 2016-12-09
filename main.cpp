#include <exception>
#include <exception>
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
#define DELIM ":"

static const std::map<unsigned int, std::string> err = { { 1, "File can not be access!" },
    { 2,
        "An Question has not an answer." },
    { 3, "Number of Question record doesn not match" }, { 4, "Question ID doesn't match" },
    { 5,
        "Number of Answer record doesn not match" },
    { 6, "Answer ID doesn't match" }, { 7, "Can not covert to a number" }, { 8, "Invalid Option" }, { 9, "File IO failed!" } };

enum errid : unsigned int {
    file = 1,
    question_answer = 2,
    question_n = 3,
    question_id = 4,
    answer_n = 5,
    answer_id = 6,
    NaN = 7,
    invalid_opt = 8,
    file_io = 9
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

static std::pair<unsigned int, unsigned int> init(game& g, std::ifstream& ifs)
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
            return std::make_pair(errid::question_n, line);
        else if (grap[0].compare(Q))
            return std::make_pair(errid::question_id, line);

        q.clear();
        q.quizstr = grap[1];

        do {
            if (ifs.eof())
                return std::make_pair(errid::question_answer, line);

            line++;

            ifs.getline(buff.get(), BSIZE);

            grap.clear();
            grap.action(buff.get(), DELIM);

        } while (grap.empty());

        if (grap.size() < 3)
            return std::make_pair(errid::answer_n, line);
        else if (grap[0].compare(A))
            return std::make_pair(errid::answer_id, line);

        for (auto i = grap.begin(); i != grap.end(); ++i) {
            auto j = std::distance(grap.begin(), i);

            switch (j) {
            case 0:
                break;
            case 1:
                try {
                    q.answer = std::stoul(grap[j]);
                } catch (const std::exception& e) {
                    std::cerr << " Exception what():" << e.what() << std::endl;
                    return std::make_pair(errid::NaN, line);
                }

                break;
            default:
                q.push_back(grap[j]);
            }
        }

        g.push_back(q);
    }

    return std::make_pair(0, line);
}

static unsigned int showerr(const std::map<unsigned int, std::string>& err, unsigned int retcode, unsigned int line)
{
    std::cerr << " FILE: " << file << std::endl;
    std::cerr << " Line: " << line << std::endl;
    std::cerr << " Error code:" << retcode << " = " << err.at(retcode) << std::endl;
    return retcode;
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
                std::cerr << " Exception what():" << e.what() << std::endl;
                std::cerr << " Error code:" << errid::NaN << " = " << err.at(errid::NaN) << std::endl;
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
            std::cerr << " Error code:" << errid::invalid_opt << " = " << err.at(errid::invalid_opt) << std::endl;
            std::cerr << std::endl;
            showHelp(argv, option, optionstr);
            return errid::invalid_opt;
        }
    }

    //********************* Add Questions ************************

    {
        unsigned int retcode, line;
        std::ifstream ifs(file);

        if (!ifs) {
            std::cerr << " FILE: " << file << std::endl;
            std::cerr << " Error code:" << errid::file << " = " << err.at(errid::file) << std::endl;
            return errid::file;
        }
        ifs.exceptions(std::ifstream::badbit);
        try {
            std::tie(retcode, line)
                = init(g, ifs);
        } catch (const std::exception& e) {
            std::cerr << " Exception what():" << e.what() << std::endl;
            return errid::file_io;
        }

        if (retcode)
            return showerr(err, retcode, line);
    }

    //********************* Add Helper ************************

    g.addhelper(10, new randomhelper("Random"));
    g.addhelper(11, new doublehelper("Double"));
    g.addhelper(12, new passhelper("Pass"));
    g.addhelper(13, new hinthelper("Hint"));
    g.addhelper(14, new pumphelper("Pump"));

    //********************* Play game ************************

    if (shuffle)
        g.shuffle();

    for (auto i = g.begin(); i != g.end();) {
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