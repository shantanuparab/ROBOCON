#include "../include/LineDetector.hxx"
#include <opencv2/core/ocl.hpp>

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

   bool Line::Centroid(Image const& src, Detector::Point* p_centroid) const
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
                                                      Detector::Point*     p_centroid,
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

      p_centroid->x = (Detector::Point::value_type)(moment.m10 / moment.m00);
      p_centroid->y = (Detector::Point::value_type)(moment.m01 / moment.m00);

      return true;
   }
   // Correct the Illumination of the Given Image
   // Assumes Image is Non-Empty & BGR

   inline void Line::CorrectIllumination(Image& img) const
   {
      // The following code relies on using the CLAHE Algorithm
      // We could use Adapative Histogram but it was found
      // That the Given Algorithm has issues related to accuracy.
      // Read more about CLAHE at
      // https://en.wikipedia.org/wiki/Adaptive_histogram_equalization#Contrast_Limited_AHE

      // Please see
      // https://stackoverflow.com/questions/24341114/simple-illumination-correction-in-images-opencv-c

      // Store the Image in YUV Format
      // YUV Stores Luminosity Component as well
      // In Y Part
      // We apply CLAHE to it

      Image yuv;

      cv::cvtColor(img, yuv, CV_BGR2YUV);

      std::vector<cv::Mat> channels(3 /*Y,U,V*/);
      cv::split(yuv, channels);

      // Application of Clahe Algorithm
      {
         auto clahe = cv::createCLAHE(m_properties.CLAHEClipLimit(), m_properties.CLAHETilesGrid());

         // First Apply Histogram Equalisation on Y
         // TODO:- Verify if required
         // cv::equalizeHist(channels[0], channels[0]);

         // This is done to make it more accurate
         // Apply CLAHE on Y Component
         clahe->apply(channels[0] /*Y*/, channels[0]);
         // Call Destructor
         clahe->collectGarbage();
      }
      // Merge Channels Back
      cv::merge(channels, yuv);

      // Note that cvtColor fails to work
      // In Case OpenCL is Turned On
      // Hence Disable OpenCL Here
      // To make it work
      cv::ocl::setUseOpenCL(false);
      cv::cvtColor(yuv, img, CV_YUV2BGR);
      cv::ocl::setUseOpenCL(true);
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
      Image roi = std::empty(m_properties.ROI())
                      // If No Bounding Area Specified
                      ? src.clone()
                      // Extract Image based on Bounding Area
                      : Image(src, m_properties.ROI());

      // Apply Illumination Correction to ROI
      CorrectIllumination(roi);
      UI::Window windows{"Illuminated", false};
      windows.displayImage(roi);
      windows.move(1400, 0);

      // TODO:- Verify if Adding Threshold( cv::threshold) is required here
      // Or Would prove to be productive

      // TODO:- verify if Blurring is Required Here
      // And Verify which Blurring. Gaussian/median

      // Source is
      // https://www.opencv-srf.com/2010/09/object-detection-using-color-seperation.html

      // Convert Original Image to HLS Thresh Image
      Image hls;
      cv::cvtColor(roi /*Input*/, hls /*Output*/, cv::ColorConversionCodes::COLOR_BGR2HLS);

      // Note that we are using CV_8UC1
      // Because the Resultant Output Image of inRange
      // Will Contain only 1 Single Dimension
      Image output = Image::zeros(hls.rows, hls.cols, CV_8UC1);

      // Extract only Portions present in
      // Given Bounding Ranges
      for (auto const& bounds : m_properties.ColourBounds())
      {
         Image mask;
         // Extract Mask present in Range
         cv::inRange(
             hls /*Input*/, bounds.first /*Lower*/, bounds.second /*Higher*/, mask /*Output*/);
         // Add mask to Output Image
         // Perform Bitwise Or for this purpose
         cv::bitwise_or(output /*Src 1*/, mask /*Src 2*/, output /*Destination*/);
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

      UI::Window window{"Processed", false};
      window.displayImage(output);
      window.move(800, 0);
      // window.waitKey(2250);

      return output;
   }

} // namespace Detector