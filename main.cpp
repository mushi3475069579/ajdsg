#include "pet.h"
#include "backpack.h"
#include "battle.h"
#include "datamanager.h"
#include "exceptions.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <limits>

using namespace std;

void clear_input_buffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int get_valid_int_input(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clear_input_buffer();
            return value;
        }
        clear_input_buffer();
        cout << "输入无效，请输入数字！" << endl;
    }
}

shared_ptr<Pet> create_pet() {
    cout << "\n=== 创建宠物 ===" << endl;
    cout << "1. 火系宠物" << endl;
    cout << "2. 水系宠物" << endl;
    cout << "3. 机械系宠物" << endl;
    
    int choice = get_valid_int_input("选择宠物类型 (1-3): ");
    string name;
    cout << "输入宠物名字: ";
    cin >> name;
    clear_input_buffer();
    
    shared_ptr<Pet> pet;
    switch (choice) {
        case 1: pet = make_shared<FirePet>(name); break;
        case 2: pet = make_shared<WaterPet>(name); break;
        case 3: pet = make_shared<MechPet>(name); break;
        default:
            cout << "无效选择，创建火系宠物" << endl;
            pet = make_shared<FirePet>(name);
            break;
    }
    
    cout << "\n宠物创建成功！" << endl;
    cout << *pet << endl;
    
    return pet;
}

void train_pet(shared_ptr<Pet>& pet) {
    try {
        if (pet->get_hp() <= 10) {
            throw NotEnoughHpException("血量不足，无法训练！");
        }
        pet->train();
        cout << *pet << endl;
    } catch (const NotEnoughHpException& e) {
        cout << "异常: " << e.what() << endl;
    }
}

void evolve_pet(shared_ptr<Pet>& pet) {
    pet->evolve();
    cout << *pet << endl;
}

void show_pets(const vector<shared_ptr<Pet>>& pets) {
    if (pets.empty()) {
        cout << "暂无宠物" << endl;
        return;
    }
    for (size_t i = 0; i < pets.size(); ++i) {
        cout << "[" << i << "] " << *pets[i] << endl;
    }
}

void battle_menu(vector<shared_ptr<Pet>>& pets, Backpack<Item>& backpack) {
    if (pets.size() < 2) {
        cout << "需要至少2只宠物才能进行战斗！" << endl;
        return;
    }
    
    cout << "\n=== 选择战斗宠物 ===" << endl;
    show_pets(pets);
    
    int idx1 = get_valid_int_input("选择第一只宠物 (索引): ");
    int idx2 = get_valid_int_input("选择第二只宠物 (索引): ");
    
    try {
        if (idx1 < 0 || idx1 >= static_cast<int>(pets.size()) ||
            idx2 < 0 || idx2 >= static_cast<int>(pets.size())) {
            throw InvalidMoveException("宠物索引无效！");
        }
        if (idx1 == idx2) {
            throw InvalidMoveException("不能选择同一只宠物！");
        }
        
        BattleSystem::battle(pets[idx1], pets[idx2], backpack);
    } catch (const InvalidMoveException& e) {
        cout << "异常: " << e.what() << endl;
    }
}

void backpack_menu(Backpack<Item>& backpack) {
    while (true) {
        cout << "\n=== 背包系统 ===" << endl;
        cout << "1. 查看背包" << endl;
        cout << "2. 添加药水" << endl;
        cout << "3. 添加强化石" << endl;
        cout << "4. 添加技能石" << endl;
        cout << "5. 使用道具" << endl;
        cout << "6. 丢弃道具" << endl;
        cout << "7. 按价值排序" << endl;
        cout << "0. 返回" << endl;
        
        int choice = get_valid_int_input("选择操作: ");
        
        try {
            switch (choice) {
                case 1:
                    backpack.show();
                    break;
                case 2: {
                    string name;
                    cout << "输入药水名字: ";
                    cin >> name;
                    clear_input_buffer();
                    backpack.add(make_shared<Potion>(name, 10, 30));
                    cout << "已添加药水" << endl;
                    break;
                }
                case 3: {
                    string name;
                    cout << "输入强化石名字: ";
                    cin >> name;
                    clear_input_buffer();
                    backpack.add(make_shared<EnhancementStone>(name, 20, 5));
                    cout << "已添加强化石" << endl;
                    break;
                }
                case 4: {
                    string name;
                    cout << "输入技能石名字: ";
                    cin >> name;
                    clear_input_buffer();
                    backpack.add(make_shared<SkillStone>(name, 25));
                    cout << "已添加技能石" << endl;
                    break;
                }
                case 5: {
                    if (backpack.empty()) {
                        cout << "背包为空！" << endl;
                        break;
                    }
                    backpack.show();
                    int idx = get_valid_int_input("选择道具索引: ");
                    auto item = backpack.get_item(idx);
                    cout << "使用了: " << *item << endl;
                    backpack.remove(idx);
                    break;
                }
                case 6: {
                    if (backpack.empty()) {
                        cout << "背包为空！" << endl;
                        break;
                    }
                    backpack.show();
                    int idx = get_valid_int_input("选择要丢弃的道具索引: ");
                    backpack.remove(idx);
                    cout << "已丢弃道具" << endl;
                    break;
                }
                case 7:
                    backpack.sort_by_value();
                    cout << "已按价值排序" << endl;
                    backpack.show();
                    break;
                case 0:
                    return;
                default:
                    cout << "无效选择" << endl;
            }
        } catch (const exception& e) {
            cout << "异常: " << e.what() << endl;
        }
    }
}

int main() {
    cout << "========================================" << endl;
    cout << "   虚拟宠物养成与战斗模拟器" << endl;
    cout << "========================================" << endl;
    
    vector<shared_ptr<Pet>> pets = DataManager::load_pets();
    Backpack<Item> backpack = DataManager::load_backpack();
    
    while (true) {
        cout << "\n========== 主菜单 ==========" << endl;
        cout << "1. 创建宠物" << endl;
        cout << "2. 查看宠物" << endl;
        cout << "3. 训练宠物" << endl;
        cout << "4. 进化宠物" << endl;
        cout << "5. 放生宠物" << endl;
        cout << "6. 宠物对战" << endl;
        cout << "7. 背包系统" << endl;
        cout << "8. 保存并退出" << endl;
        cout << "0. 直接退出" << endl;
        
        int choice = get_valid_int_input("选择操作: ");
        
        try {
            switch (choice) {
                case 1:
                    pets.push_back(create_pet());
                    break;
                case 2:
                    show_pets(pets);
                    break;
                case 3: {
                    if (pets.empty()) {
                        cout << "暂无宠物可训练" << endl;
                        break;
                    }
                    show_pets(pets);
                    int idx = get_valid_int_input("选择要训练的宠物 (索引): ");
                    if (idx >= 0 && idx < static_cast<int>(pets.size())) {
                        train_pet(pets[idx]);
                    } else {
                        throw InvalidMoveException("宠物索引无效！");
                    }
                    break;
                }
                case 4: {
                    if (pets.empty()) {
                        cout << "暂无宠物可进化" << endl;
                        break;
                    }
                    show_pets(pets);
                    int idx = get_valid_int_input("选择要进化的宠物 (索引): ");
                    if (idx >= 0 && idx < static_cast<int>(pets.size())) {
                        evolve_pet(pets[idx]);
                    } else {
                        throw InvalidMoveException("宠物索引无效！");
                    }
                    break;
                }
                case 5: {
                    if (pets.empty()) {
                        cout << "暂无宠物可放生" << endl;
                        break;
                    }
                    show_pets(pets);
                    int idx = get_valid_int_input("选择要放生的宠物 (索引): ");
                    if (idx >= 0 && idx < static_cast<int>(pets.size())) {
                        cout << "确定要放生 " << pets[idx]->get_name() << " 吗？(y/n): ";
                        char confirm;
                        cin >> confirm;
                        clear_input_buffer();
                        if (confirm == 'y' || confirm == 'Y') {
                            cout << pets[idx]->get_name() << " 已被放生" << endl;
                            pets.erase(pets.begin() + idx);
                        } else {
                            cout << "取消放生" << endl;
                        }
                    } else {
                        throw InvalidMoveException("宠物索引无效！");
                    }
                    break;
                }
                case 6:
                    battle_menu(pets, backpack);
                    break;
                case 7:
                    backpack_menu(backpack);
                    break;
                case 8:
                    DataManager::save_pets(pets);
                    DataManager::save_backpack(backpack);
                    cout << "游戏已保存，再见！" << endl;
                    return 0;
                case 0:
                    DataManager::save_pets(pets);
                    DataManager::save_backpack(backpack);
                    cout << "数据已自动保存，再见！" << endl;
                    return 0;
                default:
                    cout << "无效选择" << endl;
            }
        } catch (const InvalidMoveException& e) {
            cout << "异常: " << e.what() << endl;
        } catch (const exception& e) {
            cout << "未知异常: " << e.what() << endl;
        }
    }
    
    return 0;
}
