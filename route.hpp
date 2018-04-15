#pragma once

struct xy32;

namespace ss {
    class route {
    public:
        typedef std::pair<float, float> fxfy;
        typedef std::pair<fxfy, fxfy> fxfyvxvy;

        route(const std::vector<xy32>& waypoints);
        void set_velocity(float v) { velocity = v; }
        void update(float delta_time);
        fxfyvxvy get_pos(bool& finished) const;
        float get_left() const;
        void reverse();
    private:
        std::vector<float> accum_distance;
        std::vector<xy32> waypoints;
        float velocity;
        float param;
    };
}
