/*
 * documentazione funzioni xdo utili per il progetto 
 * 
 * OS-prj1819
 * installare xdo
 *
 * apt-get install libxdo-dev
 *
 * compilare
 * 
 * gcc test1.c -lxdo -o test1-xdo
 * 
 */


#include <stdio.h>
#include <xdo.h>

int main(int argc, char **argv)
{

/*  devo creare un istanza del main context  di xdo
*   per poter usare le funzioni (ogni funzione richiede come
*   							 primo parametro il contesto)
*	struct xdo {
*		Display* xdpy;
*		char* display_name;
*		charcodemap_t* charcodes;
*		int charcodes_len;
*		XModifierKeymap* modmap;
*		KeySym* keymap;
*		int keycode_high;
*		int keycode_low;
*		int keysyms_per_keycode;
*		int close_display_when_freed;
*		int quiet;
*		int debug_;
*		int features_mask;
*	}
* 
*/

	xdo_t * X = xdo_new(NULL);

/*
*
* xdo_send_keysequence_window_down
*
* come dal nome genera un evento key_down per il carattere selezionato
* solo un carattere alla volta
* 
* int xdo_send_keysequence_window_down (
*	const xdo_t*	xdo
* 	Window	window,
* 	const char*	keysequence,
* 	useconds_t	delay
* )
* 
*/

xdo_send_keysequence_window_down(x, CURRENTWINDOW, "w", 200000);

/*
*
* xdo_send_keysequence_window_up
*
* come dal nome genera un evento key_up per il carattere selezionato
* solo un carattere alla volta
* 
* int xdo_send_keysequence_window_up (
*	const xdo_t*	xdo
* 	Window	window,
* 	const char*	keysequence,
* 	useconds_t	delay
* )
* 
*/

xdo_send_keysequence_window_down(x, CURRENTWINDOW, "w", 200000);

/*
 *
 * prima di chiudere il programma liberare la memoria dedicata a xdo
 * 
 */

     xdo_free(x);

	
	return 0;
}

