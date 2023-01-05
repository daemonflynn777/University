#include <iostream>
#include "parser.h"
#include "execution.h"

Table TID (N); //Table of ids

int main()
{
    try
    {
        Parser parser("program.txt"); //executable program
        parser.analyze();
        Executor executor;
        executor.execute(parser._RPN);
    }
    catch (expected_error err)
    {
        std::cout << err._str << " - " << err._stlb << " : ";
        std::cout << err.what() << std::endl;
    }
    catch (unknown_symbol err)
    {
        std::cout << err.what() << std::endl;
    }
    catch (Lexem err)
    {
        std::cout << "Error.  ";
        std::cout << err.get_type() << std::endl;
    }
    catch (semantic_error err)
    {
        std::cout << err._str << " - " << err._stlb << " : ";
        std::cout << err.what() << std::endl;
    }
    catch (const char * err)
    {
        std::cout << err <<std::endl;
    }
    return 0;
}
