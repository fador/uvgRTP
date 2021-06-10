#include "hello_ack.hh"

#include "zrtp_receiver.hh"

#include "../zrtp.hh"
#include "crypto.hh"
#include "frame.hh"
#include "socket.hh"
#include "debug.hh"

#include <cstring>

#define ZRTP_HELLO_ACK "HelloACK"

uvgrtp::zrtp_msg::hello_ack::hello_ack()
{
    allocate_frame(sizeof(zrtp_hello_ack));
    zrtp_hello_ack *msg = (zrtp_hello_ack *)frame_;
    set_zrtp_start_base(msg->msg_start, ZRTP_HELLO_ACK);

    msg->crc = uvgrtp::crypto::crc32::calculate_crc32((uint8_t *)frame_, len_ - sizeof(uint32_t));
}

uvgrtp::zrtp_msg::hello_ack::~hello_ack()
{
    LOG_DEBUG("Freeing ZRTP Hello ACK message...");
    (void)uvgrtp::frame::dealloc_frame(frame_);
}

rtp_error_t uvgrtp::zrtp_msg::hello_ack::send_msg(uvgrtp::socket *socket, sockaddr_in& addr)
{
    rtp_error_t ret;

    if ((ret = socket->sendto(addr, (uint8_t *)frame_, len_, 0, nullptr)) != RTP_OK)
        log_platform_error("Failed to send ZRTP Hello message");

    return ret;
}

rtp_error_t uvgrtp::zrtp_msg::hello_ack::parse_msg(uvgrtp::zrtp_msg::receiver& receiver,
    zrtp_session_t& session)
{
    (void)receiver;

    return RTP_OK;
}
