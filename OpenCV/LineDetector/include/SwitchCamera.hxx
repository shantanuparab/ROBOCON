#pragma once

// To Access PiCamera
// We Shall use the Brilliantly Designed RasPiCam class
// You could find it at
// https://www.uco.es/investiga/grupos/ava/node/40
#if !defined(__has_include) || __has_include(<raspicam/raspicam_cv.h>)
#   define PI_CAM_AVAILABLE
#   include <raspicam/raspicam_cv.h>
#else
#   undef PI_CAM_AVAILABLE
#endif

// To Access Video Camera
// Provided by OpenCV
#include <opencv2/videoio/videoio.hpp>

namespace Camera
{
   // As of Now We Support Only 2 Types of Cameras
   // PiCam
   // And Non-PI Cam
   enum class Type
   {
      PI,
      STILL_PI,
      NON_PI
   };

   // In Case PiCamera is not available
#ifndef PI_CAM_AVAILABLE
   // Use this just to ensure something is defined
   // And No Error is Thrown by Compiler
   // This is because we may sometimes like to work
   // With the same code in a system that
   // Is Not a Raspberry Pi
   // And Would like to Test out the code
   namespace
   {
      namespace raspicam
      {
         using RaspiCam_Cv       = std::void_t<>;
         using RaspiCam_Still_Cv = std::void_t<>;
      } // namespace raspicam
   }    // namespace
#endif

   template <Type cam_type>
   struct SwitchCameraCV
   {
      using Device = std::conditional_t<cam_type == Type::PI,
                                        raspicam::RaspiCam_Cv,
                                        // As condition is not Pi, Check for Still Pi
                                        std::conditional_t<cam_type == Type::STILL_PI,
                                                           raspicam::RaspiCam_Still_Cv,
                                                           // Condition is Neither Pi nor Still Pi,
                                                           // it is Non-Pi
                                                           cv::VideoCapture>>;
      Device m_camera;

      template <typename... Args>
      SwitchCameraCV(Args... p_cam_args) : m_camera{std::forward<Args>(p_cam_args)...}
      {
      }
      ~SwitchCameraCV()
      {
         // On Destroy, Release Given Object
         release();
      }

      SwitchCameraCV& release()
      {
         m_camera.release();
         return *this;
      }

      template <Type Cam = cam_type>
      std::enable_if_t<Cam == Type::PI || Cam == Type::STILL_PI, bool /*Return Value on Success*/>
          open()
      {
         // Note that this Peculiar Overload
         // Needs to be performed because
         // RaspiCam does not open Camera Interface
         // Until open function is called
         // However, VideoCapture does this on it's own
         // Instantly
         return m_camera.open();
      }
      template <Type Cam = cam_type>
      std::enable_if_t<Cam == Type::NON_PI, bool /*Return Value on Success*/> open()
      {
         // Note that this Peculiar Overload
         // Needs to be performed because
         // RaspiCam does not open Camera Interface
         // Until open function is called
         // However, VideoCapture does this on it's own
         // Within the Constructor
         return isOpened();
      }
      template <typename Arg, typename... Args, Type Cam = cam_type>
      std::enable_if_t<Cam == Type::NON_PI, bool /*Return Value on Success*/> open(Arg arg,
                                                                                   Args... rest)
      {
         // Note that this Peculiar Overload
         // Needs to be performed because
         // RaspiCam does not open Camera Interface
         // Until open function is called
         // However, VideoCapture does this on it's own
         // Within the Constructor
         // This However does not mean, VideoCapture
         // Does not use it's own open function
         // Which the user might require to use someday
         return m_camera.open(arg, std::forward<Args>(rest)...);
      }

      bool isOpened()
      {
         return m_camera.isOpened();
      }

      SwitchCameraCV& set(int const prop, double const value)
      {
         m_camera.set(prop, value);
         return *this;
      }
      template<typename T = double/*get returns double value*/>
      T get(int const prop)
      {
         return static_cast<T>(m_camera.get(prop));
      }

      // Set the Resolution
      // For Example 640X480
      SwitchCameraCV& Resolution(std::uint16_t const width, std::uint16_t const height)
      {
         set(CV_CAP_PROP_FRAME_WIDTH, width);
         set(CV_CAP_PROP_FRAME_HEIGHT, height);

         return *this;
      }
      SwitchCameraCV& Resolution(cv::Size_<std::uint16_t> const& p_resolution)
      {
         return Resolution(p_resolution.width, p_resolution.height);
      }

      // Returns a std::pair of Elements
      // First Element is Width
      // Second Element is Height
      auto Resolution()
      {
         auto const width  = get<std::uint16_t>(CV_CAP_PROP_FRAME_WIDTH);
         auto const height = get<std::uint16_t>(CV_CAP_PROP_FRAME_HEIGHT);

         return std::make_pair(width, height);
      }

      // Set Format for PiCamera Only
      template <Type Cam = cam_type>
      std::enable_if_t<Cam == Type::PI || Cam == Type::STILL_PI, SwitchCameraCV&>
          Format(double const p_format)
      {
         // This only works on for PiCamera
         return set(CV_CAP_PROP_FORMAT, p_format);
      }

      // Note that NON-Pi Camera does not support
      // Format Command
      // As such, in a Normal Camera,
      // This function does nothing
      template <Type Cam = cam_type>
      std::enable_if_t<Cam == Type::NON_PI, SwitchCameraCV&>
          Format(double const /*As Operation is Unsupported*/)
      {
         // Normal VideoCapture does not support this operation
         return *this;
      }

      // Note that grab captures and stores the image
      // Use retrieve to get Image back
      bool grab()
      {
         return m_camera.grab();
      }
      SwitchCameraCV& retrieve(cv::OutputArray p_img)
      {
         m_camera.retrieve(p_img);
         return *this;
      }
      // Note that Read Function is Implemented
      // By VideoCapture as a simple
      // Grab and Retrieve
      // According to the Documentation
      // However, RasPiCam_CV still does not implement
      // Read Function
      // So we will have to implement a Read Function Ourselves

      template <Type Cam = cam_type>
      std::enable_if_t<Cam == Type::PI || Cam == Type::STILL_PI, bool> read(cv::OutputArray img)
      {
         // The Implementation here is quite similar to OpenCV's own Implementation
         // Check if Grabbing Image was successful
         // If it was, return captured image
         if (grab())
            retrieve(img);
         else
            img.release();
         return !std::empty(img);
      }
      // Note that Read Function is Implemented
      // By VideoCapture as a simple
      // Grab and Retrieve
      // According to the Documentation

      template <Type Cam = cam_type>
      std::enable_if_t<Cam == Type::NON_PI, bool> read(cv::OutputArray img)
      {
         return m_camera.read(img);
      }

      // Use this In case you want to access
      // Any Functions specified by the Camera
      // Which have not been implemented here
      Device& GetUnderlyingCamera()
      {
         return std::addressof(m_camera);
      }
      // Use this function to get the underlying type
      // Of Camera being used currently
      constexpr Type GetUnderlyingType() const noexcept
      {
         return cam_type;
      }
   };
} // namespace Camera