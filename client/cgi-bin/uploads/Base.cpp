#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"

Base::~Base() 
{
}

Base * generate(void)
{
    int	nb;

	srand(time(0)); 
  	nb = (rand() % 3);

    switch (nb) 
    {
        case 0:
            return new A();
        case 1:
            return new B();
        case 2:
            return new C();
        default:
            return NULL;
    }
}

void identify(Base *p)
{
	std::cout << "\n  Identify Pointer" << std::endl;
    if (!p) 
    {
        std::cout << "  NULL pointer " << std::endl;
        return;
    }

    if (dynamic_cast<A*>(p))
        std::cout << "  A" << std::endl;
    else if (dynamic_cast<B*>(p))
        std::cout << "  B" << std::endl;
    else if (dynamic_cast<C*>(p))
        std::cout << "  C" << std::endl;
    else
        std::cout << "  Unknown pointer" << std::endl;
}

void identify(Base &p)
{

	std::cout << "\n  Identify Reference" << std::endl;
    try
	{
		(void)dynamic_cast<A&>(p);
		std::cout << "  A&" << std::endl;
		return ;
	}
	catch(const std::exception& e)
	{
		
	}
	
	try
	{
		(void)dynamic_cast<B&>(p);
		std::cout << "  B&" << std::endl;
		return ;
	}
	catch(const std::exception& e)
	{
		
	}
	try
	{
		(void)dynamic_cast<C&>(p);
		std::cout << "  C&" << std::endl;
		return ;
	}
	catch(const std::exception& e)
	{
		std::cout << "  Unknown Type" << std::endl;
	}
}
