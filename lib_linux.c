#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>			//for sleep 
#include <netdb.h>			//host data 
#include <arpa/inet.h>		//for ip 
#include <sys/utsname.h>	//for os info
#include <dirent.h>			//for dir open close
#include <sys/stat.h>		//for mkdir
//#include <sys/types.h>
#include <wchar.h>		//for  unicode	
#include <locale.h>		//foir unicode

#define stringMax 50

#define Menu 15

#define Ilux 94     //input filed left up corner
#define Iluy 35
#define Ildx 94     //input filed left down corner
#define Ildy 37
#define Irux 140    //input filed right up corner
#define Iruy 35

#define blux 30     //book filed left up corner
#define bluy 10
#define bldx 30     //book filed left down corner
#define bldy 34
#define brux 109    //book filed right up corner
#define bruy 10

int primarybook;
int primaryborrower;
char timeBeg[stringMax];

typedef struct primaryKey{
    int bookId;
    int borrowerId;
} primaryKey;

typedef struct surveil{
	char hostIp[stringMax];
	char osName[stringMax];
	char hardware[stringMax];
	char kernal[stringMax];
	char hostName[stringMax];
	char osVersion[stringMax+50];
	char startTime[stringMax];
	char endTime[stringMax];
} surveil;


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

int pstrcmp(char *,char *); 				//done
int dateOp(int *dd,int *,int *,int *,int);	//done
int inputInt(void);							//done
void addDate(int *,int *,int *,int);		//done
void menu(void);							//done
void border(void);							//done
void innerBorder(void);						//done
void gotoxy(int x,int y);					//done
void addBooks(void); 						//done
void inputField(void);						//done
void viewBooks(void);						//done
void searchBook(void);						//done
void editBook(void);						//done
void deleteBook(void);						//done
void issueBook(void);						//done	
void showIssuedData(void);					//done
void collectBook(void);						//done
void primaryInitalize(void);				//done
void primaryRecord(void);					//done
void surveillance(void);					//done


void gotoxy(int x,int y){
 printf("%c[%d;%df",0x1B,y,x);
}

void innerBorder(){
    gotoxy(blux-15,bluy+1);
    gotoxy(blux,bluy);
    printf("═══════════════════════════════════════════════════════════════════════════════");
    gotoxy(blux-1,bluy);
    printf("╔");
    for(int i=bluy+1;i<bldy;i++){
        gotoxy(blux-1,i);
        printf("║");
    }
    gotoxy(blux-1,bldy);
    printf("╚");
    gotoxy(bldx,bldy);
    printf("═══════════════════════════════════════════════════════════════════════════════");
    gotoxy(brux,bruy);
    printf("╗");
    for(int i=bluy+1;i<bldy;i++){
        gotoxy(brux,i);
        printf("║");
    }   
    gotoxy(brux,bldy);
    printf("╝");
    gotoxy(blux+2,bluy+1);
}

void inputField(){
    gotoxy(Ilux-15,Iluy+1);
    printf("Input Field");
    gotoxy(Ilux,Iluy);
    printf("══════════════════════════════════════════════");
    gotoxy(Ilux-1,Iluy);
    printf("╔");
    gotoxy(Ilux-1,Iluy+1);
    printf("║");
    gotoxy(Ilux-1,Iluy+2);
    printf("╚");
    gotoxy(Ildx,Ildy);
    printf("══════════════════════════════════════════════");
    gotoxy(Irux,Iruy);
    printf("╗");
    gotoxy(Irux,Iruy+1);
    printf("║");    
    gotoxy(Irux,Iruy+2);
    printf("╝");
    gotoxy(Ilux+2,Iluy+1);
}

void border(){
    setlocale(LC_CTYPE, "");
    wchar_t star = 0x25A7;
    gotoxy(0,0);
    for(int i=0;i<=70;i++){ //top
        printf("%lc ", star);
    }
    
    for(int i=0;i<=35;i++){ //left
        printf("%lc\n", star);
    }

   for(int i=0;i<=70;i++){  //end
        printf("%lc ", star);
    }

    gotoxy(3,5);
    for(int i=0;i<=70;i+=1){    //top2
        printf("%lc ", star);
    }

    for(int i=2;i<=40;i+=1){    //right
        gotoxy(141,i);
        printf("%lc", star);
    }

    gotoxy(60,3);
    printf("Library Management System");
    gotoxy(5,35);
}

void menu(){
    setlocale(LC_CTYPE, "");
    wchar_t star = 0x26CE;
    gotoxy(55,Menu);
    printf("%lc%lc 1. Add Books   ",star,star);
    gotoxy(55,Menu+2);
    printf("%lc%lc 2. Delete books",star,star);
    gotoxy(55,Menu+4);
    printf("%lc%lc 3. Search Books",star,star);
    gotoxy(55,Menu+6);
    printf("%lc%lc 4. Issue Books",star,star);
    gotoxy(55,Menu+8);
    printf("%lc%lc 5. Collect Book",star,star);
    gotoxy(55,Menu+10);
    printf("%lc%lc 6. Edit Book's Record",star,star);
    gotoxy(55,Menu+12);
    printf("%lc%lc 7. View Book list",star,star);
    gotoxy(55,Menu+14);
    printf("%lc%lc 8. View Issue Records",star,star);
    gotoxy(55,Menu+16);
    printf("%lc%lc 9. Close Application",star,star);
    gotoxy(55,Menu+18);
}

void surveillance(void){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	surveil sir;
	char hostbuffer[256]; 
	char *IPbuffer; 
	struct hostent *host_entry; 
	int hostname; 

    int ret;
    struct utsname buf;
    ret = uname(&buf);

	// To retrieve hostname 
	hostname = gethostname(hostbuffer, sizeof(hostbuffer)); 
    // To retrieve host information 
	host_entry = gethostbyname(hostbuffer); 
    // To convert an Internet network 
	// address into ASCII string 
	IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); 

	sprintf(sir.hostIp,"Host IP: %s", IPbuffer); 

    if(!ret){
        sprintf(sir.osName,"OperatingSystem name: %s",buf.sysname);
        sprintf(sir.hostName,"Node(Host) name: %s",buf.nodename);
        sprintf(sir.kernal,"Kernel Release Version: %s",buf.release);
        sprintf(sir.osVersion,"OS Version: %s",buf.version);
        sprintf(sir.hardware,"Hardware: %s",buf.machine);
    }
    else {
        printf("Error in the System call");
        exit(1);
    }  	

	strcpy(sir.startTime,timeBeg);
	sprintf(sir.endTime,"End| Date: %d-%d-%d Time: %d:%d:%d",tm.tm_mday,tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

	FILE *file; 
    file = fopen ("unity/.usrBkp", "a+");
    if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    } 
	fwrite (&sir, sizeof(surveil), 1, file);
    fclose (file); 
}

int inputInt(){
    char term; int num;
    x:
    if(scanf("%d%c", &num, &term) != 2 || (term != ' ' && term!='\n') || num<0){
    printf("Invalid Input Try again");
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
 
    file = fopen ("unity/.primaryRecord", "r");
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
 
    file = fopen ("unity/.primaryRecord", "w+");
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
    file = fopen ("unity/.bookDatabse", "a+");
    if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    } 
	
	//printf("Enter the no of entry you want to do: ");
	//a=inputInt();
	a=1;
	gotoxy(55,8);
    printf("ENTER THE BOOK RECORD");
	
	while(a){
		
		gotoxy(40,15);
		printf("Enter book name: ");
		fgets (add.bookName,stringMax,stdin);
		if ((p=strchr(add.bookName, '\n')) != NULL)
            *p = '\0';	

		add.bookId=primarybook;
		primarybook++;


		gotoxy(40,17);
		printf("Enter author name: ");
		fgets (add.authName,stringMax,stdin);
		if ((p=strchr(add.authName, '\n')) != NULL)
            *p = '\0';
		
		gotoxy(40,19);
		printf("Enter genre: ");
		fgets (add.genre,stringMax,stdin);
		if ((p=strchr(add.genre, '\n')) != NULL)
            *p = '\0';
		
		gotoxy(40,21);
		printf("Enter self number: ");
		//fgets (value,10,stdin);
		add.self=inputInt();//atoi(value);

		gotoxy(40,23);
		printf("Enter Quantity: ");
		//fgets (value,10,stdin);
		add.quantTotal=inputInt();//atoi(value);
		add.avalQuantity=add.quantTotal;//atoi(value);

		gotoxy(40,27);
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
	
	
	file = fopen ("unity/.bookDatabse", "r+");
    if (file == NULL) { 
        fprintf(stderr, "\nError opend file\n"); 
        exit (1); 
    } 

	while(fread(&input, sizeof(struct book), 1, file) && ((ans[0]=='n')||(ans[0]=='N'))){
		
		printf("\033[2J");
    	border();
		innerBorder();

		gotoxy(40,15);
		printf ("Book Name: %s", input.bookName);
		gotoxy(40,17);
		printf ("Book Id: %d",input.bookId);
		gotoxy(40,19);
		printf ("Author Name: %s", input.authName);
		gotoxy(40,21);
		printf ("Genre: %s",input.genre);
		gotoxy(40,23);
		printf ("Self Number: %d",input.self);
		gotoxy(40,25);
		printf ("Total Quantity: %d",input.quantTotal);
		gotoxy(40,27);
		printf ("Available Quantity: %d",input.avalQuantity);

		gotoxy(40,30);
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

	file = fopen ("unity/.bookDatabse", "a+");
	if (file == NULL) { 
        fprintf(stderr, "\nError opend file\n"); 
        exit (1); 
    }

	gotoxy(40,15);
	printf("Enter book Name|Id|Author|Genre: ");
	inputField();
	fgets (ch,stringMax,stdin);
	if ((p=strchr(ch, '\n')) != NULL){
        *p = '\0';
	}

	while(fread(&input, sizeof(struct book), 1, file) && ((ans[0]=='n')||(ans[0]=='N')) ){
		 
		sprintf(c,"%d",input.bookId);
		if(!pstrcmp(c,ch)  || !pstrcmp(input.bookName,ch) || (!pstrcmp(input.genre,ch)) || !pstrcmp(input.authName,ch)  ){
			flag=0;
			printf("\033[2J");
    		border();
			innerBorder();

			gotoxy(40,17);
			printf ("Book Name: %s", input.bookName);
			gotoxy(40,19);
			printf ("Book Id: %d",input.bookId);
			gotoxy(40,21);
			printf ("Author Name: %s", input.authName);
			gotoxy(40,23);
			printf ("Genre: %s",input.genre);
			gotoxy(40,25);
			printf ("Self Number: %d",input.self);
			gotoxy(40,27);
			printf ("Total Quantity: %d",input.quantTotal);
			gotoxy(40,29);
			printf ("Available Quantity: %d",input.avalQuantity);

			gotoxy(40,33);
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
	gotoxy(40,20);
	printf("Your search did not match any documents.");
	}
	gotoxy(Ilux+2,Iluy+1);
	
	}
	
	fclose (file); 
}

void editBook(void){
	FILE *file;
	book input;
	char ch[stringMax],*p,c[10],value[stringMax],flag[5];
	int cr,buff;

	file = fopen ("unity/.bookDatabse", "r+");
	if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }

	gotoxy(40,15);
	printf("Enter book Name|Id|Author|Genre you want to edit ");

	inputField();
	fgets (ch,stringMax,stdin);
	if ((p=strchr(ch, '\n')) != NULL)
        *p = '\0';
	

	while(fread(&input, sizeof(struct book), 1, file)){
		 sprintf(c,"%d",input.bookId);
		if(!pstrcmp(c,ch)  || !pstrcmp(input.bookName,ch) || !pstrcmp(input.authName,ch) || !pstrcmp(input.genre,ch)){
			
			printf("\033[2J");
    		border();
			innerBorder();

			gotoxy(40,15);
			printf ("Book Name: %s", input.bookName);
			gotoxy(40,17);
			printf ("Book Id: %d",input.bookId);
			gotoxy(40,19);
			printf ("Author Name: %s", input.authName);
			gotoxy(40,21);
			printf ("Genre: %s",input.genre);
			gotoxy(40,23);
			printf ("Self Number: %d",input.self);
			gotoxy(40,25);
			printf ("Total Quantity: %d",input.quantTotal);
			gotoxy(40,27);
			printf ("Available Quantity: %d",input.avalQuantity);

			gotoxy(40,30);
			printf ("Do you want to edit above displayed book?(Yes|No):");
			gotoxy(40,32);
			printf ("Enter N for next, Q to quit: (N|Q)");

			inputField();	
			scanf("%s",flag);
			gotoxy(Ilux+2,Iluy+1);

			if(flag[0]=='Q' || flag[0]=='q'){
				strcpy(flag,"No");
				break;
			}

			else if(flag[0]=='y' || flag[0]=='Y'){
				printf("\033[2J");
    			border();
				innerBorder();
				gotoxy(40,15);
				printf("Wanna edit book name?(Y/N): ");
				scanf("%s",flag);
				if(flag[0]=='y' || flag[0]=='Y'){
					gotoxy(40,16);
					printf("Enter new book name: ");
					while ((cr = getchar()) != '\n' && cr != EOF); 
					fgets (input.bookName,stringMax,stdin);
					if ((p=strchr(input.bookName, '\n')) != NULL)
            			*p = '\0';	
				}
				gotoxy(40,17);
				printf("Wanna edit author name?(Y/N): ");
				scanf("%s",flag);
				if(flag[0]=='y' || flag[0]=='Y'){
					gotoxy(40,18);
					printf("Enter new author name: ");
					while ((cr = getchar()) != '\n' && cr != EOF);
					fgets (input.authName,stringMax,stdin);
					if ((p=strchr(input.authName, '\n')) != NULL)
            			*p = '\0';
				}

				gotoxy(40,19);
				printf("Wanna edit genre name?(Y/N): ");
				scanf("%s",flag);
				if(flag[0]=='y' || flag[0]=='Y'){
					gotoxy(40,20);
					printf("Enter new genre: ");
					while ((cr = getchar()) != '\n' && cr != EOF);
					fgets (input.genre,stringMax,stdin);
					if((p=strchr(input.genre, '\n')) != NULL)
            			*p = '\0';
				}
				gotoxy(40,21);
				printf("Wanna edit self number?(Y/N): ");
				scanf("%s",flag);
				if(flag[0]=='y' || flag[0]=='Y'){
					gotoxy(40,22);
					printf("Enter new self number: ");
					while ((cr = getchar()) != '\n' && cr != EOF);
					//fgets (value,10,stdin);
					input.self= inputInt(); //atoi(value);
				}
				gotoxy(40,23);
				printf("Wanna edit quantity?(Y/N): ");
				scanf("%s",flag);
				if(flag[0]=='y' || flag[0]=='Y'){
					gotoxy(40,24);
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

						gotoxy(40,26);
						printf("Aborting: Illegal input for quantity");
						//printf("Caution: The Total Quantity can never be less then number of books allready in issue!");
						//printf("Aborting: Book edit!, Try agin, with correct quantity of book or after deissuing issued books! ");
						break;				
					}	
				}
				printf("\033[2J");
    			border();
				innerBorder();

				gotoxy(40,15);
				printf("Updated record is: "); 
				gotoxy(40,17);
				printf ("Book Name: %s", input.bookName);
				gotoxy(40,19);
				printf ("Book Id: %d",input.bookId);
				gotoxy(40,21);
				printf ("Author Name: %s", input.authName);
				gotoxy(40,23);
				printf ("Genre: %s",input.genre);
				gotoxy(40,25);
				printf ("Self Number: %d",input.self);
				gotoxy(40,27);
				printf ("Total Quantity: %d",input.quantTotal);
				gotoxy(40,29);
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

	file = fopen ("unity/.bookDatabse", "r");
	if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }

	gotoxy(40,15);
	printf("Enter book Name|Id|Author|Genre you want to delete:");
	inputField();
	fgets (ch,stringMax,stdin);
	if ((p=strchr(ch, '\n')) != NULL)
        *p = '\0';
	strcpy(flag,"TrI");

	while(fread(&input, sizeof(struct book), 1, file)){
		sprintf(c,"%d",input.bookId);
		if(!pstrcmp(c,ch)  || !pstrcmp(input.bookName,ch) || !pstrcmp(input.authName,ch) || !pstrcmp(input.genre,ch)){
			printf("\033[2J");
    		border();
			innerBorder();

			gotoxy(40,15);
			printf ("Book Name: %s", input.bookName);
			gotoxy(40,17);
			printf ("Book Id: %d",input.bookId);
			gotoxy(40,19);
			printf ("Author Name: %s", input.authName);
			gotoxy(40,21);
			printf ("Genre: %s",input.genre);
			gotoxy(40,23);
			printf ("Self Number: %d",input.self);
			gotoxy(40,25);
			printf ("Total Quantity: %d",input.quantTotal);
			gotoxy(40,27);
			printf ("Available Quantity: %d",input.avalQuantity);


			gotoxy(40,31);
			printf ("Do you want to delete above displayed book?(Yes|No):");
			gotoxy(40,32);
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

		file = fopen ("unity/.bookDatabse", "r");
		if (file == NULL){ 
        	fprintf(stderr, "\nError opening file\n"); 
        	exit (1); 
    	}

		restore = fopen (".bookDatabse.restore", "a+");
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
			gotoxy(40,20);
			printf("The book you wanna delete does't exist in library");
		}		 
		inputField();
	}
}

void issueBook(void){

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	FILE *file,*issuefile;
	book input;
	borrower take;
	char ch[stringMax],*p,c[10],value[stringMax],flag[5];
	int cr,dd,mm,yyyy,flagAval=1;

	file = fopen ("unity/.bookDatabse", "r+");
	if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }

	issuefile = fopen ("unity/.issueDatabse", "a+");
    if (issuefile == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }
	gotoxy(40,15);
	printf("Enter book Name|Id|Author|Genre you want to issue:");
	inputField();
	fgets (ch,stringMax,stdin);
	if ((p=strchr(ch, '\n')) != NULL)
        *p = '\0';
	
	//fseek(stdin,0,SEEK_END);

	while(fread(&input, sizeof(struct book), 1, file)){
		 sprintf(c,"%d",input.bookId);

		if(!pstrcmp(c,ch)  || !pstrcmp(input.bookName,ch) || !pstrcmp(input.authName,ch) || !pstrcmp(input.genre,ch)){
			
			flagAval=0;
			printf("\033[2J");
    		border();
			innerBorder();
			gotoxy(40,15);
			printf ("Book Name: %s", input.bookName);
			gotoxy(40,17);
			printf ("Book Id: %d",input.bookId);
			gotoxy(40,19);
			printf ("Author Name: %s", input.authName);
			gotoxy(40,21);
			printf ("Genre: %s",input.genre);
			gotoxy(40,23);
			printf ("Self Number: %d",input.self);
			gotoxy(40,25);
			printf ("Total Quantity: %d",input.quantTotal);
			gotoxy(40,27);
			printf ("Available Quantity: %d",input.avalQuantity);


			gotoxy(40,31);
			printf ("Confirm The isue of above book, press (Yes|No):");
			gotoxy(40,32);
			printf ("Enter N for next, Q to quit: (N|Q)");
			inputField();	
			scanf("%s",flag);

			if((flag[0]=='Y'|| flag[0]=='y') && input.avalQuantity!=0){
				
				input.avalQuantity--;// decrese aval quantity		
				printf("\033[2J");
    			border();
				innerBorder();
				gotoxy(40,15);
				printf ("Enter the borrower's name: ");
				while ((cr = getchar()) != '\n' && cr != EOF);
				fgets (take.borrowerName,stringMax,stdin);
				if ((p=strchr(take.borrowerName, '\n')) != NULL)
            		*p = '\0';

				take.borrowerId=primaryborrower;
				primaryborrower++;

				gotoxy(40,17);
				printf ("Enter the number days for whisch this book is issued: ");
				//fgets (value,10,stdin);
				//cr=atoi(value);
				cr=inputInt();
				dd=tm.tm_mday;mm=tm.tm_mon;yyyy=tm.tm_year;
				addDate(&dd,&mm,&yyyy,cr);
				sprintf(take.dueDate," %d-%d-%d",dd,mm,yyyy);

				sprintf(take.returnDate,"Not yet returned!");

				sprintf(take.fineImposed,"Not applicable now!");

				sprintf(take.borrowingDate, " %d-%d-%d Time: %d:%d:%d",tm.tm_mday,tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
				
				take.bookId=input.bookId;

				strcpy(take.borrowedBook,input.bookName);

				fwrite (&take, sizeof(borrower), 1, issuefile);

				fseek(file, -sizeof(book), SEEK_CUR); // rewrite decrese quantity
				fwrite (&input, sizeof(book), 1, file);
				break;
			}
			else if((flag[0]=='Q'|| flag[0]=='q'))
				break;
			else if((flag[0]=='Y'|| flag[0]=='y') && input.avalQuantity==0){
				inputField();
				for(int i=0;i<50;i++){
					gotoxy(40,20);
					printf("Sorry the intendend book is not present right now: ");
				}
				break;
			}
		}
	}
	if(flagAval){
		inputField();
		for(int i=0;i<50;i++){
			gotoxy(40,20);
			printf("Sorry the intendend book is not available in library:");
		}
	}
	// close file 
	fclose (file);
	fclose(issuefile);
}

void showIssuedData(void){
	FILE *file;
	borrower input;
	char ans[4]="n";
	file = fopen ("unity/.issueDatabse", "a+");
    if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    } 

	while(fread(&input, sizeof(borrower), 1, file) && ((ans[0]=='n')||(ans[0]=='N'))){
		 
		printf("\033[2J");
    	border();
		innerBorder();
		gotoxy(40,15);
		printf ("Borrower Name: %s", input.borrowerName);
		gotoxy(40,17);
		printf ("Borrower Id: %d",input.borrowerId);
		gotoxy(40,19);
		printf ("Book Name: %s", input.borrowedBook);
		gotoxy(40,21);
		printf("Book Id: %d",input.bookId);
		gotoxy(40,23);
		printf ("Date of Issue: %s",input.borrowingDate);
		gotoxy(40,25);
		printf ("Due Date: %s",input.dueDate);
		gotoxy(40,27);
		printf ("Return Date: %s",input.returnDate);
		gotoxy(40,29);
		printf ("Fine Imposed: %s",input.fineImposed);

		gotoxy(40,32);
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

	FILE *file,*issuefile;
	book input;
	borrower take;
	char ch[stringMax],*p,c[10],value[stringMax],flag[5];
	int cr,flagAval=1;

	file = fopen ("unity/.bookDatabse", "r+");
	if (file == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }

	issuefile = fopen ("unity/.issueDatabse", "r+");
    if (issuefile == NULL) { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }
	gotoxy(40,15);
	printf("Enter borrower Id you want to collect from: ");
	inputField();
	fgets (ch,stringMax,stdin);
	if ((p=strchr(ch, '\n')) != NULL)
        *p = '\0';
	
	//fseek(stdin,0,SEEK_END);

	while(fread(&take, sizeof(borrower), 1, issuefile)){
		 sprintf(c,"%d",take.borrowerId);

		if(!pstrcmp(c,ch) && !pstrcmp(take.returnDate,"Not yet returned!")){

			printf("\033[2J");
    		border();
			innerBorder();

			gotoxy(40,15);
			printf ("Borrower Name: %s", take.borrowerName);
			gotoxy(40,17);
			printf ("Borrower Id: %d",take.borrowerId);
			gotoxy(40,19);
			printf ("Book Name: %s", take.borrowedBook);
			gotoxy(40,21);
			printf("Book Id: %d",take.bookId);
			gotoxy(40,23);
			printf ("Date of Issue: %s",take.borrowingDate);
			gotoxy(40,25);
			printf ("Due Date: %s",take.dueDate);
			gotoxy(40,27);
			printf ("Return Date: %s",take.returnDate);
			gotoxy(40,29);
			printf ("Fine Imposed: %s",take.fineImposed);


			gotoxy(40,32);
			printf ("Confirm The collecton of above book from Borrower!(Y/N): ");
			inputField();	
			scanf("%s",flag);
			gotoxy(Ilux+2,Iluy+1);

			//set flag that book is present in database
			flagAval=0;
			if(flag[0]=='Y'|| flag[0]=='y'){

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
				gotoxy(40,33);
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
			gotoxy(40,20);
			printf("Sorry the borrower don't exist/ No pending collect: ");
		}		 
		inputField();		
	}
	// close file 
	fclose (file);
	fclose(issuefile);
}

int main (){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	sprintf(timeBeg,"Beginning| Date: %d-%d-%d Time: %d:%d:%d",tm.tm_mday,tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
	primaryInitalize();

	DIR* dir = opendir("unity");
	if (dir){
    	/* Directory exists. */
    	closedir(dir);
	}
	else{
		mkdir("unity", 0777);
	    /* Directory does not exist. */
	}

	int x=8,cr;
	while (x){
		printf("\033[2J");
    	menu();
    	border();
		innerBorder();
    	inputField();
		gotoxy(Ilux+2,Iluy+1);
		scanf("%d",&x);

		while ((cr = getchar()) != '\n' && cr != EOF);

		printf("\033[2J");
    	border();
		innerBorder();

		if(x==1)
			addBooks(); 

  		else if(x==2)
			deleteBook(); 	

       	else if(x==3)
			searchBook(); 

       	else if(x==4)
			issueBook();

       	else if(x==5)
			collectBook(); 

    	else if(x==6)
			editBook();			

       	else if(x==7)
			viewBooks();
	
    	else if(x==8)
			showIssuedData(); 
			
		else if(x==9)
			x=0;
		
		usleep(600000); 
	} 
	primaryRecord();
	surveillance();
	printf("\033[2J");
	gotoxy(0,0);
	return 0; 
} 