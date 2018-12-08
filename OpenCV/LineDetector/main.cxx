#include <iostream>

#include "include/LineDetector.hxx"
#include "include/Window.hxx"

#include "include/SwitchCamera.hxx"

// Note that the code is primarily sourced from
// http://einsteiniumstudios.com/beaglebone-opencv-line-following-robot.html

// Simple Video Line Detector

//int main()
//{
//    Get Default Camera
//   Camera::SwitchCameraCV<Camera::Type::NON_PI> camera{0};
//
//   camera.Format(CV_8UC3);
//   camera.Resolution(160, 120);
//
//   if (!camera.open())
//   {
//      std::cerr << "Unable to Access Camera";
//      return EXIT_FAILURE;
//   }
//
//   Detector::Characteristics props;
//   props.addColourBounds(Detector::Colour::WHITE).BoundingArea(camera.Resolution());
//
//   Detector::Line  line{props};
//   cv::Mat         src;
//   UI::Window      window{"src"};
//   Detector::Point line_center;
//
//   while (camera.isOpened())
//   {
//      camera.read(src);
//      if (line.Centroid(src, line_center))
//      {
//         std::cout << "Object detected at " << line_center;
//         cv::circle(src, line_center, 5, cv::Scalar{0, 0, 255});
//      }
//      else
//         std::cerr << "No Object Detected";
//
//      window.displayImage(src);
//      UI::Window::waitKey();
//   }
//}

// Simple Timer and Line Detector

// #include <chrono>
// int main()
//{
//   // Get Default Camera
//   Camera::SwitchCameraCV<Camera::Type::NON_PI> camera{0};
//
//   camera.Format(CV_8UC3);
//   camera.Resolution(160, 120);
//
//   if (!camera.open())
//   {
//      std::cerr << "Unable to Access Camera";
// return EXIT_FAILURE;
//   }
//
//   Detector::Characteristics props;
//
//   // Get Colour Values for Yellow from
//   // https://stackoverflow.com/questions/9179189/detect-yellow-color-in-opencv
//   // .setColourBounds(cv::Scalar{20, 70, 70}, cv::Scalar{30, 255, 255})
//   // Get Colour Values for Purple from
//   // https://stackoverflow.com/questions/17474020/finding-exact-hsv-values-of-colors
//   //.setColourBounds2(cv::Scalar(140, 0, 210), cv::Scalar(180, 255, 255))
//   //.setColourBounds(cv::Scalar{40, 50, 30}, cv::Scalar{140, 255, 255})
//   props.addColourBounds(Detector::Colour::WHITE)
//       .BoundingArea(camera.Resolution());
//
//   Detector::Line line{props};
//
//   cv::Mat    src;
//   UI::Window source{"src"};
//
//   Detector::Point line_center;
//
//   std::size_t count = 0;
//
//   while (camera.isOpened())
//   {
//      {
//         auto const prev_time = std::chrono::system_clock::now().time_since_epoch();
//         camera.read(src);
//         auto const cur_read_time = std::chrono::system_clock::now().time_since_epoch();
//
//         auto const millis_read_time =
//             std::chrono::duration_cast<std::chrono::milliseconds>(cur_read_time - prev_time)
//                 .count();
//
//         if (std::empty(src))
//            break;
//
//         if (line.Centroid(src, line_center))
//            cv::circle(src, line_center, 3, {0, 0, 255});
//         else
//         {
//            std::cout << "Object not Detected";
//            continue;
//         }
//         auto const cur_time = std::chrono::system_clock::now().time_since_epoch();
//
//         auto const millis_spent =
//             std::chrono::duration_cast<std::chrono::milliseconds>(cur_time - prev_time).count();
//
//         if (millis_spent > 40)
//         {
//            std::cout << "Time Processing " << (millis_spent - millis_read_time) << " NoOfReads "
//            << count
//                      << " Read Portion " << millis_read_time << '\n';
//            count = 0;
//         }
//         else
//         {
//            ++count;
//         }
//      }
//      source.displayImage(src);
//      source.move(800, 0);
//
//      if (source.waitKey(5) == 'q')
//         break;
//   }
//
//   return 0;
//}

// Simple Image Line Detector

 int main()
{
   Detector::Characteristics props;
   props.addColourBounds(Detector::Colour::WHITE);

   Detector::Line line{props};

   cv::Mat src = cv::imread("path/to/image.ext");
   Detector::Point line_center;

   if (line.Centroid(src, line_center))
      cv::circle(src, line_center, 5, {0, 0, 255});

   UI::Window source{"src"};
   source.displayImage(src);
   UI::Window::waitKey();
}

// Simple Line Detector using Raspberry Pi

//#include "include/SPI.hxx"
//#include <wiringPi.h>
//
//auto constexpr const SPI_CHANNEL   = 0;
//auto constexpr const SPI_FREQUENCY = 500'000;
//
//int main()
//{
//   try
//   {
//      wiringPiSetup();
//
//      Pi::SPI device{SPI_CHANNEL, SPI_FREQUENCY};
//
//      // Get Default Camera
//      Camera::SwitchCameraCV<Camera::Type::NON_PI> camera{0};
//
//      // TODO: Verify Resolution
//      camera.Format(CV_8UC3).Resolution(160, 120);
//
//      if (!camera.isOpened())
//      {
//         std::cerr << "Unable to Access Camera";
//         return 1;
//      }
//
//      Detector::Characteristics props;
//
//      // Get Colour Values for Yellow from
//      // https://stackoverflow.com/questions/9179189/detect-yellow-color-in-opencv
//      // .setColourBounds(cv::Scalar{20, 70, 70}, cv::Scalar{30, 255, 255})
//      // Get Colour Values for Purple from
//      // https://stackoverflow.com/questions/17474020/finding-exact-hsv-values-of-colors
//      //.setColourBounds(cv::Scalar{40, 50, 30}, cv::Scalar{140, 255, 255})
//      //.addColourBounds(cv::Scalar{0, 0, 210}, cv::Scalar{40, 255, 255})
//      //    .addColourBounds(cv::Scalar(140, 0, 210), cv::Scalar(180, 255, 255))
//      props.addColourBounds(Detector::Colour::WHITE).BoundingArea(camera.Resolution());
//
//      Detector::Line line{props};
//
//      cv::Mat    img;
//      UI::Window source{"src"};
//
//      std::cout << "Start";
//
//      // source.displayImage(cv::Mat::zeros({700, 700}, CV_8UC1));
//      while (camera.isOpened())
//      {
//         if (!camera.read(img))
//            break;
//
//         Detector::Point line_center;
//         if (line.Centroid(img, line_center))
//         {
//            cv::circle(img, line_center, 5, {0, 0, 255});
//            device.write(line_center.x);
//            std::cout << "The LineCenter is " << line_center << '\n';
//         }
//         else
//         {
//            std::cerr << "Unable to Detect Image\n";
//            img = cv::Mat::zeros(img.rows, img.cols, img.type());
//         }
//
//         source.displayImage(img);
//         source.move(800, 0);
//         if (source.waitKey(5) == 'q')
//         {
//            break;
//         }
//      }
//   }
//   catch (...)
//   {
//      std::cout << "Exception occured";
//   }
//
//   return 0;
//}