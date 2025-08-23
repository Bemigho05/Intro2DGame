#pragma once

#include "../Animation/Animation.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <map>

struct TexConf { std::string name, filepath; };
struct FontConf { std::string name, filepath; };
struct AnimConf { std::string animName, texName; int frameCount, animSpeed, index; bool reverse; };

class Assets {

	std::map<std::string, sf::Texture> m_textures;
	std::map<std::string, Animation> m_animations;
	std::map<std::string, sf::Font> m_fonts;

	void addTexture(const std::string& name, const std::string& path, bool smooth = true) {
		m_textures[name] = sf::Texture();

		if (!m_textures[name].loadFromFile(path)) {
			std::cerr << "Could not load texture from: " << path << "\n";
			m_textures.erase(name);
		}
		else {
			m_textures[name].setSmooth(smooth);
			std::cout << "Loaded textures: " << path << "\n";
		}
	}

	void addAnimation(const std::string& animName, const int& index, const std::string& texName, const size_t& frameCount, const size_t& speed, const bool& reverse) {
		m_animations[animName] = Animation(animName, index, getTexture(texName), frameCount, speed, reverse);
	}

	void addFont(const std::string& name, const std::string& path) {
		m_fonts[name] = sf::Font();
		if (!m_fonts[name].loadFromFile(path)) {
			std::cerr << "Could not load from file: " << path << "\n";
			m_fonts.erase(name);
		}
	}
public:
	Assets() = default;
	~Assets() = default;

	// TODO
	void loadFromFile(const std::string& path) {
		std::ifstream file;
		file.open(path);

		if (!file) {
			std::cerr << "Could not load " << path << " file!\n";
			exit(-1);
		}

		std::string tmp;

		while (file >> tmp) {
			if (tmp == "Font") {
				std::string name, path;
				file >> name >> path;
				addFont(name, path);
			}
			if (tmp == "Texture") {
				std::string name, path;
				file >> name >> path;
				addTexture(name, path);
			}
			if (tmp == "Animation") {
				AnimConf animTemp{};
				file >> animTemp.animName >> animTemp.texName >> animTemp.frameCount >> animTemp.animSpeed >> animTemp.index >> animTemp.reverse;
				addAnimation(animTemp.animName, animTemp.index, animTemp.texName, animTemp.frameCount, animTemp.animSpeed, animTemp.reverse);
			}
		}
	}

	const sf::Texture& getTexture(const std::string& name) const {
		if (!m_textures.contains(name)) { std::cerr << "There's no texture with name: " << name << "\n"; exit(-1); }
		return m_textures.at(name);
	}

	const Animation& getAnimation(const std::string& name) const {
		if (!m_animations.contains(name)) { std::cerr << "There's no animation with name: " << name << "\n"; exit(-1); }
		return m_animations.at(name);
	}

	const sf::Font& getFont(const std::string& name) const {
		if (!m_fonts.contains(name)) { std::cerr << "There's not texture with name: " << name << "\n"; exit(-1); }
		return m_fonts.at(name);
	}

};
