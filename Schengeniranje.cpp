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
    printf("usao u exchanges\n");
    while (fgets(buffer1, sizeof(buffer1), fp) != NULL)
    {
        printf("While\n");
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

void putovanje(drzava drzave[100], int brdr,int start, int end,int *rezultat) {
    printf("usao\n");
    int nprovjereni = 1;
    int nnaprovjeri = 0;
    int nadzen = 0;
    int korak = 0;
    int provjereni[1000];
    provjereni[0] = start;
    int predhodni[1000];
    predhodni[0] = 0;
    while (!nadzen) 
    {   
        printf("usao u while\n");
        korak = nprovjereni - nnaprovjeri;
        for (int i = 0; i <= korak; i++)
        {
            for (int j = 0; j < drzave[provjereni[nnaprovjeri]].brojsusjeda; j++)
            {
                provjereni[nprovjereni] = drzave[provjereni[nnaprovjeri]].susjediindex[j];
                predhodni[nprovjereni] = nnaprovjeri;
                nprovjereni++;
            }
            nnaprovjeri++;
        }
        for (int i = 0; i < nprovjereni; i++)
        {
            if (provjereni[i] == end) 
            {
                nadzen++;
                printf("%d\n",i);
            }
                
        }
       
    }
    for (int i = 0; i < nprovjereni; i++)
    {
        printf("%d    %d    %d\n", i, provjereni[i], predhodni[i]);
    }
}




int main() {
    drzava drzave[100] = { 0 };
    int brdr=0;
    int rezultat[100];
    FILE* countries = fopen("countries.txt", "r");
    readcountries(drzave, countries, &brdr);
    fclose(countries);
    FILE* exchange = fopen("exchange.txt", "r");
    readexchanges(drzave, exchange, &brdr);
    fclose(exchange);
    for (int i = 0; i < brdr; i++)
    {    
        dodajindexe(&drzave[i], drzave, brdr);
    }

    for (int i = 0; i < brdr; i++)
    {
        ispis(drzave[i]);
    }
    //putovanje(drzave, brdr, 7, 46, rezultat);

    return 0;
}
