#include "timer.hpp"
#include <iostream>
#include <omp.h>

#include <fstream>

std::string name;
double		start;
double		end;

void timer::start(const std::string &n)
{
    name = n;
    ::start = get_time();
}
void timer::end()
{
    ::end = get_time();
}
void timer::print()
{
    std::cout << "[" << name << "] : " << ::end - ::start << "(sec), " << (::end - ::start) * 1e+3 << "(msec)" << std::endl;
}
void timer::end_print()
{
    end();
    print();
}

double timer::get_time()
{
    return omp_get_wtime();
}

double timer::get_elapsed_time()
{
    return (::end) - (::start);
}

// stat_time
void stat_timer::clear_time()
{
    times.clear();
}
void stat_timer::add_time(double t)
{
    times.push_back(t);
}
void stat_timer::save_time(const std::string &filename)
{
    std::ofstream out(filename);
    if (out.is_open())
    {
        for (auto it = times.begin(); it != times.end(); it++)
        {
            out << *it;
            out << std::endl;
        }
    }
    out.close();
}
int stat_timer::get_size()
{
    return (int)times.size();
}
void stat_timer::start()
{
    this->tbeg = timer::get_time();
}
void stat_timer::end()
{
    this->tend = timer::get_time();
}
void stat_timer::add_elapsed_time()
{
    add_time(get_elapsed_time());
}
double stat_timer::get_elapsed_time()
{
    return this->tend - this->tbeg;
}