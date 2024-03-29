/* BALAN Mihnea Andrei - 312CB */

#include "tlg.h"

int Ins_LG(TLG* aL, void* ae)
{
	TLG p, ant, aux = malloc(sizeof(TCelulaG));
	if(!aux)
	    return 0;
	aux->info = ae;
	aux->urm = *aL;
	*aL = aux;
	return 1;
}

int Ins_Ord(TLG* aL, void* ae, TFCmp fcmp)
{
	TLG p, ant, aux;
	for ( ant = NULL, p = *aL; p && fcmp(p->info, ae); ant = p, p = p->urm);
	aux = malloc(sizeof(TCelulaG));
	if(!aux)
	    return 0;
	if (ant != NULL){
		aux->urm = p;
		ant->urm = aux;
	}
	else{
		aux->urm = p;
		*aL = aux;
	}
	aux->info = ae;
	return 1;
}

void DistrugeLG(TLG* aL, TF free_elem) /* distruge lista */
{
	while(*aL != NULL)
    {
        TLG aux = *aL;     /* adresa celulei eliminate */
        if (!aux)
            return;

        free_elem(aux->info);  /* elib.spatiul ocupat de element*/
        *aL = aux->urm;    /* deconecteaza celula din lista */
        free(aux);   /* elibereaza spatiul ocupat de celula */
    }
}

size_t LungimeLG(TLG* a)      /* numarul de elemente din lista */
{
	size_t lg = 0;
	TLG p = *a;

     /* parcurge lista, numarand celulele */
	for (; p != NULL; p = p->urm)
        lg++;

	return lg;
}

void Afisare(TLG* aL, TF afiEL)
{
	if(!*aL) {
        printf("Lista vida\n");
        return;
    }
	printf("[\n");
	for(; *aL; aL = &(*aL)->urm) {
		afiEL((*aL)->info);
    }
	printf("]\n");
}