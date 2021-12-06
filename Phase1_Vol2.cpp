#include<conio.h>
#include<iostream>
#include<fstream>
#include <stdlib.h>
#include<string.h>
using namespace std;

int base,ele;

class OS{
    private:
        char M[100][4];     //Physical Memory
        char IR[5];         //Instruction Register (4 bytes)
        char R[4];          //General Purpose Register (4 bytes)
      	bool C;             //True/False
        char B[41];         //Buffer
        int PTE,IC,SI,PI,TI;
        int ranum[30] ;
        int ADMAP(int);
struct PCB
{
    int TLL;
    int TTL;
    int JID;
    int TTC;
};
struct PCB *pcb;
    public:
        void INIT();
        void LOAD();
        void Execute();
        void MOS(int);
        void EXECUTEPROGRAM();
        void STARTEXECUTION();
		void READ(int);
		char BUFF[40];
		void WRITE();
		void TERMINATE();
		void EXEC_USER_PROG();
		void INIT_PCB();
		void ALLOCATE();

	fstream infile;
	fstream outfile;

}os;
void OS::INIT_PCB()
{
	int a,b,c,d;
	printf("**********INSIDE INIT_PCB**********\n");
    pcb = (struct PCB*)malloc(sizeof(struct PCB));
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
//init Function

void OS::INIT()
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
    	ranum[i]='\0';
    }
}
void OS::ALLOCATE()
{
	printf("**********INSIDE ALLOCATE**********\n");
    int n;
    n = rand()%30;
    while(ranum[n]==1)
	{
        n = (rand()%30);
    }
    ranum[n]=1;
	printf("Random number generated is %d\n",n);


}
//LOAD Function

void OS::LOAD()
{
	printf("**********INSIDE LOAD FUNCTION**********\n");
    int i,k;
    char temp;
    while(getc(infile)!=EOF)
    {
        printf("\n\n-----------PAGE TABLE Pointer: %d\n",PTE );
        fseek(infile,-1,SEEK_CUR);
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
            //Initializing page table
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
			for(int i=0;i<10;i++)
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
            //random number generated will be of two character
            //q will retreive 1st char and r will retrive 2nd char
            int q=index/10;
            int r=index%10;
            //updating page table
	       		M[k][0]='1';
				char a=q+48;
				//itoa(q,a,10);
				M[k][2]=a;
				//itoa(r,a,10);
				a=r+48;
				M[k][3]=a;
				printf("M[%d]\t  %c   %c    %c   %c \n",k,M[k][0],M[k][1],M[k][2],M[k][3]);
			printf("\n-----------------------------------\n");
			//Load program card in allocated index
			index=index*10;
			printf("\t [0] [1] [2] [3]\n");
			int j=0;
			while(j<40 && BUFF[j]!='\n')
			{
				cout<<"M["<<index<<"]"<<endl;
				for(i=0; i<4 && BUFF[j]!='\n'; i++)
				{
					M[index][i]=BUFF[j++];
					cout<<M[index][i]<<endl;
				}
				index++;
				cout<<"\n"<<endl;
			}
			k++;
		}
    }


}


//STARTEXECUTION Function

void OS::STARTEXECUTION()
{
	IC=0;
	EXECUTEPROGRAM();
}



//Execution Function

void OS::EXECUTEPROGRAM()
{
	int k,i,j,loc,x,y;
	while(IC<base)
	{
		for(int i = 0; i<4; i++)        //Load in register
        {
            IR[i] = M[IC][i];
        }

   		loc = atoi(&IR[2]);
      	if(IR[0]=='G'&&IR[1]=='D')
		{
			SI=1;
			MOS(loc);
		}
		else if(IR[0]=='P'&&IR[1]=='D')
		{
			SI=2;
			MOS(loc);
		}
		else if(IR[0]=='H')
		{
			SI=3;
			MOS(loc);
		}
		else
		if(IR[0]=='L'&&IR[1]=='R')
		{
			for(i=0;i<5;i++)
			R[i]='\0';
			for (j=0;j<4;j++)
			{
				if(M[loc][j]!='\0'&&M[loc][j]!='\n')
				R[j]=M[loc][j];
				else
				break;
			}
     	}

     	else if(IR[0]=='S'&&IR[1]=='R'){
            for(j=0;j<4&&R[j]!='\0';j++)
            {
                M[loc][j]=R[j];
            }
     	}

		else if (IR[0]=='C'&&IR[1]=='R')
		{
			for(i=0;i<4;i++)
			{
				if(R[i] == M[loc][i])
				C = 1;
				else
				{
					C = 0;
					break;
				}
			}
		}
		else if(IR[0]=='B'&&IR[1]=='T')
		{
			if(C == 1)
			{
				IC=loc-1;

			}
		}
		IC++;
	}
}


//MOS Function

void OS::MOS(int loc)
{

	switch(SI)
	{
		case 1:
			READ(loc);
			SI=0;
			break;

	 	case 2:
	 	    WRITE();
	 	    SI=0;
	 	    break;

	 	case 3:
	 		TERMINATE();
	 		SI=0;
	 		break;

	}
}

//READ Function
void OS::READ(int loc)
{
		cout<<"\nIN READ MODE\n";

		for(int i=0;i<41;i++)      //clear Buffer
           B[i]='\0';

        infile.getline(B,40);

       	int j;
        int k = 0;
        int i = IR[2]-48;
        i = i*10;

        if(B[0] == '$' && B[1] == 'E' && B[2] == 'N' && B[3] == 'D')
        {
            LOAD();
    	}
    	else
    	{

        	for( int l=0 ; l<10 ;  ++l)
        	{
	            for(j = 0 ; j<4; ++j)
	            {
	                M[i][j] = B[k];
	                k++;
	            }
	            if(k == 40)
    	        {
	                break;
    	        }
	            i++;
    	    }
    	}
}


//WRITE Function

void OS::WRITE()
{
	cout<<"\nIN WRITE MODE\n";
      for(int i=0;i<=39;i++)
           B[i]='\0';

        int k = 0;
        int i = IR[2]-48;
        i = i*10;


        for( int l=0 ; l<10 ;  ++l)
        {
            for(int j = 0 ; j<4; ++j)
            {
                B[k]=M[i][j];
               	outfile<<B[k];

                k++;
            }
            if(k == 40)
            {
                break;
            }
            i++;
        }
        outfile<<endl;

}



//TERMINATE Function

void OS::TERMINATE()
{
     for(int i=0;i<100;i++)
		{
			cout<<"M["<<i<<"]=";
			for(int j=0;j<4;j++)
			cout<<M[i][j];
			cout<<endl;

		}
	outfile<<"\n";
    outfile<<"\n";
    cout<<"in terminate";
	LOAD();
}

//Main Function

int main()
{
	char f1[30];

    os.infile.open("input2.txt", ios::in);
    os.outfile.open("output2.txt", ios::out);

    if(!os.infile)
    {
        cout<<"Failure"<<endl;
    }
    else
    {
        cout<<"File Exist"<<endl;
        os.LOAD();
    }

	getch();
    return 0;
}
