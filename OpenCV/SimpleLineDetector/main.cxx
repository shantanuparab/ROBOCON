#include <iostream>

#include "include/LineDetector.hxx"
#include "include/Window.hxx"

// Note that the code is primarily sourced from
// http://einsteiniumstudios.com/beaglebone-opencv-line-following-robot.html

int main()
{
   cv::VideoCapture video(0);

   if (!video.isOpened())
      return 1;

   Detector::Characteristics props;

   // Get Colour Values for Yellow from
   // https://stackoverflow.com/questions/9179189/detect-yellow-color-in-opencv
   // .setColourBounds(cv::Scalar{20, 70, 70}, cv::Scalar{30, 255, 255})
   // Get Colour Values for Purple from
   // https://stackoverflow.com/questions/17474020/finding-exact-hsv-values-of-colors
   //.setColourBounds(cv::Scalar{40, 50, 30}, cv::Scalar{140, 255, 255})
   props.setColourBounds1(cv::Scalar{0, 0, 200}, cv::Scalar{35, 30, 255})
       .setColourBounds2(cv::Scalar{140, 0, 200}, cv::Scalar{180, 45, 255})
       .setCannyThreshold(250, 300);

   Detector::Detector detector{props};		

   cv::Mat img;
   UI::Window source{"src"};
   while (true)
   {
      cv::Mat src;
      video >> src;

      if (std::empty(src))
         break;

      const auto line_center = detector.LineCenterPosition(src);
      if (line_center.has_value())
      {
         // Draw a Line Between the Two Points
         cv::circle(src, line_center.value(), 5, {0, 0, 255});
         source.displayImage(src);
         source.move(800, 0);
      }
   }
   UI::Window::waitKey();
   return 0;
}