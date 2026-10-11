/*

Course: CS213 - Object Oriented Programming (2026-2027)
Assignment: Assignment 1 - Part 2 (16 Filters + Main Menu)
File Name: CS213_A1_Part1_S3_20250686_20250458_20250607.cpp

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

Document: https://drive.google.com/file/d/1zJUikcQ2g2O8hwMYNokaND-b8aV-b2qa/view?usp=sharing
Demo Video: https://youtu.be/ScNTBvacwhM 
Repo: https://github.com/0xMoamen/Mozzarella

Section: [S3,S4]
Shared/Collaborative Tasks: 
- Main Menu creation, integration of all filters, and File Handling (Load / Save / Exit).
1. Name: [Moamen Osama Zakaria]
    ID: [20250686]
    Assigned Tasks / Filters:
   - Filter [1]: [ Grayscale Conversion, e.g., Filter 1: Grayscale]
   - Filter [2]: [ Black and White, e.g., Filter 2: Black and White]
   - Filter [5]: [ Flip Image, e.g., Filter 5: Flip Image]
   - Filter [9]: [ Merge Images, e.g., Filter 9: Merge Images]
   - Filter [13]: [ Sunlight Filter, e.g., Filter 13: Sunlight Filter]
   - Filter [14]: [ Old TV Filter, e.g., Filter 14: TV Filter]

2. Name: [Fady Emad Hanna]
    ID: [20250458]
    Assigned Tasks / Filters:
    - Filter [3]: [ Invert Image, e.g., Filter 3: Invert Image]
    - Filter [7]: [ Darken and Lighten, e.g., Filter 7: Brightness  Filter]  
    - Filter [10]: [ Detect Image Edges, e.g., Filter 10: Edges Detection]
    - Filter [11]: [ Crop Images, e.g., Filter 11: Crop Image]
    - Filter [15]: [ Purple Filter, e.g., Filter 15: Purple]

3. Name: [Mahmoud Hafez Abdelfatah]
    ID: [20250607]
    Assigned Tasks / Filters:
   - Filter [4]: [ Adding a Frame to the Picture, e.g., Filter 4: Frame Filter]
   - Filter [6]: [ Rotate Image, e.g., Filter 6: Rotate Image]
   - Filter [8]: [ Resizing Images, e.g., Filter 8: Resize Filter]
   - Filter [12]: [ Blur Images, e.g., Filter 12: Blur Filter]
   - Filter [16]: [ Infrared looking, e.g., Filter 16: Infrared Filter]

*/
#include <string>
#include <iostream>
#include<filesystem>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include "libraries/Image_Class.h"


bool THERE_IS_IMAGE = 0;
std::string SAVE_DIRECTORY = "Saved/";
std::string TEMP_DIRECTORY = "Preview/";
std::string inputPath="newImage.jpg"; //default value if there is no inputPath
// std::vector<Image>History; //for undo

void check_make_dir(std::string &path){
    bool there_is_directory = std::filesystem::is_directory(path);
    if (!there_is_directory){
        std::filesystem::create_directory(path);
    }

}
// when user do the outpath like "notcreatedfolder/newImage.jpg" it makes the folder for the image
void get_create_filepath(std::filesystem::path path){ 
    if (path.parent_path()!=""){
        std::filesystem::create_directories(path.parent_path());
    }
}

// clear the temp folder after save and exit
void remove_temp(){
    std::filesystem::remove_all("Preview");
    std::filesystem::create_directory("Preview/");
}

//  parser to get the extension from the path 
std::string extension(std::string &path){
    std::string ext = std::filesystem::path(path).extension().string();
    if (ext!="" && ext[0]=='.'){
        return ext.substr(1);
    }
    return "";
    
}
// we check the extension when loading and saving images
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
//check whether the use will save or not and on same file or not 
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

// function to check wheter the image exists
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

//get the filename from the user path to create the file in preview folder without the any other folders inside
std::string get_filename(std::string path){
    signed int found = path.find_last_of("/");
    // if (found!=-1){
    return path.substr(found+1);
    //     return path.substr(found+1);
    // }
    // return path;
}

void invert_filter(Image &image){
    for (int i = 0;i < image.width;i++){
        for (int j = 0;j < image.height;j++){
            for (int k = 0;k<image.channels;k++){
                image(i,j,k) = 255 - image(i,j,k);
            }
        }
    }
    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    std::cout << "Invert Applied\n";
}

void grayscale_filter(Image &image){
  
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
    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    std::cout << "grayscale applied"<<"\n";
}

void blackandwhite_filter(Image &image){
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
    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    std::cout << "Black&White applied"<<"\n";
}

void frame_filter(Image &image){
    
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
    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
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
void flip(Image &image){
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


    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    std::cout << "Flip applied"<<"\n";
}

void brightness_filter(Image &image){
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
    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    std::cout << "Brightness Edited"<<"\n";
}

void resize_filter(Image &image){
    double newWidth, newHeight ;

    std::cout << "Please Enter Your prefered width and height \n" ;

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
    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    std::cout << "Image resized"<<"\n";
}

void rotate_filter(Image &image){
    
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

    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    std::cout << "Image rotated"<<"\n";
}

void CropImage(Image& image) {
	std::cout << "Enter the coordinates of the top-left corner (x, y):\n";
	int x, y;
	bool validInput = false;
	while (!validInput) {
		std::cin >> x >> y;
		if (std::cin.fail() || x < 0 || y < 0 || x >= image.width || y >= image.height) {
			std::cout << "Invalid input. Please enter valid coordinates(positive integers).\n";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		} else {
			validInput = true;
		}
	}

	std::cout << "Enter the width and height of the crop area:\n";
    std::cout <<"Max Width: "<<image.width - x<< "Max height: "<<image.height - y<< "\n";
	int width, height;
	validInput = false;
	while (!validInput) {
		std::cin >> width >> height;
       
		if (std::cin.fail() || width <= 0 || height <= 0 || width > image.width - x || height > image.height - y) {
			std::cout << "Invalid input. Please enter valid dimensions(positive integers).\n";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		} else {
			validInput = true;
		}
	}

	Image NewImage(width, height);

	for (int i=x , i_new = 0;i<x+width;i++, i_new++) {
		for (int j=y,j_new = 0;j<y+height;j++,j_new++) {
			for (int k = 0;k < image.channels;k++) {
				NewImage(i_new, j_new, k) = image(i, j, k);
			}
		}
	}

	image = NewImage;
    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    std::cout << "Image cropped"<<"\n";
}

// Notes , first remove the THERE_IS_IMAGE check from here and the menu
// Make 3 option, Direct merge "if same (w,h)"
// Resize to biggest(w,h)
// intersection merge
void merge(Image &image){
    std::string path1,path2;
    std::cout << "Path1: ";std::cin>>path1;
    std::cout << "Path2: ";std::cin>>path2;

    Image image1(path1);
    Image image2(path2);

    image = image1; // Make the first image is the dominant and the first layer
    ::inputPath = path1; // Make the outputPath you pass to save_state to the original pic for "SaveOnFile"

    // Direct if same (w,h)
    if (image1.height==image2.height){
        if(image1.width==image2.width){
            for (int i =0 ; i<image1.width ; i++){
                for (int j=0 ; j<image1.height ; j++){
                    for (int k=0 ; k<3; k++){
                        image(i,j,k) =( image1(i,j,k)+image2(i,j,k))/2;
                    }
                }   
            }
        }
    }
    // Resize and intersection things
    else{
        int c1 = 1;
        std::cout << "You have 2 options: (1) Resize, (2) Intersection: ";
        std::cin >> c1;
        if (c1==1){
            int newWidth = image1.width;
            int newHeight=image1.height;
            Image newImage(newWidth,newHeight);
            double xRatio =  (double)image2.width/newWidth;
            double yRatio = (double)image2.height/newHeight;
            for (int y=0 ; y<newHeight ; y++){
                for (int x=0 ; x<newWidth ; x++){
                    int pw = std::min((int)(x * xRatio), image2.width - 1);
                    int ph = std::min((int)(y * yRatio), image2.height - 1);
                        newImage (x, y, 0) = image2 (pw, ph, 0) ;
                        newImage (x, y, 1) = image2 (pw, ph, 1) ;
                        newImage (x, y, 2) = image2 (pw, ph, 2) ;
                }
            }
            image2 = newImage;
            for (int i =0 ; i<image1.width ; i++){
                for (int j=0 ; j<image1.height ; j++){
                    for (int k=0 ; k<3; k++){
                        image(i,j,k) =(image1(i,j,k) + image2(i,j,k))/2;
                    }
                }   
            }
        }
        else if(c1==2){
            int width_inter = std::min(image1.width,image2.width);
            int height_inter = std::min(image1.height,image2.height);
            for (int x=0; x<width_inter; x++){
                for (int y=0; y<height_inter; y++){
                    for (int channel=0; channel<3; channel++){
                        image(x,y,channel) = (image1(x,y,channel)+image2(x,y,channel))/2;
                    }
                }
            }

        }

    }
    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    
    std::cout << "Merged"<<"\n";

}
void sunlight_filter(Image &image){
    for (int i=0;i<image.width;i++){
        for (int j=0;j<image.height;j++){
            int r = image(i,j,0);// r
            int g = image(i,j,1);// r
            r+=40;
            g+=40;
            r = std::clamp(r,0,255);
            g = std::clamp(g,0,255);
            image(i,j,0)=r;
            image(i,j,1)=g;
        }
    }
    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    std::cout << "Sunlight Added !\n";
}

// void infrared_filter(Image &image){
//     int power = 200;
//     std::cout << "Enter intensity of red (50,100,200): ";
//     std::cin >>power;
//     for (int i = 0;i < image.width;i++){
//         for (int j = 0;j < image.height;j++){
//             for (int k = 0;k<3;k++){
//                 image(i,j,k) = 255 - image(i,j,k);
//             } // invert
//             int r = image(i,j,0)+power; // 50 good and natural,100 cool,200 whats in the assignment
//             r = std::clamp(r,0,255);
//             image(i,j,0) = r;
//         }
//     }
//     std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
//     save_image(image,previewPath);
//     std::cout << "Infrared Applied\n";
// }

void purple_filter(Image& image) {
	for (int i = 0; i < image.width; i++) {
		for (int j = 0; j < image.height; j++) {	
		if ((image (i,j,0) * 1.2) > 255)
			image(i, j, 0) = 255;
		else
			image(i, j, 0) *= 1.2;

		if ((image(i, j, 1) * 0.81) < 0)
			image(i, j, 1) = 0;
		else
			image(i, j, 1) *= 0.81;
		
		if ((image(i, j, 2) * 1.2) > 255)
			image(i, j, 2) = 255;
		else
			image(i, j, 2) *= 1.2;
	
		}
	}

    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    std::cout << "Image is purple"<<"\n";
}
void tv_filter(Image &image){
    
    // Color Distortions
    for (int i =0;i<image.width;i++){
        for (int y=0;y<image.height;y+=3){ // Play in how many lines applied to for intensity
            for (int channel =0;channel<3;channel++){
                int temp = image(i,y,channel);
                temp -= std::rand()%200; 
                temp = std::clamp(temp,0,255);
                image(i,y,channel)=temp;
            }
        }
    }

    
    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    std::cout << "Tv-Filter Applied \n";
}
void detect_edges(Image& image) {
	
    grayscale_filter(image);

	int pivot = 35;

	for (int i = 0; i < image.width - 1;++i) {
		for (int j = 0;j < image.height - 1; ++j) {
			
			if ( std:: abs(image(i, j, 0) - image(i+1,j,0)) >= pivot || std::abs(image(i, j, 0) - image(i,j+1,0)) >= pivot) {
				image(i, j, 0) = 0;
				image(i, j, 1) = 0;
				image(i, j, 2) = 0;
			}
			else {
				image(i, j, 0) = 255;
				image(i, j, 1) = 255;
				image(i, j, 2) = 255;
			}
		}
	}	

    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    std::cout << "edges detected"<<"\n";
}
// void blur(Image &image){
//     Image newImage = image;

//     for (int x =0;x<image.width; x++){
//         for (int y=0;y<image.height;y++){
//             for (int c=0;c<3;c++){
//                 int temp = 0;
//                 int count=0;
//                 int steps = 10; // how many pixel will he take around the pixel in each direction = intensity of blur
//                 for (int step =1; step<=steps;step++){
//                     //up
//                     if (y-step>=0){
//                         temp+=image(x,y-step,c);
//                         count++;
//                     }
//                     //left
//                     if (x-step>=0){
//                         temp+=image(x-step,y,c);
//                         count++;
//                     }
//                     //down
//                     if (y+step>=0 && y+step<=image.height){
//                         temp+=image(x,y+step,c);
//                         count++;
//                     }
//                     //right
//                     if (x+step>=0 && x+step<=image.width){
//                         temp+=image(x+step,y,c);
//                         count++;
//                     }

//                 }
//                 temp = temp/count; //avg
//                 newImage(x,y,c) = temp;
//             }
//         }
//     }
//     image = newImage;
//     std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
//     save_image(image,previewPath);
//     std::cout << "Blurred!"<<"\n";
// }

void blur_filter (Image &image){

    int h = image.height ;
    int w = image.width ;
    int R = 0 ;
    int G = 0 ;
    int B = 0 ;

    int n = 7 ;
    // cout << "Enter Thr Blur Intecity Between 1 to 5  :" << endl ;
    // cin >> n ;

    Image nimage(w, h);

    for (int y = 0; y < image.height; y++)
    {
        for (int x = 0; x < image.width; x++)
        {
            R = image(x,y,0) ;
            G = image(x,y,1) ;
            B = image(x,y,2) ;
            int count = 1 ;

            int RM = 0 ;
            int GM = 0 ;
            int BM = 0 ;

            for (int i=1 ; i<=n ; i++){

                if (x >= 0 && x < w-i ){  // add the right
                    count+=1 ;
                    RM += image(x+i,y,0) ;
                    GM += image(x+i,y,1) ;
                    BM += image(x+i,y,2) ;
                }

                if (x >= i && x < w ){    // add the left
                    count+=1 ;
                    RM += image(x-i,y,0) ;
                    GM += image(x-i,y,1) ;
                    BM += image(x-i,y,2) ;
                }

                if (y >= 0 && y < h-i ){   // add the bottom
                    count+=1 ;
                    RM += image(x,y+i,0) ;
                    GM += image(x,y+i,1) ;
                    BM += image(x,y+i,2) ;
                }

                if (y >= i && y < h ){    // add the top
                    count+=1 ;
                    RM += image(x,y-i,0) ;
                    GM += image(x,y-i,1) ;
                    BM += image(x,y-i,2) ;
                }


            }



            nimage(x,y,0) = ((R+RM)/count);
            nimage(x,y,1) = ((G+GM)/count);
            nimage(x,y,2) = ((B+BM)/count);



        }
    }
    image  = nimage;
    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    save_image(image,previewPath);
    std::cout << "Blurred!"<<"\n";

}
void infrared_filter (Image &image){
    
    int R = 0;
    int G = 0;
    int B = 0;

    for (int y = 0; y < image.height; y++)
    {
        for (int x = 0; x < image.width; x++)
        {

            R = image(x,y,0) ;
            G = image(x,y,1) ;
            B = image(x,y,2) ;

            int gray = 0.299 * R + 0.587 * G + 0.114 * B;
            int inv = 255 - gray;

            image(x,y,0) = 255;
            image(x,y,1) = inv;
            image(x,y,2) = inv;

            // الفاتح احمر الغامق ابيض

        }
    }
    std::cout << "Infrared Applied\n";
    std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
    
    save_image(image,previewPath);

}

// void undo(Image &image){
//     // stack pop
//     if(History.size()>1){
//         History.pop_back();
//         // image = top
//         image = History.back();
//         // preview
//         std::string previewPath = TEMP_DIRECTORY + get_filename(inputPath);
//         save_image(image,previewPath);
//     }
//     else{
//         std::cout << "Nothing to undo\n";
//         return;
//     }
// }

int main(){

    std::cout << R"(
    ========================================================================================
    || |\   /|     ___    |----   |----   /\     |---\    |-----  |       |       /---\   ||
    || | \ / |    |   |      /       /   /  \    |   |    |       |       |       |   |   ||
    || |     |    |   |     /       /   /----\   |---/    |---    |       |       |---|   ||
    || |     |    |   |    /       /    |    |   | \      |       |       |       |   |   ||
    || |     |    |___|   /----   /---- |    |   |   \    |-----  |-----  |-----  |   |   ||
    ========================================================================================
    )"<<"\n";
    
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
        std::cout << "10 - Crop Filter"<<"\n";
        std::cout << "11 - Merge Filter"<<"\n";
        std::cout << "12 - Sunlight Filter"<<"\n";
        std::cout << "13 - Infrared Filter"<<"\n";
        std::cout << "14 - Purple Filter"<<"\n";
        std::cout << "15 - Tv-Filter Filter"<<"\n";
        std::cout << "16 - Detect Edges Filter"<<"\n";
        std::cout << "17 - Blur Filter"<<"\n";

        std::cout << "18 - Save Image"<<"\n";
        std::cout << "19 - Exit"<<"\n";
        
        int number;
        
        std::cin>>number;
        switch (number)
        {
        case 1:
            load_image(img,inputPath);
            break;
        case 2:
            if(THERE_IS_IMAGE){
                grayscale_filter(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 3:
            if(THERE_IS_IMAGE){
                blackandwhite_filter(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 4:
            if (THERE_IS_IMAGE){
                invert_filter(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 5:
            if(THERE_IS_IMAGE){
                frame_filter(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 6:
            if(THERE_IS_IMAGE){
                flip(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 7:
            if(THERE_IS_IMAGE){
                rotate_filter(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 8:
            if(THERE_IS_IMAGE){
                brightness_filter(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 9:
            if(THERE_IS_IMAGE){
                resize_filter(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 10:
            if(THERE_IS_IMAGE){
                CropImage(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 11:
            if(THERE_IS_IMAGE){
                merge(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 12:
            if(THERE_IS_IMAGE){
                sunlight_filter(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 13:
            if(THERE_IS_IMAGE){
                infrared_filter(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 14:
            if(THERE_IS_IMAGE){
                purple_filter(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 15:
            if(THERE_IS_IMAGE){
                tv_filter(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 16:
            if(THERE_IS_IMAGE){
                detect_edges(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;
        case 17:
            if(THERE_IS_IMAGE){
                blur_filter(img);
            }
            else{
                std::cout << "Please Load Image first "<<"\n";
            }
            break;

        // case 18:
        //     // undo
        //     undo(img);
        //     break;
        case 18:
            // save Image 
            saving_state(img,outputPath);
            break;
        case 19:
            // exit
            exiting(img,running);
            break;
        default:
            std::cout << "Invalid number"<<"\n";
            break;

        }


    }

}
