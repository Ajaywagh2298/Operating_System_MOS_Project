#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
char m[100][4],ir[4],r[4],c,buff[10][4];
int b,ic,si,e,readf;
FILE *fp, *fw;

void init();
void load();
void startexecution();
void mos(int);
void read();
void write();
void terminate();
void executeprogram();




void main()
{
	int i,j;

	fp=fopen("input.txt" , 'r');
	fw=fopen("output.txt" , 'a');
	if (fp==NULL)
		printf("file not found ");
	else
	{
		load();

	}
	printf("operation complete");


}

void init()
{
	int i , j ;
	for (i=0;i<100;i++)
	{
		for (j=0;j<4;j++)
		{
		  	m[i][j]='\0';
		}
	}
	b=0;
	e=b+10;
	readf=0;

}

void load()
{
	int i, j, k;
	char  ch, temp[4];
	ch=fgetc(fp);
	init();
	do
	{
		i=0;
		while(i<4&&ch!='\n')
	 	{
	 		temp[i]=ch;
	 		ch=fgetc(fp);
	 		i++;

		 }
		 temp[i]='\0';


		if((strcmp(temp,"$AMJ"))==0)
		{
		while(ch!='\n')
		    ch=getc(fp);

        }
		else if ((strcmp(temp,"$DTA"))==0)
		{
			startexecution();

	    }
		else if((strcmp(temp,"$DTA"))!=0)
		{
			i=0;
			if(b==e)
			{
				e=e+10;
			}
			for(k=0;k<4;k++)
			{
				m[b][k]=temp[i];
				i++;
			}
			    b++;
     	 }
		  if(ch=='\n')
		ch=fgetc(fp);
	} while(ch!=EOF);
}


void startexecution()
{
	ic=0;
	executeprogram();
}


void executeprogram()
{
	int k,i,j,loc,x,y;
	while(ic<b)
	{
		for(k=0;k<4;k++)
		{
			ir[k]=m[ic][k];

		}
//ic++;
		x=atoi(&ir[2]);
		loc=x;
		if(ir[0]=='L'&&ir[1]=='R')
		{
			for(i=0;i<4;i++)
			r[i]='\0';
			for (j=0;j<4;j++)
			{
				if(m[loc][j]!='\0'&&m[loc][j]!='\n')
				r[j]=m[loc][j];
				else
				break;
			}
     	}
     	else if(ir[0]=='S'&&ir[1]=='R'){
            for(j=0;j<4&&r[j]!='\0';j++)
            {
                m[loc][j]=r[j];
            }
     	}

		else if (ir[0]=='c'&&ir[1]=='R')
		{
			for(i=0;i<4;i++)
			{
				if(r[i] == m[loc][i])
				c = 't';
				else
				{
					c = 'f';
					break;
				}
			}
		}
		else if(ir[0]=='B'&&ir[1]=='T')
		{
			if(c == 't')
			{
				ic=loc;

			}
		}
		else if(ir[0]=='G'&&ir[1]=='D')
		{
			si=1;
			mos(loc);
		}
		else if(ir[0]=='P'&&ir[1]=='D')
		{
			si=2;
			mos(loc);
		}
		else if(ir[0]=='H')
		{
			si=3;
			mos(loc);
		}
		ic++;

	}
}


void mos(int loc)
{

	switch(si)
	{
		case 1:
			read(loc);
			si=0;
			break;
	 	case 2:
	 	    write(loc);
	 	    si=0;
	 	    break;
	 	case 3:
	 		terminate();
	 		si=0;
	 		break;

	}
}


void read(int loc)
{
	int i,j,k;
	char ch,temp[20];
	if(readf==0)
	{
		k=loc-1;
		e=loc+10;
		ch=fgetc(fp);
		while(1)
		{
			i=0;
			while(i<4&&ch!='\n')
			{
				temp[i]=ch;
				i++;
				ch=fgetc(fp);
			}
		temp[i]='\0';
		if((strcmp(temp,"$END"))==0)
		{
			while(ch!='\n'&&ch!=EOF)
			ch=fgetc(fp);
			break;
		}
		else
		{
			if(ch=='\n')
			{
				k++;
				i=0;
				while(i<4)
				{
					m[k][i]=temp[i];
					i++;
				}
				k=e-1;
				e=e+10;

			}
			else
			{
				k++;
				i=0;
				while(i<4)
				{
					m[k][i]=temp[i];
					i++;
				}
			}
		}
			if(ch=='\n')
			ch=fgetc(fp);
		}
		readf=1;
	}
}


void write()
{
	int i,j,k,loc,x;
	char ch1;
	ir[3]=0;
	x=atoi(&ir[2]);
	loc=x*10;
	i=loc;
	while(i<=loc+9)
	{
		for(k=0;k<4;k++)
		{
			if(m[i][k]!='\0')
			{
				fputc(m[i][k],fw);
				ch1=m[i][k];
			}
			else
			break;
		}

	i++;

	}
	if(ch1!='\n')
	fputc('\n',fw);


}

void terminate()
{
	fputc('\n',fw);
	fputc('\n',fw);
	load();
}
