#pragma once

#include <unordered_map>

#include "runner.hh"
#include "socket.hh"
#include "util.hh"

namespace uvg_rtp {

    typedef rtp_error_t (*packet_handler)(ssize_t, void *);

    class pkt_dispatcher : public runner {
        public:
            pkt_dispatcher(uvg_rtp::socket socket);
            ~pkt_dispatcher();

            /* Install a generic handler for an incoming packet
             *
             * Return RTP_OK on successfully
             * Return RTP_INVALID_VALUE if "handler" is nullptr */
            rtp_error_t install_handler(packet_handler handler);

            /* Install receive hook for the RTP packet dispatcher
             *
             * Return RTP_OK on success
             * Return RTP_INVALID_VALUE if "hook" is nullptr */
            rtp_error_t install_receive_hook(void *arg, void (*hook)(void *, uvg_rtp::frame::rtp_frame *));

            /* Return reference to the vector that holds all installed handlers */
            std::vector<uvg_rtp::packet_handler>& get_handlers();

        private:
            static void runner(uvg_rtp::pkt_dispatcher *dispatcher, uvg_rtp::socket& socket);

            uvg_rtp::socket socket_;
            std::vector<packet_handler> packet_handlers_;

            void *recv_hook_arg_;
            void (*recv_hook_)(void *arg, uvg_rtp::frame::rtp_frame *frame);
    };
}
