#pragma once
#include "cstroke.h"
#include "cfill.h"
#include "csymbol.h"
#include "ctext.h"

#include "../TMUtil/cstringutil.h"

#include <iostream>
#include <fstream>
using namespace std;
namespace TMStyle {
	class CStyle
	{
	public:
		CStyle();
		CStyle(std::string index);
		~CStyle();

		CFill* fill() const;
		CStroke* stroke() const;
		CSymbol* symbol() const;
		CText* text() const;

		CFill* fill(int i) const;
		CStroke* stroke(int i) const;
		CSymbol* symbol(int i) const;
		CText* text(int i) const;

		std::string styleIndex() const;
		std::string styleType() const;

		void setStyleType(std::string type);

		void createFill();
		void createStroke();
		void createSymbol();
		void createText();

		int loadStyle(const char* path);
		int loadStyleCmethod(const char* path);

	private:
		CFill* _fill;
		CStroke* _stroke;
		CSymbol* _symbol;
		CText* _text;

		vector<CFill*> _fills;
		vector<CStroke*> _strokes;
		vector<CSymbol*> _symbols;
		vector<CText*> _texts;

		std::string _styleIndex;
		std::string _styleType;

		void setStroke(vector<string> data);
		void setStroke(vector<string> data, CStroke* stroke);

		void setFill(vector<string> data); 
		void setFill(vector<string> data, CFill* fill);

		void setFont(vector<string> data);
		void setFont(vector<string> data, CText* text);
	};
}


