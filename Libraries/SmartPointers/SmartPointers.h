#pragma once

#include "Algorithm.h"

template <typename _Type>
class SmartUniquePtr
{
   // The Type Found After Processing
   using SimpleType = underlying_array_type_t<_Type>;

   // The Type of the Raw Pointer
   using RawPtr = SimpleType*;

 private:
   RawPtr m_raw_data = nullptr;

 public:
   bool empty() const
   {
      return m_raw_data == nullptr;
   }

   // Note that We Need Two Reset Functions
   // One for Array and One for Other Types
   // This is because
   // Array Delete Take Place Differently
   void reset()
   {
      if (!empty())
      {
         if (is_array_v<_Type>)
         {
            // As this is for Array
            // Delete All Elements of Array
            delete[] m_raw_data;
         }
         else
         {
            // As this is for Normal Element
            delete m_raw_data;
         }
         m_raw_data = nullptr;
      }
   }

   // Copy the Raw Data Created
   // Works for All types
   explicit SmartUniquePtr(RawPtr* p_raw_data) : m_raw_data{p_raw_data} {}
   // Argument Based Constructor
   // Arguments to Supply to Constructor
   // Enable only for Non Array Types
   template <typename... Args, bool B = is_array_v<_Type>, typename = enable_if_t<!B>>
   SmartUniquePtr(const Args&... p_args) : m_raw_data(new SimpleType((p_args)...))
   {
   }

   // For Array Based Types
   // Take Array Size
   // And Allocate
   template <bool B = is_array_v<_Type>, typename = enable_if_t<B>>
   SmartUniquePtr(const size_t p_size) : m_raw_data(new SimpleType[p_size])
   {
   }

   ~SmartUniquePtr()
   {
      reset();
   }

   RawPtr get() const
   {
      return m_raw_data;
   }
   RawPtr get()
   {
      return m_raw_data;
   }

   // Get the Raw Value present
   _Type& operator*() const
   {
      return *m_raw_data;
   }
   _Type& operator*()
   {
      return *m_raw_data;
   }
   // Get the Operator to Obtain Pointer Functions
   RawPtr operator->() const
   {
      return m_raw_data;
   }
   RawPtr operator->()
   {
      return m_raw_data;
   }

   template <bool B = is_array_v<_Type>, typename = enable_if_t<B>>
   SimpleType& operator[](const size_t pos)
   {
      return get()[pos];
   }
   template <bool B = is_array_v<_Type>, typename = enable_if_t<B>>
   SimpleType& operator[](const size_t pos) const
   {
      return get()[pos];
   }

   // In All Cases, A Move Occurs
   SmartUniquePtr(SmartUniquePtr& sp)
   {
      // Assignment operator
      if (this != &sp) // Avoid self assignment
      {
         reset();
         m_raw_data    = sp.m_raw_data;
         sp.m_raw_data = nullptr;
      }
   }
   SmartUniquePtr& operator=(SmartUniquePtr& sp)
   {
      // Assignment operator
      if (this != &sp) // Avoid self assignment
      {
         reset();
         m_raw_data    = sp.m_raw_data;
         sp.m_raw_data = nullptr;
      }
      return *this;
   }
   SmartUniquePtr<_Type>& operator=(SmartUniquePtr<_Type>&& sp)
   {
      // Assignment operator
      if (this != &sp && !sp.empty()) // Avoid self assignment
      {
         reset();
         m_raw_data    = sp.m_raw_data;
         sp.m_raw_data = nullptr;
      }
      return *this;
   }
   bool operator<(const SmartUniquePtr& right)
   {
      // this acts as left
      return *get() < *right.get();
   }
   bool operator>(const SmartUniquePtr& right)
   {
      // this acts as left
      return !(*this < right);
   }
   bool operator<=(const SmartUniquePtr& right)
   {
      // this acts as left
      return *get() <= *right.get();
   }
   bool operator>=(const SmartUniquePtr& right)
   {
      // this acts as left
      return !(*this >= right);
   }
   bool operator==(const SmartUniquePtr& right)
   {
      // this acts as left
      return *get() == *right.get();
   }
   bool operator!=(const SmartUniquePtr& right)
   {
      // this acts as left
      return !(*this == right);
   }
};