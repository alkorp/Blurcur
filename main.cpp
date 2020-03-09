#include <iostream>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <opencv2/opencv.hpp>

int g_x, g_y;

cv::Range make_range(int v1, int v2, int min, int max) {
  v1 = std::max(min, v1);
  v2 = std::min(max, v2);
  if (v1 < v2)
    return cv::Range(v1, v2);
  return cv::Range();
}

cv::Mat submatrix(cv::Mat &in, int x, int y, int size) {
  auto colrange = make_range(x - size / 2, x + size / 2, 0, in.cols);
  auto rowrange = make_range(y - size / 2, y + size / 2, 0, in.rows);
  if (!colrange.empty() && !rowrange.empty())
    return in.colRange(colrange).rowRange(rowrange);
  return cv::Mat();
}

void window_callback(int event, int x, int y, int /*flags*/,
                     void * /*userdata*/) {
  if (event == cv::EVENT_MOUSEMOVE) {
    g_x = x;
    g_y = y;
  }
}

int main() {
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini("config.ini", pt);
  auto input = pt.get<std::string>("Basic.Input");
  auto blur_size = pt.get<int>("Basic.BlurSize");
  auto blur_kernel_size = pt.get<int>("Basic.BlurKernelSize");

  cv::VideoCapture cap(input);
  if (cap.isOpened() == false) {
    std::cerr << "Cannot open the video file" << std::endl;
    return -1;
  }
  cv::String window_name = "Blurcur";
  cv::namedWindow(window_name, cv::WINDOW_NORMAL);
  cv::setMouseCallback(window_name, window_callback, NULL);
  double fps = cap.get(CV_CAP_PROP_FPS);

  while (true) {
    cv::Mat frame;
    bool ok = cap.read(frame);
    if (ok == false) {
      std::clog << "Reacked end of the video" << std::endl;
      break;
    }

    cv::Mat sub = submatrix(frame, g_x, g_y, blur_size);
    cv::GaussianBlur(sub, sub, cv::Size(blur_kernel_size, blur_kernel_size), 0);

    cv::imshow(window_name, frame);
    if (cv::waitKey(static_cast<int>(1000/fps)) != -1) break;
  }

  return 0;
}
