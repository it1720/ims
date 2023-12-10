#include <iostream>
#include "simlib.h"
using namespace std;


int pocet_varek = 0;
Facility susenky_linka("Linka na susenky");

class susenky : public Process
{
	void Behavior()
	{
		Seize(susenky_linka);
		cout << "Priprava surovin testa" << endl;
		Wait(10);
		cout << "Michani testa" << endl;
		Wait(15);
		cout << "Rozvalovani testa" << endl;
		Wait(30);
		cout << "Peceni" << endl;
		Wait(15);
		cout << "Chlazeni" << endl;
		Wait(30);
		Release(susenky_linka);
		pocet_varek++;
		cout << "Varka c." << pocet_varek << " je hotova" << endl;
		(new susenky)->Activate();	
	}
};


int main()
{
	Init(0, 500);
	(new susenky)->Activate();
	Run();
	return 0;
}
