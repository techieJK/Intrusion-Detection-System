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

int main()
{
	int j=readData(); //Read training data
	Mat labelsMat(j,1,CV_32SC1,labels);
	Mat trainingDataMat(j,7524,CV_32FC1,trainingData);
   
	// Set up SVM
	Ptr<SVM> svm=SVM::create();
	svm->setCoef0(0.0);
	svm->setDegree(3);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-3));
	svm->setGamma(0);
	svm->setKernel(SVM::LINEAR);
	svm->setNu(0.5);
	svm->setP(0.1); // for EPSILON_SVR, epsilon in loss function?
	svm->setC(0.01); // From paper, soft classifier
	svm->setType(SVM::EPS_SVR); // C_SVC; // EPSILON_SVR; // may be also NU_SVR; // do regression task
	svm->train(trainingDataMat,ROW_SAMPLE,labelsMat);
	

	svm->save("savefile.xml");
	
	cout<<"Training Done\n";
	return 0;
}
