#include<iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int stocastic_texture_synthesis(const char* texture_file)
{
    
    
    Mat3b small_texture, patch;
    int rows = 1000;
    int cols = 1000;
    int random_x, random_y;

    small_texture = imread(texture_file, IMREAD_COLOR);
    if ( !small_texture.data )
    {
        printf("No small_texture data \n");
        return -1;
    }
    
    // imshow("Small Texture", small_texture);

    Mat3b large_texture(rows, cols, Vec3b(0,0,0));
    /*
    for(int i=0; i<rows; i+=50){
        for(int j=0; j<cols; j+=50){
            random_x = rand() % (small_texture.size().width-100);
            random_y = rand() % (small_texture.size().height-100);
            patch = small_texture(Range(random_x,random_x+50), Range(random_y,random_y+50));
            patch.copyTo(large_texture(Rect(i, j, patch.cols, patch.rows)));
        }
    }
    //loadImage(large_texture,i);
    */
    vec<int> patches(rows);

    for(int i=0; i<rows; i+=53){
        for(int j=0; j<cols; j+=53){
            random_x = rand() % (small_texture.size().width-100);
            random_y = rand() % (small_texture.size().height-100);
            patch = small_texture(Range(random_x,random_x+54), Range(random_y,random_y+54));
            patches[i]=patch;
            
        }
    }
    for(int i=0;i<rows-1;i+=53){
        for(int j=0;j<cols;j+=53){
            GaussianBlur(cv::add(patches[i], patches[i+1], patch, Size(3,3),0);
            patch.copyTo(large_texture(Rect(i, j, patch.cols, patch.rows)));
        }
    }


    
    if(!large_texture.empty()){
        imshow("Large Texture", large_texture);
    }
     
    
    imwrite("./texture/output.jpg", large_texture);
    waitKey(0);
    destroyAllWindows();
    
    return 0;
}
