#include "cont_sett_struct.h"

#define NUM_ELEMENTS 5

int main (int argc, char** argv) {

	Controller cnt;
	Controller_init(&cnt);

	//ia init things
	printf("inizializzo il controller con %d elementi", NUM_ELEMENTS);
	for (int i = 0; i < NUM_ELEMENTS; ++i) {
		Elemento* elem = (Elemento*)malloc(sizeof(Elemento));
		if (!elem) {
			printf("ERRORE allocamento memoria\n");
			exit(EXIT_FAILURE);
		}
    
		elem->next = NULL;
		elem->value = i + 0.5f;

		ListItem* res = List_insert(&head, head.last, (ListItem*)f_item);
		if (!res) {
			printf("impossible to insert new element, exit\n");
			exit(EXIT_FAILURE);
		}
    
		printf(".");
  }
  printf("done\n");
  printf("list size: %d\n", head.size);
	
}
