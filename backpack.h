#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

class Item {
protected:
    std::string name_;
    int value_;
public:
    Item(const std::string& name, int value) : name_(name), value_(value) {}
    virtual ~Item() = default;

    std::string get_name() const { return name_; }
    int get_value() const { return value_; }

    virtual std::string get_description() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Item& item) {
        os << item.name_ << " (价值: " << item.value_ << ")";
        return os;
    }
};

class Potion : public Item {
private:
    int heal_amount_;
public:
    Potion(const std::string& name, int value, int heal) 
        : Item(name, value), heal_amount_(heal) {}

    std::string get_description() const override {
        return "药水 - 恢复 " + std::to_string(heal_amount_) + " HP";
    }

    int get_heal_amount() const { return heal_amount_; }
};

class EnhancementStone : public Item {
private:
    int boost_amount_;
public:
    EnhancementStone(const std::string& name, int value, int boost) 
        : Item(name, value), boost_amount_(boost) {}

    std::string get_description() const override {
        return "强化石 - 提升 " + std::to_string(boost_amount_) + " 攻击力";
    }

    int get_boost_amount() const { return boost_amount_; }
};

class SkillStone : public Item {
public:
    SkillStone(const std::string& name, int value) 
        : Item(name, value) {}

    std::string get_description() const override {
        return "技能石 - 恢复1次特殊技能使用次数";
    }
};

template<typename T>
class Backpack {
private:
    std::vector<std::shared_ptr<T>> items_;

public:
    void add(std::shared_ptr<T> item) {
        items_.push_back(item);
    }

    void remove(int index) {
        if (index < 0 || index >= static_cast<int>(items_.size())) {
            throw std::out_of_range("索引超出范围");
        }
        items_.erase(items_.begin() + index);
    }

    void show() const {
        if (items_.empty()) {
            std::cout << "背包为空" << std::endl;
            return;
        }
        for (size_t i = 0; i < items_.size(); ++i) {
            std::cout << "[" << i << "] " << *items_[i] << std::endl;
        }
    }

    std::shared_ptr<T> get_item(int index) const {
        if (index < 0 || index >= static_cast<int>(items_.size())) {
            throw std::out_of_range("索引超出范围");
        }
        return items_[index];
    }

    size_t size() const { return items_.size(); }

    bool empty() const { return items_.empty(); }

    void sort_by_value() {
        std::sort(items_.begin(), items_.end(),
            [](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
                return a->get_value() > b->get_value();
            });
    }

    std::vector<std::shared_ptr<T>>& get_items() { return items_; }
    const std::vector<std::shared_ptr<T>>& get_items() const { return items_; }
};
