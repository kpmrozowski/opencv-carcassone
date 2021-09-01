#ifndef DISPLAYER_H
#define DISPLAYER_H
#pragma once

#include <memory>
#include <queue>
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <iostream>
#include "Hough/Point.h"
#include "Hough/Line.h"
#include "Utils/matplotlibcpp.h"
#include "Utils/display.h"

using PointLines = std::vector<std::pair<Point<double>, Point<double>>>;

namespace twm::clusters {

/**
 * @brief Randomly choose a color for a cluster. 
 * For all colors check: https://stackoverflow.com/questions/22408237/named-colors-in-matplotlib 
 * @return a map of "color" and hex
  */
const std::map<std::string, std::string> random_color() {
  std::string hex_char = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  std::string hex{'#'};
  for (int i = 0; i < 6; ++i) { hex += hex_char[rand()%16]; }
  return std::map<std::string, std::string>{ {"color", hex.c_str() } };
}

struct EventBase {
    EventBase() {};
    virtual void Execute() = 0;
    virtual ~EventBase() {};
};
class PrintMessage : public EventBase {
    std::string msg;
  public:
    PrintMessage(std::string str)
        : msg(str) {}
    PrintMessage(std::string str, std::string str2)
        : msg(str + str2) {}
    virtual void Execute() override {
        std::cout << msg << std::endl;
    }
};
class PrintPointLines : public EventBase {
  PointLines pointLines;
  public:
    PrintPointLines(PointLines _pointLines)
        : pointLines(_pointLines) {}
    virtual void Execute() override {
        auto size = pointLines.size();
        std::vector<double> x(size, -1), y(size, -1), x1(size, -1), y1(size, -1), x2(size, -1), y2(size, -1);
        for (int i = 0; i < size; ++i) {
        x1.at(i) = pointLines.at(i).first.x;
        y1.at(i) = pointLines.at(i).first.y;
        x2.at(i) = pointLines.at(i).second.x;
        y2.at(i) = pointLines.at(i).second.y;
        x.at(i) = pointLines.at(i).first.fi / M_PI * 180;
        y.at(i) = pointLines.at(i).second.fi / M_PI * 180;
        }
        matplotlibcpp::scatter(x1, y1, 5.0, random_color());
        matplotlibcpp::scatter(x2, y2, 5.0, random_color());
        matplotlibcpp::show();
        matplotlibcpp::plot(x, y, "r.");
        matplotlibcpp::plot(y, x, "b.");
        matplotlibcpp::show();
    }
};
class DisplayClusteredLines : public EventBase {
  std::vector<std::vector<Line>> clusters_vect;
  public:
    DisplayClusteredLines(std::vector<std::vector<Line>> _clusters_vect)
        : clusters_vect(_clusters_vect) {}
    virtual void Execute() override {
        int i = 0;
        for (const auto& cluster : clusters_vect) {
            auto color = random_color();
            color.insert({"size", "2.0"});
            std::vector<double> x(2), y(2);
            for (int i = 0; i < cluster.size(); ++i) {
                x.at(0) = cluster.at(i).p1().x;
                y.at(0) = cluster.at(i).p1().y;
                x.at(1) = cluster.at(i).p2().x;
                y.at(1) = cluster.at(i).p2().y;
                matplotlibcpp::plot(x, y, color);
            }
        }
        matplotlibcpp::show();
    }
};
class DisplayClusteredVects : public EventBase {
  std::vector<std::vector<std::vector<double>>> clusters_vect;
  public:
    DisplayClusteredVects(std::vector<std::vector<std::vector<double>>> _clusters_vect)
        : clusters_vect(_clusters_vect) {}
    virtual void Execute() override {
        int i = 0;
        for (const auto& cluster : clusters_vect) {
            std::vector<double> x(cluster.size()), y(cluster.size());
            for (int i = 0; i < cluster.size(); ++i) {
                x.at(i) = cluster.at(i).at(0);
                y.at(i) = cluster.at(i).at(1);
            }
            if (i++) {
                matplotlibcpp::scatter(x, y, 5.0, random_color());
            } else {
                matplotlibcpp::scatter(x, y, 5.0, std::map<std::string, std::string>{ {"color", "lightgray"} });
            }
        }
        matplotlibcpp::show();
    }
};
class DisplayImage : public EventBase {
  cv::Mat img;
  std::string wndName;
  bool wait;
  public:
    DisplayImage(std::string _wndName, const cv::Mat& _img, bool _wait = true)
        : img(_img)
        , wndName(_wndName)
        , wait(_wait) {}
    virtual void Execute() override {
        display(wndName, img, wait);
    }
};

template <typename T>
class EventQueue {
    std::queue<std::unique_ptr<T>> _events;
  public:
    mutable std::mutex mut;
    EventQueue() = default;
    EventQueue(const EventQueue& other) {
        std::lock_guard<std::mutex> lock(other.mut);
        _events = other._events;
    }
    // EventQueue& operator=(const EventQueue&) = delete;
    ~EventQueue() = default;
    
    template<class EventType, typename... Args>
    void AddEvent(Args&&... args) {
        printf("Adding...\n");
        // std::lock_guard<std::mutex> lock(mut); 
        _events.emplace(std::make_unique<EventType>(std::forward<Args>(args)...));
        printf("...Added! size=%zu\n", _events.size());
    }
    
    // std::unique_ptr<T> pop() {
    void pop() {
        std::lock_guard<std::mutex> lock(mut);
        // if (_events.empty()) throw Empty_stack{};
        // std::unique_ptr<T> res(std::make_unique<T>(_events.front()));
        _events.pop();
        // return res;
    }
    bool empty() const {
        std::lock_guard<std::mutex> lock(mut);
        return _events.empty();
    }
    bool ExecuteOne() {
        // std::lock_guard<std::mutex> lock(mut);
        // mut.lock();
        bool empty = _events.empty();
        if (!empty) {
            // mut.unlock();
            _events.front()->Execute();
            // mut.lock();
            _events.pop();
            // mut.unlock();
        } else {
            return false;
        }
        return true;
    }
    void ExecuteAll() const {
        // for (auto &event : _events)
        // { event->Execute(); } 
    }
};

} // namespace twm::clusters

#endif // DISPLAYER_H
