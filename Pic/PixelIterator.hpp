#ifndef PIC_PIXELITERATOR_HPP
#define PIC_PIXELITERATOR_HPP

#include <Stick/Platform.hpp>
#include <Stick/Utility.hpp>
#include <cstdlib>

namespace pic
{
    template<class T>
    class PixelIteratorT
    {
    public:

        typedef T Image;

        typedef typename Image::Pixel Pixel;

        typedef typename Image::ValueType ValueType;


        inline PixelIteratorT() :
            m_image(nullptr),
            m_pos(0)
        {

        }

        inline PixelIteratorT(Image & _image) :
            m_image(&_image),
            m_pos(reinterpret_cast<char *>(_image.ptr())),
            m_currentX(0)
        {

        }

        inline PixelIteratorT(Image & _image, stick::Size _pixelOff) :
            m_image(&_image),
            m_pos(reinterpret_cast<char *>(_image.ptr())),
            m_currentX(0)
        {
            increment(_pixelOff);
        }

        inline void decrement(stick::Size _n)
        {
            STICK_ASSERT(m_image);
            stick::Int64 noff = m_currentX - _n;
            if (noff < 0)
            {
                auto anoff = std::abs (noff);
                stick::Size div = anoff / m_image->width() + 1;
                m_currentX = m_image->width() - anoff % m_image->width();
                m_pos -= div * m_image->rowPadding() + _n * sizeof (Pixel);
            }
            else
            {
                m_pos -= _n * sizeof (Pixel);
                m_currentX = noff;
            }
        }

        inline void increment(stick::Size _n)
        {
            STICK_ASSERT(m_image);
            stick::Size noff = m_currentX + _n;
            if (noff > m_image->width() - 1)
            {
                stick::Size div = noff / m_image->width();
                m_currentX = noff % m_image->width();
                m_pos += div * m_image->rowPadding() + _n * sizeof (Pixel);
            }
            else
            {
                m_pos += _n * sizeof (Pixel);
                m_currentX = noff;
            }
        }

        //pre increment
        inline PixelIteratorT & operator++()
        {
            increment(1);
            return *this;
        }

        //post increment
        inline PixelIteratorT operator++(int) const
        {
            const_cast<PixelIteratorT *>(this)->increment(1);
            return *this;
        }

        //pre decrement
        inline PixelIteratorT & operator--()
        {
            decrement(1);
            return *this;
        }

        //post decrement
        inline PixelIteratorT operator--(int) const
        {
            const_cast<PixelIteratorT *>(this)->decrement(1);
            return *this;
        }

        inline PixelIteratorT operator + (stick::Size _s) const
        {
            PixelIteratorT ret(*this);
            ret.increment(_s);
            return ret;
        }

        inline PixelIteratorT operator - (stick::Size _s) const
        {
            PixelIteratorT ret(*this);
            ret.decrement(_s);
            return ret;
        }

        inline PixelIteratorT & operator += (stick::Size _off)
        {
            increment (_off);
            return *this;
        }

        inline PixelIteratorT & operator -= (stick::Size _off)
        {
            decrement (_off);
            return *this;
        }

        inline bool operator==(const PixelIteratorT & _other) const
        {
            return m_pos == _other.m_pos;
        }

        inline bool operator!=(const PixelIteratorT & _other) const
        {
            return !(*this == _other);
        }

        inline bool operator < (const PixelIteratorT & _other)  const
        {
            return m_pos < _other.m_pos;
        }

        inline bool operator > (const PixelIteratorT & _other)  const
        {
            return m_pos > _other.m_pos;
        }

        inline Pixel & operator*()
        {
            STICK_ASSERT(m_image);
            return *reinterpret_cast<Pixel *>(m_pos);
        }

        inline const Pixel & operator*() const
        {
            STICK_ASSERT(m_image);
            return *reinterpret_cast<const Pixel *>(m_pos);
        }

        inline Pixel * operator->()
        {
            STICK_ASSERT(m_image);
            return reinterpret_cast<Pixel *>(m_pos);
        }

        inline const Pixel * operator->() const
        {
            STICK_ASSERT(m_image);
            return reinterpret_cast<const Pixel *>(m_pos);
        }


    private:

        Image * m_image;

        char * m_pos;

        stick::Size m_currentX;
    };
}

#endif //PIC_PIXELITERATOR_HPP
