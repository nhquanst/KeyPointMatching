#include <iostream>
#include "opencv2/core.hpp"
#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using std::endl;
const char* keys =
    "{ help h |                          | Print help message. }"
    "{ input1 | ../data/box.png          | Path to input image 1. }"
    "{ input2 | ../data/box_in_scene.png | Path to input image 2. }";
int main( int argc, char* argv[] )
{
    CommandLineParser parser( argc, argv, keys );
    Mat img1 = imread( parser.get<String>("input1"), IMREAD_COLOR );
    Rect bbox1=selectROI(img1,false);
    Mat mask1 = Mat::zeros(img1.size(), CV_8U);  // type of mask is CV_8U
    Mat roi1(mask1, bbox1);
    roi1 = Scalar(255, 255, 255);

    Mat img2 = imread( parser.get<String>("input2"), IMREAD_COLOR );
    Rect bbox2=selectROI(img2,false);
    Mat mask2 = Mat::zeros(img2.size(), CV_8U);  // type of mask is CV_8U
    Mat roi2(mask2, bbox2);
    roi2 = Scalar(255, 255, 255);

    if ( img1.empty() || img2.empty() )
    {
        cout << "Could not open or find the image!\n" << endl;
        parser.printMessage();
        return -1;
    }
    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    int minHessian = 20000;
    Ptr<SURF> detector = SURF::create( minHessian );
    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    detector->detectAndCompute( img1, mask1, keypoints1, descriptors1);
    detector->detectAndCompute( img2, mask2, keypoints2, descriptors2 );
    //-- Step 2: Matching descriptor vectors with a FLANN based matcher
    // Since SURF is a floating-point descriptor NORM_L2 is used
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector< std::vector<DMatch> > knn_matches;
    matcher->knnMatch( descriptors1, descriptors2, knn_matches, 2 );
    //-- Filter matches using the Lowe's ratio test
    float ratio_thresh = 0.05f;
    std::vector<DMatch> good_matches;
    while (ratio_thresh<1.0){
        good_matches.clear();
        for (size_t i = 0; i < knn_matches.size(); i++)
        {
            if (knn_matches[i].size() > 1 && knn_matches[i][0].distance / knn_matches[i][1].distance <= ratio_thresh)
            {
                good_matches.push_back(knn_matches[i][0]);
                std::cout<<knn_matches[i][0].distance<<endl;
            }
        }
        if (good_matches.size()>2){
            std::cout<<ratio_thresh<<endl;
            break;
        }
        ratio_thresh=ratio_thresh+0.05;
    }
    //-- Draw matches
    Mat img_matches;
    drawMatches( img1, keypoints1, img2, keypoints2, good_matches, img_matches, Scalar::all(-1),
                 Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    //-- Show detected matches
    imshow("Good Matches", img_matches );
    waitKey();
    return 0;
}
#else
int main()
{
    std::cout << "This tutorial code needs the xfeatures2d contrib module to be run." << std::endl;
    return 0;
}
#endif
