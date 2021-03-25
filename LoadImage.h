#ifndef BIOSIM_LOAD_TGAIMAGE_H
#define BIOSIM_LOAD_TGAIMAGE_H



#include <fstream>
#include <vector>

/*
Enth�lt Member f�r den Eingabestream, H�he und Breite des TGA-Bildes,
Bits Pro Pixel, einen Vektor f�r die Bilddaten, einen Puffer zum Einlesen
der Bilddaten und die Gr��e des Bildes.
Au�erdem Methoden zum Laden des Bildes, zum �berpr�fen, ob das Bild den
geforderten Ma�stab entspricht und eine Funktion, um die Bildpixel korrekt zu sortieren.
*/




class TGA{
public:

	TGA(const std::string  &filename);
	~TGA();
	TGA();
	short int getHeight();
	short int getWidth();
	int getType();
	std::vector <unsigned char> getImageData();
private:
	int type;
	std::vector<unsigned char> TGAimage;
	std::string filepath;
	short int bpp;
	short int width;
	short int height;
	int id;
	unsigned long imagesize;
	bool LoadTGA_intoMemory(std::ifstream &fIn);
	bool readHeader(char header[]);
	bool loadData(unsigned char* imageData);
	void BGRtoRGB();
	

};


#endif