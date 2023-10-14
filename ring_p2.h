#include <iostream>
#include <climits>  // CHAR_BIT


namespace jaxos::data_struct
{
	template<typename T, size_t POW>
	class ring
	{
	public:
		using size_type = size_t;
        using index_type = size_t;

		ring();
		bool		push_front(const T & data );
		bool		pop_back();
		T&			peek(index_type offset = 0 );
		size_type	count() const noexcept;
        size_type	capacity() const noexcept {return CAPACITY;}
		bool		empty() const noexcept;
		bool		full() const noexcept;

	protected:
		size_type	input;
		size_type	output;

		/// CAPACITY is always power of 2 to ease calculations of indices.
		static constexpr size_t CAPACITY = 1 << std::min(POW, sizeof(size_type)*CHAR_BIT);

		T	data_buffer[ CAPACITY ];

		size_type RINGBUF_WRAP(index_type index) const
		{	return index & (CAPACITY - 1);	}

		T & RINGBUF_CURR_i()
		{	return data_buffer[ RINGBUF_WRAP( input ) ];	}

		T & RINGBUF_CURR_o(index_type offset)
		{
			return data_buffer[ RINGBUF_WRAP ( output + offset ) ];
		}
	};

    template<typename T, size_t POW>
    ring<T, POW>::ring()
    :	input(0), output(0)
    {
    }

    /** Inserts new element to the queue front, if possible.

    \return `true` if OK, `false` otherwise (queue full).
    */
    template<typename T, size_t POW>
    bool	ring<T, POW>::push_front (const T & data )
    {
        if ( full() )
            return false;

        RINGBUF_CURR_i() = data;

        input++;	// Don't care about clamping since "WRAP" will handle it.
        return true;
    }

    /** Removes item from back (oldest), if possible.
    
    \return `true` if OK, `false` otherwise (queue empty).
    */
    template<typename T, size_t POW>
    bool	ring<T, POW>::pop_back ()
    {
        if ( empty() )
            return false;

        output++;	// Just increment since read will be masked.
        return true;
    }

    /** Retrieve item from back, optionally adding an offset.
    
    \return	Reference to item. Throws exception if empty!
    */
    template<typename T, size_t POW>
    T&			ring<T, POW>::peek (index_type offset )
    {
//std::cout << "((output: " << output << "; offset: " << offset << "))";
        if ( count() > offset )
            return RINGBUF_CURR_o( offset );
        throw("Peeking from empty buffer!");
    }

    /** Current element count.
    */
    template<typename T, size_t POW>
    auto	ring<T, POW>::count() const noexcept -> size_type
    {
        return ( input - output );
    }

    /** Tells if queue is currently empty.
    */
    template<typename T, size_t POW>
    bool	ring<T, POW>::empty() const noexcept
    {
        return 0 == count();
    }

    /** Tells if queue is currently full.
    */
    template<typename T, size_t POW>
    bool	ring<T, POW>::full() const noexcept
    {
        return CAPACITY == count();
    }

}	// namespace jaxos::data_struct

/*
Example program:
https://godbolt.org/z/bfWcMrxGz

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

    std::cout << "\n\n---- How about changing items in the queue?\n";
    std::cout << "-> Original item: " << r16.peek(r16.count()-1)
        << "; count: " << r16.count()
        << ".\n";
    r16.peek(r16.count()-1) = 42;
    std::cout << "-> Modified item: " << r16.peek(r16.count()-1)
        << "; count: " << r16.count()
        << ".\n";
}

Output:

---- Inspecting.
	Capacity: 16
	Count: 0
	Full: false
	Empty: true

---- Adding elements to the queue til it is full.
0) pushing... true; peek: ((output: 0; offset: 0))0; count: 1.
1) pushing... true; peek: ((output: 0; offset: 1))1; count: 2.
2) pushing... true; peek: ((output: 0; offset: 2))2; count: 3.
3) pushing... true; peek: ((output: 0; offset: 3))3; count: 4.
4) pushing... true; peek: ((output: 0; offset: 4))4; count: 5.
5) pushing... true; peek: ((output: 0; offset: 5))5; count: 6.
6) pushing... true; peek: ((output: 0; offset: 6))6; count: 7.
7) pushing... true; peek: ((output: 0; offset: 7))7; count: 8.
8) pushing... true; peek: ((output: 0; offset: 8))8; count: 9.
9) pushing... true; peek: ((output: 0; offset: 9))9; count: 10.
10) pushing... true; peek: ((output: 0; offset: 10))10; count: 11.
11) pushing... true; peek: ((output: 0; offset: 11))11; count: 12.
12) pushing... true; peek: ((output: 0; offset: 12))12; count: 13.
13) pushing... true; peek: ((output: 0; offset: 13))13; count: 14.
14) pushing... true; peek: ((output: 0; offset: 14))14; count: 15.
15) pushing... true; peek: ((output: 0; offset: 15))15; count: 16.
------ Ensuring another addition is not possible.
16) pushing... false.

---- Consuming until queue is empty.
17) popping... ; peek: ((output: 0; offset: 0))0; true; count: 15.
18) popping... ; peek: ((output: 1; offset: 0))1; true; count: 14.
19) popping... ; peek: ((output: 2; offset: 0))2; true; count: 13.
20) popping... ; peek: ((output: 3; offset: 0))3; true; count: 12.
21) popping... ; peek: ((output: 4; offset: 0))4; true; count: 11.
22) popping... ; peek: ((output: 5; offset: 0))5; true; count: 10.
23) popping... ; peek: ((output: 6; offset: 0))6; true; count: 9.
24) popping... ; peek: ((output: 7; offset: 0))7; true; count: 8.
25) popping... ; peek: ((output: 8; offset: 0))8; true; count: 7.
26) popping... ; peek: ((output: 9; offset: 0))9; true; count: 6.
27) popping... ; peek: ((output: 10; offset: 0))10; true; count: 5.
28) popping... ; peek: ((output: 11; offset: 0))11; true; count: 4.
29) popping... ; peek: ((output: 12; offset: 0))12; true; count: 3.
30) popping... ; peek: ((output: 13; offset: 0))13; true; count: 2.
31) popping... ; peek: ((output: 14; offset: 0))14; true; count: 1.
32) popping... ; peek: ((output: 15; offset: 0))15; true; count: 0.
------ Ensuring no more items can be retrieved.
?) popping... false; count: 0; peek: ((output: 16; offset: 0))
Exception: "Peeking from empty buffer!".


---- Ensure we can add again.
33) pushing... true; peek: ((output: 16; offset: 0))33; count: 1.
34) pushing... true; peek: ((output: 16; offset: 1))34; count: 2.
35) pushing... true; peek: ((output: 16; offset: 2))35; count: 3.
36) pushing... true; peek: ((output: 16; offset: 3))36; count: 4.
37) pushing... true; peek: ((output: 16; offset: 4))37; count: 5.
38) pushing... true; peek: ((output: 16; offset: 5))38; count: 6.
39) pushing... true; peek: ((output: 16; offset: 6))39; count: 7.
40) pushing... true; peek: ((output: 16; offset: 7))40; count: 8.
41) pushing... true; peek: ((output: 16; offset: 8))41; count: 9.
42) pushing... true; peek: ((output: 16; offset: 9))42; count: 10.
43) pushing... true; peek: ((output: 16; offset: 10))43; count: 11.
44) pushing... true; peek: ((output: 16; offset: 11))44; count: 12.
45) pushing... true; peek: ((output: 16; offset: 12))45; count: 13.
46) pushing... true; peek: ((output: 16; offset: 13))46; count: 14.
47) pushing... true; peek: ((output: 16; offset: 14))47; count: 15.
48) pushing... true; peek: ((output: 16; offset: 15))48; count: 16.


---- How about changing items in the queue?
-> Original item: ((output: 16; offset: 15))48; count: 16.
((output: 16; offset: 15))-> Modified item: ((output: 16; offset: 15))42; count: 16.
*/