#include <iostream>

std::string ft_to_upper(char *str)
{
	std::string res;

	for (int i = 0; str[i]; i++)
		res += toupper(str[i]);
	return res;
}

int main(int argc, char **argv)
{
	std::string str;

	if (argc == 1)
	{
		std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *" << std::endl;
		return 0;
	}
    else
    {
	    for (int i = 1; i < argc; i++)
		{
		    str += ft_to_upper(argv[i]);

		}
	    std::cout << str << std::endl;
    }
	return (0);
}
