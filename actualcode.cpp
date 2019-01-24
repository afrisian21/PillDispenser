#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main() {
int cameraport = 0;
VideoCapture stream(cameraport);   //0 is the id of video device.0 if you have only one camera.

if (!stream.isOpened()) { //check if video device has been initialised
cout << "Camera can't be found on port " << cameraport<<endl;
}

//else
	//exit(3);

Mat LastFrame, AdjustedFrame;


while (true) {
stream.read(LastFrame);
cv::rotate(LastFrame, AdjustedFrame, cv::ROTATE_90_COUNTERCLOCKWISE);
imshow("Camera Preview", AdjustedFrame);
if (waitKey(16)==32)
{
imwrite("PictureTakentou.jpg",LastFrame);
cout<<"Picture was taken"<<endl;
break;
}
}
return 0;
}
