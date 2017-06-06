#include<iostream>
#include<fstream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/ml/ml.hpp>

using namespace cv;
using namespace cv::ml;
using namespace std;

int labels[50000];
float trainingData[50000][7524];
float testData[8000];

int readData()
{
	fstream DataSet;
	DataSet.open("features.dat");
	
	int i=0,j=0,temp1;
	char temp2;
	while (DataSet>>labels[j])
	{
		cout<<labels[j]<<'\n';
		for(i=0;i<=7523;i++)
		{
			DataSet>>temp1>>temp2>>trainingData[j][i];
		}
		j++;
	}
	return j;
}

void readTestData()
{
	fstream DataSet;
	DataSet.open("test.dat");
	
	int i=0,temp1;
	char temp2;
	for(i=0;i<=7523;i++)
	{
		DataSet>>temp1>>temp2>>testData[i];
	}
}

int main()
{
	int j=readData(); //Read training data
	Mat labelsMat(j,1,CV_32SC1,labels);
	Mat trainingDataMat(j,7524,CV_32FC1,trainingData);
   
	// Set up SVM
	Ptr<SVM> svm=SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	svm->train(trainingDataMat,ROW_SAMPLE,labelsMat);
	

	cout<<"\nReading test Data:\n ";
	readTestData();
	cout<<"\nDONE\n";
	
	Mat query(1,7524,CV_32FC1,testData);
	
	float res;
	res=svm->predict(query);
	
	if(res<0)
		cout<<"\n"<<res<<"  NEGATIVE class\n";
	else
		cout<<"\n"<<res<<"  POSITIVE class\n";
	
	cout<<"SUCCESS\n";
	return 0;
}
