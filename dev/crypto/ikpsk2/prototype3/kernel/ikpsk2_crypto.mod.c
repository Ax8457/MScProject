#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

SYMBOL_CRC(noise_handshake_create_initiation, 0xa467db1f, "");
SYMBOL_CRC(handshake_consume_initiation, 0xe1a29ec0, "");
SYMBOL_CRC(handshake_create_response, 0x004d02bf, "");
SYMBOL_CRC(handshake_consume_response, 0x9345c8a6, "");
SYMBOL_CRC(begin_session, 0x39c12ac2, "");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xadae6df8, "blake2s_final" },
	{ 0x4a5a8811, "curve25519_null_point" },
	{ 0xa3cefaa0, "blake2s_update" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x92997ed8, "_printk" },
	{ 0x147c3f2e, "chacha20poly1305_encrypt" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x9ec6ca96, "ktime_get_real_ts64" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x5a44f8cb, "__crypto_memneq" },
	{ 0xc832c670, "curve25519_arch" },
	{ 0xc20134e7, "chacha20poly1305_decrypt" },
	{ 0x3c74a43e, "curve25519_base_arch" },
	{ 0x41ed3709, "get_random_bytes" },
	{ 0x742578a5, "wait_for_random_bytes" },
	{ 0x160c03af, "module_layout" },
};

MODULE_INFO(depends, "libcurve25519-generic,libchacha20poly1305,curve25519-x86_64");


MODULE_INFO(srcversion, "B2979BDD75FE7D2B549E2E8");
