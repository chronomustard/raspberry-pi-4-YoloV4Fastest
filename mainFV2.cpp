// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

//modified 12-31-2021 Q-engineering

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "yolo-fastestv2.h"
#include <cstdio>
#include <ctime>
#include <fstream>

yoloFastestv2 yoloF2;

using namespace std;

const char* class_names[] = {
    "background", "person", "bicycle",
    "car", "motorbike", "aeroplane", "bus", "train", "truck",
    "boat", "traffic light", "fire hydrant", "stop sign",
    "parking meter", "bench", "bird", "cat", "dog", "horse",
    "sheep", "cow", "elephant", "bear", "zebra", "giraffe",
    "backpack", "umbrella", "handbag", "tie", "suitcase",
    "frisbee", "skis", "snowboard", "sports ball", "kite",
    "baseball bat", "baseball glove", "skateboard", "surfboard",
    "tennis racket", "bottle", "wine glass", "cup", "fork",
    "knife", "spoon", "bowl", "banana", "apple", "sandwich",
    "orange", "broccoli", "carrot", "hot dog", "pizza", "donut",
    "cake", "chair", "sofa", "pottedplant", "bed", "diningtable",
    "toilet", "tvmonitor", "laptop", "mouse", "remote", "keyboard",
    "cell phone", "microwave", "oven", "toaster", "sink",
    "refrigerator", "book", "clock", "vase", "scissors",
    "teddy bear", "hair drier", "toothbrush"
}; // Here are the classes that have been pretrained

int humans = 0; // Transfered Data
float capacity = 0.0; // Transfered Data
int delay = 0;
const int full_capacity = 36; // Full capacity of the class

static void draw_objects(cv::Mat& cvImg, const std::vector<TargetBox>& boxes)
{
    humans = 0;
    for(size_t i = 0; i < boxes.size(); i++) {
  //      std::cout<<boxes[i].x1<<" "<<boxes[i].y1<<" "<<boxes[i].x2<<" "<<boxes[i].y2
  //               <<" "<<boxes[i].score<<" "<<boxes[i].cate<<std::endl; // Use this if needed for debugging

        if(boxes[i].cate == 0){ // Category for human class index is 0
            humans ++; // For each box made for human, human count add 1
            std::cout << humans << std::endl; // Used for debugging to show human number
        }

        char text[256];
        sprintf(text, "%s %.1f%%", class_names[boxes[i].cate+1], boxes[i].score * 100); // Print the category and confidence score on the box

        int baseLine = 0;
        cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

        int x = boxes[i].x1;
        int y = boxes[i].y1 - label_size.height - baseLine;
        if (y < 0) y = 0;
        if (x + label_size.width > cvImg.cols) x = cvImg.cols - label_size.width;

        cv::rectangle(cvImg, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),
                      cv::Scalar(255, 255, 255), -1); // Set box for class label

        cv::putText(cvImg, text, cv::Point(x, y + label_size.height),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0)); // Set the sprintf text into the box with font

        cv::rectangle (cvImg, cv::Point(boxes[i].x1, boxes[i].y1),
                       cv::Point(boxes[i].x2, boxes[i].y2), cv::Scalar(255,0,0)); // Put the big box in the object
    }
}

int main(int argc, char** argv)
{
    float f;
    delay = 15 * CLOCKS_PER_SEC;
    clock_t now = clock();
    float FPS[16];
    int i,Fcnt=0;
    cv::Mat frame;
    //some timing
    std::chrono::steady_clock::time_point Tbegin, Tend;

    for(i=0;i<16;i++) FPS[i]=0.0;

    yoloF2.init(false); //we have no GPU in raspberri Pi 4

    yoloF2.loadModel("yolo-fastestv2-opt.param","yolo-fastestv2-opt.bin"); // Load the pretrained model and parameter

    cv::VideoCapture cap(0); // Connect to the camera, else change the 0 into video name. ("oliveganteng.mp4")

    if (!cap.isOpened()) {
        std::cerr << "ERROR: Unable to open the camera" << std::endl;
        return 0;
    }

    std::cout << "Start grabbing, press ESC on Live window to terminate" << std::endl;
	while(1){
   //     frame=cv::imread("parking.jpg");  //need to refresh frame before dnn class detection
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "ERROR: Unable to grab from the camera" << std::endl;
            break;
        }

        Tbegin = std::chrono::steady_clock::now();

        std::vector<TargetBox> boxes;
        yoloF2.detection(frame, boxes); // Object detection and categorization is in this function (from yolo-fastestv2.cpp)
        draw_objects(frame, boxes); // Draw the box and the category
        Tend = std::chrono::steady_clock::now();

        //calculate frame rate
        f = std::chrono::duration_cast <std::chrono::milliseconds> (Tend - Tbegin).count();
        if(f>0.0) FPS[((Fcnt++)&0x0F)]=1000.0/f;
        for(f=0.0, i=0;i<16;i++){ f+=FPS[i]; }
        putText(frame, cv::format("FPS %0.2f", f/16),cv::Point(10,20),cv::FONT_HERSHEY_SIMPLEX,0.6, cv::Scalar(0, 0, 255));

        //Change this
        std::cout << "how many human: " << humans << std::endl;
        std::cout << "delay: " << delay << std::endl;
        std::cout << "dt: " << clock() - now << std::endl;
        std::cout << "capacity: " << humans*100/full_capacity-1 << "%" << std::endl;

        //Delay send
        if (clock() - now > delay){
            std::cout << "Delayed print" << std::endl;
            ofstream MyFile("humans.txt");
            MyFile << humans - 1;
            ofstream MyFile1("capacity.txt");
            MyFile1 << humans*100 / full_capacity-1 ;

            MyFile.close();
            MyFile1.close();

            now = clock();
        }

        //show outputstd::cerr << "ERROR: Unable to grab from the camera" << std::endl;
        cv::imshow("Jetson Nano",frame);
        //cv::imwrite("test.jpg",frame);
        char esc = cv::waitKey(5);
        if(esc == 27) break;
	}
    return 0;
}
