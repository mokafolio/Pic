#ifndef PIC_CHANNELS_HPP
#define PIC_CHANNELS_HPP

#include <Stick/TypeInfo.hpp>

namespace pic
{
struct STICK_API GrayChannel
{
};
struct STICK_API RedChannel
{
};
struct STICK_API GreenChannel
{
};
struct STICK_API BlueChannel
{
};
struct STICK_API AlphaChannel
{
};
struct STICK_API CyanChannel
{
};
struct STICK_API MagentaChannel
{
};
struct STICK_API YellowChannel
{
};
struct STICK_API BlackChannel
{
};

template <class T>
struct STICK_API Gray
{
    using ValueType = T;
    typedef GrayChannel ChannelFlag;

    T v;
};

template <class T>
struct STICK_API Red
{
    using ValueType = T;
    typedef RedChannel ChannelFlag;

    T r;
};

template <class T>
struct STICK_API Green
{
    using ValueType = T;
    typedef GreenChannel ChannelFlag;

    T g;
};

template <class T>
struct STICK_API Blue
{
    using ValueType = T;
    typedef BlueChannel ChannelFlag;

    T b;
};

template <class T>
struct STICK_API Alpha
{
    using ValueType = T;
    typedef AlphaChannel ChannelFlag;

    T a;
};

template <class T>
struct STICK_API Cyan
{
    using ValueType = T;
    typedef CyanChannel ChannelFlag;

    T c;
};

template <class T>
struct STICK_API Magenta
{
    using ValueType = T;
    typedef MagentaChannel ChannelFlag;

    T m;
};

template <class T>
struct STICK_API Yellow
{
    using ValueType = T;
    typedef YellowChannel ChannelFlag;

    T y;
};

template <class T>
struct STICK_API Black
{
    using ValueType = T;
    typedef BlackChannel ChannelFlag;

    T k;
};

// compile time branching to determine alpha position
namespace detail
{
class NoChannel
{
};

template <class A, class B, class C, class D>
struct AlphaPosition
{
    static stick::Int32 alphaPosition()
    {
        return -1;
    }
};

template <class B, class C, class D>
struct AlphaPosition<AlphaChannel, B, C, D>
{
    static stick::Int32 alphaPosition()
    {
        return 0;
    }
};

template <class A, class C, class D>
struct AlphaPosition<A, AlphaChannel, C, D>
{
    static stick::Int32 alphaPosition()
    {
        return 1;
    }
};

template <class A, class B, class D>
struct AlphaPosition<A, B, AlphaChannel, D>
{
    static stick::Int32 alphaPosition()
    {
        return 2;
    }
};

template <class A, class B, class C>
struct AlphaPosition<A, B, C, AlphaChannel>
{
    static stick::Int32 alphaPosition()
    {
        return 3;
    }
};
} // namespace detail

template <class T0>
class ChannelLayout1
{
  public:
    typedef T0 Channel0;

    typedef stick::TypeInfoT<ChannelLayout1> TypeInfo;

    static constexpr stick::UInt32 s_channelCount = 1;

    static constexpr stick::UInt32 channelCount()
    {
        return s_channelCount;
    }

    static constexpr bool hasAlpha()
    {
        return alphaPosition() != -1;
    }

    static constexpr stick::Int32 alphaPosition()
    {
        return detail::AlphaPosition<Channel0,
                                     detail::NoChannel,
                                     detail::NoChannel,
                                     detail::NoChannel>::alphaPosition();
    }

    static constexpr stick::TypeID channelLayoutTypeID()
    {
        return stick::TypeInfoT<ChannelLayout1>::typeID();
    }
};

template <class T0, class T1>
class ChannelLayout2
{
  public:
    typedef T0 Channel0;

    typedef T1 Channel1;

    typedef stick::TypeInfoT<ChannelLayout2> TypeInfo;

    static constexpr stick::UInt32 s_channelCount = 2;

    static constexpr stick::UInt32 channelCount()
    {
        return s_channelCount;
    }

    static constexpr bool hasAlpha()
    {
        return alphaPosition() != -1;
    }

    static constexpr stick::Int32 alphaPosition()
    {
        return detail::AlphaPosition<Channel0, Channel1, detail::NoChannel, detail::NoChannel>::
            alphaPosition();
    }

    static constexpr stick::TypeID channelLayoutTypeID()
    {
        return stick::TypeInfoT<ChannelLayout2>::typeID();
    }
};

template <class T0, class T1, class T2>
class ChannelLayout3
{
  public:
    typedef T0 Channel0;

    typedef T1 Channel1;

    typedef T2 Channel2;

    typedef stick::TypeInfoT<ChannelLayout3> TypeInfo;

    static constexpr stick::UInt32 s_channelCount = 3;

    static constexpr stick::UInt32 channelCount()
    {
        return s_channelCount;
    }

    static constexpr bool hasAlpha()
    {
        return alphaPosition() != -1;
    }

    static constexpr stick::Int32 alphaPosition()
    {
        return detail::AlphaPosition<Channel0, Channel1, Channel2, detail::NoChannel>::
            alphaPosition();
    }

    static constexpr stick::TypeID channelLayoutTypeID()
    {
        return stick::TypeInfoT<ChannelLayout3>::typeID();
    }
};

template <class T0, class T1, class T2, class T3>
class ChannelLayout4
{
  public:
    typedef T0 Channel0;

    typedef T1 Channel1;

    typedef T2 Channel2;

    typedef T3 Channel3;

    typedef stick::TypeInfoT<ChannelLayout4> TypeInfo;

    static constexpr stick::UInt32 s_channelCount = 4;

    static constexpr stick::UInt32 channelCount()
    {
        return s_channelCount;
    }

    static constexpr bool hasAlpha()
    {
        return alphaPosition() != -1;
    }

    static constexpr stick::Int32 alphaPosition()
    {
        return detail::AlphaPosition<Channel0, Channel1, Channel2, Channel3>::alphaPosition();
    }

    static constexpr stick::TypeID channelLayoutTypeID()
    {
        return stick::TypeInfoT<ChannelLayout4>::typeID();
    }
};

typedef ChannelLayout1<GrayChannel> ChannelLayoutGray;
typedef ChannelLayout2<GrayChannel, AlphaChannel> ChannelLayoutGrayAlpha;
typedef ChannelLayout2<AlphaChannel, GrayChannel> ChannelLayoutAlphaGray;
typedef ChannelLayout3<BlueChannel, GreenChannel, RedChannel> ChannelLayoutBGR;
typedef ChannelLayout4<BlueChannel, GreenChannel, RedChannel, AlphaChannel> ChannelLayoutBGRA;
typedef ChannelLayout4<AlphaChannel, BlueChannel, GreenChannel, RedChannel> ChannelLayoutABGR;
typedef ChannelLayout3<RedChannel, GreenChannel, BlueChannel> ChannelLayoutRGB;
typedef ChannelLayout4<RedChannel, GreenChannel, BlueChannel, AlphaChannel> ChannelLayoutRGBA;
typedef ChannelLayout4<AlphaChannel, RedChannel, GreenChannel, BlueChannel> ChannelLayoutARGB;
typedef ChannelLayout4<CyanChannel, MagentaChannel, YellowChannel, BlackChannel> ChannelLayoutCMYK;
} // namespace pic

#endif // PIC_CHANNELS_HPP
