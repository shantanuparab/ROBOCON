#pragma once

#include <opencv2/highgui/highgui.hpp>

namespace UI
{
   struct Window
   {
      using SizeFlags = cv::WindowFlags;
      using Property  = cv::WindowPropertyFlags;

    private:
      const cv::String m_window_id;

    public:
      Window(const cv::String& p_window_id,
             const SizeFlags   p_flag = SizeFlags::WINDOW_AUTOSIZE) noexcept :
          m_window_id{p_window_id}
      {
         cv::namedWindow(m_window_id, p_flag);
      }
      Window& resize(const cv::Size& p_size)
      {
         cv::resizeWindow(m_window_id, p_size);
         return *this;
      }
      Window& resize(const std::int16_t p_x, const std::int16_t p_y)
      {
         cv::resizeWindow(m_window_id, p_x, p_y);
         return *this;
      }

      Window& setTitle(const cv::String& p_title)
      {
         cv::setWindowTitle(m_window_id, p_title);
         return *this;
      }

      Window& addProperty(const Property& p_flag, const SizeFlags& p_val)
      {
         cv::setWindowProperty(m_window_id, p_flag, p_val);
         return *this;
      }
      auto getProperty(const Property& p_id)
      {
         return (cv::getWindowProperty(m_window_id, p_id));
      }

      Window& addMouseCallbackHandler(cv::MouseCallback& p_handler,
                                      void* const        p_user_data = nullptr)
      {
         cv::setMouseCallback(m_window_id, p_handler, p_user_data);
         return *this;
      }

      Window& move(const std::int16_t p_x, const std::int16_t p_y)
      {
         cv::moveWindow(m_window_id, p_x, p_y);
         return *this;
      }

      Window& displayImage(const cv::Mat& p_img)
      {
         if (!std::empty(p_img))
            cv::imshow(m_window_id, p_img);
         return *this;
      }
      cv::Rect getAsRect()
      {
         return cv::getWindowImageRect(m_window_id);
      }

      void destroy()
      {
         return cv::destroyWindow(m_window_id);
      }

      ~Window()
      {
         destroy();
      }

      static void destroyAllWindows()
      {
         cv::destroyAllWindows();
      }

      static auto waitKey(const std::uint16_t p_delay_ms = 0 /*Wait Forever*/)
      {
         return cv::waitKey(p_delay_ms);
      }
      // Returns Full Key Error Code
      static auto waitKeyEx(const std::uint16_t p_delay_ms = 0 /*Wait Forever*/)
      {
         return cv::waitKeyEx(p_delay_ms);
      }
   };
} // namespace UI
