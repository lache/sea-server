#pragma once

#include "container.hpp"

namespace ss {
    namespace bg = boost::geometry;
    namespace bgi = boost::geometry::index;

    struct sea_object_public {
        int id;
        int type;
        float fx, fy;
        float fw, fh;
        float fvx, fvy;
        char guid[64];
    };

    enum SEA_OBJECT_STATE {
        SOS_SAILING,
        SOS_LOADING,
        SOS_UNLOADING,
        SOS_ERROR,
    };

    class sea_object {
        typedef bg::model::point<float, 2, bg::cs::cartesian> point;
        typedef bg::model::box<point> box;
        typedef std::pair<box, int> value;
    public:
        sea_object(int id, int type, float fx, float fy, float fw, float fh, const value& rtree_value)
            : id(id),
            type(type),
            fx(fx),
            fy(fy),
            fw(fw),
            fh(fh),
            fvx(0),
            fvy(0),
            rtree_value(rtree_value),
            state(SOS_SAILING),
            remain_loading_time(0) {
        }
        void fill_sop(sea_object_public& sop) const {
            sop.fx = fx;
            sop.fy = fy;
            sop.fw = fw;
            sop.fh = fh;
            sop.fvx = fvx;
            sop.fvy = fvy;
            sop.id = id;
            sop.type = type;
            strcpy(sop.guid, guid.c_str());
            if (state == SOS_LOADING) {
                strcat(sop.guid, "[LOADING]");
            } else if (state == SOS_UNLOADING) {
                strcat(sop.guid, "[UNLOADING]");
            }
        }
        void set_guid(const std::string& v) {
            guid = v;
        }
        void translate_xy(float dxv, float dyv) {
            set_xy(fx + dxv, fy + dyv);
        }
        void get_xy(float& fx, float& fy) const {
            fx = this->fx;
            fy = this->fy;
        }
        void set_xy(float fx, float fy) {
            this->fx = fx;
            this->fy = fy;
            rtree_value.first.min_corner().set<0>(fx);
            rtree_value.first.min_corner().set<1>(fy);
            rtree_value.first.max_corner().set<0>(fx + fw);
            rtree_value.first.max_corner().set<1>(fy + fh);
        }
        void set_velocity(float fvx, float fvy) {
            this->fvx = fvx;
            this->fvy = fvy;
        }
        void set_destination(float fvx, float fvy) {
            this->dest_fx = fvx;
            this->dest_fy = fvy;
        }
        void get_velocity(float& fvx, float& fvy) const {
            fvx = this->fvx;
            fvy = this->fvy;
        }
        float get_distance_to_destination() const {
            return sqrtf((dest_fx - fx) * (dest_fx - fx) + (dest_fy - fy) * (dest_fy - fy));
        }
        const value& get_rtree_value() const { return rtree_value; }
        void set_state(SEA_OBJECT_STATE state) { this->state = state; }
        SEA_OBJECT_STATE get_state() const { return state; }
        void set_remain_loading_time(float remain_loading_time) {
            this->remain_loading_time = remain_loading_time;
        }
        void update(float delta_time) {
            if (remain_loading_time > 0) {
                remain_loading_time -= delta_time;
                if (remain_loading_time <= 0) {
                    remain_loading_time = 0;
                    state = SOS_SAILING;
                }
            }
        }
        int get_type() const { return type; }
        int get_id() const { return id; }
    private:
        explicit sea_object() {}
        int id;
        int type;
        float fx, fy;
        float fw, fh;
        float fvx, fvy;
        float dest_fx, dest_fy;
        std::string guid;
        value rtree_value;
        SEA_OBJECT_STATE state;
        float remain_loading_time;
        std::vector<container> containers;
    };
}
