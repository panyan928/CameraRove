#ifndef CFONTRENDER_H
#define CFONTRENDER_H
#include "cfontmanager.h"

#include <vector>
using namespace std;
namespace textRender {
    class CFontRender
    {
    public:
        CFontRender(int screenWidth, int screenHeight);
        virtual ~CFontRender();

        int initialize(const char* path);

        void loadFont(const char* path, int face, int tall, bool bold, bool italic, bool antialias);

        //int render(wstring text, Vec2i pos, int fontSize, string fontName, Color fontColor);
        //int render(wstring text, Vec2i pos, int fontSize, string fontName, Color fontColor, bool bold);
        //int render(wstring text, Vec2i pos, int fontIndex, Color fontColor);
        int render(char* text, Vec2i pos, int fontSize, string fontName, Color fontColor);
        int render(string text, Vec2i pos, int fontSize, string fontName, Color fontColor);
        int render(string text, Vec2i pos, int fontIndex, Color fontColor);

        void exportScreenImage(const char* path);

        CFontManager* getFontManager() const;

        void clearScreen();

        unsigned char* getData() const;


    private:
        unsigned char* m_screen;
        CFontManager * m_fontManager;

    };
}
#endif // CFONTRENDER_H
