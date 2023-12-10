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
		Wait(10);
		Wait(15);
		Wait(30);
		Wait(15);
		Wait(30);
		Release(susenky_linka);
		pocet_varek++;
	}
};

class krem : public Process
{
	void Behavior()
	{
		Seize(krem_linka);
		Wait(10);
		Wait(20);
		Wait(20);
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
				Wait(15);
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
				Wait(15);
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
		if (zabaliZamestnanci > 0)
		{
			zabaliZamestnanci--;
			double tmptime = Uniform(10, 15);
			Wait(tmptime);
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
				Wait(Uniform(30, 60));
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

int main()
{
	pocetZamestnancu = 4;
	Init(0, 1500);
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
	cout << "Zabalili zamestnanci: " << paletZamestnanci << endl;
	cout << "Varek: " << pocet_varek << endl;
	cout << "Kremu: " << pocet_kremu << endl;
	cout << "Sestaveno: " << sestavene << endl;
	cout << "Kontrola: " << kontrola << endl;
	cout << "Zabaleno: " << zabaleno << endl;
	cout << "Palet: " << palet + paletZamestnanci << endl;
	cout << "Zabali zamestnanci: " << zabaliZamestnanci << endl;
	return 0;
}
