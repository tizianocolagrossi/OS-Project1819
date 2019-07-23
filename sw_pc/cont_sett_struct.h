#include <xdo.h>
#ifndef DITO_H
#define DITO_H

	typedef struct Dito{
		char char_associato;
		size_t rilevamentoFisico; // 0 no premuto 1 premuto
		size_t premuto; // 0 no premuto 1 premuto
	}Dito;

	Dito* newDito(char charAss);
	void modCharAssDito(Dito* dito, char newChar);
	void premi(Dito* dito, xdo_t * x);
	void rilascia(Dito* dito, xdo_t * x);
	
#endif

#ifndef CONTROLLER_H
#define CONTROLLER_H

	typedef struct Controller{
		Dito *mignolo;
		Dito *anulare;
		Dito *medio;
		Dito *indice;
		Dito *pollice;
	}Controller;

	
	Controller* newController();
	Controller* newPersController(char mignolo,char anulare,char medio,char indice,char pollice);
	void printControllerSetting(Controller* cnt);
	
#endif
