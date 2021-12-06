
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char BUFF[40];
char M[300][4];
char IR[4];
char R[4];
int PTE,IC,SI,PI,TI;
int RANUM[30] ;
int C;
int ADMAP(int);
void STARTEXECUTION();
void EXEC_USER_PROG();
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

FILE *inputfile,*outputfile;

struct PCB *pcb;

//---------------------------init PCB-----------------------------------------------------
void INIT_PCB()
{
	int a,b,c,d;
	printf("**********INSIDE INIT_PCB**********\n");
    pcb = (struct PCB*)malloc(sizeof(struct PCB));
    pcb->LLC = 0;
    pcb->TTC = 0;
    a=(int)BUFF[4]-48;
    b=(int)BUFF[5]-48;
    c=(int)BUFF[6]-48;
    d=(int)BUFF[7]-48;
    //printf("\n %d\t %d\t%d\t %d\n ",a,b,c,d);
    pcb->JID=(a*1000+b*100+c*10+d);
    //int x=atoi(&BUFF[4]);
    //int y=atoi(&BUFF[5]);
    //int z=atoi(&BUFF[6]);
    //printf("\n %d\t %d\t%d\n ",x,y,z);
    //printf("\n %d ",(int)BUFF[5]-48);
    //printf("\n %d ",(int)BUFF[6]-48);
    printf("JOB ID JID %d\n",pcb->JID);
    a=(int)BUFF[8]-48;
    b=(int)BUFF[9]-48;
    c=(int)BUFF[10]-48;
    d=(int)BUFF[11]-48;
    //printf("\n %d\t %d\t%d\t %d\n ",a,b,c,d);
    pcb->TTL=(a*1000+b*100+c*10+d);
      //(int)-48 is used to convert string to integer
    //is multiplied by 1000,100,10 to take thousands hundreds and tens place
    printf("Total time limit TTL %d\n",pcb->TTL);
    a=(int)BUFF[12]-48;
    b=(int)BUFF[13]-48;
    c=(int)BUFF[14]-48;
    d=(int)BUFF[15]-48;
    //printf("\n %d\t %d\t%d\t %d\n ",a,b,c,d);
    pcb->TLL=(a*1000+b*100+c*10+d);
    printf("Total Line Limit TLL %d\n",pcb->TLL);
}

//----------------------------init-------------------------------------------------------------
void INIT()
{
	printf("\n**********INSIDE INIT**********\n");
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
    	RANUM[i]='\0';
    }
}

//-----------------------allocate--------------------------------------------------------------
int ALLOCATE()
{
	printf("**********INSIDE ALLOCATE**********\n");
    int n;
    n = rand()%30;
    while(RANUM[n]==1)
	{
        n = (rand()%30);
    }
    RANUM[n]=1;
	printf("Random number generated is %d\n",n);
    return n;
}

//--------------------load --------------------------------------------------------------------
void LOAD()
{
	printf("**********INSIDE LOAD FUNCTION**********\n");
    int i,k;
    char temp;
    while(getc(inputfile)!=EOF)
    {
        printf("\n\n-----------PAGE TABLE Pointer: %d\n",PTE );
        fseek(inputfile,-1,SEEK_CUR);
        memset(BUFF,0,40);
		fgets(BUFF,41,inputfile);
		printf("BUFF array contains :  ");
		for(i=0;i<40;i++)
		{
			printf("%c",BUFF[i]);
        }
		printf("\n");

        if(BUFF[0] == '$' && BUFF[1] == 'A' && BUFF[2] =='M' && BUFF[3] == 'J')
        {
            printf("**********INSIDE $AMJ**********");
            INIT();
            INIT_PCB();
            PTE = ALLOCATE();
            PTE = 10*PTE;
            k=PTE;
            printf("PTE = %d\n",PTE);
            for(int i=0;i<10;i++)
            {
            	M[k][0]='0';
            	M[k][1]='0';
				M[k][2]='*';
				M[k][3]='*';
				k++;
			}
			k=PTE;
			printf("Initialied Page Table ..\n");
			printf("\t [0] [1] [2] [3]\n");
			for(i=0;i<10;i++)
			{
				printf("M[%d]\t  %c   %c   %c   %c \n",k,M[k][0],M[k][1],M[k][2],M[k][3]);
				k++;
			}
			k=PTE;
        }
        else if(BUFF[0] == '$' && BUFF[1] == 'D' && BUFF[2] =='T' && BUFF[3] == 'A')
        {
            printf("\nINSIDE $DTA\n");
            STARTEXECUTION();
	    }
        else if(BUFF[0] == '$' && BUFF[1] == 'E' && BUFF[2] =='N' && BUFF[3] == 'D')
        {
            printf("\nINSIDE $END\nPROCESS ENDED...........................................\n\n");
            continue;
        }
        else
        {
            printf("**********PROGRAM CARD FOUND**********\n");
            int index = ALLOCATE();
            int q=index/10;
            int r=index%10;
            //updating page table
            printf("Updating Page table..\n");
	       		M[k][0]='1';
				char a=q+48;
				//itoa(q,a,10);
				M[k][2]=a;
				//itoa(r,a,10);
				a=r+48;
				M[k][3]=a;
				printf("\t [0] [1] [2] [3]\n");
				for(i=0;i<10;i++)
				{
					printf("M[%d]\t  %c   %c   %c   %c \n",k,M[k][0],M[k][1],M[k][2],M[k][3]);
					k++;
				}
				//printf("M[%d]\t  %c   %c    %c   %c \n",k,M[k][0],M[k][1],M[k][2],M[k][3]);
			index=index*10;
			printf("\t [0] [1] [2] [3]\n");
			int j=0;
			while(j<40 && BUFF[j]!='\n')
			{
				printf("M[%d]: ",index);
				for(i=0; i<4 && BUFF[j]!='\n'; i++)
				{
					M[index][i]=BUFF[j++];
					printf("  %c ",M[index][i]);
				}
				index++;
				printf("\n");
			}
			k++;
		}
    }
}

//--------------start execution------------------------------------------------------------
void STARTEXECUTION()
{
	printf("inside start\n");
    IC=0;
    EXEC_USER_PROG();
}

//------------Execute user program--------------------------------------------------------------
void EXEC_USER_PROG()
{
	printf("inside EXEC_USER_PROG\n");
    int i;
    int ra = ADMAP(IC);			//ADMAP will return the real address i.e where 1st instructor is stored
    if(PI==0)
	{
		IR[0]=M[ra][0];
		if(M[ra][1]!='\n' && M[ra][1]!='$' && IR[0]!='H')		//
		{
			IR[1]=M[ra][1];
			IR[2]=M[ra][2];
			IR[3]=M[ra][3];
			printf("\nIR=%s\n",IR);
			IC++;
			int m=(int)IR[3] -48;
			int n=(int)IR[2] -48;
			if(m<10 && n<10 && m>=0 && n>=0)
			{
				n=(n*10)+m;
				ra=ADMAP(n);
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
		printf("going to mos function\n");
		MOS();
	}
	else{
		printf("NOT going to mos function instad going to EXEC_USER_PROG\n");
        EXEC_USER_PROG();
	}
}

//-----------address mapping----------------------------------------------------------------------------
int ADMAP(int va)			///va is IC means instruction conuter
{
	printf("inside admap\n");
	printf("PTE: %d va: %d\n",PTE,va );
    int j;
    int PTR = PTE + (va/10);
    printf("PTR: %d\n",PTR);
    char temp[4];
    for(j=0;j<4;j++)			//to fetech 4 characters from page table
		temp[j]=M[PTE][j];

    int ra;

    if(temp[0]=='1')	//||temp[1]=='1'||temp[0]=='2')
    {
        ra = ((int)temp[2] -48)*10 + ((int)temp[3] -48);//to fetch int value of page number
    }
    if(ra>=0&&ra<30)
	{
		ra = ra*10;
		ra = ra + (va)%10;
	}
	else		//M[pte][2]=*,M[pte][3]=*	//page fault //occurs when page table is found with 00**
    {
        PI = 3;
    }
    printf("RA: %d\n",ra);
    printf("PI: %d\n",PI);
    return(ra);
}

//------------------------MOS----------------------------------
void MOS()
{
	printf("inside mos\n");
	if(PI!=0)   //Cases(TI and PI)
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
					printf("\nValid Page Fault Handled with Block No. - %d\n",t);
                    int j = t/10;
                    j=j+'0';
                    M[m][0]= j;
                    j = t%10;
                    j=j+'0';
                    M[m][1]= j;

					IC--;
					pcb->TTC--;
					PI=0;
					EXEC_USER_PROG();	//handling valid page fault
				}
				else
				{
					TERMINATE(6);   //invalid page fault
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
	else   //if PI==0(cases of TI and SI)
	{
		if(TI==0)
		{
			if(SI==1)
			{
				READ();
				printf("Will read");
			}
			else if(SI==2)
			{
				WRITE();
				printf("Will write");
			}
			else if(SI==3)
			{	TERMINATE(0);
				printf("Will terminate");
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
//----------------------------------------------------------------------------------------------------
void READ()
{
	printf("inside read\n");
    SI=0;
    IR[3]='0';
    char temp;
    int val=((int)IR[2] -48)*10 + ((int)IR[3] -48);
    int i,p=0,j;
    memset(BUFF,0,40);
	for(i=0;i<40&&getc(inputfile)!=EOF;i++)
	{
	    fseek(inputfile,-1,SEEK_CUR);
		temp = (char)(getc(inputfile));
		if(temp=='\n')
			break;
		//store in BUFF
		BUFF[i]=temp;
		printf("%c",BUFF[i]);
	}
	printf("\n");
	if(BUFF[0]!='$')
	{
		int ra=ADMAP(val);
		int sh=ra;
		do{
			printf("M[%d]: ",ra);
			for(j=0;j<4&&BUFF[p]!='\n'&&BUFF[p]!=NULL;j++)
			{
				M[ra][j]=BUFF[p++];
				printf("%c",M[ra][j] );
			}
			printf("\n");
			ra++;
		}
		while(BUFF[p]!=NULL && BUFF[p]!='\n');
			EXEC_USER_PROG();
	}
	else
	{
		TERMINATE(1);
	}
}

//----------------------------------------------------------------------------------------------------
void WRITE()
{
	printf("inside write\n");
    SI=0;
    IR[3]='0';
    int val=((int)IR[2] -48)*10 + ((int)IR[3] -48);
    pcb->LLC++;
	if(pcb->LLC > pcb->TLL)
		TERMINATE(2);
	else
	{
		int j=0,i=0;
		int ra=ADMAP(val);
		while(i<10)
		{

			if(M[ra][j]!='\n'&&M[ra][j]!=NULL&&M[ra][j]!='$'){
                fputc(M[ra][j],outputfile);
			}
			j++;
			if(j==4)
			{
				ra++;
                i++;
				j=0;
			}
		}
		fputc('\n',outputfile);
		if(TI==0){
            EXEC_USER_PROG();        //check here
		}
	}
}
//----------------------------------------------------------------------------------------------------
void TERMINATE(int em)
{
	printf("inside terminate\n");
    //fputc('\n',outputfile);
	fputs("JOB ID 	:	",outputfile);
	fprintf(outputfile,"%d\n",pcb->JID);
    switch(em)
	{
		case 0:
		fputs("NO ERROR",outputfile);
		break;
		case 1:
		fputs("OUT OF DATA",outputfile);
		break;
		case 2:
		fputs("LINE LIMIT EXCEEDED",outputfile);
		break;
		case 3:
		fputs("TIME LIMIT EXCEEDED",outputfile);
		break;
		case 4:
		fputs("OPERATION CODE ERROR",outputfile);
		break;
		case 5:
		fputs("Operand Error",outputfile);
		break;
		case 6:
		fputs("INVALID PAGE FAULT",outputfile);
		break;
		case 7:
		fputs("TIME LIMIT EXCEEDED AND OPERATION CODE ERROR",outputfile);
		break;
		case 8:
		fputs("TIME LIMIT EXCEEDED AND OPERAND ERROR",outputfile);
		break;
	}
    fputs("\nIC 		:	",outputfile);
    fprintf(outputfile,"%d\n",IC);
    // char tempIR;
    if(IR[0]=='H'){
    	fputs("IR 		:	",outputfile);/////
    	fprintf(outputfile,"%c\n",IR[0]);//////
    }
    else{
    	fputs("IR 		:	",outputfile);/////
    	fprintf(outputfile,"%s\n",IR);//////
    }

    fputs("TTC		:	",outputfile);
    fprintf(outputfile,"%d\n",pcb->TTC);
    fputs("LLC		:	",outputfile);
    fprintf(outputfile,"%d\n",pcb->LLC);

	fputc('\n',outputfile);
}

//-------------------------------------------------------
int main()
{
	printf("\n\n****************INSIDE MAIN****************\n\n");
    inputfile = fopen("input_gouri.txt","r");
    outputfile = fopen("kamlesh.txt","w");
	if(inputfile==NULL)
		printf("\n\t\t..file not found.\n");
	else
	    LOAD();
    return(0);
    fclose(inputfile);
    fclose(outputfile);
}
