#include <iostream>

class b
{
	std::string	a;
	public :
		virtual void	ok() { std::cout << "ok of b" << std::endl;}
		 void	l() {std::cout << "dd" << std::endl;}
		b(){ std::cout << "constructer b" << std::endl;}
		virtual ~b(){ std::cout << "destructer b" << std::endl;}
};

class a : public b
{
	std::string	g;
	public :
		void	ok() { std::cout << "ok of a" << std::endl;}
		//void	l() {std::cout << "l" << std::endl;}
		a(){ std::cout << "constructer a" << std::endl;}
		~a(){ std::cout << "destructer a" << std::endl;}
};

int	main()
{
	a *d = new a();

	//a d;

	d->l();
	d->ok();

	//d->ok();
	//d->l();

	//a d;
	//b g;
	//g.ok();
	//d.b::ok();
}
