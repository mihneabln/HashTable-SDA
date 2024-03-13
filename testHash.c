/* BALAN Mihnea Andrei - 312CB */

#include <stdio.h>
#include "tlg.h"
#include "thash.h"
#include <string.h>

typedef struct {
	char word[50];
	int frecv;
} TCuvant;

typedef struct { 
	int lung;
	TLG l;
} TElement;

int Ins_Ord(TLG* aL, void* ae, TFCmp fcmp);
int Ins_LG(TLG* aL, void* ae);
TH * GenerareHash(TLG listaCuvinte);
void AfiTH_caz1(TH* ah,TF afi_elem);
void AfiTH_caz2(TH* ah,TF afi_elem, char c, char n);
void AfiTH_caz3(TH* ah,TF afi_elem, char n);

int codHash(void * element){
	TCuvant * cuvant = (TCuvant *) element;
	char * word = cuvant->word;
	if ( (*word) <= 'Z' )
		return *word - 'A';
	return *word - 'a';
}

void afisareCuvant(void * element){
	TCuvant * cuvant = (TCuvant *) element;
	printf("%s/%d", cuvant->word, cuvant->frecv);
}

char * filtrare (char *p){
	int i;
    for (i = 0; i <= strlen(p); i++){
		while(p[i] == '.' || p[i] >= '0' && p[i] <= '9' || p[i] == ','){
			strcpy (p+i, p+i+1);
		}
	}
	if (p[strlen(p) - 1] == '\n'){
		p[strlen(p) - 1] = '\0';
	}
	return p;
}

int lung (void * e1, void * e2){
	TCuvant * cuvant1 = (TCuvant *) e1;
	TCuvant * cuvant2 = (TCuvant *) e2;

	if (strlen(cuvant1->word) < strlen(cuvant2->word)){
		return 0;
	}	
	return 1;
}

int cmpCuvinte(void * e1, void * e2){
	TCuvant * cuvant1 = (TCuvant *) e1;
	TCuvant * cuvant2 = (TCuvant *) e2;

	if (strcmp(cuvant1->word, cuvant2->word) != 0)
		return 0;
	return 1;
}

int cmpLungime(void * e1, void * e2){
	TCuvant *cuvant1 = (TCuvant *) e1;
	TCuvant *cuvant2 = (TCuvant *) e2;

	if (strlen(cuvant1->word) != strlen(cuvant2->word))
		return 0;
	return 1;
}

void counting (TLG l){
	int c;
    TLG i;
	for (i = l; i != NULL; i = i->urm){
		c = 1;
		TLG ant = i;
        TLG j;
		for(j = i->urm; j !=NULL; j = j->urm){
			if (cmpCuvinte(i->info, j->info)){
				c++;
				ant->urm = j->urm;
            }
			else
				ant = j;
		}
		if ( ((TCuvant*)(i->info))->frecv == 0)
			 ((TCuvant*)(i->info))->frecv = c;
		else 
			((TCuvant*)(i->info))->frecv = ((TCuvant*)(i->info))->frecv + c-1;
	}
}

TLG citesteListaCuvinte(char ** argv) {
	FILE *f;
	char * line = NULL;
	size_t len = 0;
	TLG L = NULL;
	f = fopen(argv[1], "rt");
	if (f == NULL)
		return NULL;

	while (getline(&line, &len, f) != -1) {
		char *p;
		p = strtok (line, " ");
        p = filtrare(p);

		if (strcmp(p, "insert") == 0){                                     // insert
			p = strtok (NULL, " \n");

			while (p){        // insereaza in lista cuvantul p
				p = filtrare(p);					
				if (strlen(p) >= 3){
					TCuvant * cuvant = malloc(sizeof(TCuvant));
					if (cuvant == NULL)
						return L;
					strcpy(cuvant->word, p);
					Ins_Ord (&L, (void *) cuvant, lung);
					}
				p = strtok (NULL, " ");
			}
			counting(L);
		}
        
		else if (strcmp(p, "print") == 0 || strcmp(line, "print") == 0){    // print
            TH * h = NULL;
            h = GenerareHash(L);
            if (h == NULL) {
                printf("Tabela hash nu a putut fi generata\n");
                return NULL;
            }
            p = filtrare(p);
            p = strtok (NULL, " ");

            if (p == NULL){                                             //printf("caz 1 de printare\n");                
                printf("caz 1\n");
                AfiTH_caz1(h, afisareCuvant);
            }
            else

            if (p[0] >= 'a' && p[0] <= 'z' || p[0] >= 'A' && p[0] <= 'Z'){  //printf("caz 2 de printare\n");
                printf("caz 2\n");
                char c = p[0];
                p = strtok(NULL, " ");
                AfiTH_caz2(h, afisareCuvant, c, p[0]);
            }

            else{                                                       //printf("caz 3 de printare\n");
                printf("caz 3\n");
                AfiTH_caz3(h, afisareCuvant, p[0]);
            }
		}
    }
	fclose(f);
	return L;
}

void swap(TLG a, TLG b){
	void* aux = a->info;
    a->info = b->info;
    b->info = aux;
}

void bubbleSort(TLG celula){
    int inv = 1, i;
    TLG curent;

    /* Checking for empty list */
    if (celula == NULL){       // start=celula, swapped=inv, prt1=curent, 
        printf("eroare bubble\n");
		return;               // next=urm
	}
    while(inv){
        inv = 0;
        curent = celula;
    
        while (curent->urm != NULL){
			TLG ant;
            
        	if ((((TCuvant*)(curent->info))->frecv < ((TCuvant*)(curent->urm->info))->frecv) || 
				((((TCuvant*)(curent->info))->frecv == ((TCuvant*)(curent->urm->info))->frecv) && 
				strcmp(((TCuvant*)(curent->info))->word, ((TCuvant*)(curent->urm->info))->word) > 0))
			{
                swap(curent, curent->urm);         
                inv = 1;
            }
            curent = curent->urm;
        }
    }
}

TH * GenerareHash(TLG listaCuvinte){
    TH *h = NULL;
	TLG p;
    TCuvant * cuvant;
	int rez, i;

	/* calcul dimensiuni maxime pt tabela hash */
	size_t M = ('Z'-'A')+1;

	/* initializare tabela hash */
	h = (TH *) InitTH(M, codHash);
	if(h == NULL)
		return NULL;

	for(p = listaCuvinte; p != NULL; p = p->urm) {
        cuvant = (TCuvant *) malloc(sizeof(TCuvant));
        if(cuvant == NULL)
			return h;
        memcpy(cuvant, p->info, sizeof(TCuvant));
		rez = InsTH(h, cuvant, cmpLungime);
        if(!rez) {
			free(cuvant);
			return h;
		}
	}
	for (i=0; i<M; i++){
		for (p = h->v[i]; p != NULL; p = p->urm){
			bubbleSort(((TElement*)(p->info))->l);
		}
	}
	return h;
}

TH* InitTH(size_t M, TFHash fh){
    TH* h = (TH *) calloc(sizeof(TH), 1);
    if (!h) {
        printf("eroare alocare hash\n");
        return NULL;
    }

    h->v = (TLG *) calloc(M, sizeof(TLG));
    int i;
	for (i=0; i<M; i++){
		h->v[i] = NULL;
	}
    if(!h->v) {
        printf("eroare alocare vector de pointeri TLG in hash\n");
        free(h);
        return NULL;
    }

    h->M = M;
    h->fh = fh;
    return h;
}

void DistrTH(TH** ah, TF elib_elem){
    TLG * p, el, aux;

    /* parcurgere cu pointeri */
    for (p = (*ah)->v; p < (*ah)->v + (*ah)->M; p++) {
        /* daca exista elemente corespunzatoare acestui hash
         * eliberam info din celula si apoi eliberam celula */
        for(el = *p; el != NULL; ) {
            aux = el;
            el = el->urm;
            elib_elem(aux->info);
            free(aux);
        }
    }
    free((*ah)->v);
    free(*ah);
    *ah = NULL;
}

void AfiTH_caz1(TH* ah,TF afi_elem){
    TLG p, el;
    int i;
    for(i = 0; i < ah->M; i++) {
        
        if(ah->v[i]){
            printf("pos %d: ", i);
            
            for (p = ah->v[i]; p != NULL; p = p->urm){               
                    int inceput = 0;
                    printf("(%d:", ((TElement*)(p->info))->lung);
                    el = ((TElement*)(p->info))->l;
                    if (el == NULL)     
                        printf("=(\n");

                    for(el = ((TElement*)(p->info))->l; el != NULL; el = el->urm){
                        if (inceput){
                            printf(", ");
                        }
                        afi_elem(el->info);
                        inceput = 1;
                    }
                printf(")");
            }
            printf("\n");
        }
    }
}

void AfiTH_caz2(TH* ah,TF afi_elem, char c, char n){
    TLG p, el;
    int i, ok = 0;

    if ( c <= 'Z' )
		i = c - 'A';
	else 
        i = c - 'a';

    for (p = ah->v[i]; p != NULL; p = p->urm)
        if (((TElement*)(p->info))->lung == n-'0')
            ok = 1;

    if (ok){
        for (p = ah->v[i]; p != NULL; p = p->urm){  
            if ( ((TElement*)(p->info))->lung == n-'0'){
                int inceput = 0;
                printf("(%d:", ((TElement*)(p->info))->lung);
                el = ((TElement*)(p->info))->l;
                if (el == NULL)     
                    printf("=(\n");
                for(el = ((TElement*)(p->info))->l; el != NULL; el = el->urm){
                    if (inceput){
                        printf(", ");
                    }
                    afi_elem(el->info);
                    inceput = 1;
                }
                printf(")");
            }
        }
        printf("\n");
    }
}

void AfiTH_caz3(TH* ah,TF afi_elem, char n){
    TLG p, q, el;
    int i, j, ok1, ok2;
    for(i = 0; i < ah->M; i++) {
        ok1 = 0;
        if(ah->v[i]){
            ok1 = 0;

            for (p = ah->v[i]; p != NULL; p = p->urm)
                for (q = ((TElement*)(p->info))->l; q != NULL; q = q->urm)
                    if ( ((TCuvant*)(q->info))->frecv <= n-'0'){
                        ok1 = 1;
                        break;
                    }

            if (ok1){
                printf("pos%d: ", i);   
                for (p = ah->v[i]; p != NULL; p = p->urm){               
                    ok2 = 0;         
                    for (q = ((TElement*)(p->info))->l; q != NULL; q = q->urm){
                        if ( ((TCuvant*)(q->info))->frecv <= n-'0'){
                            ok2 = 1;
                            break;
                        }
                    }

                    if (ok2){
                        int inceput = 0;
                        printf("(%d: ", ((TElement*)(p->info))->lung);
                        el = ((TElement*)(p->info))->l;
                        if (el == NULL)     
                            printf("=(\n");
                        for(el = ((TElement*)(p->info))->l; el != NULL; el = el->urm){
                            if (inceput){
                                printf(", ");
                            }
                            if ( ((TCuvant*)(el->info))->frecv <= n-'0'){
                                afi_elem(el->info);
                                inceput = 1;
                            }
                        }
                        printf(")");
                    }
                }
                printf("\n");
            }
        }
    }
}

/* daca elementul exista functia intoarce 0
 *altfel se incearca inserarea elementului si se intoarce rezultatul inserarii */
int InsTH(TH*a, void* ae, TFCmp fcmp){
    int cod = a->fh(ae), rez, ok=1, c=0;
    TLG el;
    for(el = a->v[cod]; el != NULL; el = el->urm) {
        if (((TElement*)(el->info))->lung == strlen((char*)ae)){
            ok = 0;
            break;
        }
        c++;
    }
    if (ok==1){
        TElement* ins=malloc(sizeof(TElement));
        ins->l = NULL;
        ins->lung = strlen((char*)ae);
		TCuvant* add = malloc(sizeof(TCuvant));
		add->frecv = ((TCuvant*)ae)->frecv;
		strcpy(add->word,(char*)ae);

        Ins_LG(&(a->v[cod]), (void*) ins);
		rez = Ins_LG(&(((TElement*)(a->v[cod]->info))->l), (void*) add);

    }
    else{
        TLG* verticala = &((TElement*)(a->v[cod]->info))->l;
        TCuvant* add = malloc(sizeof(TCuvant));
        add->frecv = ((TCuvant*)ae)->frecv;
        strcpy(add->word,(char*)ae);
        rez =  Ins_LG(verticala, (void*) add);
    }
    return rez;
}

int main(int argc, char **argv)
{   
	TLG listaCuvinte = citesteListaCuvinte(argv);
	if (listaCuvinte == NULL) {
		printf("Lista nu a putut fi generata\n");
        return 0;
	}

  	// DistrugeLG(&listaCuvinte, free);
	// DistrTH(&h, free);
  	return 0;
}
