#include <iostream>
#include <string>
#include <serial/serial.h>
#include <opencv2/opencv.hpp>

int main() {
 


    cv::VideoCapture cap("C:/Users/CSAP/Downloads/Bad Apple.mp4");
    cv::Mat frame, gray, resized, binary, output;
    
    while (cap.read(frame)) {
        cv::resize(frame, resized, cv::Size(23, 17));
        cv::cvtColor(resized, gray, cv::COLOR_BGR2GRAY);
        cv::threshold(gray, binary, 127, 255, cv::THRESH_BINARY);
        cv::resize(binary, output, cv::Size(690, 510), cv::INTER_NEAREST);
        cv::imshow("First Frame", output);
        for (;;) {
           int input = cv::waitKey(0) & 0xff;
           if (input == 27) return 0;
           if (input == 'q' || input == 'Q') break;
        }
    }

    /*
    std::cout << "[ports]\n";
    auto ports = serial::list_ports();
    for (auto port : ports) {
        std::cout << port.port << '\n';
    }
    serial::Serial arduino_serial("COM4", 9600, serial::Timeout::simpleTimeout(250));

    std::string send_buffer;
    std::string receive_buffer;
    std::cout << ">> Enter a message: \n";
    std::getline(std::cin, send_buffer);
    arduino_serial.write(send_buffer + '\n');
    receive_buffer = arduino_serial.readline(65536, "\n");
    std::cout << "\n>> Received message: (length: " << receive_buffer.length() << ")\n";
    std::cout << receive_buffer << '\n';

    arduino_serial.close();
    */
}
