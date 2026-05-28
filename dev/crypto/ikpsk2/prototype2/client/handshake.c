// HWU MSc project
/*
*	NFSv4 Data-In-Flight Encryption over Noise protocol Framework 
*
*	Axel Biegalski
*/
#include <stdio.h>
#include <string.h>
#include <sodium.h>
#include "noise_crypto.h"

void load_conf(const char *filename, uint8_t local_priv[NOISE_PUBLIC_KEY_LEN], uint8_t remote_pub[NOISE_PUBLIC_KEY_LEN], uint8_t psk[NOISE_SYMMETRIC_KEY_LEN]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("[x] failed openning file");
        return;
    }
	printf("conf file opened.\n");
    char line[256], key[64], val[128];
    size_t len;

    while (fgets(line, sizeof(line), fp)) {
        // CORRECTION DU SSCANF : On isole proprement la clé sans récupérer l'espace avant le '='
        if (sscanf(line, " %63[^= ] = %127s", key, val) == 2) {
            
            // Nettoyage des caractères de fin de ligne et du padding '=' s'il est tronqué
            val[strcspn(val, "\r\n")] = 0;

            if (strcasecmp(key, "PrivateKey") == 0) {
                if (sodium_base642bin(local_priv, NOISE_PUBLIC_KEY_LEN, val, strlen(val), NULL, &len, NULL, sodium_base64_VARIANT_ORIGINAL_NO_PADDING) != 0) {
                    sodium_base642bin(local_priv, NOISE_PUBLIC_KEY_LEN, val, strlen(val), NULL, &len, NULL, sodium_base64_VARIANT_ORIGINAL);
                }
                printf("privkey loaded.\n");
            }
            else if (strcasecmp(key, "PublicKey") == 0) {
                if (sodium_base642bin(remote_pub, NOISE_PUBLIC_KEY_LEN, val, strlen(val), NULL, &len, NULL, sodium_base64_VARIANT_ORIGINAL_NO_PADDING) != 0) {
                    sodium_base642bin(remote_pub, NOISE_PUBLIC_KEY_LEN, val, strlen(val), NULL, &len, NULL, sodium_base64_VARIANT_ORIGINAL);
                }
                printf("remote pubkey loaded.\n");
            }
            else if (strcasecmp(key, "PresharedKey") == 0) {
                if (sodium_base642bin(psk, NOISE_SYMMETRIC_KEY_LEN, val, strlen(val), NULL, &len, NULL, sodium_base64_VARIANT_ORIGINAL_NO_PADDING) != 0) {
                    sodium_base642bin(psk, NOISE_SYMMETRIC_KEY_LEN, val, strlen(val), NULL, &len, NULL, sodium_base64_VARIANT_ORIGINAL);
                }
                printf("psk loaded.\n");
            }
        }
    }
    fclose(fp);
}

//INITIATOR -> handshake initiation creation 
void noise_handshake_create_initiation(struct ikpsk2_msg1 *m1, struct noise_handshake *handshake)
{	
	//vars 
	uint8_t timestamp[NOISE_TIMESTAMP_LEN];
	uint8_t key[NOISE_SYMMETRIC_KEY_LEN];
	//init handshake between client and server
	/*
	*	H1 = hash(H0 || Static_pubkey_responder)
	*/
	init_handshake(handshake->chaining_key, handshake->hash_transcript, handshake->remote_static);
	
	/*
		e : C1 & H2
		C1 = hkdf1(C0,E_pub_i)
		H2 = hash(H1, E_pub_i)

	*/
	//generate ephemeral
	generate_ephemeral_secret(handshake->ephemeral_private);
	generate_public(m1->unencrypted_ephemeral, handshake->ephemeral_private);
	//hkdf & hash update
	message_e(m1->unencrypted_ephemeral, handshake->chaining_key, handshake->hash_transcript);
	
	/*
		es: C2 & k1
		C2 || k1 = hkdf2(C1, dh(E_priv_i,S_pub_r))
	*/
	mix_dh(handshake->chaining_key, key , handshake->ephemeral_private, handshake->remote_static);

	/*
		s: timestamps encrypt & H3
		S pub encrypted = aenc(k1,0,S_pub_i,H2)
		H3 = hash(H2, S_pub_encrypted)
	*/
	message_encrypt(m1->encrypted_static, handshake->static_identity->static_public, NOISE_PUBLIC_KEY_LEN ,key, handshake->hash_transcript);
	

	/*
		ss: C3 & K2
		C3 || k2 = hkdf2(C2, dh(S_priv_i, S_pub_r))
		/!\ in the future use pre computed static static instead
	*/
	mix_dh(handshake->chaining_key, key, handshake->static_identity->static_private, handshake->remote_static);

	/*
		encrypte ts & H4
	*/
	get_userspace_timestamp(timestamp);
	message_encrypt(m1->encrypted_timestamp, timestamp, NOISE_TIMESTAMP_LEN, key, handshake->hash_transcript);

	//update state
	/*
		Temp: outsourced in client main function	
	*/
	//handshake->state = HANDSHAKE_CREATED_INITIATION;
}

/*

void handshake_consume_initiation(struct message_handshake_initiation *src, struct wg_device *wg)
{
	struct wg_peer *peer = NULL, *ret_peer = NULL;
	struct noise_handshake *handshake;
	bool replay_attack, flood_attack;
	u8 key[NOISE_SYMMETRIC_KEY_LEN];
	u8 chaining_key[NOISE_HASH_LEN];
	u8 hash[NOISE_HASH_LEN];
	u8 s[NOISE_PUBLIC_KEY_LEN];
	u8 e[NOISE_PUBLIC_KEY_LEN];
	u8 t[NOISE_TIMESTAMP_LEN];
	u64 initiation_consumption;

}

*/

/*

bool handshake_create_response(struct message_handshake_response *dst,	struct noise_handshake *handshake)
{
	u8 key[NOISE_SYMMETRIC_KEY_LEN];
	bool ret = false;

}

struct wg_peer * wg_noise_handshake_consume_response(struct message_handshake_response *src, struct wg_device *wg)
{
	enum noise_handshake_state state = HANDSHAKE_ZEROED;
	struct wg_peer *peer = NULL, *ret_peer = NULL;
	struct noise_handshake *handshake;
	u8 key[NOISE_SYMMETRIC_KEY_LEN];
	u8 hash[NOISE_HASH_LEN];
	u8 chaining_key[NOISE_HASH_LEN];
	u8 e[NOISE_PUBLIC_KEY_LEN];
	u8 ephemeral_private[NOISE_PUBLIC_KEY_LEN];
	u8 static_private[NOISE_PUBLIC_KEY_LEN];
	u8 preshared_key[NOISE_SYMMETRIC_KEY_LEN];

}

bool wg_noise_handshake_begin_session(struct noise_handshake *handshake,
				      struct noise_keypairs *keypairs)
{
	struct noise_keypair *new_keypair;
	bool ret = false;
}

*/