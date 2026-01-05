#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
//Ovo sam promijenio na mobitelu

typedef struct {
    char name[50];
    int brojsusjeda;
    char susjedi[10][50];
    int index;
    int susjediindex[10];
    float exchange;
} drzava;

void ispis(drzava d) {
    printf("%d. %s\n",d.index , d.name);
    printf("%d\n", d.brojsusjeda);
    for (int i = 0; i < d.brojsusjeda; i++) {
        printf("%s ", d.susjedi[i]);
    }
    for (int i = 0; i < d.brojsusjeda; i++) {
        printf("%d ", d.susjediindex[i]);
    }
    printf("\n");
    printf("exchange = %.2f\n", d.exchange);
}

void readcountries(drzava drz[100], FILE* fp, int *brdr) {
    char buffer[255];
    int k = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(drz[k].name, buffer);
        fgets(buffer, sizeof(buffer), fp);
        int n = 0;
        int m = 0;
        for (int i = 0; i < (strlen(buffer)-1); i++)
        {   
            if (buffer[i] == ',')
            {
                n++;
                m = 0;
            }
            else
            {   
                drz[k].susjedi[n][m] = buffer[i];
                m++;
            }
        }
        
        drz[k].brojsusjeda = n+1;
        n = 0;
        drz[k].index = k;
        k++;
    }
    *brdr = k;
}

void readexchanges(drzava drz[100], FILE* fp, int* brdr) {
    char buffer1[255];
    char buffer2[255];
    int k = 0;
    double temp = 0;
    while (fgets(buffer1, sizeof(buffer1), fp) != NULL)
    {
        buffer1[strcspn(buffer1, "\n")] = 0;
        fgets(buffer2, sizeof(buffer2), fp);
        for (int i = 0; i < *brdr; i++)
        {
            if (!strcmp(drz[i].name, buffer1))
            {   
                sscanf(buffer2, "%lf", &temp);

                drz[i].exchange = temp;
            }
        }
    }

}

void dodajindexe(drzava *d, drzava drz[100],int n) {
    for (int i = 0; i < d->brojsusjeda; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (!(strcmp(d->susjedi[i],drz[j].name)))
            {
                d->susjediindex[i] = drz[j].index;
            }
        }
    }
}

int decode(char ime[50], drzava drz[100],int brdr) {
    for (int i = 0; i < brdr; i++)
    {
        if (!(strcmp(ime, drz[i].name)))
        {
            return drz[i].index;
        }
    }
    return -1;
}

float vratitecaj(drzava drz[100],int index){
	return drz[index].exchange;	
}


int maxinput(int *listadrzava,float *listanovaca, int n,int trazeni){
	int maxnovci=-1000;
	int index;
	for(int i=0;i < n;i++){
		if(listadrzava[i]==trazeni && maxnovci<listanovaca[i]){
			maxnovci=listanovaca[i];
			index=i;
		}
	}
	return index;
}
void putovanje(drzava drzave[100], int brdr,int start, int end,int *rezultat, int novci,int troskovi[10], int brtrokova,int *brdrzava,float *ostatak) {

    int nprovjereni = 1;
    int nnaprovjeri = 0;
    int nadzen = 0;
    int korak = 0;
    int provjereni[1000];
    provjereni[0] = start;
    int predhodni[1000];
    predhodni[0] = -1;
    float  pare[1000];
    int ntroskovi = 0;
    pare[0] = novci;
    int indextrazenog;
    int brgranica=0;
    int a = 0;
    while (!nadzen) 
    {   
        korak = nprovjereni - nnaprovjeri;;
        for (int i = 0; i < korak; i++)
        {
            for (int j = 0; j < drzave[provjereni[nnaprovjeri]].brojsusjeda; j++)
            {
                provjereni[nprovjereni] = drzave[provjereni[nnaprovjeri]].susjediindex[j];
                predhodni[nprovjereni] = nnaprovjeri;
                pare[nprovjereni] = pare[nnaprovjeri] - troskovi[ntroskovi]/vratitecaj(drzave,drzave[provjereni[nnaprovjeri]].susjediindex[j]); 
                nprovjereni++;
            }
            nnaprovjeri++;
        }
        for (int i = 0; i < nprovjereni; i++)
        {
            if (provjereni[i] == end) 
            {
                nadzen++;
            }
                
        }
        ntroskovi++;
       
    }
    indextrazenog=maxinput(provjereni,pare,nprovjereni,end);
    int trenutn=indextrazenog;
    *ostatak = pare[indextrazenog];
    while(predhodni[trenutn]!=-1){
    	
        rezultat[brgranica] = provjereni[trenutn];
        trenutn = predhodni[trenutn];
        brgranica++;
    }
    rezultat[brgranica] = provjereni[trenutn];
    *brdrzava = brgranica;
}




int main() {
    drzava drzave[100] = { 0 };
    int brdr=0;
    int rezultat[10];
    int novci;
    int troskovi[10]={0};
    int brtroskova;
    char pocetakime[50];
    char krajime[50];
    int pocetak;
    int kraj;
    int brgranica;
    float ostatakpara;
    scanf("%s[^\n]&*c", pocetakime);
    scanf("%s[^\n]&*c", krajime);
    scanf("%d", &novci);
    scanf("%d", &brtroskova);
    for(int i =0;i<10;i++){
    	rezultat[i]=0;
    }
    for (int i = 0; i < brtroskova; i++)
    {
        scanf("%d", &troskovi[i]);
    }
    FILE* countries = fopen("countries.txt", "r");
    readcountries(drzave, countries, &brdr);
    fclose(countries);
    FILE* exchange = fopen("exchange.txt", "r");
    readexchanges(drzave, exchange, &brdr);
    fclose(exchange);
    pocetak = decode(pocetakime, drzave, brdr);
    kraj = decode(krajime, drzave, brdr);
    for (int i = 0; i < brdr; i++)
    {    
        dodajindexe(&drzave[i], drzave, brdr);
    }
    putovanje(drzave, brdr, pocetak, kraj, rezultat, novci, troskovi, brtroskova, &brgranica, &ostatakpara);
    printf("%s",drzave[rezultat[brgranica]].name);
    for (int i = brgranica-1; i >= 0; i--) {
        printf(" -> %s", drzave[rezultat[i]].name);
    }
	printf("\n%.2f\n",ostatakpara);
	
    return 0;
}
