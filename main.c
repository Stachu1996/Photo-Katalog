 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define fileName photoKatalog.csv;

int cCat = -1;
int cFil = -1;
char cName[1024] = "";

struct photo{
    char id[128];
    char cat[128];
    char name[1024];
    char date[84];
    char path[1024];
};
struct photo * zdj;

struct photo empty;

int sizeZdj = 0;


void clrscr(){
    system("@cls||clear");
}

void menu(){
    if(cFil !=-1) printf("selected Filter: %i ", cFil);
    if(cCat !=-1 ) printf("selected CATEGORY: %i ", cCat);
    if(strcmp(cName , "")) printf("search by Name: %s \n");
    printf("\n1 - add record | 2 - remove record | 3 - filter settings | 4 - select category | 5 - search record | 6 - exit program");
}
void header(){
    printf("|   ID   | CATEGORY |         NAME         |   DATE   |                    PATH                    \n");
}
void whiteSpace(int ilo){
    int i;
    for(i=0;i<ilo;i++){
        printf(" ");
    }
}
int checkLen(int j, int value){
    int i;
    int x=1;
    for(i=0; i<j; i++){
        x=x*10;
        if(value-x<0){
            return j;
        }
        j--;
    }
    return j;
}
void printData(struct photo zdj){
    //if(strlen(zdj.id)==0) return;
    if(cCat==-1 || atoi(zdj.cat)==cCat){
        printf("|");
        printf("%s",zdj.id);
        whiteSpace(8-strlen(zdj.id));
        printf("|");

        printf("%s",zdj.cat);
        whiteSpace(10-strlen(zdj.cat));
        printf("|");

        printf("%s",zdj.name);
        whiteSpace(22-strlen(zdj.name));
        printf("|");

        printf("%s",zdj.date);
        whiteSpace(10-strlen(zdj.date));
        printf("|");

        printf("%s",zdj.path);
        whiteSpace(10-strlen(zdj.path));
        printf("\n");
    }
}

const char* getRecord(char* line, int num){
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return "";
}

void readFile(){
	sizeZdj=0;
	free(zdj);
	zdj = (struct photo *) malloc (sizeZdj * sizeof(struct photo));
	FILE *stream = fopen("photoKatalog.csv", "r");
	char line[1024];
	int i=0;
	while (fgets(line, 1024, stream)){
		sizeZdj++;
		zdj = realloc(zdj, sizeZdj * sizeof(struct photo));
        char* tmp1 = strdup(line);
        char* tmp2 = strdup(line);
        char* tmp3 = strdup(line);
        char* tmp4 = strdup(line);
        char* tmp5 = strdup(line);
		
        strcpy(zdj[i].id, getRecord(tmp1, 1));
        strcpy(zdj[i].cat, getRecord(tmp2, 2));
        strcpy(zdj[i].name, getRecord(tmp3, 3));
    	strcpy(zdj[i].date, getRecord(tmp4, 4));
        strcpy(zdj[i].path, getRecord(tmp5, 5));

        free(tmp1);
        free(tmp2);
        free(tmp3);
        free(tmp4);
        free(tmp5);
        i++;
    }
	fclose(stream);
}

void saveAllData(){
    FILE *out_file  = fopen("photoKatalog.csv", "w"); // read only
    int i;

    for(i=0; i<sizeZdj; i++){
        if( strcmp(zdj[i].id, "NULL") ){
            fprintf(out_file, "%s,%s,%s,%s,%s\n", zdj[i].id, zdj[i].cat, zdj[i].name, zdj[i].date, zdj[i].path);
        }
    }
    fclose(out_file);
    readFile();
}

void printAllData(){
    int i;

    for(i=0; i<sizeZdj; i++){
        printData(zdj[i]);
    }
}

void addRecord(){
    struct photo one;
    int ok=1;
    do{
        ok=1;
        printf("Type ID: ");
        scanf("%s", one.id);
        int i;
        for(i=0; i<sizeZdj; i++){
            if(strcmp(one.id, zdj[i].id)==0) {
                ok=0;
                break;
            }
        }
    }while(ok==0);
    printf("Type CATEGORY: ");
    scanf("%s", one.cat);
    printf("Type NAME: ");
    scanf("%s", one.name);
    printf("Type DATE (dd.mm.rrrr): ");
    scanf("%s", one.date);
    printf("Type PATH: ");
    scanf("%s", one.path);
    
    sizeZdj++;
	zdj = realloc(zdj, sizeZdj * sizeof(struct photo));

	zdj[sizeZdj-1]=one;
	printf("\nRecord added");
	printf("\nPress Any Key to Continue\n");
    getchar();
    saveAllData();
}

void removeRecord(){
    char id[128];
    printf("Type ID to be removed: ");
    scanf("%s", id);
    int i;
    for(i=0; i<sizeZdj; i++){
        if(strcmp(id, zdj[i].id)==0){
            zdj[i]=empty; //TODO poprawa czysczenia;
            strcpy(zdj[i].id, "NULL");
            saveAllData();
            break;
        }
    }
}

void searchRecord(){
    char name[128];
    printf("Type NAME to be search: ");
    scanf("%s", name);
    char *ret;
    int i;
    //printf("Result found for (%s):\n",name);
    for(i=0; i<sizeZdj; i++){
        ret = strstr(zdj[i].name, name);
        if(ret){
            printData(zdj[i]);
        }
    }
    printf("Press Any Key to Continue\n");
    getchar();
    getchar();
    clrscr();
}

void selectCategory(){
    printf("type CATEGORY (to restore default type -1): ");
    scanf("%i", &cCat);
}

void selectFilter(){
	int grow;
    printf("type Filter (-1)default (0)ID (1)Category (2)Name (3)Date (4)Path: ");
    scanf("%i", &cFil);
    if(cFil!=-1){
	do{
	    printf("select (0) growing filtering (1) decending filtering: ");
	    scanf("%i", &grow);
	    printf("%i", grow);
    }while(grow!=0 && grow!=1);
	}

	if(cFil==-1){
		readFile();
    }else if(cFil==0){//ID
    	if(grow==1){
	        int i, j;
	        int n = sizeZdj;
            for (j = 0; j < n-1; j++)
            for(i=0; i<n-j-1; i++){
	                printf("%i>%i; ", atoi(zdj[i+1].id), atoi(zdj[i].id));

	                if( atoi(zdj[i+1].id) > atoi(zdj[i].id) ){
	                    struct photo tem;
	                    tem = zdj[i+1];
	                    zdj[i+1]=zdj[i];
	                    zdj[i]=tem;
	                }
	            }
	        
    	}else if(grow==0){
	        int i, j;
	        int n = sizeZdj;
            for (j = 0; j < n-1; j++)
            for(i=0; i<n-j-1; i++){
	                printf("%i>%i; ", atoi(zdj[i+1].id), atoi(zdj[i].id));

	                if( atoi(zdj[i+1].id) < atoi(zdj[i].id) ){
	                    struct photo tem;
	                    tem = zdj[i+1];
	                    zdj[i+1]=zdj[i];
	                    zdj[i]=tem;
	                }
	            }
    	}
    }else if(cFil==1){//CAT
        if(grow==1){
	        int i,j;
	        int n = sizeZdj;
            for (j = 0; j < n-1; j++)
            for(i=0; i<n-j-1; i++){
                    printf(" (%d)(%s>%s); ", i, zdj[i].cat, zdj[i+1].cat);

                    if( atoi(zdj[i].cat) < atoi(zdj[i+1].cat) ){
                        
                        struct photo tem;
                        tem = zdj[i];
                        zdj[i]=zdj[i+1];
                        zdj[i+1]=tem;
                    }
	            }
	        
    	}else if(grow==0){
	        int i,j;
	        int n = sizeZdj;
            for (j = 0; j < n-1; j++)
            for(i=0; i<n-j-1; i++){
	                if( zdj[i+1].cat < zdj[i].cat ){

	                    struct photo tem;
	                    tem = zdj[i+1];
	                    zdj[i+1]=zdj[i];
	                    zdj[i]=tem;
	                }
	            }
    	}
    }else if(cFil==2){//name
        if(grow==1){
	        int i;
	        int j;
	        int n = sizeZdj;
                for (j = 0; j < n-1; j++)
	            for(i=0; i<n-j-1; i++){
	               // printf(" strcoll(%d) i+1:%s i:%s \n",strcoll(zdj[i+1].name, zdj[i].name), zdj[i+1].name, zdj[i].name);
	                if(strcoll(zdj[i].name, zdj[i+1].name)<0){
                        struct photo tem;
	                    tem = zdj[i+1];
	                    zdj[i+1]=zdj[i];
	                    zdj[i]=tem;
                    }
	            }
    	}else if(grow==0){
	        int i;
	        int j;
	        int n = sizeZdj;
            for (j = 0; j < n-1; j++)
            for(i=0; i<n-j-1; i++){
                // printf(" strcoll(%d) i+1:%s i:%s \n",strcoll(zdj[i+1].name, zdj[i].name), zdj[i+1].name, zdj[i].name);
                if(strcoll(zdj[i].name, zdj[i+1].name)>0){
                    struct photo tem;
                    tem = zdj[i+1];
                    zdj[i+1]=zdj[i];
                    zdj[i]=tem;
                }
            }
    	}
    }else if(cFil==3){//DATE
        if(grow==1){
	        int i;
	        int j;
	        int n = sizeZdj;
                for (j = 0; j < n-1; j++)
	            for(i=0; i<n-j-1; i++){

	                if(strcoll(zdj[i].date, zdj[i+1].date)<0){
                        struct photo tem;
	                    tem = zdj[i+1];
	                    zdj[i+1]=zdj[i];
	                    zdj[i]=tem;
                    }
	            }
    	}else if(grow==0){
	        int i;
	        int j;
	        int n = sizeZdj;
            for (j = 0; j < n-1; j++)
            for(i=0; i<n-j-1; i++){

                if(strcoll(zdj[i].date, zdj[i+1].date)>0){
                    struct photo tem;
                    tem = zdj[i+1];
                    zdj[i+1]=zdj[i];
                    zdj[i]=tem;
                }
            }
    	}
    }else if(cFil==4){//PATH
        if(grow==1){
	        int i;
	        int j;
	        int n = sizeZdj;
                for (j = 0; j < n-1; j++)
	            for(i=0; i<n-j-1; i++){

	                if(strcoll(zdj[i].path, zdj[i+1].path)<0){
                        struct photo tem;
	                    tem = zdj[i+1];
	                    zdj[i+1]=zdj[i];
	                    zdj[i]=tem;
                    }
	            }
    	}else if(grow==0){
	        int i;
	        int j;
	        int n = sizeZdj;
            for (j = 0; j < n-1; j++)
            for(i=0; i<n-j-1; i++){

                if(strcoll(zdj[i].path, zdj[i+1].path)>0){
                    struct photo tem;
                    tem = zdj[i+1];
                    zdj[i+1]=zdj[i];
                    zdj[i]=tem;
                }
            }
    	}
    }
}

int main()
{
    //----------------------------------------------------------------- CHAPTER 1 -----------------------------------------------------------------
    FILE *in_file  = fopen("photoKatalog.csv", "r"); // read only
    
    printf("___________/------ Photo Katalog -----\\___________\n");
    printf("    Welcome in program writen by Szymon Stach \n");

    if (in_file == NULL)
    {
    	FILE *out_file  = fopen("photoKatalog.csv", "w"); // read only
        fprintf(out_file, "0,1,zachod slonca,20.02.2018,E:\\!kopie stanowsik\\Patrycja\\DCIM\\Camera\\20180220_2123.jpg\n");
        fprintf(out_file, "1,2,zbuchtowana ziemia,19.01.2018,D:\\zdjecia\\Bieszczady\\DCIM\\Camera\\20190119_57653.jpg\n");
        fprintf(out_file, "2,2,wadera o zmierzchu,19.01.2018,D:\\zdjecia\\Bieszczady\\DCIM\\Camera\\20190119_63442.jpg\n");
		fclose(out_file);
        printf("New file with example data created\n");
        readFile();
    }else if(in_file != NULL){
        readFile();
        printf("Data was loaded\n");
    }else{
        printf("\nError! Could not open file\n");
    }
	fclose(in_file);
    printf("Press Any Key to Continue\n");
    getchar();
    clrscr();
    //----------------------------------------------------------------- CHAPTER 2 -----------------------------------------------------------------
    char a;
    do{
    header();
    printAllData();
    menu(); 
	    
    a=getchar();
    clrscr();

    switch(a){
        case '1': // add record
            addRecord();
            break;
        case '2': //remove record
            removeRecord();
            break;
        case '3': // filter settings
            selectFilter();
            break;
        case '4': // select category
            selectCategory();
            break;
        case '5': // search
            searchRecord();
            break;
    }


    }while(a!='6');

    //----------------------------------------------------------------- EPILOG -----------------------------------------------------------------

    return 0;
}
