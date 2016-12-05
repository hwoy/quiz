#include <cstring>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#ifndef _COPT_H
#define _COPT_H

class Copt {
public:
    int argc;
    unsigned int index;
    unsigned int start;
    const char** argv;

    std::vector<std::string> param;

    enum ID : unsigned int { END = -2UL,
        OTHER = -1UL };

    Copt(int argc, const char** argv, const std::vector<std::string>& param, unsigned start = 1)
        : argc(argc)
        , index(start)
        , start(start)
        , argv(argv)
        , param(param)
    {
    }

    void init(int argc, const char** argv, const std::vector<std::string>& param, unsigned int start)
    {
        this->argc = argc;
        this->argv = argv;
        this->param.assign(param.begin(), param.end());
        this->index = start;
        this->start = start;
    }

    std::pair<unsigned int, std::string> action()
    {

        for (unsigned int j = index; j < reinterpret_cast<unsigned int&>(argc); j++) {

            for (auto i = param.begin(); i != param.end(); ++i) {

                if (!std::strncmp(argv[j], i->data(), i->size())) {
                    index = j + 1;
                    return std::make_pair(std::distance(param.begin(), i), std::string(&argv[j][i->size()]));
                }
            }

            index = j + 1;

            return std::make_pair(ID::OTHER, std::string(&argv[j][0]));
        }

        return std::make_pair(ID::END, std::string("End of Argument"));
    }

    std::pair<unsigned int, std::string> operator()()
    {
        return action();
    }
};
#endif
