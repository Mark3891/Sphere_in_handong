#include "packet_sender/e131_sender.h"
#include "image_processor/image_processor.h"
#include "packet_sender/sleep.h"
#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>
#include <unistd.h> // usleep 사용

using namespace std;

int main() {
    std::string video_path = "videotest.mp4";

    cv::VideoCapture cap(video_path);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video file." << std::endl;
        return -1;
    }

    
    
    int rows[54] = {
        104, 126, 141, 157, 172, 187, 199, 211, 224, 235, 246, 255, 263, 272, 279, 286, 293, 299,
        306, 311, 316, 320, 326, 331, 335, 337, 342, 345, 347, 351, 353, 355, 357, 356, 358, 359,
        359, 359, 359, 358, 358, 356, 355, 353, 351, 348, 345, 342, 338, 334, 330, 325, 319, 314
    };

    const char *ip = "192.168.50.72"; 
    cv::Mat frame, frame_rgb;

    cap >> frame;
    if (frame.empty()) {
        std::cerr << "Error: Failed to read the first frame." << std::endl;
        return -1;
    }

    
    ImageProcessor *image = new ImageProcessor(frame.rows, frame.cols, 5, 90, 270, 0, rows, 54);
    E131Sender *sender = new E131Sender(ip);

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // BGR → RGB 변환
        // cv::cvtColor(frame, frame_rgb, cv::COLOR_BGR2RGB);

        // OpenCV 프레임 데이터를 unsigned char* 형식으로 가져오기
        unsigned char *rgb_data = frame.data;

        // LED 패널에 맞게 변환 및 전송
        image->mask(rgb_data);
        image->rotate();
        unsigned char* im = image->get_processed_image();
        
        sender->send(im, 48771);
        sender->next();
        
        // OpenCV 화면 출력
        cv::imshow("Local Video Stream", frame);

       
        if (cv::waitKey(30) == 27) break;
    }
    cap.release();
    cv::destroyAllWindows();
    delete image;
    delete sender;

    return 0;
}
