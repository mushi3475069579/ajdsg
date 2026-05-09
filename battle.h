#pragma once
#include "pet.h"
#include "backpack.h"
#include "exceptions.h"
#include <memory>
#include <iostream>
#include <string>
#include <limits>

class BattleSystem {
public:
    static void battle(std::shared_ptr<Pet>& pet1, std::shared_ptr<Pet>& pet2, Backpack<Item>& backpack) {
        std::cout << "\n========== 战斗开始 ==========" << std::endl;
        std::cout << *pet1 << " | 技能次数: " << pet1->get_skill_uses() << "/" << pet1->get_max_skill_uses() << std::endl;
        std::cout << *pet2 << " | 技能次数: " << pet2->get_skill_uses() << "/" << pet2->get_max_skill_uses() << std::endl;

        std::shared_ptr<Pet> player_pet = pet1;
        std::shared_ptr<Pet> enemy_pet = pet2;

        if (enemy_pet->get_speed() > player_pet->get_speed()) {
            std::cout << "\n" << enemy_pet->get_name() << " 速度更快，先攻击！" << std::endl;
            execute_enemy_attack(enemy_pet, player_pet);
            if (player_pet->get_hp() <= 0) {
                end_battle(enemy_pet, player_pet);
                return;
            }
        } else if (enemy_pet->get_speed() == player_pet->get_speed()) {
            std::cout << "\n双方速度相同，" << player_pet->get_name() << " 先手！" << std::endl;
        } else {
            std::cout << "\n" << player_pet->get_name() << " 速度更快，先攻击！" << std::endl;
        }

        while (player_pet->get_hp() > 0 && enemy_pet->get_hp() > 0) {
            std::cout << "\n--- 你的回合 ---" << std::endl;
            std::cout << player_pet->get_name() << " HP: " << player_pet->get_hp() << "/" << player_pet->get_max_hp() 
                      << " | 技能次数: " << player_pet->get_skill_uses() << "/" << player_pet->get_max_skill_uses() << std::endl;
            std::cout << "请选择操作:" << std::endl;
            std::cout << "1. 普通攻击" << std::endl;
            std::cout << "2. 特殊技能" << std::endl;
            std::cout << "3. 使用道具" << std::endl;
            
            int choice = get_valid_int_input("选择操作 (1-3): ");

            try {
                switch (choice) {
                    case 1:
                        execute_normal_attack(player_pet, enemy_pet);
                        break;
                    case 2:
                        if (player_pet->get_skill_uses() <= 0) {
                            throw InvalidMoveException("特殊技能已用完！");
                        }
                        execute_special_skill(player_pet, enemy_pet);
                        player_pet->use_skill();
                        break;
                    case 3:
                        use_item_in_battle(player_pet, backpack);
                        continue;
                    default:
                        throw InvalidMoveException("无效的操作选择！");
                }
            } catch (const InvalidMoveException& e) {
                std::cout << "异常: " << e.what() << std::endl;
                continue;
            }

            if (enemy_pet->get_hp() <= 0) break;

            std::cout << "\n--- " << enemy_pet->get_name() << " 的回合 ---" << std::endl;
            execute_enemy_attack(enemy_pet, player_pet);
        }

        end_battle(player_pet, enemy_pet);
    }

private:
    static int get_valid_int_input(const std::string& prompt) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入无效，请输入数字！" << std::endl;
        }
    }

    static double get_type_multiplier(const std::string& attacker_type, const std::string& defender_type) {
        if ((attacker_type == "火系" && defender_type == "水系") ||
            (attacker_type == "水系" && defender_type == "机械系") ||
            (attacker_type == "机械系" && defender_type == "火系")) {
            return 1.5;
        } else if ((attacker_type == "水系" && defender_type == "火系") ||
                   (attacker_type == "机械系" && defender_type == "水系") ||
                   (attacker_type == "火系" && defender_type == "机械系")) {
            return 0.75;
        }
        return 1.0;
    }

    static void execute_normal_attack(std::shared_ptr<Pet>& attacker, std::shared_ptr<Pet>& defender) {
        std::cout << attacker->get_name() << " 使用 " << attacker->attack() << std::endl;
        
        double multiplier = get_type_multiplier(attacker->get_type(), defender->get_type());
        int damage = static_cast<int>(attacker->get_attack() * multiplier);
        
        defender->take_damage(damage);
        
        if (multiplier > 1.0) {
            std::cout << "  属性克制！" << attacker->get_type() << "克制" << defender->get_type() << "！" << std::endl;
        } else if (multiplier < 1.0) {
            std::cout << "  属性弱效！" << attacker->get_type() << "被" << defender->get_type() << "克制！" << std::endl;
        }
        
        std::cout << "  " << defender->get_name() << " 受到 " << damage 
                  << " 点伤害，剩余HP: " << defender->get_hp() << std::endl;
    }

    static void execute_special_skill(std::shared_ptr<Pet>& attacker, std::shared_ptr<Pet>& defender) {
        std::cout << attacker->get_name() << " 使用特殊技能: " << attacker->special_skill() << std::endl;
        
        int damage = attacker->get_attack();
        
        std::string type = attacker->get_type();
        if (type == "火系") {
            damage = static_cast<int>(damage * 1.5);
        } else if (type == "水系") {
            damage = static_cast<int>(damage * 1.3);
            attacker->set_hp(attacker->get_hp() + 10);
            std::cout << "  " << attacker->get_name() << " 回复了10点HP！当前HP: " << attacker->get_hp() << std::endl;
        } else if (type == "机械系") {
            damage = static_cast<int>(damage * 2.0);
        }
        
        double multiplier = get_type_multiplier(attacker->get_type(), defender->get_type());
        damage = static_cast<int>(damage * multiplier);
        
        defender->take_damage(damage);
        
        if (multiplier > 1.0) {
            std::cout << "  属性克制！" << attacker->get_type() << "克制" << defender->get_type() << "！" << std::endl;
        } else if (multiplier < 1.0) {
            std::cout << "  属性弱效！" << attacker->get_type() << "被" << defender->get_type() << "克制！" << std::endl;
        }
        
        std::cout << "  " << defender->get_name() << " 受到 " << damage 
                  << " 点伤害，剩余HP: " << defender->get_hp() << std::endl;
    }

    static void execute_enemy_attack(std::shared_ptr<Pet>& attacker, std::shared_ptr<Pet>& defender) {
        if (attacker->get_skill_uses() > 0 && rand() % 2 == 0) {
            execute_special_skill(attacker, defender);
            attacker->use_skill();
        } else {
            execute_normal_attack(attacker, defender);
        }
    }

    static void use_item_in_battle(std::shared_ptr<Pet>& pet, Backpack<Item>& backpack) {
        if (backpack.empty()) {
            std::cout << "背包为空，无法使用道具！" << std::endl;
            return;
        }

        std::cout << "\n--- 选择道具 ---" << std::endl;
        backpack.show();
        int index = get_valid_int_input("选择要使用的道具索引 (输入-1取消): ");
        
        if (index == -1) {
            std::cout << "取消使用道具" << std::endl;
            return;
        }

        try {
            auto item = backpack.get_item(index);
            
            if (auto potion = std::dynamic_pointer_cast<Potion>(item)) {
                int heal = potion->get_heal_amount();
                int current_hp = pet->get_hp();
                int max_hp = pet->get_max_hp();
                int actual_heal = std::min(heal, max_hp - current_hp);
                pet->set_hp(current_hp + actual_heal);
                std::cout << "使用 " << item->get_name() << "，" << pet->get_name() 
                          << " 回复了 " << actual_heal << " HP！当前HP: " << pet->get_hp() << "/" << max_hp << std::endl;
                backpack.remove(index);
            } else if (auto skill_stone = std::dynamic_pointer_cast<SkillStone>(item)) {
                if (pet->add_skill_use()) {
                    std::cout << "使用 " << item->get_name() << "，" << pet->get_name() 
                              << " 的技能次数恢复1次！当前技能次数: " 
                              << pet->get_skill_uses() << "/" << pet->get_max_skill_uses() << std::endl;
                    backpack.remove(index);
                } else {
                    std::cout << "技能次数已满，无法使用 " << item->get_name() << "！" << std::endl;
                }
            } else if (auto stone = std::dynamic_pointer_cast<EnhancementStone>(item)) {
                int boost = stone->get_boost_amount();
                pet->set_attack(pet->get_attack() + boost);
                std::cout << "使用 " << item->get_name() << "，" << pet->get_name() 
                          << " 的攻击力永久提升 " << boost << "！当前攻击力: " << pet->get_attack() << std::endl;
                backpack.remove(index);
            }
        } catch (const std::out_of_range& e) {
            std::cout << "无效的道具索引！" << std::endl;
        }
    }

    static void end_battle(std::shared_ptr<Pet>& winner, std::shared_ptr<Pet>& loser) {
        std::cout << "\n========== 战斗结束 ==========" << std::endl;
        std::cout << winner->get_name() << " 获胜！" << std::endl;
        std::cout << loser->get_name() << " 被击败！" << std::endl;
        
        winner->restore_hp();
        loser->restore_hp();
        std::cout << "\n战斗结束，双方宠物血量已自动回复满！" << std::endl;
    }
};
