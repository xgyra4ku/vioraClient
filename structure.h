#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <map>
#include <string>
#include <vector>
#include <cstring>

struct Message {
    std::map<std::string, std::string> data; // Данные сообщения

    // Сериализация в массив байт
    [[nodiscard]] std::vector<char> serialize() const {
        std::vector<char> buffer;
        for (const auto& [key, value] : data) {
            size_t key_size = key.size();
            size_t value_size = value.size();

            // Запись размеров ключа и значения
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&key_size), reinterpret_cast<const char*>(&key_size) + sizeof(size_t));
            buffer.insert(buffer.end(), key.begin(), key.end());

            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&value_size), reinterpret_cast<const char*>(&value_size) + sizeof(size_t));
            buffer.insert(buffer.end(), value.begin(), value.end());
        }
        return buffer;
    }

    // Десериализация из массива байт
    void deserialize(const std::vector<char>& buffer) {
        data.clear();
        size_t offset = 0;
        while (offset < buffer.size()) {
            if (offset + sizeof(size_t) > buffer.size()) break;

            // Чтение размера ключа
            size_t key_size;
            std::memcpy(&key_size, buffer.data() + offset, sizeof(size_t));
            offset += sizeof(size_t);
            if (offset + key_size > buffer.size()) break;

            // Чтение ключа
            std::string key(buffer.data() + offset, key_size);
            offset += key_size;

            if (offset + sizeof(size_t) > buffer.size()) break;

            // Чтение размера значения
            size_t value_size;
            std::memcpy(&value_size, buffer.data() + offset, sizeof(size_t));
            offset += sizeof(size_t);
            if (offset + value_size > buffer.size()) break;

            // Чтение значения
            std::string value(buffer.data() + offset, value_size);
            offset += value_size;

            data[key] = value;
        }
    }
};

#endif // STRUCTURE_H
