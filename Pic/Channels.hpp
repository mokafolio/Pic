#ifndef PIC_CHANNELS_HPP
#define PIC_CHANNELS_HPP

#include <Stick/TypeInfo.hpp>

namespace pic
{
    struct STICK_API GrayChannel {};
    struct STICK_API RedChannel {};
    struct STICK_API GreenChannel {};
    struct STICK_API BlueChannel {};
    struct STICK_API AlphaChannel {};
    struct STICK_API CyanChannel {};
    struct STICK_API MagentaChannel {};
    struct STICK_API YellowChannel {};
    struct STICK_API BlackChannel {};

    template<class T>
    struct STICK_API Gray
    {
        typedef GrayChannel ChannelFlag;

        T v;
    };

    template<class T>
    struct STICK_API Red
    {
        typedef RedChannel ChannelFlag;

        T r;
    };

    template<class T>
    struct STICK_API Green
    {
        typedef GreenChannel ChannelFlag;

        T g;
    };

    template<class T>
    struct STICK_API Blue
    {
        typedef BlueChannel ChannelFlag;

        T b;
    };

    template<class T>
    struct STICK_API Alpha
    {
        typedef AlphaChannel ChannelFlag;

        T a;
    };

    template<class T>
    struct STICK_API Cyan
    {
        typedef CyanChannel ChannelFlag;

        T c;
    };

    template<class T>
    struct STICK_API Magenta
    {
        typedef MagentaChannel ChannelFlag;

        T m;
    };

    template<class T>
    struct STICK_API Yellow
    {
        typedef YellowChannel ChannelFlag;

        T y;
    };

    template<class T>
    struct STICK_API Black
    {
        typedef BlackChannel ChannelFlag;

        T k;
    };


    //compile time branching to determine alpha position
    namespace detail
    {
        class NoChannel {};

        template<class A, class B, class C, class D>
        struct AlphaPosition
        {
            static stick::Int32 alphaPosition()
            {
                return -1;
            }
        };

        template<class B, class C, class D>
        struct AlphaPosition<AlphaChannel, B, C, D>
        {
            static stick::Int32 alphaPosition()
            {
                return 0;
            }
        };

        template<class A, class C, class D>
        struct AlphaPosition<A, AlphaChannel, C, D>
        {
            static stick::Int32 alphaPosition()
            {
                return 1;
            }
        };

        template<class A, class B, class D>
        struct AlphaPosition<A, B, AlphaChannel, D>
        {
            static stick::Int32 alphaPosition()
            {
                return 2;
            }
        };


        template<class A, class B, class C>
        struct AlphaPosition<A, B, C, AlphaChannel>
        {
            static stick::Int32 alphaPosition()
            {
                return 3;
            }
        };
    }


    template<class T0>
    class ChannelLayout1
    {
    public:

        typedef T0 Channel0;

        typedef stick::TypeInfoT<ChannelLayout1> TypeInfo;

        static const stick::UInt32 s_channelCount = 1;


        static stick::UInt32 channelCount()
        {
            return s_channelCount;
        }

        static bool hasAlpha()
        {
            return alphaPosition() != -1;
        }

        static stick::Int32 alphaPosition()
        {
            return detail::AlphaPosition<Channel0, detail::NoChannel, detail::NoChannel, detail::NoChannel>::alphaPosition();
        }

        static stick::TypeID channelLayoutTypeID()
        {
            return stick::TypeInfoT<ChannelLayout1>::typeID();
        }
    };

    template<class T0, class T1>
    class ChannelLayout2
    {
    public:

        typedef T0 Channel0;

        typedef T1 Channel1;

        typedef stick::TypeInfoT<ChannelLayout2> TypeInfo;

        static const stick::UInt32 s_channelCount = 2;


        static stick::UInt32 channelCount()
        {
            return s_channelCount;
        }

        static bool hasAlpha()
        {
            return alphaPosition() != -1;
        }

        static stick::Int32 alphaPosition()
        {
            return detail::AlphaPosition<Channel0, Channel1, detail::NoChannel, detail::NoChannel>::alphaPosition();
        }

        static stick::TypeID channelLayoutTypeID()
        {
            return stick::TypeInfoT<ChannelLayout2>::typeID();
        }
    };

    template<class T0, class T1, class T2>
    class ChannelLayout3
    {
    public:

        typedef T0 Channel0;

        typedef T1 Channel1;

        typedef T2 Channel2;

        typedef stick::TypeInfoT<ChannelLayout3> TypeInfo;

        static const stick::UInt32 s_channelCount = 3;


        static stick::UInt32 channelCount()
        {
            return s_channelCount;
        }

        static bool hasAlpha()
        {
            return alphaPosition() != -1;
        }

        static stick::Int32 alphaPosition()
        {
            return detail::AlphaPosition<Channel0, Channel1, Channel2, detail::NoChannel>::alphaPosition();
        }

        static stick::TypeID channelLayoutTypeID()
        {
            return stick::TypeInfoT<ChannelLayout3>::typeID();
        }
    };

    template<class T0, class T1, class T2, class T3>
    class ChannelLayout4
    {
    public:

        typedef T0 Channel0;

        typedef T1 Channel1;

        typedef T2 Channel2;

        typedef T3 Channel3;

        typedef stick::TypeInfoT<ChannelLayout4> TypeInfo;

        static const stick::UInt32 s_channelCount = 4;


        static stick::UInt32 channelCount()
        {
            return s_channelCount;
        }

        static bool hasAlpha()
        {
            return alphaPosition() != -1;
        }

        static stick::Int32 alphaPosition()
        {
            return detail::AlphaPosition<Channel0, Channel1, Channel2, Channel3>::alphaPosition();
        }

        static stick::TypeID channelLayoutTypeID()
        {
            return stick::TypeInfoT<ChannelLayout4>::typeID();
        }
    };

    template<class T, template<class> class C0>
    struct Channels1 :
        public C0<T>
    {
        typedef T ValueType;
        typedef ChannelLayout1<typename C0<T>::ChannelFlag> ChannelLayout;
    };

    template<class T, template<class> class C0, template<class> class C1>
    struct Channels2 :
        public C0<T>,
        public C1<T>
    {
        typedef T ValueType;
        typedef ChannelLayout2<typename C0<T>::ChannelFlag, typename C1<T>::ChannelFlag> ChannelLayout;
    };

    template<class T, template<class> class C0, template<class> class C1, template<class> class C2>
    struct Channels3 :
        public C0<T>,
        public C1<T>,
        public C2<T>
    {
        typedef T ValueType;
        typedef ChannelLayout3<typename C0<T>::ChannelFlag, typename C1<T>::ChannelFlag, typename C2<T>::ChannelFlag> ChannelLayout;
    };

    template<class T, template<class> class C0, template<class> class C1, template<class> class C2, template<class> class C3>
    struct Channels4 :
        public C0<T>,
        public C1<T>,
        public C2<T>,
        public C3<T>
    {
        typedef T ValueType;
        typedef ChannelLayout4<typename C0<T>::ChannelFlag, typename C1<T>::ChannelFlag, typename C2<T>::ChannelFlag, typename C3<T>::ChannelFlag> ChannelLayout;
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


    typedef Channels1<stick::UInt8, Gray> ChannelsGray8;
    typedef Channels1<stick::UInt16, Gray> ChannelsGray16;
    typedef Channels1<stick::UInt32, Gray> ChannelsGray32;
    typedef Channels1<stick::Float32, Gray> ChannelsGray32f;

    typedef Channels2<stick::UInt8, Gray, Alpha> ChannelsGrayAlpha8;
    typedef Channels2<stick::UInt16, Gray, Alpha> ChannelsGrayAlpha16;
    typedef Channels2<stick::UInt32, Gray, Alpha> ChannelsGrayAlpha32;
    typedef Channels2<stick::Float32, Gray, Alpha> ChannelsGrayAlpha32f;

    typedef Channels2<stick::UInt8, Alpha, Gray> ChannelsAlphaGray8;
    typedef Channels2<stick::UInt16, Alpha, Gray> ChannelsAlphaGray16;
    typedef Channels2<stick::UInt32, Alpha, Gray> ChannelsAlphaGray32;
    typedef Channels2<stick::Float32, Alpha, Gray> ChannelsAlphaGray32f;

    typedef Channels3<stick::UInt8, Red, Green, Blue> ChannelsRGB8;
    typedef Channels3<stick::UInt16, Red, Green, Blue> ChannelsRGB16;
    typedef Channels3<stick::UInt32, Red, Green, Blue> ChannelsRGB32;
    typedef Channels3<stick::Float32, Red, Green, Blue> ChannelsRGB32f;

    typedef Channels4<stick::UInt8, Red, Green, Blue, Alpha> ChannelsRGBA8;
    typedef Channels4<stick::UInt16, Red, Green, Blue, Alpha> ChannelsRGBA16;
    typedef Channels4<stick::UInt32, Red, Green, Blue, Alpha> ChannelsRGBA32;
    typedef Channels4<stick::Float32, Red, Green, Blue, Alpha> ChannelsRGBA32f;

    typedef Channels4<stick::UInt8, Alpha, Red, Green, Blue> ChannelsARGB8;
    typedef Channels4<stick::UInt16, Alpha, Red, Green, Blue> ChannelsARGB16;
    typedef Channels4<stick::UInt32, Alpha, Red, Green, Blue> ChannelsARGB32;
    typedef Channels4<stick::Float32, Alpha, Red, Green, Blue> ChannelsARGB32f;

    typedef Channels3<stick::UInt8, Blue, Green, Red> ChannelsBGR8;
    typedef Channels3<stick::UInt16, Blue, Green, Red> ChannelsBGR16;
    typedef Channels3<stick::UInt32, Blue, Green, Red> ChannelsBGR32;
    typedef Channels3<stick::Float32, Blue, Green, Red> ChannelsBGR32f;

    typedef Channels4<stick::UInt8, Blue, Green, Red, Alpha> ChannelsBGRA8;
    typedef Channels4<stick::UInt16, Blue, Green, Red, Alpha> ChannelsBGRA16;
    typedef Channels4<stick::UInt32, Blue, Green, Red, Alpha> ChannelsBGRA32;
    typedef Channels4<stick::Float32, Blue, Green, Red, Alpha> ChannelsBGRA32f;

    typedef Channels4<stick::UInt8, Alpha, Blue, Green, Red> ChannelsABGR8;
    typedef Channels4<stick::UInt16, Alpha, Blue, Green, Red> ChannelsABGR16;
    typedef Channels4<stick::UInt32, Alpha, Blue, Green, Red> ChannelsABGR32;
    typedef Channels4<stick::Float32, Alpha, Blue, Green, Red> ChannelsABGR32f;

    typedef Channels4<stick::UInt8, Cyan, Magenta, Yellow, Black> ChannelsCMYK8;
    typedef Channels4<stick::UInt16, Cyan, Magenta, Yellow, Black> ChannelsCMYK16;
    typedef Channels4<stick::UInt32, Cyan, Magenta, Yellow, Black> ChannelsCMYK32;
    typedef Channels4<stick::Float32, Cyan, Magenta, Yellow, Black> ChannelsCMYK32f;
}

#endif //PIC_CHANNELS_HPP
