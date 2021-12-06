#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>

using namespace std;

char B[40];
char M[300][4];
char IR[4];
char R[4];
int PTE,IC,SI,PI,TI;
int RNUM[30] ;
int C;
int ADDRMAP(int);
void STARTEXECUTION();
void EXE_PROGRAM();
void MOS();
void TERMINATE(int);
void WRITE();
void READ();

struct PCB
{
    int TLL;
    int TTL;
    int JID;
    int TTC;
    int LLC;
};

FILE *infile,*outfile;

struct PCB *pcb;

void INIT_PCB()  //init_PCB()
{
	int a,b,c,d;
	//printf("------INSIDE INIT_PCB Method------\n");
    pcb = (struct PCB*)malloc(sizeof(struct PCB));
    pcb->LLC = 0;
    pcb->TTC = 0;
    a=(int)B[4]-48;
    b=(int)B[5]-48;
    c=(int)B[6]-48;
    d=(int)B[7]-48;
    pcb->JID=(a*1000+b*100+c*10+d);

    printf("Job Id JId :  %d\n",pcb->JID);
    a=(int)B[8]-48;
    b=(int)B[9]-48;
    c=(int)B[10]-48;
    d=(int)B[11]-48;
    pcb->TTL=(a*1000+b*100+c*10+d); //for convert string to int

    printf("Total Time Limit TTL : %d\n",pcb->TTL);
    a=(int)B[12]-48;
    b=(int)B[13]-48;
    c=(int)B[14]-48;
    d=(int)B[15]-48;
    pcb->TLL=(a*1000+b*100+c*10+d);
    printf("Total Line Limit TLL: %d\n",pcb->TLL);
}

void INIT()           //init()
{
	//printf("\n------INSIDE INIT Method------\n");
    int i, j;
    for(i=0;i<300;i++)
    {
        for(j=0;j<4;j++)
        {
            M[i][j]='\0';
        }
    }
    for(i=0;i<4;i++)
    {
        IR[i]=R[i]='\0';
    }
    PTE = 0;
    IC = 0;
    SI = 0;
    PI = 0;
    TI = 0;
    for(i=0;i<30;i++)
	{
    	RNUM[i]='\0';
    }
}

int ALLOCATE()      //allocate
{
	//printf("------INSIDE ALLOCATE Method------\n");
    int n;
    n = rand()%30;
    while(RNUM[n]==1)
	{
        n = (rand()%30);
    }
    RNUM[n]=1;
	printf("Random number generated: %d\n",n);
    return n;
}

void LOAD()     //load
{
	//printf("------INSIDE LOAD Method------\n");
    int i,k;
    char temp;
    while(getc(infile)!=EOF)
    {
        printf("\n\nPAGE TABLE Pointer: %d\n",PTE );
        fseek(infile,-1,SEEK_CUR);
        memset(B,0,40);
		fgets(B,41,infile);
		printf("Buffer array contains :  ");
		for(i=0;i<40;i++)
		{
			printf("%c",B[i]);
        }
		printf("\n");

        if(B[0] == '$' && B[1] == 'A' && B[2] =='M' && B[3] == 'J')
        {
            printf("\nIN $AMJ \n");
            INIT();
            INIT_PCB();
            PTE = ALLOCATE();
            PTE = 10*PTE;
            k=PTE;
            printf("PTE = %d\n",PTE);
            for(int j=0;j<10;j++)
            {
            	M[k][0]='0';
            	M[k][1]='0';
				M[k][2]='#';
				M[k][3]='#';
				k++;
			}
			k=PTE;
			//printf("------Initialied Page Table------\n");
			printf("\t [0] [1] [2] [3]\n");
			for(i=0;i<10;i++)
			{
				printf("M[%d]\t  %c   %c   %c   %c \n",k,M[k][0],M[k][1],M[k][2],M[k][3]);
				k++;
			}
			k=PTE;
        }
        else if(B[0] == '$' && B[1] == 'D' && B[2] =='T' && B[3] == 'A')
        {
            printf("\nIn $DTA\n");
            STARTEXECUTION();
	    }
        else if(B[0] == '$' && B[1] == 'E' && B[2] =='N' && B[3] == 'D')
        {
            printf("\nIn $END \nPROCESS ENDED...\n\n");
            continue;
        }
        else
        {
            //printf("------PROGRAM CARD FOUND------\n");
            int index = ALLOCATE();
            int q=index/10;
            int r=index%10;

            printf("Updating Page table..\n");   //updating page table
	       		M[k][0]='1';
				char a=q+48;

				M[k][2]=a;
				a=r+48;
				M[k][3]=a;
				printf("\t [0] [1] [2] [3]\n");
				for(i=0;i<10;i++)
				{
					printf("M[%d]\t  %c   %c   %c   %c \n",k,M[k][0],M[k][1],M[k][2],M[k][3]);
					k++;
				}
			index=index*10;
			printf("\t [0] [1] [2] [3]\n");
			int j=0;
			while(j<40 && B[j]!='\n')
			{
				printf("M[%d]: ",index);
				for(i=0; i<4 && B[j]!='\n'; i++)
				{
					M[index][i]=B[j++];
					printf("  %c ",M[index][i]);
				}
				index++;
				printf("\n");
			}
			k++;
		}
    }
}


void STARTEXECUTION()    //start execution
{
	printf("\nIn start\n");
    IC=0;
    EXE_PROGRAM();
}

void EXE_PROGRAM()    //Execute user program
{
	//printf("inside EXE_PROGRAM\n");
    int i;
    int ra = ADDRMAP(IC);
    if(PI==0)
	{
		IR[0]=M[ra][0];
		if(M[ra][1]!='\n' && M[ra][1]!='$' && IR[0]!='H')
		{
			IR[1]=M[ra][1];
			IR[2]=M[ra][2];
			IR[3]=M[ra][3];

			printf("\nIR=%s\n",IR);
			//fprintf(outfile, "%S",IR);
			IC++;
			int m=(int)IR[3] -48;
			int n=(int)IR[2] -48;
			if(m<10 && n<10 && m>=0 && n>=0)
			{
				n=(n*10)+m;
				ra=ADDRMAP(n);
			}
			else
			{
                PI = 2;
			}
            if(PI==0)
            {
                if(IR[0]=='L'&&IR[1]=='R')
                {
                    printf("\nIn LR\n");
                    for(i=0;i<4;i++)
                    {
                        R[i]=M[ra][i];
                    }
                }
				else if(IR[0]=='S'&&IR[1]=='R')
                {
                    printf("\nIn SR\n");
                    int i;
                    for(i=0;i<4;i++){
                    	M[ra][i]=R[i];
                    }
				}
                else if(IR[0]=='C'&&IR[1]=='R')
                {
                    printf("\nIn CR\n");
                    int a;
					for(a=0;a<4;)
					{
						if(M[ra][a]==R[a]){
                           	a++;
						}
						else{
					    	break;
						}
					}
					if(a==4){
                        C=1;
					}
					else{
                        C=0;
					}
                }
                else if(IR[0]=='B'&&IR[1]=='T')
                {
                    printf("\nIn BT\n");
					if(C==1)
                        IC=((int)IR[2] -48)*10 + ((int)IR[3] -48);
                }
				else if(IR[0]=='G'&&IR[1]=='D')
				{
                    printf("\nIn GD\n");
                    SI = 1;
                }
				else if(IR[0]=='P'&&IR[1]=='D')
                {
                    printf("\nIn PD\n");
                    SI = 2;
                }
				else{
					PI=1;
				}
            }
		}
		else if(IR[0]=='H')
        {
            printf("\nIn H\n");
            IC++;
            SI=3;
        }
        else{
            PI = 1;
        }
    }
	pcb->TTC++;                  //simulate
	printf("pcb->TTC: %d\n",pcb->TTC);
    if(pcb->TTC >= pcb->TTL)
	{
        TI=2;
    }
    if(TI!=0||PI!=0||SI!=0)
	{
		MOS();
	}
	else{
		printf("NOT getting mos function\n");
        EXE_PROGRAM();
	}
}

int ADDRMAP(int va)		//ADDRESS MAPPING
{
	printf("Address map\n");
	printf("PTE: %d va: %d\n",PTE,va );
    int j;
    int PTR = PTE + (va/10);
    printf("PTR: %d\n",PTR);
    char temp[4];
    for(j=0;j<4;j++)
		temp[j]=M[PTE][j];

    int ra;

    if(temp[0]=='1')
    {
        ra = ((int)temp[2] -48)*10 + ((int)temp[3] -48);
    }
    if(ra>=0&&ra<30)
	{
		ra = ra*10;
		ra = ra + (va)%10;
	}
	else
    {
        PI = 3;
    }
    printf("RA: %d\n",ra);
    printf("PI: %d\n",PI);
    return(ra);
}

void MOS()   			//MOS METHOD
{
	printf("\nIn MOS\n");
	if(PI!=0)
	{
		if(TI==0)
		{
			if(PI==1)
			{
				TERMINATE(4);
				printf("Terminate with OPERATION CODE ERROR");
			}
			else if(PI==2)
			{
				TERMINATE(5);
				printf("Terminate with Operand Error");
			}
			else if(PI==3)
			{
				if((IR[0]=='G'&& IR[1]=='D' ) ||  (IR[0]=='S'&& IR[1]=='R' ))
				{

					int ra=((int)IR[2] -48)*10 + ((int)IR[3] -48);
					int m= PTE + (ra/10);
					int t = ALLOCATE();
					printf("\nValid Page Fault Handled with Block NUMBER: %d\n",t);
                    int j = t/10;
                    j=j+'0';
                    M[m][0]= j;
                    j = t%10;
                    j=j+'0';
                    M[m][1]= j;

					IC--;
					pcb->TTC--;
					PI=0;
					EXE_PROGRAM();
				}
				else
				{
					TERMINATE(6);
					printf("Terminate with INVALID PAGE FAULT");
				}
			}
		}
		else if(TI==2)
		{

			if(PI==1)
			{
				TERMINATE(7);
				printf("Terminate with TIME LIMIT EXCEEDED AND OPERATION CODE ERROR");
			}
			else if(PI==2)
			{
				TERMINATE(8);
				printf("Terminate with TIME LIMIT EXCEEDED AND OPERAND ERROR");
			}
			else if(PI==3)
			{
				TERMINATE(3);
				printf("Terminate with TIME LIMIT EXCEEDED");
			}
		}
	}
	else
	{
		if(TI==0)
		{
			if(SI==1)
			{
				READ();
				printf("read");
			}
			else if(SI==2)
			{
				WRITE();
				printf("write");
			}
			else if(SI==3)
			{	TERMINATE(0);
				printf("terminate");
			}
		}
		else if(TI==2)
		{
			if(SI==1)
			{
				TERMINATE(3);
				printf("Terminate with TIME LIMIT EXCEEDED");
			}
			else if(SI==2)
			{
				WRITE();
				TERMINATE(3);
				printf("Terminate with TIME LIMIT EXCEEDED");
			}
			else if(SI==3)
			{
				TERMINATE(0);
				printf("Will terminate");
			}
			else if(SI==0)
			{
				TERMINATE(3);
				printf("Will terminate");
			}
		}
	}
}

void READ()				//READ METHOD
{
    SI=0;
    IR[3]='0';
    char temp;
    int val=((int)IR[2] - 48)*10 + ((int)IR[3] - 48);
    int i,p=0,j;
    memset(B,0,40);
	for(i=0;i<40 && getc(infile)!=EOF;i++)
	{
	    fseek(infile,-1,SEEK_CUR);
		temp = (char)(getc(infile));
		if(temp=='\0')
			break;
		B[i]=temp;
		printf("%c",B[i]);
	}
	printf("\n");
	if(B[0]!='$')
	{
		int ra=ADDRMAP(val);
		int sh=ra;
		do{
			printf("M[%d]: ",ra);
			for(j=0;j<4&&B[p]!='\n'&&B[p]!=NULL;j++)
			{
				M[ra][j]=B[p++];
				printf("%c",M[ra][j] );
			}
			printf("\n");
			ra++;
		}
		while(B[p]!=NULL && B[p]!='\n');
			EXE_PROGRAM();
	}
	else
	{
		TERMINATE(1);
	}
}

void WRITE()
{
    SI=0;
    IR[3]='\0';
    int value=(((int)IR[2] -48)*10) + ((int)IR[3] -48);
    pcb->LLC++;
    //fprintf(outfile, "%d", pcb->LLC);
	if(pcb->LLC > pcb->TLL)
		TERMINATE(2);
	else
	{
		int j=0,i=0;
		int ra=ADDRMAP(value);
		while(i<10)
		{
			if(M[ra][j]=='\0' && M[ra][j]!=NULL && M[ra][j]!='$') {
                fputc(M[ra][j],outfile);
                //fprintf(outfile, "%s", M[ra][j]);
			}
			j++;
			if(j==4)
			{
				ra++;
                i++;
				j=0;
			}
		}

		//fprintf(outfile, "%d", pcb->LLC);
		fputc('\n',outfile);
		if(TI==0){
            EXE_PROGRAM();
		}
	}
}

void TERMINATE(int em)
{
	printf("\nIn Terminate\n");
	fputs("Job Id 	:	",outfile);
	fprintf(outfile,"%d\n",pcb->JID);
    switch(em)
	{
		case 0:
		fputs("NO ERROR",outfile);
		break;
		case 1:
		fputs("OUT OF DATA",outfile);
		break;
		case 2:
		fputs("LINE LIMIT EXCEEDED",outfile);
		break;
		case 3:
		fputs("TIME LIMIT EXCEEDED",outfile);
		break;
		case 4:
		fputs("OPERATION CODE ERROR",outfile);
		break;
		case 5:
		fputs("Operand Error",outfile);
		break;
		case 6:
		fputs("INVALID PAGE FAULT",outfile);
		break;
		case 7:
		fputs("TIME LIMIT EXCEEDED AND OPERATION CODE ERROR",outfile);
		break;
		case 8:
		fputs("TIME LIMIT EXCEEDED AND OPERAND ERROR",outfile);
		break;
	}
    fputs("\nIC 		:	",outfile);
    fprintf(outfile,"%d\n",IC);
    if(IR[0]=='H'){
    	fputs("IR 		:	",outfile);
    	fprintf(outfile,"%c\n",IR[0]);
    }
    else{
    	fputs("IR 		:	",outfile);
    	fprintf(outfile,"%s\n",IR);
    }

    fputs("TTC 		:	",outfile);
    fprintf(outfile,"%d\n",pcb->TTC);
    fputs("LLC 		:	",outfile);
    fprintf(outfile,"%d\n",pcb->LLC);

	fputc('\n',outfile);
}

int main()
{
    infile = fopen("input_gouri.txt","r");
    outfile = fopen("Kamlesh2.txt","w");
	if(infile==NULL)
		printf("\n\t\tFile not found..\n");
	else
	    LOAD();
    return(0);
    fclose(infile);
    fclose(outfile);
}
