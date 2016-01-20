#ifndef PIC_PIXEL_HPP
#define PIC_PIXEL_HPP

#include <Pic/Channels.hpp>
#include <type_traits>
#include <string.h> //for memcpy

namespace pic
{
    template<class C>
    class STICK_API Pixel : public C
    {
    public:

        typedef C Channels;

        typedef typename Channels::ValueType ValueType;

        typedef typename Channels::ChannelLayout ChannelLayout;

        typedef stick::TypeInfoT<Pixel> TypeInfo;


        Pixel()
        {

        }

        explicit Pixel(ValueType * _ptr)
        {
            setValue(_ptr);
        }

        explicit Pixel(ValueType _val)
        {
            setValue(_val);
        }

        Pixel(ValueType _a, ValueType _b)
        {
            setValue(_a, _b);
        }

        Pixel(ValueType _a, ValueType _b, ValueType _c)
        {
            setValue(_a, _b, _c);
        }

        Pixel(ValueType _a, ValueType _b, ValueType _c, ValueType _d)
        {
            setValue(_a, _b, _c, _d);
        }

        bool operator == (const Pixel & _other)
        {
            for (stick::UInt32 i = 0; i < ChannelLayout::channelCount(); ++i)
            {
                if (channel(i) != _other.channel(i))
                    return false;
            }

            return true;
        }

        bool operator != (const Pixel & _other)
        {
            return !(*this == _other);
        }

        void setValue(const ValueType * _ptr)
        {
            memcpy(ptr(), _ptr, sizeof(ValueType) * ChannelLayout::channelCount());
        }

        void setValue(ValueType _val)
        {
            ValueType arr[ChannelLayout::s_channelCount] = {_val};
            setValue(arr);
        }

        void setValue(ValueType _a, ValueType _b)
        {
            ValueType arr[ChannelLayout::s_channelCount] = { _a, _b };
            setValue(arr);
        }

        void setValue(ValueType _a, ValueType _b, ValueType _c)
        {
            ValueType arr[ChannelLayout::s_channelCount] = { _a, _b, _c };
            setValue(arr);
        }

        void setValue(ValueType _a, ValueType _b, ValueType _c, ValueType _d)
        {
            ValueType arr[ChannelLayout::s_channelCount] = { _a, _b, _c, _d };
            setValue(arr);
        }

        void setChannelValue(stick::UInt32 _channel, const ValueType & _val)
        {
            channel(_channel) = _val;
        }

        const ValueType & channel(stick::UInt32 _channel) const
        {
            return *(ptr() + _channel);
        }

        ValueType & channel(stick::UInt32 _channel)
        {
            return *(ptr() + _channel);
        }

        static stick::UInt32 channelCount()
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

        static bool isFloatingPoint()
        {
            return std::is_floating_point<ValueType>();
        }

        static bool hasAlpha()
        {
            return ChannelLayout::hasAlpha();
        }

        static stick::Int32 alphaPosition()
        {
            return ChannelLayout::alphaPosition();
        }

        static stick::TypeID pixelTypeID()
        {
            return TypeInfo::typeID();
        }

        static stick::TypeID valueTypeID()
        {
            return stick::TypeInfoT<ValueType>::typeID();
        }
    };

    typedef Pixel<ChannelsGray8> PixelGray8;
    typedef Pixel<ChannelsGrayAlpha8> PixelGrayAlpha8;
    typedef Pixel<ChannelsAlphaGray8> PixelAlphaGray8;
    typedef Pixel<ChannelsRGB8> PixelRGB8;
    typedef Pixel<ChannelsBGR8> PixelBGR8;
    typedef Pixel<ChannelsRGBA8> PixelRGBA8;
    typedef Pixel<ChannelsBGRA8> PixelBGRA8;
    typedef Pixel<ChannelsARGB8> PixelARGB8;
    typedef Pixel<ChannelsABGR8> PixelABGR8;

    typedef Pixel<ChannelsGray16> PixelGray16;
    typedef Pixel<ChannelsGrayAlpha16> PixelGrayAlpha16;
    typedef Pixel<ChannelsAlphaGray16>PixelAlphaGray16;
    typedef Pixel<ChannelsRGB16> PixelRGB16;
    typedef Pixel<ChannelsBGR16>  PixelBGR16;
    typedef Pixel<ChannelsRGBA16> PixelRGBA16;
    typedef Pixel<ChannelsBGRA16> PixelBGRA16;
    typedef Pixel<ChannelsARGB16> PixelARGB16;
    typedef Pixel<ChannelsABGR16> PixelABGR16;

    typedef Pixel<ChannelsGray32> PixelGray32;
    typedef Pixel<ChannelsGrayAlpha32> PixelGrayAlpha32;
    typedef Pixel<ChannelsAlphaGray32>PixelAlphaGray32;
    typedef Pixel<ChannelsRGB32> PixelRGB32;
    typedef Pixel<ChannelsBGR32>  PixelBGR32;
    typedef Pixel<ChannelsRGBA32> PixelRGBA32;
    typedef Pixel<ChannelsBGRA32> PixelBGRA32;
    typedef Pixel<ChannelsARGB32> PixelARGB32;
    typedef Pixel<ChannelsABGR32> PixelABGR32;

    typedef Pixel<ChannelsGray32f> PixelGray32f;
    typedef Pixel<ChannelsGrayAlpha32f> PixelGrayAlpha32f;
    typedef Pixel<ChannelsAlphaGray32f>PixelAlphaGray32f;
    typedef Pixel<ChannelsRGB32f> PixelRGB32f;
    typedef Pixel<ChannelsBGR32f>  PixelBGR32f;
    typedef Pixel<ChannelsRGBA32f> PixelRGBA32f;
    typedef Pixel<ChannelsBGRA32f> PixelBGRA32f;
    typedef Pixel<ChannelsARGB32f> PixelARGB32f;
    typedef Pixel<ChannelsABGR32f> PixelABGR32f;
}

#endif //PIC_PIXEL_HPP
