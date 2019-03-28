#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

static const std::string OPENCV_WINDOW = "Image window";
//static long long static_count = 0;
string rgb_image_name;
string depth_image_name;

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_rgb, image_sub_depth;
  image_transport::Publisher image_pub_rgb;
  
  // ofstream define
  ofstream ofstream_rgb;
  ofstream ofstream_depth;

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_rgb = it_.subscribe("/zed/rgb/image_raw_color", 1,
      &ImageConverter::imageCbRgb, this);
    image_sub_depth = it_.subscribe("/zed/depth/depth_registered", 1,
      &ImageConverter::imageCbDepth, this);
      
    ofstream_rgb.open("/home/nvidia/SLAM/uav_dataset/dataset1/rgb.txt");
    if(ofstream_rgb.fail())
    {
        cout << "could not open rgb.txt file! " << endl;
//        return -1; /// !!! wrong !!! could not return a value from a constructor
    }
    
    ofstream_rgb << "# color images" << endl;
    ofstream_rgb << "# file: 'dataset1.bag'" << endl;
    ofstream_rgb << "# timestamp filename" << endl;
    
    ofstream_depth.open("/home/nvidia/SLAM/uav_dataset/dataset1/depth.txt");
    if(ofstream_depth.fail())
    {
        cout << "could not open depth.txt file! " << endl;
    }
    
    ofstream_depth << "# depth images" << endl;
    ofstream_depth << "# file: 'dataset1.bag'" << endl;
    ofstream_depth << "# timestamp filename" << endl;
    
//    image_pub_rgb = it_.advertise("/image_converter/output_video", 1);

//    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
//    cv::destroyWindow(OPENCV_WINDOW);
      ofstream_rgb.close();
      ofstream_depth.close();
  }

  void imageCbRgb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    
    // cout the length and width of the image
    std::cout << "image length: " << cv_ptr->image.cols << std::endl;
    std::cout << "image width: " << cv_ptr->image.rows << std::endl;
    std::cout << "image channels: " << cv_ptr->image.channels() << std::endl;
    std::cout << "image type: " << cv_ptr->image.type() << std::endl;
    
    cout << "image timestamp: " << cv_ptr->header.stamp << endl;
    
//    ostringstream os;
//    os << static_count;
//    istringstream is(os.str());
//    is >> rgb_image_name;
//    static_count++;
    
    // set image timestamp as filename
    // rgb image name
    string tmp;
    ostringstream os;
    os << cv_ptr->header.stamp;
    istringstream is(os.str());
    is >> tmp;
    rgb_image_name = tmp.substr(0,17);
    
    ofstream_rgb << rgb_image_name << " " << "rgb/" + rgb_image_name + ".png" << endl;
    
    // save rgb image
    cv::imwrite("/home/nvidia/SLAM/uav_dataset/dataset1/rgb/" + rgb_image_name + ".png", cv_ptr->image);
        
//    // Draw an example circle on the video stream
//    if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
//      cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));
//
//    // Update GUI Window
//    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
//    cv::waitKey(3);
//
//    // Output modified video stream
//    image_pub_rgb.publish(cv_ptr->toImageMsg());
  }
  
  void imageCbDepth(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_depth_ptr;
    try
    {
//      cv_depth_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8); // There is a bug for depth image ???
      cv_depth_ptr = cv_bridge::toCvCopy(msg);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    
    // cout the length and width of the image
    std::cout << "image length: " << cv_depth_ptr->image.cols << std::endl;
    std::cout << "image width: " << cv_depth_ptr->image.rows << std::endl;
    std::cout << "image channels: " << cv_depth_ptr->image.channels() << std::endl;
    std::cout << "image type: " << cv_depth_ptr->image.type() << std::endl;
    
    cout << "image timestamp: " << cv_depth_ptr->header.stamp << endl;
    
//    ostringstream os;
//    os << static_count;
//    istringstream is(os.str());
//    is >> rgb_image_name;
//    static_count++;
    
    // set image timestamp as filename
    // rgb image name
    string tmp;
    ostringstream os;
    os << cv_depth_ptr->header.stamp;
    istringstream is(os.str());
    is >> tmp;
    depth_image_name = tmp.substr(0,17);
    
    ofstream_depth << depth_image_name << " " << "depth/" + depth_image_name + ".png" << endl;

    // save rgb image
    cv::imwrite("/home/nvidia/SLAM/uav_dataset/dataset1/depth/" + depth_image_name + ".png", cv_depth_ptr->image);
        
    // Draw an example circle on the video stream
//    if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
//      cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));

    // Update GUI Window
//    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
//    cv::waitKey(3);

    // Output modified video stream
//    image_pub_rgb.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
