#pragma once
class CTile
{
public:
	CTile(int zoom, int row, int column, int type);

	virtual ~CTile();

	virtual int loadTile(const char* path) = 0;
	int zoom() const;
	int row() const;
	int column() const;

protected:
	int _zoom; 
	int _row; 
	int _column; 
	int _type; 
};

