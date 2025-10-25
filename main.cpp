#include <iostream>
#include <string>
#include <serial/serial.h>
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <chrono>

int main() {

    /*
    cv::VideoCapture cap("C:\\Bad Apple.mp4");
    if (!cap.isOpened()) return -1;

    cv::Mat frame, gray, resized, binary, output;
    
    while (cap.read(frame)) {
        cv::resize(frame, resized, cv::Size(23, 17), cv::INTER_NEAREST);
        cv::threshold(resized, binary, 127, 255, cv::THRESH_BINARY);
        cv::resize(binary, output, cv::Size(230, 170));
        cv::imshow("Video", output);

        bool pixel = output.at<char>(0, 0);
        cv::imshow("Image", cv::Mat(cv::Size(230, 170), CV_8UC1, pixel ? 255 : 0));
   
        for (;;) {
           int input = cv::waitKey(0) & 0xff;
           if (input == 27) return 0;
           if (input == 'q' || input == 'Q') break;
           break;
        }
    }
    */
     

    std::string arduino_port_name;
    auto ports = serial::list_ports();
    for (auto port : ports) { // automaticaly selects the port with arduino
        serial::Serial test_port(port.port, 9600, serial::Timeout::simpleTimeout(250));
        uint8_t send_buffer{6};
        test_port.write(&send_buffer, 1);
        uint8_t receive_buffer{};
        test_port.read(&receive_buffer, 1);
        if (receive_buffer == 7) {
            arduino_port_name = port.port;
        }
    }

    if (arduino_port_name.empty()) return -1;
    serial::Serial arduino_serial(arduino_port_name, 9600, serial::Timeout::simpleTimeout(250));

    std::string send_buffer;
    std::string receive_buffer;
    std::cout << ">> Enter a message: \n";
    std::getline(std::cin, send_buffer);
    arduino_serial.write(send_buffer + '\n');
    receive_buffer = arduino_serial.readline(65536, "\n");
    std::cout << "\n>> Received message: (length: " << receive_buffer.length() << ")\n";
    std::cout << receive_buffer << '\n';
        
    arduino_serial.close();
}
