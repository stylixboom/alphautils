/*
 * imtools.cpp
 *
 *  Created on: November 3, 2013
 *      Author: Siriwat Kasamwattanarote
 */
#include <ctime>
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

#include "opencv2/core/core.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// Siriwat's header
#include "lapwrap.h"
#include "alphautils.h"
//#include "../sifthesaff/SIFThesaff.h"
//#include "../orb/orb.h"

#include "imtools.h"

using namespace std;
using namespace cv;

namespace alphautils
{
    namespace imtools
    {
        Size get_image_size(const string& img_path)
        {
            return imread(img_path).size();
        }

		// http://www.robots.ox.ac.uk/~vgg/research/affine/detectors.html
		void vgg_abc2ellipse(float a, float b, float c, float& degree, float& l1, float& l2)
		{
			// degree is in degree
			// l1 is major axis
			// l2 is minor axis

			// original code display_features.m
			// display_features.m
			/*
			[v e]=eig(Mi);

			l1=1/sqrt(e(1));

			l2=1/sqrt(e(4));

			alpha=atan2(v(4),v(3));
			s=1;
			t = 0:pi/50:2*pi;
			y=s*(l2*sin(t));
			x=s*(l1*cos(t));

			xbar=(x*cos(alpha) + y*sin(alpha)) / 1;
			ybar=(y*cos(alpha) - x*sin(alpha)) / 1;
			plot(ybar+i,xbar+j,'-k','LineWidth',3+size);
			plot(ybar+i,xbar+j,col,'LineWidth',1+size);
			*/

			double rad;                     // radian
			double a_eivec[4] = { a, b,
								  b, c  };
			double* eival = new double[2];

			lap_eig(a_eivec, eival, 2);

			rad = atan2(a_eivec[3], a_eivec[2]);            // degree in radian
			degree = rad * 180 / PI + 180;                  // Convert radian to degree (with offset 180 degree)
			l1 = 1 / sqrt(eival[1]);                        // Major axis
			l2 = 1 / sqrt(eival[0]);                        // Minor axis

			// Release memory
			delete[] eival;
		}

        // atan(b/(a-c))/2 is radian
        // atan(b/(a-c))/2 * 180 / PI is degree
        // http://math.stackexchange.com/questions/280937/finding-the-angle-of-rotation-of-an-ellipse-from-its-general-equation-and-the-ot
        float ellipse_abc2degree(float a, float b, float c)
        {
            float raw_degree = 0.0f;
            //raw_degree = (0.5f * atan(b / (a - c)) + PI) * 180 / PI;                      // Boom's new --> https://www.wolframalpha.com/input/?i=tan%282x%29%3Db%2F%28a-c%29
            raw_degree = atan(b / (a - c)) / 2 * 180 / PI;                                  // Boom's old
            //raw_degree = atan((c - a + sqrt((c - a) * (c - a) + b * b)) / b) * 180 / PI;  // jack


            float major_ax = 1/sqrt(a);
            float minor_ax = 1/sqrt(c);
            // Degree and axis correction
            if (minor_ax > major_ax)
            {
                // Degree correction
                if (b < 0)
                    raw_degree += 90;
                else
                    raw_degree -= 90;

                // Axis correction
                /*float tmp = major_ax;
                major_ax = minor_ax;
                minor_ax = tmp;*/
            }

            // Range between 0-360
            /*if ((int)raw_degree <= -1)
                raw_degree += 360;*/

            return raw_degree;
        }

        float ellipse_getlength(float a, float b, float c)
        {
            float major_ax = 1/sqrt(a);
            float minor_ax = 1/sqrt(c);

            if (major_ax > minor_ax)
                return major_ax;
            else
                return minor_ax;
        }
		
        // http://stackoverflow.com/questions/13133055/opencv-displaying-2-images-adjacently-in-the-same-window
        void concatimage(const Mat& img1, const Mat& img2, Mat& img_out)
        {
            Size sz1 = img1.size();
            Size sz2 = img2.size();
            int out_height = sz1.height;
            if (sz2.height > out_height)
                out_height = sz2.height;
            img_out = Mat(out_height, sz1.width + sz2.width, CV_8UC3);
            Mat left(img_out, Rect(0, 0, sz1.width, sz1.height));
            img1.copyTo(left);
            Mat right(img_out, Rect(sz1.width, 0, sz2.width, sz2.height));
            img2.copyTo(right);
        }

        void draw_matched(const string& img1_path, const string& img2_path, const string& out_path,  const vector<Point2f>& vert1, const vector<Point2f>& vert2, bool normpoint)
        {
            // Draw match for tentative
            Mat img_out;

            // Load image
            Mat img1 = imread(img1_path.c_str());
            Mat img2 = imread(img2_path.c_str());

            // Concatinate image
            concatimage(img1, img2, img_out);

            // Draw line
            for (size_t point_idx = 0; point_idx < vert1.size(); point_idx++)
            {
                Point2f pstart = vert1[point_idx];
                Point2f pend = vert2[point_idx];
                if (normpoint)
                {
                    pstart.x *= img1.cols;   // Re-scale pstart
                    pstart.y *= img1.rows;
                    pend.x *= img2.cols; // Re-scale pend
                    pend.y *= img2.rows;
                    pend.x += img1.cols; // Shift end Offset
                }
                line(img_out, pstart, pend, Scalar(0, 255, 0), 1, CV_AA);
            }

            imwrite(out_path, img_out);
        }

        void create_mask(const string& out_path, const Size img_size, const Point vert[], const int vert_count)
        {
            // Prepare out image
            Mat img_out(img_size.height, img_size.width, CV_8UC1, Scalar(0)); // Gray scale image

            // Color as mask weight
            Scalar white(255);

            // Draw
            fillConvexPoly(img_out, vert, vert_count, white);

            // Write file
            imwrite(out_path, img_out);
        }

        void overlay_mask(const string& in_path, const string& out_path, const string& mask_path)
        {
            // Load image
            Mat overlay_img = imread(in_path.c_str());
            Mat mask_img = imread(mask_path.c_str(), 0);

            // Convert finding edge of this mask
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;

            Scalar red(0, 0, 255);

            try
            {
                Canny( mask_img, mask_img, 10, 30, 3 );
                findContours( mask_img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
                for( int idx = 0; idx >= 0; idx = hierarchy[idx][0] )
                {
                    //Scalar color( rand()&255, rand()&255, rand()&255 );
                    drawContours( overlay_img, contours, idx, red, 2, 8, hierarchy );
                }
            }
            catch (...)
            {
                throw;
            }

            // Write file
            imwrite(out_path, overlay_img);
        }

        void overlay_mask(const string& in_path, const string& out_path, const vector< vector<Point2f> >& polygons, bool normpoint)
        {
            for (size_t polygon_idx = 0; polygon_idx < polygons.size(); polygon_idx++)
                overlay_mask(in_path, out_path, polygons[polygon_idx], normpoint);
        }

        void overlay_mask(const string& in_path, const string& out_path, const vector<Point2f>& polygon, bool normpoint)
        {
            // Load image
            Mat overlay_img = imread(in_path.c_str());

            for (size_t polygon_idx = 0; polygon_idx < polygon.size(); polygon_idx++)
            {
                Point2f pstart = polygon[polygon_idx];
                Point2f pend = polygon[(polygon_idx + 1) % polygon.size()];
                // better not to scale (same as ins_online.cpp oxMaskExport)
                /*if (normpoint)
                {
                    pstart.x *= overlay_img.cols;   // Re-scale pstart
                    pstart.y *= overlay_img.rows;
                    pend.x *= overlay_img.cols; // Re-scale pend
                    pend.y *= overlay_img.rows;
                }*/
                Scalar red(0, 0, 255);
                line(overlay_img, pstart, pend, red, 2, CV_AA);
            }

            // Write file
            imwrite(out_path, overlay_img);
        }

        void overlay_point2centroid(const string& in_path, const string& out_path, const Point2f centroid, const vector<Point2f>& point, bool normpoint)
        {
            // Load image
            Mat overlay_img;
            if (is_path_exist(out_path))
                overlay_img = imread(out_path.c_str());
            else
                overlay_img = imread(in_path.c_str());

            Scalar blue(255, 0, 0);
            Scalar green(0, 255, 0);
            Scalar red(0, 0, 255);
            Scalar yellow(0, 255, 255);

            // Draw Point with line
            for (size_t point_id = 0; point_id < point.size(); point_id++)
            {
                Point2f pstart = centroid;
                Point2f pend = point[point_id];
                if (normpoint)
                {
                    pstart.x *= overlay_img.cols;   // Re-scale pstart
                    pstart.y *= overlay_img.rows;
                    pend.x *= overlay_img.cols; // Re-scale pend
                    pend.y *= overlay_img.rows;
                }
                line(overlay_img, pstart, pend, red, 1, CV_AA);

                // Draw point
                circle(overlay_img, pend, 0, green, 2, CV_AA);
            }

            Point2f pcent = centroid;
            if (normpoint)
            {
                pcent.x *= overlay_img.cols;
                pcent.y *= overlay_img.rows;
            }
            // Draw Center
            circle(overlay_img, pcent, 1, blue, 2, CV_AA);

            // Draw polygon from convex point
            vector<Point2f> convex_point;
            // Find convex hull
            convexHull(point, convex_point);
            // Draw convex hull
            for (size_t convex_point_id = 0; convex_point_id < convex_point.size(); convex_point_id++)
            {
                Point2f pstart = convex_point[convex_point_id];
                Point2f pend = convex_point[(convex_point_id + 1) % convex_point.size()];
                if (normpoint)
                {
                    pstart.x *= overlay_img.cols;   // Re-scale pstart
                    pstart.y *= overlay_img.rows;
                    pend.x *= overlay_img.cols; // Re-scale pend
                    pend.y *= overlay_img.rows;
                }
                line(overlay_img, pstart, pend, yellow, 1, CV_AA);
            }

            // Write file
            imwrite(out_path, overlay_img);
        }

        void overlay_rect(const string& in_path, const string& out_path, const Rect box)
        {
            // Load image
            Mat overlay_img;
            if (is_path_exist(out_path))
                overlay_img = imread(out_path.c_str());
            else
                overlay_img = imread(in_path.c_str());

            Scalar green(0, 255, 0);

            // Draw rectangle
            rectangle(overlay_img, box, green, 2, CV_AA);

            // Write file
            imwrite(out_path, overlay_img);
        }
    };
};
//;)
