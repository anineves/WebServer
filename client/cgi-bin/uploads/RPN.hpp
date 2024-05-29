#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <fstream>
#include <string>
#include <sstream> 
#include <limits.h>
#include <stdlib.h>
#include <filesystem>
#include <exception>
#include <float.h>
#include <string>



class Rpn {
  private:

  public:
    Rpn();
    Rpn(const Rpn &source);
    Rpn& operator=(const Rpn &rhs);
    ~Rpn();
    bool isOperator (char op);
    bool validate_input (std::string input);
    void operation(char op, std::stack<float> &stack);
    float solve (std::string input);

};

#endif
