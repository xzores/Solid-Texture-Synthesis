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

    for(int i=0; i<rows; i+=50){
        for(int j=0; j<cols; j+=50){
            random_x = rand() % (small_texture.size().width-100);
            random_y = rand() % (small_texture.size().height-100);
            patch = small_texture(Range(random_x,random_x+50), Range(random_y,random_y+50));
            patch.copyTo(large_texture(Rect(i, j, patch.cols, patch.rows)));
        }
    }

    imshow("Large Texture", large_texture);
    imwrite("./texture/output.jpg", large_texture);
    waitKey(0);
    destroyAllWindows();
    return 0;
}