#include<iostream>

#include <filesystem>
using namespace std;

void get_create_filepath(filesystem::path path){
    if (path.parent_path()!=""){
        filesystem::create_directories(path.parent_path());
    }
}
int main() {
    std::string s = "hellooooooo.jpg";
    get_create_filepath(s);
}