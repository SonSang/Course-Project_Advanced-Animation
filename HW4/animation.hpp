#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "math/transformation.hpp"
#include <vector>
#include <map>

class animation {
public:
    // Each of body part has its own [ frame ] structure.
    class frame {
    public:
        std::vector<transformation> fJointTR;   // Joint transformation for each frame.
    };
    std::map<std::string, frame> fSequence;     // Frame sequence.
    int fID = -1;                               // Current frame id.

    int size() const noexcept;
    int length() const noexcept;
    bool has_bodypart(const std::string &bodypart) const noexcept;
    
    void add_single_frame(const std::string &bodypart, const transformation &TR);
    void add_frame(const std::string &bodypart, const std::vector<transformation> &TRs);

    const frame& get_frame_c(const std::string &bodypart) const;
    frame& get_frame(const std::string &bodypart);

    void set_frame_id(int id);
    int get_frame_id() const noexcept;

    int get_valid_id(const std::string &bodypart, int id) const;
    const transformation& get_single_frame(const std::string &bodypart, int id) const;
    const transformation& get_valid_single_frame(const std::string &bodypart, int id) const;   // Even if [id] is out of boundary, set it to nearest valid id and return.    

    // Use [fID] for frame id.
    int get_valid_id(const std::string &bodypart) const;
    const transformation& get_single_frame(const std::string &bodypart) const;
    const transformation& get_valid_single_frame(const std::string &bodypart) const;

    // Extract sub animation.
    animation extract(int begin, int end) const;

    // Increment [ fID ] with given amount of step.
    // @loop :  If the result is larger than animation length, set [ fID ] to ( fID - length ).
    //          Else, just make [ fID ] to be the last frame.
    void increment_fid(int step, bool loop) noexcept;
};

#endif