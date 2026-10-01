/*
===================================================================================================
Course: CS213 - Object Oriented Programming (2026-2027)
Assignment: Assignment 1 - Part 1 (8 Filters + Main Menu)
File Name: CS213_A1_Part1_SectionNum_YourID_YourID_YourID_YourID.cpp

Program Description:
This C++ file implements a console-based image processing application using the
`Image_Class.h` library. It shows a menu that continuously loops, 
allowing users to load, edit, and save images until they choose to exit. 
The program automatically manages file directories by creating "Saved/" and "Preview/" folders  
so that output and temporary files are stored safely, and verifies valid image extensions
(PNG, JPG, TGA, BMP, JPEG). Users can apply eight image manipulation filters: 
Grayscale conversion, Black and White thresholding, Color Inversion, Frame addition 
(with customizable colors and simple/fancy styles), Image Flipping (horizontal or vertical), 
Rotation (90, 180, or 270 degrees), Brightness adjustment (darken or lighten by a specific percentage), 
and Image Resizing to custom dimensions. Additionally, the integrated file handling system prompts 
users with options to either overwrite the original loaded image or save the newly modified version to a 
designated output directory.

Team Details & Work Breakdown:
---------------------------------------------------------------------------------------------------
Section: [s13,s14]
Shared/Collaborative Tasks: 
- Main Menu creation, integration of all filters, and File Handling (Load / Save / Exit).
1. Name: [Moamen Osama Zakaria]
   ID: [20250686]
   Assigned Tasks / Filters:
   - Filter [1]: [ Grayscale Conversion, e.g., Filter 1: Grayscale]
   - Filter [5]: [ Flip Image, e.g., Filter 5: Flip Image]

2. Name: [Ammar Mohamed Abdelmagid]
   ID: [20250413]
   Assigned Tasks / Filters:
   - Filter [2]: [ Black and White, e.g., Filter 2: Black and White]
   - Filter [6]: [ Rotate Image, e.g., Filter 6: Rotate Image]

3. Name: [Fady Emad Hanna]
   ID: [20250458]
   Assigned Tasks / Filters:
   - Filter [3]: [ Invert Image, e.g., Filter 3: Invert Image]
   - Filter [7]: [Darken and Lighten, e.g., Filter 7: Brightness Filter]

4. Name: [Mahmoud Hafez Abdelfatah]
   ID: [20250607]
   Assigned Tasks / Filters:
   - Filter [4]: [Adding a Frame to the Picture, e.g., Filter 4: Frame Filter]
   - Filter [8]: [Resizing Images, e.g., Filter 8: Resize Filter]
===================================================================================================
*/
#include <string>
#include <iostream>
#include<filesystem>
#include <algorithm>
#include "libraries/Image_Class.h"


bool THERE_IS_IMAGE = 0;
std::string SAVE_DIRECTORY = "Saved/";
std::string TEMP_DIRECTORY = "Preview/";
std::string inputPath;

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
    std::filesystem::remove_all("Preview");
    std::filesystem::create_directory("Preview/");
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
                bool picking = true;
                while(picking){
                    char saveOnFile;
                    std::cout<<"Save on file?(y/n)\n";
                    std::cin >> saveOnFile;
                    if (saveOnFile=='y'){
                        outputPath = inputPath;
                        picking = false;
                    }
                    else if(saveOnFile=='n'){
                        output_path(outputPath);
                        outputPath = SAVE_DIRECTORY + outputPath;
                        picking = false;
                    }
                    else{
                        std::cout <<"invalid input\n";
                    }
                }
                if(valid_extenstion(outputPath)){
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

void invert_filter(Image &image,std::string inputPath){
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

void grayscale_filter(Image &image,std::string inputPath){
  
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

void blackandwhite_filter(Image &image,std::string  inputPath){
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

void frame_filter(Image &image, std::string inputPath){
    
    char c;
    std::cout << "Choose frame: s = Simple, f = Fancy: ";
    std::cin >> c;

    while (c!='s' && c!='S' && c!='f' && c!='F')
    {
        std::cout << "Invalid choice. Enter s or f: ";
        std::cin >> c;
    }

    bool fancy = (c=='f' || c=='F');

    int colors[8][3] = {
        {255,0,0}, {0,255,0}, {0,0,255}, {168,5,250},
        {255,255,0}, {16,163,222}, {0,0,0}, {255,255,255}
    };        

    std::cout << "1) Red  2) Green  3) Blue  4) Purple\n"
    << "5) Yellow  6) Cyan  7) Black  8) White\n";
    
    int c1;
    std::cout << "Main color: ";
    std::cin >> c1;

    while (c1 < 1 || c1 > 8)
    {
        std::cout << "Invalid choice. Enter 1-8: ";
        std::cin >> c1;
    }

    int c2 = 0;

    if (fancy)
    {
        std::cout << "Inner color: ";
        std::cin >> c2;
        while (c2 < 1 || c2 > 8)
        {
            std::cout << "Invalid choice. Enter 1-8: ";
            std::cin >> c2;
        }
    }

    int q = 0.027*image.height, e = 0.027*image.width;

    Image nimage(image.width + 2*e, image.height + 2*q);

    int a = e/3, b = q/3;
    int W = nimage.width, H = nimage.height;

    for (int y = 0; y < nimage.height; y++)
    {
        for (int x = 0; x < nimage.width; x++)
        {
            if (x < e || x >= nimage.width-e || y < q || y >= nimage.height-q)
            {
                int color = c1-1;

                if (fancy &&(

                    (x >= a && x < 2*a && y >= b && y < H-b) ||
                    (x >= W-2*a && x < W-a && y >= b && y < H-b) ||
                    (y >= b && y < 2*b && x >= a && x < W-a) ||
                    (y >= H-2*b && y < H-b && x >= a && x < W-a)

                    )
                ) 
                    color = c2-1;

                    nimage(x,y,0) = colors[color][0];
                    nimage(x,y,1) = colors[color][1];
                    nimage(x,y,2) = colors[color][2];
            }

            else
            {
                nimage(x,y,0) = image(x-e,y-q,0);
                nimage(x,y,1) = image(x-e,y-q,1);
                nimage(x,y,2) = image(x-e,y-q,2);
            }
        }
    }
    image = nimage;
    inputPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,inputPath);
    std::cout << "Frame Added"<<"\n";
}

/*void weird_filter(Image &image,std::string inputPath){
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
}*/
void flip(Image &image,std::string inputPath){
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

void brightness_filter(Image &image,std::string inputPath){
    std::cout << "Select either darken or lighten:\n	1- Darken\n	2- Lighten\n";
    bool picking = true;
    int selection = 0;

    while (picking)
    {
        std::cin >> selection;
        if (selection != 1 && selection != 2) {
            std::cout << "Invalid selection. Please choose 1 or 2.\n";
        } else {
            picking = false;
        }
    }
    	
	if (selection == 1) {

		std::cout << "Write down the percentage you want to darken by (from 0 to 100):\n";
		float percentage;
		std::cin >> percentage;
		percentage /= 100;
		for (int i = 0;i < image.width;++i) {
			for (int j = 0;j < image.height;j++) {
				for (int k = 0;k < 3;k++) {
					if ((image(i, j, k) - (image(i, j, k) * percentage)) >= 0) {
						image(i, j, k) -= (image(i, j, k) * percentage);
					}
					else image(i, j, k) = 0;
				}
			}
		}
	}

	else if (selection == 2) {

		std::cout << "Write down the percentage you want to lighten by (from 0 to 100):\n";
		float percentage;
		std:: cin >> percentage;
		percentage /= 100;
		for (int i = 0;i < image.width;++i) {
			for (int j = 0;j < image.height;j++) {
				for (int k = 0;k < 3;k++) {
					if ((image(i, j, k) + (image(i, j, k) * percentage)) <= 255) {
						image(i, j, k) += (image(i, j, k) * percentage);
					}
					else image(i, j, k) = 255;
				}
			}
		}
	}
    inputPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,inputPath);
    std::cout << "Brightness Edited"<<"\n";
}

void resize_filter(Image &image,std::string inputPath){
    double newWidth, newHeight ;

    std::cout << "Please Enter Your prefered width and height : " ;

    std::cout << "Width: ";
    std::cin >> newWidth ;

    std::cout << "Height: ";
    std::cin >> newHeight ;


    Image newImage(newWidth,newHeight);

    double xRatio =  (double)image.width/newWidth;
    double yRatio = (double)image.height/newHeight;

    for (int y=0 ; y<newHeight ; y++){
        
        for (int x=0 ; x<newWidth ; x++){

            int pw = std::min((int)(x * xRatio), image.width - 1);
            int ph = std::min((int)(y * yRatio), image.height - 1);
            
                newImage (x, y, 0) = image (pw, ph, 0) ;
                newImage (x, y, 1) = image (pw, ph, 1) ;
                newImage (x, y, 2) = image (pw, ph, 2) ;

        }

    }

    image = newImage; 
    inputPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,inputPath);
    std::cout << "Image resized"<<"\n";
}

void rotate_filter(Image &image,std::string inputPath){
    
    bool picking = true;
    int choice = 0;

    while(picking){
        std::cout << "Rotate 90-180-270: ";
        std::cin >>choice;
        if (choice==90){

            picking = false;
        }
        else if(choice==180){

            picking = false;
        }
        else if(choice==270){

            picking = false;
        }
        else{
            std::cout<<"invalid input!\n";
        }
    } 


    if(choice==90){
        Image newImage(image.height,image.width);
        for (int i =0;i<image.width;i++){
            for (int j=0;j<image.height;j++){
                newImage(image.height-1-j,i, 0) = image(i, j, 0);
                newImage(image.height-1-j,i, 1) = image(i, j, 1);
                newImage(image.height-1-j,i, 2) = image(i, j, 2);
            }
        }
        image = newImage;
    }
    if(choice==180){
        Image newImage(image.width,image.height);
        for (int i =0;i<image.width;i++){
            for (int j=0;j<image.height;j++){
                newImage(image.width-1-i,image.height-1-j, 0) = image(i, j, 0);
                newImage(image.width-1-i,image.height-1-j, 1) = image(i, j, 1);
                newImage(image.width-1-i,image.height-1-j, 2) = image(i, j, 2);
            }
        }
        image = newImage;
    }
    if(choice==270){
        Image newImage(image.height,image.width);
        for (int i =0;i<image.width;i++){
            for (int j=0;j<image.height;j++){
                newImage(j, image.width-i-1, 0) = image(i, j, 0);
                newImage(j, image.width-i-1, 1) = image(i, j, 1);
                newImage(j, image.width-i-1, 2) = image(i, j, 2);
            }
        }
        image = newImage;
    }

    inputPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,inputPath);
    std::cout << "Image rotated"<<"\n";
}

int main(){
    
    get_create_filepath("Saved/");
    get_create_filepath("Preview/");
    bool running = true;

    Image img;
    std::string outputPath;

    while(running){

        std::cout << "1 - Load Image"<<"\n";
        std::cout << "2 - Grayscale filter"<<"\n";
        std::cout << "3 - Black & White Filter"<<"\n";
        std::cout << "4 - Invert Filter"<<"\n";
        std::cout << "5 - Frame Filter"<<"\n";
        std::cout << "6 - Flip Filter"<<"\n";
        std::cout << "7 - Rotate Filter"<<"\n";
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
            if(THERE_IS_IMAGE){
                blackandwhite_filter(img,inputPath);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 4:
            if (THERE_IS_IMAGE){
                invert_filter(img,inputPath);
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
                rotate_filter(img,inputPath);
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
