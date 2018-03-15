#pragma once
#include "sea-object.hpp"

namespace ss {
    namespace bg = boost::geometry;
    namespace bgi = boost::geometry::index;

    class sea {
        typedef bg::model::point<float, 2, bg::cs::cartesian> point;
        typedef bg::model::box<point> box;
        typedef std::pair<box, int> value;

    public:
        void populate_test();
        int spawn(int type, float x, float y, float w, float h);
        int spawn(const char* guid, int type, float x, float y, float w, float h);
        void teleport_to(const char* guid, float x, float y);
        void query_near_to_packet(float xc, float yc, float ex, std::vector<sea_object_public>& sop_list);

    private:
        std::vector<int> query_tree(float xc, float yc, float ex) const;

        std::unordered_map<int, sea_object> sea_objects;
        std::unordered_map<std::string, int> sea_guid_to_id;
        bgi::rtree< value, bgi::quadratic<16> > rtree;
    };
}
