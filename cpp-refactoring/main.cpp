// install: jsoncpp http://macappstore.org/jsoncpp/
// to run: g++ -std=c++11  -L /usr/local/lib -ljsoncpp tamash.cpp -o tamash.o && ./tamash.o

#include "Theater.h"

#include <chrono>

#define CLOCK_NOW std::chrono::high_resolution_clock::now

int main()
{
    auto start = CLOCK_NOW();
    Theater::LoadPlays();
    
    Theater::LoadPrices();
    Theater::LoadInvoices();

    auto stop = CLOCK_NOW();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "\nExecution time in microseconds: " << duration.count() << "\n";

    return 0;
}