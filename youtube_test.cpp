
#include "packet_sender/e131_sender.h"
#include "image_processor/image_processor.h"
#include "packet_sender/sleep.h"
#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>
#include <unistd.h> // usle
using namespace std;




int main() {
    std::string video_url = "https://rr3---sn-3u-u5xd.googlevideo.com/videoplayback?expire=1739274594&ei=AuWqZ4anC4qD2roPmKGeaA&ip=203.252.119.110&id=o-AOr1alAGfdPfQ00kIhzSbhs9kDzLRqJl1FG5NmsRg620&itag=400&aitags=133%2C134%2C135%2C136%2C137%2C160%2C242%2C243%2C244%2C247%2C248%2C271%2C278%2C394%2C395%2C396%2C397%2C398%2C399%2C400&source=youtube&requiressl=yes&xpc=EgVo2aDSNQ%3D%3D&met=1739252994%2C&mh=Mv&mm=31%2C29&mn=sn-3u-u5xd%2Csn-3u-bh2se&ms=au%2Crdu&mv=m&mvi=3&pl=23&rms=au%2Cau&pcm2=yes&initcwndbps=3823750&bui=AUWDL3zHnKGRc8vvJE2-o6GdX0DnzDDZqGDcV-ty6FypWELw_lpV8wY99jV6uuhpp-zEoBJiMSRr67uF&spc=RjZbSdJ7veGC4Lz3s7kejuX4c2p34cXFlhHy3HHwjf-zlEJ9Zac9NxqtGHmPSCo&vprv=1&svpuc=1&mime=video%2Fmp4&ns=LiyRbz1x8N216rlPD6EqNaoQ&rqh=1&gir=yes&clen=669024008&dur=2764.461&lmt=1739098299479047&mt=1739252459&fvip=1&keepalive=yes&lmw=1&fexp=51326932%2C51371294&c=TVHTML5&sefc=1&txp=5532534&n=CNAKi7r15LrsiQ&sparams=expire%2Cei%2Cip%2Cid%2Caitags%2Csource%2Crequiressl%2Cxpc%2Cpcm2%2Cbui%2Cspc%2Cvprv%2Csvpuc%2Cmime%2Cns%2Crqh%2Cgir%2Cclen%2Cdur%2Clmt&sig=AJfQdSswRAIgAzqGSeQOI22IwlO3WDDWikTvkvCVz6oNIm916tBNrYACIH_AHRFLNRnRyC_JVG0ybsy-kxXS3ZFWBrMTZPaz9YKw&lsparams=met%2Cmh%2Cmm%2Cmn%2Cms%2Cmv%2Cmvi%2Cpl%2Crms%2Cinitcwndbps&lsig=AGluJ3MwRgIhANjOi58UBiND_aiCt7-RZm9vsf3zbDZYeqA0OGo6hxOXAiEAgrvuKcIXyLkFcX_BFk0axLUqCTcBjQEgf5tjMsl86_8%3D";
    cv::VideoCapture cap(video_url);
    if (!cap.isOpened()) {
        std::cerr << "❌ Error: Could not open video stream." << std::endl;
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

    ImageProcessor *image = new ImageProcessor(frame.rows, frame.cols, 5, 90, 270, 0, rows, 54);
    E131Sender *sender = new E131Sender(ip);

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // 🔹 BGR → RGB 변환
        //cv::cvtColor(frame, frame_rgb, cv::COLOR_BGR2RGB);

        // 🔹 unsigned char* 데이터 변환
        unsigned char *rgb_data = frame.data;

        // 🔹 LED 패널로 데이터 매핑 및 전송
        image->mask(rgb_data);
        image->rotate();
        unsigned char* im = image->get_processed_image();
        
        
        sender->send(image->get_processed_image(), 48771);
        sender->next();
        //usleep(1000000);


        // 🔹 변환된 RGB 프레임을 다시 BGR로 변환하여 화면에 출력
        // cv::cvtColor(frame_rgb, frame, cv::COLOR_RGB2BGR);
        cv::imshow("YouTube Frame", frame);

        // 🔹 ESC 키(27)로 종료
        if (cv::waitKey(30) == 27) break;
    }

    cap.release();
    cv::destroyAllWindows();
    delete image;
    delete sender;

    return 0;
}
