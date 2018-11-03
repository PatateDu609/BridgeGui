#include "Game.h"

void game::showContract(game::Contract const& c) {

}

cards::Card game::compare(int color, int contract, cards::Card const& a, cards::Card const& b) {
	cards::Card c;
	int x = a[0] - b[0], y = a[1] - b[1];
	if (x == 0) {
		c = (y > 0) ? a : b;
	}
	else {
		bool ac = a[0] == color,
			bc = b[0] == color,
			acontract = a[0] == contract,
			bcontract = b[0] == contract;

		if (contract != color) {
			if (!ac && !bc && !acontract && !bcontract) c = { -10, -10 };
			else if (ac && !bc && !bcontract) c = a; // cas o� le joueur A pose une carte de la couleur du pli et le joueur B se d�fausse d'une carte
			else if (ac && bcontract) c = b; // cas o� le joueur A pose une carte de la couleur du pli mais que le joueur B coupe avec une carte d'atout
			else if (acontract && bcontract) c = (y > 0) ? a : b; // cas o� les deux joueurs coupent avec une carte d'atout, on compare la puissance des cartes
		}
		else if (contract == color || contract == 5) {
			if (!ac && !bc) c = { -10, -10 };
			else if (ac && !bc) c = a; // cas o� B se d�fausse mais que A a jou� dans la couleur du pli (qui peut aussi �tre celle du contrat si jeu avec atout)
			else if (ac && bc) c = (y > 0) ? a : b; // cas o� A et B jouent dans la couleur du pli
		}
	}

	return c;
}