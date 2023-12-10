#include <iostream>
#include "simlib.h"
using namespace std;

int pocet_varek = 0;
int pocet_kremu = 0;
int sestavene = 0;
int kontrola = 0;
int zabaleno = 0;
bool funguje = true;
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

class kontrola_kvality : public Process
{
	void Behavior()
	{
		while (true)
		{
			if (sestavene > 0)
			{
				Wait(15);
				sestavene--;
				kontrola++;
				cout << "Kontrola kvality" << endl;
			}
			else
			{
				Wait(1);
			}
		}
	}
};

class baleni : public Process
{
	void Behavior()
	{
		while (true)
		{
			if (kontrola > 0)
			{
				Wait(25);
				kontrola--;
				zabaleno++;
				cout << "Zabaleno: " << zabaleno << endl;
			}
			else
			{
				Wait(1);
			}
		}

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

class porucha : public Process
{
	void Behavior()
	{
		Exp(60);
		funguje = false;
	}
};

int main()
{
	Init(0, 550);
	(new susenky)->Activate();
	(new krem)->Activate();
	(new sestaveni)->Activate();
	(new kontrola_kvality)->Activate();
	(new baleni)->Activate();
	Run();
	return 0;
}
