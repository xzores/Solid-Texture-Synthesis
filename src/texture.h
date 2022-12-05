#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
int texture_viewer()
{
    Mat image;
    image = imread("./texture/texture_3.jpg", IMREAD_COLOR);
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Texture", WINDOW_AUTOSIZE );
    imshow("Display Texture", image);
    waitKey(0);
    return 0;
}

int stocastic_texture_synthesis()
{
    Mat image;
    image = imread("./texture/texture_3.jpg", IMREAD_COLOR);
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    for (int i = 0; i < 4; i++)
        std::cout << (rand() % 100) + 1 << " \n";
    
    return 0;
}