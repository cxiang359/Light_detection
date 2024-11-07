#include <opencv2/opencv.hpp>
#include <iostream>

void detectLights(const cv::Mat& inputImage) {
    cv::Mat grayImage, binaryImage, contoursImage;

    // 将图像转换为灰度图
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

    // 对灰度图进行二值化
    cv::threshold(grayImage, binaryImage, 150, 255, cv::THRESH_BINARY);

    // 轮廓检测
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binaryImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 筛选轮廓并绘制结果
    contoursImage = inputImage.clone();
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Rect boundingRect = cv::boundingRect(contours[i]);
        float aspectRatio = static_cast<float>(boundingRect.width) / boundingRect.height;

        // 筛选长宽比在合理范围内的轮廓
        if (aspectRatio > 0.1 && aspectRatio < 0.4 && boundingRect.area() > 100) {
            // 绘制轮廓
            cv::rectangle(contoursImage, boundingRect, cv::Scalar(0, 255, 0), 2);

            // 判断颜色
            cv::Mat roi = inputImage(boundingRect);
            int sumR = 0, sumB = 0;
            for (int y = 0; y < roi.rows; ++y) {
                for (int x = 0; x < roi.cols; ++x) {
                    cv::Vec3b color = roi.at<cv::Vec3b>(y, x);
                    sumB += color[0];
                    sumR += color[2];
                }
            }
            std::string color = sumR > sumB ? "Red" : "Blue";
            std::cout << "Detected a " << color << " light!" << std::endl;
        }
    }

    // 显示结果
    cv::imshow("Detected Lights", contoursImage);
    cv::waitKey(0);
}

int main() {
    // 读取图像
    cv::Mat inputImage = cv::imread("../image1.png");
    if (inputImage.empty()) {
        std::cerr << "Error: Could not load image!" << std::endl;
        return -1;
    }

    // 检测灯条
    detectLights(inputImage);

    return 0;
}