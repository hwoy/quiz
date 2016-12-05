#include <iostream>
#include "Cgrap.h"
#include "Copt.h"


int main(int argc,const char *argv[])
{
	Copt opt(argc,argv,{"-a:","-b:"});
	unsigned int id;
	std::string str;
	
	while(std::tie(id,str)=opt(),id!=Copt::ID::END)
	{
		std::cout << id << " = " << str << std::endl;
	}
	
	return 0;
}