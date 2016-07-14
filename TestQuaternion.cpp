#include <iostream>
#include <string> 

#include "Quaternion.hpp"

bool passed = true;

void checkResult(Quaternion* result, Quaternion* answer, std::string message)
{
    if(result->equals(answer) == false)
    {
        passed = false;

        std::cout << "Failed test: " << message << std::endl;
        std::cout << "Expected: "; answer->print();
        std::cout << "Got     : "; result->print();
    }
}

int main(int argc, char** argv)
{
    Quaternion* testOneA    = new Quaternion(  -1.0f   ,   2.0f   ,   3.0f   ,   4.0f   );
    Quaternion* testOneB    = new Quaternion(   1.0f   ,  -2.0f   ,   3.0f   ,   4.0f   );
    Quaternion* answerOne   = new Quaternion(  12.0f   ,   6.0f   ,  24.0f   ,  12.0f   );
    testOneA->multiply(testOneB);
    checkResult(testOneA, answerOne, "1st test.");

    Quaternion* testTwoA    = new Quaternion(   3.0f   ,   5.0f   ,   1.5f   ,  -4.0f   );
    Quaternion* testTwoB    = new Quaternion(  -1.0f   ,  -2.0f   ,  -3.5f   ,   3.0f   );
    Quaternion* answerTwo   = new Quaternion(  -1.5f   ,  32.0f   ,  17.5f   ,   6.25f  );
    testTwoA->multiply(testTwoB);
    checkResult(testTwoA, answerTwo, "2nd test.");

    Quaternion* testThreeA  = new Quaternion(   2.2f   ,  -1.0f   ,   1.1f   ,  -3.4f   );
    Quaternion* testThreeB  = new Quaternion( -10.0f   ,   7.0f   ,  -3.8f   ,   8.1f   );
    Quaternion* answerThree = new Quaternion(  47.92f  , -34.54f  ,  27.23f  ,   5.64f  );
    testThreeA->multiply(testThreeB);
    checkResult(testThreeA, answerThree, "3rd test.");

    if(passed == true)
    {
        std::cout << "All tests passed." << std::endl;
    }
}

