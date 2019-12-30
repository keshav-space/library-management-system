#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <io.h>		//windows directry handler
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <windows.h>
#include <wchar.h>
#include <locale.h>
#include <fcntl.h>
#define stringMax 50 

#define Menu 15

#define Ilux 115     //input filed left up corner
#define Iluy 35
#define Ildx 115    //input filed left down corner
#define Ildy 37
#define Irux 161    //input filed right up corner
#define Iruy 35

#define blux 50     //book filed left up corner
#define bluy 10
#define bldx 50    //book filed left down corner
#define bldy 34
#define brux 129    //book filed right up corner
#define bruy 10

#pragma comment(lib, "user32.lib")


int primarybook;
int primaryborrower;
char timeBeg[stringMax];

typedef struct primaryKey{
    int bookId;
    int borrowerId;
} primaryKey;

typedef struct borrower{
	int bookId;
	int borrowerId;
	char fineImposed[stringMax];
	char borrowedBook[stringMax];
	char borrowingDate[stringMax];
	char borrowerName[stringMax];
	char dueDate[stringMax];
	char returnDate[stringMax];
} borrower;

typedef struct book {
	int bookId; 
	int self;
	int quantTotal;
	int avalQuantity;
	char bookName[stringMax]; 
	char authName[stringMax];
	char genre[stringMax];
} book;

typedef struct surveil{
	char oemId[stringMax];
	char processors[stringMax];
	char pagesize[stringMax];
	char processorType[stringMax];
	char minAdd[stringMax];
	char maxAdd[stringMax];
	char activeProc[stringMax];
	char computerName[stringMax];
	char userName[stringMax];
	char windoVersion[stringMax];
	char startTime[stringMax];
	char endTime[stringMax];
} surveil;

int inputInt(void);							
int pstrcmp(char *,char *); 				
int DirectoryExists( const char* );			 //win specific
int dateOp(int *dd,int *,int *,int *,int);	
void addDate(int *,int *,int *,int);		
void addBooks(void); 						
void viewBooks(void);						
void searchBook(void);						
void editBook(void);						
void deleteBook(void);						
void issueBook(void);							
void showIssuedData(void);					
void collectBook(void);						
void primaryInitalize(void);				
void primaryRecord(void);					
void surveillance(void);					 //win specific
void innerBorder(void);						
void gotoxy(int , int );					
void inputField(void);						
void border(void);							
void menu(void);							
void surveillance(void);					


void gotoxy(int x, int y){
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
 
void innerBorder(){
	_setmode(_fileno(stdout), 0x00040000);

    gotoxy(blux-15,bluy+1);
    gotoxy(blux,bluy);
     wprintf(L"═══════════════════════════════════════════════════════════════════════════════");
    gotoxy(blux-1,bluy);
     wprintf(L"╔");
    for(int i=bluy+1;i<bldy;i++){
        gotoxy(blux-1,i);
         wprintf(L"║");
    }
    gotoxy(blux-1,bldy);
     wprintf(L"╚");
    gotoxy(bldx,bldy);
     wprintf(L"═══════════════════════════════════════════════════════════════════════════════");
    gotoxy(brux,bruy);
     wprintf(L"╗");
    for(int i=bluy+1;i<bldy;i++){
        gotoxy(brux,i);
         wprintf(L"║");
    }   
    gotoxy(brux,bldy);
     wprintf(L"╝");
    gotoxy(blux+2,bluy+1);
	_setmode(_fileno(stdout), _O_TEXT);
} 

void inputField(){
	_setmode(_fileno(stdout), 0x00040000);
    gotoxy(Ilux-15,Iluy+1);
     wprintf(L"Input Field");
    gotoxy(Ilux,Iluy);
     wprintf(L"══════════════════════════════════════════════");
    gotoxy(Ilux-1,Iluy);
     wprintf(L"╔");
    gotoxy(Ilux-1,Iluy+1);
     wprintf(L"║");
    gotoxy(Ilux-1,Iluy+2);
     wprintf(L"╚");
    gotoxy(Ildx,Ildy);
     wprintf(L"══════════════════════════════════════════════");
    gotoxy(Irux,Iruy);
     wprintf(L"╗");
    gotoxy(Irux,Iruy+1);
     wprintf(L"║");    
    gotoxy(Irux,Iruy+2);
     wprintf(L"╝");
    gotoxy(Ilux+2,Iluy+1);
	_setmode(_fileno(stdout), _O_TEXT);

}

void border(){
	_setmode(_fileno(stdout), 0x00040000);
    setlocale(LC_CTYPE, "");
    gotoxy(0,0);
    wprintf(L"╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗");
    
    gotoxy(0,1);
    for(int i=0;i<=37;i++){ //left
         wprintf(L"║\n");
    }

    gotoxy(0,38);
    wprintf(L"╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝");

    gotoxy(1,5);
    wprintf(L"═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════");

    for(int i=1;i<=37;i+=1){    //right
        gotoxy(166,i);
        wprintf(L"║");
    }

    gotoxy(60,3);
     wprintf(L"Library Management System");
    gotoxy(5,35);
	_setmode(_fileno(stdout), _O_TEXT);
}

void menu(){
	_setmode(_fileno(stdout), 0x00040000);
    setlocale(LC_CTYPE, "");
    gotoxy(65,Menu);
     wprintf(L"○○ 1. Add Books   ");
    gotoxy(65,Menu+2);
     wprintf(L"○○ 2. Delete books");
    gotoxy(65,Menu+4);
     wprintf(L"○○ 3. Search Books");
    gotoxy(65,Menu+6);
     wprintf(L"○○ 4. Issue Books");
    gotoxy(65,Menu+8);
     wprintf(L"○○ 5. Collect Book");
    gotoxy(65,Menu+10);
     wprintf(L"○○ 6. Edit Book's Record");
    gotoxy(65,Menu+12);
     wprintf(L"○○ 7. View Book list");
    gotoxy(65,Menu+14);
     wprintf(L"○○ 8. View Issue Records");
    gotoxy(65,Menu+16);
     wprintf(L"○○ 9. Close Application");
    gotoxy(65,Menu+18);
	_setmode(_fileno(stdout), _O_TEXT);
}


void surveillance(void){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	SYSTEM_INFO siSysInfo;
   	// Copy the hardware information to the SYSTEM_INFO structure. 
   	GetSystemInfo(&siSysInfo); 
   	// Display the contents of the SYSTEM_INFO structure. 
	char buffer[stringMax + 1]; 
  	DWORD size =stringMax + 1 ;
	surveil sir;
   	sprintf(sir.oemId,"OEM ID: %u", siSysInfo.dwOemId);
   	sprintf(sir.processors,"Number of processors: %u", siSysInfo.dwNumberOfProcessors); 
  	sprintf(sir.pagesize,"Page size: %u", siSysInfo.dwPageSize); 
   	sprintf(sir.processorType,"Processor type: %u", siSysInfo.dwProcessorType); 
   	sprintf(sir.minAdd,"Minimum application address: %lx",siSysInfo.lpMinimumApplicationAddress); 
   	sprintf(sir.maxAdd,"Maximum application address: %lx",siSysInfo.lpMaximumApplicationAddress); 
   	sprintf(sir.activeProc,"Active processor mask: %u",siSysInfo.dwActiveProcessorMask);

	GetComputerName( buffer, &size );
  	sprintf( sir.computerName,"Computer name = %s", buffer );
  
  	GetUserName( buffer, &size );
  	sprintf( sir.userName,"user name = %s", buffer );

  	OSVERSIONINFOEX info;
  	ZeroMemory(&info, sizeof(OSVERSIONINFOEX));
  	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
  	GetVersionEx((LPOSVERSIONINFO)&info);//info requires typecasting

  	sprintf(sir.windoVersion,"Windows version: %u.%u", info.dwMajorVersion, info.dwMinorVersion);

	strcpy(sir.startTime,timeBeg);
	sprintf(sir.endTime,"End| Date: %d-%d-%d Time: %d:%d:%d",tm.tm_mday,tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

	FILE *file; 
    file = fopen ("unity/.usrBkp", "ab+");
    if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    } 
	fwrite (&sir, sizeof(surveil), 1, file);
    fclose (file); 
}


int DirectoryExists( const char* absolutePath ){

    if( _access( absolutePath, 0 ) == 0 ){
        struct stat status;
        stat( absolutePath, &status );

        return (status.st_mode & S_IFDIR) != 0;
    }
    return 0;
}

int inputInt(){
    char term; int num;
    x:
    if(scanf("%d%c", &num, &term) != 2 || (term != ' ' && term!='\n') || num<0){
     printf("\n\t Oops!, Plese enter Posetive numeric value\nEnter value again: ");
    while ((getchar()) != '\n');    //flushing the input buffer
    goto x;
    }
    else if(term==' ')
	     while((getchar())!='\n');
    return num;
}


int pstrcmp(char *s1, char *s2){ 
	/* implementation of strcmp that ignores cases */
	int i; 
	for (i = 0; s1[i] && s2[i]; ++i){ 
		/* If characters are same or inverting the 6th bit makes them same */
		if (s1[i] == s2[i] || (s1[i] ^ 32) == s2[i]) 
		continue; 
		else
		break; 
	} 

	/* Compare the last (or first mismatching in case of not same) characters */
	if (s1[i] == s2[i]) 
		return 0; 

	// Set the 6th bit in both, then compare 
	if ((s1[i] | 32) < (s2[i] | 32)) 
		return -1; 
	return 1; 
} 

int dateOp(int *dd,int *mm,int *yyyy,int *days,int op){
	int flag=0;
	if(*days>op){
		*days-=(op-*dd);
		if(*mm+1==13){
			*mm=1;
			*yyyy+=1;
		}
        else
        	*mm+=1;

        if(*days)
			*dd=0;
        else
            *dd=31;
                    
	}
	else if(*dd+ *days<=op){
		*dd+=*days;
		flag=1;
	}
	else{
		*mm+=1;
		*days-=(31-*dd);
		*dd=*days;
		flag=1;
	}
	return flag;	
}

void primaryInitalize(){
	FILE *file; 
	primaryKey rec;   
 
    file = fopen ("unity/.primaryRecord", "rb");
    if (file==NULL) { 
        primarybook= 3001;
        primaryborrower=9001;
    }
    else{
        fread(&rec, sizeof(primaryKey), 1, file);
        primarybook=rec.bookId;
        primaryborrower=rec.borrowerId;
        fclose (file); 
    }
}

void primaryRecord(){
    FILE *file; 
	primaryKey rec;   
 
    file = fopen ("unity/.primaryRecord", "wb+");
    if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    } 
    rec.bookId=primarybook;
    rec.borrowerId=primaryborrower;
    fseek(file, -sizeof(primaryKey), SEEK_CUR);
	fwrite (&rec, sizeof(primaryKey), 1, file);
    fclose (file); 
}

void addDate(int *dd,int *mm,int *yyyy,int days){
	*mm+=1;
	*yyyy+=1900;
	while (days){
		if( *mm==1 || *mm==3 || *mm==5 || *mm==7 || *mm==8 || *mm==10 || *mm==12){
			if(dateOp(dd,mm,yyyy,&days,31)){
				break;
			}
		}

		else if(*mm==4 || *mm==6 || *mm==9 || *mm==11){
			if(dateOp(dd,mm,yyyy,&days,30)){
				break;
			}
		}

		else if(*mm==2 && ((*yyyy)%4==0)){
			if(dateOp(dd,mm,yyyy,&days,29)){
				break;
			}
		}

		else if(*mm==2 && ((*yyyy)%4!=0)){
			if(dateOp(dd,mm,yyyy,&days,28)){
				break;
			}
		}
	}
}

void addBooks(){
	int a=1,c;
	FILE *file; 
	char *p,value[10];
	book add;   
    // open file for writing 
    file = fopen ("unity/.bookDatabse", "ab+");
    if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    } 
	
	gotoxy(70,8);
    printf("ENTER THE BOOK RECORD");
	//a=inputInt();
	a=1;
	
	while(a){
		gotoxy(55,15);
		printf("Enter book name: ");
		fgets (add.bookName,stringMax,stdin);
		if ((p=strchr(add.bookName, '\n')) != NULL)
            *p = '\0';	

		add.bookId=primarybook;
		primarybook++;


		gotoxy(55,17);
		printf("Enter author name: ");
		fgets (add.authName,stringMax,stdin);
		if ((p=strchr(add.authName, '\n')) != NULL)
            *p = '\0';
		
		gotoxy(55,19);
		printf("Enter genre: ");
		fgets (add.genre,stringMax,stdin);
		if ((p=strchr(add.genre, '\n')) != NULL)
            *p = '\0';
		
		gotoxy(55,21);
		printf("Enter self number: ");
		//fgets (value,10,stdin);
		add.self=inputInt();//atoi(value);

		gotoxy(55,23);
		printf("Enter Quantity: ");
		//fgets (value,10,stdin);
		add.quantTotal=inputInt();//atoi(value);
		add.avalQuantity=add.quantTotal;//atoi(value);

		gotoxy(55,27);		

    	// write struct to file 
    	fwrite (&add, sizeof(book), 1, file);   
    	if(fwrite != 0)  
        	 printf("Contents to file written successfully !\n"); 
    	else 
        	 printf("error writing file !\n");
		a--;	
	}
	fclose (file); 
}

void viewBooks(void){
	FILE *file;
	book input;
	char ans[4]="n";
	file = fopen ("unity/.bookDatabse", "ab+");
    if (file == NULL) { 
        fprintf(stderr, "\nError opend file\n"); 
        exit (1); 
    } 

	while(fread(&input, sizeof(struct book), 1, file) && ((ans[0]=='n')||(ans[0]=='N'))){
		system("clS");
    	border();
		innerBorder();
		gotoxy(55,15);
		printf ("BookName: %s", input.bookName);
		gotoxy(55,17);
		printf ("Book Id: %d",input.bookId);
		gotoxy(55,19);
		printf ("Author Name: %s", input.authName);
		gotoxy(55,21);
		printf ("Genre: %s",input.genre);
		gotoxy(55,23);
		printf ("Self Number: %d",input.self);
		gotoxy(55,25);
		printf ("Total Quantity: %d",input.quantTotal);
		gotoxy(55,27);
		printf ("Available Quantity: %d",input.avalQuantity);

		gotoxy(55,30);
		printf ("Enter N for next, Q to quit: (N|Q)");

		inputField();	
		scanf("%s",ans);
		gotoxy(Ilux+2,Iluy+1);
	}
	// close file 
	fclose (file); 
}

void searchBook(void){
	FILE *file;
	book input;
	char ch[stringMax],*p,c[10],ans[4]="n";
	int flag=1;

	file = fopen ("unity/.bookDatabse", "ab+");
	if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }

	gotoxy(55,15);
	printf("Enter book name|Id|Author|Genre: ");
	inputField();
	fgets (ch,stringMax,stdin);
	if ((p=strchr(ch, '\n')) != NULL)
        *p = '\0';

	while(fread(&input, sizeof(struct book), 1, file) && ((ans[0]=='n')||(ans[0]=='N')) ){
		 sprintf(c,"%d",input.bookId);
		if(!pstrcmp(c,ch)  || !pstrcmp(input.bookName,ch) || (!pstrcmp(input.genre,ch)) || !pstrcmp(input.authName,ch)  ){
			flag=0;
			system("cls");
    		border();
			innerBorder();
			gotoxy(55,17);
			printf ("BookName: %s", input.bookName);
			gotoxy(55,19);
			printf ("Book Id: %d",input.bookId);
			gotoxy(55,21);
			printf ("Author Name: %s", input.authName);
			gotoxy(55,23);
			printf ("Genre: %s",input.genre);
			gotoxy(55,25);
			printf ("Self Number: %d",input.self);
			gotoxy(55,27);
			printf ("Total Quantity: %d",input.quantTotal);
			gotoxy(55,29);
			printf ("Available Quantity: %d",input.avalQuantity);


			gotoxy(55,33);
			printf ("Enter N for next, Q to quit: (N|Q)");

			inputField();	
			scanf("%s",ans);
			gotoxy(Ilux+2,Iluy+1);
		}
	}

	if(flag){
	inputField();
	for(int i=0;i<50;i++)
	{
	gotoxy(55,20);
	printf("Your search did not match any documents.");
	}
	gotoxy(Ilux+2,Iluy+1);
	
	}
	// close file 
	fclose (file); 
}

void editBook(void){
	FILE *file;
	book input;
	char ch[stringMax],*p,c[10],value[stringMax],flag[5];
	int cr,buff;

	file = fopen ("unity/.bookDatabse", "rb+");
	if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }

	gotoxy(55,15);
	printf("Enter book name|Id|Author|Genre you want to edit ");
	inputField();
	fgets (ch,stringMax,stdin);
	if ((p=strchr(ch, '\n')) != NULL)
        *p = '\0';
	
	//fseek(stdin,0,SEEK_END);

	while(fread(&input, sizeof(struct book), 1, file)){
		 sprintf(c,"%d",input.bookId);
		if(!pstrcmp(c,ch)  || !pstrcmp(input.bookName,ch) || !pstrcmp(input.authName,ch) || !pstrcmp(input.genre,ch)){
			
			system("cls");
    		border();
			innerBorder();
			gotoxy(55,15);
			printf ("BookName: %s", input.bookName);
			gotoxy(55,17);
			printf ("Book Id: %d",input.bookId);
			gotoxy(55,19);
			printf ("Author Name: %s", input.authName);
			gotoxy(55,21);
			printf ("Genre: %s",input.genre);
			gotoxy(55,23);
			printf ("Self Number: %d",input.self);
			gotoxy(55,25);
			printf ("Total Quantity: %d",input.quantTotal);
			gotoxy(55,27);
			printf ("Available Quantity: %d",input.avalQuantity);

			gotoxy(55,30);
			printf ("Do you want to edit above displayed book?(Yes|No):");
			gotoxy(55,32);
			printf ("Enter N for next, Q to quit: (N|Q)");

			inputField();	
			scanf("%s",flag);
			gotoxy(Ilux+2,Iluy+1);

			if(flag[0]=='Q' || flag[0]=='q'){
				strcpy(flag,"No");
				break;
			}

			else if(flag[0]=='y' || flag[0]=='Y'){
				system("cls");
    			border();
				innerBorder();
				gotoxy(55,15);
				printf("Wanna edit book name?(Y/N): ");
				scanf("%s",flag);
				if(flag[0]=='y' || flag[0]=='Y'){
					gotoxy(55,16);
					printf("Enter new book name: ");
					while ((cr = getchar()) != '\n' && cr != EOF); 
					fgets (input.bookName,stringMax,stdin);
					if ((p=strchr(input.bookName, '\n')) != NULL)
            			*p = '\0';	
				}
				gotoxy(55,17);
				printf("Wanna edit author name?(Y/N): ");
				scanf("%s",flag);
				if(flag[0]=='y' || flag[0]=='Y'){
					gotoxy(55,18);
					printf("Enter new author name: ");
					while ((cr = getchar()) != '\n' && cr != EOF);
					fgets (input.authName,stringMax,stdin);
					if ((p=strchr(input.authName, '\n')) != NULL)
            			*p = '\0';
				}

				gotoxy(55,19);
				printf("Wanna edit genre name?(Y/N): ");
				scanf("%s",flag);
				if(flag[0]=='y' || flag[0]=='Y'){
					gotoxy(55,20);
					printf("Enter new genre: ");
					while ((cr = getchar()) != '\n' && cr != EOF);
					fgets (input.genre,stringMax,stdin);
					if((p=strchr(input.genre, '\n')) != NULL)
            			*p = '\0';
				}
				gotoxy(55,21);
				printf("Wanna edit self number?(Y/N): ");
				scanf("%s",flag);
				if(flag[0]=='y' || flag[0]=='Y'){
					gotoxy(55,22);
					printf("Enter new self number: ");
					while ((cr = getchar()) != '\n' && cr != EOF);
					//fgets (value,10,stdin);
					input.self= inputInt(); //atoi(value);
				}
				gotoxy(55,23);
				printf("Wanna edit quantity?(Y/N): ");
				scanf("%s",flag);
				if(flag[0]=='y' || flag[0]=='Y'){
					gotoxy(55,24);
					printf("Enter new Quantity: ");
					while ((cr = getchar()) != '\n' && cr != EOF);
					//fgets (value,10,stdin);
					buff=inputInt();
					if(buff>=input.quantTotal){
						input.avalQuantity+=buff-input.quantTotal;
						input.quantTotal=buff;
					}
					else if(input.quantTotal-(input.avalQuantity)<=buff){
						input.avalQuantity=buff-(input.quantTotal-input.avalQuantity);
						input.quantTotal=buff;
					}
					else{

						gotoxy(55,26);
						printf("Aborting: Illegal input for quantity");
						//printf("Caution: The Total Quantity can never be less then number of books allready in issue!");
						//printf("Aborting: Book edit!, Try agin, with correct quantity of book or after deissuing issued books! ");
						break;				
					}	
				}
				system("cls");
    			border();
				innerBorder();

				gotoxy(55,15);
				printf("Updated record is: "); 
				gotoxy(55,17);
				printf ("BookName: %s", input.bookName);
				gotoxy(55,19);
				printf ("Book Id: %d",input.bookId);
				gotoxy(55,21);
				printf ("Author Name: %s", input.authName);
				gotoxy(55,23);
				printf ("Genre: %s",input.genre);
				gotoxy(55,25);
				printf ("Self Number: %d",input.self);
				gotoxy(55,27);
				printf ("Total Quantity: %d",input.quantTotal);
				gotoxy(55,29);
				printf ("Available Quantity: %d",input.avalQuantity);
				fseek(file, -sizeof(book), SEEK_CUR);
				fwrite (&input, sizeof(book), 1, file);
				break;
			}	
		}
	}
	// close file 
	fclose (file);
}

void deleteBook(void){
	FILE *file,*restore;
	book input;
	char ch[stringMax],chh[stringMax],*p,c[10],value[stringMax],flag[5];
	int cr;

	file = fopen ("unity/.bookDatabse", "rb");
	if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }

	gotoxy(55,15);
	printf("Enter book name|Id|Author|Genre you want to delete:");
	inputField();
	fgets (ch,stringMax,stdin);
	if ((p=strchr(ch, '\n')) != NULL)
        *p = '\0';
	strcpy(flag,"TrI");

	while(fread(&input, sizeof(struct book), 1, file)){
		sprintf(c,"%d",input.bookId);
		if(!pstrcmp(c,ch)  || !pstrcmp(input.bookName,ch) || !pstrcmp(input.authName,ch) || !pstrcmp(input.genre,ch)){
			
			system("cls");
    		border();
			innerBorder();
			gotoxy(55,15);
			printf ("BookName: %s", input.bookName);
			gotoxy(55,17);
			printf ("Book Id: %d",input.bookId);
			gotoxy(55,19);
			printf ("Author Name: %s", input.authName);
			gotoxy(55,21);
			printf ("Genre: %s",input.genre);
			gotoxy(55,23);
			printf ("Self Number: %d",input.self);
			gotoxy(55,25);
			printf ("Total Quantity: %d",input.quantTotal);
			gotoxy(55,27);
			printf ("Available Quantity: %d",input.avalQuantity);

			gotoxy(55,31);
			printf ("Do you want to delete above displayed book?(Yes|No):");
			gotoxy(55,32);
			printf ("Enter N for next, Q to quit: (N|Q)");
			inputField();	
			scanf("%s",flag);
			gotoxy(Ilux+2,Iluy+1);

			if(flag[0]=='Q' || flag[0]=='q'){
				strcpy(flag,"No");
				break;
			}
			else if(flag[0]=='Y' || flag[0]=='y'){
				sprintf(chh,"%d",input.bookId);
				break;
			}	
		}
	}
	fclose (file);

	if(flag[0]=='Y' || flag[0]=='y'){

		file = fopen ("unity/.bookDatabse", "rb");
		if (file == NULL){ 
        	fprintf(stderr, "\nError opening file\n"); 
        	exit (1); 
    	}

		restore = fopen (".bookDatabse.restore", "ab+");
		if (restore == NULL){ 
        	fprintf(stderr, "\nError opening file\n"); 
        	exit (1); 
    	}

		while(fread(&input, sizeof(struct book), 1, file)){
			sprintf(c,"%d",input.bookId);
			if(pstrcmp(c,chh)){
				fwrite (&input, sizeof(book), 1, restore);   
    			if(!fwrite){  
        			fprintf(stderr, "\nError duplicating file\n"); 
        			exit (1);
				}
			}
		}
		fclose(restore);
    	fclose(file);
    	remove("unity/.bookDatabse");
    	//rename the file .bookDatabse.restore to original name
    	rename(".bookDatabse.restore", "unity/.bookDatabse");
	}

	else if(!strcmp(flag,"TrI")){
		inputField();
		for(int i=0;i<50;i++){
			gotoxy(55,20);
			printf("The book you wanna delete does't exist in library");
		}		 
		inputField();
	}
}

void issueBook(void){

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	FILE *file,*issuefile;
	book input,fcuk;
	borrower take;
	char ch[stringMax],*p,c[10],value[stringMax],flag[5];
	int cr,dd,mm,yyyy,flagAval=1;

	file = fopen ("unity/.bookDatabse", "ab+");
	if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }

	issuefile = fopen ("unity/.issueDatabse", "ab+");
    if (issuefile == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }

	gotoxy(55,15);
	printf("Enter book name|Id|Author|Genre you want to issue:");
	inputField();
	fgets (ch,stringMax,stdin);
	if ((p=strchr(ch, '\n')) != NULL)
        *p = '\0';
	
	//fseek(stdin,0,SEEK_END);

	while(fread(&input, sizeof(struct book), 1, file)){
		 sprintf(c,"%d",input.bookId);

		if(!pstrcmp(c,ch)  || !pstrcmp(input.bookName,ch) || !pstrcmp(input.authName,ch) || !pstrcmp(input.genre,ch)){
			printf("\n\nBookName: %s\nBook Id: %d\nAuthor Name: %s\nGenre: %s\nSelf Number: %d\nTotal Quantity: %d\nAvailable Quantity: %d\n", input.bookName,input.bookId, input.authName,input.genre,input.self,input.quantTotal,input.avalQuantity);
			
			flagAval=0;
			system("cls");
    		border();
			innerBorder();
			gotoxy(55,15);
			printf ("BookName: %s", input.bookName);
			gotoxy(55,17);
			printf ("Book Id: %d",input.bookId);
			gotoxy(55,19);
			printf ("Author Name: %s", input.authName);
			gotoxy(55,21);
			printf ("Genre: %s",input.genre);
			gotoxy(55,23);
			printf ("Self Number: %d",input.self);
			gotoxy(55,25);
			printf ("Total Quantity: %d",input.quantTotal);
			gotoxy(55,27);
			printf ("Available Quantity: %d",input.avalQuantity);

			gotoxy(55,31);
			printf ("Confirm The isue of above book, press (Yes|No):");
			gotoxy(55,32);
			printf ("Enter N for next, Q to quit: (N|Q)");
			inputField();
			scanf("%s",flag);

			if((flag[0]=='Y'|| flag[0]=='y') && input.avalQuantity!=0){
				
				//input.avalQuantity--;// decrese aval quantity		

				system("cls");
    			border();
				innerBorder();
				gotoxy(55,15);
				printf ("Enter the borrower's name: ");
				while ((cr = getchar()) != '\n' && cr != EOF);
				fgets (take.borrowerName,stringMax,stdin);
				if ((p=strchr(take.borrowerName, '\n')) != NULL)
            		*p = '\0';

				take.borrowerId=primaryborrower;
				primaryborrower++;

				gotoxy(55,17);
				printf ("Enter the number days for whisch this book is issued: ");
				//fgets (value,10,stdin);
				//cr=atoi(value);
				cr=inputInt();
				gotoxy(Ilux+2,Iluy+1);

				dd=tm.tm_mday;mm=tm.tm_mon;yyyy=tm.tm_year;
				addDate(&dd,&mm,&yyyy,cr);
				sprintf(take.dueDate," %d-%d-%d",dd,mm,yyyy);

				sprintf(take.returnDate,"Not yet returned!");

				sprintf(take.fineImposed,"Not applicable now!");

				sprintf(take.borrowingDate, " %d-%d-%d Time: %d:%d:%d",tm.tm_mday,tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
				
				take.bookId=input.bookId;

				strcpy(take.borrowedBook,input.bookName);

				fwrite (&take, sizeof(borrower), 1, issuefile);

				fclose(file);
				file = fopen ("unity/.bookDatabse", "rb+");
				if (file == NULL) { 
       		 		fprintf(stderr, "\nError opening file\n"); 
        			exit (1); 
    			}

				while(fread(&fcuk, sizeof(book), 1, file)){
					 printf("%d",fcuk.bookId);
					if(take.bookId==fcuk.bookId){
						fcuk.avalQuantity--;
						fseek(file, -sizeof(book), SEEK_CUR); // rewrite decrese quantity
						fwrite (&fcuk, sizeof(book), 1, file);
						break;
					}
				}
				break;
			}
			else if((flag[0]=='Q'|| flag[0]=='q'))
				break;
			else if((flag[0]=='Y'|| flag[0]=='y') && input.avalQuantity==0){
				inputField();
				for(int i=0;i<50;i++){
					gotoxy(55,20);
					printf("Sorry the intendend book is not present right now: ");
				}
				break;
			}
		}
	}
	if(flagAval){
		inputField();
		for(int i=0;i<50;i++){
			gotoxy(55,20);
			printf("Sorry the intendend book is not available in library:");
		}
	}
	fclose (file);
	fclose(issuefile);
}
 
void showIssuedData(void){
	FILE *file;
	borrower input;
	char ans[4]="n";

	file = fopen ("unity/.issueDatabse", "ab+");
    if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    } 

	while(fread(&input, sizeof(borrower), 1, file) && ((ans[0]=='n')||(ans[0]=='N')) ){
		 
		system("cls");
    	border();
		innerBorder();
		gotoxy(55,15);
		printf ("Borrower Name: %s", input.borrowerName);
		gotoxy(55,17);
		printf ("Borrower Id: %d",input.borrowerId);
		gotoxy(55,19);
		printf ("Book Name: %s", input.borrowedBook);
		gotoxy(55,21);
		printf("Book Id: %d",input.bookId);
		gotoxy(55,23);
		printf ("Date of Issue: %s",input.borrowingDate);
		gotoxy(55,25);
		printf ("Due Date: %s",input.dueDate);
		gotoxy(55,27);
		printf ("Return Date: %s",input.returnDate);
		gotoxy(55,29);
		printf ("Fine Imposed: %s",input.fineImposed);

		gotoxy(55,32);
		printf ("Enter N for next, Q to quit: (N|Q)");
		inputField();	
		scanf("%s",ans);
	}
	// close file 
	fclose (file); 
}

void collectBook(void){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	FILE *file;
	FILE *issuefile;
	book input;
	borrower take;
	char ch[stringMax],*p,c[10],value[stringMax]="Not applicable now!",flag[5]="n";
	int cr,flagAval=1;

	issuefile = fopen ("unity/.issueDatabse", "rb+");//"rt+");
    if (issuefile == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }

	gotoxy(55,15);
	printf("Enter borrower Id you want to collect from: ");
	inputField();
	fgets (ch,stringMax,stdin);
	if ((p=strchr(ch, '\n')) != NULL)
        *p = '\0';
	
	//fseek(stdin,0,SEEK_END);

	while(fread(&take, sizeof(borrower), 1, issuefile)){
		sprintf(c,"%d",take.borrowerId);
		if(!pstrcmp(c,ch) && !pstrcmp(take.fineImposed,value)){
			system("cls");
    		border();
			innerBorder();
			gotoxy(55,15);
			printf ("Borrower Name: %s", take.borrowerName);
			gotoxy(55,17);
			printf ("Borrower Id: %d",take.borrowerId);
			gotoxy(55,19);
			printf ("Book Name: %s", take.borrowedBook);
			gotoxy(55,21);
			printf("Book Id: %d",take.bookId);
			gotoxy(55,23);
			printf ("Date of Issue: %s",take.borrowingDate);
			gotoxy(55,25);
			printf ("Due Date: %s",take.dueDate);
			gotoxy(55,27);
			printf ("Return Date: %s",take.returnDate);
			gotoxy(55,29);
			printf ("Fine Imposed: %s",take.fineImposed);

			gotoxy(55,32);
			printf ("Confirm The collecton of above book from Borrower!(Y/N): ");
			inputField();	
			scanf("%s",flag);
			gotoxy(Ilux+2,Iluy+1);
			flagAval=0;
		}
	}
	fclose(issuefile);

	issuefile = fopen ("unity/.issueDatabse", "rb+");
    if (issuefile == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }

	while(fread(&take, sizeof(borrower), 1, issuefile)){
		 sprintf(c,"%d",take.borrowerId);
		
		if(!pstrcmp(c,ch) && !pstrcmp(take.fineImposed,value)){
			if(flag[0]=='Y'|| flag[0]=='y'){

				file = fopen ("unity/.bookDatabse", "rb+");
				if (file == NULL) { 
        			fprintf(stderr, "\nError opening file\n"); 
       			 	exit (1); 
   				}

				while(fread(&input, sizeof(book), 1, file)){
					if(input.bookId==take.bookId){
						input.avalQuantity+=1;
						fseek(file, -sizeof(book), SEEK_CUR);
						fwrite (&input, sizeof(book), 1, file);
						break;	
					}
				}
				// close file 
				fclose (file);

				gotoxy(55,33);
				printf ("Enter the fine imposed if any else enter 0: ");
				gotoxy(Ilux+2,Iluy+1);
				printf("    ");
				inputField();
				cr=inputInt();
				sprintf(take.fineImposed,"%d",cr);
				//fgets (value,10,stdin);
				//strcpy(take.fineImposed,value);

				sprintf(take.returnDate, " %d-%d-%d Time: %d:%d:%d",tm.tm_mday,tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
				fseek(issuefile, -sizeof(borrower), SEEK_CUR);
				fwrite (&take, sizeof(borrower), 1, issuefile);
				break;
			}

		}
	}
	if(flagAval){
		inputField();
		for(int i=0;i<50;i++){
			gotoxy(55,20);
			printf("Sorry the borrower don't exist/ No pending collect: ");
		}		 
		inputField();		
	}
	// close file 
	fclose(issuefile);
}

int main (){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	sprintf(timeBeg,"Beginning| Date: %d-%d-%d Time: %d:%d:%d",tm.tm_mday,tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
	
	system("cls");
	ShowWindow( GetConsoleWindow() , SW_MAXIMIZE); // open windows in full screen mode
  

	int check; 
    char* dirname = "unity"; 
	if(!DirectoryExists("unity")){
		 printf("\nCreting Directory!");
    	check = mkdir(dirname); 
    	if (check) {
        	 printf("\nUnable to create directory\n"); 
        	exit(1); 
    	} 
	}
  
	primaryInitalize();
	int x=8,cr;
	while (x){
		system("cls");
    	menu();
    	border();
		innerBorder();
    	inputField();
		gotoxy(Ilux+2,Iluy+1);
		scanf("%d",&x);

		while ((cr = getchar()) != '\n' && cr != EOF);

			if(x==1){
				system("cls");
    			border();
				innerBorder();
				addBooks(); 
				Sleep(1000);
               	 
			} 

       		if(x==2){
				system("cls");
    			border();
				innerBorder();
				deleteBook(); 
				Sleep(1000);
				system("cls");
    			menu();
    			border();
				innerBorder();
    			inputField();
				gotoxy(Ilux+2,Iluy+1);
				
			} 

       		if(x==3){
				system("cls");
    			border();
				innerBorder();
				searchBook(); 
				Sleep(1000);
				system("cls");
    			menu();
    			border();
				innerBorder();
    			inputField();
				gotoxy(Ilux+2,Iluy+1);
			} 

       		if(x==4){
				system("cls");
    			border();
				innerBorder();
				issueBook();
				Sleep(1000);
				system("cls");
    			menu();
    			border();
				innerBorder();
    			inputField();
				gotoxy(Ilux+2,Iluy+1);
			}

       		if(x==5){
				system("cls");
    			border();
				innerBorder();
				collectBook(); 
				Sleep(1000);
				system("cls");
    			menu();
    			border();
				innerBorder();
    			inputField();
				gotoxy(Ilux+2,Iluy+1);
			}	

       		if(x==6){
				system("cls");
    			border();
				innerBorder();
				editBook();
				system("cls");
    			menu();
    			border();
				innerBorder();
    			inputField();
				gotoxy(Ilux+2,Iluy+1);
				
			}

       		if(x==7){
				system("cls");
    			border();
				innerBorder();
				viewBooks();
				Sleep(1000);
				system("cls");
    			menu();
    			border();
				innerBorder();
    			inputField();
				gotoxy(Ilux+2,Iluy+1);
				
			}
	
       		if(x==8){
				system("cls");
    			border();
				innerBorder();
				showIssuedData(); 
				system("cls");
    			menu();
    			border();
				innerBorder();
    			inputField();
				gotoxy(Ilux+2,Iluy+1);
			}
			
			if(x==9){
				x=0;
			}	  

       		else{
				system("cls");
			}	
	}
	primaryRecord();
	surveillance();
	return 0; 
} 
