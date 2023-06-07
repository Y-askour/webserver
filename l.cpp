#include <iostream>

class b
{
	std::string	a;
	public :
		void	ok() { std::cout << "ok of b" << std::endl;}
		b(){ std::cout << "constructer b" << std::endl;}
		~b(){ std::cout << "destructer b" << std::endl;}
};

class a : public b
{
	std::string	g;
	public :
		void	ok() { std::cout << "ok of a" << std::endl;}
		void	l() {std::cout << "l" << std::endl;}
		a(){ std::cout << "constructer a" << std::endl;}
		~a(){ std::cout << "destructer a" << std::endl;}
};

int	main()
{
	b *d = new a();

	d->ok();
	d->l();
	//a d;
	//b g;
	//g.ok();
	//d.b::ok();
}
