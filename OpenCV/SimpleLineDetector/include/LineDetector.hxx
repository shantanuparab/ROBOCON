#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>

#include "Window.hxx"

namespace Detector
{
   struct Characteristics
   {
    private:
      // Define the Lower and Higher Colour Bounds
      cv::Scalar m_lower_colour_bound1{0, 0, 0};
      cv::Scalar m_higher_colour_bound1{0, 0, 0};
      cv::Scalar m_lower_colour_bound2{0, 0, 0};
      cv::Scalar m_higher_colour_bound2{0, 0, 0};

      // Add Characteristics as and when required

    public:
      // Note provide Limits as HSV
      Characteristics& setColourBounds1(const cv::Scalar& p_lower_colour_bound1,
                                        const cv::Scalar& p_higher_colour_bound1) noexcept
      {
         m_lower_colour_bound1  = p_lower_colour_bound1;
         m_higher_colour_bound1 = p_higher_colour_bound1;
         return *this;
      }
      // Note provide Limits as HSV
      Characteristics& setColourBounds2(const cv::Scalar& p_lower_colour_bound2,
                                        const cv::Scalar& p_higher_colour_bound2) noexcept
      {
         m_lower_colour_bound2  = p_lower_colour_bound2;
         m_higher_colour_bound2 = p_higher_colour_bound2;
         return *this;
      }
      cv::Scalar getLowerColourBounds1() const noexcept
      {
         return m_lower_colour_bound1;
      }
      cv::Scalar getHigherColourBounds1() const noexcept
      {
         return m_higher_colour_bound1;
      }
      cv::Scalar getLowerColourBounds2() const noexcept
      {
         return m_lower_colour_bound2;
      }
      cv::Scalar getHigherColourBounds2() const noexcept
      {
         return m_higher_colour_bound2;
      }
   };

   struct Detector
   {
    private:
      Characteristics m_obj_detect_properties;

    public:
      Detector(const Characteristics& p_obj_detect_properties) :
          m_obj_detect_properties{p_obj_detect_properties}
      {
      }
      inline Detector& setCharacteristics(const Characteristics& p_obj_detect_properties)
      {
         m_obj_detect_properties = p_obj_detect_properties;
         return *this;
      }

      // Note that the code is primarily sourced from
      // http://einsteiniumstudios.com/beaglebone-opencv-line-following-robot.html
      bool LineCenterPosition(cv::Mat& img_src, cv::Point& line_center)
      {
         if (std::empty(img_src))
            return false;

         // Processing Done as No 2 Images are the Same
         // Ever. Hence after Processing and removing
         // Unnecessary details, we get a real clean image
         // This clean Image contains exactly the data we need

         img_src= cv::Mat(img_src, cv::Rect{0, 60, 160, 60});

         cv::Mat img_proc;
         processImage(img_src, img_proc);
         if (std::empty(img_proc))
            return false;

         // cv::Canny(img_proc,
         //          *img_proc,
         //          m_obj_detect_properties.getCannyThresholdFirst(),
         //          m_obj_detect_properties.getCannyThresholdSecond());

         std::vector<std::vector<cv::Point>> line_points;
         cv::findContours(img_proc, line_points, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		 if(std::empty(line_points))
			 return false;

         // We Shall work under the Assumption that the Largest Line
         // Is the Object We want to Detect
         const auto line_detected =
             *(std::max_element(std::cbegin(line_points),
                                std::cend(line_points),
                                [](const auto& p_left, const auto& p_right) {
                                   // Sort in Descending order of area
                                   return cv::contourArea(p_left) < cv::contourArea(p_right);
                                }));
         // Line Detected is the Largest Contour Found
         // As such we calculate the Moment to find out
         // It's centroid
         // Please Refer to http://www.aishack.in/tutorials/image-moments/
         const auto moments = cv::moments(line_detected /*Contour of Detected Line*/,
                                          true /*Assume is Binary Image*/);

         line_center.x = moments.m10 / moments.m00;
         line_center.y = moments.m01 / moments.m00;

         return true;
      }

    private:
      // Applies Blurr, inRange dilate and erode to make the image better and more visible
      // Original Image Assumed to be in BGR Format
      // Also performs Background Subtraction
      inline void processImage(const cv::Mat& img_src, cv::Mat& img_out)
      {
         if (std::empty(img_src))
            return; // Returns Empty Matrix

         // Source is
         // https://www.opencv-srf.com/2010/09/object-detection-using-color-seperation.html

         // Convert Original Image to HSV Thresh Image
         cv::cvtColor(img_src, img_out, cv::ColorConversionCodes::COLOR_BGR2HSV);

         {
            cv::Mat img_1;
            cv::inRange(img_out,
                        m_obj_detect_properties.getLowerColourBounds1(),
                        m_obj_detect_properties.getHigherColourBounds1(),
                        img_out);
            // cv::Mat img_2;
            // cv::inRange(img_out,
            //            m_obj_detect_properties.getLowerColourBounds2(),
            //            m_obj_detect_properties.getHigherColourBounds2(),
            //            img_2);
            // img_out = img_1 | img_2;
         }
         // UI::Window window{"aabcd"};
         // window.displayImage(img_out);
         // window.move(800, 0);
         // window.waitKey(250);

         static const cv::Size kSize{1, 1};

         // blur effect
         cv::GaussianBlur(img_out, img_out, kSize, 0);
         // cv::pyrUp(img_out, img_out);
         /*
         const auto structuring_elem = cv::getStructuringElement(cv::MORPH_ELLIPSE, kSize);

         // morphological opening (remove small objects from the foreground)
         cv::erode(img_out, img_out, structuring_elem);
         cv::dilate(img_out, img_out, structuring_elem);

         // morphological closing (fill small holes in the foreground)
         cv::dilate(img_out, img_out, structuring_elem);
         cv::erode(img_out, img_out, structuring_elem);
       */
      }
   };
} // namespace Detector