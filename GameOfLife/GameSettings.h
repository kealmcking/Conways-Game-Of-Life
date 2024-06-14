#pragma once
#include <fstream>

enum ColorType {
	Living,
	Dead,
	Grid
};

struct GameSettings {
	int gridSize = 15;
	int interval = 100;
	
	bool showNeighborCount;
	bool isFinite = true;
	bool displayGrid = true;

	int windowWidth = 800;
	int windowHeight = 600;

	unsigned int livingRed = 128;
	unsigned int livingGreen = 128;
	unsigned int livingBlue = 128;
	unsigned int livingAlpha = 128;

	unsigned int deadRed = 255;
	unsigned int deadGreen = 255;
	unsigned int deadBlue = 255;
	unsigned int deadAlpha = 255;

	void SetColor(wxColor color, ColorType type) {
		if (type == ColorType::Living) {
			livingRed = color.GetRed();
			livingGreen = color.GetGreen();
			livingBlue = color.GetBlue();
			livingAlpha = color.GetAlpha();
		}
		else if (type == ColorType::Dead) {
			deadRed = color.GetRed();
			deadGreen = color.GetGreen();
			deadBlue = color.GetBlue();
			deadAlpha = color.GetAlpha();
		}
		else {
			std::cout << "Not a supported type" << std::endl;
		}
	}

	wxColor GetColor(ColorType type) {
		wxColor rColor;
		if (type == ColorType::Living) {
			wxColor color(livingRed, livingGreen, livingBlue, livingAlpha);
			rColor = color;
		}
		else if (type == ColorType::Dead) {
			wxColor color(deadRed, deadGreen, deadBlue, deadAlpha);
			rColor = color;
		}
		else {
			std::cout << "Not a supported type" << std::endl;
		}
		return rColor;
		
	}

	int GetGridSize() {
		return gridSize;
	}

	void SetGridSize(int _gridSize) {
		gridSize = _gridSize;
	}

	int GetInterval() {
		return interval;
	}

	void SetInterval(int _interval) {
		interval = _interval;
	}

	void SaveData() {
		std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
		file.write((char*)this, sizeof(GameSettings));
		file.close();
	}

	void LoadData() {
		std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
		file.read((char*)this, sizeof(GameSettings));
		file.close();
	}

};
