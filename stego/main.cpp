#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>


void embed_message(const std::string &filename, const std::string &message);
std::string extract_message(const std::string &filename);
void delete_message(const std::string &fileName);

int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cerr << "Использование: " << argv[0] << " <mode> <filename> [message]" << std::endl;
        return 1; 
    }

    int mode = std::stoi(argv[1]); 
    std::string filename = argv[2];
    std::string message;

    if (mode == 0) {

        if (argc < 4) {
            std::cerr << "Ошибка: сообщение не указано для режима 0." << std::endl;
            return 1;
        }

        message = argv[3];
        embed_message(filename, message);

    } else if (mode == 1) {

        std::string extracted_message = extract_message(filename);
        std::cout << "Извлеченное сообщение: " << extracted_message << std::endl;

    } else if (mode == 2) {

        delete_message(filename);

    } else {

        std::cerr << "Ошибка: неверный режим. Используйте 0, 1 или 2." << std::endl;
        return 1;

    }

    return 0;
}


void embed_message(const std::string &filename, const std::string &message) {

    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "Ошибка открытия файла " << filename << std::endl;
        return;
    }

    size_t message_length = message.size();

    file.write(message.data(), message_length);

    uint64_t length = static_cast<uint64_t>(message_length);
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));

    file.close();
}


std::string extract_message(const std::string &filename) {

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Ошибка открытия файла " << filename << std::endl;
        return "";
    }

    std::streamsize size = file.tellg(); 
    if (size < 8) {
        std::cerr << "Недостаточно данных для извлечения сообщения." << std::endl;
        return "";
    }

    file.seekg(-8, std::ios::end);
    uint64_t message_length;
    file.read(reinterpret_cast<char*>(&message_length), sizeof(message_length));

    if (size < static_cast<std::streamsize>(message_length + 8)) {
        std::cerr << "Недостаточно данных для извлечения сообщения." << std::endl;
        return "";
    }

    file.seekg(-(message_length + 8), std::ios::end);
    std::vector<char> message(message_length);
    file.read(message.data(), message_length);

    file.close();

    return std::string(message.data(), message_length);
}

void delete_message(const std::string &fileName){

    std::ifstream file(fileName, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Ошибка открытия файла " << fileName << std::endl;
        return;
    }

    std::streamsize size = file.tellg();
    if (size < 8) {
        std::cerr << "Недостаточно данных для удаления сообщения." << std::endl;
        return;
    }

    file.seekg(-8, std::ios::end);
    uint64_t message_length;
    file.read(reinterpret_cast<char*>(&message_length), sizeof(message_length));

    if (size < static_cast<std::streamsize>(message_length + 8)) {
        std::cerr << "Недостаточно данных для удаления сообщения." << std::endl;
        return;
    }

    file.seekg(0, std::ios::beg);
    
    std::vector<char> buffer;
    buffer.resize(size - (message_length + 8)); 
    file.read(buffer.data(), size - (message_length + 8));

    std::ofstream tempFile("temp.bin", std::ios::binary);
    if (!tempFile) {
        std::cerr << "Ошибка создания временного файла." << std::endl;
        return;
    }

    tempFile.write(buffer.data(), buffer.size());

    file.close();
    tempFile.close();

    std::remove(fileName.c_str());
    std::rename("temp.bin", fileName.c_str());

    std::cout << "Сообщение успешно удалено." << std::endl;
}