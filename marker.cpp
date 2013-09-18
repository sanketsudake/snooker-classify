// This code will allow you to crop and image by selecting with your mouse.
// press space to exit and "s" to save

// included necessary libraries
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdio>

// define the callback which we will install for mouse events
void my_mouse_callback( int event, int x, int y, int flags, void* param );
void draw_box(IplImage* img, CvRect rect);

// parameters for mouse callback
CvRect box;
bool mouse_down = false;

int main (int argc, const char * argv[])
{
    int saved = 0;
    IplImage* img = cvLoadImage("pos/img1.jpg");
    IplImage* imgDrawn = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);

    // create necessary window to display video (windows are created throughout the file as they become necessary
    cvNamedWindow("window");

    // install the mouse callback.
    box = cvRect(-1,-1,0,0);
    cvSetMouseCallback("window", my_mouse_callback, img);

    while(1)
    {
        // allows us to draw a rectangle over the image with our mouse
        cvCopy(img,imgDrawn);

        draw_box(imgDrawn, box );
//        if (mouse_down){
//
//        }

        // shows the image with the rectangle over it
        cvShowImage("window", imgDrawn);

        // if you press "s", it will set the region of interest to the boxed area, and save a file to your desktop, otherwise you will stay in the while loop and continue to draw rectangles with your mouse
//        char s = cvWaitKey(10);

        // if you press "s" the box will be cropped and saved
        char s = cvWaitKey(10);
        if ( s == 115)
        {
            cvSetImageROI(img, box);

            char *name = new char[100];
            sprintf(name, "pos/cimg1.jpg", saved);
            cvSaveImage(name, img);
            cvResetImageROI(img);
            saved += 1;
            printf("saved");
        }
        // press the space bar to exit
        if (s == 32){
            cvReleaseImage(&img);
            cvReleaseImage(&imgDrawn);
            cvDestroyWindow("window");
            return 0;
        }

    }
}



// mouse callback (used to determine area of image to crop)
void my_mouse_callback( int event, int x, int y, int flags, void* param ){
//    printf("mouse call back: %d\ndrawing?: %d\n", event, drawing_box);

    switch( event ){
        case CV_EVENT_MOUSEMOVE:
            if( mouse_down ){
                box.width = x-box.x;
                box.height = y-box.y;
            }
            break;

        case CV_EVENT_LBUTTONDOWN:
            mouse_down = true;
            box = cvRect( x, y, 0, 0 );
            break;

        case CV_EVENT_LBUTTONUP:
            mouse_down = false;
            if( box.width < 0 ){
                box.x += box.width;
                box.width *= -1;
            }
            if( box.height < 0 ){
                box.y += box.height;
                box.height *= -1;
            }
            break;
    }
}

// helper function to draw a box
void draw_box( IplImage* img, CvRect rect ){
    cvRectangle( img, cvPoint(box.x, box.y), cvPoint(box.x+box.width,box.y+box.height),
                cvScalar(0xff,0x00,0x00) );
}
