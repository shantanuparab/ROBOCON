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

      double   m_clahe_clip_limit = 40;
      cv::Size m_clahe_tiles_grid = {15, 15};

      // Add Properties as and when required

    public:
      // Provide Fine Tuned
      // HLS Limits
      // To Detect Object of Given Colour
      Properties& addColourBounds(cv::Scalar const&& p_lower_colour_bound,
                                  cv::Scalar const&& p_higher_colour_bound) noexcept
      {
         m_colour_bounds.emplace_back(p_lower_colour_bound, p_higher_colour_bound);
         return *this;
      }

      // Sets the HLS Bounds
      // For a given colour
      // From a predefined HLS Bound Set
      // To use your own Limits
      // Use addColourBounds instead
      constexpr Properties& FindColour(Detector::Colour const p_colour)
      {
         // Colour Bounds must be HLS
         // Check out
         // https://en.wikipedia.org/wiki/File:HSL_color_solid_cylinder_saturation_gray.png
         // To Find the Colour Ranges
         switch (p_colour)
         {
            // TODO: Set L Min Value for White

            // From the above Image Provided we find
            // that White Has
            // Hue from 0-180 // Complete
            // Saturation from 0-255 // Complete
            // But occupies a very high level of Lightness
            // From a certain minimum level to Max Level
            case Detector::Colour::WHITE:
               // http://answers.opencv.org/question/93899/hsv-range-for-rubiks-cube-color/
               return addColourBounds(
                   {0 /*H Min*/, 230 /*L Min. Set Value*/, 0 /*S Min*/} /*Lower*/,
                   {180 /*H Max*/, 255 /*L Max*/, 255 /*S Max*/} /*Higher*/);
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

      // Set the Clip Limit for the CLAHE Algorithm
      Properties& CLAHEClipLimit(double const p_clip_limit) noexcept
      {
         m_clahe_clip_limit = p_clip_limit;
         return *this;
      }
      double CLAHEClipLimit() const noexcept
      {
         return m_clahe_clip_limit;
      }
      Properties& CLAHETilesGrid(cv::Size const& p_clahe_tiles_grid) noexcept
      {
         m_clahe_tiles_grid = p_clahe_tiles_grid;
         return *this;
      }
      cv::Size CLAHETilesGrid() const noexcept
      {
         return m_clahe_tiles_grid;
      }
   };

   struct Line
   {
    private:
      using Contour = std::vector<cv::Point>;
      using Image   = cv::UMat;

    private:
      Properties const m_properties;

    public:
      Line(Properties const& p_obj_detect_properties) :
          m_properties{std::move(p_obj_detect_properties)}
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
      bool Centroid(Image const& src /*Input*/, Detector::Point* p_centroid /*Output*/) const;

    private:
      inline Contour FindLargestContourByArea(cv::InputOutputArray img) const;
      // Please Refer to http://www.aishack.in/tutorials/image-moments/
      inline bool CalculateContourCentroidByMoment(cv::InputArray   p_contour,
                                                   Detector::Point* p_centroid,
                                                   bool const       is_binary = true) const;
      // Original Image Assumed to be in BGR Format
      // Processes the Image
      // Performs all Operations required to smooth it
      // And make Object Detection etc. simpler
      inline Image processImage(Image const& src) const;

      // Correct the Illumination of the Given Image
      // Assumes Image is Non-Empty & BGR
      inline void CorrectIllumination(cv::InputOutputArray p_img) const;

      // Applies InRange Function on Image
      // Extract the Objects which are within the Range
      inline void InRangeImage(cv::InputArray p_src, cv::OutputArray p_dest) const;
   };
} // namespace Detector