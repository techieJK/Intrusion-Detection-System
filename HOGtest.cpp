#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/ml/ml.hpp>

using namespace cv;
using namespace cv::ml;
using namespace std;

void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector )
{
	// get the support vectors
	Mat sv = svm->getSupportVectors();
	const int sv_total = sv.rows;
	// get the decision function
	Mat alpha, svidx;
	double rho = svm->getDecisionFunction(0, alpha, svidx);

	CV_Assert( alpha.total() == 1 && svidx.total() == 1 && sv_total == 1 );
	CV_Assert( (alpha.type() == CV_64F && alpha.at<double>(0) == 1.) ||
           (alpha.type() == CV_32F && alpha.at<float>(0) == 1.f) );
	CV_Assert( sv.type() == CV_32F );
	hog_detector.clear();

	hog_detector.resize(sv.cols + 1);
	memcpy(&hog_detector[0], sv.ptr(), sv.cols*sizeof(hog_detector[0]));
	hog_detector[sv.cols] = (float)-rho;
}
	
int main()
{
	Ptr<SVM> svm=Algorithm::load<SVM>("savefile.xml");
	vector<float> hog_detector;
	get_svm_detector(svm, hog_detector);
	HOGDescriptor hog(Size(96,160),Size(16,16),Size(8,8),Size(8,8),9);
	hog.setSVMDetector(hog_detector);
	//HOGDescriptor hog;
	//hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	vector<Point> foundLocations;
	Mat window=imread("test.jpg", CV_LOAD_IMAGE_UNCHANGED);
	
    vector<Rect> rects;
    hog.detectMultiScale(window, rects, 0, Size(8,8), Size(32,32), 1.05, 2);
   // hog.detectMultiScale(window, rects);
    size_t i;

    for (i=0; i<rects.size(); i++)
    {
        rectangle( window, rects[i], Scalar(0,255,0), 2);
    }

    imshow("result", window);
    waitKey(0);
	return 0;
}
