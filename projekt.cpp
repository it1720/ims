#include <iostream>
#include "simlib.h"
using namespace std;

int pocetZamestnancu = 0;
int pocet_varek = 0;
int pocet_kremu = 0;
int sestavene = 0;
int kontrola = 0;
int zabaleno = 0;
int palet = 0;
int zabaliZamestnanci = 0;
int krabicNaPalete = 0;
int paletZamestnanci = 0;
bool jeVPoruse = false;
Facility susenky_linka("Linka na susenky");
Facility krem_linka("Linka na krem");

class susenky : public Process
{
	void Behavior()
	{
		Seize(susenky_linka);
		Wait(10); // priprava surovin
		Wait(15); // kombinovani surovin
		Wait(30); // formovani testa
		Wait(15); // peceni
		Wait(30); // chlazeni
		Release(susenky_linka);
		pocet_varek++;
	}
};

class krem : public Process
{
	void Behavior()
	{
		Seize(krem_linka);
		Wait(10); // priprava surovin
		Wait(20); // kombinovani surovin
		Wait(20); // chlazeni
		Release(krem_linka);
		pocet_kremu++;
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
				Wait(15); // kontrola kvality
				sestavene--;
				kontrola++;
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
				Wait(25); //baleni do obalu
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
				Wait(15); // spojeni kremu a susenek
				pocet_varek--;
				pocet_kremu--;
				sestavene++;
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
				Wait(30); //baleni do krabice strojem
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
		if (zabaliZamestnanci > 0)
		{
			zabaliZamestnanci--;
			double tmptime = Uniform(10, 15);
			Wait(tmptime); //baleni do krabice zamestnancem
			krabicNaPalete++;
			if (krabicNaPalete>100)
			{
				krabicNaPalete -= 100;
				paletZamestnanci++;
			}
			
		}
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
				pocetZamestnancu--;
				(new zamestnanciBali)->Activate();
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
		Wait(Uniform(20, 40)); // doba opravy
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
				Wait(Uniform(30, 60)); // doba nez se udela porucha
				(new oprava)->Activate();
				jeVPoruse = true;
			}
			else
			{
				Wait(1);
			}
		}
	}
};

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		cout << "Usage: ./projekt -t [Cas v minutach] -z [Pocet zamestnancu]" << endl;
		return 0;
	}
	
	pocetZamestnancu = atoi(argv[4]);
	Init(0, double(atoi(argv[2])));
	(new susenky)->Activate();
	(new krem)->Activate();
	(new sestaveni)->Activate();
	(new kontrolaKvality)->Activate();
	(new baleni)->Activate();
	(new baleniDoKrabice)->Activate();
	(new zamestnanciLinka)->Activate();
	(new porucha)->Activate();
	Run();
	cout << "Zabalil stroj: " << palet << " palet" <<endl;
	cout << "Zabalili zamestnanci: " << paletZamestnanci<< " palet" << endl;
	cout << "Palet: " << palet + paletZamestnanci << " palet" << endl;
	cout << "Chybi zabalit zamestnancum: " << zabaliZamestnanci << " krabic" <<endl;
	return 0;
}
