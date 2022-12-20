#include <iostream>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double compare(Mat patch1, Mat patch2)
{
    int height = patch1.size().height;
    int width = patch1.size().width;
    Mat difference;

    // Calculate Mean Squared Error between the Patches to measure similarity

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

double match(Mat patch1, Mat patch2) // Histogram Matching Score
{   
    Mat hsv_base, hsv_test1, hsv_test2;
    cvtColor( patch1, hsv_base, COLOR_BGR2HSV );
    cvtColor( patch2, hsv_test2, COLOR_BGR2HSV );

    int h_bins = 50, s_bins = 60;
    int histSize[] = { h_bins, s_bins };
    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };
    const float* ranges[] = { h_ranges, s_ranges };

    // Use the 0-th and 1-st channels
    int channels[] = { 0, 1 };

    Mat hist_base, hist_half_down, hist_test1, hist_test2;

    calcHist( &hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false );
    normalize( hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &hsv_test2, 1, channels, Mat(), hist_test2, 2, histSize, ranges, true, false );
    normalize( hist_test2, hist_test2, 0, 1, NORM_MINMAX, -1, Mat() );

    double base_base = compareHist( hist_base, hist_base, 0);
    double base_test2 = compareHist( hist_base, hist_test2, 0);
    // cout << "1: " << base_base << "\n";
    // cout << "2: " << base_test2 << "\n";
    return base_test2;
}

Mat3b histogram_equalization(Mat patch){
    //Convert the image from BGR to YCrCb color space
    cvtColor(patch, patch, COLOR_BGR2YCrCb);

    //Split the image into 3 channels; Y, Cr and Cb channels respectively and store it in a std::vector
    vector<Mat> vec_channels;
    split(patch, vec_channels); 

    //Equalize the histogram of only the Y channel 
    equalizeHist(vec_channels[0], vec_channels[0]);
    equalizeHist(vec_channels[1], vec_channels[1]);
    equalizeHist(vec_channels[2], vec_channels[2]);

    //Merge 3 channels in the vector to form the color image in YCrCB color space.
    merge(vec_channels, patch); 
        
    //Convert the histogram equalized image from YCrCb to BGR color space again
    cvtColor(patch, patch, COLOR_YCrCb2BGR);

    return patch;
}

int stocastic_texture_synthesis(const char* texture_file)
{

    Mat3b original_texture, patch, prev_patch;

    int rows = 1000;
    int cols = 1000;
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
    vector<Mat3b> final_textures;
    Mat3b result_texture(rows, cols, Vec3b(0,0,0)); // Create Empty Result Texture to load the Patches
    Mat3b temp_texture(rows, cols, Vec3b(0,0,0));
    srand(time(0)); // Make sure to generate new random values each 

    vector<Mat3b> patches(rows); //Store Patches
    int threshold = 15;

    for(int k=0; k<3; k+=1){
        for(int i=0; i<rows; i+=50){
            for(int j=0; j<cols; j+=50){
                double err = 50;

                neighbour_x = rand() % (original_texture.size().width); //Select a random neighbourhood
                neighbour_y = rand() % (original_texture.size().height);
                
                int count = 1;
                while(err > threshold){
                    random_x = neighbour_x - 75 + rand() % 150; //Select a patch from the neighbourhood
                    random_y = neighbour_y - 75 + rand() % 150;
                    if((random_x <= 25 or (random_x >= (original_texture.size().width - 25)))
                        or (random_y <= 25 or (random_y >= (original_texture.size().height - 25)))) continue;
                    else{
                        patch = original_texture(Range(random_y-25,random_y+25), Range(random_x-25,random_x+25));
                        patch.copyTo(temp_texture(Rect(i, j, patch.cols, patch.rows)));
                        Mat temp = temp_texture(Rect(i, j, patch.cols, patch.rows));
                        double score = match(original_texture, temp); // Get Histogram Matching Score
                        if(score < 0.85) {
                            threshold *= 1.1;
                            continue;
                        }
                        if(flag) err = compare(prev_patch, patch);
                        else err = 0;
                    }
                    if(count++ % 10) break;
                }
                // patch = histogram_equalization(patch);
                // if(flag) match(original_texture, patch);
                patch.copyTo(result_texture(Rect(i, j, patch.cols, patch.rows))); // Add patch to final result
                temp_texture = result_texture;
                patches[j]=patch; 
                prev_patch = patch; //Store previous patch for comparision
                flag = true;
            }
        }
        final_textures.push_back(result_texture);
        
    }
    imwrite("./texture/output_xy.jpg", final_textures[0]);
    imwrite("./texture/output_yz.jpg", final_textures[1]);
    imwrite("./texture/output_xz.jpg", final_textures[2]);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
