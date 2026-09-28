#include <string>
#include <iostream>
#include<filesystem>
#include <algorithm>
#include "Image_Class.h"


bool THERE_IS_IMAGE = 0;
std::string SAVE_DIRECTORY = "Saved/";
std::string TEMP_DIRECTORY = "temp/";

void check_make_dir(std::string &path){
    bool there_is_directory = std::filesystem::is_directory(path);
    if (!there_is_directory){
        std::filesystem::create_directory(path);
    }

}
void get_create_filepath(std::filesystem::path path){
    if (path.parent_path()!=""){
        std::filesystem::create_directories(path.parent_path());
    }
}

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
                    get_create_filepath(outputPath);
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
void exiting(Image &image,bool &running){
    // std::cout << "Do you want to keep editing?(y/n)"<<"\n";
    if (THERE_IS_IMAGE){
        std::string temp;
        saving_state(image,temp);
    }
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
    if (THERE_IS_IMAGE){
        saving_state(image,inputPath);
    }
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
void blackandwhite_filter(Image &image,std::string & inputPath){
        for (int i = 0;i < image.width;i++){
        unsigned int avg=0;
            for (int j = 0;j < image.height;j++){
                for (int k = 0;k<image.channels;k++){
                    avg += image(i,j,k);
                }
                avg /=3;
                if (avg<=128) {
                    avg = 0;
                }
                else{avg = 255;}
                image.setPixel(i,j,0,avg) ;
                image.setPixel(i,j,1,avg) ;
                image.setPixel(i,j,2,avg);
            }
    }
    inputPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,inputPath);
    std::cout << "Black&White applied"<<"\n";
}
void frame_filter(Image &image, std::string &inputPath){
    int frame_size = 20;
    int color = 0;
    char c = 'r';

    bool picking = true;
    while(picking){
        std::cout << "choose color (r, g ,b): ";
        std::cin>>c;
        if (c=='r'){
            color = 1;
            picking = false;
        }
        else if (c=='g'){ 
            color =2;
            picking=false;
        }
        else if(c=='b') {
            color = 0;
            picking=false;
        }
        else{
            std::cout << "invalid character,"<<"\n";
        }
    }
    picking = true;
    while(picking){
        std::cout << "Frame size 1-100 : ";
        std::cin >> frame_size;
        frame_size %= 101;
        picking = false;
    }
    for (int i = 0;i<image.width;i++){
        for (int j = 0;j<frame_size;j++){
            for (int k = 0;k<3;k++){
                image(i,j,color) = 255;
                image(i,j,(color+1)%3) = 0;
                image(i,j,(color+2)%3) = 0;
                color = (color+1) % 3;
            }
        }
    }
    for (int i = 0;i<frame_size;i++){
        for (int j = 0;j<image.height;j++){
            for (int k = 0;k<3;k++){
                image(i,j,color) = 255;
                image(i,j,(color+1)%3) = 0;
                image(i,j,(color+2)%3) = 0;
                color = (color+1) % 3;
            }
        }
    }
    for (int i = image.width-1;i>=image.width-frame_size;i--){
        for (int j = 0 ; j<image.height;j++){
            for (int k = 0;k<3;k++){
                image(i,j,color) = 255;
                image(i,j,(color+1)%3) = 0;
                image(i,j,(color+2)%3) = 0;
                color = (color+1) % 3;
            }
        }
    }
    for (int i =0 ;i<image.width;i++){
        for (int j = image.height-1;j>=image.height-frame_size;j--){
            for (int k = 0;k<3;k++){
                image(i,j,color) = 255;
                image(i,j,(color+1)%3) = 0;
                image(i,j,(color+2)%3) = 0;
                color = (color+1) % 3;
            }
        }
    }
    
    inputPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,inputPath);
    std::cout << "Frame Added"<<"\n";
}
void weird_filter(Image &image,std::string &inputPath){
    for (int i = 0 ; i<image.width;i++){
        for (int j = 0; j<image.height/2;j++){
            std::swap(image(i,j,0),image(i,(image.height)-1-j,0));
            std::swap(image(i,j,1),image(i,(image.height)-1-j,0));
            std::swap(image(i,j,2),image(i,(image.height)-1-j,0));
        }
    }
    inputPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,inputPath);
    std::cout << "Weird applied"<<"\n";
}
void flip(Image &image,std::string &inputPath){
    char choice = 'h';
    bool picking = true;
    while(picking){
        std::cout << "Horizontal,Vertical flip enter (h,v): ";
        std::cin>>choice;
        if (choice=='v'){
            for (int i = 0 ; i<image.width;i++){
                for (int j = 0; j<image.height/2;j++){
                    std::swap(image(i,j,0),image(i,(image.height)-1-j,0));
                    std::swap(image(i,j,1),image(i,(image.height)-1-j,1));
                    std::swap(image(i,j,2),image(i,(image.height)-1-j,2));
                }
            }
            picking = false;
        }
        else if (choice=='h'){
            for (int i = 0 ; i<image.width/2;i++){
                for (int j = 0; j<image.height;j++){
                    std::swap(image(i,j,0),image(image.width-1-i,j,0));
                    std::swap(image(i,j,1),image(image.width-1-i,j,1));
                    std::swap(image(i,j,2),image(image.width-1-i,j,2));
                }
            }
            picking = false;
        }
        else{
            std::cout << "invalid input"<<"\n";
        }
    }


    inputPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,inputPath);
    std::cout << "Flip applied"<<"\n";
}
void brightness_filter(Image &image,std::string &inputPath){
    int precentage = 1;
    char choice = 'b';
    bool b = true;
    bool picking = true;
    while(picking){
        std::cout << "Bright or Darken (b,d): ";
        std::cin >> choice;
        if (choice=='b'){
            b = true;
            picking = false;
        }
        else if (choice=='d'){
            b = false;
            picking = false;
        }
        else{
            std::cout << "invalid input\n";
        }
    }
    std::cout << "precentage (1-100) : ";
    std::cin >> precentage;
    for (int i =0 ;i<image.width;i++){
        for (int j=0;j<image.height;j++){
            for (int k =0;k<3;k++){
                if (b){
                    int temp = image(i,j,k)+(255*(precentage/100.0f));
                    image (i,j,k) = std::clamp(temp,0,255);

                }
                else{
                    int temp = image(i,j,k)-(255*(precentage/100.0f));
                    image (i,j,k) = std::clamp(temp,0,255);
                }
            }
        }
    }
    inputPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,inputPath);
    std::cout << "Brightness Edited"<<"\n";
}
void resize_filter(Image &image,std::string &inputPath){
    double newWidth = 1280,newHeight = 720;

    std::cout << "Width: ";
    std::cin >> newWidth;
    std::cout << "Height: ";
    std::cin >> newHeight;


    Image newImage(newWidth,newHeight);
    double xRatio =  (double)image.width/newWidth;
    double yRatio = (double)image.height/newHeight;

    for (int i = 0;i<newWidth;i++){
        for (int j = 0;j<newHeight;j++){
            int x = i*xRatio;
            int y = j*yRatio;
            newImage(i,j,0) = image(x,y,0); 
            newImage(i,j,1) = image(x,y,1); 
            newImage(i,j,2) = image(x,y,2); 
        }
    }
    image = newImage; 
    inputPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,inputPath);
    std::cout << "Image resized"<<"\n";
}

int main(){
    
    get_create_filepath("Saved/");
    get_create_filepath("Temp/");
    bool running = true;

    Image img;
    std::string inputPath,outputPath;

    while(running){

        std::cout << "1 - Load Image"<<"\n";
        std::cout << "2 - Grayscale filter"<<"\n";
        std::cout << "3 - Invert Filter"<<"\n";
        std::cout << "4 - Black & White Filter"<<"\n";
        std::cout << "5 - Frame Filter"<<"\n";
        std::cout << "6 - Flip Filter"<<"\n";
        std::cout << "7 - Weird Filter"<<"\n";
        std::cout << "8 - Brightness Filter"<<"\n";
        std::cout << "9 - Resize Filter"<<"\n";
        std::cout << "10 - Save Image"<<"\n";
        std::cout << "11 - Exit"<<"\n";
        
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
            if(THERE_IS_IMAGE){
                blackandwhite_filter(img,inputPath);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 5:
            if(THERE_IS_IMAGE){
                frame_filter(img,inputPath);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 6:
            if(THERE_IS_IMAGE){
                flip(img,inputPath);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 7:
            if(THERE_IS_IMAGE){
                weird_filter(img,inputPath);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 8:
            if(THERE_IS_IMAGE){
                brightness_filter(img,inputPath);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 9:
            if(THERE_IS_IMAGE){
                resize_filter(img,inputPath);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 10:
            // save Image 
            saving_state(img,outputPath);
            break;
        case 11:
            // exit
            exiting(img,running);
            break;
        default:
            std::cout << "Invalid number"<<"\n";
            break;

        }


    }

    /*
    original pic > filter > new pic > image var = new pic
    */
}
