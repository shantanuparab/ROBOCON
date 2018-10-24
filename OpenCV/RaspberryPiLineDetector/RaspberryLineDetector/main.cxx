#include <iostream>

#include "include/LineDetector.hxx"
#include "include/Window.hxx"

#include "include/Camera.hxx"

#include "include/SPI.hxx"
#include <wiringPi.h>

constexpr const int SPI_CHANNEL   = 0;
constexpr const int SPI_FREQUENCY = 2'000'000;

// Note that the code is primarily sourced from
// http://einsteiniumstudios.com/beaglebone-opencv-line-following-robot.html

int main()
{ 
   try
   {
      wiringPiSetup();

      Pi::SPI device{SPI_CHANNEL, SPI_FREQUENCY};

      for (std::int16_t i = 0; i < 790; ++i)
      {
         std::cout << "fgh" << i << '\n';
         if (!device.write(i))
            std::cout << "zoh zn";
         delayMicroseconds(1'000'000);
      }
   }
   catch (...)
   {
      std::cout << "Exception occured";
   }
   
   std::cout << "Start";


   // Set caputre Params
   std::cout << "def";

   // Get Default Camera
   Pi::Camera<Pi::CameraType::NON_PI> camera{0};

   camera.setFormat(CV_8UC3);
   camera.setResolution(320, 240);

   camera.open();

   std::cout << "abc";
   if (!camera.isOpened())
   {
      std::cout << "Unable to Access PiCamera";
      return 1;
   }

      Detector::Characteristics props;

   // Get Colour Values for Yellow from
   // https://stackoverflow.com/questions/9179189/detect-yellow-color-in-opencv
   // .setColourBounds(cv::Scalar{20, 70, 70}, cv::Scalar{30, 255, 255})
   // Get Colour Values for Purple from
   // https://stackoverflow.com/questions/17474020/finding-exact-hsv-values-of-colors
   //.setColourBounds(cv::Scalar{40, 50, 30}, cv::Scalar{140, 255, 255})
   props.setColourBounds1(cv::Scalar{0, 0, 200}, cv::Scalar{180, 255, 255})
       .setCannyThreshold(250, 300);

   Detector::Detector detector{props};

   cv::Mat    src;
   UI::Window source{"src"};
   //source.displayImage(cv::Mat::zeros({700, 700}, CV_8UC1));
   while (camera.isOpened())
   {
      camera.grabAndRetrieve(src);

      if (std::empty(src))
         break;

      cv::Point2f line_center;
      if (detector.LineCenterPosition(src, line_center))
         cv::circle(src, line_center, 5, {0, 0, 255});
      else
         src = cv::Mat::zeros(cv::Size{50, 50}, src.type());

      source.displayImage(src);
      source.move(800, 0);
	  if (source.waitKey(5) == 'q')
	  {

	  }
   }
   std::cout << "Hello Woeld";
   UI::Window::waitKey();

   return 0;
}