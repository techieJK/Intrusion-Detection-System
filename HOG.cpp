#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

string posSamplesDir="pos/"; //Directory containing positive samples
string negSamplesDir="neg/"; //Directory containing negative samples

// HOG parameters for training
const Size trainingPadding=Size(0,0);
const Size winStride=Size(8,8);

void calculateFeaturesFromInput(const string& imageFilename, vector<float>& featureVector, HOGDescriptor& hog)
{
    Mat imageData=imread(imageFilename,IMREAD_GRAYSCALE);
	
	if(imageData.empty()) //Check for error in reading file
    {
        featureVector.clear();
        cout<<"Error "<<imageFilename;
        return;
    }
    if(imageData.cols!=hog.winSize.width || imageData.rows!=hog.winSize.height) //Check for mismatching resolution
    {
        featureVector.clear();
        cout<<"Error: "<<imageFilename<<" resolution"<<imageData.cols<<" "<<imageData.rows<<" do not match HOG window size!\n";
        return;
    }

    vector<Point> locations;
    hog.compute(imageData,featureVector,winStride,trainingPadding,locations); //Compute feature vectors
    imageData.release(); //Release the image again after features are extracted
}

int main()
{
    HOGDescriptor hog;
    hog.winSize=Size(96,160); //Training images resolution
    
    static vector<string> positiveTrainingImages;
    static vector<string> negativeTrainingImages;
    
    int n;
    string fname;
    cout<<"Enter no. of pos samples: ";
    cin>>n;
    for(int i=0;i<n;++i)
    {
    	cin>>fname;
    	positiveTrainingImages.push_back(posSamplesDir+fname);
    }
    cout<<"Enter no. of neg samples: ";
    cin>>n;
    for(int i=0;i<n;++i)
    {
    	cin>>fname;
    	negativeTrainingImages.push_back(negSamplesDir+fname);
    }
    
    unsigned long overallSamples=positiveTrainingImages.size()+negativeTrainingImages.size();
    cout<<"Generating HOG features and saving them in \'genfiles/features.dat\'\n";
    float percent;
    
    fstream File;
    File.open("genfiles/features.dat",ios::out);
    if(File.good() && File.is_open())
    {
        for(unsigned long currentFile=0;currentFile<overallSamples;++currentFile)
        {
            vector<float> featureVector;
            // Get file path
            const string currentFileName = (currentFile<positiveTrainingImages.size())? positiveTrainingImages[currentFile]:negativeTrainingImages[currentFile-positiveTrainingImages.size()];
            
            // Output progress
            if((currentFile+1)%10==0 || (currentFile+1)==overallSamples) 
            {
                percent=((currentFile+1)*100/overallSamples);
                cout<<percent<<"% done\n";
            }
                       
            calculateFeaturesFromInput(currentFileName,featureVector,hog); //Calculate feature vector from current image file
            if(!featureVector.empty()) 
            {
                File<<((currentFile<positiveTrainingImages.size())? "+1":"-1");
                for(unsigned int feature=0;feature<featureVector.size();++feature) //Save feature vector
                {
                    File<<" "<<feature<<": "<<featureVector[feature];
                }
                File<<'\n';
            }
        }
        cout<<"\n";
        File.flush();
        File.close();
    }
    else
    {
        cout<<"Error opening file \'genfiles/features.dat\'\n";
        return 0;
    }

    return 0;
}
