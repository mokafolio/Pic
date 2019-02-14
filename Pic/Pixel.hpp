#ifndef PIC_PIXEL_HPP
#define PIC_PIXEL_HPP

#include <Pic/Channels.hpp>
#include <string.h> //for memcpy
#include <type_traits>

namespace pic
{
template <class T, class ChannelLayoutT>
class STICK_API BasePixelT
{
  public:
    using ChannelLayout = ChannelLayoutT;
    using ValueType = T;

    bool operator==(const BasePixelT & _other) const
    {
        for (stick::UInt32 i = 0; i < ChannelLayout::channelCount(); ++i)
        {
            if (channel(i) != _other.channel(i))
                return false;
        }

        return true;
    }

    bool operator!=(const BasePixelT & _other) const
    {
        return !(*this == _other);
    }

    void setValue(const ValueType * _ptr)
    {
        memcpy(ptr(), _ptr, sizeof(ValueType) * ChannelLayout::channelCount());
    }

    void setChannelValue(stick::UInt32 _channel, const ValueType & _val)
    {
        channel(_channel) = _val;
    }

    const ValueType & channel(stick::UInt32 _channel) const
    {
        STICK_ASSERT(_channel < channelCount());
        return *(ptr() + _channel);
    }

    ValueType & channel(stick::UInt32 _channel)
    {
        STICK_ASSERT(_channel < channelCount());
        return *(ptr() + _channel);
    }

    static constexpr stick::UInt32 channelCount()
    {
        return ChannelLayout::channelCount();
    }

    stick::TypeID channelLayoutTypeID()
    {
        return ChannelLayout::channelLayoutTypeID();
    }

    const ValueType * ptr() const
    {
        return reinterpret_cast<const ValueType *>(this);
    }

    ValueType * ptr()
    {
        return reinterpret_cast<ValueType *>(this);
    }

    static constexpr bool isFloatingPoint()
    {
        return std::is_floating_point<ValueType>();
    }

    static constexpr bool hasAlpha()
    {
        return ChannelLayout::hasAlpha();
    }

    static constexpr stick::Int32 alphaPosition()
    {
        return ChannelLayout::alphaPosition();
    }

    static constexpr stick::TypeID pixelTypeID()
    {
        // return TypeInfo::typeID();
        return 0;
    }

    static constexpr stick::TypeID valueTypeID()
    {
        return stick::TypeInfoT<ValueType>::typeID();
    }

    void printShit() const
    {
        printf("SHIT\n");
    }
};

template <class T, template <class> class C0>
class STICK_API Pixel1T : public C0<T>,
                          public BasePixelT<T, ChannelLayout1<typename C0<T>::ChannelFlag>>
{
  public:
    using BasePixel = BasePixelT<T, ChannelLayout1<typename C0<T>::ChannelFlag>>;
    using BasePixel::setValue;

    typedef T ValueType;
    typedef ChannelLayout1<typename C0<T>::ChannelFlag> ChannelLayout;
    using TypeInfo = stick::TypeInfoT<Pixel1T>;

    Pixel1T()
    {
    }

    Pixel1T(ValueType _val)
    {
        setValue(_val);
    }

    void setValue(ValueType _val)
    {
        this->setChannelValue(0, _val);
    }
};

template <class T, template <class> class C0, template <class> class C1>
class STICK_API Pixel2T
    : public C0<T>,
      public C1<T>,
      public BasePixelT<T, ChannelLayout2<typename C0<T>::ChannelFlag, typename C1<T>::ChannelFlag>>
{
  public:
    using BasePixel =
        BasePixelT<T, ChannelLayout2<typename C0<T>::ChannelFlag, typename C1<T>::ChannelFlag>>;
    using BasePixel::setValue;

    typedef T ValueType;
    typedef ChannelLayout2<typename C0<T>::ChannelFlag, typename C1<T>::ChannelFlag> ChannelLayout;
    using TypeInfo = stick::TypeInfoT<Pixel2T>;

    Pixel2T()
    {
    }

    Pixel2T(ValueType _val)
    {
        setValue(_val);
    }

    Pixel2T(ValueType _a, ValueType _b)
    {
        setValue(_a, _b);
    }

    void setValue(ValueType _val)
    {
        ValueType arr[2] = { _val };
        setValue(arr);
    }

    void setValue(ValueType _a, ValueType _b)
    {
        this->setChannelValue(0, _a);
        this->setChannelValue(1, _b);
    }
};

template <class T, template <class> class C0, template <class> class C1, template <class> class C2>
class STICK_API Pixel3T : public C0<T>,
                          public C1<T>,
                          public C2<T>,
                          public BasePixelT<T,
                                            ChannelLayout3<typename C0<T>::ChannelFlag,
                                                           typename C1<T>::ChannelFlag,
                                                           typename C2<T>::ChannelFlag>>
{
  public:
    using BasePixel = BasePixelT<T,
                                 ChannelLayout3<typename C0<T>::ChannelFlag,
                                                typename C1<T>::ChannelFlag,
                                                typename C2<T>::ChannelFlag>>;
    using BasePixel::setValue;

    typedef T ValueType;
    typedef ChannelLayout3<typename C0<T>::ChannelFlag,
                           typename C1<T>::ChannelFlag,
                           typename C2<T>::ChannelFlag>
        ChannelLayout;
    using TypeInfo = stick::TypeInfoT<Pixel3T>;

    Pixel3T()
    {
    }

    Pixel3T(ValueType _val)
    {
        setValue(_val);
    }

    Pixel3T(ValueType _a, ValueType _b)
    {
        setValue(_a, _b);
    }

    Pixel3T(ValueType _a, ValueType _b, ValueType _c)
    {
        setValue(_a, _b, _c);
    }

    void setValue(ValueType _val)
    {
        ValueType arr[3] = { _val };
        setValue(arr);
    }

    void setValue(ValueType _a, ValueType _b)
    {
        this->setChannelValue(0, _a);
        this->setChannelValue(1, _b);
    }

    void setValue(ValueType _a, ValueType _b, ValueType _c)
    {
        this->setChannelValue(0, _a);
        this->setChannelValue(1, _b);
        this->setChannelValue(2, _c);
    }
};

template <class T,
          template <class> class C0,
          template <class> class C1,
          template <class> class C2,
          template <class> class C3>
class STICK_API Pixel4T : public C0<T>,
                          public C1<T>,
                          public C2<T>,
                          public C3<T>,
                          public BasePixelT<T,
                                            ChannelLayout4<typename C0<T>::ChannelFlag,
                                                           typename C1<T>::ChannelFlag,
                                                           typename C2<T>::ChannelFlag,
                                                           typename C3<T>::ChannelFlag>>
{
  public:
    using BasePixel = BasePixelT<T,
                                 ChannelLayout4<typename C0<T>::ChannelFlag,
                                                typename C1<T>::ChannelFlag,
                                                typename C2<T>::ChannelFlag,
                                                typename C3<T>::ChannelFlag>>;
    using BasePixel::setValue;

    typedef T ValueType;
    typedef ChannelLayout4<typename C0<T>::ChannelFlag,
                           typename C1<T>::ChannelFlag,
                           typename C2<T>::ChannelFlag,
                           typename C3<T>::ChannelFlag>
        ChannelLayout;
    using TypeInfo = stick::TypeInfoT<Pixel4T>;

    Pixel4T()
    {
    }

    Pixel4T(ValueType _val)
    {
        setValue(_val);
    }

    Pixel4T(ValueType _a, ValueType _b)
    {
        setValue(_a, _b);
    }

    Pixel4T(ValueType _a, ValueType _b, ValueType _c)
    {
        setValue(_a, _b, _c);
    }

    Pixel4T(ValueType _a, ValueType _b, ValueType _c, ValueType _d)
    {
        setValue(_a, _b, _c, _d);
    }

    void setValue(ValueType _val)
    {
        ValueType arr[3] = { _val };
        setValue(arr);
    }

    void setValue(ValueType _a, ValueType _b)
    {
        this->setChannelValue(0, _a);
        this->setChannelValue(1, _b);
    }

    void setValue(ValueType _a, ValueType _b, ValueType _c)
    {
        this->setChannelValue(0, _a);
        this->setChannelValue(1, _b);
        this->setChannelValue(2, _c);
    }

    void setValue(ValueType _a, ValueType _b, ValueType _c, ValueType _d)
    {
        this->setChannelValue(0, _a);
        this->setChannelValue(1, _b);
        this->setChannelValue(2, _c);
        this->setChannelValue(3, _d);
    }
};

using PixelGray8 = Pixel1T<stick::UInt8, Gray>;
using PixelGrayAlpha8 = Pixel2T<stick::UInt8, Gray, Alpha>;
using PixelAlphaGray8 = Pixel2T<stick::UInt8, Alpha, Gray>;
using PixelRGB8 = Pixel3T<stick::UInt8, Red, Green, Blue>;
using PixelBGR8 = Pixel3T<stick::UInt8, Blue, Green, Red>;
using PixelRGBA8 = Pixel4T<stick::UInt8, Red, Green, Blue, Alpha>;
using PixelBGRA8 = Pixel4T<stick::UInt8, Blue, Green, Red, Alpha>;
using PixelARGB8 = Pixel4T<stick::UInt8, Alpha, Red, Green, Blue>;
using PixelABGR8 = Pixel4T<stick::UInt8, Alpha, Blue, Green, Red>;

using PixelGray16 = Pixel1T<stick::UInt16, Gray>;
using PixelGrayAlpha16 = Pixel2T<stick::UInt16, Gray, Alpha>;
using PixelAlphaGray16 = Pixel2T<stick::UInt16, Alpha, Gray>;
using PixelRGB16 = Pixel3T<stick::UInt16, Red, Green, Blue>;
using PixelBGR16 = Pixel3T<stick::UInt16, Blue, Green, Red>;
using PixelRGBA16 = Pixel4T<stick::UInt16, Red, Green, Blue, Alpha>;
using PixelBGRA16 = Pixel4T<stick::UInt16, Blue, Green, Red, Alpha>;
using PixelARGB16 = Pixel4T<stick::UInt16, Alpha, Red, Green, Blue>;
using PixelABGR16 = Pixel4T<stick::UInt16, Alpha, Blue, Green, Red>;

using PixelGray32 = Pixel1T<stick::UInt32, Gray>;
using PixelGrayAlpha32 = Pixel2T<stick::UInt32, Gray, Alpha>;
using PixelAlphaGray32 = Pixel2T<stick::UInt32, Alpha, Gray>;
using PixelRGB32 = Pixel3T<stick::UInt32, Red, Green, Blue>;
using PixelBGR32 = Pixel3T<stick::UInt32, Blue, Green, Red>;
using PixelRGBA32 = Pixel4T<stick::UInt32, Red, Green, Blue, Alpha>;
using PixelBGRA32 = Pixel4T<stick::UInt32, Blue, Green, Red, Alpha>;
using PixelARGB32 = Pixel4T<stick::UInt32, Alpha, Red, Green, Blue>;
using PixelABGR32 = Pixel4T<stick::UInt32, Alpha, Blue, Green, Red>;

using PixelGray32f = Pixel1T<stick::Float32, Gray>;
using PixelGrayAlpha32f = Pixel2T<stick::Float32, Gray, Alpha>;
using PixelAlphaGray32f = Pixel2T<stick::Float32, Alpha, Gray>;
using PixelRGB32f = Pixel3T<stick::Float32, Red, Green, Blue>;
using PixelBGR32f = Pixel3T<stick::Float32, Blue, Green, Red>;
using PixelRGBA32f = Pixel4T<stick::Float32, Red, Green, Blue, Alpha>;
using PixelBGRA32f = Pixel4T<stick::Float32, Blue, Green, Red, Alpha>;
using PixelARGB32f = Pixel4T<stick::Float32, Alpha, Red, Green, Blue>;
using PixelABGR32f = Pixel4T<stick::Float32, Alpha, Blue, Green, Red>;
} // namespace pic

#endif // PIC_PIXEL_HPP
