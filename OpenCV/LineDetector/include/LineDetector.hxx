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

   struct Properties
   {
    private:
      // Define the Lower and Higher Colour Bounds
      std::vector<std::pair<cv::Scalar const /*Lower*/, cv::Scalar const /*Higher*/>>
          m_colour_bounds;

      // Region of Interest in Image To Be Considered for Processing
      cv::Rect m_roi;

      // Add Properties as and when required

    public:
      // Provide Fine Tuned
      // HSV Limits
      // To Detect Object of Given Colour
      Properties& addColourBounds(cv::Scalar const&& p_lower_colour_bound,
                                  cv::Scalar const&& p_higher_colour_bound) noexcept
      {
         m_colour_bounds.emplace_back(p_lower_colour_bound, p_higher_colour_bound);
         return *this;
      }

      // Sets the HSV Bounds
      // For a given colour
      // From a predefined HSV Bound Set
      // To use your own Limits
      // Use addColourBounds instead
      constexpr Properties& FindColour(Detector::Colour const p_colour)
      {
         // Colour Bounds must be HSV
         switch (p_colour)
         {
            case Detector::Colour::WHITE:
               // http://answers.opencv.org/question/93899/hsv-range-for-rubiks-cube-color/
               return addColourBounds({0, 0, 225} /*Lower*/, {180, 25, 255} /*Higher*/);
         }
      }
      auto ColourBounds() const noexcept
      {
         return m_colour_bounds;
      }

      // Set The Image Region of Interest
      Properties& ROI(cv::Rect const& p_bounding_area)
      {
         m_roi = std::move(p_bounding_area);
         return *this;
      }
      // Set the Image Region of Interest from the Resolution
      // Only Consider Upper Half
      // By Default
      Properties&
          ROIHalf(std::pair<std::uint16_t /*Width*/, std::uint16_t /*Height*/> const&& p_resolution)
      {
         // Set Upper Half as the
         // Region of Interest of Image
         return ROI(cv::Rect{0 /*X-Axis Initial*/,
                             0 /*Y-Axis Initial*/,
                             p_resolution.first /*Width*/,
                             (p_resolution.second / 2) /*Height*/});
      }
      // Returns the Region of Interest
      cv::Rect ROI() const noexcept
      {
         return m_roi;
      }
   };

   struct Line
   {
    private:
      using Contour = std::vector<cv::Point>;
      using Image   = cv::Mat;

    private:
      Properties const m_obj_detect_properties;

    public:
      Line(Properties const& p_obj_detect_properties) :
          m_obj_detect_properties{std::move(p_obj_detect_properties)}
      {
      }

      // This Function Detects the Centroid of the Line
      // Returns True On Success
      // With the Value of Centroid reference being
      // updated
      // Note that the code is primarily sourced from
      // http://einsteiniumstudios.com/beaglebone-opencv-line-following-robot.html
      // Using an optional type would be more appropriate
      // But std::optional was introduced in C++17
      bool Centroid(Image const& src /*Input*/, Detector::Point& p_centroid /*Output*/) const;

    private:
      inline Contour FindLargestContourByArea(cv::InputOutputArray img) const;
      // Please Refer to http://www.aishack.in/tutorials/image-moments/
      inline bool CalculateContourCentroidByMoment(cv::InputOutputArray p_contour,
                                                   Detector::Point&     p_centroid,
                                                   bool const           is_binary = true) const;
      // Original Image Assumed to be in BGR Format
      // Processes the Image
      // Performs all Operations required to smooth it
      // And make Object Detection etc. simpler
      inline Image processImage(Image const& src) const;
   };
} // namespace Detector