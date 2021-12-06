#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

struct pcb
{
	int JOBID, TotalLineLimit, TotalTimeLimit;
}p;

char M[300][4], IR[5], R[4], Toggle, ch, temp[5];
int b, IC, ISR, TI, PI, ef, e, flag[30], PTE, PTEc, TotalTimeCounter, TotalLimitCounter, LineLimitCounter;
int RealAddress, VirtualAddress, T; //ISR- Service Interrupt
FILE *FP, *FT;

void LOAD();
int ALLOCATE();
void disp();
void init();
void STARTEXECUTION();
void EXECUTEUSERPROGRAM();
int ADDRESSMAP(int );
void MOS();
void READ();
void WRITE();
void TERMINATE(int);
void SIMULATION();

void print_memory() {
    int i,j;
    printf("Memory content are");
    printf("\n  0 1 2 3\n");
    for(i=170;i<=180;i++)
	{
	printf("%d ",i);
	for(j=0;j<4;j++)
	printf("%c ",M[i][j]);
	printf("\n");
    }
}
void main()
{
	system("cls");
	FP=fopen("input.txt", "r");
	FT=fopen("output.txt", "w");
	if(FP == NULL)
	{
		printf("\n File not Found");
		exit(1);
	}
	else
	{
		init();
		LOAD();
		disp();
	}

	print_memory();
	printf("\n Successfully Completed!");
	getch();
}

void init()
{
	int i, j;
	TotalTimeCounter= 0;
	TotalLimitCounter= 0;
	for(i= 0; i<300; i++)
	{
		for(j= 0; j< 4; j++)
		{
			M[i][j]='\0';
		}
	}
	for(i=0;i<30;i++)
	{
		flag[i]= 0;
	}
	b= 0;
	ef= 0;	
	ISR= 0;
	TI= 0;
	PI= 0;
	e= b+10;
}

void disp()
{
	int i,j,k;
    /*printf("Memory content are");
    printf("\n  0 1 2 3\n");
    for(i=0;i<10;i++)
	{
        printf("%d ",i);
        for(j=0;j<4;j++) 
        printf("%c ",M[i][j]);
        printf("\n");
    }
	*/
	printf("\n Contents of Main Memory-");
 	printf("\n0 1 2 3\n");
	for(i =0; i < 30; i++)
	{
		if(flag[i]==1)
		{	
			for(j=i*10;j<(i*10)+10;j++)
			{
				printf("\n");
				for(k=0;k<4;k++)
				{
					if(M[j][k]!='\0')
					printf("%c ", M[j][k]);
				}
			}
		}
	}
	printf("PTE");
	for(i=PTE; i <PTE+10; i++)
	{
		printf("\n %d\t", i);
		for(j=0; j<4; j++)
		{
			printf("%c", M[i][j]);
		}
	}
}

void LOAD()
{
	int i, j, k, t, r ;
	char a[2];
	ch = fgetc(FP);
	init();
	do
	{
		for(i=0;i<4;i++)
		temp[i]= '\0';
		i=0;
		while(i<4 && ch!='\n')
		{
			temp[i]=ch;
			ch=fgetc(FP);
			i++;			
		}
		temp[i]='\0';

	
		if((strcmp(temp, "$AMJ"))==0)
		{
			i=0;
			while(i<4)
			{
				temp[i]=ch;
				ch= fgetc(FP);
				i++;
			}
			temp[i]='\0';
			p.JOBID=atoi(&temp[0]);
			for(i=0;i<4;i++)
			temp[i]= '\0';
			i=0;
			while(i<4)
			{
				temp[i]=ch;
				ch=fgetc(FP);
				i++;
			}
			temp[i]='\0';
			p.TotalTimeLimit=atoi(&temp[0]);
			for(i=0;i<4;i++)
		temp[i]= '\0';
			i=0;
			while(i<4)
			{
				temp[i]=ch;
				ch=fgetc(FP);
				i++;
			}
			temp[i]='\0';
			p.TotalLineLimit=atoi(&temp[0]);
			PTE=ALLOCATE()*10;
			flag[PTE/10]=1;
			PTEc=PTE;
			for(i=PTE;i<PTE+10;i++)
			{
				itoa(0, a, 10);
				M[i][0]=a[0];
				M[i][1]=a[0];
				itoa(3, a, 10);
				M[i][2]=a[0];
				itoa(1, a, 10);
				M[i][3]=a[0];
			}
		}
		else if((strcmp(temp, "$DTA"))==0)
		{
			STARTEXECUTION();
			if(ef==1)
			{
				if((strcmp(temp, "$END"))!=0)
				{
					ch= fgetc(FP);
					while(ch!='$' && ch!=EOF)
					{
						ch= fgetc(FP);
					}
					while(ch!='\n' && ch!=EOF)
					{
						ch= fgetc(FP);
					}
				}
				else
				{
					while(ch!='\n'&& ch!=EOF)
					{
						ch = fgetc(FP);
					}
				}
				init();
			}break;
		}
		else if((strcmp(temp, "$END"))==0)
		{
			while(ch!='\n' && ch!=EOF)
			{
				ch = fgetc(FP);
			}
			if(ef == 1)
			{
				init();
			}
		}	
		else if((strcmp(temp, "$DTA"))!=0)
		{
			b= ALLOCATE();
			while (flag[b]==1)
			{
				b= ALLOCATE();
			}
			flag[b] = 1;
			itoa(b/10, a, 10);
			M[PTEc][2]=a[0];
			itoa(b%10, a, 10);
			M[PTEc][3]=a[0];
			PTEc++;
			b=b*10;
		
			do
			{
				for(k=0; k<4; k++)
				{
					M[b][k]=temp[k];
				}
				for(i=0;i<4;i++) {
					temp[i] = '\0';
				}
			 	for(k=0;k<4;k++)
				{
					if(ch!=EOF && ch!= '\n')
					{
						temp[k]=ch;
						ch=fgetc(FP);
					}
					else
						break;
				}
				b++;
			}
			while(ch!='\n' && ch!= EOF);
			for(k=0; k < 4; k++)
			{
				M[b][k]= temp[k];
			}
		}	
		if(ch=='\n')
		ch= fgetc(FP);		
	}while(!feof(FP));
}

int ALLOCATE()
{
	int i;
	i= rand() % 30;
	//flag[i]=1;
	return(i);
}

void STARTEXECUTION()
{
	IC= 0;
	EXECUTEUSERPROGRAM();
}

