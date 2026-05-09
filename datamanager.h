#pragma once
#include "pet.h"
#include "backpack.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <iostream>

class DataManager {
private:
    static const std::string SAVE_FILE;

public:
    static void save_pets(const std::vector<std::shared_ptr<Pet>>& pets) {
        std::ofstream file(SAVE_FILE);
        if (!file.is_open()) {
            std::cerr << "无法打开存档文件" << std::endl;
            return;
        }

        for (const auto& pet : pets) {
            file << pet->get_name() << "|"
                 << pet->get_type() << "|"
                 << pet->get_hp() << "|"
                 << pet->get_max_hp() << "|"
                 << pet->get_attack() << "|"
                 << pet->get_defense() << "|"
                 << pet->get_speed() << "|"
                 << pet->get_level() << "|"
                 << pet->get_skill_uses() << "\n";
        }
        file.close();
        std::cout << "宠物数据已保存" << std::endl;
    }

    static std::vector<std::shared_ptr<Pet>> load_pets() {
        std::vector<std::shared_ptr<Pet>> pets;
        std::ifstream file(SAVE_FILE);
        
        if (!file.is_open()) {
            std::cout << "未找到存档文件，将创建新游戏" << std::endl;
            return pets;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            std::istringstream iss(line);
            std::string name, type;
            int hp, max_hp, attack, defense, speed, level, skill_uses;
            std::string token;
            
            std::getline(iss, name, '|');
            std::getline(iss, type, '|');
            std::getline(iss, token, '|'); hp = std::stoi(token);
            std::getline(iss, token, '|'); max_hp = std::stoi(token);
            std::getline(iss, token, '|'); attack = std::stoi(token);
            std::getline(iss, token, '|'); defense = std::stoi(token);
            std::getline(iss, token, '|'); speed = std::stoi(token);
            std::getline(iss, token, '|'); level = std::stoi(token);
            std::getline(iss, token, '|'); skill_uses = std::stoi(token);

            std::shared_ptr<Pet> pet = create_pet_by_type(name, type, hp, max_hp, attack, defense, speed, level, skill_uses);
            if (pet) {
                pets.push_back(pet);
            }
        }
        
        file.close();
        std::cout << "已加载 " << pets.size() << " 只宠物" << std::endl;
        return pets;
    }

    static void save_backpack(const Backpack<Item>& backpack) {
        std::ofstream file("backpack_data.txt");
        if (!file.is_open()) return;

        for (size_t i = 0; i < backpack.size(); ++i) {
            auto item = backpack.get_item(i);
            file << item->get_name() << "|"
                 << item->get_value() << "\n";
        }
        file.close();
    }

    static Backpack<Item> load_backpack() {
        Backpack<Item> backpack;
        std::ifstream file("backpack_data.txt");
        
        if (!file.is_open()) return backpack;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            std::istringstream iss(line);
            std::string name;
            int value;
            
            std::getline(iss, name, '|');
            iss >> value;

            if (name.find("药水") != std::string::npos) {
                backpack.add(std::make_shared<Potion>(name, value, 30));
            } else {
                backpack.add(std::make_shared<EnhancementStone>(name, value, 5));
            }
        }
        
        file.close();
        return backpack;
    }

private:
    static std::shared_ptr<Pet> create_pet_by_type(const std::string& name, 
                                                    const std::string& type,
                                                    int hp, int max_hp, int attack, 
                                                    int defense, int speed, int level,
                                                    int skill_uses) {
        std::shared_ptr<Pet> pet;
        
        if (type == "火系") {
            pet = std::make_shared<FirePet>(name);
        } else if (type == "水系") {
            pet = std::make_shared<WaterPet>(name);
        } else if (type == "机械系") {
            pet = std::make_shared<MechPet>(name);
        } else {
            return nullptr;
        }

        pet->set_level(level);
        pet->set_max_hp(max_hp);
        pet->set_hp(hp);
        pet->set_attack(attack);
        pet->set_defense(defense);
        pet->set_speed(speed);
        pet->set_skill_uses(skill_uses);
        
        return pet;
    }
};

const std::string DataManager::SAVE_FILE = "save_data.txt";
