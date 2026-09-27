#include <string>
#include <iostream>
#include<filesystem>
#include <algorithm>
#include "Image_Class.h"


bool THERE_IS_IMAGE = 0;
std::string SAVE_DIRECTORY = "Saved/";
std::string TEMP_DIRECTORY = "Temp/";

// clear the temp folder after save
void remove_temp(){
    std::filesystem::remove_all("Temp");
    std::filesystem::create_directory("Temp");
}

//  parser to get the extension from the input 
std::string extension(std::string &path){
    std::string ext = std::filesystem::path(path).extension().string();
    if (ext!="" && ext[0]=='.'){
        return ext.substr(1);
    }
    return "";
    
}
bool valid_extenstion(std::string &path){
    std::string types[5] = {"png","jpg","tga","bmp","jpeg"};
    std::string x = extension(path);
    for (auto type : types){
        if (type==x){
            return true;
        }
    }
    return false;
}

// output path
void output_path(std::string &outputPath){
        std::cout << "Enter outputPath: ";
        std::cin >> outputPath;
}
// saving images
void save_image(Image &image,std::string &outputPath ){
    image.saveImage(outputPath);
}

void saving_state(Image& image,std::string &outputPath){
        std::string save;

        bool saving = 1;
        
        while(saving){
            std::cout << "Do you want to save?(y/n)"<<"\n";
            std::cin >> save;
            if (save=="y"){
                output_path(outputPath);
                if(valid_extenstion(outputPath)){
                    outputPath = SAVE_DIRECTORY + outputPath;
                    save_image(image,outputPath);
                    std::cout << "Saved succesfully!"<<"\n";
                    saving = 0;
                }
                else{
                    std::cout << "Invalid extension"<<"\n";
                }
            }
            else if (save=="n"){
                saving = 0;
            }
            else{
                std::cout <<"Enter valid choice: "<<"\n"; 
            }
        }
        remove_temp();
}


// exiting function
void exiting(bool &running){
    // std::cout << "Do you want to keep editing?(y/n)"<<"\n";
    running = 0;
    remove_temp();
}

// function to check wheter the image there or not
bool file_exists(std::string &path){
    bool exists = std::filesystem::exists(path);
    return exists;
}
// load image
void load_image(Image &image,std::string &inputPath){
    std::cout << "Enter inputPath: ";
    std::cin >>inputPath;
    if (file_exists(inputPath) && valid_extenstion(inputPath)){
        Image temp(inputPath);
        image = temp;
        THERE_IS_IMAGE = 1;
        std::cout << "Image loaded"<<"\n";
    }
    else{
        std::cout << "Inavlid!"<<"\n";
        load_image(image,inputPath);
    }
}


std::string get_filename(std::string path){
    signed int found = path.find_last_of("/");
    // if (found!=-1){
    return path.substr(found+1);
    //     return path.substr(found+1);
    // }
    // return path;
}
void invert_filter(Image &image,std::string &inputPath){
    for (int i = 0;i < image.width;i++){
        for (int j = 0;j < image.height;j++){
            for (int k = 0;k<image.channels;k++){
                image(i,j,k) = 255 - image(i,j,k);
            }
        }
    }
    inputPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,inputPath);
}

void grayscale_filter(Image &image,std::string &inputPath){
  
    for (int i = 0;i < image.width;i++){
        unsigned int avg=0;
        for (int j = 0;j < image.height;j++){
            for (int k = 0;k<image.channels;k++){
                avg += image(i,j,k);
            }
            avg /=3;
            image.setPixel(i,j,0,avg) ;
            image.setPixel(i,j,1,avg) ;
            image.setPixel(i,j,2,avg);

        }
    }
    inputPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,inputPath);
    std::cout << "grayscale applied"<<"\n";
}

int main(){

  
    bool running = true;

    Image img;
    std::string inputPath,outputPath;

    while(running){

        std::cout << "1 - Load Image"<<"\n";
        std::cout << "2 - Grayscale filter"<<"\n";
        std::cout << "3 - Invert Filter"<<"\n";
        std::cout << "4 - Save Image"<<"\n";
        std::cout << "5 - Exit"<<"\n";
        
        // Select filter 
        // filiter selection in function

        int number;
        
        std::cin>>number;
        switch (number)
        {
        case 1:
            load_image(img,inputPath);
            break;
        case 2:
            if(THERE_IS_IMAGE){
                grayscale_filter(img,inputPath);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 3:
            if (THERE_IS_IMAGE){
                invert_filter(img,inputPath);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 4:
            // save Image 
            saving_state(img,outputPath);
            break;
        case 5:
            // exit
            exiting(running);
            break;
        default:
            std::cout << "Invalid number"<<"\n";
            break;

        }

        // filter make current image to work on, and reserve the original

    }

    /*
    original pic > filter > new pic > image var = new pic
    */
}
