#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Window.hxx"

namespace Detector
{
   // Instead of adding HSV Ranges
   // It would be far nicer if we could
   // Just specify the colour
   // And the Ranges
   // Were automatically added for us
   // As such rather than Specifying the Range
   // In the User Class
   // We would just specify the Colour
   // And let our code here find the range for us
   enum class Colour
   {
      WHITE
   };

   // Use this as Point Type
   // This is Because we will send
   // 16 Bits of Data
   // By Design
   using Point = cv::Point_<std::int16_t>;

   struct Characteristics
   {
    private:
      // Define the Lower and Higher Colour Bounds
      std::vector<std::pair<cv::Scalar const /*Lower*/, cv::Scalar const /*Higher*/>>
          m_colour_bounds;

      // Set the Bounding Area of Image To Be Considered for Processing
      cv::Rect m_bounding_area;

      // Add Characteristics as and when required

    public:
      // Note provide Limits as HSV
      Characteristics& addColourBounds(cv::Scalar const&& p_lower_colour_bound,
                                       cv::Scalar const&& p_higher_colour_bound) noexcept
      {
         m_colour_bounds.emplace_back(p_lower_colour_bound, p_higher_colour_bound);
         return *this;
      }
      constexpr Characteristics& addColourBounds(Detector::Colour const p_colour)
      {
         // Note that Colour Bounds must be HSV
         switch (p_colour)
         {
            case Detector::Colour::WHITE:
               // http://answers.opencv.org/question/93899/hsv-range-for-rubiks-cube-color/
               return addColourBounds(cv::Scalar{0, 0, 225}, cv::Scalar{180, 25, 255});
         }
      }
      auto getColourBounds() const noexcept
      {
         return m_colour_bounds;
      }

      // Set The Image Bounding Area
      Characteristics& BoundingArea(cv::Rect const&& p_bounding_area)
      {
         m_bounding_area = p_bounding_area;
         return *this;
      }
      // Set the Image Bounding Area from the Resolution
      Characteristics& BoundingArea(
          std::pair<std::uint16_t /*Width*/, std::uint16_t /*Height*/> const&& p_resolution)
      {
         // Here we Shall not Mess with the X-Axis
         // Keep both Original X-Axis Position
         // And Width Same
         // However, we Half the X-Axis Height
         // As such currently
         // We Shall Extract Half the Image
         return BoundingArea(cv::Rect{0 /*X-Axis Position*/,
                                      (p_resolution.second / 2) /*Y-Axis Initial*/,
                                      p_resolution.first /*Width*/,
                                      (p_resolution.second / 2) /*Height*/});
      }
      auto BoundingArea() const noexcept
      {
         return m_bounding_area;
      }
   };

   struct Line
   {
    private:
      Characteristics const m_obj_detect_properties;

    public:
      Line(Characteristics const& p_obj_detect_properties) :
          m_obj_detect_properties{p_obj_detect_properties}
      {
      }

      // This Function Detects the Centroid of the Line
      // Note that the code is primarily sourced from
      // http://einsteiniumstudios.com/beaglebone-opencv-line-following-robot.html
      // Using an optional type would be more appropriate
      // But std::optional was introduced in C++17
      bool Centroid(cv::Mat const& img_src, Detector::Point& line_center) const
      {
         // ProcessImage Function Extracts Data from Image
         // That We Require
         // In an Average Image for our Task, most of the Data
         // can be discarded
         auto const img_proc = processImage(img_src);
         if (std::empty(img_proc))
            return false;

         std::vector<std::vector<cv::Point>> line_points;
         // TODO: Verify if RETR_EXTERNAL is Proper
         // Or If RETR_LIST is More Suitable
         // Or If Using CHAIN_APPROX_NONE would be somewhat even more appropriate
         cv::findContours(img_proc, line_points, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

         // No Contour Detected
         if (std::empty(line_points))
            return false;

         // We Shall work under the Assumption that the Largest Line
         // Is the Object We want to Detect
         auto const line_detected =
             *(std::max_element(std::begin(line_points),
                                std::end(line_points),
                                [](auto const& p_left, auto const& p_right) {
                                   // Search in Descending order of area
                                   return cv::contourArea(p_left) < cv::contourArea(p_right);
                                }));

         // Line Detected is the Largest Contour Found
         // As such we calculate the Moment to find out
         // It's centroid
         // Please Refer to http://www.aishack.in/tutorials/image-moments/

         auto const moments = cv::moments(line_detected /*Contour of Detected Line*/,
                                          true /*Assume is Binary Image*/);

         line_center.x = (std::int16_t)(moments.m10 / moments.m00);
         line_center.y = (std::int16_t)(moments.m01 / moments.m00);

         return true;
      }

    private:
      // Applies Blur, inRange dilate and erode to make the image better and more visible
      // Original Image Assumed to be in BGR Format
      inline cv::Mat processImage(cv::Mat const& img_src) const
      {
         if (std::empty(img_src))
            return cv::Mat(); // Returns Empty Matrix

         if (std::empty(m_obj_detect_properties.getColourBounds()))
            return cv::Mat(); // Returns Empty Matrix

         // Extract Image present in the Bounding Area Property
         auto img_bounded = std::empty(m_obj_detect_properties.BoundingArea())
                                ? img_src // If No Bounding Area Specified
                                : cv::Mat(img_src, m_obj_detect_properties.BoundingArea());

         // TODO:- Verify if Adding Threshold( cv::threshold) is required here
         // Or Would prove to be productive

         // Source is
         // https://www.opencv-srf.com/2010/09/object-detection-using-color-seperation.html

         // Convert Original Image to HSV Thresh Image
         cv::Mat img_hsv;
         cv::cvtColor(img_bounded, img_hsv, cv::ColorConversionCodes::COLOR_BGR2HSV);

         // TODO: Check if This works without using cv::Mat::zeroes
         // Note that we are using CV_8UC1
         // Because the Resultant Output Image of inRange
         // Will Contain only 1 Single Dimension
         // Instead of the Regular HSV Image which contains 3
         cv::Mat img_out = cv::Mat::zeros(img_hsv.rows, img_hsv.cols, CV_8UC1);

         // Extract only Portions present in
         // Given Bounding Ranges
         for (auto const& bounds : m_obj_detect_properties.getColourBounds())
         {
            cv::Mat img_in_range;
            cv::inRange(img_hsv, bounds.first /*Lower*/, bounds.second /*Higher*/, img_in_range);
            img_out = img_out | img_in_range;
         }

         // Verify if Gaussian Blur Required
         // Gaussian Blur takes a lot of time

         //{
         //   // TODO: Verify if this is Appropriate Size
         //   // For the Given Kernel Size
         //   // By Checking out Image Results
         //   cv::Size static const kernel_size{3, 3};

         //   // blur effect
         //   cv::GaussianBlur(img_out, img_out, kernel_size, 0);

         //   auto static const structuring_elem =
         //       cv::getStructuringElement(cv::MORPH_ELLIPSE, kernel_size);

         //   // TODO: Verify if Morphological Opening is Helpful or Not

         //   // morphological opening (remove small objects from the foreground)
         //   cv::erode(img_out, img_out, structuring_elem);
         //   cv::dilate(img_out, img_out, structuring_elem);

         //   // morphological closing (fill small holes in the foreground)
         //   cv::dilate(img_out, img_out, structuring_elem);
         //   cv::erode(img_out, img_out, structuring_elem);
         //}

         // UI::Window window{"aabcd"};
         // window.displayImage(img_out);
         // window.move(800, 0);
         // window.waitKey(2250);

         return img_out;
      }
   };
} // namespace Detector