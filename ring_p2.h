#ifndef __RING_BUF_H
#	define __RING_BUF_H

#include <climits>

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
		size_type	count() const;
		bool		empty() const;
		bool		full() const;

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

    template<typename T, size_t POW>
    bool		ring<T, POW>::push_front (const T & data )
    {
        if ( full() )
            return false;

        RINGBUF_CURR_i() = data;

        input++;	// Don't care about clamping since "WRAP" will handle it.
        return true;
    }

    template<typename T, size_t POW>
    bool		ring<T, POW>::pop_back ()
    {
        if ( empty() )
            return false;

        output++;	// Just increment since read will be masked.
        return true;
    }

    template<typename T, size_t POW>
    T&			ring<T, POW>::peek (index_type offset )
    {
std::cout << "((output: " << output << "; offset: " << offset << "))";
        if ( count() > offset )
            return RINGBUF_CURR_o( offset );
        throw("Peeking from empty buffer!");
    }

    template<typename T, size_t POW>
    auto	ring<T, POW>::count() const -> size_type
    {
        return ( input - output );
    }

    template<typename T, size_t POW>
    bool		ring<T, POW>::empty() const
    {
        return 0 == count();
    }

    template<typename T, size_t POW>
    bool		ring<T, POW>::full() const
    {
        return CAPACITY == count();
    }

}	// namespace jaxos::data_struct

/*
Test program:
   https://godbolt.org/z/PsE7js4K1

int main(int argc, char** argv)
{
    jaxos::data_struct::ring<int,4> r16;

    std::cout << std::boolalpha;
    int i = 0;

    for (; !r16.full(); ++i)
    {
        std::cout << i << ") pushing... "
            << r16.push_front(i)
            << "; peek: " << r16.peek(r16.count()-1)
            << "; count: " << r16.count()
            << ".\n";
    }

    std::cout << i << ") pushing... "
        << r16.push_front(i) << ".\n";

    for (; !r16.empty(); ++i)
    {
        std::cout << i << ") popping... "
            << "; peek: " << r16.peek()
            << "; " << r16.pop_back()
            << "; count: " << r16.count()
            << ".\n";
    }
}

Output:
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
16) pushing... false.
16) popping... ; peek: ((output: 0; offset: 0))0; true; count: 15.
17) popping... ; peek: ((output: 1; offset: 0))1; true; count: 14.
18) popping... ; peek: ((output: 2; offset: 0))2; true; count: 13.
19) popping... ; peek: ((output: 3; offset: 0))3; true; count: 12.
20) popping... ; peek: ((output: 4; offset: 0))4; true; count: 11.
21) popping... ; peek: ((output: 5; offset: 0))5; true; count: 10.
22) popping... ; peek: ((output: 6; offset: 0))6; true; count: 9.
23) popping... ; peek: ((output: 7; offset: 0))7; true; count: 8.
24) popping... ; peek: ((output: 8; offset: 0))8; true; count: 7.
25) popping... ; peek: ((output: 9; offset: 0))9; true; count: 6.
26) popping... ; peek: ((output: 10; offset: 0))10; true; count: 5.
27) popping... ; peek: ((output: 11; offset: 0))11; true; count: 4.
28) popping... ; peek: ((output: 12; offset: 0))12; true; count: 3.
29) popping... ; peek: ((output: 13; offset: 0))13; true; count: 2.
30) popping... ; peek: ((output: 14; offset: 0))14; true; count: 1.
31) popping... ; peek: ((output: 15; offset: 0))15; true; count: 0.
*/

#endif	// __RING_BUF_H
