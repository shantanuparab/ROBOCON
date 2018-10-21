#include <optional>
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
      cv::Scalar m_lower_colour_bound{0, 0, 0};
      cv::Scalar m_higher_colour_bound{0, 0, 0};

      // Store a Pair of Values
      // These Values Represent Canny Thresholds
      std::uint32_t m_canny_threshold1;
      std::uint32_t m_canny_threshold2;

      // Add Characteristics as and when required

    public:
      // Note provide Limits as HSV
      Characteristics& setColourBounds(const cv::Scalar& p_lower_colour_bound,
                                       const cv::Scalar& p_higher_colour_bound) noexcept
      {
         m_lower_colour_bound  = p_lower_colour_bound;
         m_higher_colour_bound = p_higher_colour_bound;
         return *this;
      }
      cv::Scalar getLowerColourBounds() const noexcept
      {
         return m_lower_colour_bound;
      }
      cv::Scalar getHigherColourBounds() const noexcept
      {
         return m_higher_colour_bound;
      }

      Characteristics& setCannyThreshold(const std::uint32_t p_canny_threshold1,
                                         const std::uint32_t p_canny_threshold2)
      {
         m_canny_threshold1 = p_canny_threshold1;
         m_canny_threshold2 = p_canny_threshold2;

         return *this;
      }
      std::uint32_t getCannyThresholdFirst() const noexcept
      {
         return m_canny_threshold1;
      }
      std::uint32_t getCannyThresholdSecond() const noexcept
      {
         return m_canny_threshold2;
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
      std::optional<std::int16_t> LineCenterPosition(const cv::Mat& img_src)
      {
         if (std::empty(img_src))
            return std::nullopt;

         // Processing Done as No 2 Images are the Same
         // Ever. Hence after Processing and removing
         // Unnecessary details, we get a real clean image
         // This clean Image contains exactly the data we need

         auto img_proc = processImage(img_src);
         if (std::empty(img_proc))
            return std::nullopt;

         cv::Canny(img_proc,
                   img_proc,
                   m_obj_detect_properties.getCannyThresholdFirst(),
                   m_obj_detect_properties.getCannyThresholdSecond());

         std::vector<std::vector<cv::Point>> line_points;
         cv::findContours(img_proc, line_points, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

         if (std::empty(line_points))
            return std::nullopt;

         // We Shall work under the Assumption that the Largest Line
         // Is the Object We want to Detect
         const auto line_detected =
             (*std::max_element(std::cbegin(line_points),
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

         const auto centroidX = static_cast<std::int16_t>(moments.m10 / moments.m00);
         // const auto centroidY = moments.m01 / moments.m00;

         return centroidX;
      }

    private:
      // Applies Blurr, inRange dilate and erode to make the image better and more visible
      // Original Image Assumed to be in BGR Format
      // Also performs Background Subtraction
      inline cv::Mat processImage(const cv::Mat& img_src)
      {

         if (std::empty(img_src))
            return cv::Mat(); // Returns Empty Matrix

         cv::Mat img_out;

         // Source is
         // https://www.opencv-srf.com/2010/09/object-detection-using-color-seperation.html

         // Convert Original Image to HSV Thresh Image
         cv::cvtColor(img_src, img_out, cv::ColorConversionCodes::COLOR_BGR2HSV);

         cv::inRange(img_out,
                     m_obj_detect_properties.getLowerColourBounds(),
                     m_obj_detect_properties.getHigherColourBounds(),
                     img_out);

         const cv::Size kSize{3, 3};

         // blur effect
         cv::GaussianBlur(img_out, img_out, kSize, 0);

         const auto structuring_elem = cv::getStructuringElement(cv::MORPH_ELLIPSE, kSize);

         // morphological opening (remove small objects from the foreground)
         cv::erode(img_out, img_out, structuring_elem);
         cv::dilate(img_out, img_out, structuring_elem);

         // morphological closing (fill small holes in the foreground)
         cv::dilate(img_out, img_out, structuring_elem);
         cv::erode(img_out, img_out, structuring_elem);

         return img_out;
      }
   };
} // namespace Detector