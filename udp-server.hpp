#pragma once

namespace ss {
    namespace bg = boost::geometry;
    namespace bgi = boost::geometry::index;
    using boost::asio::ip::udp;
    class sea;
    class sea_static;
    class seaport;
    class route;
    class region;

    class udp_server {

    public:
        udp_server(boost::asio::io_service& io_service,
                   std::shared_ptr<sea> sea,
                   std::shared_ptr<sea_static> sea_static,
                   std::shared_ptr<seaport> seaport,
                   std::shared_ptr<region> region);
        bool set_route(int id, int seaport_id1, int seaport_id2);
    private:
        void make_test_route();
        void update();
        void start_receive();
        void send_full_state(float lng, float lat, float ex_lng, float ex_lat, int view_scale);
        void send_land_cell(float lng, float lat, float ex_lng, float ex_lat, int view_scale);
        void send_land_cell_aligned(int xc0_aligned, int yc0_aligned, float ex_lng, float ex_lat, int view_scale);
        void send_seaport(float lng, float lat, float ex_lng, float ex_lat, int view_scale);
        void send_seaport_cell_aligned(int xc0_aligned, int yc0_aligned, float ex_lng, float ex_lat, int view_scale);
        void send_track_object_coords(int track_object_id, int track_object_ship_id);
        void send_seaarea(float lng, float lat);
        void send_waypoints(int ship_id);
        std::shared_ptr<const route> find_route_map_by_ship_id(int ship_id) const;

        // How to test handle_receive():
        // $ perl -e "print pack('ff',10.123,20.456)" > /dev/udp/127.0.0.1/3100

        void handle_receive(const boost::system::error_code& error,
                            std::size_t bytes_transferred);

        void handle_send(const boost::system::error_code& error,
                         std::size_t bytes_transferred);

        std::shared_ptr<route> create_route_id(const std::vector<int>& seaport_id_list) const;
        std::shared_ptr<route> create_route(const std::vector<std::string>& seaport_list) const;

        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        boost::array<char, 1024> recv_buffer_;
        boost::asio::deadline_timer timer_;
        std::shared_ptr<sea> sea_;
        std::shared_ptr<sea_static> sea_static_;
        std::shared_ptr<seaport> seaport_;
        std::shared_ptr<region> region_;
        std::unordered_map<int, std::shared_ptr<route> > route_map_; // id -> route
        int tick_seq_;
    };
}
