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

    enum ID : unsigned int { END = -2U,
        OTHER = -1U };

	template <typename iterator>
    Copt(int argc, const char** argv, iterator bparam, iterator eparam, unsigned int start = 1)
        : argc(argc)
        , index(start)
        , start(start)
        , argv(argv)
        , param(bparam,eparam)
    {
    }
	
	template <typename T,std::size_t N>
    Copt(int argc, const char** argv, const T (&param)[N], unsigned int start = 1)
        : argc(argc)
        , index(start)
        , start(start)
        , argv(argv)
        , param(param,param+N)
    {
    }

	template <typename iterator>
    void init(int argc, const char** argv, iterator bparam, iterator eparam, unsigned int start = 1)
    {
        this->argc = argc;
        this->argv = argv;
        this->param.assign(bparam, eparam);
        this->index = start;
        this->start = start;
    }

	template <typename T,std::size_t N>	
    void init(int argc, const char** argv, const T (&param)[N], unsigned int start = 1)
    {
        this->argc = argc;
        this->argv = argv;
        this->param.assign(param, param+N);
        this->index = start;
        this->start = start;
    }

    std::pair<unsigned int, std::string> action()
    {

        for (unsigned int j = index; j < reinterpret_cast<unsigned int&>(argc); j++) {

            for (auto i = param.begin(); i != param.end(); ++i) {

                if (!std::strncmp(argv[j], i->c_str(), i->size())) {
                    index = j + 1;
                    return std::make_pair(i-param.begin(), std::string(argv[j] + i->size()));
                }
            }

            index = j + 1;

            return std::make_pair(ID::OTHER, std::string(argv[j]));
        }

        return std::make_pair(ID::END, std::string());
    }
};
#endif
