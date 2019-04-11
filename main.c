#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include<stdlib.h>



enum pubType {
    Encyclopedia = 1,
    Periodical = 2,
    Book = 3
};
typedef union {
    int iPrice;
    float fPrice;
} prices ;

typedef union {
    char Author[60];
    char Publisher[60];
    char Editor[60];

} workers ;
struct publications{

    enum pubType pub;
    char name[60];
    prices price;
    workers worker;
    int volNo;
    bool isInStock;
    struct publications *next;
};

//FUNCTION DECLARATIONS
void push(struct publications *);
void readFile(char filename[]);
void writeFile();
void display(int i);
void delete(int i);
void insert();



#define MAXCHAR 1000
void readFile(char filename[] ){


    FILE *fp;
    char str[MAXCHAR];
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return;
    }
    while (fgets(str, MAXCHAR, fp) != NULL){
        int count=0;
        char delim[] = "#"; // her bir line '#'  char'ina gore split edilir
        char *ptr = strtok(str, delim);


        struct publications *piv = (struct publications*)malloc(sizeof(struct publications));
        count=0;
        while(ptr != NULL)
        {
            if(count==0){ //count 0 ise publication type belirlenir
                char sub[2];   // tum sub degerleri, gereksiz space char elemelerini yapmak icin olusturuldu
                memcpy( sub, ptr, strlen(ptr)-1 );
                sub[1] = '\0';

                if(strcmp("E",sub)==0)
                    piv->pub=Encyclopedia;
                else if(strcmp("B",sub)==0)
                    piv->pub=Book;
                else if(strcmp("P",sub)==0)
                    piv->pub=Periodical;
            }
            if(count==1) {//count 1 ise name belirlenir
                char sub[60];
                memcpy( sub, ptr+1, strlen(ptr)-2 );
                sub[strlen(ptr)-2] = '\0';
                strcpy(piv->name,sub);
            }

            if(count==2) {//count 2 ise calisanin tipi belirlenir
                char sub[40];
                memcpy( sub, ptr+1, strlen(ptr)-2 );
                sub[strlen(ptr)-2] = '\0';
                if (piv->pub == Encyclopedia) {
                    strcpy(piv->worker.Publisher,sub);
                }
                else if (piv->pub == Book) {
                    strcpy(piv->worker.Author,sub);
                }
                else if (piv->pub == Periodical) {
                    strcpy(piv->worker.Editor,sub);
                }
            }
            if(count==3) {//count 3 ise price belirlenir
                char sub[10];
                memcpy( sub, ptr+1, strlen(ptr)-2 );
                sub[strlen(ptr)-2] = '\0';

                if (piv->pub == Encyclopedia) {
                    piv->price.iPrice= atoi(sub);
                }
                else if (piv->pub == Book) {
                    piv->price.iPrice= atoi(sub);
                }
                else if (piv->pub == Periodical) {
                    piv->price.fPrice= atof(sub);
                }
            }
            if(count==4) {//count 4 ise stok durumu belirlenir

                char sub[2];
                memcpy( sub, ptr+1, 1 );
                sub[1] = '\0';

                if(strcmp("T",sub)==0)
                    piv->isInStock= true;
                else
                    piv->isInStock=false;


            }
            if(count==5) {//count 5 ise VolNo belirlenir
                char sub[10];
                memcpy( sub, ptr+1, strlen(ptr)-1 );
                sub[strlen(ptr)-1] = '\0';
                piv->volNo=atoi(sub);
            }

            count++;
            ptr = strtok(NULL, delim);
        }
        piv->next=NULL;
        push(piv);

    }

    fclose(fp);
}

int size=0;//linkedlist'in size'i tutulur
struct publications *head;

void push(struct publications *pub){
    if(head==NULL) {
        head = pub;
        size++;
        return;
    }

    struct publications *piv = head;
    struct publications *prev=NULL;

    while(piv->next!=NULL && strcmp(piv->name,pub->name)< 0){
        prev=piv;
        piv=piv->next;
    }

    if (piv==head){ // listenin basinda ise
        if(strcmp(piv->name,pub->name)< 0) //listenin basinda ve head'den sonra ise
            head->next=pub;
        else{ //listenin basinda ve head'e gelecek ise
            struct publications *next = head;
            head = pub;
            pub->next=next;
        }
    }
    else if(piv->next!=NULL){ // arada bir yerde
        prev->next = pub;
        pub->next = piv;
    }
    else { // listteki son eleman
        piv->next=pub;
    }
    size++;
}


void display(int i){
    struct publications *piv = head;

    if(i>size){
        printf("There is no %dth element in the list\n",i);
        return;
    }

    int count=1;
    while(piv->next!=NULL){
        if(count==i)
            break;
        piv=piv->next;
        count++;
    }

    if (piv->pub == Encyclopedia) {
        printf("%dth element :\nPublication: Encyclopedia\nName : %s\nPublisher : %s\nPrice : %d\nIsInStock : %s\n\n",i,piv->name,piv->worker.Publisher,piv->price.iPrice,piv->isInStock ? "Yes" : "No");
    }
    else if (piv->pub == Periodical) {
        printf("%dth element :\nPublication: Periodical\nName : %s\nPublisher : %s\nPrice : %.2f\nIsInStock : %s\nVolNo : %d\n\n",i,piv->name,piv->worker.Editor,piv->price.fPrice,piv->isInStock ? "Yes" : "No",piv->volNo);
    }
    else if (piv->pub == Book) {
        printf("%dth element :\nPublication: Book\nName : %s\nPublisher : %s\nPrice : %d\nIsInStock : %s\n\n",i,piv->name,piv->worker.Author,piv->price.iPrice,piv->isInStock ? "Yes" : "No");
    }


}


void delete(int i){
    if(i>size){
        printf("There is no %dth element in the list\n",i);
        return;
    }
    struct publications *piv = head;
    if(i==1){ //eger ki silmemiz istenen node==head ise, buna uygun islem yapilir
        head = piv->next;
        free(piv);
        size--;
        return;
    }

    for (int j=1; piv!=NULL && j<i-1; j++) //silinecek node'dan bir onceki node'da durduk
        piv = piv->next;

    struct publications *next = piv->next->next; //silinecek node'un devami
    free(piv->next); //node'u sildik
    piv->next=next; // node'un devamini ekledik
    size--;
}

void insert(){
    struct publications *piv = (struct publications*)malloc(sizeof(struct publications));
    char str[60];
    printf( "Enter the type of publications: 'E' For Encyclopedia, 'B' for Book, 'P' for Periodical\n");
    scanf("%s", str);

    if(strcmp("E",str)==0){
        piv->pub=Encyclopedia;
        printf( "Enter the name :\n");
        scanf("%s", str);
        strcpy(piv->name,str);
        printf( "Enter the Publisher:\n");
        scanf("%s", str);
        strcpy(piv->worker.Publisher,str);
        printf( "Enter the Price:\n");
        scanf("%s", str);
        piv->price.iPrice=atoi(str);
        printf( "Enter if in Stock: 'True' or 'False'\n");
        scanf("%s", str);
        bool bol=true;
        if(strcmp(str,"False")==0)
            bol=false;
        piv->isInStock=str;
    }
    else if(strcmp("B",str)==0) {
        piv->pub = Book;
        printf( "Enter the name :\n");
        scanf("%s", str);
        strcpy(piv->name,str);
        printf( "Enter the Author:\n");
        scanf("%s", str);
        strcpy(piv->worker.Author,str);
        printf( "Enter the Price:\n");
        scanf("%s", str);
        piv->price.iPrice=atoi(str);
        printf( "Enter if in Stock: 'True' or 'False'\n");
        scanf("%s", str);
        bool bol=true;
        if(strcmp(str,"False")==0)
            bol=false;
        piv->isInStock=str;
    }
    else if(strcmp("P",str)==0) {
        piv->pub = Periodical;
        printf( "Enter the name :\n");
        scanf("%s", str);
        strcpy(piv->name,str);
        printf( "Enter the Editor:\n");
        scanf("%s", str);
        strcpy(piv->worker.Editor,str);
        printf( "Enter the Price:\n");
        scanf("%s", str);
        piv->price.fPrice=atof(str);
        printf( "Enter if in Stock: 'True' or 'False'\n");
        scanf("%s", str);
        bool bol=true;
        if(strcmp(str,"False")==0)
            bol=false;
        piv->isInStock=str;
        printf( "Enter the VolNo:\n");
        scanf("%s", str);
        piv->volNo=atoi(str);
    }
    piv->next=NULL;
    push(piv);
}



void writeFile(){
    FILE *f = fopen("outputbook.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    struct publications *piv = head;
    while(piv!=NULL){
        if(piv->pub==Encyclopedia)
            fprintf(f, "E # %s # %s # %d # %s\n",piv->name,piv->worker.Publisher,piv->price.iPrice,piv->isInStock ? "T" : "F");
        else if(piv->pub==Book)
            fprintf(f, "B # %s # %s # %d # %s\n",piv->name,piv->worker.Author,piv->price.iPrice,piv->isInStock ? "T" : "F");
        else if(piv->pub==Periodical)
            fprintf(f, "E # %s # %s # %.2f # %s # %d\n",piv->name,piv->worker.Editor,piv->price.fPrice,piv->isInStock ? "T" : "F",piv->volNo);
        piv=piv->next;

    }

    fclose(f);




}
void Quit(){
    while(size>0){ //hep ilk elementi siler, ne zaman ki liste bosalir o zaman dongu sona erer
        delete(1);
    }
}

int main() {

    char *fileName="inputbook.txt";
    head=NULL;
    readFile(fileName);
    display(1);
    display(2);
    display(3);
    display(4);
    delete(1);
    insert(); // inputlarla bir girdi yapilir
    display(1);
    display(2);
    display(3);
    display(4);
    writeFile();
    Quit();
    display(1);
    return 0;
}