/*
 * imtools.h
 *
 *  Created on: November 3, 2013
 *      Author: Siriwat Kasamwattanarote
 */
#pragma once
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>       // setw
#include <iomanip>
#include <sys/time.h>   // time
#include <sys/stat.h>   // file-directory existing
#include <sys/types.h>  // file-directory
#include <dirent.h>     // file-directory
#include <cmath>        // Math
#include <limits>       // limit (for max math limit and etc)

#include "lapwrap.h"
#include "../sifthesaff/SIFThesaff.h"

#include "opencv2/core/core.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

namespace alphautils
{
    namespace imtools
    {
        static const int DRAW_POINT = 0;
        static const int DRAW_AFFINE = 1;
        static const int DRAW_AFFINE_MASK = 2;

        // SIFT Keypoint
        typedef struct _SIFT_Keypoint{ float x; float y; float a; float b; float c; } SIFT_Keypoint;

        // Color space
        const static int RGB_SPACE = 0;
        const static int IRGB_SPACE = 1;
        const static int LAB_SPACE = 2;

        Size get_image_size(const string& img_path);
        void vgg_abc2ellipse(float a, float b, float c, float& degree, float& l1, float& l2);
        float ellipse_abc2degree(float a, float b, float c);
        float ellipse_getlength(float a, float b, float c);
        void draw_sifts(const string& in_img_path, const string& out_img_path, const string& sift_path, int draw_mode, int colorspace = RGB_SPACE, bool normpoint = true, bool rootsift = true, bool binary = true);
        void draw_sifts(Mat& in_img, const string& sift_path, int draw_mode, int colorspace = RGB_SPACE, bool normpoint = true, bool rootsift = true, bool binary = true);
        float draw_a_sift(Mat& in_img, SIFT_Keypoint in_keypoint, int draw_mode, int colorspace = RGB_SPACE, bool normpoint = true, bool rootsift = true, bool binary = true);
        void concatimage(const Mat& img1, const Mat& img2, Mat& img_out);
        void draw_matched(const string& path1, vector<Point2f> vert1, const string& path2, vector<Point2f> vert2, bool normpoint = true);
        void create_mask(const string& out_path, const Size img_size, const Point vert[], const int vert_count);
        void overlay_mask(const string& in_path, const string& out_path, const vector<Point2f>& vert, bool normpoint = true);
        void overlay_point2centroid(const string& in_path, const string& out_path, const Point2f centroid, const vector<Point2f>& point, bool normpoint = true);
        void overlay_rect(const string& in_path, const string& out_path, const Rect box);
    };
};
//;)
