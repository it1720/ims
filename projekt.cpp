#include <iostream>
#include "simlib.h"
using namespace std;

int pocet_varek = 0;
int pocet_kremu = 0;
int sestavene = 0;
Facility susenky_linka("Linka na susenky");
Facility krem_linka("Linka na krem");

class susenky : public Process
{
	void Behavior()
	{
		Seize(susenky_linka);
		// cout << "Priprava surovin testa" << endl;
		Wait(10);
		// cout << "Michani testa" << endl;
		Wait(15);
		// cout << "Rozvalovani testa" << endl;
		Wait(30);
		// cout << "Peceni" << endl;
		Wait(15);
		// cout << "Chlazeni" << endl;
		Wait(30);
		Release(susenky_linka);
		pocet_varek++;
		cout << "Varek: " << pocet_varek << endl;
	}
};

class krem : public Process
{
	void Behavior()
	{
		Seize(krem_linka);
		// cout << "Priprava surovin kremu" << endl;
		Wait(10);
		// cout << "Michani kremu" << endl;
		Wait(20);
		// cout << "Ztuhnuti kremu v lednici" << endl;
		Wait(20);
		Release(krem_linka);
		pocet_kremu++;
		cout << "Kremu: " << pocet_kremu << endl;
	}
};

class sestaveni : public Process
{
	void Behavior()
	{
		while (true)
		{
			if (pocet_varek > 0 && pocet_kremu > 0)
			{
				(new susenky)->Activate();
				(new krem)->Activate();
				cout << "Sestaveni susenky" << endl;
				Wait(15);
				pocet_varek--;
				pocet_kremu--;
				sestavene++;
				cout << "Sestaveno: " << sestavene << endl;
			}
			else
			{
				Wait(1);
			}
		}
	}
};

int main()
{
	Init(0, 550);
	(new susenky)->Activate();
	(new krem)->Activate();
	(new sestaveni)->Activate();
	Run();
	return 0;
}
