#include "noise_crypto.h"

static void wg_packet_send_handshake_initiation(struct noise_peer *peer)
{
	struct ikpsk2_msg1 packet;


	if (handshake_create_initiation(&packet, &peer->handshake)) {
		wg_cookie_add_mac_to_packet(&packet, sizeof(packet), peer);
		wg_timers_any_authenticated_packet_traversal(peer);
		wg_timers_any_authenticated_packet_sent(peer);
		atomic64_set(&peer->last_sent_handshake,
			     ktime_get_coarse_boottime_ns());
		wg_socket_send_buffer_to_peer(peer, &packet, sizeof(packet),
					      HANDSHAKE_DSCP);
		wg_timers_handshake_initiated(peer);
	}
}