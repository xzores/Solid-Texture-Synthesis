#include <iostream>
#include <stdio.h>
#include <time.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double compare(Mat patch1, Mat patch2)
{
    int height = patch1.size().height;
    int width = patch1.size().width;
    Mat difference;

    subtract(patch1, patch2, difference);
    difference = difference.mul(difference);
    double err = sum(difference)[0];
    double mse = err/(height*width);
    double msre = sqrt(mse);
    // cout << "MSE: " << msre << "\n";

    // imshow("Result Texture", difference);
    // waitKey(0);
    return msre;
}

int stocastic_texture_synthesis(const char* texture_file)
{
    Mat3b original_texture, patch, prev_patch;
    int rows = 1000;
    int cols = 1000;
    int kernel_size = 5;
    int random_x, random_y;
    int neighbour_x, neighbour_y;
    bool flag = false;

    original_texture = imread(texture_file, IMREAD_COLOR);
    if ( !original_texture.data )
    {
        printf("No original_texture data \n");
        return -1;
    }

    // imshow("original Texture", original_texture);
    // cout << "Patch Width  = " << original_texture.size().width << endl << endl;
    // cout << "Patch height = " << original_texture.size().height << endl << endl;

    Mat3b result_texture(rows, cols, Vec3b(0,0,0));
    srand(time(0));

    for(int i=0; i<rows; i+=50){
        for(int j=0; j<cols; j+=50){
            double err = 50;

            neighbour_x = rand() % (original_texture.size().width);
            neighbour_y = rand() % (original_texture.size().height);
            
            int count = 1;
            while(err > 9.3){
                random_x = neighbour_x - 75 + rand() % 150;
                random_y = neighbour_y - 75 + rand() % 150;
                // cout << "Random X = " << random_x << endl << endl;
                // cout << "Random Y = " << random_y << endl << endl;
                if((random_x <= 25 or (random_x >= (original_texture.size().width - 25)))
                    or (random_y <= 25 or (random_y >= (original_texture.size().height - 25)))) continue;
                else{
                    // cout << "Random X = " << random_x << endl << endl;
                    // cout << "Random Y = " << random_y << endl << endl;
                    patch = original_texture(Range(random_y-25,random_y+25), Range(random_x-25,random_x+25));
                    if(flag) err = compare(prev_patch, patch);
                    else err = 0;
                }
                if(count++ % 10){
                    neighbour_x = rand() % (original_texture.size().width);
                    neighbour_y = rand() % (original_texture.size().height);
                } 
            }
                
            patch.copyTo(result_texture(Rect(i, j, patch.cols, patch.rows)));
            imwrite("./texture/output.jpg", result_texture);
            prev_patch = patch;
            flag = true;
        }
    }

    // cout << "Patch Width  = " << patch.size().width << endl << endl;
    // cout << "Patch height = " << patch.size().height << endl << endl;

    imshow("Result Texture", result_texture);
    imwrite("./texture/output.jpg", result_texture);
    waitKey(0);
    destroyAllWindows();
    return 0;
}