#include "main.hpp"

int main()
{
#ifdef CLASSCREATE
    shared_ptr<classCreate> sharedPtr = classCreate::classInit();
#endif
}