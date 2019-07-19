#ifndef DITO_H
#define DITO_H

	extern typedef struct Dito{
		char char_associato = '';
		size_t premuto = 0; // 0 no premuto 1 premuto
	};

	Dito* newDito(char charAss);
	void modCharAssDito(Dito* dito, char newChar);
	void premi(Dito* dito, xdo_t * x);
	void rilascia(Dito* dito, xdo_t * x);
	
#endif

#ifndef CONTROLLER_H
#define CONTROLLER_H

	extern typedef struct Controller{
		Dito mignolo = NULL;
		Dito anulare = NULL;
		Dito medio   = NULL;
		Dito indice  = NULL;
		Dito pollice = NULL;
	};

	Controller* newInitDefaultColtroller();
	Controller* newInitPersonalizedController(char mignolo,char anulare,char medio,char indice,char pollice);
	void printControllerSetting(Controller* cnt);
	
#endif
