#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include<fstream>
#include<iostream>
using namespace std;
using namespace cv;
using namespace cv::ml;

int main()
{
	fstream  f;
	f.open("a.txt"); //dataset
	int labels[800];
	float trainingData[800][8];
    int ct=0;
	while(f>>labels[ct])
	{
		f>>trainingData[ct][0]>>trainingData[ct][1]>>trainingData[ct][2]>>trainingData[ct][3]>>trainingData[ct][4]>>trainingData[ct][5]>>trainingData[ct][6]>>trainingData[ct][7];
		ct++;
	}

    Mat labelsMat(ct, 1, CV_32SC1, labels);
    Mat trainingDataMat(ct, 8, CV_32FC1, trainingData);

    // Train the SVM
 	Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	svm->train(trainingDataMat, ROW_SAMPLE, labelsMat);

	float testData[8];
	cout<<"\nEnter test Data:\n ";
	for(int i=0;i<8;i++)
		cin>>testData[i];
		
	float response;
            Mat query = (Mat_<float>(1,8)<<testData[0],testData[1],testData[2],testData[3],testData[4],testData[5],testData[6],testData[7]);
            response = svm->predict(query);

	if(response<0)
		cout<<"\nNEGATIVE class\n";
	else
		cout<<"\nPOSITIVE class\n";
}
