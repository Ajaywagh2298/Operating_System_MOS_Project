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
        int IC;             //Instruction Counter Register (2 bytes)
        int SI;             //DAT Values
      	bool C;             //True/False
        char B[41];         //Buffer
   
    public:
        void INIT();
        void LOAD();
        void Execute();
        void MOS(int);
        void EXECUTEPROGRAM();
        void STARTEXECUTION();
		void READ(int);
		void WRITE();
		void TERMINATE();
		
	fstream infile;
	fstream outfile;
        
}os;

//init Function

void OS::INIT()
{

    for(int i = 0; i<100; i++)
    {    
        for(int j = 0; j<4; j++ )
        {  
            M[i][j]=' ';
            
        }
    }    
     
   
    IR[0] = {' '};
    R[0] = {' '};
    base=0;
    ele=base+10;
    C = false;

}

//LOAD Function

void OS::LOAD()
{
	int i, j, k,x=0;
	do
	{
	
		for(int i=0;i<41;i++)      //clear Buffer
           B[i]='\0';

        infile.getline(B,41);
//--Printing Buffer Values	
//		for(int k=0;k<=39;k++)      
//           cout<<B[k];
        
	  if(B[0] == '$' && B[1] == 'A' && B[2] == 'M' && B[3] == 'J')
		{
			x=0;
			INIT();
        }
        
	  else if(B[0] == '$' && B[1] == 'D' && B[2] == 'T' && B[3] == 'A')
		{
			STARTEXECUTION();
	    }
	  else 
		{
		 int k = 0;
            
            if(base==ele)
			{
				ele=ele+10;
			}
			
            for(; x <100; ++x)
            {   
                for(int j = 0 ; j<4; ++j)
                {
                    
                    M[x][j] = B[k];
                    k++;
                }
                base++;
                if(k == 40 || B[k] == '\n')
                {
                	x++;
                    break;
                }
                
            }   
		}
	} while(!infile.eof());
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
		//cout<<"\nIN READ MODE\n";
		
		for(int i=0;i<41;i++)      //clear Buffer
           B[i]='\0';
        
        infile.getline(B,40);
       
       	int j;
        int k = 0;
        int i = loc;
        
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
	//cout<<"\nIN WRITE MODE\n";
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
//------For Printing Memory data--------
//     for(int i=0;i<100;i++)
//		{	
//			cout<<"M["<<i<<"]=";
//			for(int j=0;j<4;j++)
//			cout<<M[i][j];
//			cout<<endl;
//
//		}
	outfile<<"\n";
    outfile<<"\n";
    LOAD();
}

//Main Function

int main()
{ 
	char f1[30];
	cout<<"Enter Input file name:";
	cin.getline(f1,30);
    os.infile.open(f1, ios::in);
    

    if(!os.infile)
    {
        cout<<"Failure"<<endl;
    }
    else
    {
        cout<<"File Exist"<<endl;
        cout<<"Enter Output file name:";
    	cin.getline(f1,30);
    	os.outfile.open(f1, ios::out);
        os.LOAD();
    }
    cout<<"\nOperation complete successfully";
	getch();
    return 0;
}
