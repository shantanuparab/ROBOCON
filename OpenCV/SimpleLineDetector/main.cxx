#include <iostream>

#include "include/LineDetector.hxx"
#include "include/Window.hxx"

#include "include/Camera.hxx"

#include <chrono>

constexpr const int SPI_CHANNEL   = 0;
constexpr const int SPI_FREQUENCY = 2'000'000;

// Note that the code is primarily sourced from
// http://einsteiniumstudios.com/beaglebone-opencv-line-following-robot.html

int main()
{
   // Get Default Camera
   Pi::Camera<Pi::CameraType::NON_PI> camera{0};

   camera.setFormat(CV_8UC3);
   camera.setResolution(160, 120);

   camera.open();

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
   props.setColourBounds1(cv::Scalar{0, 0, 210}, cv::Scalar{180, 60, 255});
   //.setColourBounds2(cv::Scalar(140, 0, 210), cv::Scalar(180, 255, 255))

   Detector::Detector detector{props};

   cv::Mat    src;
   UI::Window source{"src"};
   cv::Point  line_center;

   int count = 0;

   while (camera.isOpened())
   {
      const auto prev_time = std::chrono::system_clock::now().time_since_epoch();
      camera.grabAndRetrieve(src);
      const auto cur_read_time = std::chrono::system_clock::now().time_since_epoch();

	  const auto millis_read_time = std::chrono::duration_cast<std::chrono::milliseconds>(cur_read_time-prev_time).count();

      if (std::empty(src))
         break;

      if (detector.LineCenterPosition(src, line_center))
         cv::circle(src, line_center, 3, {0, 0, 255});
      else
         continue;

      const auto cur_time = std::chrono::system_clock::now().time_since_epoch();

      const auto millis_spent =
          std::chrono::duration_cast<std::chrono::milliseconds>(cur_time - prev_time).count();

      if (millis_spent > 40)
      {
         std::cout << "Time Spent" << (millis_spent-millis_read_time) << " " << count << " Read Portion " << millis_read_time << '\n';
         count = 0;
      }
      else
         ++count;

      source.displayImage(src);
      source.move(800, 0);

      if (source.waitKey(5) == 'q')
         break;
   }

   return 0;
}