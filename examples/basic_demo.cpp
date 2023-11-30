#include <iostream>

#include "ring_p2.h"


int main(int argc, char** argv)
{
    jaxos::data_struct::ring<int,4> r16;

    std::cout << std::boolalpha;
    int i = 0;

    std::cout << "---- Inspecting.\n"
        << "\tCapacity: " << r16.capacity()
        << "\n\tCount: " << r16.count()
        << "\n\tFull: " << r16.full()
        << "\n\tEmpty: " << r16.empty()
        << "\n";

    std::cout << "\n---- Adding elements to the queue til it is full.\n";
    for (; !r16.full(); ++i)
    {
        std::cout << i << ") pushing... "
            << r16.push_front(i)
            << "; peek: " << r16.peek(r16.count()-1)
            << "; count: " << r16.count()
            << ".\n";
    }

    std::cout << "------ Ensuring another addition is not possible.\n";
    std::cout << i << ") pushing... "
        << r16.push_front(i) << ".\n";

    std::cout << "\n---- Consuming until queue is empty.\n";
    for (++i; !r16.empty(); ++i)
    {
        std::cout << i << ") popping... "
            << "; peek: " << r16.peek()
            << "; " << r16.pop_back()
            << "; count: " << r16.count()
            << ".\n";
    }
    
    std::cout << "------ Ensuring no more items can be retrieved.\n";
    try
    {
        std::cout << "?) popping... " << r16.pop_back();
        std::cout << "; count: " << r16.count();
        std::cout << "; peek: " << r16.peek();
    }
    catch(const char *e)
    {
        std::cout << "\nException: \"" << e << "\".\n";
    }

    std::cout << "\n\n---- Ensure we can add again.\n";
    for (; !r16.full(); ++i)
    {
        std::cout << i << ") pushing... "
            << r16.push_front(i)
            << "; peek: " << r16.peek(r16.count()-1)
            << "; count: " << r16.count()
            << ".\n";
    }

    std::cout << "\n\n---- Testing negative offset.\n";
    
    std::cout << "r16.count(): " << r16.count() << "\n";

    try
    {
        for (i = 3; i >= -3; --i)
        {
            auto &var = r16.peek(i);

            std::cout << i << ") peeked :" << var
                << "\n";

            //ASSERT_EQ(666+i, this->ring.peek(i));
        }
        
    }
    catch (const char *msg)
    {
        std::cout << "Exception caught: '"
            << msg << ".\n";
    }

    std::cout << "\n\n---- How about changing items in the queue?\n";
    std::cout << "-> Original item: " << r16.peek(r16.count()-1)
        << "; count: " << r16.count()
        << ".\n";
    r16.peek(r16.count()-1) = 42;
    std::cout << "-> Modified item: " << r16.peek(r16.count()-1)
        << "; count: " << r16.count()
        << ".\n";
}
