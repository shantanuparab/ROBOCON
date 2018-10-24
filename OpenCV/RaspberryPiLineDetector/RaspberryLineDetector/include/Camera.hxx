#pragma once

// To Access PiCamera
#include <raspicam/raspicam_cv.h>
// To Access Web Camera
#include <opencv2/videoio/videoio.hpp>

namespace Pi
{
   enum class CameraType
   {
      PI,
      NON_PI
   };

   template <CameraType cam_type = CameraType::PI>
   struct Camera
   {
      using Type = std::
          conditional_t<cam_type == CameraType::PI, raspicam::RaspiCam_Cv, cv::VideoCapture>;
      Type m_camera;

      // As Pi Camera has no default constructor
      template <bool B = cam_type == CameraType::PI, typename = std::enable_if_t<B>>
      Camera() : m_camera{}
      {
         open();
      }
      // As WebCam Camera has these Constructor
      template <bool B = cam_type == CameraType::NON_PI, typename = std::enable_if_t<B>>
      Camera(const std::int16_t p_selected_cam) : m_camera{p_selected_cam}
      {
         open();
      }
      ~Camera()
      {
         // On Destroy, Release Given Object
         release();
      }

      Camera& release()
      {
         m_camera.release();
         return *this;
      }

      bool open()
      {
         return m_camera.open();
      }
      bool isOpened()
      {
         return m_camera.isOpened();
      }

      Camera& set(const int prop, const double value)
      {
         m_camera.set(prop, value);
         return *this;
      }

      // Set the Resolution
      // For Example 640X480
      Camera& setResolution(const uint16_t width, const uint16_t height)
      {
         set(CV_CAP_PROP_FRAME_WIDTH, width);
         set(CV_CAP_PROP_FRAME_HEIGHT, height);

         return *this;
      }

      Camera& setFormat(const double p_format)
      {
         return set(CV_CAP_PROP_FORMAT, p_format);
      }

      double get(const int prop)
      {
         return m_camera.get(prop);
      }

      // Note that grab captures and stores the image
      // Use retreive to get Image back
      bool grab()
      {
         return m_camera.grab();
      }
      Camera& retrieve(cv::Mat& p_img)
      {
         m_camera.retrieve(p_img);
         return *this;
      }
      Camera& grabAndRetrieve(cv::Mat& img)
      {
         // Check if Grabbing Image was successfull
         // If it was, return captured image
         if (grab())
            return retrieve();

         // If However that fails, set image to empty
         img = cv::Mat {}
         return *this;
      }
   };
} // namespace Pi