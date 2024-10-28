#include <iostream>
#include <filesystem>
#include <map>

namespace fs = std::filesystem;

int main() {

    std::map<std::string, int> fileTypeCount;

    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            if (extension.empty()){
                extension = "empty type";
            }
            fileTypeCount[extension]++;
        }
    }

    std::cout << "Количество файлов по типам в текущей директории:\n";

    for (const auto& [type, count] : fileTypeCount) {
        std::cout << type << ": " << count << std::endl;
    }

    return 0;
}