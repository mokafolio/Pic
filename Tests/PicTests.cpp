#include <Pic/Image.hpp>
#include <Stick/Test.hpp>

#include <iostream>

using namespace stick;
using namespace pic;

const Suite spec[] =
{
    SUITE("Pixel Tests")
    {
        PixelRGB8 a(1, 128, 64);
        EXPECT(a.r == 1);
        EXPECT(a.g == 128);
        EXPECT(a.b == 64);

        a.printShit();
        unsigned char vals[3] = {30, 20, 1};
        a.setValue(vals);
        EXPECT(a.r == 30);
        EXPECT(a.g == 20);
        EXPECT(a.b == 1);

        EXPECT(a.channel(0) == 30);
        EXPECT(a.channel(1) == 20);
        EXPECT(a.channel(2) == 1);

        PixelRGB8 b = a;
        EXPECT(b.channel(0) == 30);
        EXPECT(b.channel(1) == 20);
        EXPECT(b.channel(2) == 1);
        EXPECT(a == b);
    },
    SUITE("Image Tests")
    {
        ImageGray32f img(100, 200);
        EXPECT(img.width() == 100);
        EXPECT(img.height() == 200);

        img.resize(50, 25, 10);
        EXPECT(img.width() == 50);
        EXPECT(img.height() == 25);
        EXPECT(img.depth() == 10);

        float pixels[9] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f};
        img.loadPixels(3, 3, pixels);
        EXPECT(img.width() == 3);
        EXPECT(img.height() == 3);
        EXPECT(img.pixel(0, 0).v == 0.1f);
        EXPECT(img.pixel(1, 0).v == 0.2f);
        EXPECT(img.pixel(2, 0).v == 0.3f);
        EXPECT(img.pixel(0, 1).v == 0.4f);
        EXPECT(img.pixel(1, 1).v == 0.5f);
        EXPECT(img.pixel(2, 1).v == 0.6f);
        EXPECT(img.pixel(0, 2).v == 0.7f);
        EXPECT(img.pixel(1, 2).v == 0.8f);
        EXPECT(img.pixel(2, 2).v == 0.9f);
        EXPECT(img.pixelCount() == 9);

        img.flipRows();
        EXPECT(img.pixel(0, 0).v == 0.7f);
        EXPECT(img.pixel(1, 0).v == 0.8f);
        EXPECT(img.pixel(2, 0).v == 0.9f);
        EXPECT(img.pixel(0, 1).v == 0.4f);
        EXPECT(img.pixel(1, 1).v == 0.5f);
        EXPECT(img.pixel(2, 1).v == 0.6f);
        EXPECT(img.pixel(0, 2).v == 0.1f);
        EXPECT(img.pixel(1, 2).v == 0.2f);
        EXPECT(img.pixel(2, 2).v == 0.3f);

        img.flipColumns();
        EXPECT(img.pixel(0, 0).v == 0.9f);
        EXPECT(img.pixel(1, 0).v == 0.8f);
        EXPECT(img.pixel(2, 0).v == 0.7f);
        EXPECT(img.pixel(0, 1).v == 0.6f);
        EXPECT(img.pixel(1, 1).v == 0.5f);
        EXPECT(img.pixel(2, 1).v == 0.4f);
        EXPECT(img.pixel(0, 2).v == 0.3f);
        EXPECT(img.pixel(1, 2).v == 0.2f);
        EXPECT(img.pixel(2, 2).v == 0.1f);

        img.resize(2, 2);
        EXPECT(img.pixelCount() == 4);
        EXPECT(img.width() == 2);
        EXPECT(img.height() == 2);
    },
    SUITE("UpdatePixels Tests")
    {
        UInt8 rawPixels[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        ImageGray8 pixels;
        pixels.loadPixels(3, 3, &rawPixels[0]);

        EXPECT(pixels.pixel(0, 0).channel(0) == 0);
        EXPECT(pixels.pixel(1, 0).channel(0) == 1);
        EXPECT(pixels.pixel(2, 0).channel(0) == 2);
        EXPECT(pixels.pixel(0, 1).channel(0) == 3);
        EXPECT(pixels.pixel(1, 1).channel(0) == 4);
        EXPECT(pixels.pixel(2, 1).channel(0) == 5);
        EXPECT(pixels.pixel(0, 2).channel(0) == 6);
        EXPECT(pixels.pixel(1, 2).channel(0) == 7);
        EXPECT(pixels.pixel(2, 2).channel(0) == 8);

        UInt8 updatePixels[4] = {8, 7, 6, 5};
        pixels.updatePixels(1, 1, 2, 2, &updatePixels[0]);
        EXPECT(pixels.pixel(0, 0).channel(0) == 0);
        EXPECT(pixels.pixel(1, 0).channel(0) == 1);
        EXPECT(pixels.pixel(2, 0).channel(0) == 2);
        EXPECT(pixels.pixel(0, 1).channel(0) == 3);
        EXPECT(pixels.pixel(1, 1).channel(0) == 8);
        EXPECT(pixels.pixel(2, 1).channel(0) == 7);
        EXPECT(pixels.pixel(0, 2).channel(0) == 6);
        EXPECT(pixels.pixel(1, 2).channel(0) == 6);
        EXPECT(pixels.pixel(2, 2).channel(0) == 5);
    },
    SUITE("UpdatePixels3D Tests")
    {
        UInt8 rawPixels[27] =
        {
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
            11, 12, 13, 14, 15, 16, 17, 18, 19,
            20, 21, 22, 23, 24, 25, 26
        };

        ImageGray8 pixels;
        pixels.loadPixels(3, 3, 3, &rawPixels[0]);

        EXPECT(pixels.pixel(0, 0, 0).channel(0) == 0);
        EXPECT(pixels.pixel(1, 0, 0).channel(0) == 1);
        EXPECT(pixels.pixel(2, 0, 0).channel(0) == 2);

        EXPECT(pixels.pixel(0, 1, 0).channel(0) == 3);
        EXPECT(pixels.pixel(1, 1, 0).channel(0) == 4);
        EXPECT(pixels.pixel(2, 1, 0).channel(0) == 5);

        EXPECT(pixels.pixel(0, 2, 0).channel(0) == 6);
        EXPECT(pixels.pixel(1, 2, 0).channel(0) == 7);
        EXPECT(pixels.pixel(2, 2, 0).channel(0) == 8);

        EXPECT(pixels.pixel(0, 0, 1).channel(0) == 9);
        EXPECT(pixels.pixel(1, 0, 1).channel(0) == 10);
        EXPECT(pixels.pixel(2, 0, 1).channel(0) == 11);

        EXPECT(pixels.pixel(0, 1, 1).channel(0) == 12);
        EXPECT(pixels.pixel(1, 1, 1).channel(0) == 13);
        EXPECT(pixels.pixel(2, 1, 1).channel(0) == 14);

        EXPECT(pixels.pixel(0, 2, 1).channel(0) == 15);
        EXPECT(pixels.pixel(1, 2, 1).channel(0) == 16);
        EXPECT(pixels.pixel(2, 2, 1).channel(0) == 17);

        EXPECT(pixels.pixel(0, 0, 2).channel(0) == 18);
        EXPECT(pixels.pixel(1, 0, 2).channel(0) == 19);
        EXPECT(pixels.pixel(2, 0, 2).channel(0) == 20);

        EXPECT(pixels.pixel(0, 1, 2).channel(0) == 21);
        EXPECT(pixels.pixel(1, 1, 2).channel(0) == 22);
        EXPECT(pixels.pixel(2, 1, 2).channel(0) == 23);

        EXPECT(pixels.pixel(0, 2, 2).channel(0) == 24);
        EXPECT(pixels.pixel(1, 2, 2).channel(0) == 25);
        EXPECT(pixels.pixel(2, 2, 2).channel(0) == 26);

        UInt8 updatePixels[8] = {8, 7, 6, 5, 4, 3, 2, 1};
        pixels.updatePixels(1, 1, 1, 2, 2, 2, &updatePixels[0]);

        EXPECT(pixels.pixel(0, 0, 0).channel(0) == 0);
        EXPECT(pixels.pixel(1, 0, 0).channel(0) == 1);
        EXPECT(pixels.pixel(2, 0, 0).channel(0) == 2);

        EXPECT(pixels.pixel(0, 1, 0).channel(0) == 3);
        EXPECT(pixels.pixel(1, 1, 0).channel(0) == 4);
        EXPECT(pixels.pixel(2, 1, 0).channel(0) == 5);

        EXPECT(pixels.pixel(0, 2, 0).channel(0) == 6);
        EXPECT(pixels.pixel(1, 2, 0).channel(0) == 7);
        EXPECT(pixels.pixel(2, 2, 0).channel(0) == 8);

        EXPECT(pixels.pixel(0, 0, 1).channel(0) == 9);
        EXPECT(pixels.pixel(1, 0, 1).channel(0) == 10);
        EXPECT(pixels.pixel(2, 0, 1).channel(0) == 11);

        EXPECT(pixels.pixel(0, 1, 1).channel(0) == 12);
        EXPECT(pixels.pixel(1, 1, 1).channel(0) == 8);
        EXPECT(pixels.pixel(2, 1, 1).channel(0) == 7);

        EXPECT(pixels.pixel(0, 2, 1).channel(0) == 15);
        EXPECT(pixels.pixel(1, 2, 1).channel(0) == 6);
        EXPECT(pixels.pixel(2, 2, 1).channel(0) == 5);

        EXPECT(pixels.pixel(0, 0, 2).channel(0) == 18);
        EXPECT(pixels.pixel(1, 0, 2).channel(0) == 19);
        EXPECT(pixels.pixel(2, 0, 2).channel(0) == 20);

        EXPECT(pixels.pixel(0, 1, 2).channel(0) == 21);
        EXPECT(pixels.pixel(1, 1, 2).channel(0) == 4);
        EXPECT(pixels.pixel(2, 1, 2).channel(0) == 3);

        EXPECT(pixels.pixel(0, 2, 2).channel(0) == 24);
        EXPECT(pixels.pixel(1, 2, 2).channel(0) == 2);
        EXPECT(pixels.pixel(2, 2, 2).channel(0) == 1);
    },
    SUITE("Flip Tests")
    {
        ImageGray8 img;
        UInt8 data[4] = {255, 125, 50, 25};
        img.loadPixels(2, 2, data);
        EXPECT(img.pixel(0, 0).v == 255);
        EXPECT(img.pixel(1, 0).v == 125);
        EXPECT(img.pixel(0, 1).v == 50);
        EXPECT(img.pixel(1, 1).v == 25);
        img.flipRows();
        EXPECT(img.pixel(0, 0).v == 50);
        EXPECT(img.pixel(1, 0).v == 25);
        EXPECT(img.pixel(0, 1).v == 255);
        EXPECT(img.pixel(1, 1).v == 125);

        img.flipColumns();
        EXPECT(img.pixel(0, 0).v == 25);
        EXPECT(img.pixel(1, 0).v == 50);
        EXPECT(img.pixel(0, 1).v == 125);
        EXPECT(img.pixel(1, 1).v == 255);

        ImageGray8 img2;
        UInt8 data2[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        img2.loadPixels(2, 2, 2, data2);
        EXPECT(img2.pixel(0, 0, 0).v == 1);
        EXPECT(img2.pixel(1, 0, 0).v == 2);
        EXPECT(img2.pixel(0, 1, 0).v == 3);
        EXPECT(img2.pixel(1, 1, 0).v == 4);
        EXPECT(img2.pixel(0, 0, 1).v == 5);
        EXPECT(img2.pixel(1, 0, 1).v == 6);
        EXPECT(img2.pixel(0, 1, 1).v == 7);
        EXPECT(img2.pixel(1, 1, 1).v == 8);
        img2.flipLayers();
        EXPECT(img2.pixel(0, 0, 0).v == 5);
        EXPECT(img2.pixel(1, 0, 0).v == 6);
        EXPECT(img2.pixel(0, 1, 0).v == 7);
        EXPECT(img2.pixel(1, 1, 0).v == 8);
        EXPECT(img2.pixel(0, 0, 1).v == 1);
        EXPECT(img2.pixel(1, 0, 1).v == 2);
        EXPECT(img2.pixel(0, 1, 1).v == 3);
        EXPECT(img2.pixel(1, 1, 1).v == 4);
    },
    SUITE("PixelIterator Tests")
    {
        ImageGray8 img;
        UInt8 data[4] = {255, 125, 50, 25};
        img.loadPixels(2, 2, data);
        EXPECT(img.pixel(0, 0).v == 255);
        EXPECT(img.pixel(1, 0).v == 125);
        EXPECT(img.pixel(0, 1).v == 50);
        EXPECT(img.pixel(1, 1).v == 25);

        auto it = img.begin();
        EXPECT(it->v == 255);
        ++it;
        EXPECT(it != img.end());
        EXPECT((*it).v == 125);
        it++;
        EXPECT(it != img.end());
        EXPECT((*it).v == 50);
        it++;
        EXPECT(it != img.end());
        EXPECT((*it).v == 25);
        it++;
        EXPECT(it == img.end());
        --it;
        EXPECT(it != img.end());
        EXPECT((*it).v == 25);
        --it;
        EXPECT(it != img.end());
        EXPECT((*it).v == 50);
        --it;
        EXPECT(it != img.end());
        EXPECT((*it).v == 125);
        --it;
        EXPECT(it != img.end());
        EXPECT((*it).v == 255);
        EXPECT(it == img.begin());

        //check if padding works as expected
        ImageGray8 img2;
        UInt8 data2[12] = {1, 2, 3, 0, 4, 5, 6, 0, 7, 8, 9, 0};
        img2.loadPixels(3, 3, data2, 1);
        EXPECT(img2.pixel(0, 0).v == 1);
        EXPECT(img2.pixel(1, 0).v == 2);
        EXPECT(img2.pixel(2, 0).v == 3);
        EXPECT(img2.pixel(0, 1).v == 4);
        EXPECT(img2.pixel(1, 1).v == 5);
        EXPECT(img2.pixel(2, 1).v == 6);
        EXPECT(img2.pixel(0, 2).v == 7);
        EXPECT(img2.pixel(1, 2).v == 8);
        EXPECT(img2.pixel(2, 2).v == 9);

        auto it2 = img2.begin();
        EXPECT(it2->v == 1);
        ++it2;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 2);
        it2++;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 3);
        it2++;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 4);
        it2++;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 5);
        it2++;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 6);
        it2++;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 7);
        it2++;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 8);
        it2++;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 9);
        it2++;
        EXPECT(it2 == img2.end());

        it2--;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 9);
        --it2;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 8);
        --it2;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 7);
        --it2;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 6);
        --it2;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 5);
        --it2;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 4);
        --it2;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 3);
        --it2;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 2);
        --it2;
        EXPECT(it2 != img2.end());
        EXPECT((*it2).v == 1);
        EXPECT(it2 == img2.begin());

        it2 = img2.begin() + 7;
        EXPECT(it2->v == 8);
        it2 = it2 - 6;
        EXPECT(it2->v == 2);
    },
    SUITE("Image Load Tests")
    {
        //TODO: Test more image formats.
        auto a = loadImage("I DO NOT EXIST");
        EXPECT(!a);
        auto res = loadImage("../../Tests/TestFiles/test01.png");
        EXPECT(res);
        EXPECT(res.get()->width() == 2);
        EXPECT(res.get()->height() == 2);
        EXPECT(res.get()->channelLayoutTypeID() == ImageBGRA8::channelLayoutTID);

        const ImageBGRA8 & img = static_cast<const ImageBGRA8 &>(*res.get());
        //TODO: Add some tolerance to this comparison? Depending on the underlying implementation
        //we should propably allow for some variation due to rounding errors etc. NOT SURE
        EXPECT(img.pixel(0, 0) == PixelBGRA8(0, 0, 0, 255));
        EXPECT(img.pixel(1, 0) == PixelBGRA8(0, 255, 0, 255));
        EXPECT(img.pixel(0, 1) == PixelBGRA8(255, 255, 255, 0));
        EXPECT(img.pixel(1, 1) == PixelBGRA8(0, 0, 255, 255));
    },
    SUITE("Image Save Tests")
    {
        //TODO: Test more image formats.
        UInt8 pixels[4] = {255, 128, 64, 0};
        ImageGray8 a(2, 2, pixels);
        String path("../../Tests/TestFiles/saveTest.png");
        Error err = a.saveFile(path);
        EXPECT(!err);
        auto res = loadImage(path);
        EXPECT(res);
        EXPECT(res.get()->width() == 2);
        EXPECT(res.get()->height() == 2);
        EXPECT(res.get()->depth() == 1);
        EXPECT(res.get()->channelLayoutTypeID() == ImageGray8::channelLayoutTID);

        ImageGray8 & img = static_cast<ImageGray8 &>(*res.get());
        EXPECT(img.pixel(0, 0).v == 255);
        EXPECT(img.pixel(1, 0).v == 128);
        EXPECT(img.pixel(0, 1).v == 64);
        EXPECT(img.pixel(1, 1).v == 0);
        remove(path.cString());

        UInt8 pixels2[12] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120};
        ImageBGR8 b(2, 2, pixels2);
        String path2("../../Tests/TestFiles/saveTest2.jpg");
        err = b.saveFile(path2);
        EXPECT(!err);
        res = loadImage(path2);
        EXPECT(res);
        EXPECT(res.get()->width() == 2);
        EXPECT(res.get()->height() == 2);
        EXPECT(res.get()->depth() == 1);
        EXPECT(res.get()->channelLayoutTypeID() == ImageBGR8::channelLayoutTID);

        ImageBGR8 & img2 = static_cast<ImageBGR8 &>(*res.get());
        //since we are in compressed land, we compare with tolerance
        EXPECT(abs(10 - img2.pixel(0, 0).b) <= 5);
        EXPECT(abs(20 - img2.pixel(0, 0).g) <= 5);
        EXPECT(abs(30 - img2.pixel(0, 0).r) <= 5);
        EXPECT(abs(40 - img2.pixel(1, 0).b) <= 5);
        EXPECT(abs(50 - img2.pixel(1, 0).g) <= 5);
        EXPECT(abs(60 - img2.pixel(1, 0).r) <= 5);
        EXPECT(abs(70 - img2.pixel(0, 1).b) <= 5);
        EXPECT(abs(80 - img2.pixel(0, 1).g) <= 5);
        EXPECT(abs(90 - img2.pixel(0, 1).r) <= 5);
        EXPECT(abs(100 - img2.pixel(1, 1).b) <= 5);
        EXPECT(abs(110 - img2.pixel(1, 1).g) <= 5);
        EXPECT(abs(120 - img2.pixel(1, 1).r) <= 5);
        remove(path2.cString());
    }
};

int main(int _argc, const char * _args[])
{
    return runTests(spec, _argc, _args);
}
