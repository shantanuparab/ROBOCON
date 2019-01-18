#include "../include/LineDetector.hxx"

namespace Detector
{
   // This Function Detects the Centroid of the Line
   // Returns True On Success
   // With the Value of Centroid reference being
   // updated
   // Note that the code is primarily sourced from
   // http://einsteiniumstudios.com/beaglebone-opencv-line-following-robot.html
   // Using an optional type would be more appropriate
   // But std::optional was introduced in C++17

   bool Line::Centroid(Image const& src, Detector::Point& p_centroid) const
   {
      // ProcessImage Function Extracts Data from Image
      // That We Require
      // In an Average Image for our Task, most of the Data
      // can be discarded
      Image const proc = processImage(src);
      if (std::empty(proc))
         return false;

      // The Largest Contour is the Line Detected
      // This is a major assumption that we have made
      // And would have to rely on
      Contour const line_detected = FindLargestContourByArea(proc);

      // Check if any contour was actually found
      if (std::empty(line_detected))
         return false;

      // Line Detected is the Largest Contour Found
      // As such we calculate the Moment to find out
      // It's centroid
      // Please Refer to http://www.aishack.in/tutorials/image-moments/

      return CalculateContourCentroidByMoment(line_detected, p_centroid);
   }

   inline Line::Contour Line::FindLargestContourByArea(cv::InputOutputArray img) const
   {
      // Stores List of All Detected Contours
      std::vector<Contour> contours;
      // TODO: Verify if RETR_EXTERNAL is Proper
      // Or If RETR_LIST is More Suitable
      // Or If Using CHAIN_APPROX_NONE would be somewhat even more appropriate
      cv::findContours(img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

      // Check if Contour Detected
      if (std::empty(contours))
         return Contour{};

      // Return the Largest Contour Detected by Area
      Contour const largest_by_area = *(std::max_element(
          std::begin(contours), std::end(contours), [](auto const& p_left, auto const& p_right) {
             // Search in Descending order of area
             return cv::contourArea(p_left) < cv::contourArea(p_right);
          }));
      return largest_by_area;
   }

   // Please Refer to http://www.aishack.in/tutorials/image-moments/

   inline bool Line::CalculateContourCentroidByMoment(cv::InputOutputArray p_contour,
                                                      Detector::Point&     p_centroid,
                                                      bool const           is_binary) const
   {
      // Store the moment of the Object
      auto const moment = cv::moments(std::move(p_contour), is_binary);

      // Denominator cannot be Zero
      // Chance of this happening is improbable at best
      // TODO: Check if we should keep this check
      // Removing this might save us an instruction
      // Keeping it might raise Safety
      if (!std::isfinite(moment.m00) || moment.m00 == 0)
         return false;

      p_centroid.x = (Detector::Point::value_type)(moment.m10 / moment.m00);
      p_centroid.y = (Detector::Point::value_type)(moment.m01 / moment.m00);

      return true;
   }

   // Original Image Assumed to be in BGR Format
   // Processes the Image
   // Performs all Operations required to smooth it
   // And make Object Detection etc. simpler

   inline Line::Image Line::processImage(Line::Image const& src) const
   {
      if (std::empty(src))
         return Image(); // Returns Empty Image

      // We only need to Operate within the ROI
      Image const roi = std::empty(m_obj_detect_properties.ROI())
                            // If No Bounding Area Specified
                            ? src
                            // Extract Image based on Bounding Area
                            : cv::Mat(src, m_obj_detect_properties.ROI());

      // TODO:- Verify if Adding Threshold( cv::threshold) is required here
      // Or Would prove to be productive

      // TODO:- verify if Blurring is Required Here
      // And Verify which Blurring. Gaussian/median

      // Source is
      // https://www.opencv-srf.com/2010/09/object-detection-using-color-seperation.html

      // Convert Original Image to HSV Thresh Image
      Image hsv;
      cv::cvtColor(roi /*Input*/, hsv /*Output*/, cv::ColorConversionCodes::COLOR_BGR2HSV);

      // Note that we are using CV_8UC1
      // Because the Resultant Output Image of inRange
      // Will Contain only 1 Single Dimension
      Image output = cv::Mat::zeros(hsv.rows, hsv.cols, CV_8UC1);

      // Extract only Portions present in
      // Given Bounding Ranges
      for (auto const& bounds : m_obj_detect_properties.ColourBounds())
      {
         Image mask;
         // Extract Mask present in Range
         cv::inRange(
             hsv /*Input*/, bounds.first /*Lower*/, bounds.second /*Higher*/, mask /*Output*/);
         // Add mask to Output Image
         output |= mask;
      }

      // TODO:- Verify if Gaussian Blur Required
      // Gaussian Blur takes a lot of time

      //{
      //   // TODO: Verify if this is Appropriate Size
      //   // For the Given Kernel Size
      //   // By Checking out Image Results
      //   cv::Size static const kernel_size{3, 3};

      //   // blur effect
      //   cv::GaussianBlur(output, output, kernel_size, 0);

      //   auto static const structuring_elem =
      //       cv::getStructuringElement(cv::MORPH_ELLIPSE, kernel_size);

      //   // TODO: Verify if Morphological Opening is Helpful or Not

      //   // morphological opening (remove small objects from the foreground)
      //   cv::erode(output, output, structuring_elem);
      //   cv::dilate(output, output, structuring_elem);

      //   // morphological closing (fill small holes in the foreground)
      //   cv::dilate(output, output, structuring_elem);
      //   cv::erode(output, output, structuring_elem);
      //}

      // UI::Window window{"aabcd"};
      // window.displayImage(output);
      // window.move(800, 0);
      // window.waitKey(2250);

      return output;
   }
} // namespace Detector