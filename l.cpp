#include <iostream>

class a
{
	public :
		static	int	d;
		a(void) {
			//d = 10;
		}
		~a(void) {}
};

int	a::d = 10;

int	main()
{
	a b;
	a g;
	std::cout << b.d << std::endl;
	b.d = 33;
	std::cout << b.d << std::endl;
	std::cout << g.d << std::endl;
	
}
