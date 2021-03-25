// tga.cpp : Definiert den Einstiegspunkt f�r die Konsolenanwendung.
//
#include "stdafx.h"
#include "LoadImage.h"
#include "BioSim-Win32.h"
#include <string>
#include <memory>
#include <setjmp.h>
#include <stdio.h>
#include <setjmp.h>
#include <iostream>

#define RGB 24
#define RGBA 32


//Vorw�rtsdeklaration

//Konstruktor, wird mit Dateipfad und Werten(0) f�r
// Bits pro Pixel, Bildweite und -h�he initialisiert
TGA::TGA(const std::string &filename) :
filepath(filename), bpp(0), width(0), height(0){
	std::ifstream fIn;
	fIn.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try{
		fIn.open(filename, std::ios::binary);

	}
	catch (std::ifstream::failure const&e){
		std::cerr << "Fehler beim Oeffnen der Datei: " << filepath << ". Fehler: " << e.what() << std::endl;
	}
	try{
		LoadTGA_intoMemory(fIn);
	}
	catch (std::exception const& e){
		std::cout << e.what() << std::endl;
	}
	if (fIn.is_open())
		fIn.close();
}

//Standardkonstruktor
TGA::TGA(){}
//Destruktor
TGA::~TGA(){}

/*
L�dt die Datei durch �bergebenen Pfad und ruft die Methoden
checkheader, loaddata und BGRtoRGB auf.
*/
bool TGA::LoadTGA_intoMemory(std::ifstream &fIn){
	unsigned char TGAheader[] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned char TGACompare[12];
	unsigned char header[6];
	unsigned char* imageData;

	if (!fIn.read(reinterpret_cast<char*>(TGACompare), sizeof(TGACompare))){
		delete[] & TGACompare;
		throw std::exception("Einlesen abgebrochen:Fehler beim Einlesen der ersten 12 Byte des Headers");
	}

	if (memcmp(TGACompare, (TGAheader), sizeof(TGAheader))){
		delete[] & TGACompare;
		throw std::exception("Einlesen abgebrochen:Fehler im Datei-Header!");
	}

	if (!fIn.read(reinterpret_cast<char*>(header), sizeof(header))){
		delete[] & TGACompare;
		throw std::exception("Einlesen abgebrochen: Fehler beim Einlesen der Headerdatei");
	}

	if (!readHeader(reinterpret_cast<char*>(header))) {
		delete[] & TGACompare;
		delete[] & header;
		throw std::exception("Einlesen abgebrochen: Hoehe, Breite oder Bits Pro Pixel sind fehlerhaft!");
	}

	fIn.seekg(0, fIn.end);
	unsigned int length = fIn.tellg();
	fIn.seekg(0, fIn.beg);

	imageData = new unsigned char[length];
	if (!fIn.read(reinterpret_cast<char*>(imageData), length)){
		delete[] & TGACompare;
		delete[] & header;
		throw std::exception("Einlesen abgebrochen: Fehler beim Einlesen der Bilddaten");
	}

	if (imageData == NULL){
		delete[] & TGACompare;
		delete[] & header;
		delete[] & imageData;
		throw std::exception("Einlesen abgebrochen: Fehler bei Speicherbelegung.");
	}

	loadData(imageData);

	if ((imagesize + 18) > length){
		delete[] & TGACompare;
		delete[] & header;
		delete[] & imageData;
		throw std::exception("Fehler beim Einlesen der Bilddaten!");
	}

	BGRtoRGB();
	
	if (bpp == RGB){
		type = 0;
	}

	if (bpp == RGBA){
		type = 1;
	}
	


	std::cout << " Einlesen der TGA-Datei:" << filepath << " war erfolgreich!" << std::endl;
	return true;
}
//Liest die signifikanten Daten aus dem Header
bool TGA::readHeader(char header[]){
	short bytesPerPixel;
	width = header[0] + (header[1] << 8);
	height = header[2] + (header[3] << 8);
	bpp = header[4];
	bytesPerPixel = (bpp) / 8;
	if (width <= 0 || height <= 0 || (bpp != 24 && bpp != 32)){
		throw std::exception("Einlesen abgebrochen: Datei hat entweder falsche H�he, Breite oder Anzahlt Bits pro Pixel.");
	}
	imagesize = (height* width * bytesPerPixel);
	return true;
}
//L�dt die Bilddaten
bool TGA::loadData(unsigned char* imageData){

	TGAimage.resize(imagesize);
	memcpy((void*)TGAimage.data(), &imageData[0] + 18, imagesize);
	return true;
}

//Tauscht die Bildpixel, sodass sie RGB entsprechen
void TGA::BGRtoRGB(){
	unsigned long index, pixel;
	unsigned char *current;
	unsigned char temp;

	current = &TGAimage[0];
	pixel = height*width;

	for (index = 0; index != pixel; index++){
		temp = *current;
		*current = *(current + 2);
		*(current + 2) = temp;

		current += bpp / 8;
	}
}

int TGA::getType(){
	return type;
}

short int TGA::getHeight(){
	return height;
}

short int TGA::getWidth(){
	return width;
}

std::vector <unsigned char> TGA::getImageData(){
	return TGAimage;
}

