/*-- testHash.c  */
#include <stdio.h>
#include "tlg.h"
#include "thash.h"
#include <string.h>

typedef struct {
	char word[20];
	int frecv;
} TCuvant;
typedef struct { 
	int lung;
	TLG l;
} TElement;

int codHash(void * element)
{
	TCuvant * cuvant = (TCuvant *) element;
	char * word = cuvant->word;
	if ( (*word) < 'Z' )
		return *word - 'A';
	return *word - 'a';
}

void afisareCuvant(void * element)
{
	TCuvant * cuvant = (TCuvant *) element;
	printf("%s - %d\n", cuvant->word, cuvant->frecv);
	//printf("e de treaba\n");
}

char * filtrare (char *p)
{
	for (int i=0; i<=strlen(p); i++)
	{
		while(p[i] == '.') // || p[i] == '-')
		{
			strcpy (p+i, p+i+1);
		}
	}
	if (p[strlen(p) - 1] == '\n'){
		p[strlen(p) - 1] = '\0';
	}
	return p;
}

int lung (void * e1, void * e2)
{
	TCuvant * cuvant1 = (TCuvant *) e1;
	TCuvant * cuvant2 = (TCuvant *) e2;

	if (strlen(cuvant1->word) < strlen(cuvant2->word)){
		return 0;
	}	
	return 1;
}
int cmpCuvinte(void * e1, void * e2)
{
	TCuvant * cuvant1 = (TCuvant *) e1;
	TCuvant * cuvant2 = (TCuvant *) e2;

	if (strcmp(cuvant1->word, cuvant2->word) != 0)
		return 0;
//printf("cuvantul  %s  se afla de mai multe ori\n", cuvant2->word);
	return 1;
}
int cmpLungime(void * e1, void * e2)
{
	TCuvant * cuvant1 = (TCuvant *) e1;
	TCuvant * cuvant2 = (TCuvant *) e2;

	if (strlen(cuvant1->word) != strlen(cuvant2->word))
		return 0;
//printf("cuvantul  %s  are o lung care se repeta\n", cuvant2->word);
//printf("okkk\n");
	return 1;
}

void counting (TLG l)
{
	int c;
	for (TLG i = l; i != NULL; i = i->urm){
		c = 1;
		TLG ant = i;
		for(TLG j = i->urm; j !=NULL; j = j->urm){
			//Afisare(&j, afisareCuvant);
			if ( cmpCuvinte(i->info, j->info) ){
				c++;
				//printf("anterior:\n");
				//Afisare(&ant, afisareCuvant);
				
					ant->urm = j->urm;
					//ant = j;
					
				
				//free(j);
			}
			else
				ant = j;
			//printf("ok");
		}
		if ( ((TCuvant*)(i->info))->frecv == 0)
			((TCuvant*)(i->info))->frecv = c;
		else 
			((TCuvant*)(i->info))->frecv = ((TCuvant*)(i->info))->frecv + c-1;
	}
}

TLG citesteListaCuvinte(char * numeFisier) {
	FILE *f;
	char * line = NULL;
	size_t len = 0;
	TLG L = NULL;
	f = fopen(numeFisier, "rt");
	if (f == NULL)
		return NULL;

	while (getline(&line, &len, f) != -1) {
		char *p;
		p = strtok (line, " ");

		if (strcmp(p, "insert") == 0){
			p = strtok (NULL, " ");

			while (p){        // insereaza in lista cuvantul p
				p = filtrare(p);					
				if (strlen(p) >= 3){
					TCuvant *cuvant = malloc(sizeof(TCuvant));
					if (cuvant == NULL) {
						return L;
					}
					strcpy(cuvant->word, p);
					Ins_Ord (&L, (void *) cuvant, lung);		
					}
				p = strtok (NULL, " ");
			}\
			//Afisare(&L, afisareCuvant);
			counting(L);
			//Afisare(&L, afisareCuvant);
		}
		else if (strcmp(p, "print") == 0){
			//Afisare(&L, afisareCuvant);
		}
    }
	fclose(f);
	return L;
}

void swap(TLG a, TLG b)
{
	void* temp;
	memcpy(temp, a->info, sizeof(TCuvant));
	memcpy(a->info, b->info, sizeof(TCuvant));
	memcpy(b->info, temp, sizeof(TCuvant));
}
void bubbleSort(TLG celula)
{
    int inv = 1, i;
    TLG curent;
  //printf("ok1\n");
    /* Checking for empty list */
    if (celula == NULL){       // start=celula, swapped=inv, prt1=curent, 
        printf("eroare bubble\n");
		return;               // next=urm
	}
    while(inv){
		//Afisare(&celula, afisareCuvant);
        inv = 0;
        curent = celula;
  	//printf("%s\n", ((TCuvant*)(curent->info))->word);
        while (curent->urm != NULL){
			TLG ant;
        	if ( (((TCuvant*)(curent->info))->frecv <
				  ((TCuvant*)(curent->urm->info))->frecv) || 

				  ( (((TCuvant*)(curent->info))->frecv == 
					 ((TCuvant*)(curent->urm->info))->frecv) && 
				  strcmp(((TCuvant*)(curent->info))->word, 
					  	 ((TCuvant*)(curent->urm->info))->word) > 0))
			{
				//printf("se face swap\n");          //PROBLEMA
                swap(curent, curent->urm);         
                inv = 1;
            }
            curent = curent->urm;
			//printf("ok aici\n");
        }
    }
}

TH * GenerareHash(TLG listaCuvinte)
{
    TH *h = NULL;
	TLG p;
    TCuvant * cuvant;
	int rez, i;

	/* calcul dimensiuni maxime pt tabela hash */
	size_t M = ('Z'-'A');

	/* initializare tabela hash */
	h = (TH *) InitTH(M, codHash);
	if(h == NULL)
		return NULL;

	for(p = listaCuvinte; p != NULL; p = p->urm) {
        cuvant = (TCuvant *) malloc(sizeof(TCuvant));
        if(cuvant == NULL)
			return h;
//printf("\n \n %s \n \n", ((TCarte*)(p->info))->editura);
        memcpy(cuvant, p->info, sizeof(TCuvant));
		rez = InsTH(h, cuvant, cmpLungime);
        if(!rez) {
			free(cuvant);
			return h;
		}
	}
	for (i=0; i<M; i++){
		//printf("apelez\n");
		for (p = h->v[i]; p != NULL; p = p->urm){
			//printf("ok\n");                           //PROBLEMA
			bubbleSort(((TElement*)(p->info))->l);
		}
	}
	return h;
}

TLG clasificare()
{
	char* s = calloc(10, sizeof(char));
	FILE *f;
	f = fopen("fisier_txt.txt", "rt");
	if (f == NULL)
		return NULL;
	fscanf(f, "%s ", s);
	puts(s);
	if (strcmp(s, "insert") == 0)
		return citesteListaCuvinte("fisier_txt.txt");
	return NULL;
}



TH* InitTH(size_t M, TFHash fh)
{
    TH* h = (TH *) calloc(sizeof(TH), 1);
    if (!h) {
        printf("eroare alocare hash\n");
        return NULL;
    }

    h->v = (TLG *) calloc(M, sizeof(TLG));
	for (int i=0; i<M; i++){
		h->v[i] = NULL;
	}
    if(!h->v) {
        printf("eroare alocare vector de pointeri TLG in hash\n");
        free(h);
        return NULL;
    }
    // for (int i = 0; i < M; i++){
    //     h->v[i] = calloc (15, sizeof(TLG));
    // }
    // for (int i = 0; i < M; i++){
    //         h->v[i];
    // }

    h->M = M;
    h->fh = fh;
    return h;
}

void DistrTH(TH** ah, TF elib_elem)
{
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

void AfiTH(TH* ah,TF afi_elem)
{
    TLG p, el;
    int i;
    for(i = 0; i < ah->M; i++) {
        for (p = ah->v[i]; p != NULL; p = p->urm){
            if(p) {
                //printf("hmmmmmmm : %s\n", ((TElement*) p)->word);
printf("LISTA elementelor de lungime %d care incep cu %d:\n", ((TElement*) (p->info))->lung, i);
                el = ((TElement*)(p->info))->l;
				if (el == NULL) printf("=(\n");
				for(el = ((TElement*)(p->info))->l; el != NULL; el = el->urm)
                    afi_elem(el->info);
                printf("\n");
            }
        }
    }
}

/* daca elementul exista functia intoarce 0
 *altfel se incearca inserarea elementului si se intoarce rezultatul inserarii */
int InsTH(TH*a, void* ae, TFCmp fcmp)
{
    int cod = a->fh(ae), rez, ok=1, c=0;
    TLG el;
//printf("cuvantul de inserat: %s, cu codul: %d\n", ae, cod);
    for(el = a->v[cod]; el != NULL; el = el->urm) {
        //printf("%d\n", ((TElement*)(el->info))->lung);
        if (((TElement*)(el->info))->lung == strlen((char*)ae)){
            ok = 0;
            break;
        }
        c++;
    }
    //printf("ok\n");
    if (ok==1){
        ////printf("Sunt pe cazul cand fac un nou Telement\n");
        //printf("nu a gasit lungime %d, %s\n", strlen(ae), ae);
        TElement* ins=malloc(sizeof(TElement));
        ins->l=NULL;
        ins->lung=strlen((char*)ae);
		TCuvant* add=malloc(sizeof(TCuvant));
		add->frecv = ((TCuvant*)ae)->frecv;
		strcpy(add->word,(char*)ae);

//printf("%d\n\n", ins->lung);
        Ins_LG(&(a->v[cod]), (void*) ins);
		rez = Ins_LG(&(((TElement*)(a->v[cod]->info))->l), (void*) add);
        //rez = Ins_LG(a->v+cod, ae);

    }
    else{
        //printf("Sunt pe cazul cand fac doar un nou Tcuvant\n");
        //for(; c; el = a->v[cod]->urm, c--);
		//printf("pas 1\n");
        TLG* verticala=&((TElement*)(a->v[cod]->info))->l;
		

        TCuvant* add=malloc(sizeof(TCuvant));
        add->frecv = ((TCuvant*)ae)->frecv;
        
        strcpy(add->word,(char*)ae);
//printf("%s\n", add->word);
        rez =  Ins_LG(verticala, (void*) add);
		//Afisare(verticala, afisareCuvant);
        //printf("a gasit lungime %d, %s\n", strlen(ae), ae);
        //printf("\n");
        //rez = Ins_LG(a->v+cod, ae);
    }
    //printf(" lungimea cuvantului %s este %d\n", ae, strlen(ae));
    //rez = Ins_LG(a->v+cod, ae); /* reminder: a->v+cod <=> &a->v[cod] */
    return rez;
}


  
/* function to swap data of two nodes a and b*/

int main()
{//printf("ok\n");
	//TLG listaCarti = clasificare();
	
	TLG listaCuvinte = citesteListaCuvinte("fisier_txt.txt");
	if (listaCuvinte == NULL) {
		printf("Lista nu a putut fi generata\n");
        return 0;
	}

	//counting(listaCuvinte);
	//bubbleSort(listaCuvinte);
	printf("=========== LISTA CARTI ===========\n");
	Afisare(&listaCuvinte, afisareCuvant);

	TH * h = NULL;
	h = GenerareHash(listaCuvinte);
	if (h == NULL) {
		printf("Tabela hash nu a putut fi generata\n");
        return 0;
	}
//printf("okish");
	printf("\n\n=========== TABELA HASH ===========\n");
	AfiTH(h, afisareCuvant);
//printf("okk\n");
  	DistrugeLG(&listaCuvinte, free);
	DistrTH(&h, free);
  	return 0;
}
