#ifndef __CMATH_TIMER_H__
#define __CMATH_TIMER_H__

#ifdef _MSC_VER
#pragma once 
#endif

#include <string>
#include <vector>

class timer
{
public:
    static void start(const std::string &n);
    static void end();
    static void print();
    static void end_print();

    static double get_time();

    static double get_elapsed_time();
};

class stat_timer : timer
{
private:
    std::vector<double> times;
    double tbeg;
    double tend;

public:
    void clear_time();
    void add_time(double t);
    void save_time(const std::string &filename);
    int get_size();

    void start();
    void end();
    void add_elapsed_time();
    double get_elapsed_time();
};

#endif