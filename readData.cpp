#include<iostream>
#include<fstream>
using namespace std;

int labels[50000];
float trainingData[50000][7524];

int main()
{
	fstream DataSet;
	DataSet.open("features.dat");
	
	int i=0,j=0,temp1;
	char temp2;
	while (DataSet>>labels[j])
	{
		cout<<labels[j]<<"\n";
		for(i=0;i<=7523;i++)
		{
			DataSet>>temp1>>temp2>>trainingData[j][i];
			cout<<temp1<<trainingData[j][i]<<"\n";
		}
		j++;
	}
	return 0;
}
