#pragma once

// To Access PiCamera
// We Shall use the Brilliantly Designed RasPiCam class
// You could find it at
// https://www.uco.es/investiga/grupos/ava/node/40
//#if !defined(__has_include) || __has_include(<raspicam/raspicam_cv.h>)
//#   define PI_CAM_AVAILABLE
//#   include <raspicam/raspicam_cv.h>
//#else
//#   undef PI_CAM_AVAILABLE
//#endif

// To Access Video Camera
// Provided by OpenCV
#include <opencv2/videoio/videoio.hpp>

namespace Camera
{
   // In Case PiCamera is not available
#ifndef PI_CAM_AVAILABLE
   // Use this just to ensure something is defined
   // And No Error is Thrown by Compiler
   // This is because we may sometimes like to work
   // With the same code in a system that
   // Is Not a Raspberry Pi
   // And Would like to Test out the code
   // But the System would not contain the RaspiCam_Cv
   // Library
   // Thus giving us an error
   namespace
   {
      namespace raspicam
      {
         class RaspiCam_Cv;
         class RaspiCam_Still_Cv;
      } // namespace raspicam
   }    // namespace
#endif

   // As of Now We Support Only 2 Types of Cameras
   // PiCam
   // And Non-PI Cam
   enum class Type 
   {
      PI,
      STILL_PI,
      NON_PI
   };
   template <Camera::Type cam_type>
   struct SwitchCameraCV
   {
    private:
       // Device Represents the Type of Camera used
       // Based on the Parameter cam_type
      using Device =
          typename std::conditional<cam_type == Type::PI,
                                    raspicam::RaspiCam_Cv,
                                    // As condition is not Pi, Check for Still Pi
                                    typename std::conditional<cam_type == Type::STILL_PI,
                                                              raspicam::RaspiCam_Still_Cv,
                                                              // Condition is Neither Pi nor Still
                                                              // Pi, it is Non-Pi
                                                              cv::VideoCapture>::type>::type;
      Device m_camera;

    public:
      template <typename... Args>
      // Only VideoCapture supports
      // Multiple Arguments within Constructor
      SwitchCameraCV(Args&&... p_cam_args) : m_camera{std::forward<Args>(p_cam_args)...}
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
      auto open() -> typename std::enable_if<Cam == Type::PI || Cam == Type::STILL_PI,
                                             bool /*Return Type*/>::type

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
      auto open() -> typename std::enable_if<Cam == Type::NON_PI, bool /*Return Type*/>::type
      {
         // Note that this Peculiar Overload
         // Needs to be performed because
         // RaspiCam does not open Camera Interface
         // Until open function is called
         // However, VideoCapture does this on it's own
         // Within the Constructor
         return isOpened();
      }
      // Opens based on the Specified Arguments
      // For More details
      // See VideoCapture's Open Command
      template <typename Arg, typename... Args, Type Cam = cam_type>
      auto open(Arg arg, Args... rest) ->
          typename std::enable_if<Cam == Type::NON_PI, bool /*Return Type*/>::type
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
      template <typename T = double /*get returns double value*/>
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
      // Set the Resolution
      // For Example 640X480
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

      // Set Format Function
      // This may not work/be supported by
      // VideoCapture
      SwitchCameraCV& Format(double const p_format)
      {
         // This only works for PiCamera
         return set(CV_CAP_PROP_FORMAT, p_format);
      }

      // Note that grab captures and stores the image
      // Use retrieve to get Image back
      bool grab()
      {
         return m_camera.grab();
      }
      // Use Retrieve to get the image grabbed by
      // grab back
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

      // Read an input image from the stream
      template <Type Cam = cam_type>
      auto read(cv::OutputArray p_out) ->
          typename std::enable_if<Cam == Type::PI || Cam == Type::STILL_PI,
                                  bool /*Return Type*/>::type
      {
         // The Implementation here is quite similar to OpenCV's own Implementation
         // Check if Grabbing Image was successful
         // If it was, return captured image
         if (grab())
            retrieve(p_out);
         else
            p_out.release();
         return !p_out.empty();
      }
      // Note that Read Function is Implemented
      // By VideoCapture as a simple
      // Grab and Retrieve
      // According to the Documentation

      template <Type Cam = cam_type>
      auto read(cv::OutputArray img) ->
          typename std::enable_if<Cam == Type::NON_PI, bool /*Return Type*/>::type
      {
         return m_camera.read(img);
      }

      // Use this In case you want to access
      // Any Functions specified by the Camera
      // Which have not been implemented here
      Device& GetUnderlyingCamera()
      {
         // Returns a Reference to the Underlying Camera
         // Via this we can modify the Camera Parameters
         // If we are using Functions that are not as
         // Standardised that both Cameras
         // Still provide complete support to them
         return std::addressof(m_camera);
      }
   };
} // namespace Camera