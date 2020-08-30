#include "bvh_parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

// check if [filename] is valid.
void check_valid_filename(const std::string &filename)
{
    std::ifstream
        ifs(filename);
    if (!ifs.is_open())
    {
        
        throw std::invalid_argument(filename);
    }
    ifs.close();
}

// Reference : https://modoocode.com/215
std::string read_text_file(const std::string &filename) {
    check_valid_filename(filename);

    std::ifstream
        ifs(filename);
    std::string
        str;
    ifs.seekg(0, std::ios::end);
    auto
        size = ifs.tellg();
    str.resize(size);
    ifs.seekg(0, std::ios::beg);
    ifs.read(&str[0], size);
    ifs.close();
    return str;
}
std::vector<std::string> read_text_file_multiline(const std::string &filename)
{
    check_valid_filename(filename);
    std::ifstream
        ifs(filename);
    std::vector<std::string>
        ret;
    while (!ifs.eof())
    {
        std::string
            line;
        std::getline(ifs, line);
        ret.push_back(line);
    }
    ifs.close();
    return ret;
}
std::vector<std::string> tokenize(const std::string &line, const char delimiter) {
    std::vector<std::string> words;
    std::string word;
    std::stringstream ss(line);

    while (std::getline(ss, word, delimiter))
    {
        words.push_back(word);
    }
    return words;
}
std::vector<std::string> tokenize(const std::string &line, const std::vector<char> delimiters) {
    std::vector<std::string> ret{line}, tmpret;
    for(auto c = delimiters.begin(); c != delimiters.end(); c++) {
        tmpret = ret;
        ret.clear();
        for(auto l = tmpret.begin(); l != tmpret.end(); l++) {
            auto ls = tokenize(*l, *c);
            for(auto lss = ls.begin(); lss != ls.end(); lss++) {
                if(*lss != std::string(""))
                    ret.push_back(*lss);
            }
                
        }
    }
    return ret;
}
body bvh_parser::parse(const std::string &filename) {
    // used for mapping axis angle to certain node.
    struct node_axis {
        body::node *nodeptr = nullptr;    // if nullptr, [position] of entire body.
        int axis;   // 0 for X, 1 for Y, 2 for Z.
    };

    body ret;
    std::vector<int> node_id_stack;
    std::vector<body::node *> node_stack;

    int angle_map_index = 0;
    std::map<int, node_axis> angle_map; 

    bool end_flag = false;
    bool motion_section = false;

    std::vector<std::string> lines;
    lines = read_text_file_multiline(filename);

    std::vector<char> delimiters = {'\r', '\t', ' '};
    for(auto it = lines.begin(); it != lines.end(); it++) {
        std::string line = *it;

        std::vector<std::string> words = tokenize(line, delimiters);

        std::string det = *words.begin();
        
        // hierarchy.
        if(!motion_section) {
            if (det == std::string("HIERARCHY"))
                continue;
            else if (det == std::string("ROOT"))
            {
                node_stack.push_back(&ret.get_node(ret.get_root()));
                node_id_stack.push_back(ret.get_root());

                box nb;
                nb.set_xmin(-1); nb.set_xmax(1);
                nb.set_ymin(-1); nb.set_ymax(1);
                nb.set_zmin(-1); nb.set_zmax(1);
                ret.get_node(ret.get_root()).set_bodypart(nb);

                ret.get_node(ret.get_root()).set_name(words.at(1));
            }
            else if (det == std::string("JOINT"))
            {
                body::node n;
                std::shared_ptr<joint_ballsocket> jointptr = std::make_shared<joint_ballsocket>();
                std::shared_ptr<link> linkptr = std::make_shared<link>();
                n.set_name(words.at(1));
                n.set_joint(jointptr);
                n.set_joint_type(1);    // default : ball-and-socket joint.
                n.set_link(linkptr);
                n.set_parent(node_id_stack.back());

                box nb;
                nb.set_xmin(-1); nb.set_xmax(1);
                nb.set_ymin(-1); nb.set_ymax(1);
                nb.set_zmin(-1); nb.set_zmax(1);
                n.set_bodypart(nb);

                int nid = ret.add_node(node_id_stack.back(), n);
                node_stack.push_back(&ret.get_node(nid));
                node_id_stack.push_back(nid);
            }
            else if (det == std::string("{"))
            {
            }
            else if (det == std::string("}"))
            {
                if (end_flag)
                    end_flag = false;
                else
                {
                    node_stack.pop_back();
                    node_id_stack.pop_back();
                }
            }
            else if (det == std::string("End"))
            {
                end_flag = true;
            }
            else if (det == std::string("OFFSET"))
            {
                double x, y, z;
                x = std::stod(words.at(1));
                y = std::stod(words.at(2));
                z = std::stod(words.at(3));
                if (end_flag)
                {
                    // end site offset configuration.
                    auto parent = node_stack.back();
                    box nb = parent->get_bodypart();

                    if (x < nb.get_xmin())
                        nb.set_xmin(x);
                    if (x > nb.get_xmax())
                        nb.set_xmax(x);
                    if (y < nb.get_ymin())
                        nb.set_ymin(y);
                    if (y > nb.get_ymax())
                        nb.set_ymax(y);
                    if (z < nb.get_zmin())
                        nb.set_zmin(z);
                    if (z > nb.get_zmax())
                        nb.set_zmax(z);

                    parent->set_bodypart(nb);
                }
                else
                {
                    node_stack.back()->get_link()->translate(x, y, z);

                    // set box configuration of parent node.
                    if(node_stack.size() > 1) {
                        auto parent = node_stack.at(node_stack.size() - 2);
                        box nb = parent->get_bodypart();

                        if(x < nb.get_xmin()) nb.set_xmin(x);
                        if(x > nb.get_xmax()) nb.set_xmax(x);
                        if(y < nb.get_ymin()) nb.set_ymin(y);
                        if(y > nb.get_ymax()) nb.set_ymax(y);
                        if(z < nb.get_zmin()) nb.set_zmin(z);
                        if(z > nb.get_zmax()) nb.set_zmax(z);

                        parent->set_bodypart(nb);
                    }
                }
            }
            else if (det == std::string("CHANNELS"))
            {
                for (auto word = words.begin(); word != words.end(); word++)
                {
                    node_axis na;
                    if (*word == std::string("Xposition"))
                    {
                        na.nodeptr = nullptr;
                        na.axis = 0;
                        angle_map.insert(std::pair<int, node_axis>(angle_map_index++, na));
                    }
                    else if (*word == std::string("Yposition"))
                    {
                        na.nodeptr = nullptr;
                        na.axis = 1;
                        angle_map.insert(std::pair<int, node_axis>(angle_map_index++, na));
                    }
                    else if (*word == std::string("Zposition"))
                    {
                        na.nodeptr = nullptr;
                        na.axis = 2;
                        angle_map.insert(std::pair<int, node_axis>(angle_map_index++, na));
                    }
                    else if (*word == std::string("Xrotation"))
                    {
                        na.nodeptr = node_stack.back();
                        na.axis = 0;
                        angle_map.insert(std::pair<int, node_axis>(angle_map_index++, na));
                    }
                    else if (*word == std::string("Yrotation"))
                    {
                        na.nodeptr = node_stack.back();
                        na.axis = 1;
                        angle_map.insert(std::pair<int, node_axis>(angle_map_index++, na));
                    }
                    else if (*word == std::string("Zrotation"))
                    {
                        na.nodeptr = node_stack.back();
                        na.axis = 2;
                        angle_map.insert(std::pair<int, node_axis>(angle_map_index++, na));
                    }
                }
            }
            else if(det == std::string("MOTION"))
                motion_section = true;
        }
        // motion.
        else {
            if(det == std::string("Frame")) {   // frame time
                double frame_time = std::stod(words.at(2));
                ret.set_frame_time(frame_time);
            }
            else if(det == std::string("Frames")) {
                // frame number.
            }
            else if(words.size() == angle_map.size()) { // motion data.
                int size = (int)words.size();
                vec3d anim_pos;
                for(int i = 0; i < size;) {
                    double val0 = std::stod(words.at(i));
                    double val1 = std::stod(words.at(i + 1));
                    double val2 = std::stod(words.at(i + 2));
                    const auto na = angle_map.at(i);
                    if(na.nodeptr == nullptr) { // position.
                        vec3d anim_pos(val0, val1, val2);
                        ret.get_anim_pos().push_back(anim_pos);
                    }
                    else {  // rotation.
                        // change to radian angle.
                        val0 = (val0 / 180.0) * 3.141592;
                        val1 = (val1 / 180.0) * 3.141592;
                        val2 = (val2 / 180.0) * 3.141592;

                        transformation jointTR;

                        if(angle_map.at(i + 2).axis == 0) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(1, 0, 0), val2);
                        } 
                        else if(angle_map.at(i + 2).axis == 1) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(0, 1, 0), val2);
                        }
                        else if(angle_map.at(i + 2).axis == 2) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(0, 0, 1), val2);
                        }

                        if(angle_map.at(i + 1).axis == 0) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(1, 0, 0), val1);
                        } 
                        else if(angle_map.at(i + 1).axis == 1) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(0, 1, 0), val1);
                        }
                        else if(angle_map.at(i + 1).axis == 2) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(0, 0, 1), val1);
                        }

                        if(angle_map.at(i).axis == 0) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(1, 0, 0), val0);
                        } 
                        else if(angle_map.at(i).axis == 1) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(0, 1, 0), val0);
                        }
                        else if(angle_map.at(i).axis == 2) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(0, 0, 1), val0);
                        }
                        na.nodeptr->get_anim().push_back(jointTR);
                    }
                    i = i + 3;
                }
            }
        }
    }
    std::cout<<"Anim size : "<<ret.get_node_c(1).get_anim_c().size()<<std::endl;
    return ret;
}
animation bvh_parser::parse_anim(const std::string &filename) {
    body b = parse(filename);
    
    animation anim;
    
    // Root position.
    for(auto it = b.get_anim_pos_c().begin(); it != b.get_anim_pos_c().end(); it++) {
        transformation TR;
        TR.translate(it->get(0), it->get(1), it->get(2));
        anim.add_single_frame("Root", TR);
    }

    // Joint transformations.
    for(auto it = b.get_tree().begin(); it != b.get_tree().end(); it++) {
        auto name = it->second.get_name();
        anim.add_frame(name, it->second.get_anim());
    }

    return anim;
}
live_body bvh_parser::parse_live_body(const std::string &filename) {
     // used for mapping axis angle to certain node.
    struct node_axis {
        body::node *nodeptr = nullptr;    // if nullptr, [position] of entire body.
        int axis;   // 0 for X, 1 for Y, 2 for Z.
    };

    live_body ret;
    std::vector<int> node_id_stack;
    std::vector<body::node *> node_stack;

    int angle_map_index = 0;
    std::map<int, node_axis> angle_map; 

    bool end_flag = false;
    bool motion_section = false;

    std::vector<std::string> lines;
    try {
        lines = read_text_file_multiline(filename);
    } catch(std::invalid_argument &e) {
        std::cerr<<"Invalid file name : "<<e.what()<<std::endl;
        return live_body();
    }

    std::vector<char> delimiters = {'\r', '\t', ' '};
    for(auto it = lines.begin(); it != lines.end(); it++) {
        std::string line = *it;

        std::vector<std::string> words = tokenize(line, delimiters);

        std::string det = *words.begin();
        
        // hierarchy.
        if(!motion_section) {
            if (det == std::string("HIERARCHY"))
                continue;
            else if (det == std::string("ROOT"))
            {
                node_stack.push_back(&ret.get_node(ret.get_root()));
                node_id_stack.push_back(ret.get_root());

                box nb;
                nb.set_xmin(-1); nb.set_xmax(1);
                nb.set_ymin(-1); nb.set_ymax(1);
                nb.set_zmin(-1); nb.set_zmax(1);
                ret.get_node(ret.get_root()).set_bodypart(nb);

                ret.get_node(ret.get_root()).set_name(words.at(1));
            }
            else if (det == std::string("JOINT"))
            {
                body::node n;
                std::shared_ptr<joint_ballsocket> jointptr = std::make_shared<joint_ballsocket>();
                std::shared_ptr<link> linkptr = std::make_shared<link>();
                n.set_name(words.at(1));
                n.set_joint(jointptr);
                n.set_joint_type(1);    // default : ball-and-socket joint.
                n.set_link(linkptr);
                n.set_parent(node_id_stack.back());

                box nb;
                nb.set_xmin(-1); nb.set_xmax(1);
                nb.set_ymin(-1); nb.set_ymax(1);
                nb.set_zmin(-1); nb.set_zmax(1);
                n.set_bodypart(nb);

                int nid = ret.add_node(node_id_stack.back(), n);
                node_stack.push_back(&ret.get_node(nid));
                node_id_stack.push_back(nid);
            }
            else if (det == std::string("{"))
            {
            }
            else if (det == std::string("}"))
            {
                if (end_flag)
                    end_flag = false;
                else
                {
                    node_stack.pop_back();
                    node_id_stack.pop_back();
                }
            }
            else if (det == std::string("End"))
            {
                end_flag = true;
            }
            else if (det == std::string("OFFSET"))
            {
                double x, y, z;
                x = std::stod(words.at(1));
                y = std::stod(words.at(2));
                z = std::stod(words.at(3));
                if (end_flag)
                {
                    // end site offset configuration.
                    auto parent = node_stack.back();
                    box nb = parent->get_bodypart();

                    if (x < nb.get_xmin())
                        nb.set_xmin(x);
                    if (x > nb.get_xmax())
                        nb.set_xmax(x);
                    if (y < nb.get_ymin())
                        nb.set_ymin(y);
                    if (y > nb.get_ymax())
                        nb.set_ymax(y);
                    if (z < nb.get_zmin())
                        nb.set_zmin(z);
                    if (z > nb.get_zmax())
                        nb.set_zmax(z);

                    parent->set_bodypart(nb);
                }
                else
                {
                    node_stack.back()->get_link()->translate(x, y, z);

                    // set box configuration of parent node.
                    if(node_stack.size() > 1) {
                        auto parent = node_stack.at(node_stack.size() - 2);
                        box nb = parent->get_bodypart();

                        if(x < nb.get_xmin()) nb.set_xmin(x);
                        if(x > nb.get_xmax()) nb.set_xmax(x);
                        if(y < nb.get_ymin()) nb.set_ymin(y);
                        if(y > nb.get_ymax()) nb.set_ymax(y);
                        if(z < nb.get_zmin()) nb.set_zmin(z);
                        if(z > nb.get_zmax()) nb.set_zmax(z);

                        parent->set_bodypart(nb);
                    }
                }
            }
            else if (det == std::string("CHANNELS"))
            {
                for (auto word = words.begin(); word != words.end(); word++)
                {
                    node_axis na;
                    if (*word == std::string("Xposition"))
                    {
                        na.nodeptr = nullptr;
                        na.axis = 0;
                        angle_map.insert(std::pair<int, node_axis>(angle_map_index++, na));
                    }
                    else if (*word == std::string("Yposition"))
                    {
                        na.nodeptr = nullptr;
                        na.axis = 1;
                        angle_map.insert(std::pair<int, node_axis>(angle_map_index++, na));
                    }
                    else if (*word == std::string("Zposition"))
                    {
                        na.nodeptr = nullptr;
                        na.axis = 2;
                        angle_map.insert(std::pair<int, node_axis>(angle_map_index++, na));
                    }
                    else if (*word == std::string("Xrotation"))
                    {
                        na.nodeptr = node_stack.back();
                        na.axis = 0;
                        angle_map.insert(std::pair<int, node_axis>(angle_map_index++, na));
                    }
                    else if (*word == std::string("Yrotation"))
                    {
                        na.nodeptr = node_stack.back();
                        na.axis = 1;
                        angle_map.insert(std::pair<int, node_axis>(angle_map_index++, na));
                    }
                    else if (*word == std::string("Zrotation"))
                    {
                        na.nodeptr = node_stack.back();
                        na.axis = 2;
                        angle_map.insert(std::pair<int, node_axis>(angle_map_index++, na));
                    }
                }
            }
            else if(det == std::string("MOTION"))
                motion_section = true;
        }
        // motion.
        else {
            if(det == std::string("Frame")) {   // frame time
                double frame_time = std::stod(words.at(2));
                ret.set_frame_time(frame_time);
            }
            else if(det == std::string("Frames")) {
                // frame number.
            }
            else if(words.size() == angle_map.size()) { // motion data.
                int size = (int)words.size();
                vec3d anim_pos;
                for(int i = 0; i < size;) {
                    double val0 = std::stod(words.at(i));
                    double val1 = std::stod(words.at(i + 1));
                    double val2 = std::stod(words.at(i + 2));
                    const auto na = angle_map.at(i);
                    if(na.nodeptr == nullptr) { // position.
                        vec3d anim_pos(val0, val1, val2);
                        ret.get_anim_pos().push_back(anim_pos);
                    }
                    else {  // rotation.
                        // change to radian angle.
                        val0 = (val0 / 180.0) * 3.141592;
                        val1 = (val1 / 180.0) * 3.141592;
                        val2 = (val2 / 180.0) * 3.141592;

                        transformation jointTR;

                        if(angle_map.at(i + 2).axis == 0) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(1, 0, 0), val2);
                        } 
                        else if(angle_map.at(i + 2).axis == 1) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(0, 1, 0), val2);
                        }
                        else if(angle_map.at(i + 2).axis == 2) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(0, 0, 1), val2);
                        }

                        if(angle_map.at(i + 1).axis == 0) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(1, 0, 0), val1);
                        } 
                        else if(angle_map.at(i + 1).axis == 1) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(0, 1, 0), val1);
                        }
                        else if(angle_map.at(i + 1).axis == 2) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(0, 0, 1), val1);
                        }

                        if(angle_map.at(i).axis == 0) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(1, 0, 0), val0);
                        } 
                        else if(angle_map.at(i).axis == 1) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(0, 1, 0), val0);
                        }
                        else if(angle_map.at(i).axis == 2) {
                            jointTR.rotate(vec3d(0, 0, 0), vec3d(0, 0, 1), val0);
                        }
                        na.nodeptr->get_anim().push_back(jointTR);
                    }
                    i = i + 3;
                }
            }
        }
    }
    std::cout<<"Anim size : "<<ret.get_node_c(1).get_anim_c().size()<<std::endl;
    return ret;
}