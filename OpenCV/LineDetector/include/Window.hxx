#pragma once

#include <opencv2/highgui/highgui.hpp>

namespace UI
{
   struct Window
   {
      using SizeFlag     = cv::WindowFlags;
      using PropertyFlag = cv::WindowPropertyFlags;

    private:
      cv::String const m_window_id;

      bool const m_destroy;

    public:
      Window(cv::String const& p_window_id,
             bool              p_destroy = true,
             SizeFlag const    p_flag    = SizeFlag::WINDOW_AUTOSIZE) noexcept :
          m_window_id{p_window_id},
          m_destroy{p_destroy}
      {
         cv::namedWindow(m_window_id, p_flag);
      }
      void resize(cv::Size const& p_size) const
      {
         cv::resizeWindow(m_window_id, p_size);
      }
      void resize(int const p_width, int const p_height) const
      {
         cv::resizeWindow(m_window_id, p_width, p_height);
      }

      void setTitle(cv::String const& p_title) const
      {
         cv::setWindowTitle(m_window_id, p_title);
      }

      void Property(PropertyFlag const p_flag, SizeFlag const p_val) const
      {
         cv::setWindowProperty(m_window_id, p_flag, p_val);
      }
      template <typename RetType = double /*Default Window Property Type*/>
      RetType Property(const PropertyFlag p_id) const
      {
         return (RetType)(cv::getWindowProperty(m_window_id, p_id));
      }

      void addMouseCallbackHandler(cv::MouseCallback& p_handler,
                                      void* const        p_user_data = nullptr) const
      {
         cv::setMouseCallback(m_window_id, p_handler, p_user_data);
      }

     void move(int const p_x, int const p_y) const
      {
         cv::moveWindow(m_window_id, p_x, p_y);
      }

      void show(cv::InputArray p_img) const
      {
         if (!std::empty(p_img))
            cv::imshow(m_window_id, p_img);
      }
      cv::Rect asRect() const noexcept
      {
         return cv::getWindowImageRect(m_window_id);
      }

      void destroy() const
      {
         cv::destroyWindow(m_window_id);
      }

      ~Window()
      {
         if (m_destroy)
            destroy();
      }

      static void destroyAllWindows()
      {
         cv::destroyAllWindows();
      }

      static auto waitKey(std::uint16_t const p_delay_ms = 0 /*Wait Forever*/)
      {
         return cv::waitKey(p_delay_ms);
      }
      // Returns Full Key Code
      static auto waitKeyEx(std::uint16_t const p_delay_ms = 0 /*Wait Forever*/)
      {
         return cv::waitKeyEx(p_delay_ms);
      }
   };
} // namespace UI
