/*
 * imtools.h
 *
 *  Created on: November 3, 2013
 *      Author: Siriwat Kasamwattanarote
 */
#pragma once

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
        const static int RGB_SPACE = 10;
        const static int IRGB_SPACE = 11;
        const static int LAB_SPACE = 12;

        Size get_image_size(const string& img_path);
        void vgg_abc2ellipse(float a, float b, float c, float& degree, float& l1, float& l2);
        float ellipse_abc2degree(float a, float b, float c);
        float ellipse_getlength(float a, float b, float c);
        void draw_sifts(const string& in_img_path, const string& out_img_path, const string& sift_path, int draw_mode, int colorspace = RGB_SPACE, bool normpoint = true, bool rootsift = true, bool binary = true);
        void draw_sifts(const string& in_img_path, const string& out_img_path, const vector<SIFT_Keypoint>& sift_keypoints, int draw_mode, int colorspace, bool normpoint, bool rootsift);
        void draw_sifts(Mat& in_img, const string& sift_path, int draw_mode, int colorspace = RGB_SPACE, bool normpoint = true, bool rootsift = true, bool binary = true);
        float draw_a_sift(Mat& in_img, SIFT_Keypoint in_keypoint, int draw_mode, bool normpoint = true);
        void concatimage(const Mat& img1, const Mat& img2, Mat& img_out);
        void draw_matched(const string& path1, vector<Point2f> vert1, const string& path2, vector<Point2f> vert2, bool normpoint = true);
        void create_mask(const string& out_path, const Size img_size, const Point vert[], const int vert_count);
        void overlay_mask(const string& in_path, const string& out_path, const string& mask_path);
        void overlay_mask(const string& in_path, const string& out_path, const vector< vector<Point2f> >& polygons, bool normpoint = true);
        void overlay_mask(const string& in_path, const string& out_path, const vector<Point2f>& polygon, bool normpoint = true);
        void overlay_point2centroid(const string& in_path, const string& out_path, const Point2f centroid, const vector<Point2f>& point, bool normpoint = true);
        void overlay_rect(const string& in_path, const string& out_path, const Rect box);
    };
};
//;)
