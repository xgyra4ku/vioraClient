#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <map>
#include <string>
#include <vector>

struct Message {
    std::map<std::string, std::string> data; // Данные сообщения

    // Сериализация в массив байт
    [[nodiscard]] std::vector<char> serialize() const {
        std::vector<char> buffer;

        // Сериализация data
        for (const auto& [key, value] : data) {
            // Добавляем ключ
            buffer.insert(buffer.end(), key.begin(), key.end());
            buffer.push_back('\0'); // Разделитель

            // Добавляем значение
            buffer.insert(buffer.end(), value.begin(), value.end());
            buffer.push_back('\0'); // Разделитель
        }

        return buffer;
    }

    // Десериализация из массива байт
    void deserialize(const std::vector<char>& buffer) {
        // Десериализация data
        size_t offset = sizeof(int);
        while (offset < buffer.size()) {
            // Чтение ключа
            std::string key(buffer.data() + offset);
            offset += key.size() + 1;

            // Чтение значения
            std::string value(buffer.data() + offset);
            offset += value.size() + 1;

            // Добавление в map
            data[key] = value;
        }
    }
};

#endif // STRUCTURE_H
