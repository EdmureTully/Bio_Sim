#ifndef BIOSIM_LOAD_TGAIMAGE_H
#define BIOSIM_LOAD_TGAIMAGE_H



#include <fstream>
#include <vector>

/*
Enthält Member für den Eingabestream, Höhe und Breite des TGA-Bildes,
Bits Pro Pixel, einen Vektor für die Bilddaten, einen Puffer zum Einlesen
der Bilddaten und die Größe des Bildes.
Außerdem Methoden zum Laden des Bildes, zum Überprüfen, ob das Bild den
geforderten Maßstab entspricht und eine Funktion, um die Bildpixel korrekt zu sortieren.
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