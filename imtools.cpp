/*
 * imtools.cpp
 *
 *  Created on: November 3, 2013
 *      Author: Siriwat Kasamwattanarote
 */

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

        void draw_sifts(const string& in_img_path, const string& out_img_path, const string& sift_path, int draw_mode, int colorspace, bool normpoint, bool rootsift, bool binary)
        {
            Mat in_img = imread(in_img_path.c_str());

            draw_sifts(in_img, sift_path, draw_mode, colorspace, normpoint, rootsift, binary);

            imwrite(out_img_path.c_str(), in_img);
        }

        void draw_sifts(Mat& in_img, const string& sift_path, int draw_mode, int colorspace, bool normpoint, bool rootsift, bool binary)
        {
            SIFThesaff sift_reader(colorspace, normpoint, rootsift);
            sift_reader.importKeypoints(sift_path, binary);
            int num_kp = sift_reader.num_kp;

            float min_degree = 1000;
            float max_degree = -1000;

            // Save degree for analyzing
            float* degree_pack = new float[num_kp];

            for (int kp_idx = 0; kp_idx < num_kp; kp_idx++)
            {
                SIFT_Keypoint curr_kp = {sift_reader.kp[kp_idx][0], sift_reader.kp[kp_idx][1], sift_reader.kp[kp_idx][2], sift_reader.kp[kp_idx][3], sift_reader.kp[kp_idx][4]};

                float raw_degree = draw_a_sift(in_img, curr_kp, draw_mode, colorspace, normpoint, rootsift, binary);

                // Save degree
                degree_pack[kp_idx] = raw_degree;

                if (min_degree > raw_degree)
                    min_degree = raw_degree;
                if (max_degree < raw_degree)
                    max_degree = raw_degree;
            }

            cout << "Min degree = " << min_degree << " Max degree: " << max_degree << endl;

            // Degree analyzing
            size_t* degree_hist;
            float* degree_hist_label;
            size_t degree_hist_size;
            HIST_CONF hist_config = {HIST_RANGEMODE_MANUAL, -1, 0, 360, 1};
            data_to_range_histogram(degree_pack, sift_reader.kp.size(), hist_config, degree_hist, degree_hist_label, degree_hist_size);

            cout << "Degree histogram:" << endl;
            for (size_t degree_hist_id = 0; degree_hist_id < degree_hist_size; degree_hist_id++)
                cout << degree_hist_label[degree_hist_id] << " ";
            cout << endl;
            for (size_t degree_hist_id = 0; degree_hist_id < degree_hist_size; degree_hist_id++)
                cout << degree_hist[degree_hist_id] << " ";
            cout << endl;

            // Release memory
            delete[] degree_pack;
            delete[] degree_hist;
            delete[] degree_hist_label;
        }

        float draw_a_sift(Mat& in_img, SIFT_Keypoint in_keypoint, int draw_mode, int colorspace, bool normpoint, bool rootsift, bool binary)
        {
            float ret_raw_degree = 0.0f;

            Point2f center(in_keypoint.x, in_keypoint.y);             // x, y
            if (normpoint)
            {
                center.x *= in_img.cols;
                center.y *= in_img.rows;
            }

            if (draw_mode == DRAW_POINT)
            {
                // void circle(Mat& in_img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
                circle(in_img, center, 0, Scalar(0, 255, 0), 2, CV_AA);
            }
            else // Affine by ellipse
            {
                /*float raw_degree = ellipse_abc2degree(in_keypoint.a, in_keypoint.b, in_keypoint.c);
                float major_ax = 1/sqrt(in_keypoint.a);
                float minor_ax = 1/sqrt(in_keypoint.c);*/
                float raw_degree, major_ax, minor_ax;
                vgg_abc2ellipse(in_keypoint.a, in_keypoint.b, in_keypoint.c, raw_degree, major_ax, minor_ax);
                ret_raw_degree = raw_degree;
                // Degree and axis correction
//                    if (minor_ax > major_ax)
//                    {
//                        // Degree correction
//                        /*if (in_keypoint.b < 0)
//                            raw_degree += 90;
//                        else
//                            raw_degree -= 90;*/
//
//                        // Axis correction
//                        float tmp = major_ax;
//                        major_ax = minor_ax;
//                        minor_ax = tmp;
//                    }

                Size axes(major_ax, minor_ax);            // major axis, minor axis
                // void ellipse(Mat& in_img, Point center, Size axes, double angle, double startAngle, double endAngle, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
                int rand_b = rand() % 175 + 80;
                int rand_g = rand() % 175 + 80;
                int rand_r = rand() % 175 + 80;

                // Color coded degree

                if (raw_degree <= 90)       // Blue
                {
                    rand_b = 255;
                    rand_g = 0;
                    rand_r = 0;
                }
                else if (raw_degree <= 180) // Green
                {
                    rand_b = 0;
                    rand_g = 255;
                    rand_r = 0;
                }
                else if (raw_degree <= 270) // Red
                {
                    rand_b = 0;
                    rand_g = 0;
                    rand_r = 255;
                }
                else if (raw_degree <= 360) // White
                {
                    rand_b = 255;
                    rand_g = 255;
                    rand_r = 255;
                }

                if (draw_mode == DRAW_AFFINE)
                {
                    if (0)  // Draw transparent
                    {
                        Mat transparent_buffer = Mat(in_img.rows, in_img.cols, CV_8UC3, Scalar(0, 0, 0));
                        ellipse(transparent_buffer, center, axes, raw_degree, 0, 360, Scalar(0, 0, 0), 2, CV_AA);                           // Ellipse shadow
                        ellipse(transparent_buffer, center, axes, raw_degree, 0, 360, Scalar(rand_b, rand_g, rand_r) * 0.5, 1, CV_AA);      // Ellipse line
                        ellipse(transparent_buffer, center, axes, raw_degree, -1, 1, Scalar(0, 0, 255) * 0.5, 3, CV_AA);                    // Ellipse point (direction)
                        in_img += transparent_buffer;
                    }
                    else    // Draw over
                    {
                        ellipse(in_img, center, axes, raw_degree, 0, 360, Scalar(0, 0, 0), 6, CV_AA);                       // Ellipse shadow
                        ellipse(in_img, center, axes, raw_degree, 0, 360, Scalar(rand_b, rand_g, rand_r), 2, CV_AA);        // Ellipse line
                        ellipse(in_img, center, axes, raw_degree, -1, 1, Scalar(0, 0, 255), 3, CV_AA);                      // Ellipse point (direction)
                    }
                }
                else // Affine mask
                    ellipse(in_img, center, axes, raw_degree, 0, 360, Scalar(255, 255, 255), -1, CV_AA);                    // Ellipse mask (filled ellipse)
            }

            return ret_raw_degree;
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

        void overlay_mask(const string& in_path, const string& out_path, const vector<Point2f>& vert, bool normpoint)
        {
            // Load image
            Mat overlay_img = imread(in_path.c_str());

            for (size_t vert_id = 0; vert_id < vert.size(); vert_id++)
            {
                Point2f pstart = vert[vert_id];
                Point2f pend = vert[(vert_id + 1) % vert.size()];
                if (normpoint)
                {
                    pstart.x *= overlay_img.cols;   // Re-scale pstart
                    pstart.y *= overlay_img.rows;
                    pend.x *= overlay_img.cols; // Re-scale pend
                    pend.y *= overlay_img.rows;
                }
                Scalar red(0, 0, 255);
                line(overlay_img, pstart, pend, red, 1, CV_AA);
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
