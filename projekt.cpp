#include <iostream>
#include "simlib.h"
using namespace std;

int dostaljsemse = 0;
int pocetZamestnancu = 0;
int pocet_varek = 0;
int pocet_kremu = 0;
int sestavene = 0;
int kontrola = 0;
int zabaleno = 0;
int palet = 0;
int zabaliZamestnanci = 0;
int krabicNaPalete = 0;
bool funguje = true;
bool jeVPoruse = false;
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
		// cout << "Varek: " << pocet_varek << endl;
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
		// cout << "Kremu: " << pocet_kremu << endl;
	}
};

class kontrolaKvality : public Process
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
				// cout << "Kontrola kvality" << endl;
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
				// cout << "Sestaveni susenky" << endl;
				Wait(15);
				pocet_varek--;
				pocet_kremu--;
				sestavene++;
				// cout << "Sestaveno: " << sestavene << endl;
			}
			else
			{
				Wait(1);
			}
		}
	}
};

class baleniDoKrabice : public Process
{
	void Behavior()
	{
		while (true)
		{
			if (jeVPoruse && zabaleno > 0)
			{
				zabaleno--;
				zabaliZamestnanci += 100;
			}
			else if (zabaleno > 0)
			{
				Wait(30);
				zabaleno--;
				palet++;
			}
			else
			{
				Wait(1);
			}
		}
	}
};

class zamestnanciBali : public Process
{
	void Behavior()
	{
		double tmptime = Uniform(7.5, 12.5);
		Wait(tmptime);
		zabaliZamestnanci--;
		krabicNaPalete++;
		pocetZamestnancu++;
	}
};

class zamestnanciLinka : public Process
{
	void Behavior()
	{
		while (true)
		{
			if (zabaliZamestnanci > 0 && pocetZamestnancu > 0)
			{
				// cout<<"Zabali zamestnanci: "<<zabaliZamestnanci<<endl;
				(new zamestnanciBali)->Activate();
				pocetZamestnancu--;
			}
			else
			{
				Wait(1);
			}
		}
	}
};

class oprava : public Process
{
	void Behavior()
	{
		Wait(Uniform(20, 40));
		jeVPoruse = false;
	}
};

class porucha : public Process
{
	void Behavior()
	{
		while (true)
		{
			if (!jeVPoruse)
			{
				Wait(Uniform(40, 80));
				(new oprava)->Activate();
				// cout << "Porucha" << endl;
				jeVPoruse = true;
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
	pocetZamestnancu = 500;
	Init(0, 500);
	(new susenky)->Activate();
	(new krem)->Activate();
	(new sestaveni)->Activate();
	(new kontrolaKvality)->Activate();
	(new baleni)->Activate();
	(new baleniDoKrabice)->Activate();
	(new zamestnanciLinka)->Activate();
	(new porucha)->Activate();
	Run();
	cout << "Zabalil stroj: " << palet << endl;
	palet += krabicNaPalete/100;
	cout << "Zabalil zamestnanci: " << palet << endl;
	cout << "Varek: " << pocet_varek << endl;
	cout << "Kremu: " << pocet_kremu << endl;
	cout << "Sestaveno: " << sestavene << endl;
	cout << "Kontrola: " << kontrola << endl;
	cout << "Zabaleno: " << zabaleno << endl;
	cout << "Palet: " << palet << endl;
	cout << "Krabic na palete: " << krabicNaPalete << endl;
	cout << dostaljsemse << endl;
	return 0;
}
