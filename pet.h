#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <cstdlib>

class Pet {
protected:
    std::string name_;
    int hp_;
    int max_hp_;
    int attack_;
    int defense_;
    int speed_;
    int level_;
    double growth_rate_;
    int exp_;
    int skill_uses_;
    const int max_skill_uses_;

public:
    Pet(const std::string& name, int hp, int attack, int defense, int speed, int level)
        : name_(name), hp_(hp), max_hp_(hp), attack_(attack), defense_(defense), 
          speed_(speed), level_(level), growth_rate_(1.0), exp_(0), 
          skill_uses_(2), max_skill_uses_(2) {}

    virtual ~Pet() = default;

    virtual std::string attack() const = 0;
    virtual std::string get_type() const = 0;
    virtual std::string special_skill() const = 0;

    void train() {
        exp_ += 10;
        if (exp_ >= level_ * 20) {
            level_up();
        }
    }

    virtual void level_up() {
        level_++;
        exp_ = 0;
        attack_ += static_cast<int>(5 * growth_rate_);
        defense_ += static_cast<int>(3 * growth_rate_);
        speed_ += static_cast<int>(2 * growth_rate_);
        max_hp_ += static_cast<int>(10 * growth_rate_);
        hp_ = max_hp_;
        std::cout << name_ << " 升级了！当前等级: " << level_ << std::endl;
    }

    void evolve() {
        if (level_ >= 10) {
            double old_rate = growth_rate_;
            growth_rate_ *= 1.5;
            attack_ += 20;
            defense_ += 10;
            speed_ += 8;
            max_hp_ += 50;
            hp_ = max_hp_;
            std::cout << name_ << " 进化了！形态改变！" << std::endl;
            std::cout << "  属性大幅提升！攻击力+" << 20 << " 防御力+" << 10 
                      << " 速度+" << 8 << " HP+" << 50 << std::endl;
            std::cout << "  属性增长率从 " << old_rate << " 提升至 " << growth_rate_ << "！" << std::endl;
        } else {
            std::cout << "等级不足10级，无法进化！" << std::endl;
        }
    }

    void take_damage(int damage) {
        int damage_reduction = defense_ / 2;
        int final_damage = damage - damage_reduction;
        if (final_damage < 1) final_damage = 1;
        hp_ -= final_damage;
        if (hp_ < 0) hp_ = 0;
    }

    void restore_hp() {
        hp_ = max_hp_;
    }

    int get_hp() const { return hp_; }
    int get_max_hp() const { return max_hp_; }
    int get_attack() const { return attack_; }
    int get_defense() const { return defense_; }
    int get_speed() const { return speed_; }
    int get_level() const { return level_; }
    std::string get_name() const { return name_; }
    int get_skill_uses() const { return skill_uses_; }
    int get_max_skill_uses() const { return max_skill_uses_; }

    void set_hp(int hp) { hp_ = hp; }
    void set_max_hp(int max_hp) { max_hp_ = max_hp; }
    void set_attack(int attack) { attack_ = attack; }
    void set_defense(int defense) { defense_ = defense; }
    void set_speed(int speed) { speed_ = speed; }
    void set_level(int level) { level_ = level; }
    void set_skill_uses(int uses) { skill_uses_ = uses; }
    void use_skill() { if (skill_uses_ > 0) skill_uses_--; }
    bool add_skill_use() { 
        if (skill_uses_ < max_skill_uses_) { 
            skill_uses_++; 
            return true; 
        } 
        return false; 
    }

    int get_combat_power() const {
        return attack_ * level_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Pet& pet) {
        os << "【" << pet.get_type() << "】" << pet.name_ 
           << " | HP: " << pet.hp_ << "/" << pet.max_hp_
           << " | 攻击力: " << pet.attack_ 
           << " | 防御力: " << pet.defense_
           << " | 速度: " << pet.speed_
           << " | 等级: " << pet.level_ 
           << " | 战斗力: " << pet.get_combat_power();
        return os;
    }

    friend bool operator>(const Pet& a, const Pet& b) {
        return a.get_combat_power() > b.get_combat_power();
    }

    friend bool operator<(const Pet& a, const Pet& b) {
        return a.get_combat_power() < b.get_combat_power();
    }
};

class FirePet : public Pet {
public:
    FirePet(const std::string& name) 
        : Pet(name, 90 + rand() % 20, 13 + rand() % 6, 5 + rand() % 4, 12 + rand() % 6, 1) {}

    std::string attack() const override {
        return "喷出火焰！";
    }

    std::string get_type() const override {
        return "火系";
    }

    std::string special_skill() const override {
        return "烈焰风暴 - 造成1.5倍伤害！";
    }
};

class WaterPet : public Pet {
public:
    WaterPet(const std::string& name) 
        : Pet(name, 110 + rand() % 20, 10 + rand() % 5, 8 + rand() % 5, 8 + rand() % 5, 1) {}

    std::string attack() const override {
        return "发射水炮！";
    }

    std::string get_type() const override {
        return "水系";
    }

    std::string special_skill() const override {
        return "海啸冲击 - 造成1.3倍伤害并回复10HP！";
    }
};

class MechPet : public Pet {
public:
    MechPet(const std::string& name) 
        : Pet(name, 70 + rand() % 20, 18 + rand() % 6, 12 + rand() % 6, 6 + rand() % 4, 1) {}

    std::string attack() const override {
        return "激光扫描！";
    }

    std::string get_type() const override {
        return "机械系";
    }

    std::string special_skill() const override {
        return "电磁脉冲 - 造成2倍伤害！";
    }
};
