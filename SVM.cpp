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

int main()
{
	int j=readData(); //Read training data
	Mat labelsMat(j,1,CV_32SC1,labels);
	Mat trainingDataMat(j,7523,CV_32FC1,trainingData);
   
	// Set up SVM
	Ptr<SVM> svm=SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	svm->train(trainingDataMat,ROW_SAMPLE,labelsMat);
	

/*	cout<<"\nEnter test Data:\n ";
	float testData[8];
	for(i=0;i<8;i++)
		cin>>c>>c>>testData[i];
	
	Mat query = (Mat_<float>(1,8)<<testData[0],testData[1],testData[2],testData[3],testData[4],testData[5],testData[6],testData[7]);

	float res;
	res=svm->predict(query);
	
	if(res<0)
		cout<<"\n"<<res<<"  NEGATIVE class\n";
	else
		cout<<"\n"<<res<<"  POSITIVE class\n";
	*/
	cout<<"SUCCESS\n";
	return 0;
}
