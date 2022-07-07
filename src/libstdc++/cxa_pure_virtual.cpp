#include <stdlib.h>

extern "C" void __cxa_pure_virtual ()
{
    while(1) {;} // FIXME: should return to crt0
}
