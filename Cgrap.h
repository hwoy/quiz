#include <list>
#include <string>
#include <cstring>


#ifndef _CGRAP_H
#define _CGRAP_H

class Cgrap : public std::list< std::string >
{
protected:

unsigned int skipgrap(const std::string &str,char grap,unsigned int i) const
{
	unsigned int j;
	for(j=i;str[j]==grap;j++);
	
	return j;
}

public:
enum class Cgaction {action1,action2};

Cgrap()=default;
Cgrap(const std::string &str,const std::string &grap)
{
	action(str,grap);
}

Cgrap(const std::string &str,char grap)
{
	action(str,grap);
}

void action(const std::string &str,const std::string &grap)
{
	unsigned int i,j;
	
	for(j=0,i=0;i<str.size();)
	{
		if( !str.compare(i,grap.size(),grap) )
		{
			push_back(std::string(str,j,i-j));
			
			i+=grap.size();
			j=i;
		}
		
		else { i++; }
		
	}
	
	if(i>j)
	{	
		push_back(std::string(str,j,i-j));
	}		
}

void action(const std::string &str,char grap)
{
	unsigned int i,j;
	
	for(j=0,i=0;i<str.size();)
	{
		if( str[i]==grap )
		{
			push_back(std::string(str,j,i-j));
			
			i=skipgrap(str,grap,i);
			j=i;
		}
		
		else { i++; }
		
	}
	
}

};

#endif

