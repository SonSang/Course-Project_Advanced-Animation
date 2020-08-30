#include "animation.hpp"

int animation::size() const noexcept {
    return (int)fSequence.size();
}
int animation::length() const noexcept {
    if(fSequence.empty())
        return 0;
    return (int)fSequence.begin()->second.fJointTR.size();
}
bool animation::has_bodypart(const std::string &bodypart) const noexcept {
    return fSequence.find(bodypart) != fSequence.end();
}
void animation::add_single_frame(const std::string &bodypart, const transformation &TR) {
    if(has_bodypart(bodypart))
        get_frame(bodypart).fJointTR.push_back(TR);
    else {
        frame nf;
        nf.fJointTR.push_back(TR);
        fSequence.insert({bodypart, nf});
    }
}
void animation::add_frame(const std::string &bodypart, const std::vector<transformation> &TRs) {
    if(has_bodypart(bodypart)) {
        auto &list = get_frame(bodypart).fJointTR;
        list.insert(list.end(), TRs.begin(), TRs.end());
    }
    else {
        frame nf;
        nf.fJointTR = TRs;
        fSequence.insert({bodypart, nf});
    }
}
const animation::frame& animation::get_frame_c(const std::string &bodypart) const {
    return fSequence.at(bodypart);
}
animation::frame& animation::get_frame(const std::string &bodypart) {
    return fSequence.at(bodypart);
}
void animation::set_frame_id(int id) {
    this->fID = id;
}
int animation::get_frame_id() const noexcept {
    return this->fID;
}
int animation::get_valid_id(const std::string &bodypart, int id) const {
    int size = get_frame_c(bodypart).fJointTR.size();
    if(size == 0)
        throw std::out_of_range(bodypart + std::string(" has no animation info at all."));
    if(id < 0)
        return 0;
    else if(id >= size)
        return size - 1;
    return id;
}
const transformation& animation::get_single_frame(const std::string &bodypart, int id) const {
    return get_frame_c(bodypart).fJointTR.at(id);
}
const transformation& animation::get_valid_single_frame(const std::string &bodypart, int id) const {
    id = get_valid_id(bodypart, id);
    return get_single_frame(bodypart, id);
}
int animation::get_valid_id(const std::string &bodypart) const {
    return get_valid_id(bodypart, fID);
}
const transformation& animation::get_single_frame(const std::string &bodypart) const {
    return get_single_frame(bodypart, fID);
}
const transformation& animation::get_valid_single_frame(const std::string &bodypart) const {
    return get_valid_single_frame(bodypart, fID);
}
animation animation::extract(int begin, int end) const {
    int len = length();
    if(begin < 0 || begin >= len || end < 0 || end >= len || begin > end)
        throw(std::out_of_range("Invalid animation range."));
    
    animation ret;
    for(auto it = fSequence.begin(); it != fSequence.end(); it++) {
        animation::frame nframe;
        auto begit = it->second.fJointTR.begin();
        auto endit = it->second.fJointTR.begin();
        std::advance(begit, begin);
        std::advance(endit, end);
        nframe.fJointTR.insert(nframe.fJointTR.end(), begit, endit);

		ret.fSequence.insert({it->first, nframe});
    }
    return ret;    
}
void animation::increment_fid(int step, bool loop) noexcept {
    int nfid = get_frame_id() + step;
    if(nfid >= length()) {
        if(loop)
            nfid -= length();
        else 
            nfid = length() - 1;
    }
    set_frame_id(nfid);
}