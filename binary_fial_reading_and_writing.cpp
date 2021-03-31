#include "stdafx.h"
#include<fstream>
#include<iostream>
#include<stdlib.h>

using namespace std;

const int MaxNameLen=30;
typedef struct _StudInfo
{
	char name[MaxNameLen];
	int num;
	int grade;
}STUDINFO;

class Student 
{
	STUDINFO stud;
public:
	Student(char *nm,int n,int g) {strcpy(stud.name,nm); stud.num=n;stud.grade=g;}
	Student() { }
	~Student() { }
	STUDINFO *GetStud() { return &stud;}

	friend void CreateStudent(Student *s1);
	friend ostream & operator <<(ostream &out,Student *s);
	friend istream & operator >>(istream &in,Student  *s);
};

ostream & operator <<(ostream &out,Student *s)
{
	out<<s->stud.name<<" "<<s->stud.num<<"  "<<s->stud.grade;
	return out;
}
istream & operator >>(istream &in,Student *s)
{
	in>>s->stud.name>>s->stud.num>>s->stud.grade;
	return in;
}

//CREATE STUDENT BY COMPUTER
void CreateStudent( Student * s)
{
	static int snum=0;
	snum++;
	
	sprintf(s->stud.name,"Student%06d",snum);
	s->stud.num = 2015*100000+snum;
	s->stud.grade= 500+rand()%251;
	
}
int CreateDataFile(char *fn)
{
	ofstream outFile;
	int snum=60000;
	int i;
	
	Student st;
	outFile.open(fn,ios::out| ios::binary);
	
	if(outFile.fail())
	{
		return 0;
	}
	for(i=0;i<snum;i++)
	{
		CreateStudent(&st);
		outFile.write((char *) st.GetStud(),sizeof(STUDINFO));		
	}
	outFile.close();
	return 1;

}
int MoveToNext(ifstream &infile)
{
	
	STUDINFO st;
	if(!infile.eof())
	{
		infile.read((char *)&st,sizeof(STUDINFO));
		
		cout<<"The current record:"<<st.name<<"  "<<st.num<<" "<<st.grade<<endl;
		return 1;
	}
	else
		cout<<"********already move the end of the file"<<endl;
	return 0;

}

int MoveToPrev(ifstream &infile)
{
	STUDINFO st;
	long pos=infile.tellg();
	long x=-sizeof(STUDINFO);
	if(infile.tellg())
	{
		infile.seekg(x,ios::cur);
		
		infile.read((char *)&st,sizeof(STUDINFO));
		
		infile.seekg(x,ios::cur);// here is a bug, x must be defined in long,
		pos=infile.tellg();
		cout<<"The current record:"<<st.name<<"  "<<st.num<<" "<<st.grade<<endl;
		return 1;
	}
	else
		cout<<"************Already at the begin of the file"<<endl;

	return 0;
}

int MoveToEnd(ifstream &infile)
{
	infile.seekg(0l,ios::end);
	cout<<"end of file=="<<infile.tellg()<<endl;
	return 1;
}
int MoveToBegin(ifstream &infile)
{
	infile.seekg(0l,ios::beg);
	return 1;
}
int MoveN(ifstream &infile,int n)
{
	long off=n*sizeof(STUDINFO);
	infile.seekg(off,ios::cur);
	if(infile.fail())
	{
		cout<<infile.tellg()<<endl;	
		return 0;
	}
	STUDINFO st;
	if(infile.tellg())
	{
		infile.read((char *)&st,sizeof(STUDINFO));
		cout<<"The current record:"<<st.name<<"  "<<st.num<<" "<<st.grade<<endl;
		return 1;
	}
	return 0;
}

int ProcessDataFile(char *fn)
{
	ifstream inFile;	
	STUDINFO st;
	inFile.open(fn,ios::in| ios::binary|ios::out) ;
	
	if(inFile.fail())
	{
		return 0;
	}
	streampos cur=inFile.tellg();//Get the current position of the file
	inFile.seekg(0l,ios::end);//move to the end of the file

	long len =inFile.tellg();
	cout<<"Total size of the file="<<len<<"  Record number="<<len/sizeof(STUDINFO)<<endl;
	cout<<" the current position is at the begin of the file"<<endl;

	inFile.seekg(cur, ios::beg);   
	
	int select;
	int cont=1;
	int n;
	while(cont)
	{
		cout<<"******************************************************"<<endl;
		printf("0.Exit\n");
		printf("1.Move to next record\n");
		printf("2.Move to previous record\n");
		printf("3.Move to end of file\n");
		printf("4.Move to begin of file\n");
		printf("5.Skip N record\n");

		cout<<"******************************************************"<<endl;
		cout<<"Select(0..5):";
		cin>>select;
		switch(select)
		{
		case 0: cont=0; break;
		case 1:MoveToNext(inFile);break;
		case 2:MoveToPrev(inFile);break;
		case 3:MoveToEnd(inFile);break;
		case 4:MoveToBegin(inFile);break;
		case 5:
			cout<<"Input an integer:";
			cin>>n;
			MoveN(inFile,n);break;

		default:printf("Input error\n");

		}
	}

	inFile.close();
	return 1;

}
int main()
{
	char filename[]="a1.dat";

	if(CreateDataFile(filename)==0)
	{
		printf("create file error\n");
		return 1;
	}
	ProcessDataFile(filename);
	return 0;
}
