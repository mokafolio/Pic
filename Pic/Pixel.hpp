#ifndef PIC_PIXEL_HPP
#define PIC_PIXEL_HPP

#include <Pic/Channels.hpp>
#include <type_traits>
#include <string.h> //for memcpy

namespace pic
{
    template<class C>
    class STICK_API PixelT : public C
    {
    public:

        typedef C Channels;

        typedef typename Channels::ValueType ValueType;

        typedef typename Channels::ChannelLayout ChannelLayout;

        typedef stick::TypeInfoT<PixelT> TypeInfo;


        PixelT()
        {

        }

        explicit PixelT(ValueType * _ptr)
        {
            setValue(_ptr);
        }

        explicit PixelT(ValueType _val)
        {
            setValue(_val);
        }

        PixelT(ValueType _a, ValueType _b)
        {
            setValue(_a, _b);
        }

        PixelT(ValueType _a, ValueType _b, ValueType _c)
        {
            setValue(_a, _b, _c);
        }

        PixelT(ValueType _a, ValueType _b, ValueType _c, ValueType _d)
        {
            setValue(_a, _b, _c, _d);
        }

        bool operator == (const PixelT & _other)
        {
            for (stick::UInt32 i = 0; i < ChannelLayout::channelCount(); ++i)
            {
                if (channel(i) != _other.channel(i))
                    return false;
            }

            return true;
        }

        bool operator != (const PixelT & _other)
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

    typedef PixelT<ChannelsGray8> PixelGray8;
    typedef PixelT<ChannelsGrayAlpha8> PixelGrayAlpha8;
    typedef PixelT<ChannelsAlphaGray8> PixelAlphaGray8;
    typedef PixelT<ChannelsRGB8> PixelRGB8;
    typedef PixelT<ChannelsBGR8> PixelBGR8;
    typedef PixelT<ChannelsRGBA8> PixelRGBA8;
    typedef PixelT<ChannelsBGRA8> PixelBGRA8;
    typedef PixelT<ChannelsARGB8> PixelARGB8;
    typedef PixelT<ChannelsABGR8> PixelABGR8;

    typedef PixelT<ChannelsGray16> PixelGray16;
    typedef PixelT<ChannelsGrayAlpha16> PixelGrayAlpha16;
    typedef PixelT<ChannelsAlphaGray16>PixelAlphaGray16;
    typedef PixelT<ChannelsRGB16> PixelRGB16;
    typedef PixelT<ChannelsBGR16>  PixelBGR16;
    typedef PixelT<ChannelsRGBA16> PixelRGBA16;
    typedef PixelT<ChannelsBGRA16> PixelBGRA16;
    typedef PixelT<ChannelsARGB16> PixelARGB16;
    typedef PixelT<ChannelsABGR16> PixelABGR16;

    typedef PixelT<ChannelsGray32> PixelGray32;
    typedef PixelT<ChannelsGrayAlpha32> PixelGrayAlpha32;
    typedef PixelT<ChannelsAlphaGray32>PixelAlphaGray32;
    typedef PixelT<ChannelsRGB32> PixelRGB32;
    typedef PixelT<ChannelsBGR32>  PixelBGR32;
    typedef PixelT<ChannelsRGBA32> PixelRGBA32;
    typedef PixelT<ChannelsBGRA32> PixelBGRA32;
    typedef PixelT<ChannelsARGB32> PixelARGB32;
    typedef PixelT<ChannelsABGR32> PixelABGR32;

    typedef PixelT<ChannelsGray32f> PixelGray32f;
    typedef PixelT<ChannelsGrayAlpha32f> PixelGrayAlpha32f;
    typedef PixelT<ChannelsAlphaGray32f>PixelAlphaGray32f;
    typedef PixelT<ChannelsRGB32f> PixelRGB32f;
    typedef PixelT<ChannelsBGR32f>  PixelBGR32f;
    typedef PixelT<ChannelsRGBA32f> PixelRGBA32f;
    typedef PixelT<ChannelsBGRA32f> PixelBGRA32f;
    typedef PixelT<ChannelsARGB32f> PixelARGB32f;
    typedef PixelT<ChannelsABGR32f> PixelABGR32f;
}

#endif //PIC_PIXEL_HPP
