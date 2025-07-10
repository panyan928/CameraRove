#include "cstyle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace TMStyle {
	CStyle::CStyle()
	{
		this->_fill = 0x00;
		this->_stroke = 0x00;
		this->_symbol = 0x00;
		this->_text = 0x00;
	}

	CStyle::CStyle(std::string index)
	{
		this->_fill = 0x00;
		this->_stroke = 0x00;
		this->_symbol = 0x00;
		this->_text = 0x00;
		this->_styleIndex = index;
	}

	CStyle::~CStyle()
	{
		if (_fill != 0x00) {
			delete _fill;
			_fill = 0x00;
		}
		if (_stroke != 0x00) {
			delete _stroke;
			_stroke = 0x00;
		}
		if (_text != 0x00) {
			delete _text;
			_text = 0x00;
		}
		if (_symbol != 0x00) {
			delete _symbol;
			_symbol = 0x00;
		}
		this->_styleIndex = "";

       /* vector<CFill*> _fills;
        vector<CStroke*> _strokes;
        vector<CSymbol*> _symbols;
        vector<CText*> _texts;*/
        int len = _fills.size();
        for (int i = 0; i < len; i++) {
            if (_fills[i] != 0x00) {
                delete _fills[i];
                _fills[i] = 0x00;
            }
        }
        len = _strokes.size();
        for (int i = 0; i < len; i++) {
            if (_strokes[i] != 0x00) {
                delete _strokes[i];
                _strokes[i] = 0x00;
            }
        }
        len = _symbols.size();
        for (int i = 0; i < len; i++) {
            if (_symbols[i] != 0x00) {
                delete _symbols[i];
                _symbols[i] = 0x00;
            }
        }
        len = _texts.size();
        for (int i = 0; i < len; i++) {
            if (_texts[i] != 0x00) {
                delete _texts[i];
                _texts[i] = 0x00;
            }
        }

        _fills.clear();     _symbols.clear();
        _strokes.clear();   _texts.clear();
	}

	CFill* CStyle::fill() const
	{
		return _fill;
	}

	CStroke* CStyle::stroke() const
	{
		return _stroke;
	}

	CSymbol* CStyle::symbol() const
	{
		return _symbol;
	}

	CText* CStyle::text() const
	{
        return _text;
	}

    CFill* CStyle::fill(int i) const
    {
        if (i >= _fills.size())
            return 0x00;
        return _fills[i];
    }

    CStroke* CStyle::stroke(int i) const
    {
        if (i >= _strokes.size())
            return 0x00;
        return _strokes[i];
    }

    CSymbol* CStyle::symbol(int i) const
    {
        if (i >= _symbols.size())
            return 0x00;
        return _symbols[i];
    }

    CText* CStyle::text(int i) const
    {
        if (i >= _texts.size())
            return 0x00;
        return _texts[i];
    }

	std::string CStyle::styleIndex() const
	{
		return _styleIndex;
	}
	std::string CStyle::styleType() const
	{
		return _styleType;
	}
	void CStyle::setStyleType(std::string type)
	{
		_styleType = type;
	}
	void CStyle::createFill()
	{
		_fill = new CFill();
	}
	void CStyle::createStroke()
	{
		_stroke = new CStroke();
	}
	void CStyle::createSymbol()
	{
		_symbol = new CSymbol();
	}
	void CStyle::createText()
	{
		_text = new CText();
	}
	int CStyle::loadStyle(const char* path)
	{
        ifstream infile;

        infile.open(path);
        if (!infile) {
            cout << "open style file failed" << endl;
            return -1;
        }
        string buffer = "";

        vector<string> results;
        getline(infile, buffer); // layer name or style index

        while (!infile.eof()) {
            buffer.clear();
            results.clear();
            getline(infile, buffer);
            int size = buffer.size();
            if (size && buffer[size - 1] == '\r')
                buffer = buffer.substr(0, size - 1);
            CStringUtil::split(buffer, ":", results);
            
            /*************************** 解析一个符号样式 ****************************/
            if (results.size() == 2 && results[0].compare("symbolizerType") == 0) {
                setStyleType(results[1]);
                while (!infile.eof() && buffer.size()) {
                    buffer.clear();
                    results.clear();
                    getline(infile, buffer);

                    size = buffer.size();
                    if (size && buffer[size - 1] == '\r' )
                        buffer = buffer.substr(0, size - 1);
                    CStringUtil::split(buffer, ":", results);
                    
                    /********************** 解析一个样式单元，如 stroke *****************/
                    if (results.size() == 1 && results[0].compare("stroke") == 0) {
                        //createStroke();
                        CStroke* stroke = new CStroke();
                        while (!infile.eof() && buffer.compare("end") != 0) {
                            buffer.clear();
                            results.clear();
                            getline(infile, buffer);
                            size = buffer.size();
                            if (size && buffer[size - 1] == '\r')
                                buffer = buffer.substr(0, size - 1);

                            CStringUtil::split(buffer, ":", results);

                            setStroke(results, stroke);
                        }

                        _strokes.push_back(stroke);
                    }
                    if (results.size() == 1 && results[0].compare("fill") == 0) {
                        //createFill();
                        CFill* fill = new CFill();
                        while (!infile.eof() && buffer.compare("end") != 0) {
                            buffer.clear();
                            results.clear();
                            getline(infile, buffer);

                            size = buffer.size();
                            if (size && buffer[size - 1] == '\r')
                                buffer = buffer.substr(0, size - 1);
                            CStringUtil::split(buffer, ":", results);

                            setFill(results, fill);
                        }
                        _fills.push_back(fill);
                    }

                    if (results.size() == 1 && results[0].compare("font") == 0) {
                        //createText();
                        CText* text = new CText();
                        while (!infile.eof() && buffer.compare("end") != 0) {
                            buffer.clear();
                            results.clear();
                            getline(infile, buffer);

                            size = buffer.size();
                            if (size && buffer[size - 1] == '\r')
                                buffer = buffer.substr(0, size - 1);
                            CStringUtil::split(buffer, ":", results);
                            
                            setFont(results, text);
                        }
                        _texts.push_back(text);
                    }
                }
            }
        }
		return 0;
	}

    void wipeNRofString(char* buffer)
    {
        int size = strlen(buffer);
        for (int i = 0; i <= size; i++) {
            if (buffer[i] == '\r' || buffer[i] == '\n') {
                buffer[i] = '\0';
                break;
            }
        }       
        return;
    }
    
    
        int CStyle::loadStyleCmethod(const char* path)
        {
            FILE* infile = fopen(path, "r");
    
            if (!infile) {
                cout << "Error opening " << path << endl;
                return -1;
            }
            else {
                //cout << "OK opening " << path << endl;
            }
            string buffer = "";
            char str[30 + 1] = "";
    
            vector<string> results;
            char* temp= fgets(str, 30, infile); // layer name or style index
            buffer = str;
    
            while (temp) {
                buffer.clear();
                results.clear();
                temp = fgets(str, 30, infile);
                wipeNRofString(str);
                buffer = str;
                //int size = buffer.size();
                //buffer = buffer.substr(0, size - 1);
                //if (size && buffer[size - 1] == '\r')
                    //buffer = buffer.substr(0, size - 1);
                CStringUtil::split(buffer, ":", results);
    
                /*************************** 解析一个符号样式 ****************************/
                if (results.size() == 2 && results[0].compare("symbolizerType") == 0) {
                    setStyleType(results[1]);
                    while (temp && buffer.size()) {
                        buffer.clear();
                        results.clear();
                        temp = fgets(str, 30, infile);
                        wipeNRofString(str);
                        buffer = str;
                        CStringUtil::split(buffer, ":", results);
                        /********************** 解析一个样式单元，如 stroke *****************/
                        if (results.size() == 1 && results[0].compare("stroke") == 0) {                    
                            //createStroke();
                            CStroke* stroke = new CStroke();
                            while (temp && buffer.compare("end") != 0) {
                                buffer.clear();
                                results.clear();
                                temp = fgets(str, 30, infile);
                                wipeNRofString(str);
                                buffer = str;
    
                                CStringUtil::split(buffer, ":", results);
    
                                setStroke(results, stroke);
                            }
    
                            _strokes.push_back(stroke);
                        }
                        if (results.size() == 1 && results[0].compare("fill") == 0) {
                            //createFill();
                            CFill* fill = new CFill();
                            while (temp && buffer.compare("end") != 0) {
                                buffer.clear();
                                results.clear();
                                temp = fgets(str, 30, infile);
                                wipeNRofString(str);
                                buffer = str;
                                CStringUtil::split(buffer, ":", results);
    
                                setFill(results, fill);
                            }
                            _fills.push_back(fill);
                        }
    
                        if (results.size() == 1 && results[0].compare("font") == 0) {
                            //createText();
                            CText* text = new CText();
                            while (temp && buffer.compare("end") != 0) {
                                buffer.clear();
                                results.clear();
                                temp = fgets(str, 30, infile);
                                wipeNRofString(str);
                                buffer = str;
                                CStringUtil::split(buffer, ":", results);
    
                                setFont(results, text);
                            }
                            _texts.push_back(text);
                        }
                    }
                }
            }
            fclose(infile);
            return 0;
        }


    void CStyle::setStroke(vector<string> data)
    {
        if (data[0].compare("color") == 0)
            stroke()->getColor()->setColor16(data[1]);
        if (data[0].compare("opacity") == 0)
            stroke()->getColor()->setOpacity(atof(data[1].c_str()));
        if (data[0].compare("width") == 0)
            stroke()->setWidth(atof(data[1].c_str()));
        if (data[0].compare("linecap") == 0)
            stroke()->setLinecap(data[1]);
        if (data[0].compare("linejoin") == 0)
            stroke()->setLineJoin(data[1]);
        if (data[0].compare("dash-offset") == 0)
            stroke()->setDashoffset(data[1]);
    }

    void CStyle::setStroke(vector<string> data, CStroke* stroke)
    {
        if (data[0].compare("color") == 0)
            stroke->getColor()->setColor16(data[1]);
        if (data[0].compare("opacity") == 0)
            stroke->getColor()->setOpacity(atof(data[1].c_str()));
        if (data[0].compare("width") == 0)
            stroke->setWidth(atof(data[1].c_str()));
        if (data[0].compare("linecap") == 0)
            stroke->setLinecap(data[1]);
        if (data[0].compare("linejoin") == 0)
            stroke->setLineJoin(data[1]);
        if (data[0].compare("dashoffset") == 0)
            stroke->setDashoffset(data[1]);
        if (data[0].compare("dash-array") == 0)
            stroke->setDashArray(data[1]);
    }

    void CStyle::setFill(vector<string> data)
    {
        if (data[0].compare("color") == 0)
            fill()->getColor()->setColor16(data[1]);
        if (data[0].compare("opacity") == 0)
            fill()->getColor()->setOpacity(atof(data[1].c_str()));
    }

    void CStyle::setFill(vector<string> data, CFill* fill)
    {
        if (data[0].compare("color") == 0)
            fill->getColor()->setColor16(data[1]);
        if (data[0].compare("opacity") == 0)
            fill->getColor()->setOpacity(atof(data[1].c_str()));
    }

    void CStyle::setFont(vector<string> data)
    {
        if (data[0].compare("font-color") == 0)
            text()->getColor()->setColor16(data[1]);
        if (data[0].compare("opacity") == 0)
            text()->getColor()->setOpacity(atof(data[1].c_str()));
        if (data[0].compare("label") == 0)
            text()->setLabel(data[1]);
        if (data[0].compare("font-family") == 0)
            text()->font()->setFont(data[1]);
        if (data[0].compare("font-size") == 0)
            text()->font()->setFontSize(atoi(data[1].c_str()));
        if (data[0].compare("font-style") == 0)
            text()->font()->setFontStyle(data[1]);
        if (data[0].compare("font-weight") == 0)
            text()->font()->setFontWeight(data[1]);
    }

    void CStyle::setFont(vector<string> data, CText* text)
    {
        if (data[0].compare("font-color") == 0)
            text->getColor()->setColor16(data[1]);
        if (data[0].compare("opacity") == 0)
            text->getColor()->setOpacity(atof(data[1].c_str()));
        if (data[0].compare("label") == 0)
            text->setLabel(data[1]);
        if (data[0].compare("font-family") == 0)
            text->font()->setFont("SIMHEI");
        if (data[0].compare("font-size") == 0)
            text->font()->setFontSize(atoi(data[1].c_str()));
        if (data[0].compare("font-style") == 0)
            text->font()->setFontStyle(data[1]);
        if (data[0].compare("font-weight") == 0)
            text->font()->setFontWeight(data[1]);
    }

}

